#pragma once

using namespace std;

class DictDataFile : public BaseDataFile
{
private:
	map<string, string> cache;

public:
	DictDataFile(const char* filename);

	const char* get(const char* key);
	int getInt(const char* key);
	float getFloat(const char* key);
	bool getBool(const char* key);
	void set(const char* key, string value);
	void set(const char* key, const char* value);
	void set(const char* key, int value);
	void set(const char* key, bool value);
	void set(const char* key, float value);

protected:
	bool handleLineInput(string* line);
	bool writeData();
};