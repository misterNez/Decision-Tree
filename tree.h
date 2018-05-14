/* CS4300
 * Project 4
 * Decision Tree Classifier
 *
 * tree.h
 *
 *  Created on: Apr 18, 2018
 *      Author: Nick Nesbit
 */

#ifndef NESBITCS4300P4_TREE_H_
#define NESBITCS4300P4_TREE_H_

#include <string>

typedef struct Tree {
	std::string label;
	int value;
	Tree* branch[2];
} Tree;

#endif /* NESBITCS4300P4_TREE_H_ */
