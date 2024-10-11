#pragma once
#include "Node.h"
#include <iostream>
#include "Notepad.h"
using namespace std;

struct StackNode {
	Notepad notepad;
	StackNode* next;

	StackNode(const Notepad& other) {
		this->notepad = Notepad();
		this->notepad = other;
		next = nullptr;
	}
};

class Stack {
	StackNode* top;

public:
	Stack() {
		top = nullptr;
	}

	void push(const Notepad& other) {
		StackNode* newNode = new StackNode(other);
		newNode->next = top;
		top = newNode;
	}

	Notepad pop() {
		if (top != nullptr) {
			Notepad res;
			res = top->notepad;
			StackNode* temp = top;
			top = top->next;
			delete temp;
			return res;
		}
		return Notepad();
	}

	bool isEmpty() {
		return top == nullptr;
	}

	Notepad peek() {
		if (top != nullptr) {
			return top->notepad;
		}
		return Notepad();
	}

	void clearStack() {
		while (!isEmpty()) {
			pop();
		}

	}

};
