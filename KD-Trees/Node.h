#pragma once
#include "RE\ReEngAppClass.h"
#include "kDTree.h"

#define DX 0
#define DY 1
#define DZ 2

class kDTree;
class Node {
	float median;
	int axis;
	int depth;

	kDTree* tree;
	Node* left;
	Node* right;
	vector3 point;

	float getAxisValue(int dimension, vector3 vec);
public:
	Node(int dimension, int depth, std::vector<vector3> points);

	~Node();

	void Insert(vector3 point);

	void Release();

	void Draw(vector3 min, vector3 max);

	Node* GetLeft();

	Node* GetRight();

	vector3 GetPoint();

	float GetAxisValue(vector3 vec);

	float GetMedian();
};