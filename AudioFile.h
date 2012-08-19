#include <string>

class AudioFile
{
public:
	AudioFile(const std::string &filename);
	bool isValid() const;
	
	std::string filename() const;
	std::string artist() const;
	std::string composer() const;
	std::string album() const;
	std::string albumArtist() const;
	std::string title() const;
	std::string genre() const;
	std::string comment() const;
	unsigned int track() const;
	unsigned int disc() const;
	unsigned int bpm() const;
	unsigned int year() const;
	bool compilation() const;

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
	bool m_compilation;
};
