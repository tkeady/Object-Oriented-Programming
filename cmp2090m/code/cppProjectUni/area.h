#ifndef area_h
#define area_h
#include <stdio.h>
#include "matrix.h"

class area : public Matrix {
public:

	area(int cols, int rows) :Matrix(cols, rows) {}

	void setSSD(int NNSscore);
	float getSSD();
	void setFirstCol(int col);
	void setFirstRow(int row);
	int getCol();
	int getRow();
	
protected:
	float SSD;
	int firstCol;
	int firstRow;
};
#endif
//16608272