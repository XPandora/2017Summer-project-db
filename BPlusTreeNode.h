#ifndef  _BPlusTreeNode_h
#define  _BPlusTreeNode_h

#define ORDER_V 16    /* Ϊ���������v�̶�Ϊ2��ʵ�ʵ�B+��vֵӦ���ǿ���� */

#define MAXNUM_KEY (ORDER_V * 2)    /* �ڲ����������������Ϊ2v */
#define MAXNUM_POINTER (MAXNUM_KEY + 1)    /* �ڲ���������ָ��������ָ�������Ϊ2v+1 */
#define MAXNUM_DATA (ORDER_V * 2)    /* Ҷ�ӽ����������ݸ�����Ϊ2v */

#define INVALID 0
#define NULL 0
typedef int KEY_TYPE;
typedef int INDEX_TYPE;

enum NODE_TYPE {
	ROOT = 1,		//���ڵ�
	INTERNAL = 2, //�ڲ��ڵ�
	LEAF = 3		//Ҷ�ӽڵ�
};

enum DIRECTION {
	D_LEFT = 1,    //����
	D_RIGHT = 2    //�ҷ���
};

class Node {
public:
	Node();
	virtual ~Node();

	NODE_TYPE getType() { return m_Type; }
	void setType(NODE_TYPE type) { m_Type = type; }

	int getCount() { return m_Count; }
	void setCount(int i) { m_Count = i; }

	Node* getParent() { return m_Parent; }
	void setParent(Node * parent) { m_Parent = parent; }

	// ��ȡ������ĳ��Ԫ�أ����м���ָ������Ҷ�ӽ��ָ����
	virtual KEY_TYPE getElement(int i) = 0;
	virtual void setElement(int i, KEY_TYPE value) = 0;

	// ��ȡ������ĳ��ָ�룬���м���ָָ�룬��Ҷ�ӽ��������
	virtual Node* getPointer(int i) = 0;
	virtual void setPointer(int i, Node* pointer) = 0;

	// ��ȡ������ĳ�����������м��������壬��Ҷ�ӽ��ָ����
	virtual INDEX_TYPE getIndex(int i) = 0;
	virtual void setIndex(int i, INDEX_TYPE index) = 0;

	// ��ȡһ��������ֵܽ��
	Node* getBrother(DIRECTION &direction);
	// ��ý������С��Ԫ��
	KEY_TYPE getMinElement();

	// ɾ�����
	void deleteChildren();

protected:

	NODE_TYPE m_Type;
	int m_Count;// ��Ч���ݸ��������м���ָ����������Ҷ�ӽ��ָ���ݸ���
	Node* m_Parent;
};

class InternalNode : public Node {
public:
	InternalNode();
	virtual ~InternalNode();

	virtual KEY_TYPE getElement(int i);
	virtual void setElement(int i, KEY_TYPE value);

	virtual Node* getPointer(int i);
	virtual void setPointer(int i, Node* pointer);

	virtual INDEX_TYPE getIndex(int i) { return INVALID; }
	virtual void setIndex(int i, INDEX_TYPE index) {}

	bool insert(KEY_TYPE data, Node* tNode);
	bool remove(KEY_TYPE data);
	bool combine(Node* bNode);
	bool MoveOneElement(Node* pNode);
	
	KEY_TYPE split(InternalNode* newNode, KEY_TYPE key);

private:
	KEY_TYPE m_Keys[MAXNUM_KEY];//������
	Node *m_Pointers[MAXNUM_POINTER];//ָ������
};

class LeafNode :public Node {
public:
	LeafNode();
	virtual ~LeafNode();

	virtual KEY_TYPE getElement(int i);
	virtual void setElement(int i, KEY_TYPE value);

	virtual Node* getPointer(int i) { return NULL; }
	virtual void setPointer(int i, Node* pointer) {}

	virtual INDEX_TYPE getIndex(int i);
	virtual void setIndex(int i, INDEX_TYPE index);

	bool insert(KEY_TYPE key, INDEX_TYPE index);
	bool remove(KEY_TYPE key);
	bool combine(Node* bNode);
	
	KEY_TYPE split(LeafNode* newNode);

private:
	KEY_TYPE m_Keys[MAXNUM_KEY];
	INDEX_TYPE m_Indexs[MAXNUM_DATA];//��������
};
#endif // !_BPlusTreeNode_h
