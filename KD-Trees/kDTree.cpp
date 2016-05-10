#include "kDTree.h"

void kDTree::NearestNeighbor(vector3 point, Node * node, vector3& nearest, float& dist)
{
	//Determine if this node is a leaf
	if (node->GetLeft() == nullptr && node->GetRight() == nullptr) {
		float ptDist = glm::distance(point, node->GetPoint());
		if (ptDist < dist) {
			nearest = node->GetPoint();
			dist = ptDist;
		}
	}
	else {
		float axisVal = node->GetAxisValue(point);
		//Determine which side to check first
		if (axisVal > node->GetMedian()) {
			if (axisVal - dist <= node->GetMedian()) NearestNeighbor(point, node->GetLeft(), nearest, dist);
			if (axisVal + dist > node->GetMedian()) NearestNeighbor(point, node->GetRight(), nearest, dist);
		}
		else {
			if (axisVal + dist > node->GetMedian()) NearestNeighbor(point, node->GetRight(), nearest, dist);
			if (axisVal - dist <= node->GetMedian()) NearestNeighbor(point, node->GetLeft(), nearest, dist);
		}
	}
}

kDTree::kDTree(int depth, std::vector<vector3> points) {
	this->depth = depth;
	root = new Node(DX, 0, points);
}

int kDTree::GetDepth() {
	return depth;
}

void kDTree::Draw(vector3 min, vector3 max)
{
	root->Draw(min, max);
}

vector3 kDTree::NearestNeighbor(vector3 point)
{
	vector3 nearest = vector3(0);
	float dist = FLT_MAX;
	NearestNeighbor(point, root, nearest, dist);
	return nearest;
}
