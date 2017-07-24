#ifndef _record_h
#define _record_h

typedef int OFFSET_TYPE; //Æ«ÒÆÁ¿

class Record {

public:
	Record();
	Record(char*record);
	Record(int newID, int newScore);

	void clear();

	int getID() { return ID; }
	int getScore(){ return score; }

	void setScore(int newScore) { score = newScore; }
private:
	int ID;
	int score;
};
#endif // !_record_h

