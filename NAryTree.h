#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
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

class Trie {
	typedef TNode* ptr;
private:
	ptr root;
public:
	Trie() {
		root = new TNode(' ');
	}
	void insert(string word) {
		int i = 0;
		ptr curr = root;
		while (i < word.length()) {
			if (curr->next[word[i] - 'a'] == nullptr) {
				curr->next[word[i] - 'a'] = new TNode(word[i]);
			}
			curr = curr->next[word[i] - 'a'];
			i++;
		}
		curr->end = true;
	}
	bool search(string word) {
		ptr curr = root;
		for (int i = 0; i < word.length(); i++) {
			if (curr->next[word[i] - 'a'] == nullptr) {
				return false;
			}
			curr = curr->next[word[i] - 'a'];
		}
		return curr->end;
	}

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
		dotCode << "digraph Trie {\n";
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

	void visualizeTrie() {
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

//int main() {
//	Trie t;
//	for (auto word : { "banana", "bandit", "daniyal", "ban" }) {
//		t.insert(word);
//	}
//	cout << boolalpha << t.search("hello") << endl;
//	cout << boolalpha << t.search("mustafa") << endl;
//	cout << boolalpha << t.search("thison") << endl;
//	t.visualizeTrie();
//	cout << t.height() << endl;
//	cout << string(10, 'z');
//	return 0;
//}