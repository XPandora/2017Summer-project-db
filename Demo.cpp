#include"BPlusTree.h"
#include <iostream>
using namespace std;

#define _CRTDBG_MAP_ALLOC
void func() {
	BPlusTree tree;
	for (int i = 1; i <= 20; i++) {
		tree.insert(i);
	}
	
	
	cout << (tree.getRoot()->getPointer(2)->getPointer(2)->getElement(2)) << endl;
	//cout << (tree.getRoot()->getPointer(3)->getElement(1)) << endl;
	//cout << (tree.getRoot()->getPointer(3)->getElement(2)) << endl;
	/*cout << (tree.getRoot()->getPointer(0)->getElement(3)) << endl;
	cout << (tree.getRoot()->getPointer(1)->getElement(1)) << endl;*/
	tree.print();

}

int main() {
	func();
	_CrtDumpMemoryLeaks();
	return 0;
}