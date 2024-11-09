#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "codes/String.h"
using namespace std;

struct TNode {
	char key;
	bool end;
	TNode* next[26];
	TNode(char key) {
		this->key = key;
		for (int i = 0; i < 26; i++) {
			next[i] = nullptr;
		}
		end = false;
	}
};

class NAryTree {
	typedef TNode* ptr;
private:
	ptr root;
public:
	NAryTree() {
		root = new TNode(' ');
	}
	void insert(String word) {
		int i = 0;
		ptr current = root;
		while (i < word.getLength()) {
			char ch;
			if (word[i] >= 'A' && word[i] <= 'Z') {
				ch = 'A';
			}
			else {
				ch = 'a';
			}
			if (current->next[word[i] - ch] == nullptr) {
				current->next[word[i] - ch] = new TNode(word[i]);
			}
			current = current->next[word[i] - ch];
			i++;
		}
		current->end = true;
	}
	bool search(String word) {
		ptr current = root;
		for (int i = 0; i < word.getLength(); i++) {
			char ch;
			if (word[i] >= 'A' && word[i] <= 'Z') {
				ch = 'A';
			}
			else {
				ch = 'a';
			}
			if (current->next[word[i] - ch] == nullptr) {
				return false;
			}
			current = current->next[word[i] - ch];
		}
		return current->end;
	}

	int getCount(String word) {
		ptr current = root;
		for (int i = 0; i < word.getLength(); i++) {
			char ch;
			if (word[i] >= 'A' && word[i] <= 'Z') {
				ch = 'A';
			}
			else {
				ch = 'a';
			}
			if (current->next[word[i] - ch] == nullptr) {
				return 0;
			}
			current = current->next[word[i] - ch];
		}
		return countWords(current);
	}

	int countWords(ptr root) {
		if (root == nullptr) {
			return 0;
		}
		int count = 0;
		if (root->end) {
			count++;
		}
		for (int i = 0; i < 26; i++) {
			count += countWords(root->next[i]);
		}
		return count;
	}

	String* getWords(String word) {
		/*if (!search(word)) {
			cout << "Not found \n";
			return nullptr;
		}*/
		int noOfWords = getCount(word);
		if (noOfWords == 0) {
			return nullptr;
		}
		String* words = new String[noOfWords];

		// Traverse to the last character of the prefix word
		ptr current = root;
		for (int i = 0; i < word.getLength(); i++) {
			char ch;
			if (word[i] >= 'A' && word[i] <= 'Z') {
				ch = 'A';
			}
			else {
				ch = 'a';
			}
			current = current->next[word[i] - ch];
		}

		int idx = 0;
		findWords(current, words, idx, word); // Fill words starting from the given prefix
		return words;
	}

	void findWords(ptr root, String* words, int& idx, String word) {
		if (root == nullptr) {
			return;
		}
		if (root->end) {
			words[idx++] = word; // Store the current word in words array
		}
		for (int i = 0; i < 26; i++) {
			if (root->next[i] != nullptr) {
				findWords(root->next[i], words, idx, word + root->next[i]->key);
			}
		}
	}

	//String* getWords(String word) {
	//	if (!search(word)) {
	//		return nullptr;
	//	}
	//	int noOfWords = getCount(word);
	//	String* words = new String[noOfWords];
	//	// now we have to find all the words that start with the given word
	//	ptr current = root;
	//	for (int i = 0; i < word.getLength(); i++) {
	//		char ch;
	//		if (word[i] >= 'A' && word[i] <= 'Z') {
	//			ch = 'A';
	//		}
	//		else {
	//			ch = 'a';
	//		}
	//		current = current->next[word[i] - ch];
	//	}
	//	int idx = 0;
	//	findWords(current, words, idx, word);
	//}

	//void findWords(ptr root, String* words, int& idx, String word) {
	//	if (root == nullptr) {
	//		return;
	//	}
	//	if (root->end) {
	//		words[idx++] = word;
	//	}
	//	for (int i = 0; i < 26; i++) {
	//		findWords(root->next[i], words, idx, word + root->next[i]->key);
	//	}
	//}

	int height() {
		return getHeight(root);
	}
	int getHeight(ptr root) {
		if (root == nullptr) {
			return -1;
		}
		int maxHeight = INT_MIN;
		for (int i = 0; i < 26; i++) {
			maxHeight = max(maxHeight, getHeight(root->next[i]) + 1);
		}
		return maxHeight;
	}

	string generateDotCode() {
		stringstream dotCode;
		dotCode << "digraph NAryTree {\n";
		dotCode << "  node [shape=circle];\n";
		queue<ptr> q;
		queue<int> idq;
		q.push(root);
		int id = 0;
		idq.push(id);
		while (!q.empty()) {
			ptr current = q.front();
			q.pop();
			int thisId = idq.front();
			idq.pop();
			dotCode << "  \"" << current->key << thisId << "\" [label=\"" << current->key << "\", shape=";
			if (current->end) {
				dotCode << "doublecircle";
			}
			else {
				dotCode << "circle";
			}
			dotCode << "];\n";

			for (int i = 0; i < 26; i++) {
				if (current->next[i] != nullptr) {
					id++;
					dotCode << "  \"" << current->key << thisId << "\" -> \"" << current->next[i]->key << id << "\";\n";
					q.push(current->next[i]);
					idq.push(id);
				}
			}
		}

		dotCode << "}\n";
		return dotCode.str();
	}

	void visualizeNAryTree() {
		string dotCode = generateDotCode();

		// Save Dot code to a file
		ofstream dotFile("trie.dot");
		dotFile << dotCode;
		dotFile.close();
		system("dot -Tpng trie.dot -o trie.png");
		system("start trie.png");
		// Display a message indicating the file creation
		cout << "Dot code generated and saved to 'trie.dot'.\n";
		cout << "Graph image saved to 'trie.png'.\n";
	}
};