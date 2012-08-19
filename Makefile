LDFLAGS += $(shell pkg-config --libs taglib)
CXXFLAGS ?= -O3 -pipe -fomit-frame-pointer -march=native
CXXFLAGS += $(shell pkg-config --cflags taglib)


all: readtags organizemusic

readtags: AudioFile.cpp AudioFile.h readtags.cpp

organizemusic: AudioFile.cpp AudioFile.h organizemusic.cpp
