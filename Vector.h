#pragma once
#include <iostream>
using namespace std;

template <class T>
class Vector {
public:
	T* arr;
	int size;
	int capacity;

	Vector() {
		size = 0;
		capacity = 1;
		arr = new T[capacity];
	}

	void push_back(const T& val) {
		if (exists(val)) {
			return;
		}
		if (size == capacity) {
			capacity *= 2;
			T* temp = new T[capacity];
			for (int i = 0; i < size; i++) {
				temp[i] = arr[i];
			}
			//delete[] arr;
			arr = temp;
		}
		arr[size++] = val;
	}

	void insert(const T& val) {
		if (size == capacity) {
			capacity *= 2;
			T* temp = new T[capacity];
			for (int i = 0; i < size; i++) {
				temp[i] = arr[i];
			}
			//delete[] arr;
			arr = temp;
		}
		arr[size++] = val;
	}

	bool exists(const T& val) {
		for (int i = 0; i < size; i++) {
			if (arr[i] == val) {
				return true;
			}
		}
		return false;
	}

	void pop_back() {
		if (size > 0) {
			size--;
		}
	}

	T& operator[](int idx) {
		return *(arr + idx);
	}

	Vector& operator=(const Vector& other) {
		if (this != &other) {
			//delete[] arr;
			this->size = other.size;
			this->capacity = other.capacity;
			this->arr = new T[capacity];
			for (int i = 0; i < size; i++) {
				this->arr[i] = other.arr[i];
			}
		}
		return *this;
	}

	~Vector() {
		//delete[] arr;
	}
};