#include "fileIO.h"
#include <fstream>
#include <sstream>
#include <string>

FileManager::FileManager() {
	r_Count = 0;
}

FileManager::~FileManager() {
	clear();
}

bool FileManager::open(char*filepath,BPlusTree &tree) {
	if (filepath == NULL) {
		return false;
	}

	std::string dataFile = filepath;
	dataFile += "-dat.txt";

	std::string indexFile = filepath;
	indexFile += "-idx.txt";

	std::ifstream finDat(dataFile, std::ios::in | std::ios::out | std::ios::binary);
	std::ifstream finIdx(indexFile, std::ios::in | std::ios::out | std::ios::binary);

	if (!finDat.is_open()) {
		return false;
	}

	r_Filepath = filepath;

	if (!finIdx.is_open()) {
		return createIndex(filepath, tree);
	}

	while (!finDat.eof()) {
		char temp[256] = {};
		char *record = temp;

		finDat.read(record, MAXLEN_RECORD);
		if (record[0] == 0)break;

		Record newRec(record);

		r_Records.push_back(newRec);
		r_Count++;
	}

	while (!finIdx.eof()) {
		char temp[256] = {};
		char *indexMsg = temp;

		finIdx.read(indexMsg, MAXLEN_RECORD);

		if (indexMsg[0] == 0)break;

		Record newMsg(indexMsg);

		int ID = newMsg.getID();
		int index = newMsg.getScore();

		tree.insert(ID, index);
	}


	return true;
}

bool FileManager::createIndex(char* filepath,BPlusTree &tree) {
	if (filepath == NULL) {
		return false;
	}

	std::string dataFile = filepath;
	dataFile += "-dat.txt";

	std::string indexFile = filepath;
	indexFile += "-idx.txt";

	std::ifstream fin(dataFile, std::ios::in | std::ios::out | std::ios::binary);
	std::ofstream fout(indexFile, std::ios::in | std::ios::out | std::ios::binary);

	if (!fin.is_open()) {
		return false;
	}

	r_Filepath = filepath;
	
	while (!fin.eof()) {
		char temp[256] = {};
		char *record = temp;

		fin.read(record, MAXLEN_RECORD);
		if (record[0] == 0)break;

		Record newRec(record);

		int ID = newRec.getID();
		int index = r_Count * MAXLEN_RECORD;

		tree.insert(ID, index);

		r_Records.push_back(newRec);
		r_Count++;

		std::string newMsg = newIndexMsg(ID, index);
		fout.write(newMsg.c_str(),MAXLEN_RECORD);

		
	}

	return true;
}
bool FileManager::writeTest(char*filepath) {
	if (filepath == NULL) {
		return false;
	}

	std::string dataFile = filepath;
	dataFile += "-dat.txt";
	
	std::ofstream foutDat(dataFile, std::ios::binary);
	//foutDat.seekp(4, std::ios::end);

	std::string s = "test";
	
	//foutDat.write(s.c_str(), sizeof(s.c_str()));

}
bool FileManager::write(char*filepath) {
	if (filepath == NULL) {
		return false;
	}

	std::string dataFile = filepath;
	dataFile += "-dat.txt";

	std::string indexFile = filepath;
	indexFile += "-idx.txt";

	std::ofstream foutDat(dataFile, std::ios::in | std::ios::out | std::ios::binary);
	std::ofstream foutIdx(indexFile, std::ios::in | std::ios::out | std::ios::binary);

	for (int i = 0; i < r_Buffer.getCount(); i++) {
		BufferToken token = r_Buffer.getToken(i);
		Record record = token.getBufferRecord();
	
		switch (token.getType())
		{
		case INSERT:{
			foutDat.seekp(0, std::ios::end);
			foutIdx.seekp(0, std::ios::end);

			std::string dataMsg = newDataMsg(record.getID(), record.getScore());
			std::string indexMsg = newIndexMsg(record.getID(), getIndex(record.getID()));

			foutDat.write(dataMsg.c_str(), MAXLEN_RECORD);
			foutIdx.write(indexMsg.c_str(), MAXLEN_RECORD);

			break;
		}
		case MODIFY: {
			OFFSET_TYPE offset;
			offset = getIndex(record.getID());

			std::string dataMsg = newDataMsg(record.getID(), record.getScore());
			std::string indexMsg = newIndexMsg(record.getID(), offset);

			foutDat.seekp(offset, std::ios::beg);
			foutIdx.seekp(offset, std::ios::beg);

			foutDat.write(dataMsg.c_str(), MAXLEN_RECORD);
			foutIdx.write(indexMsg.c_str(), MAXLEN_RECORD);

			break;
		}
		case REMOVE: {
			OFFSET_TYPE offset;
			offset = token.getOrder() * MAXLEN_RECORD;

			foutDat.seekp(offset, std::ios::beg);
			foutIdx.seekp(offset, std::ios::beg);

			std::string EmptyMsg;
			for (int i = 0; i < MAXLEN_RECORD; i++) {
				EmptyMsg += " ";
			}

			foutDat.write(EmptyMsg.c_str(), MAXLEN_RECORD);
			foutIdx.write(EmptyMsg.c_str(), MAXLEN_RECORD);

			break;
		}
		}
	}

	r_Buffer.clear();

	return true;
}

bool FileManager::insert(Record record) {
	r_Records.push_back(record);
	r_Count++;

	BufferToken token(record, INSERT, r_Count - 1);
	r_Buffer.add(token);

	if (r_Buffer.getCount() >= MAXNUM_BUFFER) {
		write(r_Filepath);
		r_Buffer.clear();
	}

	return true;
}

bool FileManager::remove(OFFSET_TYPE index) {
	int pos = index / MAXLEN_RECORD;
	r_Records[pos].clear();

	BufferToken token(r_Records[pos], REMOVE, pos);
	r_Buffer.add(token);

	if (r_Buffer.getCount() >= MAXNUM_BUFFER) {
		write(r_Filepath);
		r_Buffer.clear();
	}

	return true;
}

bool FileManager::modify(OFFSET_TYPE index, int value) {
	int pos = index / MAXLEN_RECORD;
	r_Records[pos].setScore(value);

	BufferToken token(r_Records[pos], MODIFY, pos);
	r_Buffer.add(token);

	if (r_Buffer.getCount() >= MAXNUM_BUFFER) {
		write(r_Filepath);
		r_Buffer.clear();
	}

	return true;
}

Record FileManager::fetch(OFFSET_TYPE index) {
	int pos = index / MAXLEN_RECORD;
	return r_Records[pos];
}

void FileManager::clear() {
	r_Count = 0;
	r_Records.clear();
	r_Buffer.clear();
}

OFFSET_TYPE FileManager::getIndex(int key) {
	for (int i = 0; i < r_Count; i++) {
		if (r_Records[i].getID() == key) {
			return i * MAXLEN_RECORD;
		}
	}

	return NOT_FOUND;
}


std::string newIndexMsg(int ID, int index) {
	std::stringstream ss;
	ss << ID;

	std::string stringID;
	ss >> stringID;

	ss.clear();
	ss << index;

	std::string stringIndex;
	ss >> stringIndex;

	std::string newMsg;

	newMsg = stringID;
	for (int i = stringID.length(); i < MAXLEN_RECORD / 2; i++) {
		newMsg += " ";
	}

	newMsg += stringIndex;
	for (int i = stringIndex.length(); i < MAXLEN_RECORD / 2; i++) {
		newMsg += " ";
	}

	return newMsg;
}

std::string newDataMsg(int ID, int score) {

	return newIndexMsg(ID, score);
}