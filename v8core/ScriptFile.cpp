#include "ScriptFile.h"
#include <memory>

ScriptFile::ScriptFile(const char* file):file_name_(file)
{
	
}

ScriptFile::~ScriptFile()
{
}

bool ScriptFile::load() {
	FILE* _fd = NULL;
	if (0 != fopen_s(&_fd, file_name_.c_str(), "rb")) {
		return false;
	}

	fseek(_fd, 0L, SEEK_END);
	size_t _size = ftell(_fd);
	if (_size == 0) {
		fclose(_fd);
		return false;
	}
	rewind(_fd);
	std::unique_ptr<char> _buffer(new char[_size+1]);
	_buffer.get()[_size] = '\0';
	for (size_t i = 0; i < _size;) {
		i += fread_s(&_buffer.get()[i], _size - i, 1, _size - i, _fd);
		if (ferror(_fd)) {
			fclose(_fd);
			return false;
		}
	}
	fclose(_fd);
	content_ = _buffer.get();
	return true;
}

bool ScriptFile::loadBySource(const char * source)
{
	content_ = source;
	return true;
}

const char * ScriptFile::getContent() {
	return content_.c_str();
}