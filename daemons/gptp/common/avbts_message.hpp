/******************************************************************************

  Copyright (c) 2009-2012, Intel Corporation 
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

#ifndef AVBTS_MESSAGE_HPP
#define AVBTS_MESSAGE_HPP

#include <stdint.h>
#include <avbts_osnet.hpp>
#include <ieee1588.hpp>

#include <list>
#include <algorithm>

/** @file **/

#define PTP_CODE_STRING_LENGTH 4		/*!< PTP code string length in bytes */
#define PTP_SUBDOMAIN_NAME_LENGTH 16	/*!< PTP subdomain name lenght in bytes */
#define PTP_FLAGS_LENGTH 2				/*!< PTP flags length in bytes */

#define GPTP_VERSION 2			/*!< GPTP version */
#define PTP_NETWORK_VERSION 1	/*!< PTP Network version */

#define PTP_ETHER 1		/*!< @todo Not used */
#define PTP_DEFAULT 255	/*!< @todo Not used */

#define PTP_COMMON_HDR_OFFSET 0		/*!< PTP common header offset */
#define PTP_COMMON_HDR_LENGTH 34	/*!< PTP common header length in bytes */
#define PTP_COMMON_HDR_TRANSSPEC_MSGTYPE(x) x	/*!< Gets the message type offset on PTP header */
#define PTP_COMMON_HDR_PTP_VERSION(x) x+1		/*!< Gets the ptp version offset on PTP header */
#define PTP_COMMON_HDR_MSG_LENGTH(x) x+2		/*!< Gets the message length offset on PTP header */
#define PTP_COMMON_HDR_DOMAIN_NUMBER(x) x+4		/*!< Gets the domain number offset on PTP header */
#define PTP_COMMON_HDR_FLAGS(x) x+6				/*!< Gets the flags offset on PTP header */
#define PTP_COMMON_HDR_CORRECTION(x) x+8		/*!< Gets the correction field offset on PTP header */
#define PTP_COMMON_HDR_SOURCE_CLOCK_ID(x) x+20	/*!< Gets the source clock id offset on PTP header */
#define PTP_COMMON_HDR_SOURCE_PORT_ID(x) x+28	/*!< Gets the source port id offset on PTP header */
#define PTP_COMMON_HDR_SEQUENCE_ID(x) x+30		/*!< Gets the sequence id offset on PTP header */
#define PTP_COMMON_HDR_CONTROL(x) x+32			/*!< Gets the control offset on PTP header */
#define PTP_COMMON_HDR_LOG_MSG_INTRVL(x) x+33	/*!< Gets the log message interval offset on PTP header */

#define PTP_ANNOUNCE_OFFSET 34							/*!< PTP announce offset */
#define PTP_ANNOUNCE_LENGTH 30							/*!< PTP announce length in bytes */
#define PTP_ANNOUNCE_CURRENT_UTC_OFFSET(x) x+10			/*!< Gets PTP announce current UTC offset */
#define PTP_ANNOUNCE_GRANDMASTER_PRIORITY1(x) x+13		/*!< Gets Grandmaster priority1 offset on announce fields */
#define PTP_ANNOUNCE_GRANDMASTER_CLOCK_QUALITY(x) x+14	/*!< Gets Grandmaster clock quality offset on announce fields */
#define PTP_ANNOUNCE_GRANDMASTER_PRIORITY2(x) x+18		/*!< Gets Grandmasdter priority2 offset on announce fields*/
#define PTP_ANNOUNCE_GRANDMASTER_IDENTITY(x) x+19		/*!< Gets Grandmaster identity offset on announce fields*/
#define PTP_ANNOUNCE_STEPS_REMOVED(x) x+27				/*!< Gets steps removed offset on announce fields*/
#define PTP_ANNOUNCE_TIME_SOURCE(x) x+29				/*!< Gets time source offset on announce fields*/

#define PTP_SYNC_OFFSET 34		/*!< PTP SYNC base offset */
#define PTP_SYNC_LENGTH 10		/*!< PTP SYNC length in bytes */
#define PTP_SYNC_SEC_MS(x) x	/*!< PTP SYNC seconds MSB offset */
#define PTP_SYNC_SEC_LS(x) x+2	/*!< PTP SYNC seconds LSB offset */
#define PTP_SYNC_NSEC(x) x+6	/*!< PTP SYNC nanoseconds offset */

#define PTP_FOLLOWUP_OFFSET 34		/*!< PTP FOLLOWUP base offset */
#define PTP_FOLLOWUP_LENGTH 10		/*!< PTP FOLLOWUP length in bytes */
#define PTP_FOLLOWUP_SEC_MS(x) x	/*!< Gets the followup seconds MSB offset */
#define PTP_FOLLOWUP_SEC_LS(x) x+2	/*!< Gets the followup seconds LSB offset */
#define PTP_FOLLOWUP_NSEC(x) x+6	/*!< Gets tne followup nanoseconds offset */

#define PTP_PDELAY_REQ_OFFSET 34		/*!< PTP PDELAY REQUEST base offset */
#define PTP_PDELAY_REQ_LENGTH 20		/*!< PTP PDELAY REQUEST length in bytes */
#define PTP_PDELAY_REQ_SEC_MS(x) x		/*!< Gets the pdelay request seconds MSB offset */
#define PTP_PDELAY_REQ_SEC_LS(x) x+2	/*!< Gets the pdelay request seconds LSB offset */
#define PTP_PDELAY_REQ_NSEC(x) x+6		/*!< Gets the pdelay request nanoseconds offset */

#define PTP_PDELAY_RESP_OFFSET 34				/*!< PDELAY RESPONSE base offset */
#define PTP_PDELAY_RESP_LENGTH 20				/*!< PDELAY RESPONSE length in bytes */
#define PTP_PDELAY_RESP_SEC_MS(x) x				/*!< Gets the pdelay response seconds MSB offset */
#define PTP_PDELAY_RESP_SEC_LS(x) x+2			/*!< Gets the pdelay response seconds LSB offset */
#define PTP_PDELAY_RESP_NSEC(x) x+6				/*!< Gets the pdelay nanoseconds offset */
#define PTP_PDELAY_RESP_REQ_CLOCK_ID(x) x+10	/*!< Gets the pdelay response request clock id offset */
#define PTP_PDELAY_RESP_REQ_PORT_ID(x) x+18		/*!< Gets the pdelay response request port id offset */

#define PTP_PDELAY_FOLLOWUP_OFFSET 34				/*!< PTP PDELAY FOLLOWUP base offset*/
#define PTP_PDELAY_FOLLOWUP_LENGTH 20				/*!< PTP PDELAY FOLLOWUP length in bytes */
#define PTP_PDELAY_FOLLOWUP_SEC_MS(x) x				/*!< Gets the pdelay followup seconds MSB offset*/
#define PTP_PDELAY_FOLLOWUP_SEC_LS(x) x+2			/*!< Gets the pdelay followup seconds LSB offset*/
#define PTP_PDELAY_FOLLOWUP_NSEC(x) x+6				/*!< Gets the pdelay followup nanoseconds offset*/
#define PTP_PDELAY_FOLLOWUP_REQ_CLOCK_ID(x) x+10	/*!< Gets the pdelay followup request clock id offset*/
#define PTP_PDELAY_FOLLOWUP_REQ_PORT_ID(x) x+18		/*!< Gets the pdelay followup request port id offset*/

#define PTP_LI_61_BYTE 1		/*!< PTP_LI_61(leap61) byte offset on flags field */
#define PTP_LI_61_BIT 0			/*!< PTP_LI_61(leap61) bit offset on PTP_LI_61 byte*/
#define PTP_LI_59_BYTE 1		/*!< PTP_LI_59(leap59) byte offset on flags field*/
#define PTP_LI_59_BIT 1			/*!< PTP_LI_59(leap59) bit offset on PTP_LI_59 byte*/
#define PTP_ASSIST_BYTE 0		/*!< PTP_ASSIST(two step flag) byte offset on flags field*/
#define PTP_ASSIST_BIT 1		/*!< PTP_ASSIST(two step flag) bit on PTP_ASSIST byte*/
#define PTP_PTPTIMESCALE_BYTE 1	/*!< PTPTIMESCALE byte offset on flags field*/
#define PTP_PTPTIMESCALE_BIT 3	/*!< PTPTIMESCAPE bit offset on PTPTIMESCALE byte*/

#define TX_TIMEOUT_BASE 1000 	/*!< Timeout base in microseconds */
#define TX_TIMEOUT_ITER 6		/*!< Number of timeout iteractions for sending/receiving messages*/

/**
 * Enumeration message type. IEEE 1588-2008 Clause 13.3.2.2
 */
enum MessageType {
	SYNC_MESSAGE = 0,
	DELAY_REQ_MESSAGE = 1,
	PATH_DELAY_REQ_MESSAGE = 2,
	PATH_DELAY_RESP_MESSAGE = 3,
	FOLLOWUP_MESSAGE = 8,
	DELAY_RESP_MESSAGE = 9,
	PATH_DELAY_FOLLOWUP_MESSAGE = 0xA,
	ANNOUNCE_MESSAGE = 0xB,
	SIGNALLING_MESSAGE = 0xC,
	MANAGEMENT_MESSAGE = 0xD,
};

/**
 * Enumeration legacy message type
 */
enum LegacyMessageType {
	SYNC,
	DELAY_REQ,
	FOLLOWUP,
	DELAY_RESP,
	MANAGEMENT,
	MESSAGE_OTHER
};

/**
 * Enumeration multicast type.
 */
enum MulticastType {
	MCAST_NONE,
	MCAST_PDELAY,
	MCAST_OTHER
};

/**
 * Provides the PTPMessage common interface used during building of
 * PTP messages.
 */
class PTPMessageCommon {
protected:
	unsigned char versionPTP;	/*!< PTP version */
	uint16_t versionNetwork;	/*!< Network version */
	MessageType messageType;	/*!< MessageType to be built */
	
	PortIdentity *sourcePortIdentity;	/*!< PortIdentity from source*/
	
	uint16_t sequenceId;		/*!< PTP message sequence ID*/
	LegacyMessageType control;	/*!< Control message type of LegacyMessageType */
	unsigned char flags[2];		/*!< PTP flags field */
	
	uint16_t messageLength;			/*!< PTP message length */
	char logMeanMessageInterval;	/*!< LogMessageInterval (IEEE 1588-2008 table 24)*/
	long long correctionField;		/*!< Correction Field (IEEE 1588-2008 table 21) */
	unsigned char domainNumber;		/*!< PTP domain number */
	
	Timestamp _timestamp;	/*!< PTP message timestamp */
	unsigned _timestamp_counter_value;	/*!< PTP timestamp counter value */
	bool _gc;	/*!< Garbage collection flag */
	
	/**
	 * Default constructor
	 */
	PTPMessageCommon(void) { };
 public:
	/**
	 * @brief Creates the PTPMessageCommon interface
	 * @param port IEEE1588Port where the message interface is connected to.
	 */
	PTPMessageCommon(IEEE1588Port * port);
	/**
	 * Destroys PTPMessageCommon interface
	 */
	virtual ~PTPMessageCommon(void);

	/**
	 * @brief  Gets a pointer to the flags field within the PTP message.
	 * @return Pointer to the flags field
	 */
	unsigned char *getFlags(void) {
		return flags;
	}

	/**
	 * @brief  Gets the sequenceId value within a ptp message
	 * @return Sequence ID value
	 */
	uint16_t getSequenceId(void) {
		return sequenceId;
	}
	/**
	 * @brief  Sets the sequence ID value to the PTP message.
	 * @param  seq Sequence id value to be set.
	 * @return void
	 */
	void setSequenceId(uint16_t seq) {
		sequenceId = seq;
	}

	/**
	 * @brief  Gets the MessageType field within the PTP message.
	 * @return MessageType
	 */
	MessageType getMessageType(void) {
		return messageType;
	}

	/**
	 * @brief  Gets the correctionField value in a Little-Endian format.
	 * @return correctionField
	 * @todo Little endian format could be removed by adding endianess discovery on
	 * compile/run time.
	 */
	long long getCorrectionField(void) {
		return correctionField;
	}
	/**
	 * @brief  Sets the correction field. It expects the host format.
	 * @param  correctionAmount
	 * @return void
	 * @todo Little endian format could be removed by adding endianess discovery on
	 * compile/run time.
	 */
	void setCorrectionField(long long correctionAmount) {
		correctionField = correctionAmount;
	}

	/**
	 * @brief  Gets PortIdentity field
	 * @param  identity [out] Source port identity
	 * @return void
	 */
	void getPortIdentity(PortIdentity * identity);
	/**
	 * @brief  Sets PortIdentity value
	 * @param  identity [in] Source port identity value to be set.
	 * @return void
	 */
	void setPortIdentity(PortIdentity * identity);

	/**
	 * @brief  Gets the current Timestamp value from the PTP message
	 * @return Current Timestamp value
	 */
	Timestamp getTimestamp(void) {
		return _timestamp;
	}
	/**
	 * @brief  Gets the timestamp counter value set during the RX timestamp method.
	 * @return timestamp counter value
	 */
	uint32_t getTimestampCounterValue(void) {
		return _timestamp_counter_value;;
	}
	/**
	 * @brief  Sets the timestamp value
	 * @param  timestamp [in] Reference to Timestamp value
	 * @return void
	 */
	void setTimestamp(Timestamp & timestamp) {
		_timestamp = timestamp;
	}

	/**
	 * @brief Gets the garbage collection status
	 * @return TRUE when it needs to be clean. FALSE otherwise.
	 */
	bool garbage() {
		return _gc;
	}

	/**
	 * @brief  Determine whether the message was sent by given communication technology, uuid, and
	 * port id fields
	 * @param  portIdentity PortIdentity value
	 * @return TRUE if sender equals to internal PTPCommon values, FALSE otherwise.
	 */
	bool isSenderEqual(PortIdentity portIdentity);

	/**
	 * @brief  Generic interface for processing PTP message
	 * @param  port IEEE1588 port
	 * @return void
	 */
	virtual void processMessage(IEEE1588Port * port);

	/**
	 * @brief  Builds PTP common header
	 * @param  buf [out] PTP message
	 * @return void
	 */
	void buildCommonHeader(uint8_t * buf);

	friend PTPMessageCommon *buildPTPMessage
	(char *buf, int size, LinkLayerAddress * remote, IEEE1588Port * port);
};

/*Exact fit. No padding*/
#pragma pack(push,1)

#define PATH_TRACE_TLV_TYPE 0x8		/*!< This is the value that indicates the
									  TLV is a path trace TLV, as specified in
									  16.2.7.1 and Table 34 of IEEE Std
									  1588-2008. The value is specified there
									  as PATH_TRACE, whose value is 0x8. */

/**
 * Provides the PathTraceTLV interface
 * The fields of the path TLV shall be as specified in Table 10-8 and in
 * 10.5.4.3.2 through 10.5.4.3.9 from IEEE 802.1AS. This TLV,
 * and its use, are defined in IEEE Std 1588-2008 (see 16.2 and Table 34 of IEEE Std 1588-2008).
 */
class PathTraceTLV {
 private:
	uint16_t tlvType;
	typedef std::list<ClockIdentity> IdentityList;
	IdentityList identityList;
 public:
	/**
	 * Creates the PathTraceTLV interface.
	 * Sets tlvType to PATH_TRACE_TLV_TYPE using network byte order
	 */
	PathTraceTLV() {
		tlvType = PLAT_htons(PATH_TRACE_TLV_TYPE);
	}
	/**
	 * @brief  Parses ClockIdentity from message buffer
	 * @param  buffer [in] Message buffer. It should be at least ::PTP_CLOCK_IDENTITY_LENGTH bytes long.
	 * @return void
	 */
	void parseClockIdentity(uint8_t *buffer) {
		int length = PLAT_ntohs(*((uint16_t *)buffer))/PTP_CLOCK_IDENTITY_LENGTH;
		buffer += sizeof(uint16_t);
		for(; length > 0; --length) {
			ClockIdentity add;
			add.set(buffer);
			identityList.push_back(add);
			buffer += PTP_CLOCK_IDENTITY_LENGTH;
		}
	}

	/**
	 * @brief  Appends new ClockIdentity to internal ClockIdentity list
	 * @param  id ClockIdentity to be appended
	 * @return void
	 */
	void appendClockIdentity(ClockIdentity * id) {
		identityList.push_back(*id);
	}

	/**
	 * @brief  Gets TLV value in a byte string format
	 * @param  byte_str [out] Output byte string
	 * @return void
	 */
	void toByteString(uint8_t * byte_str) {
		IdentityList::iterator iter;
		*((uint16_t *)byte_str) = tlvType;  // tlvType already in network byte order
		byte_str += sizeof(tlvType);
		*((uint16_t *)byte_str) =
			PLAT_htons(identityList.size()*PTP_CLOCK_IDENTITY_LENGTH);
		byte_str += sizeof(uint16_t);
		for
			(iter = identityList.begin();
			 iter != identityList.end(); ++iter) {
			iter->getIdentityString(byte_str);
			byte_str += PTP_CLOCK_IDENTITY_LENGTH;
		}
	}

	/**
	 * @brief  Looks for a specific ClockIdentity on the current TLV
	 * @param  id [in] Desired ClockIdentity
	 * @return TRUE if it has found it, FALSE otherwise.
	 */
	bool has(ClockIdentity *id) {
		return std::find
			(identityList.begin(), identityList.end(), *id) !=
			identityList.end();
	}

	/**
	 * @brief  Gets the total length of TLV.
	 * Total length of TLV is length of type field (UINT16) + length of 'length'
	 * field (UINT16) + length of
	 * identities (each PTP_CLOCK_IDENTITY_LENGTH) in the path
	 * @return Total length
	 */
	int length() {
		return 2*sizeof(uint16_t) + PTP_CLOCK_IDENTITY_LENGTH*identityList.size();
	}
};

/* back to whatever the previous packing mode was */
#pragma pack(pop)

/**
 * Provides the PTPMessageAnnounce interface
 * The PTPMessageAnnounce class is used to create
 * announce messages on the 802.1AS format when building
 * the ptp messages.
 */
class PTPMessageAnnounce:public PTPMessageCommon {
 private:
	uint8_t grandmasterIdentity[PTP_CLOCK_IDENTITY_LENGTH];
	ClockQuality *grandmasterClockQuality;

	PathTraceTLV tlv;

	uint16_t currentUtcOffset;
	unsigned char grandmasterPriority1;
	unsigned char grandmasterPriority2;
	ClockQuality *clockQuality;
	uint16_t stepsRemoved;
	unsigned char timeSource;

	 PTPMessageAnnounce(void);
 public:
	 /**
	  * Creates the PTPMessageAnnounce interface
	  */
	 PTPMessageAnnounce(IEEE1588Port * port);

	 /**
	  * Destroys the PTPMessageAnnounce interface
	  */
	~PTPMessageAnnounce();

	/**
	 * @brief  Compare gramdmaster's capabilities comming on the
	 * announce messages against the current grandmaster capabilities.
	 * @param  msg [in] PTPMessageAnnounce to be compared
	 * @return TRUE if it is better. FALSE otherwise.
	 */
	bool isBetterThan(PTPMessageAnnounce * msg);

	/**
	 * @brief  Gets grandmaster's priority1 value
	 * @return Grandmaster priority1
	 */
	unsigned char getGrandmasterPriority1(void) {
		return grandmasterPriority1;
	}

	/**
	 * @brief  Gets grandmaster's priority2 value
	 * @return Grandmaster priority2
	 */
	unsigned char getGrandmasterPriority2(void) {
		return grandmasterPriority2;
	}

	/**
	 * @brief  Gets grandmaster clock quality
	 * @return Pointer to a ClockQuality object.
	 */
	ClockQuality *getGrandmasterClockQuality(void) {
		return grandmasterClockQuality;
	}

	/**
	 * @brief  Gets the steps removed value. See IEEE 802.1AS clause 10.3.3
	 * @return steps removed value
	 */
	uint16_t getStepsRemoved(void) {
		return stepsRemoved;
	}

	/**
	 * @brief Gets grandmaster identity value
	 * @param  identity [out] Grandmaster identity
	 * @return void
	 */
	void getGrandmasterIdentity(char *identity) {
		memcpy(identity, grandmasterIdentity, PTP_CLOCK_IDENTITY_LENGTH);
	}

	/**
	 * @brief  Gets grandmaster's clockIdentity value
	 * @return Grandmaster ClockIdentity
	 */
	ClockIdentity getGrandmasterClockIdentity() {
		ClockIdentity ret;
		ret.set( grandmasterIdentity );
		return ret;
	}

	/**
	 * @brief  Processes PTP message
	 * @param  port IEEE1588Port
	 * @return void
	 */
	void processMessage(IEEE1588Port * port);

	/**
	 * @brief  Assembles PTPMessageAnnounce message on the IEEE1588Port payload
	 * @param  port IEEE1588Port where the message will be assembled
	 * @param  destIdentity [in] Destination PortIdentity
	 * @return void
	 */
	void sendPort(IEEE1588Port * port, PortIdentity * destIdentity);

	friend PTPMessageCommon *buildPTPMessage(char *buf, int size,
						 LinkLayerAddress * remote,
						 IEEE1588Port * port);
};

/**
 * Provides a class for building the PTP Sync message
 */
class PTPMessageSync : public PTPMessageCommon {
 private:
	Timestamp originTimestamp;

	PTPMessageSync();
 public:
	/**
	 * @brief Default constructor. Creates PTPMessageSync 
	 * @param port IEEE1588Port
	 */
	PTPMessageSync(IEEE1588Port * port);

	/**
	 * Destroys PTPMessageSync interface
	 */
	~PTPMessageSync();

	/**
	 * @brief  Processes PTP messages
	 * @param  port [in] IEEE1588Port
	 * @return void
	 */
	void processMessage(IEEE1588Port * port);

	/**
	 * @brief  Gets origin timestamp value
	 * @return Origin Timestamp
	 */
	Timestamp getOriginTimestamp(void) {
		return originTimestamp;
	}

	/**
	 * @brief  Assembles PTPMessageSync message on the IEEE1588Port payload
	 * @param  port IEEE1588Port where the message will be assembled
	 * @param  destIdentity [in] Destination PortIdentity
	 * @return void
	 */
	void sendPort(IEEE1588Port * port, PortIdentity * destIdentity);

	friend PTPMessageCommon *buildPTPMessage
	(char *buf, int size, LinkLayerAddress * remote, IEEE1588Port * port);
};

/* Exact fit. No padding*/
#pragma pack(push,1)

/**
 * Provides a scaledNs interface
 * The scaledNs type represents signed values of time and time interval in units of 2e-16 ns.
 */
class scaledNs {
 private:
	int32_t ms;
	uint64_t ls;
 public:
	/**
	 * Builds scaledNs interface
	 */
	scaledNs() {
		ms = 0;
		ls = 0;
	}

	/**
	 * @brief  Gets scaledNs in a byte string format
	 * @param  byte_str [out] scaledNs value
	 * @return void
	 */
	void toByteString(uint8_t * byte_str) {
		memcpy(byte_str, this, sizeof(*this));
	}
};

/**
 * Provides a follow-up TLV interface back to the previous packing mode
 */
class FollowUpTLV {
 private:
	uint16_t tlvType;
	uint16_t lengthField;
	uint8_t organizationId[3];
	uint8_t organizationSubType_ms;
	uint16_t organizationSubType_ls;
	int32_t cumulativeScaledRateOffset;
	uint16_t gmTimeBaseIndicator;
	scaledNs scaledLastGmPhaseChange;
	int32_t scaledLastGmFreqChange;
 public:
	/**
	 * Builds the FollowUpTLV interface
	 */
	FollowUpTLV() {
		tlvType = PLAT_htons(0x3);
		lengthField = PLAT_htons(28);
		organizationId[0] = '\x00';
		organizationId[1] = '\x80';
		organizationId[2] = '\xC2';
		organizationSubType_ms = 0;
		organizationSubType_ls = PLAT_htons(1);
		cumulativeScaledRateOffset = PLAT_htonl(0);
		gmTimeBaseIndicator = PLAT_htons(0);
		scaledLastGmFreqChange = PLAT_htonl(0);
	}

	/**
	 * @brief  Gets FollowUpTLV information in a byte string format
	 * @param  byte_str [out] FollowUpTLV values
	 */
	void toByteString(uint8_t * byte_str) {
		memcpy(byte_str, this, sizeof(*this));
	}

	/**
	 * @brief  Gets the cummulative scaledRateOffset
	 * @return 32 bit signed value with the rate offset information.
	 */
	int32_t getRateOffset() {
		return cumulativeScaledRateOffset;
	}
};

/* back to whatever the previous packing mode was */
#pragma pack(pop)

/**
 * Provides a class for a class for building a PTP follow up message
 */
class PTPMessageFollowUp:public PTPMessageCommon {
private:
	Timestamp preciseOriginTimestamp;
	
	FollowUpTLV tlv;
	
	PTPMessageFollowUp(void) { }
public:
	/**
	 * Builds the PTPMessageFollowUP object
	 */
	PTPMessageFollowUp(IEEE1588Port * port);

	/**
	 * @brief  Assembles PTPMessageFollowUp message on the IEEE1588Port payload
	 * @param  port IEEE1588Port where the message will be assembled
	 * @param  destIdentity [in] Destination PortIdentity
	 * @return void
	 */
	void sendPort(IEEE1588Port * port, PortIdentity * destIdentity);

	/**
	 * @brief  Processes PTP messages
	 * @param  port [in] IEEE1588Port
	 * @return void
	 */
	void processMessage(IEEE1588Port * port); 
	/**
	 * @brief  Gets the precise origin timestamp value
	 * @return preciseOriginTimestamp value
	 */
	Timestamp getPreciseOriginTimestamp(void) {
		return preciseOriginTimestamp;
	}

	/**
	 * @brief  Sets the precis origin timestamp value
	 * @param  timestamp Timestamp to be set
	 * @return void
	 */
	void setPreciseOriginTimestamp(Timestamp & timestamp) {
		preciseOriginTimestamp = timestamp;
	}
	
	friend PTPMessageCommon *buildPTPMessage
	(char *buf, int size, LinkLayerAddress * remote, IEEE1588Port * port);
};

/**
 * Provides a class for building the PTP Path Delay Request message
 */
class PTPMessagePathDelayReq : public PTPMessageCommon {
 private:
	Timestamp originTimestamp;

	PTPMessagePathDelayReq() {
		return;
	}
 public:
	/**
	 * Destroys the PTPMessagePathDelayReq object
	 */
	~PTPMessagePathDelayReq() {
	}

	/**
	 * Builds the PTPMessagePathDelayReq message
	 */
	PTPMessagePathDelayReq(IEEE1588Port * port);

	/**
	 * @brief  Assembles PTPMessagePathDelayReq message on the IEEE1588Port payload
	 * @param  port IEEE1588Port where the message will be assembled
	 * @param  destIdentity [in] Destination PortIdentity
	 * @return void
	 */
	void sendPort(IEEE1588Port * port, PortIdentity * destIdentity);

	/**
	 * @brief  Processes PTP messages
	 * @param  port [in] IEEE1588Port
	 * @return void
	 */
	void processMessage(IEEE1588Port * port);

	/**
	 * @brief  Gets origin timestamp value
	 * @return Origin Timestamp
	 */
	Timestamp getOriginTimestamp(void) {
		return originTimestamp;
	}
	
	friend PTPMessageCommon *buildPTPMessage
	(char *buf, int size, LinkLayerAddress * remote, IEEE1588Port * port);
};

/**
 * Provides a class for building the PTP Path Delay Response message.
 */
class PTPMessagePathDelayResp:public PTPMessageCommon {
private:
	PortIdentity * requestingPortIdentity;
	Timestamp requestReceiptTimestamp;
	
	PTPMessagePathDelayResp(void) {	
	}
public:
	/**
	 * Destroys the PTPMessagePathDelayResp object
	 */
	~PTPMessagePathDelayResp();
	/**
	 * Builds the PTPMessagePathDelayResp object
	 */
	PTPMessagePathDelayResp(IEEE1588Port * port);

	/**
	 * @brief  Assembles PTPMessagePathDelayResp message on the IEEE1588Port payload
	 * @param  port IEEE1588Port where the message will be assembled
	 * @param  destIdentity [in] Destination PortIdentity
	 * @return void
	 */
	void sendPort(IEEE1588Port * port, PortIdentity * destIdentity);

	/**
	 * @brief  Processes PTP messages
	 * @param  port [in] IEEE1588Port
	 * @return void
	 */
	void processMessage(IEEE1588Port * port);
	
	/**
	 * @brief  Sets the request receipt timestamp
	 * @param  timestamp Timestamp to be set
	 * @return void
	 */
	void setRequestReceiptTimestamp(Timestamp timestamp) {
		requestReceiptTimestamp = timestamp;
	}
	
	/**
	 * @brief  Sets requesting port identity
	 * @param  identity [in] PortIdentity to be set
	 * @return void
	 */
	void setRequestingPortIdentity(PortIdentity * identity);
	/**
	 * @brief  Gets requesting port identity
	 * @param  identity [out] Requested PortIdentity
	 * @return void
	 */
	void getRequestingPortIdentity(PortIdentity * identity);
	
	/**
	 * @brief  Gets the request receipt timestamp
	 * @return requestReceiptTimestamp
	 */
	Timestamp getRequestReceiptTimestamp(void) {
		return requestReceiptTimestamp;
	}

	friend PTPMessageCommon *buildPTPMessage
	(char *buf, int size, LinkLayerAddress * remote, IEEE1588Port * port);
};

/**
 * Provides a class for building the PTP Path Delay Response follow up message.
 */
class PTPMessagePathDelayRespFollowUp:public PTPMessageCommon {
 private:
	Timestamp responseOriginTimestamp;
	PortIdentity *requestingPortIdentity;

	PTPMessagePathDelayRespFollowUp(void) { }
public:
	/**
	 * Builds the PTPMessagePathDelayRespFollowUp object
	 */
	PTPMessagePathDelayRespFollowUp(IEEE1588Port * port);

	/**
	 * Destroys the PTPMessagePathDelayRespFollowUp object
	 */
	~PTPMessagePathDelayRespFollowUp();

	/**
	 * @brief  Assembles PTPMessageRespFollowUp message on the IEEE1588Port payload
	 * @param  port IEEE1588Port where the message will be assembled
	 * @param  destIdentity [in] Destination PortIdentity
	 * @return void
	 */
	void sendPort(IEEE1588Port * port, PortIdentity * destIdentity);

	/**
	 * @brief  Processes PTP messages
	 * @param  port [in] IEEE1588Port
	 * @return void
	 */
	void processMessage(IEEE1588Port * port);

	/**
	 * @brief  Sets the response origin timestamp
	 * @param  timestamp Timestamp to be set
	 * @return void
	 */
	void setResponseOriginTimestamp(Timestamp timestamp) {
		responseOriginTimestamp = timestamp;
	}
	/**
	 * @brief  Sets the requesting port identity
	 * @param  identity [in] PortIdentity to be set
	 * @return void
	 */
	void setRequestingPortIdentity(PortIdentity * identity);

	/**
	 * @brief  Gets the response origin timestamp
	 * @return responseOriginTimestamp
	 */
	Timestamp getResponseOriginTimestamp(void) {
		return responseOriginTimestamp;
	}
	/**
	 * @brief Gets the requesting port identity
	 * @return Pointer to requesting PortIdentity object
	 */
	PortIdentity *getRequestingPortIdentity(void) {
		return requestingPortIdentity;
	}

	friend PTPMessageCommon *buildPTPMessage
	(char *buf, int size, LinkLayerAddress * remote, IEEE1588Port * port);
};

#endif
