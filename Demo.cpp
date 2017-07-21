#include"BPlusTree.h"
#include <iostream>
using namespace std;

#define _CRTDBG_MAP_ALLOC
void func() {
	BPlusTree tree;
	for (int i = 1; i <= 13; i++) {
		tree.insert(i, i * 100);
	}
	tree.print();
	tree.remove(6);
	//tree.remove(6);
	
	
	//cout << (tree.getRoot()->getPointer(1)->getPointer(1)->getElement(0)) << endl;
	//cout << (tree.getRoot()->getPointer(1)->getElement(0)) << endl;
	//cout << (tree.getRoot()->getPointer(3)->getElement(2)) << endl;
	/*cout << (tree.getRoot()->getPointer(0)->getElement(3)) << endl;
	cout << (tree.getRoot()->getPointer(1)->getElement(1)) << endl;*/
	//cout << tree.check() << endl;
	tree.print();

}

int main() {
	func();
	_CrtDumpMemoryLeaks();
	return 0;
}