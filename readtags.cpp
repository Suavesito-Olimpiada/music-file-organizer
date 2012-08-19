#include "AudioFile.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i) {
		AudioFile f(argv[i]);
		if (!f.isValid()) {
			cout << argv[i] << " is not valid." << endl;
			continue;
		}
		cout << "Filename:	" << f.filename() << endl;
		cout << "Artist:		" << f.artist() << endl;
		cout << "Composer:	" << f.composer() << endl;
		cout << "Album:		" << f.album() << endl;
		cout << "Album Artist:	" << f.albumArtist() << endl;
		cout << "Title:		" << f.title() << endl;
		cout << "Genre:		" << f.genre() << endl;
		cout << "Comment:	" << f.comment() << endl;
		cout << "Track:		" << f.track() << endl;
		cout << "Disc:		" << f.disc() << endl;
		cout << "Bpm:		" << f.bpm() << endl;
		cout << "Year:		" << f.year() << endl;
		cout << "Length:		" << f.length() << endl;
		cout << "Bitrate:	" << f.bitrate() << endl;
		cout << "Sample Rate:	" << f.sampleRate() << endl;
		cout << "Channels:	" << f.channels() << endl;
		cout << "Compilation:	" << f.compilation() << endl;
		cout << endl;
	}
}
