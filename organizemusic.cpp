#include "AudioFile.h"

#include <unicode/translit.h>
#include <unicode/unistr.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

static string base_destination = "/home/zx2c4/Music/";
static Transliterator *transliterator;

void process_file(const char *filename, ino_t inode);
void process_directory(const char *directory);
void process_path(const char *path);

string generate_path(const AudioFile &audio);
string truncated(const string &str, const string &ext);
string transliterated(const string &str);
void strip_slash(string &name);
void disc_track(unsigned int disc, unsigned int track, ostringstream &path);
void rename_path(const string &source, const string &stem, ino_t inode);
void make_parents(const string &filepath);

string strip_slash(const string &name)
{
	string ret(name);
	replace(ret.begin(), ret.end(), '/', '-');
	return ret;
}
void disc_track(unsigned int disc, unsigned int track, ostringstream &path)
{
	if (disc > 0) {
		path << disc;
		if (track > 0)
			path << '-';
		else
			path << ' ';
	}
	if (track > 0)
		path << setw(2) << setfill('0') << track << setw(0) << ' ';
}
string generate_path(const AudioFile &audio)
{
	ostringstream path;
	if (audio.compilation()) {
		path << "Various Artists/";
		if (audio.album().length() == 0)
			path << "Unknown Album/";
		else
			path << strip_slash(audio.album()) << '/';
		disc_track(audio.disc(), audio.track(), path);
		if (audio.artist().length() > 0)
			path << strip_slash(audio.artist()) << " - ";
		path << strip_slash(audio.title());
	} else {
		if (audio.artist().length() == 0)
			path << "Unknown Artist/";
		else
			path << strip_slash(audio.artist()) << '/';
		if (audio.album().length() > 0)
			path << strip_slash(audio.album()) << '/';
		disc_track(audio.disc(), audio.track(), path);
		path << strip_slash(audio.title());
	}
	return transliterated(path.str());
}
string truncated(const string &str, const string &ext)
{
	stringstream stream;
	size_t current, next = -1;
	for (;;) {
		current = next + 1;
		next = str.find_first_of('/', current);
		if (current)
			stream << '/';
		if (next == string::npos) {
			stream << str.substr(current, static_cast<size_t>(255 - ext.length())) << ext;
			break;
		}
		stream << str.substr(current, min(static_cast<size_t>(255), next - current));
	}
	return stream.str();
}
string transliterated(const string &str)
{
	UnicodeString in(str.c_str(), "UTF-8");
	transliterator->transliterate(in);
	char out[in.length() + 1];
	out[in.extract(0, in.length(), out, in.length(), "ASCII")] = '\0';
	return out;
}
void rename_path(const string &source, const string &stem, ino_t inode)
{
	size_t pos = source.find_last_of('.');
	string ext;
	if (pos != string::npos) {
		ext = source.substr(pos);
		transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	}
	string destination = truncated(base_destination + stem, ext);
	unsigned long counter = 0;
	for (;;) {
		struct stat sbuf;
		if (stat(destination.c_str(), &sbuf)) {
			if (errno == ENOENT)
				break;
		}
		if (sbuf.st_ino == inode)
			return;
		
		ostringstream stream;
		stream << base_destination << stem << ' ' << ++counter;
		destination = truncated(stream.str(), ext);

		if (counter == 0)
			return;
	}

	make_parents(destination);
	if (!rename(source.c_str(), destination.c_str()))
		cout << CYAN << "move" << RESET << ": " << YELLOW << source << RESET << " ==> " << GREEN << destination << RESET << endl;
	else {
		cerr << RED;
		perror(destination.c_str());
		cerr << RESET;
	}
}
void make_parents(const string &filepath)
{
	string path;
	size_t current, next = -1;
	for (;;) {
		current = next + 1;
		if ((next = filepath.find_first_of('/', current)) == string::npos)
			return;

		path.append(filepath.substr(current, next - current));
		path.append("/");

		if (!mkdir(path.c_str(), 0755))
			cout << MAGENTA << "mkdir" << RESET << ": " << path.c_str() << endl;
	}
}
void process_file(const char *filename, ino_t inode)
{
	AudioFile audio(filename);
	if (!audio.isValid()) {
		cerr << RED << filename << ": not a valid audio file" << RESET << endl;
		return;
	}
	rename_path(filename, generate_path(audio), inode);
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
	if (!rmdir(directory))
		cout << BLUE << "rmdir" << RESET << ": " << directory << endl;
}
void process_path(const char *path)
{
	struct stat sbuf;
	if (stat(path, &sbuf)) {
		cerr << RED;
		perror(path);
		cerr << RESET;
		return;
	}
	if (S_ISREG(sbuf.st_mode))
		process_file(path, sbuf.st_ino);
	else if (S_ISDIR(sbuf.st_mode))
		process_directory(path);
	else
		cerr << RED << path << ": not a file nor a directory" << RESET << endl;
}

int main(int argc, char *argv[])
{
	UErrorCode status = U_ZERO_ERROR;
	transliterator = Transliterator::createInstance("Latin; NFD; [:Nonspacing Mark:] Remove; NFC; [:^ASCII:] Remove; [\\:;*?\"<>|\\\\] Remove", UTRANS_FORWARD, status);
	if (!transliterator || status != U_ZERO_ERROR) {
		cerr << RED << "Fatal: Could not initialize transliterator." << RESET << endl;
		return EXIT_FAILURE;
	}

	for (int i = 1; i < argc; ++i) {
		size_t len = strlen(argv[i]);
		if (argv[i][len - 1] == '/')
			argv[i][len - 1] = '\0';
		process_path(argv[i]);
	}
	
	return 0;
}
