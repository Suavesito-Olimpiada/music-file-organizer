#include "AudioFile.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

vector<AudioFile> songs;

void process_path(const char *path);

void process_file(const char *filename)
{
	AudioFile audio(filename);
	if (audio.isValid())
		songs.push_back(audio);
}

void process_directory(const char *directory)
{
	DIR *dir = opendir(directory);
	if (!dir)
		return;
	while (struct dirent *entry = readdir(dir)) {
		if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' ||
			(entry->d_name[1] == '.' && strlen(entry->d_name) == 2)))
			continue;

		char joined[strlen(directory) + strlen(entry->d_name) + 2];
		sprintf(joined, "%s/%s", directory, entry->d_name);
		process_path(joined);
	}
	closedir(dir);
}

void process_path(const char *path)
{
	struct stat sbuf;
	if (stat(path, &sbuf))
		return;
	if (S_ISREG(sbuf.st_mode))
		process_file(path);
	else if (S_ISDIR(sbuf.st_mode))
		process_directory(path);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		process_path(argv[i]);

	sort(songs.begin(), songs.end());

	for (vector<AudioFile>::const_iterator i = songs.begin(); i != songs.end(); ++i)
		cout << i->track() << "\t" << i->year() << "\t" << i->artist() << " - " << i->album() << " - " << i->title() << endl;

	return 0;
}
