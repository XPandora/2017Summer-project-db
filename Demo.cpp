#include"dbSystem.h"
#include <iostream>
#include <ctime>
using namespace std;

#define _CRTDBG_MAP_ALLOC
void func() {
	DataBase db;
	cout << db.open("data") << endl;

	clock_t start, finish;
	start = clock();

	for (int i = 1; i <= 100; i++) {
		db.store(i, i);
	}

	finish = clock();

	cout << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;
}

int main1() {
	func();
	_CrtDumpMemoryLeaks();
	return 0;
}