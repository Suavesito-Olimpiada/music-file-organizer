#include "AudioFile.h"

#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i) {
		AudioFile f(argv[i]);
		if (!f.isValid()) {
			cout << argv[i] << " is not valid." << endl;
			continue;
		}
		cout << setw(14) << "Filename: " << setw(0) << f.filename() << endl;
		cout << setw(14) << "Artist: " << setw(0) << f.artist() << endl;
		cout << setw(14) << "Composer: " << setw(0) << f.composer() << endl;
		cout << setw(14) << "Album: " << setw(0) << f.album() << endl;
		cout << setw(14) << "Album Artist: " << setw(0) << f.albumArtist() << endl;
		cout << setw(14) << "Title: " << setw(0) << f.title() << endl;
		cout << setw(14) << "Genre: " << setw(0) << f.genre() << endl;
		cout << setw(14) << "Comment: " << setw(0) << f.comment() << endl;
		cout << setw(14) << "Track: " << setw(0) << f.track() << endl;
		cout << setw(14) << "Disc: " << setw(0) << f.disc() << '/' << f.discTotal() << endl;
		cout << setw(14) << "Bpm: " << setw(0) << f.bpm() << endl;
		cout << setw(14) << "Year: " << setw(0) << f.year() << endl;
		cout << setw(14) << "Length: " << setw(0) << f.length() << endl;
		cout << setw(14) << "Bitrate: " << setw(0) << f.bitrate() << endl;
		cout << setw(14) << "Sample Rate: " << setw(0) << f.sampleRate() << endl;
		cout << setw(14) << "Channels: " << setw(0) << f.channels() << endl;
		cout << setw(14) << "Compilation: " << setw(0) << f.compilation() << endl;
		if (i != argc - 1)
			cout << endl;
	}
	return 0;
}
