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
			delete pNode;
		}
	}
}

Node* Node::getBrother(DIRECTION &direction) {
	Node* pNode = this->getParent();

	if (pNode == NULL) {
		return NULL;
	}
	Node* bNode;

	for (int i = 0; i < pNode->getCount() + 1; i++) {
		if (pNode->getPointer(i) == this) {
			if (i == pNode->getCount()) {
				direction = D_LEFT;
				return pNode->getPointer(i - 1);
			}
			else if(i == 0){
				direction = D_RIGHT;
				return pNode->getPointer(i + 1);
			}
			else {
				Node* rBrother = pNode->getPointer(i + 1);
				Node* lBrother = pNode->getPointer(i - 1);
				if (rBrother->getCount() > lBrother->getCount()) {
					direction = D_RIGHT;
					return rBrother;
				}
				else {
					direction = D_LEFT;
					return lBrother;
				}
			}
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

	if (m_Keys[i] != value) {
		return false;
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
		(key < this->getElement(ORDER_V)) )
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
		this->setElement(i, INVALID);
		j++;
	}

	j = 0;
	for (i = pos; i < MAXNUM_POINTER; i++) {
		this->getPointer(i)->setParent(newNode);
		newNode->setPointer(j, this->getPointer(i));
		this->setPointer(i, NULL);
		j++;
	}

	this->setElement(pos - 1, INVALID);
	this->setCount(pos - 1);
	newNode->setCount(MAXNUM_KEY - pos);

	return reKey;
}

bool InternalNode::combine(Node* bNode) {
	if (this->getCount() + bNode->getCount() + 1 > MAXNUM_KEY) {
		return false;
	}

	KEY_TYPE newKey = bNode->getMinElement();

	m_Keys[m_Count] = newKey;
	m_Count++;
	m_Pointers[m_Count] = bNode->getPointer(0);
	bNode->getPointer(0)->setParent(this);

	for (int i = 0; i < bNode->getCount(); i++) {
		m_Keys[m_Count] = bNode->getElement(i);
		m_Count++;
		m_Pointers[m_Count] = bNode->getPointer(i + 1);
		bNode->getPointer(i + 1)->setParent(this);
	}

	return true;
}

bool InternalNode::MoveOneElement(Node* pNode) {
	if (this->getCount() >= MAXNUM_KEY) {
		return false;
	}

	int i, j;

	if (pNode->getElement(0) < this->getElement(0) ){
		for (i = m_Count; i > 0; i--) {
			m_Keys[i] = m_Keys[i - 1];
		}
		
		for (j = m_Count + 1; j > 0; j--) {
			m_Pointers[j] = m_Pointers[j - 1];
		}

		m_Keys[0] = getMinElement();
		m_Pointers[0] = pNode->getPointer(pNode->getCount());

		pNode->getPointer(pNode->getCount())->setParent(this);

		pNode->setElement(pNode->getCount() - 1, INVALID);
		pNode->setPointer(pNode->getCount(), NULL);

	}
	else {
		m_Keys[m_Count] = pNode->getMinElement();
		m_Pointers[m_Count + 1] = pNode->getPointer(0);

		pNode->getPointer(0)->setParent(this);

		for (i = 0; i < pNode->getCount() - 1; i++) {
			pNode->setElement(i, pNode->getElement(i + 1));
		}

		for (j = 0; j < pNode->getCount(); j++) {
			pNode->setPointer(j, pNode->getPointer(j + 1));
		}

		pNode->setElement(pNode->getCount() - 1, INVALID);
		pNode->setPointer(pNode->getCount(), NULL);

	}

	this->setCount(this->getCount() + 1);
	pNode->setCount(pNode->getCount() - 1);

	return true;
}

KEY_TYPE Node::getMinElement() {
	if (this == NULL || m_Count == 0) {
		return INVALID;
	}

	Node* nextNode = this->getPointer(0);

	if (nextNode->getType() == LEAF) {
		return nextNode->getElement(0);
	}
	else {
		Node* nextNode = this->getPointer(0);
		return nextNode->getMinElement();
	}
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
	if (i < 0 || i >= MAXNUM_KEY) {
		return INVALID;
	}
	else {
		return m_Keys[i];
	}
}

void LeafNode::setElement(int i, KEY_TYPE value) {
	if (i >= 0 && i < MAXNUM_KEY) {
		m_Keys[i] = value;
	}
}

INDEX_TYPE LeafNode::getIndex(int i) {
	if (i < 0 || i >= MAXNUM_DATA) {
		return INVALID;
	}
	else {
		return m_Indexs[i];
	}
}

void LeafNode::setIndex(int i, INDEX_TYPE index) {
	if (i >= 0 && i < MAXNUM_DATA) {
		m_Indexs[i] = index;
	}
}

bool LeafNode::insert(KEY_TYPE key, INDEX_TYPE index) {
	if (m_Count >= MAXNUM_DATA) {
		return false;
	}

	int i, j;

	for (i = 0; i < m_Count&&m_Keys[i] < key; i++) {

	}

	for (j = m_Count - 1; j >= i; j--) {
		m_Keys[j + 1] = m_Keys[j];
		m_Indexs[j + 1] = m_Indexs[j];
	}

	m_Keys[i] = key;
	m_Indexs[i] = index;
	m_Count++;
	return true;
}

bool LeafNode::remove(KEY_TYPE key) {
	int i, j;
	for (i = 0; key > m_Keys[i]; i++) {

	}

	for (j = i; j < m_Count - 1; j++) {
		m_Keys[j] = m_Keys[j + 1];
		m_Indexs[j] = m_Indexs[j + 1];
	}

	m_Keys[j] = INVALID;
	m_Indexs[j] = INVALID;

	m_Count--;
	return true;
}

bool LeafNode::combine(Node*bNode) {
	if (this->getCount() + bNode->getCount() > MAXNUM_DATA) {
		return false;
	}

	for (int i = 0; i < bNode->getCount(); i++) {
		this->insert(bNode->getElement(i), bNode->getIndex(i));
	}

	return true;
}

KEY_TYPE LeafNode::split(LeafNode* newNode) {
	int i = 0;
	int j = 0;

	for (i = ORDER_V; i < MAXNUM_DATA; i++) {
		newNode->setElement(j, this->getElement(i));
		newNode->setIndex(j, this->getIndex(i));
		
		this->setElement(i, INVALID);
		this->setIndex(i, INVALID);

		j++;
	}

	this->setCount(ORDER_V);
	newNode->setCount(ORDER_V);

	return newNode->getElement(0);
}
