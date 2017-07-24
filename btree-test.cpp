#include"BPlusTree.h"
#include<iostream>

using namespace std;

int btree() {
	BPlusTree tree;
	for (int i = 1; i <= 1000; i++) {
		tree.insert(i, i);
	}
	
	return 0;
}