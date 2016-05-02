#pragma once
#include "RE\ReEngAppClass.h"

#define DX 0
#define DY 1
#define DZ 2

class Node {
	float median;
	int dimension;

	Node* left;
	Node* right;

	std::vector<vector3> points;
public:
	Node(int, int, std::vector<Node>);


};