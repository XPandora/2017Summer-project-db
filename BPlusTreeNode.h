#ifndef  _BPlusTreeNode_h
#define  _BPlusTreeNode_h

#define ORDER_V 16    /* 为简单起见，把v固定为2，实际的B+树v值应该是可配的 */

#define MAXNUM_KEY (ORDER_V * 2)    /* 内部结点中最多键个数，为2v */
#define MAXNUM_POINTER (MAXNUM_KEY + 1)    /* 内部结点中最多指向子树的指针个数，为2v+1 */
#define MAXNUM_DATA (ORDER_V * 2)    /* 叶子结点中最多数据个数，为2v */

#define INVALID 0
#define NULL 0
typedef int KEY_TYPE;
typedef int INDEX_TYPE;

enum NODE_TYPE {
	ROOT = 1,		//根节点
	INTERNAL = 2, //内部节点
	LEAF = 3		//叶子节点
};

enum DIRECTION {
	D_LEFT = 1,    //左方向
	D_RIGHT = 2    //右方向
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

	// 获取和设置某个元素，对中间结点指键，对叶子结点指数据
	virtual KEY_TYPE getElement(int i) = 0;
	virtual void setElement(int i, KEY_TYPE value) = 0;

	// 获取和设置某个指针，对中间结点指指针，对叶子结点无意义
	virtual Node* getPointer(int i) = 0;
	virtual void setPointer(int i, Node* pointer) = 0;

	// 获取和设置某个索引，对中间结点无意义，对叶子结点指索引
	virtual INDEX_TYPE getIndex(int i) = 0;
	virtual void setIndex(int i, INDEX_TYPE index) = 0;

	// 获取一个最近的兄弟结点
	Node* getBrother(DIRECTION &direction);
	// 获得结点中最小的元素
	KEY_TYPE getMinElement();

	// 删除结点
	void deleteChildren();

protected:

	NODE_TYPE m_Type;
	int m_Count;// 有效数据个数，对中间结点指键个数，对叶子结点指数据个数
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
	KEY_TYPE m_Keys[MAXNUM_KEY];//键数组
	Node *m_Pointers[MAXNUM_POINTER];//指针数组
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
	INDEX_TYPE m_Indexs[MAXNUM_DATA];//索引数组
};
#endif // !_BPlusTreeNode_h
