/* CS 4300
 * Project 4
 * Decision Tree Classifier
 *
 * main.cpp
 *
 *  Created on: Apr 18, 2018
 *      Author: Nick Nesbit
 */

//Includes
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <cmath>

#include "sample.h"
#include "tree.h"

using namespace std;


//Function prototypes
Tree* computeDecisionTree(Tree* tree, int numExamples, int numAttribs, Sample* examples, Sample* parent_examples);
bool mostCommonOutput(Sample* examples);
bool allTheSame(int numExamples, Sample* examples);
bool noAttributes(int numAttribs, Sample* examples);
int chooseAttribute(int numExamples, int numAttribs, Sample* examples);
void printTree(Tree* tree, int level);
int getClassification(Tree* tree, Sample query);
void deleteTree(Tree* tree);


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
	inFile >> numValues;
	inFile >> numSamples;
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
		inFile >> samples[i].outcome;
	}

	//Close the file
	inFile.close();

	//Display the dataset
	cout << "Dataset:" << endl;
	for (int i = 0; i < numSamples; i++) {
		for (int j = 0; j < numValues; j++)
			cout << samples[i].values[j] << " ";
		cout << samples[i].outcome << endl;
	}
	cout << endl;

	//Seed random number generator
	srand(time(0));

	//Create a new tree
	Tree* tree = new Tree();
	tree->value = -1;

	//Compute the decison tree using the dataset
	tree = computeDecisionTree(tree, numSamples, numValues, samples, samples);

	//Print the tree
	cout << "Decision Tree:" << endl;
	printTree(tree, -1);

	//Ask user for query
	cout << endl << "Let's test this bad boy!" << endl;

	Sample query;
	query.values = new int[numValues];

	for (int i = 0; i < numValues; i++) {
		cout << "Enter the value of attribute " << i << " (0 or 1): ";
		cin >> query.values[i];
	}

	//Determine and print the classification
	cout << "Classification: " << getClassification(tree, query) << endl;


	//Delete the allocated attribute value arrays
	delete [] query.values;
	for (int i = 0; i < numSamples; i++)
		delete [] samples[i].values;

	//Delete the allocated sample arrays
	delete [] samples;

	//Delete the decision tree
	deleteTree(tree);

	//End the program
	return 0;
}
//End of main program


/****************************************************************************
*					  FUNCTION DEFINITIONS                                  *
****************************************************************************/

/**************************************
* Tree* computeDecisionTree()        *
**************************************/
Tree* computeDecisionTree(Tree* tree, int numExamples, int numAttribs, Sample* examples, Sample* parent_examples) {
	//If terminal case 1
	if (numExamples == 0) {
		tree->label = "Decision: ";
		tree->value = mostCommonOutput(parent_examples);
		return tree;
	}
	//Else if terminal case 2
	else if (allTheSame(numExamples, examples) == true) {
		tree->label = "Decision: ";
		tree->value = examples[0].outcome;
		return tree;
	}
	//Else if terminal case 3
	else if (noAttributes(numAttribs, examples) == true) {
		tree->label = "Decision: ";
		tree->value = mostCommonOutput(examples);
		return tree;
	}
	//Else Split
	else {
		int split = chooseAttribute(numExamples, numAttribs, examples);
		tree->label = "Attribute: ";
		tree->value = split;
		int numFalse;
		int numTrue;
		for (int i = 0; i < numExamples; i++) {
			if (examples[i].values[split] == 0) {
				numFalse++;
			}
			else if (examples[i].values[split] == 1 ) {
				numTrue++;
			}
		}

		Sample* falseSet = new Sample[numFalse];
		Sample* trueSet = new Sample[numTrue];

		int trueCount = 0;
		int falseCount = 0;

		for (int i = 0; i < numExamples; i++) {
			if (examples[i].values[split] == 0) {
				falseSet[falseCount] = examples[i];
				falseCount++;
			}
			else if (examples[i].values[split] == 1 ) {
				trueSet[trueCount] = examples[i];
				trueCount++;
			}
		}

		for (int i = 0; i < falseCount; i++) {
			falseSet[i].values[split] = -1;
		}

		for (int i = 0; i < trueCount; i++) {
			trueSet[i].values[split] = -1;
		}

		//Recursively call the computeDecsionTree function
		tree->branch[0] = new Tree();
		tree->branch[0]->value = -1;
		tree->branch[0] = computeDecisionTree(tree->branch[0], falseCount, numAttribs, falseSet, examples);
		tree->branch[1] = new Tree();
		tree->branch[1]->value = -1;
		tree->branch[1] = computeDecisionTree(tree->branch[1], trueCount, numAttribs, trueSet, examples);

		//Delete the allocated samples
		delete [] falseSet;
		delete [] trueSet;

		//Return the tree
		return tree;
	}
}


/*************************************
* bool mostCommonOutput()            *
*************************************/
bool mostCommonOutput(Sample* examples) {
	//Get the number of true and the number of false instances
	int outputTrue = 0;
	int outputFalse = 0;

	for (unsigned int i = 0; i < (sizeof(examples))/(sizeof(examples[0])); i++) {
		if (examples[i].outcome == true)
			outputTrue++;
		else
			outputFalse++;
	}

	//If more true than false
	if (outputTrue > outputFalse)
		return true;
	//If more false than true
	else if (outputFalse > outputTrue)
		return false;
	//If tie break - randomly pick true
	else if (rand()%10 < 5)
		return true;
	//If tie break - randomly pick false
	else
		return false;
}


/**********************************
* bool allTheSame()               *
**********************************/
bool allTheSame(int numExamples, Sample* examples) {
	//Check if all examples have the same outcome
	int outcome = examples[0].outcome;
	for (int i = 1; i < numExamples; i++) {
		if (examples[i].outcome != outcome) {
			return false;
		}
	}
	return true;
}


/*******************************
* bool noAttributes()          *
*******************************/
bool noAttributes(int numAttribs, Sample* examples) {
	//Check if there are no more attributes left to split on
	int count = 0;
	for (int i = 0; i < numAttribs; i++) {
		if (examples[0].values[i] == -1) {
				count++;
		}
	}
	if (count == numAttribs)
		return true;
	return false;
}


/**********************************
* int chooseAttribute()           *
**********************************/
int chooseAttribute(int numExamples, int numAttribs, Sample* examples) {
	//Utility variables
	int numTrue;
	int numTrueTrue;
	int numTrueFalse;
	int numFalse;
	int numFalseTrue;
	int numFalseFalse;
	float lowest = 10;
	int retAttrib;
	float term[6];

	//Array to hold summed entropy for each attribute
	float entropy[numAttribs];

	//For all attributes
	for (int i = 0; i < numAttribs; i++) {
		//If already split on
		if (examples[0].values[i] == -1) {
			entropy[i] = -1.0;
		}
		//Else if not already split on
		else {
			//Initialize utility varibales
			numTrue = 0;
			numTrueTrue = 0;
			numTrueFalse = 0;
			numFalse = 0;
			numFalseTrue = 0;
			numFalseFalse = 0;

			//For all samples
			for (int j = 0; j < numExamples; j++) {
				//If attribute true
				if (examples[j].values[i] == 1) {
					numTrue++;
					//If outcome true
					if (examples[j].outcome == 1) {
						numTrueTrue++;
					}
					//Else if outcome false
					else if (examples[j].outcome == 0) {
						numTrueFalse++;
					}
				}
				//Else if attribute false
				else if (examples[j].values[i] == 0) {
					numFalse++;
					//If outcome true
					if (examples[j].outcome == 1) {
						numFalseTrue++;
					}
					//Else if outcome false
					else if (examples[j].outcome == 0) {
						numFalseFalse++;
					}
				}
			}

			//Check for computational concerns and compute the entropy equation terms
			if (numTrue == 0) {
				term[1] = 1;
				term[2] = 1;
			}
			else {
				term[1] = float(numTrueTrue)/numTrue;
				term[2] = float(numTrueFalse)/numTrue;
			}

			if (numFalse == 0) {
				term[4] = 1;
				term[5] = 1;
			}
			else {
				term[4] = float(numFalseTrue)/numFalse;
				term[5] = float(numFalseFalse)/numFalse;
			}

			if (numTrueTrue == 0)
				term[1] = 1;
			if (numTrueFalse == 0)
				term[2] = 1;
			if (numFalseTrue == 0)
				term[4] = 1;
			if (numFalseFalse == 0)
				term[5] = 1;

			term[0] = float(numTrue)/numExamples;
			term[3] = float(numFalse)/numExamples;

			//Cacluate the summed entropy
			entropy[i] = ((-term[0])*(term[1]*log2(term[1]) + term[2]*log2(term[2]))) + ((-term[3])*(term[4]*log2(term[4]) + term[5]*log2(term[5])));

			//cout << "Entropy " << i << ": " << entropy[i] << endl;
		}
	}

	//Determine the lowest summed entropy
	for (int i = 0; i < numAttribs; i++) {
		if (entropy[i] != -1 && entropy[i] <= lowest) {
			lowest = entropy[i];
			retAttrib = i;
		}
	}

	//Return the desired attribute
	return retAttrib;
}


/***************************
* void printTree()         *
***************************/
void printTree(Tree* tree, int level) {
	if (tree) {

		level++;

		for (int i = 0; i < level; i++)
			cout << "   ";

		cout << tree->label << tree->value << endl;

		if (tree->branch[0]) {
			level++;
			for (int i = 0; i < level; i++)
				cout << "   ";
			cout << "Choice 0:" << endl;
			printTree(tree->branch[0], level);
			level--;
		}

		if (tree->branch[1]) {
			level++;
			for (int i = 0; i < level; i++)
				cout << "   ";
			cout << "Choice 1:" << endl;
			printTree(tree->branch[1], level);
			level--;
		}
	}
}

/*******************************************
* int getClassification()                  *
*******************************************/
int getClassification(Tree* tree, Sample query) {
	if (tree->branch[0] == NULL && tree->branch[1] == NULL) {
		return tree->value;
	}
	else if(query.values[tree->value] == 0) {
		return getClassification(tree->branch[0], query);
	}
	else if(query.values[tree->value] == 1) {
		return getClassification(tree->branch[1], query);
	}
	else {
		return -1;
	}
}

/*****************************
* void deleteTree()          *
*****************************/
void deleteTree(Tree* tree) {
	if (tree) {
		if (tree->branch[0])
			deleteTree(tree->branch[0]);
		if (tree->branch[1])
			deleteTree(tree->branch[1]);
		delete tree;
	}
}
