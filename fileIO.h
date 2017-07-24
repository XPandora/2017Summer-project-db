#ifndef _fileIO_h
#define _fileIO_h

#include "buffer.h"
#include "BPlusTree.h"
#include <vector>

#define MAXLEN_RECORD 20
#define NOT_FOUND -1

class FileManager {
public:
	FileManager();
	~FileManager();

	bool open(char* filepath, BPlusTree &tree);
	bool write(char* filepath);
	bool createIndex(char*filepath, BPlusTree &tree);
	bool writeTest(char* filepath);

	bool insert(Record record);
	bool remove(OFFSET_TYPE index);
	bool modify(OFFSET_TYPE index, int value);
	Record fetch(OFFSET_TYPE index);

	char* getFilepath() { return r_Filepath; }
	int getCount() { return r_Count; }
	std::vector<Record> getAllRcd() { return r_Records; }

	OFFSET_TYPE getIndex(int key);

	void clear();

private:
	int r_Count;
	char* r_Filepath;
	Buffer r_Buffer;

	std::vector<Record> r_Records;
};

std::string newIndexMsg(int ID, int index);
std::string newDataMsg(int ID, int score);
#endif // !_fileIO_h

