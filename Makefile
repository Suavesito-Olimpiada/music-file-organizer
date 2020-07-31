PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin

LDLIBS += $(shell pkg-config --libs taglib)
CXXFLAGS ?= -O3 -pipe -march=native
CXXFLAGS += $(shell pkg-config --cflags taglib)

.PHONY: clean install

all: readmusictags organizemusic displaylibrary

readmusictags: AudioFile.cpp AudioFile.h readmusictags.cpp
organizemusic: CXXFLAGS += $(shell pkg-config --cflags icu-i18n)
organizemusic: LDLIBS += $(shell pkg-config --libs icu-i18n)
organizemusic: AudioFile.cpp AudioFile.h organizemusic.cpp
displaylibrary: AudioFile.cpp AudioFile.h displaylibrary.cpp

install:
	@mkdir -p "$(DESTDIR)$(BINDIR)"
	@install -m 0755 -v readmusictags organizemusic displaylibrary "$(DESTDIR)$(BINDIR)/"

clean:
	rm -vf readmusictags organizemusic displaylibrary
