#pragma once
#include "RE\ReEngAppClass.h"
#include "Node.h"

class Node;
class kDTree {
	Node* root;
	void NearestNeighbor(vector3 point, Node* node, vector3& ref, float& dist);
public:
	kDTree(std::vector<vector3> points);
	void Draw(vector3 min, vector3 max);
	vector3 NearestNeighbor(vector3 point);
};