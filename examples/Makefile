# gps-nmea-parser examples and tests Makefile
# (c) 2020 Sirio, Balmelli Analog & Digital

TARGETS := \
	example.exe \
	example_stat.exe \
	test.exe

.PHONY: all clean test
all: $(TARGETS)

clean:
	@rm -fv $(TARGETS)

test: $(TARGETS)
	@for tgt in $(TARGETS); do \
		echo "\n--- $$tgt ---"; \
		./$$tgt; \
	done

CFLAGS += -Wall \
	  -DGPS_CFG_STATEMENT_PUBX=1 \
	  -DGPS_CFG_STATEMENT_PUBX_TIME=1 \
	  -DDEBUG=1 \
	-I../gps_nmea_parser/src/include

example.exe: example.c

example_stat.exe: CFLAGS += -DGPS_CFG_STATUS=1
example_stat.exe: example_stat.c

test.exe: ../gps_nmea_parser/src/gps/gps.c test_code.c ../dev/VisualStudio/main.c

$(TARGETS) : ../gps_nmea_parser/src/gps/gps.c
	$(CC) -o $@ $(CFLAGS) $^
