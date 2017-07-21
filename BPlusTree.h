#ifndef _BPlusTree_h
#define _BPlusTree_h

#include "BPlusTreeNode.h"

class BPlusTree {
public:
	BPlusTree();
	~BPlusTree();

	bool search(KEY_TYPE data);
	bool insert(KEY_TYPE data, INDEX_TYPE index);
	bool remove(KEY_TYPE data);

	void print();
	void clear();

	void setRoot(Node* root) { m_Root = root; }
	Node* getRoot() { return m_Root; }

	int getDepth() { return m_Depth; }
	void incDepth() { m_Depth++; }
	void decDepth() { m_Depth--; }

	bool check();

private:
	Node* m_Root;
	int m_Depth;

	//Ϊ���������Ҷ�ӽڵ�
	LeafNode* searchInsertNode(KEY_TYPE data);
	//��������м�ڵ�
	bool insertToInternal(InternalNode* pNode, KEY_TYPE key, Node* rChildren);
	//���м�ڵ�ɾ����
	bool deleteInternalNode(InternalNode* pNode, KEY_TYPE key);
	bool checkNode(Node* tNode);
	void printNode(Node* tNode);
};
#endif // !_BPlusTree_h
