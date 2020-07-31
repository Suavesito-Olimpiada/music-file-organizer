PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin

LDLIBS += $(shell pkg-config --libs taglib)
CXXFLAGS ?= -O3 -pipe -march=native
CXXFLAGS += $(shell pkg-config --cflags taglib)

.PHONY: all clean install

BINARIES := readmusictags organizemusic displaylibrary
all: $(BINARIES)

readmusictags: AudioFile.cpp AudioFile.h readmusictags.cpp
organizemusic: CXXFLAGS += $(shell pkg-config --cflags icu-i18n)
organizemusic: LDLIBS += $(shell pkg-config --libs icu-i18n)
organizemusic: AudioFile.cpp AudioFile.h organizemusic.cpp
displaylibrary: AudioFile.cpp AudioFile.h displaylibrary.cpp

install: $(BINARIES)
	@mkdir -p "$(DESTDIR)$(BINDIR)"
	@install -m 0755 -v $(BINARIES) "$(DESTDIR)$(BINDIR)/"

clean:
	$(RM) $(BINARIES)
