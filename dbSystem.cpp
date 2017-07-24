#include "dbSystem.h"
#include <sstream>
#include <iostream>
DataBase::DataBase() {
	/*Empty*/
}

DataBase::~DataBase() {
	close();
	/*Empty*/
}

bool DataBase::open(char*filepath) {

	if (!db_File.open(filepath, db_Index)) {
		return false;
	}

	db_Filepath = filepath;

	return true;
}

void DataBase::openUser(char*filepath) {
	bool success = open(filepath);

	if(success){
		std::cout << "���ݿ��ʧ�ܣ�" << std::endl;
	}
	else{
		std::cout << "���ݿ�(" << filepath << ")�򿪳ɹ�" << std::endl;
	}
}

void DataBase::close() {
	db_File.write(db_Filepath);
	//�ѻ����л�δ���еĲ�������
}

bool DataBase::store(K key, V value) {
	if (db_Index.search(key)) {
		return false;
	}

	OFFSET_TYPE index = db_File.getCount() * MAXLEN_RECORD;
	Record record(key, value);

	db_Index.insert(key, index);
	db_File.insert(record);

	//����Ĳ���...

	return true;
}

void DataBase::storeUser() {
	K key;
	V value;
	while (true) {
		std::cout << "����������ID(��󳤶�Ϊ9)�� ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			key = StringToInt(enter);
			break;
		}
		else {
			std::cout << "�������" << std::endl;
		}
	}

	while (true) {
		std::cout << "����������ֵ(��󳤶�Ϊ9)�� ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			value = StringToInt(enter);
			break;
		}
		else {
			std::cout << "�������" << std::endl;
		}
	}

	bool success = store(key, value);

	if (success) {
		std::cout << "���ݴ洢�ɹ�" << std::endl;
	}
	else {
		std::cout << "�����Ѵ���" << std::endl;
	}
}

bool DataBase::modify(K key, V value) {
	if (!db_Index.search(key)) {
		return false;
	}

	OFFSET_TYPE index;
	index = db_File.getIndex(key);

	db_File.modify(index, value);

	return true;

}

void DataBase::modifyUser() {
	K key;
	V value;
	while (true) {
		std::cout << "����������ID(��󳤶�Ϊ9)�� ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			key = StringToInt(enter);
			break;
		}
		else {
			std::cout << "�������" << std::endl;
		}
	}

	while (true) {
		std::cout << "����������ֵ(��󳤶�Ϊ9)�� ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			value = StringToInt(enter);
			break;
		}
		else {
			std::cout << "�������" << std::endl;
		}
	}
	bool success = modify(key, value);

	if (success) {
		std::cout << "�����޸ĳɹ���" << std::endl;
	}
	else {
		std::cout << "���ݲ�����!" << std::endl;
	}
}

bool DataBase::fetch(K key) {
	if (!db_Index.search(key)) {
		return false;
	}

	OFFSET_TYPE index = db_Index.fetch(key);
	Record record = db_File.fetch(index);

	return true;
}

void DataBase::fetchUser() {
	K key;
	while (true) {
		std::cout << "����������ID(��󳤶�Ϊ9)�� ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			key = StringToInt(enter);
			break;
		}
		else {
			std::cout << "�������" << std::endl;
		}
	}
	bool success = db_Index.search(key);

	if (success) {
		OFFSET_TYPE index = db_Index.fetch(key);
		Record record = db_File.fetch(index);

		std::cout << "����ѯ����Ϊ�� " << record.getScore() << std::endl;
	}
	else {
		std::cout << "���ݲ����ڣ�" << std::endl;
	}
}

bool DataBase::remove(K key) {
	if (!db_Index.search(key)) {
		return false;
	}

	OFFSET_TYPE index = db_Index.fetch(key);

	db_Index.remove(key);
	db_File.remove(index);

	return true;
}

void DataBase::removeUser() {
	K key;
	while (true) {
		std::cout << "����������ID(��󳤶�Ϊ9)�� ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			key = StringToInt(enter);
			break;
		}
		else {
			std::cout << "�������" << std::endl;
		}
	}
	bool success = remove(key);

	if (success) {
		std::cout << "����ɾ���ɹ���" << std::endl;
	}
	else {
		std::cout << "���ݲ����ڣ�" << std::endl;
	}
}

bool isValid(std::string s) {
	if (s.length() == 0 || s.length() > 9) {
		return false;
	}

	for (int i = 0; i < s.length(); i++) {
		if (s[i] < '0' || s[i] > '9') {
			return false;
		}
	}

	return true;
}
int StringToInt(std::string s) {
	std::stringstream ss;
	ss << s;

	int i;
	ss >> i;

	return i;
}

