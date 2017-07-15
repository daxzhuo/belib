/*
   A reader that can read word from file efficiently
*/

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

class Reader {
public:
	Reader(int fd) : fd_(fd), ok(true) {
		buf = static_cast<char*>(malloc(defaultbuflen * sizeof(*buf)));
		if (buf != NULL) {
			begin = buf;
		} else {
			ok = false;
		}
	}
	~Reader() {
		delete[] buf;
		close(fd_);
	}
	void Init();
	std::string ScanWord();
	bool isok() { return ok; }
private:
	int fd_;
	char* buf;
	char* begin;
	char* end;
	bool ok;
	std::string token;
	static const int defaultbuflen;
};

const int Reader::defaultbuflen = 4096;

void Reader::Init() {
	int n = read(fd_, buf, defaultbuflen);
	if (n > 0)
		end = begin + n;
}


std::string Reader::ScanWord() {
	// skip previous charater
	for (auto b = begin; b < end; ++b) {
		if (std::isalpha(*b)) {
			begin = b;
			break;
		}
	}
	auto p = begin;
	for (auto b = begin; b < end; ++p) {
		if (!isalpha(*p)) {
			begin = p;
			return std::string(b, p - b);
		}
	}
	// scan until eof, on eof, exit or read more
	return "";
}

int main() {
	int fd = open("a.txt", 0);
	if (fd < 0)
		exit(-1);
	Reader r(fd);
	r.Init();
	//r.Scan();
	std::cout << r.ScanWord() << std::endl;

}
