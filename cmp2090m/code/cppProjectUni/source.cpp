#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <algorithm>
#include <string>
#include "matrix.h"
#include "match.h"
#include "largeImage.h"

//read .txt file and converts it into 1D array of data type double of size R*C
double* readTXT(std::string fileName, int sizeR, int sizeC);

//convert 1D array of double to .pgm image format. *info* Use Q = 255 for greyscale images and Q=1 for binary images.
void WritePGM(std::string filename, double *data, int sizeR, int sizeC, int Q);

//function to convert the pixels from the file into an object
void convertPixels(int rows, int cols, double* data, match*& image);

//overload function for the convertPixels function
void convertPixels(int rows, int cols, double* data, largeImage*& image);

//get the user algorithm choice
int getAlgorithm();

//get user search method
int getSearchMethod();


//convert data from double to .pgm
void WritePGM(std::string filename, double *data, int sizeR, int sizeC, int Q) {

	int i;
	unsigned char *image;
	std::ofstream myFile;
	image = (unsigned char *) new unsigned char[sizeR*sizeC];

	//convert int values to unsigned char

	for (i = 0; i < sizeR*sizeC; i++)
		image[i] = (unsigned char)data[i];

	myFile.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!myFile) {
		std::cout << "Cannot open file: " << filename << std::endl;
		exit(1);
	}

	myFile << "P5" << std::endl;
	myFile << sizeC << " " << sizeR << std::endl;
	myFile << Q << std::endl;

	myFile.write(reinterpret_cast<char *>(image), (sizeR*sizeC) * sizeof(unsigned char));

	if (myFile.fail()) {
		std::cout << "Cannot write image " << filename << std::endl;
		exit(0);
	}
	myFile.close();
	delete[] image;
}
int getAlgorithm() {
	int algorithmChoice;
	bool valid = false;

	//error handling to validate user input
	while (valid != true) {
		try {
			std::cout << "----------------------------------------------------------" << std::endl;
			std::cout << "Please select which Image Search Algorithm to use" << std::endl;
			std::cout << "1 - Sum of Squared Differences" << std::endl;
			std::cout << "2 - Normalised Cross Correlation" << std::endl;
			std::cout << "----------------------------------------------------------" << std::endl;

			//user input
			std::cin >> algorithmChoice;

			//is input valid
			if ((algorithmChoice != 1) && (algorithmChoice != 2)) {
				throw 1;
			}
			else {
				throw 2;
			}
			//catch error message or break the loop if it is valid
		}
		catch (int errorNumber) {
			if (errorNumber == 1) {
				std::cout << "Invalid input. Please try again" << std::endl;
			}
			else {
				valid = true;
			}
		}
	}
	return algorithmChoice;
}

int getSearchMethod() {
	int area;

	bool valid = false;

	//loop to validate user input
	while (valid != true) {
		try {
			std::cout << std::endl << "----------------------------------------------------------" << std::endl;
			std::cout << "How to search the image?" << std::endl;
			std::cout << "1 - Small Sub-Image - 3 Rows, 2 Columns" << std::endl;
			std::cout << "2 - Medium Sub-Image - 2 Rows, 2 Columns" << std::endl;
			std::cout << "3 - Entire Image - Pixel by Pixel (Long wait time, approx 30 seconds)" << std::endl;
			std::cout << "----------------------------------------------------------" << std::endl;

			//user input
			std::cin >> area;

			//test and throw error if bad input
			if ((area != 1) && (area != 2) && (area != 3)) {
				throw 1;
			}
			else {
				throw 2;
			}
		}
		catch (int errorNumber) {
			if (errorNumber == 1) {
				std::cout << "Invalid input, try again" << std::endl;
			}
			else {
				valid = true;
			}
		}
	}
	return area;
}

int main() {

	//number of rows and columns in the image
	const int clutteredRows = 768, clutteredCols = 1024, wallyRows = 49, wallyCols = 36;
	int comparisons = 0;
	double SSD, NCC;

	//initiate the matrix from the classes
	match* wallyImage = new match();
	largeImage* sceneImage = new largeImage(clutteredRows, clutteredCols);

	//required text file names
	const std::string inputFilename = "Cluttered_scene.txt", wallyInputFilename = "Wally_Grey.txt";

	//create pointers to 1D arrays of doubles read in from the above text files
	double* clutteredSceneInputDataSSD = 0;
	double* clutteredSceneInputDataNCC = 0;
	double* wallyInputData = 0;
	double* matrixAtArea;

	//arrays for the colour codes of the text files being used
	clutteredSceneInputDataSSD = readTXT(inputFilename, clutteredRows, clutteredCols);
	clutteredSceneInputDataNCC = readTXT(inputFilename, clutteredRows, clutteredCols);
	wallyInputData = readTXT(wallyInputFilename, wallyRows, wallyCols);

	//puts the colour codes in the matrix for the text files, the objects now have the codes in 2D arrays
	convertPixels(wallyRows, wallyCols, wallyInputData, wallyImage);
	convertPixels(clutteredRows, clutteredCols, clutteredSceneInputDataSSD, sceneImage);
	convertPixels(clutteredRows, clutteredCols, clutteredSceneInputDataNCC, sceneImage);

	//delete
	delete[] wallyInputData;

	//put the Wally image values into a 1D Array
	double* wallyArea = wallyImage->getArea(0, 0, wallyRows, wallyCols);

	//delete, do not need this now
	delete wallyImage;

	//create temporary objects to compare using copy constructor
	match* tempMatrixObjectSSD = new match();
	match tempMatrixObjectNCC = *tempMatrixObjectSSD;

	//basic variables for a loop
	int algorithmChoice = getAlgorithm(), area = getSearchMethod(), thresholdCols, thresholdRows;

	//assign threshold variables to search image, based on user choice of search method
	switch (area) {
	case 1:
		thresholdRows = wallyRows / 3;
		thresholdCols = wallyCols / 2;
		break;
	case 2:
		thresholdRows = wallyRows / 2;
		thresholdCols = wallyCols / 2;
		break;
	case 3:
		thresholdCols = 1;
		thresholdRows = 1;
		break;
	default:
		std::cout << R"(Error setting the rows and columns amount - Restart the program)" << std::endl;
	}
	std::cout << "Searching..." << std::endl << std::endl;

	//loop for running through the scene and calculating similarity
	for (int rowAmount = 0; rowAmount < clutteredRows - wallyRows; rowAmount = rowAmount + thresholdRows) {
		for (int colAmount = 0; colAmount < clutteredCols - wallyCols; colAmount = colAmount + thresholdCols) {

			//Get 1D array of the cluttered scene colour values
			matrixAtArea = sceneImage->getArea(rowAmount, colAmount, wallyRows, wallyCols);

			//Run search algorithm on the retrieved matrix area
			if (algorithmChoice == 1) {
				//calculate SSD of the scene area
				SSD = sceneImage->calculateSSD(wallyArea, matrixAtArea, wallyRows, wallyCols);
			}
			else {
				//calculate NCC of the scene area
				NCC = sceneImage->calculateNCC(wallyArea, matrixAtArea, wallyRows, wallyCols);
			}

			//delete so can be used again
			delete[] matrixAtArea;

			//set object variables if loop is at the first set of row and columns
			if (rowAmount == 0 && colAmount == 0) {
				if (algorithmChoice == 1) {
					tempMatrixObjectSSD->setFirstCol(colAmount);
					tempMatrixObjectSSD->setFirstRow(rowAmount);
					tempMatrixObjectSSD->setSSD(SSD);
				}
				else {
					tempMatrixObjectNCC.setFirstCol(colAmount);
					tempMatrixObjectNCC.setFirstRow(rowAmount);
					tempMatrixObjectNCC.setNCC(NCC);
				}
			}

			//Check if new calculated SSD is smaller than currently stored SSD, if yes it overwrites object or the loop continues
			if (algorithmChoice == 1) {
				if (SSD < tempMatrixObjectSSD->getSSD()) {
					tempMatrixObjectSSD->setSSD(SSD);
					tempMatrixObjectSSD->setFirstRow(rowAmount);
					tempMatrixObjectSSD->setFirstCol(colAmount);
				}
			}
			else {
				if (NCC > tempMatrixObjectNCC.getNCC()) {
					tempMatrixObjectNCC.setNCC(NCC);
					tempMatrixObjectNCC.setFirstRow(rowAmount);
					tempMatrixObjectNCC.setFirstCol(colAmount);
				}
			}
			//increase the number of comparisons
			comparisons++;
		}
	}
	//delete upon completion
	delete[] wallyArea;

	//Edit the cluttered scene inputs to where the program has found the subject
	if (algorithmChoice == 1) {
		clutteredSceneInputDataSSD = sceneImage->draw(tempMatrixObjectSSD->getFirstRow(), tempMatrixObjectSSD->getFirstCol(), clutteredSceneInputDataSSD, wallyRows, wallyCols, clutteredCols);
	}
	else {
		clutteredSceneInputDataNCC = sceneImage->draw(tempMatrixObjectNCC.getFirstRow(), tempMatrixObjectNCC.getFirstCol(), clutteredSceneInputDataNCC, wallyRows, wallyCols, clutteredCols);
	}

	//delete objects
	delete sceneImage;
	delete tempMatrixObjectSSD;

	//write out the new scene showing where wally is. Q = 255 for greyscale images and 1 for binary images.
	const int Q = 255;

	//declare output filenames
	const std::string outputFileName_SSD = "resultSSD.pgm", outputFileName_NC = "resultNCC.pgm";

	//write data to a file
	if (algorithmChoice == 1) {
		WritePGM(outputFileName_SSD, clutteredSceneInputDataSSD, clutteredRows, clutteredCols, Q);
	}
	else {
		WritePGM(outputFileName_NC, clutteredSceneInputDataNCC, clutteredRows, clutteredCols, Q);
	}

	//delete objects
	delete[] clutteredSceneInputDataSSD;
	delete[] clutteredSceneInputDataNCC;

	//output to the user
	int endbreak;
	std::ostringstream output;
	output << "Search complete." << std::endl << comparisons << "Images have been compared." << " I've drawn a black box around where Wally is. The result is stored in a PGM image file";

	//show user correct filename
	if (algorithmChoice == 1) {
		output << " 'resultSSD.pgm'.";
	}
	else {
		output << " 'resultNCC.pgm'.";
	}

	//output message
	std::cout << output.str() << std::endl << std::endl;
	endbreak;
	return 0;
}

// read .txt file with image of size RxC, and convert to doubles array
double* readTXT(std::string fileName, int sizeR, int sizeC) {

	//1D double array
	double* data = new double[sizeR*sizeC];

	int i = 0;

	std::ifstream myfile;
	myfile.open(fileName, std::ios::in);

	if (myfile.is_open()) {

		while (myfile.good()) {
			//if reaches the end of matrix
			if (i > sizeR*sizeC - 1) break;
			myfile >> *(data + i);
			i++;
		}
		myfile.close();
	}
	else {
		std::cout << "Cannot open file" << std::endl;
	}
	return data;
}

void convertPixels(int rows, int cols, double* data, largeImage*& image) {
	int count = 0;

	//sends the colour codes to the matrix for the Wally matrix by using a member function of the matrix class
	for (int rowcount = 0; rowcount < rows; rowcount++) {
		for (int colcount = 0; colcount < cols; colcount++) {
			image->setPixel(rowcount, colcount, data[count]);
			count++;
		}
	}
}

void convertPixels(int rows, int cols, double* data, match*& image) {
	int count = 0;

	//set the colour codes to matrix for the cluttered scene matrix by using member function of the matrix class
	for (int rowcount = 0; rowcount < rows; rowcount++) {
		for (int colcount = 0; colcount < cols; colcount++) {
			image->setPixel(rowcount, colcount, data[count]);
			count++;
		}
	}
}

//16608272