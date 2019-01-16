#ifndef largeImage_h
#define largeImage_h
#include <stdio.h>
#include "matrix.h"

class largeImage : public Matrix {
public:
	//constructor
	largeImage(int cols, int rows) :Matrix(cols, rows) {}
	largeImage();

	//draw line on the images
	double* draw(int finalRow, int finalCol, double* cluttered_scene_input_data, int wallyRows, int wallyCols, int clutteredCols);
};
#endif
//16608272