/* CS 4300
 * Project 5: Perceptron Learning
 *
 * perceptron.cpp
 *
 *  Created on: May 2, 2018
 *      Author: Nick Nesbit
 */

//Includes
#include <fstream>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "sample.h"
using namespace std;

//Define constants
#define LEARN_RATE 1
#define K_RATE 1
#define MAX_ITERATION 1000000

//Function prototypes
float randomFloat();
int getError(int numSamples, int numValues, Sample* samples, float weights[]);
int getSign(int numValues, int* values, float weights[]);
void updateWeights(int numSamples, int numValues, Sample* samples, float weights[], int* updates);

//Start of main program
int main(int argc, char* argv[]) {
	//Check arguments
	if (argc != 2) {
		cout << "Error: expected file as arg.\n";
		return 0;
	}

	//Open the file
	ifstream inFile(argv[1]);

	//Check for file open fail
	if (!inFile.is_open()) {
		cout << "Error: file could not open.\n";
		return 0;
	}

	//Get first line of data from file
	int numValues, numSamples;
	inFile >> numSamples;
	inFile >> numValues;
	//cout << numValues << " " << numSamples << endl;

	//Create an array of sample objects
	Sample* samples = new Sample[numSamples];

	//Allocate an array of attribute values for each sample
	for (int i = 0; i < numSamples; i++) {
		samples[i].values = new int[numValues];
	}

	//Initialize the sample data
	for (int i = 0; i < numSamples; i++) {
		for (int j = 0; j < numValues; j++)
			inFile >> samples[i].values[j];
		inFile >> samples[i].label;
	}

	//Close the file
	inFile.close();

	//Initialize weights randomly
	srand(time(0));
	float weights[numValues+1];
	for (int i = 0; i < numValues+1; i++)
		weights[i] = randomFloat();

	//Print the starting equation
	cout << "Starting equation: " << weights[0];
	for (int i = 1; i <= numValues; i++)
		cout << " + (" << weights[i] << " * x" << i << ")";
	cout << endl;

	//Initialize the training error
	int trainError = getError(numSamples, numValues, samples, weights);
	cout << "Starting in-sample error: " << trainError << " / " << numSamples << endl;

	//Train on the dataset
	int itr = 0;
	int updates = 0;
	while (itr < MAX_ITERATION && trainError > 0) {
		updateWeights(numSamples, numValues, samples, weights, &updates);
		trainError = getError(numSamples, numValues, samples, weights);
		itr++;
	}

	//Print training results
	cout << endl << "Final equation: " << weights[0];
	for (int i = 1; i <= numValues; i++)
		cout << " + (" << weights[i] << " * x" << i <<")";
	cout << endl <<  "Final in-sample error: " << trainError << " / " << numSamples << endl;
	cout << "Number of iterations: " << itr << endl;
	cout << "Number of updates: " << updates << endl;

	//Get query from the user
	cout << endl << "Time to test!" << endl;
	Sample query;
	query.values = new int[numValues];
	for (int i = 0; i < numValues; i++) {
		cout << "Enter the value of attribute " << i << " (-1 or 1): ";
		cin >> query.values[i];
	}

	//Print testing results
	if (getSign(numValues, query.values, weights) > 0)
		cout << "Classification: 1" << endl;
	else
		cout << "Classification: -1" << endl;

	//Delete allocated memory
	for (int i = 0; i < numSamples; i++)
		delete [] samples[i].values;
	delete [] samples;
	delete [] query.values;

	//End the program
	return 0;
}
//End of main program


/*****************************************************************
* 					FUNCTION DEFINITIONS						 *
******************************************************************/


/**************************************************************************
* float randomFloat() - Returns random float between 0 and 10 inclusive.  *
**************************************************************************/
float randomFloat() {
    return ((float)rand()/(float)(RAND_MAX)) * 10.00;
}

/***************************************************************************
* int getSign() - Returns sign of the current classification equation      *
***************************************************************************/
//Returns sign of equation.
int getSign(int numValues, int* values, float weights[]) {
	float sum = weights[0];
	for (int i = 1; i <= numValues; i++)
		sum += weights[i]*values[i-1];
	return (sum >= 0) ? 1 : -1;
}

/***********************************************
* void updateWeights() - Updates the weights.  *
***********************************************/
void updateWeights(int numSamples, int numValues, Sample* samples, float weights[], int* updates) {
	for (int i = 0; i < numSamples; i++) {
		if (samples[i].label != getSign(numValues, samples[i].values, weights)) {
			(*updates)++;
			weights[0] += LEARN_RATE * samples[i].label * K_RATE;
			for (int j = 1; j <= numValues; j++)
				weights[j] += LEARN_RATE * samples[i].label * samples[i].values[j-1];
		}
	}
	return;
}

/********************************************************************
* float getError() - Returns number of misclassified points.        *
********************************************************************/
int getError(int numSamples, int numValues, Sample* samples, float weights[]) {
	int error = 0;
	for (int i = 0; i < numSamples; i++)
		if (samples[i].label != getSign(numValues, samples[i].values, weights))
			error++;
	return error;
}
