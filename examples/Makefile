# gps-nmea-parser examples and tests Makefile
# (c) 2020 Sirio, Balmelli Analog & Digital

TARGETS := example.exe test.exe

.PHONY: all clean test
all: $(TARGETS)

clean:
	@rm -fv $(TARGETS)

test: $(TARGETS)
	./example.exe
	./test.exe


CFLAGS += -Wall \
	  -DGPS_CFG_STATEMENT_PUBX=1 \
	  -DGPS_CFG_STATEMENT_PUBX_TIME=1 \
	  -DDEBUG=1 \
	-I../gps_nmea_parser/src/include

test.exe: ../gps_nmea_parser/src/gps/gps.c test_code.c ../dev/VisualStudio/main.c
	$(CC) -o $@ $(CFLAGS) $^

%.exe : ../gps_nmea_parser/src/gps/gps.c %.c
	$(CC) -o $@ $(CFLAGS) $^
