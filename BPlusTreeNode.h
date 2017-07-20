#ifndef  _BPlusTreeNode_h
#define  _BPlusTreeNode_h

#define ORDER_V 2    /* Ϊ���������v�̶�Ϊ2��ʵ�ʵ�B+��vֵӦ���ǿ���� */

#define MAXNUM_KEY (ORDER_V * 2)    /* �ڲ����������������Ϊ2v */
#define MAXNUM_POINTER (MAXNUM_KEY + 1)    /* �ڲ���������ָ��������ָ�������Ϊ2v+1 */
#define MAXNUM_DATA (ORDER_V * 2)    /* Ҷ�ӽ����������ݸ�����Ϊ2v */

#define INVALID 0
#define NULL 0
typedef int KEY_TYPE;

enum NODE_TYPE {
	ROOT = 1,		//���ڵ�
	INTERNAL = 2, //�ڲ��ڵ�
	LEAF = 3		//Ҷ�ӽڵ�
};

enum DIRECTION {
	LEFT = 1,
	RIGHT = 2
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

	// ��ȡһ��������ֵܽ��
	Node* getBrother(DIRECTION &direction);

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


	bool insert(KEY_TYPE data, Node* tNode);
	bool remove(KEY_TYPE data);
	
	KEY_TYPE split(InternalNode* newNode, KEY_TYPE key);

protected:
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


	bool insert(KEY_TYPE data);
	bool remove(KEY_TYPE data);
	
	KEY_TYPE split(LeafNode* newNode);
protected:
	KEY_TYPE m_Datas[MAXNUM_DATA];
};
#endif // !1
