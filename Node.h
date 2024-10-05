#pragma once
struct Node {
	char value;
	Node* up;
	Node* down;
	Node* left;
	Node* right;

	Node(char value) {
		this->value = value;
		up = down = left = right = nullptr;
	}
};