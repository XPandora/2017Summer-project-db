#include "BPlusTreeNode.h"

Node::Node() {
	m_Type = LEAF;
	m_Count = 0;
	m_Parent = NULL;
}

Node::~Node() {
	//deleteChildren();
}

void Node::deleteChildren() {
	if (this->getType() == LEAF) {
		return;
	}

	for (int i = 0; i < m_Count; i++) {
		Node* pNode = this->getPointer(i);
		if (pNode != NULL) {
			pNode->deleteChildren();
		}
		delete pNode;
	}
}

Node* Node::getBrother(DIRECTION &direction) {
	Node* pNode = this->getParent();
	if (pNode == NULL) {
		return NULL;
	}
	Node* bNode;
	for (int i = 0; i < pNode->getCount(); i++) {
		if (pNode->getPointer(i) == this) {
			direction = LEFT;
			return pNode->getPointer(i - 1);
		}
		else {
			direction = RIGHT;
			return pNode->getPointer(i + 1);
		}
	}
}

InternalNode::InternalNode() {
	m_Type = INTERNAL;
	m_Count = 0;
	m_Parent = NULL;
}

InternalNode::~InternalNode() {
	for (int i = 0; i < MAXNUM_POINTER; i++) {
		m_Pointers[i] = NULL;
	}
}

KEY_TYPE InternalNode::getElement(int i) {
	if (i < 0 || i >= MAXNUM_KEY) {//there is some problem
		return INVALID;
	}
	else {
		return m_Keys[i];
	}
}

void InternalNode::setElement(int i, KEY_TYPE value) {
	if (i >= 0 && i < MAXNUM_KEY) {
		m_Keys[i] = value;
	}
}

Node* InternalNode::getPointer(int i) {
	if (i < 0 || i >= MAXNUM_POINTER) {
		return NULL;
	}
	else {
		return m_Pointers[i];
	}
}

void InternalNode::setPointer(int i, Node* pointer) {
	if (i >= 0 && i < MAXNUM_POINTER) {
		m_Pointers[i] = pointer;
	}
}

bool InternalNode::insert(KEY_TYPE value,Node* tNode) {
	if (m_Count >= MAXNUM_KEY) {
		return false;
	}
	int i,j;
	for (i = 0; i < m_Count && m_Keys[i] < value; i++) {

	}

	for (j = m_Count - 1; j >= i; j--) {
		m_Keys[j + 1] = m_Keys[j];
	}

	for (j = m_Count; j >= i + 1; j--) {
		m_Pointers[j + 1] = m_Pointers[j];
	}

	m_Keys[i] = value;
	m_Pointers[i + 1] = tNode;
	tNode->setParent(this);

	m_Count++;
	return true;
}

bool InternalNode::remove(KEY_TYPE value) {
	int i, j;
	for (i = 0; value > m_Keys[i]; i++) {

	}
	for (j = i; j < m_Count - 1; j++) {
		m_Keys[j] = m_Keys[j + 1];
	}
	m_Keys[j] = INVALID;

	for (j = i + 1; j < m_Count; j++) {
		m_Pointers[j] = m_Pointers[j + 1];
	}
	m_Pointers[j] = NULL;

	m_Count--;
	return true;
}

KEY_TYPE InternalNode::split(InternalNode* newNode, KEY_TYPE key){
	int i = 0;
	int j = 0;

	if ((key > this->getElement(ORDER_V - 1)) &&
		key < this->getElement(ORDER_V)) 
	{
		for (i = ORDER_V; i < MAXNUM_KEY; i++) {
			newNode->setElement(j, this->getElement(i));
			this->setElement(i, INVALID);
			j++;
		}

		j = 0;
		for (i = ORDER_V + 1; i < MAXNUM_POINTER + 1; i++) {
			this->getPointer(i)->setParent(newNode);
			newNode->setPointer(j + 1, this->getPointer(i));
			this->setPointer(i, NULL);
			j++;
		}

		this->setCount(ORDER_V);
		newNode->setCount(ORDER_V);

		return key;
	}

	int pos;
	KEY_TYPE reKey;
	if (key < this->getElement(ORDER_V - 1)) {
		pos = ORDER_V;
	}
	else {
		pos = ORDER_V + 1;
	}

	reKey = this->getElement(pos - 1);
	j = 0;
	for (i = pos; i < MAXNUM_KEY; i++) {
		newNode->setElement(j, this->getElement(i));
		this->setElement(j, INVALID);
		j++;
	}

	j = 0;
	for (i = pos + 1; i < MAXNUM_POINTER; i++) {
		this->getPointer(i)->setParent(newNode);
		newNode->setPointer(j + 1, this->getPointer(i));
		this->setPointer(i, NULL);
		j++;
	}

	this->setElement(pos - 1, INVALID);
	this->setCount(pos - 1);
	newNode->setCount(MAXNUM_KEY - pos);

	return reKey;
}

LeafNode::LeafNode() {
	m_Type = LEAF;
	m_Count = 0;
	m_Parent = NULL;
}

LeafNode::~LeafNode() {
	//Empty
}

KEY_TYPE LeafNode::getElement(int i) {
	if (i < 0 || i >= MAXNUM_DATA) {
		return INVALID;
	}
	else {
		return m_Datas[i];
	}
}

void LeafNode::setElement(int i, KEY_TYPE value) {
	if (i >= 0 && i < MAXNUM_DATA) {
		m_Datas[i] = value;
	}
}

bool LeafNode::insert(KEY_TYPE data) {
	if (m_Count >= MAXNUM_DATA) {
		return false;
	}

	int i, j;

	for (i = 0; i < m_Count&&m_Datas[i] < data; i++) {

	}

	for (j = m_Count - 1; j >= i; j--) {
		m_Datas[j + 1] = m_Datas[j];
	}

	m_Datas[i] = data;
	m_Count++;
	return true;
}

bool LeafNode::remove(KEY_TYPE data) {
	int i, j;
	for (i = 0; data < m_Datas[i]; i++) {

	}

	for (j = i; j < m_Count - 1; j++) {
		m_Datas[j] = m_Datas[j + 1];
	}
	m_Datas[j] = INVALID;

	m_Count--;
	return true;
}

KEY_TYPE LeafNode::split(LeafNode* newNode) {
	int i = 0;
	int j = 0;

	for (i = ORDER_V; i < MAXNUM_DATA; i++) {
		newNode->setElement(j, this->getElement(i));
		this->setElement(i, INVALID);
		j++;
	}

	this->setCount(ORDER_V);
	newNode->setCount(ORDER_V);

	return newNode->getElement(0);
}
