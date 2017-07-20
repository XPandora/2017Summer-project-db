#include "BPlusTree.h"
#include <iostream>

BPlusTree::BPlusTree() {
	m_Root = NULL;
	m_Depth = 0;
}

BPlusTree::~BPlusTree() {
	clear();
}

void BPlusTree::clear() {
	Node* root = getRoot();
	if (root != NULL) {
		root->deleteChildren();
		delete root;
	}
	setRoot(NULL);
	m_Depth = 0;
}

bool BPlusTree::search(KEY_TYPE data) {
	Node* tNode = getRoot();
	if (tNode == NULL) {
		return false;
	}

	while (tNode != NULL) {
		if (tNode->getType() == LEAF) {
			break;
		}

		if (data < tNode->getElement(0)) {
			tNode = tNode->getPointer(0);
			continue;
		}

		for (int i = 0; i < tNode->getCount(); i++) {
			if (i == tNode->getCount() - 1 && data > tNode->getElement(i)) {
				tNode = tNode->getPointer(i + 1);
				break;
			}
			if (data < tNode->getElement(i + 1)) {
				tNode = tNode->getPointer(i + 1);
				break;
			}
		}

	}

	for (int i = 0; i < tNode->getCount(); i++) {
		if (data == tNode->getElement(i)) {
			return true;
		}
	}

	return false;
}

bool BPlusTree::insert(KEY_TYPE data) {
	if (search(data)) {
		return false;
	}

	LeafNode* oldNode = searchInsertNode(data);

	if (oldNode == NULL) {
		oldNode = new LeafNode();
		oldNode->insert(data);
		setRoot(oldNode);
		return true;
	}
	
	if (oldNode->getCount() < MAXNUM_DATA) {
		return oldNode->insert(data);
	}

	LeafNode* newNode = new LeafNode();
	KEY_TYPE key;

	key = oldNode->split(newNode);
	if (data < key) {
		oldNode->insert(data);
	}
	else {
		newNode->insert(data);
	}

	InternalNode* parentNode = (InternalNode*)(oldNode->getParent());

	if (parentNode == NULL) {
		InternalNode* newParentNode = new InternalNode();

		newParentNode->setElement(0, key);
		newParentNode->setPointer(0, oldNode);
		newParentNode->setPointer(1, newNode);
		newParentNode->setCount(1);

		oldNode->setParent(newParentNode);
		newNode->setParent(newParentNode);

		this->setRoot(newParentNode);
		return true;
	}

	return insertToInternal(parentNode, key, newNode);
}

bool BPlusTree::remove(KEY_TYPE data) {
	if (search(data)) {
		return false;
	}
}

void BPlusTree::print() {
	Node* root = getRoot();
	printNode(root);
}

LeafNode* BPlusTree::searchInsertNode(KEY_TYPE data) {
	Node* tNode = getRoot();

	if (tNode == NULL) {
		return NULL;
	}

	while (tNode != NULL) {
		if (tNode->getType() == LEAF) {
			break;
		}

		if (data < tNode->getElement(0)) {
			tNode = tNode->getPointer(0);
			continue;
		}

		for (int i = 0; i < tNode->getCount(); i++) {
			if (i == tNode->getCount() - 1 && data > tNode->getElement(i)) {
				tNode = tNode->getPointer(i + 1);
				break;
			}
			if (data < tNode->getElement(i + 1)) {
				tNode = tNode->getPointer(i + 1);
				break;
			}
		}
	}

	return (LeafNode*)tNode;
}

bool BPlusTree::insertToInternal(InternalNode* pNode, KEY_TYPE key, Node* rChildren) {
	if (pNode == NULL) {
		return false;
	}

	if (pNode->getCount() < MAXNUM_KEY) {
		return pNode->insert(key, rChildren);
	}

	KEY_TYPE reKey;
	InternalNode* pNewNode = new InternalNode();

	reKey = pNode->split(pNewNode, key);

	if (key < reKey) {
		pNode->insert(key, rChildren);
	}
	else if (key > reKey){
		pNewNode->insert(key, rChildren);
	}
	else {
		pNewNode->setPointer(0, rChildren);
		rChildren->setParent(pNewNode);
	}

	InternalNode* pParentNode = (InternalNode*)pNode->getParent();
	
	if (pParentNode == NULL) {
		InternalNode* newParentNode = new InternalNode();
		
		newParentNode->setElement(0, reKey);
		newParentNode->setPointer(0, pNode);
		newParentNode->setPointer(1, pNewNode);
		newParentNode->setCount(1);

		pNode->setParent(newParentNode);
		pNewNode->setParent(newParentNode);

		this->setRoot(newParentNode);
		return true;
	}

	return insertToInternal(pParentNode, reKey, pNewNode);
}

bool BPlusTree::check() {
	Node* root = getRoot();
	return checkNode(root);
}

bool BPlusTree::checkNode(Node* tNode) {
	if (tNode == NULL) {
		return true;
	}

	if (tNode != getRoot() && tNode->getCount() < ORDER_V) {
		return false;
	}

	for (int i = 0; i < tNode->getCount() - 1; i++) {
		if (tNode->getElement(i + 1) < tNode->getElement(i)) {
			return false;
		}
	}

	if (tNode->getType() == LEAF) {
		return true;
	}

	for (int i = 0; i < tNode->getCount() + 1; i++) {
		if (checkNode(tNode->getPointer(i)) == false) {
			return false;
		}
	}

	return true;
}

void BPlusTree::printNode(Node* tNode) {
	if (tNode == NULL) {
		return;
	}

	if (tNode->getType() == LEAF) {
		for (int i = 0; i < tNode->getCount(); i++) {
			std::cout << tNode->getElement(i) << "|";
		}
		return;
	}

	for (int i = 0; i < tNode->getCount() + 1; i++) {
		printNode(tNode->getPointer(i));
	}
}
