#ifndef match_h
#define match_h
#include <stdio.h>
#include "matrix.h"

class match : public Matrix {
public:

	//default constructor
	match();
	//overload inherited constructor
	match(int cols, int rows) :Matrix(cols, rows) {}

	//copy constructor
	match(const match& temp);

	//assignment Operator
	match* operator=(const match& other);

	int getFirstRow();
	int getFirstCol();
	void setFirstRow(int row);
	void setFirstCol(int col);

	//function to overide the matrix variables
	void populate(int numberOfRows, int numberOfCols, double** scene, int code);

protected:
	int firstRow;
	int firstCol;
};
#endif
//16608272