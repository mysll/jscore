#pragma once
#include <string>
#include "v8.h"

class ScriptFile
{
public:
	ScriptFile(const char * file);
	~ScriptFile();

	bool load();

	bool loadBySource(const char * source);

	inline const char * getContent() { return content_.c_str(); }
	inline const char * fileName() { return file_name_.c_str(); }

private:
	std::string file_name_;
	std::string content_;
	//v8::ScriptOrigin origin_;
};

