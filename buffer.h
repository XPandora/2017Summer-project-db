#ifndef _cache_h
#define _cache_h

#include "record.h"
#include  <vector>
#define MAXNUM_BUFFER 10000

enum OP_TYPE {
	INSERT = 1,
	MODIFY = 2,
	REMOVE = 3
};

class BufferToken {
public:
	BufferToken() {}
	BufferToken(Record record, OP_TYPE type, int order) {
		b_Record = record;
		b_Type = type;
		b_Order = order;
	}

	OP_TYPE getType() { return b_Type; }
	void setType(OP_TYPE type) { b_Type = type; }
	
	Record getBufferRecord() { return b_Record; }
	void setBufferRecord(Record record) { b_Record = record; }

	int getOrder() { return b_Order; }
	void setOrder(int order) { b_Order = order; }

private:

	OP_TYPE b_Type;
	Record b_Record;
	int b_Order;
};

class Buffer {
public:
	Buffer() {
		m_Count = 0;
	}
	~Buffer() {
		clear();
	}

	void clear() {
		m_Count = 0;
		m_Tokens.clear();
	}

	int getCount() { return m_Count; }
	void setCount(int count) { m_Count = count; }

	void add(BufferToken token) {
		m_Count++;
		m_Tokens.push_back(token);
	}

	BufferToken getToken(int position) { if (0 <= position&&position < m_Count)return m_Tokens[position]; }
private:

	int m_Count;
	std::vector<BufferToken> m_Tokens;
};
#endif // !_cache_h

