#!/usr/bin/make -f

include Makefile.mk

PREFIX  ?= /usr/local
DESTDIR ?=


all: build

build: midsidegain.c
	$(CC) $^ $(BUILD_C_FLAGS) $(LINK_FLAGS) -lm $(SHARED) -o midsidegain.lv2/midsidegain$(LIB_EXT)

clean:
	rm -f midsidegain.lv2/midsidegain$(LIB_EXT)

install:
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/midsidegain.lv2
	install -m 644 midsidegain.lv2/*.so  $(DESTDIR)$(PREFIX)/lib/lv2/midsidegain.lv2/
	install -m 644 midsidegain.lv2/*.ttl $(DESTDIR)$(PREFIX)/lib/lv2/midsidegain.lv2/
