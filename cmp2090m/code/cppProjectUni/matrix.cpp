#include <iostream>
#include "matrix.h"
#include "math.h"

//overload constructor to specify the matrix dimensions
Matrix::Matrix(int numRows, int numCols) {
	rows = numRows;
	cols = numCols;
	matrixArray = new double*[rows];

	for (int i = 0; i < rows; ++i) {
		matrixArray[i] = new double[cols];
		for (int j = 0; j < cols; ++j) {
			matrixArray[i][j] = 0.0;
		}
	}
}

//assignment operator to create identical matrices
Matrix* Matrix::operator=(const Matrix& lastMatrix) {

	matrixArray = new double*[rows];

	for (int i = 0; i < rows; ++i) {
		matrixArray[i] = new double[cols];
		for (int j = 0; j < cols; ++j) {
			matrixArray[i][j] = lastMatrix.getMatrixArray()[i][j];
		}
	}
	return this;
}

//matrix destructor
Matrix::~Matrix() {

	//delete array elements
	for (int i = 0; i < rows; ++i) {
		delete matrixArray[i];
	}
	delete[] matrixArray;
}

//returns pointer to a 1D array of matrix area
double* Matrix::getArea(int row, int col, int numberRows, int numberCols) {

	double* area = new double[numberRows * numberCols];
	int count = 0;

	//add values into the 1D array
	for (int rowAmount = row; rowAmount < row + numberRows; rowAmount++) {
		for (int colAmount = col; colAmount < col + numberCols; colAmount++) {
			area[count] = matrixArray[rowAmount][colAmount];
			count++;
		}
	}
	return area;
}

//returns the matrix array
double** Matrix::getMatrixArray() const {
	return matrixArray;
}

//sets the pixel in the array
void Matrix::setPixel(int row, int col, int code) {
	matrixArray[row][col] = code;
}

//constructor creates a 1x1 matrix
Matrix::Matrix() {
	rows = 1;
	cols = 1;

	matrixArray = new double*[rows];

	for (int i = 0; i < rows; ++i) {
		matrixArray[i] = new double[cols];
		for (int j = 0; j < cols; ++j) {
			matrixArray[i][j] = 0.0;
		}
	}
}

//calculates the SSD for a big image and a sub image
double Matrix::calculateSSD(double* wallyMatrix, double* sceneMatrix, int wallyRows, int wallyCols) {

	double SSD = 0.0, diff = 0.0;

	for (int i = 0; i < wallyRows * wallyCols; i++) {
		if (wallyMatrix[i] != 255) {
			diff = wallyMatrix[i] - sceneMatrix[i];
			SSD += diff * diff;
		}
	}
	return SSD;
}

//calculates the NCC of a big image and a sub image
double Matrix::calculateNCC(double* wallyMatrix, double* sceneMatrix, int wallyRows, int wallyCols) {

	//initial variables
	double pixelTotalScene = 0.0, pixelTotalWally = 0.0, NCC = 0.0, averageWally = 0.0, averageScene = 0.0, topLine = 0.0;

	//deep copy of wallyMatrix to a new temporary wallyMatrix to prevent pointer overwrite
	double* tempWallyMatrix = new double[wallyRows * wallyCols];

	for (int ii = 0; ii < wallyRows * wallyCols; ii++) {
		tempWallyMatrix[ii] = wallyMatrix[ii];
	}

	//calculate sum of scene and wally matrices
	for (int y = 0; y < wallyRows * wallyCols; y++) {
		pixelTotalWally += tempWallyMatrix[y];
		pixelTotalScene += sceneMatrix[y];
	}

	//get averages of the sections
	averageWally = (1 / (wallyCols * wallyRows)) * pixelTotalWally;
	averageScene = (1 / (wallyCols * wallyRows)) * pixelTotalScene;

	//take average of each pixel and multiply 
	for (int y = 0; y < wallyRows * wallyCols; y++) {
		tempWallyMatrix[y] -= averageWally;
		sceneMatrix[y] -= averageScene;
		topLine += sceneMatrix[y] * tempWallyMatrix[y];
	}

	//delete array
	delete[] tempWallyMatrix;

	//compute NCC
	NCC = topLine / sqrt((pixelTotalWally * pixelTotalWally) * (pixelTotalScene * pixelTotalScene));

	return NCC;
}

//keep the single element stored
void Matrix::individualElement(int row, int col)
{
}

//print the matrix
void Matrix::matrixPrint() {

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			std::cout << matrixArray[row][col] << " ";
		}
		std::cout << "\n";
	}
}

//return SSD of object
double Matrix::getSSD() {
	return SSD;
}

//SSD score of matrix
void Matrix::setSSD(double SSDscore) {
	SSD = SSDscore;
}

//return NCC of the object
double Matrix::getNCC() {
	return NCC;
}

//method to set NCC score of matrix
void Matrix::setNCC(double NCCscore) {
	NCC = NCCscore;
}
//16608272