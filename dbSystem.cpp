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
		std::cout << "数据库打开失败！" << std::endl;
	}
	else{
		std::cout << "数据库(" << filepath << ")打开成功" << std::endl;
	}
}

void DataBase::close() {
	db_File.write(db_Filepath);
	//把缓存中还未进行的操作做完
}

bool DataBase::store(K key, V value) {
	if (db_Index.search(key)) {
		return false;
	}

	OFFSET_TYPE index = db_File.getCount() * MAXLEN_RECORD;
	Record record(key, value);

	db_Index.insert(key, index);
	db_File.insert(record);

	//缓存的操作...

	return true;
}

void DataBase::storeUser() {
	K key;
	V value;
	while (true) {
		std::cout << "请输入数据ID(最大长度为9)： ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			key = StringToInt(enter);
			break;
		}
		else {
			std::cout << "输入错误！" << std::endl;
		}
	}

	while (true) {
		std::cout << "请输入数据值(最大长度为9)： ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			value = StringToInt(enter);
			break;
		}
		else {
			std::cout << "输入错误！" << std::endl;
		}
	}

	bool success = store(key, value);

	if (success) {
		std::cout << "数据存储成功" << std::endl;
	}
	else {
		std::cout << "数据已存在" << std::endl;
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
		std::cout << "请输入数据ID(最大长度为9)： ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			key = StringToInt(enter);
			break;
		}
		else {
			std::cout << "输入错误！" << std::endl;
		}
	}

	while (true) {
		std::cout << "请输入数据值(最大长度为9)： ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			value = StringToInt(enter);
			break;
		}
		else {
			std::cout << "输入错误！" << std::endl;
		}
	}
	bool success = modify(key, value);

	if (success) {
		std::cout << "数据修改成功！" << std::endl;
	}
	else {
		std::cout << "数据不存在!" << std::endl;
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
		std::cout << "请输入数据ID(最大长度为9)： ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			key = StringToInt(enter);
			break;
		}
		else {
			std::cout << "输入错误！" << std::endl;
		}
	}
	bool success = db_Index.search(key);

	if (success) {
		OFFSET_TYPE index = db_Index.fetch(key);
		Record record = db_File.fetch(index);

		std::cout << "所查询数据为： " << record.getScore() << std::endl;
	}
	else {
		std::cout << "数据不存在！" << std::endl;
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
		std::cout << "请输入数据ID(最大长度为9)： ";

		std::string enter;
		std::getline(std::cin, enter);

		if (isValid(enter)) {
			key = StringToInt(enter);
			break;
		}
		else {
			std::cout << "输入错误！" << std::endl;
		}
	}
	bool success = remove(key);

	if (success) {
		std::cout << "数据删除成功！" << std::endl;
	}
	else {
		std::cout << "数据不存在！" << std::endl;
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

