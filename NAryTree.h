#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "Vector.h"
#include "codes/String.h"
using namespace std;

struct TNode {
	char key;
	bool end;
	TNode* next[26];
	Vector<int> lineNumbers;
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
public:
	ptr root;
	NAryTree() {
		root = new TNode(' ');
	}
	void insert(String word, int lineNumber) {
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
			current->next[word[i] - ch]->lineNumbers.push_back(lineNumber);
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
		int noOfWords = getCount(word);
		if (noOfWords == 0) {
			return nullptr;
		}
		String* words = new String[noOfWords];

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
		findWords(current, words, idx, word);
		return words;
	}

	void findWords(ptr root, String* words, int& idx, String word) {
		if (root == nullptr) {
			return;
		}
		if (root->end) {
			words[idx++] = word;
		}
		for (int i = 0; i < 26; i++) {
			if (root->next[i] != nullptr) {
				findWords(root->next[i], words, idx, word + root->next[i]->key);
			}
		}
	}

	void dfs(ptr root) {
		// we simply need to print all nodes in the tree
		if (root == nullptr) {
			return;
		}
		cout << root->key << " -> ";
		for (int i = 0; i < root->lineNumbers.size; i++) {
			cout << root->lineNumbers[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < 26; i++) {
			dfs(root->next[i]);
		}
	}

	Vector<int>* searchWord(String word) {
		ptr current = root;
		if (!current) return nullptr;

		Vector<int>* common_positions = nullptr;

		for (int i = 0; i < word.getLength(); ++i) {
			char c = word[i];
			int index = c >= 'A' && c <= 'Z' ? c - 'A' : c - 'a';

			// If there's no node for this character, word doesn't exist in trie
			if (!current->next[index]) {
				return nullptr;
			}

			// Move to the next character's node
			current = current->next[index];

			// Initialize or intersect positions
			if (i == 0) {
				// Initialize with the first character's positions
				common_positions = new Vector<int>(current->lineNumbers);
			}
			else {
				// Intersect with the currentent character's positions
				Vector<int> new_common_positions;
				int j = 0, k = 0;

				// Perform an intersection of common_positions and current->lineNumbers
				while (j < common_positions->size && k < current->lineNumbers.size) {
					if ((*common_positions)[j] == current->lineNumbers[k]) {
						new_common_positions.push_back((*common_positions)[j]);
						j++;
						k++;
					}
					else if ((*common_positions)[j] < current->lineNumbers[k]) {
						j++;
					}
					else {
						k++;
					}
				}

				// If intersection is empty, we can exit early
				if (new_common_positions.size == 0) {
					delete common_positions;
					return nullptr;
				}

				// Update common_positions with the intersection result
				*common_positions = new_common_positions;
			}
		}
		return common_positions;
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

//int height() {
//	return getHeight(root);
//}
//int getHeight(ptr root) {
//	if (root == nullptr) {
//		return -1;
//	}
//	int maxHeight = INT_MIN;
//	for (int i = 0; i < 26; i++) {
//		maxHeight = max(maxHeight, getHeight(root->next[i]) + 1);
//	}
//	return maxHeight;
//}
