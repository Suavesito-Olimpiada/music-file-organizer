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

template <typename T, typename M>
inline T extractTag(M &map, const char *key)
{
	T ret = 0;
	std::stringstream stream(extractTag<std::string>(map, key));
	stream >> ret;
	return ret;
}
template <typename M>
inline bool extractTag(M &map, const char *key)
{
	std::string str(extractTag<std::string>(map, key));
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return (str == "1" || str == "true");
}
template <>
inline std::string extractTag(const TagLib::ID3v2::FrameListMap &map, const char *key)
{
	if (map[key].isEmpty())
		return std::string();
	return map[key].front()->toString().to8Bit(true);
}
template <>
inline std::string extractTag(const TagLib::Ogg::FieldListMap &map, const char *key)
{
	if (map[key].isEmpty())
		return std::string();
	return map[key].front().to8Bit(true);
}
template <>
inline unsigned int extractTag(const TagLib::MP4::ItemListMap &map, const char *key)
{
	if (!map[key].isValid())
		return 0;
	return map[key].toInt();
}
template <>
inline std::string extractTag(const TagLib::MP4::ItemListMap &map, const char *key)
{
	if (!map[key].isValid())
		return std::string();
	return map[key].toStringList().toString().to8Bit(true);
}
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
	m_compilation(false)
{
	TagLib::FileRef fileRef(filename.c_str());
	if (fileRef.isNull() || !fileRef.tag())
		return;
	
	/* First we copy out everything TagLib actually supports. */
	const TagLib::Tag *tag = fileRef.tag();
	m_title = tag->title().to8Bit(true);
	m_artist = tag->artist().to8Bit(true);
	m_album = tag->album().to8Bit(true);
	m_comment = tag->comment().to8Bit(true);
	m_genre = tag->genre().to8Bit(true);
	m_year = tag->year();
	m_track = tag->track();

	/* Now we look at format specific tags. */
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

bool AudioFile::isValid() const
{
	return m_isValid;
}
std::string AudioFile::filename() const
{
	return m_filename;
}
std::string AudioFile::artist() const
{
	return m_artist;
}
std::string AudioFile::composer() const
{
	return m_composer;
}
std::string AudioFile::album() const
{
	return m_album;
}
std::string AudioFile::albumArtist() const
{
	return m_albumArtist;
}
std::string AudioFile::title() const
{
	return m_title;
}
std::string AudioFile::genre() const
{
	return m_genre;
}
std::string AudioFile::comment() const
{
	return m_comment;
}
unsigned int AudioFile::track() const
{
	return m_track;
}
unsigned int AudioFile::disc() const
{
	return m_disc;
}
unsigned int AudioFile::bpm() const
{
	return m_bpm;
}
unsigned int AudioFile::year() const
{
	return m_year;
}
bool AudioFile::compilation() const
{
	return m_compilation;
}
