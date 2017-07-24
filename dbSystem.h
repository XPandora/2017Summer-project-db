#ifndef _dbSystem_h
#define _dbSystem_h

#include "fileIO.h"
#include <string>

typedef int K;
typedef int V;

class DataBase {
public:
	DataBase();
	~DataBase();

	void openUser(char* filepath);
	void close();

	void storeUser();
	void modifyUser();
	void fetchUser();
	void removeUser();

	bool open(char*filepath);
	bool store(K key, V value);
	bool modify(K key, V value);
	bool fetch(K key);
	bool remove(K key);

private:
	BPlusTree db_Index;
	FileManager db_File;

	char* db_Filepath;
};

bool isValid(std::string s);
int StringToInt(std::string s);
#endif // !_dbSystem_h

