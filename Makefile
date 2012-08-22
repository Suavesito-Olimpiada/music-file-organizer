LDFLAGS += $(shell pkg-config --libs taglib icu-i18n)
CXXFLAGS ?= -O3 -pipe -fomit-frame-pointer -march=native
CXXFLAGS += $(shell pkg-config --cflags taglib icu-i18n)

all: readmusictags organizemusic displaylibrary

readmusictags: AudioFile.cpp AudioFile.h readmusictags.cpp
organizemusic: AudioFile.cpp AudioFile.h organizemusic.cpp
displaylibrary: AudioFile.cpp AudioFile.h displaylibrary.cpp

clean:
	rm -vf readmusictags organizemusic displaylibrary
