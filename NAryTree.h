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
public:
	TNode* root;

	NAryTree() {
		root = new TNode(' ');
	}
	void insert(String word, int lineNumber = 0) {
		TNode* current = root;
		for (int i = 0; i < word.getLength(); i++) {
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
		}
		current->end = true;
	}

	bool search(String word) {
		TNode* current = root;
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
		if (current->end) {
			return true;
		}
		return false;
	}

	int getCount(String word) {
		TNode* current = root;
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

	int countWords(TNode* root) {
		if (root == nullptr) {
			return 0;
		}
		int count = 0;
		if (root->end == true) {
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

		TNode* current = root;
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

	void findWords(TNode* root, String* words, int& idx, String word) {
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

	void dfs(TNode* root) {
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

	void print(TNode* root, Vector<char> word) {
		if (root == nullptr) {
			return;
		}
		word.insert(root->key);
		if (root->end) {
			for (int i = 0; i < word.size; i++) {
				cout << word[i] << " -> ";
			}
			cout << endl;
		}
		else {
			for (int i = 0; i < 26; i++) {
				print(root->next[i], word);
			}
		}
		word.pop_back();
	}

	Vector<int>* searchWord(String word) {
		TNode* current = root;
		if (current == nullptr) return nullptr;

		Vector<int>* commonPos = nullptr;
		int i = 0;
		while (i < word.getLength()) {
			char c = word[i];
			int index;
			if (c >= 'A' && c <= 'Z') {
				index = c - 'A';
			}
			else {
				index = c - 'a';
			}

			if (current->next[index] == nullptr) {
				return nullptr;
			}

			current = current->next[index];

			if (i != 0) {
				Vector<int> tempCommonPos;
				int x = 0, y = 0;

				while (x < commonPos->size && y < current->lineNumbers.size) {
					if ((*commonPos)[x] == current->lineNumbers[y]) {
						tempCommonPos.push_back((*commonPos)[x]);
						x++;
						y++;
					}
					else if ((*commonPos)[x] < current->lineNumbers[y]) {
						x++;
					}
					else {
						y++;
					}
				}

				if (!tempCommonPos.size) {
					delete commonPos;
					return nullptr;
				}

				*commonPos = tempCommonPos;
			}
			else {
				commonPos = new Vector<int>(current->lineNumbers);
			}
			i++;
		}
		return commonPos;
	}

	string generateDotCode() {
		stringstream dotCode;
		dotCode << "digraph NAryTree {\n";
		dotCode << "  node [shape=circle];\n";
		queue<TNode*> q;
		queue<int> idq;
		q.push(root);
		int id = 0;
		idq.push(id);
		while (!q.empty()) {
			TNode* current = q.front();
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