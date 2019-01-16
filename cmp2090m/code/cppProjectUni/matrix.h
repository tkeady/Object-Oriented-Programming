#ifndef matrix_h
#define matrix_h
#include <stdio.h>
#include <array>

//matrix class
class Matrix {
public:

	//matrix constructors and overload constructor
	Matrix();
	Matrix(int numRows, int numCols);

	//deconstructor
	~Matrix();

	//operator
	Matrix* operator=(const Matrix& other);

	//prints matrix
	void matrixPrint();

	//gets individual elements
	void individualElement(int row, int col);

	//set individual pixels
	void setPixel(int row, int col, int code);

	//return the Matrix
	double* getArea(int row, int col, int rows, int cols);

	//calculate the scores
	double calculateSSD(double* wallyMatrix, double* sceneMatrix, int wallyRows, int wallyCols);
	double calculateNCC(double* wallyMatrix, double* sceneMatrix, int wallyRows, int wallyCols);

	//methods for the SSD
	void setSSD(double NNSscore);
	double getSSD();

	//methods for the NC
	void setNCC(double NCCscore);
	double getNCC();

	double ** getMatrixArray() const;

protected:

	//structure of Matrix
	int rows;
	int cols;

	//scores
	double SSD;
	double NCC;

	//definition of array rows
	double** matrixArray;
};
#endif
//16608272