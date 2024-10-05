#pragma once
#include "Node.h"
#include <iostream>
using namespace std;

struct StackNode {
	Node* word;
	StackNode* next;

	StackNode(Node* word) {
		this->word = word;
		next = nullptr;
	}
};

class Stack {
	StackNode* top;

public:
	Stack() {
		top = nullptr;
	}

	void push(Node* word) {
		StackNode* newNode = new StackNode(word);
		newNode->next = top;
		top = newNode;
	}

	Node* pop() {
		if (top == nullptr) {
			return nullptr;
		}

		Node* word = top->word;
		StackNode* temp = top;
		top = top->next;
		delete temp;
		return word;
	}

	bool isEmpty() {
		return top == nullptr;
	}

};
