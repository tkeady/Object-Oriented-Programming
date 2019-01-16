#include "largeImage.h"
#include <iostream>

//large image class
largeImage::largeImage() {
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

//draws 4 pixel line around Wally
double* largeImage::draw(int finalRow, int finalCol, double* cluttered_scene_input_data, int wallyRows, int wallyCols, int clutteredCols) {

	//loop to go through each row and column to edit the data to draw on it to display result location
	for (int rowAmount = finalRow; rowAmount <= finalRow + wallyRows; rowAmount++) {
		for (int colAmount = finalCol; colAmount <= finalCol + wallyCols; colAmount++) {
			if (rowAmount == finalRow || rowAmount == finalRow + wallyRows) {
				for (int x = 0; x < 5; x++) {
					cluttered_scene_input_data[clutteredCols * (rowAmount - x) + (colAmount - x)] = 0;
				}
			}
			if (colAmount == finalCol || colAmount == finalCol + wallyCols) {
				for (int x = 0; x < 5; x++) {
					cluttered_scene_input_data[clutteredCols * (rowAmount - x) + (colAmount - x)] = 0;
				}
			}
		}
	}
	return cluttered_scene_input_data;
}
//16608272