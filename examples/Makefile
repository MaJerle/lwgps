# gps-nmea-parser examples and tests Makefile
# (c) 2020 Sirio, Balmelli Analog & Digital

TARGETS := \
	example.exe \
	example_stat.exe \
	test_code.exe \
	test_time.exe

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
	  -DDEBUG=1 \
	-I../lwgps/src/include \
	-I./

example.exe: example.c

example_stat.exe: CFLAGS += -DLWGPS_CFG_STATUS=1
example_stat.exe: example_stat.c

test_code.exe: ../lwgps/src/lwgps/lwgps.c test_code.c ../dev/VisualStudio/main.c

test_time.exe: CFLAGS += \
	  -DLWGPS_CFG_STATEMENT_PUBX=1 \
	  -DLWGPS_CFG_STATEMENT_PUBX_TIME=1
test_time.exe: ../lwgps/src/lwgps/lwgps.c test_time.c ../dev/VisualStudio/main.c

$(TARGETS) : ../lwgps/src/lwgps/lwgps.c
	$(CC) -o $@ $(CFLAGS) $^
