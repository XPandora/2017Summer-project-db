#include "dbSystem.h"
#include <iostream>
#include <string>

/*void OpenFile(DataBase &db) {
	std::cout << "����������·���� ";

	while (true) {
		std::string filepath;
		std::getline(std::cin, filepath);

		if (!db.open((char*)filepath.data())) {
			std::cout << "����·��δ�ҵ���" << std::endl;
			std::cout << "��������������·��: ";
		}
		else {
			std::cout << "���ݿ�(" << filepath << ")�򿪳ɹ�" << std::endl;
			break;
		}
	}
}*/

void Function(DataBase &db) {
	std::cout << "-----1:��ѯ����" << std::endl;
	std::cout << "-----2:�޸�����" << std::endl;
	std::cout << "-----3:��������" << std::endl;
	std::cout << "-----4:ɾ������" << std::endl;
	std::cout << "-----5:�˳�" << std::endl;

	while (true){
		std::cout << "-----��ѡ���ܣ�";
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
			std::cout << "�������������ѡ��" << std::endl;
			continue;
		}

		break;
	}
}

int main() {
	DataBase myDataBase;
	std::cout << "����������·���� ";
	std::string filepath;

	while (true) {
		std::getline(std::cin, filepath);

		if (!myDataBase.open((char*)filepath.data())) {
			std::cout << "����·��δ�ҵ���" << std::endl;
			std::cout << "��������������·��: ";
		}
		else {
			std::cout << "���ݿ�(" << filepath << ")�򿪳ɹ�" << std::endl;
			break;
		}
	}

	while (true){
		Function(myDataBase);
	}

	return 0;
}