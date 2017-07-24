#include "record.h"
#include <sstream>

Record::Record() {
	ID = 0;
	score = 0;
}
Record::Record(char* record) {
	if (record[0] == ' ') {
		ID = 0;
		score = 0;
	}
	else {
		std::stringstream ss;
		ss << record;
		ss >> ID;
		ss >> score;
	}
}

Record::Record(int newID, int newScore) {
	this->ID = newID;
	this->score = newScore;
}

void Record::clear() {
	this->ID = 0;
	this->score = 0;
}