#pragma once
#include "Node.h"
#include <iostream>
#include <fstream>
using namespace std;

const int MAX_X = 20;
const int MAX_Y = 23;

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Notepad {
public:
	Node* head;
	Node* cursor;
	int cursorX, cursorY;


public:
	Notepad() {
		head = new Node('\0');
		cursor = head;
		cursorX = 1, cursorY = 0;
	}

	bool isFull() {
		// this function will check if any text can be added without exceeding the max
		// length of the notepad
		int lines = getNoOfLines();
		if (lines < MAX_Y) {
			return false;
		}
		//Node* lineHead = getLineHead(cursor);
		Node* lastLine = head;
		while (lastLine->down) {
			lastLine = lastLine->down;
		}
		int sizeOfLine = getSizeOfRow(lastLine);
		if (sizeOfLine < MAX_X - 1) {
			return false;
		}
		return true;
	}

	void insertAtEnd(char ch) {
		// move character ahead
		Node* newNode = new Node(ch);
		cursor->right = newNode;
		cursor->right->left = cursor;
		cursor = cursor->right; // move cursor forward
		cursorX++;
	}

	void insertAtMiddle(char ch) {
		// insert character in between
		Node* newNode = new Node(ch);
		newNode->right = cursor->right;
		cursor->right->left = newNode;
		cursor->right = newNode;
		newNode->left = cursor;
		cursor = cursor->right;
		cursorX++;
	}

	void insertChar(char ch) {
		if (isFull()) return;

		if (cursor->right) {
			insertAtMiddle(ch);
		}
		else {
			insertAtEnd(ch);
		}
		wrapList();
		locateCursor();
		makeLinks(head);
	}

	void deleteSingleChar() {
		Node* temp = cursor;
		cursor = cursor->left;
		if (!temp->right) {
			cursor->right = nullptr;
		}
		else {
			cursor->right = temp->right;
			temp->right->left = cursor;
		}
		delete temp;
	}

	void deleteLine() {
		if (!cursor->right) {
			// we are deleting an empty line
			Node* temp = cursor;
			cursor = cursor->up;
			cursor->down = temp->down;
			if (temp->down) {
				temp->down->up = cursor;
			}
			delete temp;
			while (cursor->right) {
				cursor = cursor->right;
			}
		}
		else {
			// we are deleting a line with some text
			Node* current = cursor;
			cursor = cursor->up;
			while (cursor->right) {
				cursor = cursor->right;
			}
			// now cursor is at the end of the previous line, and more text need to be added from here
			Node* temp = current;
			current = current->right; // move to the first character of the line
			temp->up->down = temp->down;
			if (temp->down) {
				temp->down->up = temp->up;
			}
			cursor->right = current;
			current->left = cursor;
			delete temp;
		}
	}

	void deleteChar() {
		// no need to do anything if we are already at the start of the notepad
		if (cursor == head) {
			return;
		}

		if (cursor->value == '\0') {
			deleteLine();
		}
		else {
			deleteSingleChar();
			retractList();
		}
		wrapList();
		locateCursor();
		makeLinks(head);
	}

	void makeLinks(Node* rowHead) {
		if (rowHead == nullptr) return;
		Node* up = rowHead;
		Node* down = rowHead->down;
		while (up || down) {
			if (up && down) {
				up->down = down;
				down->up = up;
			}
			else if (up && !down) {
				up->down = nullptr;
			}
			else if (!up && down) {
				down->up = nullptr;
			}
			if (up) {
				up = up->right;
			}
			if (down) {
				down = down->right;
			}
		}
		makeLinks(rowHead->down);
	}

	int getSizeOfRow(Node* head) {
		int size = 0;
		while (head) {
			head = head->right;
			size++;
		}
		return size;
	}

	/*
	the function we are going to write now will make the notepad behave like a real one.
	which means that what this function is gonna do is that it is gonna retract the list
	for example, if we keep writing text such that it exceeds the line, and wraps to the next line
	the new line created will have the boolean false as this line is not created by enter
	so this function will check if after deleting any character, if the line following it
	is not created by enter, and the text of the following line can fit inside the current line
	from where the text is being deleted from, then it will move the text from the following line
	to the previous line, and then rewrap the list again
	*/

	void retractList() {
		Node* rowHead = getLineHead(cursor);
		if (!rowHead->down) {
			return; // there is nothing below
		}
		if (rowHead->down->createdByEnter) {
			return; // no relation with line created by enter
		}

		int sizeOfLine = getSizeOfRow(rowHead);
		int sizeOfNextWord = 0;
		Node* nextLineWord = rowHead->down;
		while (nextLineWord->value != ' ' && nextLineWord->right) {
			sizeOfNextWord++;
			nextLineWord = nextLineWord->right;
		}
		if (MAX_X - sizeOfLine > sizeOfNextWord) {
			// now start retraction of list
			Node* current = rowHead;
			Node* temp = rowHead;
			while (rowHead->down && !rowHead->down->createdByEnter) {
				// move current to end of current line
				while (current->right) {
					current = current->right;
				}
				current->right = rowHead->down->right;
				if (rowHead->down->right) {
					rowHead->down->right->left = current;
				}
				temp = rowHead->down;
				rowHead->down = temp->down;
				if (temp->down) {
					temp->down->up = rowHead;
				}
				delete temp;

			}

		}

	}

	void wrapList() {
		// in this function, we need to implement the same logic as wrapLine but for the whole list
		Node* rowHead = head;
		while (rowHead) {
			Node* current = rowHead;
			if (current->right) {
				current = current->right;

				int size = getSizeOfRow(rowHead);

				if (size < MAX_X) {
					rowHead = rowHead->down;
					continue;
				}
				else {
					current = rowHead;
					int idx = 0;
					while (current->right && idx < MAX_X) {
						current = current->right;
						idx++;
					}
					while (current && current->left && current->left->value != ' ') {
						current = current->left;
					}
					Node* temp = current;
					while (temp->right) {
						temp = temp->right;
					}

					// current at start of last word, temp at end of last word

					if (!rowHead->down) {
						addLine(current);
					}
					else if (rowHead->down->createdByEnter) {
						Node* newNode = new Node('\0');
						newNode->up = rowHead;
						newNode->down = rowHead->down;
						rowHead->down->up = newNode;
						rowHead->down = newNode;
					}
					// current is at first letter, temp is at last letter
					Node* currentHead = getLineHead(current);
					currentHead = currentHead->down;
					// currentHead is at head of next line
					if (current->left) {
						current->left->right = nullptr; // remove link from previous line
					}
					temp->right = currentHead->right; // link the word to below line
					if (currentHead->right) {
						currentHead->right->left = temp; // link back
					}
					current->left = currentHead; // point to head of next line
					currentHead->right = current; // next line head point to current

				}

			}
			rowHead = rowHead->down;
		}
	}

	void addLine(Node* node) {
		Node* lineHead = getLineHead(node);
		Node* newNode = new Node('\0');
		lineHead->down = newNode;
		lineHead->down->up = lineHead;
		//cursor = lineHead->down;
		cursorY++;
		cursorX = 1;
	}

	// function to get the head of a line
	Node* getLineHead(Node* node) {
		while (node && node->left) {
			node = node->left;
		}
		return node;
	}

	void locateCursor() {
		cursorX = 1, cursorY = 0;
		Node* row = head;
		while (row) {
			Node* col = row;
			while (col) {
				if (col == cursor) {
					return;
				}
				col = col->right;
				cursorX++;
			}
			cursorX = 1;
			cursorY++;
			row = row->down;
		}
	}

	int getNoOfLines() {
		Node* row = head;
		int size = 0;
		while (row) {
			row = row->down;
			size++;
		}
		return size;
	}

	// print the whole 2d list
	void printList() {
		int lines = getNoOfLines();
		/*int y = 0;
		while (y < MAX_Y) {
			gotoxy(1, y++);
			for (int i = 1; i < MAX_X; i++) {
				cout << ' ';
			}
		}*/
		int y = 0;
		Node* row = head;
		while (row) {
			gotoxy(1, y++);
			for (int i = 1; i < MAX_X; i++) {
				cout << ' ';
			}
			row = row->down;
			if (row == nullptr && lines < MAX_Y) {
				// clear once more
				gotoxy(1, y);
				for (int i = 1; i < MAX_X; i++) {
					cout << ' ';
				}
			}
		}

		y = 0;
		//Node* row = head;
		row = head;
		while (row && y < MAX_Y) {
			gotoxy(1, y++);
			Node* col = row;
			if (col->right) {
				col = col->right;
				while (col) {
					cout << col->value;
					col = col->right;
				}
			}
			row = row->down;
		}
	}

	void createNewLine() {
		int lines = getNoOfLines();
		if (lines >= MAX_Y) {
			return;
		}
		/*if (cursorY >= MAX_Y) {
			return;
		}*/
		cursorX = 1, cursorY++;
		Node* rowHead = getLineHead(cursor);
		Node* newNode = new Node('\0', true);

		// this part was about adding the node either at end of list or in between
		if (!rowHead->down) {
			rowHead->down = newNode;
			rowHead->down->up = rowHead;
		}
		else {
			newNode->up = rowHead;
			newNode->down = rowHead->down;
			rowHead->down->up = newNode;
			rowHead->down = newNode;
		}

		// this part is about checking if the new line is going to be empty or filled with rest
		// of previous line
		if (cursor->right) {
			/*
				here we need to implement logic for joining the whole line till it is not created
				by enter to the new line
			*/
			cursor = cursor->right;
			cursor->left->right = nullptr;
			newNode->right = cursor;
			cursor->left = newNode;
			Node* current = newNode, * row = newNode;
			Node* temp = newNode;
			while (row->down && !row->down->createdByEnter) {
				// move current to end of current line
				while (current->right) {
					current = current->right;
				}
				current->right = row->down->right;
				if (row->down->right) {
					row->down->right->left = current;
				}
				temp = row->down;
				row->down = temp->down;
				if (temp->down) {
					temp->down->up = row;
				}
				delete temp;
			}
		}
		cursor = newNode; // move to new line
		wrapList();
		locateCursor();
		makeLinks(head);
	}

	void goUp() {
		if (cursorY > 0) {
			if (cursor->up != nullptr) {
				cursorY--;
				gotoxy(cursorX, cursorY);
				// update cursor
				cursor = cursor->up;
			}
			else {
				Node* lineHead = getLineHead(cursor);
				cursor = lineHead->up;
				while (cursor->right) {
					cursor = cursor->right;
				}
				locateCursor();
				gotoxy(cursorX, cursorY);
			}
		}
	}

	void goDown() {
		if (cursorY < MAX_Y - 1) {
			if (cursor->down != nullptr) {
				cursorY++;
				gotoxy(cursorX, cursorY);
				// update cursor
				cursor = cursor->down;
			}
			else {
				Node* lineHead = getLineHead(cursor);
				if (lineHead->down) {
					cursor = lineHead->down;
					while (cursor->right) {
						cursor = cursor->right;
					}
					locateCursor();
					gotoxy(cursorX, cursorY);
				}
			}
		}
	}

	void goLeft() {
		if (cursorX > 1 && cursor->left != nullptr) {
			cursorX--;
			gotoxy(cursorX, cursorY);
			// update cursor
			cursor = cursor->left;
		}
		else if (cursorX == 1 && cursorY > 0) {
			cursor = cursor->up;
			while (cursor->right) {
				cursor = cursor->right;
			}
			locateCursor();
			gotoxy(cursorX, cursorY);
		}
	}

	void goRight() {
		if (cursorX < MAX_X - 1 && cursor->right != nullptr) {
			cursorX++;
			gotoxy(cursorX, cursorY);
			// update cursor
			cursor = cursor->right;
		}
		else if (cursor->right == nullptr && cursorY < MAX_Y - 1) {
			Node* lineHead = getLineHead(cursor);
			if (lineHead->down) {
				cursor = lineHead->down;
				locateCursor();
				gotoxy(cursorX, cursorY);
			}
		}
	}

	~Notepad() {
		Node* row = head;
		while (row != nullptr) {
			Node* current = row;
			row = row->down; // move to next line

			// now delete all nodes in the current row
			while (current != nullptr) {
				Node* temp = current;
				current = current->right; // move to the next node in the line
				delete temp; // free the current node
			}
		}
	}

};