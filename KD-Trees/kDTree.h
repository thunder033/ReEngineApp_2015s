#pragma once
#include "RE\ReEngAppClass.h"
#include "Node.h"

class Node;
class kDTree {
	int depth;
	Node* root;
	void NearestNeighbor(vector3 point, Node* node, vector3& ref, float& dist);
public:
	kDTree(int depth, std::vector<vector3> points);
	int GetDepth();
	void Draw(vector3 min, vector3 max);
	vector3 NearestNeighbor(vector3 point);
};