LDFLAGS += $(shell pkg-config --libs taglib icu-i18n)
CXXFLAGS ?= -O3 -pipe -fomit-frame-pointer -march=native
CXXFLAGS += $(shell pkg-config --cflags taglib icu-uc)

all: readtags organizemusic

readtags: AudioFile.cpp AudioFile.h readtags.cpp
organizemusic: AudioFile.cpp AudioFile.h organizemusic.cpp

clean:
	rm -vf readtags organizemusic