#pragma once
#include <iostream>
#include "Vector.h"
#include "codes/String.h"
using namespace std;

class ChilliMilliTree {
private:
	void resize() {
		int newSize = capacity * 2;
		String* tempKeys = new String[newSize];
		Vector<String>* tempValues = new Vector<String>[newSize];
		for (int i = 0; i < size; i++) {
			tempKeys[i] = keys[i];
			tempValues[i] = values[i];
		}
		delete[] keys;
		delete[] values;
		keys = tempKeys;
		values = tempValues;
		capacity = newSize;
	}

public:
	String* keys;
	Vector<String>* values;
	int size;
	int capacity;

	ChilliMilliTree() {
		size = 0;
		capacity = 1;
		keys = nullptr;
		values = nullptr;
	}

	void insert(const String& key, const String& value) {
		if (keys == nullptr) {
			size = 0;
			capacity = 1;
			keys = new String[capacity];
			values = new Vector<String>[capacity];
			keys[size] = key;
			values[size].push_back(value);
			size++;
			return;
		}
		if (size == capacity) {
			resize();
		}
		for (int i = 0; i < size; i++) {
			if (keys[i] == key) {
				for (int j = 0; j < values[i].size; j++) {
					if (values[i][j] == value) {
						return;
					}
				}
				values[i].push_back(value);
				return;
			}
		}
		keys[size] = key;
		values[size].push_back(value);
		size++;
	}

	Vector<String>* getValues(const String& key) {
		for (int i = 0; i < size; i++) {
			if (keys[i] == key) {
				return &values[i];
			}
		}
		return nullptr;
	}

	void print() {
		cout << "Map Size: " << this->size << endl;
		for (int i = 0; i < this->size; i++) {
			cout << keys[i] << " -> ";
			for (int j = 0; j < values[i].size; j++) {
				cout << values[i][j] << " ";
			}
			cout << endl;
		}
	}
};