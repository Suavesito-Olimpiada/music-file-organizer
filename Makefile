LDFLAGS += $(shell pkg-config --libs taglib icu-i18n)
CXXFLAGS ?= -O3 -pipe -fomit-frame-pointer -march=native
CXXFLAGS += $(shell pkg-config --cflags taglib icu-i18n)

all: readmusictags organizemusic

readmusictags: AudioFile.cpp AudioFile.h readmusictags.cpp
organizemusic: AudioFile.cpp AudioFile.h organizemusic.cpp

clean:
	rm -vf readmusictags organizemusic
