#include "AudioFile.h"

#include <sstream>
#include <algorithm>

#include <taglib.h>
#include <fileref.h>

#include <mpegfile.h>
#include <id3v2tag.h>

#include <vorbisfile.h>
#include <flacfile.h>
#include <xiphcomment.h>

#include <mp4file.h>
#include <mp4tag.h>

/* Turn a string into an integer type for any map class. */
template <typename T, typename M>
inline T extractTag(M &map, const char *key)
{
	T ret = 0;
	std::stringstream stream(extractTag<std::string>(map, key));
	stream >> ret;
	return ret;
}
/* Turn a string into a bool, based on "1" and "true", for any map class. */
template <typename M>
inline bool extractTag(M &map, const char *key)
{
	std::string str(extractTag<std::string>(map, key));
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return (str == "1" || str == "true");
}
/* Extract a string out of an MP3 map. */
template <>
inline std::string extractTag(const TagLib::ID3v2::FrameListMap &map, const char *key)
{
	if (map[key].isEmpty())
		return std::string();
	return map[key].front()->toString().to8Bit(true);
}
/* Extract a string out of an OGG map. */
template <>
inline std::string extractTag(const TagLib::Ogg::FieldListMap &map, const char *key)
{
	if (map[key].isEmpty())
		return std::string();
	return map[key].front().to8Bit(true);
}
/* Extract an integer out of an MP4 map. */
template <>
inline unsigned int extractTag(const TagLib::MP4::ItemListMap &map, const char *key)
{
	if (!map[key].isValid())
		return 0;
	return map[key].toInt();
}
/* Extract a string out of an MP4 map. */
template <>
inline std::string extractTag(const TagLib::MP4::ItemListMap &map, const char *key)
{
	if (!map[key].isValid())
		return std::string();
	return map[key].toStringList().toString().to8Bit(true);
}
/* Extract a bool out of an MP4 map. */
template <>
inline bool extractTag(const TagLib::MP4::ItemListMap &map, const char *key)
{
	if (!map[key].isValid())
		return false;
	return map[key].toBool();
}

AudioFile::AudioFile(const std::string &filename) :
	m_isValid(false),
	m_filename(filename),
	m_track(0),
	m_disc(0),
	m_bpm(0),
	m_year(0),
	m_length(0),
	m_bitrate(0),
	m_sampleRate(0),
	m_channels(0),
	m_compilation(false)
{
	TagLib::FileRef fileRef(filename.c_str());
	if (fileRef.isNull() || !fileRef.tag())
		return;
	
	/* First we copy out everything TagLib actually supports. */
	if (const TagLib::Tag *tag = fileRef.tag()) {
		m_title = tag->title().to8Bit(true);
		m_artist = tag->artist().to8Bit(true);
		m_album = tag->album().to8Bit(true);
		m_comment = tag->comment().to8Bit(true);
		m_genre = tag->genre().to8Bit(true);
		m_year = tag->year();
		m_track = tag->track();
		/* Sometimes these integers are supposed to wrap,
		 * and taglib doesn't know about it, so we do it manually. */
		if (m_track == 4294967295)
			m_track = 0;
		if (m_year == 4294967295)
			m_year = 0;
	}
	/* If there isn't a title, generate one from the basename sans-extension of the file. */
	if (m_title.length() == 0) {
		size_t pos;
		m_title = filename;
		pos = m_title.find_last_of('/');
		if (pos != std::string::npos)
			m_title.erase(0, pos + 1);
		pos = m_title.find_last_of('.');
		if (pos != std::string::npos)
			m_title.erase(pos);
	}
	/* TagLib also provides AudioProperties. */
	if (const TagLib::AudioProperties *audio = fileRef.audioProperties()) {
		m_length = audio->length();
		m_bitrate = audio->bitrate();
		m_sampleRate = audio->sampleRate();
		m_channels = audio->channels();
	}

	/* Now we look at format specific tags, attempting to upcast, and then extracting fields. */
	if (TagLib::MPEG::File *file = dynamic_cast<TagLib::MPEG::File*>(fileRef.file())) {
		if (file->ID3v2Tag()) {
			const TagLib::ID3v2::FrameListMap &map = file->ID3v2Tag()->frameListMap();
			m_disc = extractTag<unsigned int>(map, "TPOS");
			m_bpm = extractTag<unsigned int>(map, "TBPM");
			m_composer = extractTag<std::string>(map, "TCOM");
			m_albumArtist = extractTag<std::string>(map, "TPE2");
			m_compilation = extractTag<bool>(map, "TCMP");
		}

	} else if (TagLib::Ogg::Vorbis::File *file = dynamic_cast<TagLib::Ogg::Vorbis::File*>(fileRef.file())) {
		if (file->tag()) {
			const TagLib::Ogg::FieldListMap &map = file->tag()->fieldListMap();
			m_disc = extractTag<unsigned int>(map, "DISCNUMBER");
			m_bpm = extractTag<unsigned int>(map, "BPM");
			m_composer = extractTag<std::string>(map, "COMPOSER");
			m_compilation = extractTag<bool>(map, "COMPILATION");
		}
	} else if (TagLib::FLAC::File *file = dynamic_cast<TagLib::FLAC::File*>(fileRef.file())) {
		if (file->xiphComment()) {
			const TagLib::Ogg::FieldListMap &map = file->xiphComment()->fieldListMap();
			m_disc = extractTag<unsigned int>(map, "DISCNUMBER");
			m_bpm = extractTag<unsigned int>(map, "BPM");
			m_composer = extractTag<std::string>(map, "COMPOSER");
			m_compilation = extractTag<bool>(map, "COMPILATION");
		}
	} else if (TagLib::MP4::File *file = dynamic_cast<TagLib::MP4::File*>(fileRef.file())) {
		if (file->tag()) {
			const TagLib::MP4::ItemListMap &map = file->tag()->itemListMap();
			m_disc = extractTag<unsigned int>(map, "disk");
			m_bpm = extractTag<unsigned int>(map, "tmpo");
			m_albumArtist = extractTag<std::string>(map, "aART");
			m_composer = extractTag<std::string>(map, "\xa9wrt");
			m_compilation = extractTag<bool>(map, "cpil");
		}
	}

	m_isValid = true;
}
