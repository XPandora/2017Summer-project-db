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

bool BPlusTree::insert(KEY_TYPE data, INDEX_TYPE index) {
	if (search(data)) {
		return false;
	}

	LeafNode* oldNode = searchInsertNode(data);

	if (oldNode == NULL) {
		oldNode = new LeafNode();
		oldNode->insert(data, index);
		setRoot(oldNode);
		return true;
	}
	
	if (oldNode->getCount() < MAXNUM_DATA) {
		return oldNode->insert(data, index);
	}

	LeafNode* newNode = new LeafNode();
	KEY_TYPE key;

	key = oldNode->split(newNode);
	if (data < key) {
		oldNode->insert(data, index);
	}
	else {
		newNode->insert(data, index);
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
	if (!search(data)) {
		return false;
	}

	LeafNode* oldNode = searchInsertNode(data);

	if (oldNode == NULL) {
		return false;
	}

	oldNode->remove(data);
	InternalNode* pNode = (InternalNode*)oldNode->getParent();

	if (pNode == NULL) {
		if (oldNode->getCount() == 0) {
			delete oldNode;
			setRoot(NULL);
		}
		return true;
	}

	if (oldNode->getCount() >= ORDER_V) {
		int i;

		for (i = 0; data > pNode->getElement(i) && i < pNode->getCount(); i++) {

		}

		if (data == pNode->getElement(i)) {
			pNode->setElement(i, oldNode->getElement(0));
		}

		return true;
	}

	DIRECTION direction;
	LeafNode* brotherNode = (LeafNode*)oldNode->getBrother(direction);

	KEY_TYPE newData;
	INDEX_TYPE newIndex;
	if (brotherNode->getCount() > ORDER_V) {
		if (direction == D_LEFT) {
			newData = brotherNode->getElement(brotherNode->getCount() - 1);
			newIndex = brotherNode->getIndex(brotherNode->getCount() - 1);
		}
		else {
			newData = brotherNode->getElement(0);
			newIndex = brotherNode->getIndex(0);
		}

		oldNode->insert(newData,newIndex);
		brotherNode->remove(newData);

		if (direction == D_LEFT) {
			for (int i = 0; i <= pNode->getCount(); i++) {
				if (pNode->getPointer(i) == oldNode&&i > 0) {
					pNode->setElement(i - 1, oldNode->getElement(0));
				}
			}
		}
		else {
			for (int i = 0; i <= pNode->getCount(); i++) {
				if (pNode->getPointer(i) == brotherNode&&i > 0) {
					pNode->setElement(i - 1, brotherNode->getElement(0));
				}
			}
		}

		return true;

	}

	KEY_TYPE newKey;

	if (direction == D_LEFT) {
		brotherNode->combine(oldNode);
		newKey = oldNode->getElement(0);
	}
	else {
		oldNode->combine(brotherNode);
		newKey = brotherNode->getElement(0);
	}

	return deleteInternalNode(pNode, newKey);
}

void BPlusTree::print() {
	Node* root = getRoot();
	printNode(root);
	std::cout << std::endl;
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

bool BPlusTree::deleteInternalNode(InternalNode* pNode, KEY_TYPE key) {
	if (!pNode->remove(key)) {
		return false;
	}

	InternalNode* pParentNode = (InternalNode*)pNode->getParent();

	if (pParentNode == NULL) {
		if (pNode->getCount() == 0) {
			setRoot(pNode->getPointer(0));
			delete pNode;
		}
		return true;
	}

	if (pNode->getCount() >= ORDER_V) {
		int i;
		for (i = 0; key > pParentNode->getElement(i) && i < pParentNode->getCount(); i++) {

		}

		if (key == pParentNode->getElement(i)) {
			pParentNode->setElement(i, pNode->getElement(0));
		}

		return true;
	}

	DIRECTION direction;
	InternalNode* pBrotherNode = (InternalNode*)pNode->getBrother(direction);

	KEY_TYPE newData;
	if (pBrotherNode->getCount() > ORDER_V) {
		pNode->MoveOneElement(pBrotherNode);

		if (direction == D_LEFT) {
			for (int i = 0; i < pParentNode->getCount() + 1; i++) {
				if (pParentNode->getPointer(i) == pNode&&i > 0) {
					pParentNode->setElement(i - 1, pNode->getElement(0));
				}
			}
		}
		else {
			for (int i = 0; i < pParentNode->getCount() + 1; i++) {
				if (pParentNode->getPointer(i) == pNode&&i > 0) {
					pParentNode->setElement(i - 1, pNode->getElement(0));
				}

				if (pParentNode->getPointer(i) == pBrotherNode&&i > 0) {
					pParentNode->setElement(i - 1, pBrotherNode->getElement(0));
				}
			}

		}
		return true;
	}

	KEY_TYPE newKey;

	if (direction == D_LEFT) {
		pBrotherNode->combine(pNode);
		newKey = pNode->getPointer(0)->getElement(0);
		delete pNode;
	}
	else {
		pNode->combine(pBrotherNode);
		newKey = pBrotherNode->getPointer(0)->getElement(0);
		delete pBrotherNode;
	}

	return deleteInternalNode(pParentNode, newKey);
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
			std::cout << tNode->getElement(i) << ":" << tNode->getIndex(i) << "|";
		}
		return;
	}

	for (int i = 0; i < tNode->getCount() + 1; i++) {
		printNode(tNode->getPointer(i));
	}
}
