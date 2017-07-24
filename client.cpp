#include "dbSystem.h"
#include <iostream>
#include <string>

/*void OpenFile(DataBase &db) {
	std::cout << "请输入数据路径： ";

	while (true) {
		std::string filepath;
		std::getline(std::cin, filepath);

		if (!db.open((char*)filepath.data())) {
			std::cout << "数据路径未找到！" << std::endl;
			std::cout << "请重新输入数据路径: ";
		}
		else {
			std::cout << "数据库(" << filepath << ")打开成功" << std::endl;
			break;
		}
	}
}*/

void Function(DataBase &db) {
	std::cout << "-----1:查询数据" << std::endl;
	std::cout << "-----2:修改数据" << std::endl;
	std::cout << "-----3:增加数据" << std::endl;
	std::cout << "-----4:删除数据" << std::endl;
	std::cout << "-----5:退出" << std::endl;

	while (true){
		std::cout << "-----请选择功能：";
		std::string choice;
		std::getline(std::cin, choice);

		if (choice == "1") {
			db.fetchUser();
		}
		else if (choice == "2") {
			db.modifyUser();
		}
		else if (choice == "3") {
			db.storeUser();
		}
		else if (choice == "4") {
			db.removeUser();
		}
		else if (choice == "5") {
			db.close();
			exit(0);
		}
		else {
			std::cout << "输入错误！请重新选择！" << std::endl;
			continue;
		}

		break;
	}
}

int main() {
	DataBase myDataBase;
	std::cout << "请输入数据路径： ";
	std::string filepath;

	while (true) {
		std::getline(std::cin, filepath);

		if (!myDataBase.open((char*)filepath.data())) {
			std::cout << "数据路径未找到！" << std::endl;
			std::cout << "请重新输入数据路径: ";
		}
		else {
			std::cout << "数据库(" << filepath << ")打开成功" << std::endl;
			break;
		}
	}

	while (true){
		Function(myDataBase);
	}

	return 0;
}