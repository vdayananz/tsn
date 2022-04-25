/******************************************************************************

  Copyright (c) 2016, Xilinx Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   3. Neither the name of the Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../../lib/avtp_pipeline/rawsock/openavb_rawsock.h"

//Common usage: ./tsn_listener -i eth0 -t 8944 -r 8000 -s 1 -c 1 -m 1 -l 100

#define MAX_NUM_FRAMES 100
#define NANOSECONDS_PER_SECOND		(1000000000ULL)
#define TIMESPEC_TO_NSEC(ts) (((uint64_t)ts.tv_sec * (uint64_t)NANOSECONDS_PER_SECOND) + (uint64_t)ts.tv_nsec)

#define RAWSOCK_TX_MODE_FILL (0)
#define RAWSOCK_TX_MODE_SEQ  (1)

static char* interface = "eth1";
//static int ethertype = 33024;
static int ethertype = 2048;
static int txlen = 1500;
static int txRate = 8000;
static int chunkSize = 1;
static int reportSec = 1;
static int mode = RAWSOCK_TX_MODE_FILL;

unsigned char src_mac[6]  = { 0x00, 0x0A, 0x35, 0x00, 0x01, 0x0e };
unsigned char dest_mac[6] = { 0xa0, 0x36, 0x9f, 0x68, 0x4c, 0x96 };

#if 0
unsigned char src_mac[6] = {0x00, 0x0A, 0x35, 0x00, 0x02, 0x90 };
unsigned char dest_mac[6] = {0x00, 0x0A, 0x35, 0x00, 0x02, 0x91 };
#endif

void dumpAscii(U8 *pFrame, int i, int *j)
{
	char c;

	printf("  ");
	
	while (*j <= i) {
		c = pFrame[*j];
		*j += 1;
		if (!isprint(c) || isspace(c))
			c = '.';
		printf("%c", c);
	}
}

void dumpFrameContent(U8 *pFrame, U32 len)
{
	int i = 0, j = 0;
	while (TRUE) {
		if (i % 16 == 0) {
			if (i != 0 ) {
				// end of line stuff
				dumpAscii(pFrame, (i < len ? i : len), &j);
				printf("\n");

				if (i >= len)
					break;
			}
			if (i+1 < len) {
				// start of line stuff
				printf("0x%4.4d:  ", i);
			}
		}
		else if (i % 2 == 0) {
			printf("  ");
		}

		if (i >= len)
			printf("  ");
		else
			printf("%2.2x", pFrame[i]);

		i += 1;
	}
}

void dumpFrame(U8 *pFrame, U32 len, hdr_info_t *hdr)
{
	printf("Frame received, ethertype=0x%x len=%u\n", hdr->ethertype, len);
	printf("src: %s\n", ether_ntoa((const struct ether_addr*)hdr->shost));
	printf("dst: %s\n", ether_ntoa((const struct ether_addr*)hdr->dhost));
	if (hdr->vlan) {
		printf("VLAN pcp=%u, vid=%u\n", (unsigned)hdr->vlan_pcp, hdr->vlan_vid); 
	}
	dumpFrameContent(pFrame, len);
	printf("\n");
}

int main(int argc, char* argv[])
{

	if (interface == NULL || ethertype < 0) {
		printf("error: must specify network interface and ethertype\n");
		exit(2);
	}

	void* rs = openavbRawsockOpen(interface, FALSE, TRUE, ethertype, 0, MAX_NUM_FRAMES);
	if (!rs) {
		printf("error: failed to open raw socket (are you root?)\n");
		exit(3);
	}

	U8 *pBuf, *pData;
	U32 buflen = 0, hdrlen = 0, datalen;
	hdr_info_t hdr;

	memset(&hdr, 0, sizeof(hdr_info_t));

        hdr.shost = src_mac;
        hdr.dhost = dest_mac;
	hdr.vlan = FALSE;
#if 0
	hdr.vlan_pcp = 4;
	hdr.vlan_vid = 100;
	hdr.vlan_pcp = 4;
	hdr.vlan_vid = 2;
#endif

	openavbRawsockTxSetHdr(rs, &hdr);

	struct timespec now;
	static U64 packetIntervalNSec = 0;
	static U64 nextCycleNSec = 0;
	static U32 packetCnt = 0;
	static U64 nextReportInterval = 0;

	packetIntervalNSec = NANOSECONDS_PER_SECOND / txRate;
	clock_gettime(CLOCK_MONOTONIC, &now);
	nextCycleNSec = TIMESPEC_TO_NSEC(now);
	nextReportInterval = TIMESPEC_TO_NSEC(now) + (NANOSECONDS_PER_SECOND * reportSec);

	while (1) {
		pBuf = (U8*)openavbRawsockGetTxFrame(rs, TRUE, &buflen);
		if (!pBuf) {
			printf("failed to get TX frame buffer\n");
			exit(4);
		}

		if (buflen < txlen || txlen == -1)
			txlen = buflen;

		openavbRawsockTxFillHdr(rs, pBuf, &hdrlen);
		pData = pBuf + hdrlen;
		datalen = txlen - hdrlen;

		if (mode == RAWSOCK_TX_MODE_FILL) {
			int i;
			for (i=0; i<datalen; i++)
				pData[i] = (i & 0xff);
		}
		else {	// RAWSOCK_TX_MODE_sEQ
			static unsigned char seq = 0;
			pData[0] = 0x7F;		// Experimental subtype
			pData[1] = 0x00;
			pData[2] = seq++;
			txlen = hdrlen + 3;
		}

		openavbRawsockTxFrameReady(rs, pBuf, txlen);

		packetCnt++;

		if ((packetCnt % chunkSize) == 0) {
			openavbRawsockSend(rs);
		}

		nextCycleNSec += packetIntervalNSec;
		clock_gettime(CLOCK_MONOTONIC, &now);
		U64 nowNSec = TIMESPEC_TO_NSEC(now);;

		if (nowNSec > nextReportInterval) {
			printf("TX Packets: %d\n", packetCnt);
			packetCnt = 0;
			nextReportInterval = nowNSec + (NANOSECONDS_PER_SECOND * reportSec);
		}

		if (nowNSec < nextCycleNSec) {
			usleep((nextCycleNSec - nowNSec) / 1000);
		}
	}

	openavbRawsockClose(rs);
	return 0;
}
