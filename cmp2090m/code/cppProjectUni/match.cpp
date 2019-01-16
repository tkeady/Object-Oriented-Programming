#include "match.h"
#include "largeImage.h"
#include <iostream>

//constructor default if not using the constructor
match::match() {
	rows = 49;
	cols = 36;

	matrixArray = new double*[rows];

	for (int i = 0; i < rows; ++i) {
		matrixArray[i] = new double[cols];
		for (int j = 0; j < cols; ++j) {
			matrixArray[i][j] = 0.0;
		}
	}
}

//copy constructor to create identical matrix
match::match(const match& temp) {
	rows = 49;
	cols = 36;

	matrixArray = new double*[rows];

	for (int i = 0; i < rows; ++i) {
		matrixArray[i] = new double[cols];
		for (int j = 0; j < cols; ++j) {
			matrixArray[i][j] = temp.getMatrixArray()[i][j];
		}
	}
}

//operator to create identical matrix
match* match::operator=(const match& previousMatrix) {

	matrixArray = new double*[rows];

	for (int i = 0; i < rows; ++i) {
		matrixArray[i] = new double[cols];
		for (int j = 0; j < cols; ++j) {
			matrixArray[i][j] = previousMatrix.getMatrixArray()[i][j];
		}
	}
	return this;
}

int match::getFirstRow() {
	return firstRow;
}

int match::getFirstCol() {
	return firstCol;
}

void match::setFirstRow(int row) {
	firstRow = row;
}

void match::setFirstCol(int col) {
	firstCol = col;
}

//find and populate array
void match::populate(int numberOfRows, int numberOfCols, double** scene, int code) {
	for (int rowCount = firstRow; rowCount < firstRow + numberOfRows; rowCount++) {
		for (int colCount = firstCol; colCount < firstCol + numberOfCols; colCount++) {
			matrixArray[rowCount][colCount] = code;
		}
	}
}
//16608272