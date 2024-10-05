#pragma once
struct Node {
	char value;
	Node* up;
	Node* down;
	Node* left;
	Node* right;
	bool createdByEnter;

	Node(char value, bool createdByEnter = false) {
		this->value = value;
		this->createdByEnter = createdByEnter;
		up = down = left = right = nullptr;
	}
};