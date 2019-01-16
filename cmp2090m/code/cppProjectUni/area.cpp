#include "area.h"
#include <iostream>

using namespace std;

//get NNS of matrix
float area::getSSD() {
	return SSD;
}

//get NNS score of matrix
void area::setSSD(int NNSscore) {
	SSD = NNSscore;
}

void area::setFirstRow(int row) {
	firstRow = row;
}

void area::setFirstCol(int col) {
	firstCol = col;
}

int area::getRow() {
	return firstRow;
}

int area::getCol() {
	return firstCol;
}

//16608272