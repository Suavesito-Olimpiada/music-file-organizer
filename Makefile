PREFIX ?= /usr
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin

LDLIBS += $(shell pkg-config --libs taglib icu-i18n)
CXXFLAGS ?= -O3 -pipe -march=native
CXXFLAGS += $(shell pkg-config --cflags taglib icu-i18n)

.PHONY: clean install

all: readmusictags organizemusic displaylibrary

readmusictags: AudioFile.cpp AudioFile.h readmusictags.cpp
organizemusic: AudioFile.cpp AudioFile.h organizemusic.cpp
displaylibrary: AudioFile.cpp AudioFile.h displaylibrary.cpp

install:
	@mkdir -p "$(DESTDIR)$(BINDIR)"
	@install -m 0755 -v readmusictags organizemusic displaylibrary "$(DESTDIR)$(BINDIR)/"

clean:
	rm -vf readmusictags organizemusic displaylibrary
