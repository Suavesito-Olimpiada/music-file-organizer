PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin

LDLIBS += $(shell pkg-config --libs taglib)
CXXFLAGS ?= -O3 -pipe -march=native
CXXFLAGS += $(shell pkg-config --cflags taglib)

.PHONY: all clean install

BINARIES := readmusictags organizemusic displaylibrary
all: $(BINARIES)

readmusictags:
organizemusic: CXXFLAGS += $(shell pkg-config --cflags icu-i18n)
organizemusic: LDLIBS += $(shell pkg-config --libs icu-i18n)
organizemusic:
displaylibrary:
$(BINARIES): AudioFile.cpp

install: $(BINARIES)
	@mkdir -p "$(DESTDIR)$(BINDIR)"
	@install -m 0755 -v $(BINARIES) "$(DESTDIR)$(BINDIR)/"

clean:
	$(RM) $(BINARIES)
