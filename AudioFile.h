#include <string>

class AudioFile
{
public:
	AudioFile(const std::string &filename);

	inline bool isValid() const { return m_isValid; };
	inline std::string filename() const { return m_filename; };
	inline std::string artist() const { return m_artist; };
	inline std::string composer() const { return m_composer; };
	inline std::string album() const { return m_album; };
	inline std::string albumArtist() const { return m_albumArtist; };
	inline std::string title() const { return m_title; };
	inline std::string genre() const { return m_genre; };
	inline std::string comment() const { return m_comment; };
	inline unsigned int track() const { return m_track; };
	inline unsigned int disc() const { return m_disc; };
	inline unsigned int bpm() const { return m_bpm; };
	inline unsigned int year() const { return m_year; };
	inline unsigned int length() const { return m_length; };
	inline unsigned int bitrate() const { return m_bitrate; };
	inline unsigned int sampleRate() const { return m_sampleRate; };
	inline unsigned int channels() const { return m_channels; };
	inline bool compilation() const { return m_compilation; };

	bool operator<(const AudioFile &other) const;

private:
	bool m_isValid;
	std::string m_filename;
	std::string m_artist;
	std::string m_composer;
	std::string m_album;
	std::string m_albumArtist;
	std::string m_title;
	std::string m_genre;
	std::string m_comment;
	unsigned int m_track;
	unsigned int m_disc;
	unsigned int m_bpm;
	unsigned int m_year;
	unsigned int m_length;
	unsigned int m_bitrate;
	unsigned int m_sampleRate;
	unsigned int m_channels;
	bool m_compilation;
};
