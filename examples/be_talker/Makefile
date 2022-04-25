CC?=gcc
OPT=-O2 -g
CFLAGS=$(OPT) -Wall -Wextra -Wno-parentheses
INCFLAGS= -I../../daemons/mrpd -I../common -I../../daemons/common -I../../lib/avtp_pipeline/include -I../../lib/avtp_pipeline/platform/platTCAL/GNU \
          -I ../../lib/avtp_pipeline/util/ -I../../lib/avtp_pipeline/platform/Linux/ -I../../lib/avtp_pipeline/platform/generic/ \
	  -I ../../lib/avtp_pipeline/rawsock/
LDLIBS= -lrt -lm -pthread
LDFLAGS=-L../../lib/igb

all: be_talker

be_talker: openavb_rawsock.o be_talker.o simple_rawsock.o rawsock_impl.o ring_rawsock.o openavb_log.o \
		openavb_queue.o

openavb_rawsock.o: ../../lib/avtp_pipeline/platform/Linux/rawsock/openavb_rawsock.c 
	$(CC) $(CFLAGS) $(INCFLAGS) -I../../lib/igb -c ../../lib/avtp_pipeline/platform/Linux/rawsock/openavb_rawsock.c

simple_rawsock.o: ../../lib/avtp_pipeline/platform/Linux/rawsock/simple_rawsock.c
	$(CC) $(CFLAGS) $(INCFLAGS) -I../../lib/igb -c ../../lib/avtp_pipeline/platform/Linux/rawsock/simple_rawsock.c

rawsock_impl.o: ../../lib/avtp_pipeline/rawsock/rawsock_impl.c
	$(CC) $(CFLAGS) $(INCFLAGS) -I../../lib/igb -c ../../lib/avtp_pipeline/rawsock/rawsock_impl.c

ring_rawsock.o: ../../lib/avtp_pipeline/platform/Linux/rawsock/ring_rawsock.c
	$(CC) $(CFLAGS) $(INCFLAGS) -I../../lib/igb -c ../../lib/avtp_pipeline/platform/Linux/rawsock/ring_rawsock.c

openavb_log.o: ../../lib/avtp_pipeline/util/openavb_log.c
	$(CC) $(CFLAGS) $(INCFLAGS) -I../../lib/igb -c ../../lib/avtp_pipeline/util/openavb_log.c

openavb_queue.o: ../../lib/avtp_pipeline/util/openavb_queue.c
	$(CC) $(CFLAGS) $(INCFLAGS) -I../../lib/igb -c ../../lib/avtp_pipeline/util/openavb_queue.c

be_talker.o: be_talker.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c be_talker.c

%: %.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	$(RM) be_talker
	$(RM) `find . -name "*~" -o -name "*.[oa]" -o -name "\#*\#" -o -name TAGS -o -name core -o -name "*.orig"`
