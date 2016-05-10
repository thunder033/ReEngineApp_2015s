#include "Node.h"

float Node::getAxisValue(int dimension, vector3 vec) {
	switch (dimension) {
	case DX:
		return vec.x;
	case DY:
		return vec.y;
	case DZ:
		return vec.z;
	default:
		return 0;
	}
}

Node::Node(int axis, int depth, std::vector<vector3> points) {
	this->axis = axis;
	this->depth = depth;

	if (points.size() <= 1) {
		this->point = points.size() == 1 ? points[0] : vector3(0);
		return;
	}

	std::sort(points.begin(), points.end(), [this](vector3 a, vector3 b) ->bool {
		return getAxisValue(this->axis, a) < getAxisValue(this->axis, b);
	});

	float medianVal = points.size() / 2;
	median = (getAxisValue(axis, points[medianVal]) + getAxisValue(axis, points[medianVal - 1])) / 2.0f;

	std::vector<vector3> leftPoints;
	std::vector<vector3> rightPoints;
	std::vector<vector3>::iterator it;
	for (it = points.begin(); it != points.end(); ++it) {
		float coord = getAxisValue(axis, *it);
		if (coord >= median)
			rightPoints.push_back(*it);
		else 
			leftPoints.push_back(*it);
	}

	left = new Node((axis + 1) % 3, depth + 1, leftPoints);
	right = new Node((axis + 1) % 3, depth + 1, rightPoints);
}

Node::~Node() {
	Release();
}

void Node::Insert(vector3 point)
{
}

void Node::Release() {
	if (left != nullptr) {
		delete left;
		left = nullptr;
	}

	if (right != nullptr) {
		delete right;
		right = nullptr;
	}
}

void Node::Draw(vector3 min, vector3 max)
{
	if (right == nullptr)
		return;

	MeshManagerSingleton* renderer = MeshManagerSingleton::GetInstance();
	vector3 pos = (min + max) / 2.0f;
	vector3 size = max - min;
	vector3 color;
	switch (axis) {
	case DX:
		pos.x = median;
		size.x = 0;
		color = RERED;
		break;
	case DY:
		pos.y = median;
		size.y = 0;
		color = REGREEN;
		break;
	case DZ:
		pos.z = median;
		size.z = 0;
		color = REBLUE;
		break;
	}

	renderer->AddCubeToRenderList(glm::translate(pos) * glm::scale(size), color, WIRE);

	if (left != nullptr) {
		float x = axis == DX ? median : max.x;
		float y = axis == DY ? median : max.y;
		float z = axis == DZ ? median : max.z;
		vector3 lMax = vector3(x, y, z);
		left->Draw(min, lMax);
	}

	if (right != nullptr) {
		float x = axis == DX ? median : min.x;
		float y = axis == DY ? median : min.y;
		float z = axis == DZ ? median : min.z;
		vector3 rMin = vector3(x, y, z);
		right->Draw(rMin, max);
	}
}

Node * Node::GetLeft()
{
	return left;
}

Node * Node::
GetRight()
{
	return right;
}
vector3 Node::GetPoint()
{
	return point;
}

float Node::GetAxisValue(vector3 vec)
{
	return getAxisValue(axis, vec);
}

float Node::GetMedian()
{
	return median;
}
