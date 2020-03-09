#pragma once
#include <string>

class ScriptFile
{
public:
	ScriptFile(const char * file);
	~ScriptFile();

	bool load();

	bool loadBySource(const char * source);

	const char * getContent();

private:
	std::string file_name_;
	std::string content_;
};

