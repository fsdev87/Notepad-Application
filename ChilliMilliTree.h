#pragma once
#include <iostream>
#include "codes/String.h"
using namespace std;

// i want a map that has String type keys, and Vector<String> type values, and both
// must be dynamically increasing whenever a key is being added to the map
// also the vector should not be the stl vector, rather we need to create our own vector
// class

class Vector {
public:
	String* arr;
	int size;
	int capacity;

	Vector() {
		size = 0;
		capacity = 1;
		arr = new String[capacity];
	}

	void push_back(const String& str) {
		if (size == capacity) {
			capacity *= 2;
			String* temp = new String[capacity];
			for (int i = 0; i < size; i++) {
				temp[i] = arr[i];
			}
			delete[] arr;
			arr = temp;
		}
		arr[size++] = str;
	}

	String& operator[](int idx) {
		return arr[idx];
	}

	Vector& operator=(const Vector& other) {
		if (this != &other) {
			delete[] arr;
			size = other.size;
			capacity = other.capacity;
			arr = new String[capacity];
			for (int i = 0; i < size; i++) {
				arr[i] = other.arr[i];
			}
		}
		return *this;
	}

	~Vector() {
		delete[] arr;
	}
};

// perfect, now we need to build the map class
class ChilliMilliTree {
private:
	void resize() {
		int newSize = capacity * 2;
		String* tempKeys = new String[newSize];
		Vector* tempValues = new Vector[newSize];
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
	Vector* values;
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
			values = new Vector[capacity];
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

	Vector* getValues(const String& key) {
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