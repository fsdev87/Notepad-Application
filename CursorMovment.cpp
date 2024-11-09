#include <iostream>
#include <Windows.h>
#include "Stack.h"
#include "NAryTree.h"
#include "codes/String.h"
using namespace std;

// Function prototypes
void initWindow();
void loadFile(Notepad& notepad, filesystem::path& filePath);
void undo(Notepad& notepad);
void redo(Notepad& notepad);
void clearSuggestionsArea();
void printSuggestions(String* words, int count);

// Global variables
Stack undoStack;
Stack redoStack;
int undoCount = 5;
int redoCount = 5;
//bool wordSuggestion = false;

// Trees
NAryTree searchTree;

// Helper Functions

// function to initialize window
void initWindow() {
	int i, j;
	for (i = 0; i <= MAX_Y; i++) {
		gotoxy(0, i);
		cout << '|';
	}
	i--;
	for (j = 1; j < MAX_X; j++) {
		gotoxy(j, i);
		cout << '_';
	}
	for (i = 0; i <= MAX_Y; i++) {
		gotoxy(j, i);
		cout << '|';
	}
	// print search and word suggestions
	gotoxy(MAX_X + 1, 0);
	cout << "Search";
	gotoxy(0, MAX_Y + 1);
	cout << "Word Suggestions";
}

void loadFile(Notepad& notepad, filesystem::path& filePath) {
	ifstream file(filePath);
	if (file) {
		notepad.readFromFile(filePath);
	}
	else {
		ofstream newFile(filePath);
		newFile.close();
	}
}

void undo(Notepad& notepad) {
	if (undoCount <= 0 || undoStack.isEmpty()) {
		return;
	}
	redoStack.push(notepad);
	notepad = undoStack.pop();
	undoCount--;
	if (redoCount < 5) {
		redoCount++;
	}
}

void redo(Notepad& notepad) {
	if (redoCount <= 0 || redoStack.isEmpty()) {
		return;
	}
	undoStack.push(notepad);
	notepad = redoStack.pop();
	redoCount--;
	if (undoCount < 5) {
		undoCount++;
	}
}

void clearSuggestionsArea() {
	int tempX = 0, tempY = MAX_Y + 3;
	gotoxy(tempX, tempY);
	for (int i = 0; i < MAX_X; i++) {
		cout << " ";
	}
	tempX = 0, tempY = MAX_Y + 5;
	gotoxy(tempX, tempY);
	for (int i = 0; i < MAX_X; i++) {
		cout << " ";
	}
}

void printSuggestions(String* words, int count) {
	int tempX = 0, tempY = MAX_Y + 5;
	gotoxy(tempX, tempY);
	if (words) {
		for (int i = 0; i < count; i++) {
			cout << i + 1 << ". " << words[i].getStr() << "  ";
		}
	}
	else {
		gotoxy(0, MAX_Y + 3);
		cout << "No suggestions found";
	}
}

// End Helper Functions

int main(int argc, char* argv[]) {
	// create notepad object
	Notepad notepad;

	system("cls");

	// menu
	char choice;
	filesystem::path filePath;
	cout << "Welcome to the Notepad!!!" << endl << endl;
	cout << "Instructions for the Notepad:" << endl;
	cout << "1. Use arrow keys to move the cursor" << endl;
	cout << "2. Use backspace to delete a character" << endl;
	cout << "3. Use enter to create a new line" << endl;
	cout << "4. Use '1' to undo" << endl;
	cout << "5. Use '2' to redo" << endl;
	cout << "6. Use 'ESC' to exit" << endl << endl;
	cout << "Menu:" << endl;
	cout << "1. Create a new file" << endl;
	cout << "2. Load a file" << endl;
	cout << "3. Exit" << endl;
	cout << "Your Choice: ";
	cin >> choice;
	if (choice == '1') {
		// create new file
		cout << "Enter the name of file you want to create: ";
		cin >> filePath;
		ofstream newFile(filePath);
		newFile.close();
	}
	else if (choice == '2') {
		// load file
		cout << "Enter the name of file you want to load: ";
		cin >> filePath;
		//if file p is found open it else create it
		loadFile(notepad, filePath);
	}
	else if (choice == '3') {
		// exit
		return 0;
	}
	else {
		cout << "Invalid choice" << endl;
		return 0;
	}


	system("cls");
	// initialize window
	initWindow();
	notepad.printList();

	undoStack.push(Notepad());

	HANDLE  rhnd = GetStdHandle(STD_INPUT_HANDLE);  // handle to read console

	DWORD Events = 0;     // Event count
	DWORD EventsRead = 0; // Events read from console

	bool Running = true;
	Node deletedChar('\0');
	bool undoFlag = false;
	bool redoFlag = false;

	gotoxy(notepad.cursorX, notepad.cursorY);
	// programs main loop
	while (Running) {

		// gets the systems current "event" count
		GetNumberOfConsoleInputEvents(rhnd, &Events);

		if (Events != 0) { // if something happened we will handle the events we want

			// create event buffer the size of how many Events
			INPUT_RECORD eventBuffer[200];

			// fills the event buffer with the events and saves count in EventsRead
			ReadConsoleInput(rhnd, eventBuffer, Events, &EventsRead);

			// loop through the event buffer using the saved count
			for (DWORD i = 0; i < EventsRead; ++i) {

				// check if event[i] is a key event && if so is a press not a release
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {

					// check if the key press was an arrow key
					switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {

					case VK_UP: //up
						notepad.goUp();
						break;

					case VK_DOWN: //down
						notepad.goDown();
						break;

					case VK_RIGHT: //right
						notepad.goRight();
						break;

					case VK_LEFT: //left
						notepad.goLeft();
						break;

					case VK_RETURN:
						if (undoFlag) {
							undoFlag = false;
							undoCount = 5;
							undoStack.clearStack();
							undoStack.push(notepad);
							redoCount = 5;
							redoStack.clearStack();
						}
						undoStack.push(notepad);
						notepad.createNewLine();
						notepad.printList();
						gotoxy(notepad.cursorX, notepad.cursorY);
						break;

					case VK_BACK:
						if (undoFlag) {
							undoFlag = false;
							undoCount = 5;
							undoStack.clearStack();
							undoStack.push(notepad);
							redoCount = 5;
							redoStack.clearStack();
						}
						if (notepad.cursor->value == ' ') {
							undoStack.push(notepad);
						}
						else if (notepad.cursor->value == '\0' && notepad.cursor->createdByEnter) {
							undoStack.push(notepad);
						}
						//deletedChar = notepad.deleteChar();
						notepad.deleteChar();
						notepad.printList();
						gotoxy(notepad.cursorX, notepad.cursorY);
						break;

					case VK_ESCAPE: //escape
						Running = false;
						system("cls");
						gotoxy(0, 0);
						cout << "Do you want to save the file? (y/n): ";
						cin >> choice;
						if (choice == 'y' || choice == 'Y') {
							cout << "Enter the name of file you want to save: ";
							cin >> filePath;
							notepad.saveToFile(filePath);
						}
						break;

					default:
						char ch = eventBuffer->Event.KeyEvent.uChar.AsciiChar;
						if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ') {
							if (undoFlag) {
								undoFlag = false;
								undoCount = 5;
								undoStack.clearStack();
								undoStack.push(notepad);
								redoCount = 5;
								redoStack.clearStack();
							}
							notepad.insertChar(ch);
							notepad.printList();
							gotoxy(notepad.cursorX, notepad.cursorY);
							if (ch == ' ' && notepad.cursor->left->value != ' ') {
								undoStack.push(notepad);
							}
							if (ch == ' ') {
								String word;
								Node* temp = notepad.cursor->left;
								while (temp->value != ' ' && temp->value != '\0') {
									word.appendStart(temp->value);
									temp = temp->left;
								}
								searchTree.insert(word);
							}
						}
						else if (ch == '@') {
							cout << ch;
							// word completion
							Node* temp = notepad.cursor; // the character at cursor
							String word; // tal
							while (temp->value != ' ' && temp->value != '\0') {
								word.appendStart(temp->value);
								temp = temp->left;
							}
							// now we have got the word before '@'
							String* words = searchTree.getWords(word); // all words from tal
							int count = searchTree.getCount(word);
							clearSuggestionsArea(); // clear
							printSuggestions(words, count); // print suggestions
							if (count > 0) {
								int suggestionChoice;
								gotoxy(0, MAX_Y + 3);
								cout << "Enter your choice: ";
								cin >> suggestionChoice;
								// now we need to insert it in the notepad
								if (suggestionChoice > 0 && suggestionChoice <= count) {
									suggestionChoice--;
									String selectedWord = words[suggestionChoice];
									temp = notepad.cursor;
									while (temp->value != ' ' && temp->value != '\0') {
										notepad.deleteChar();
										temp = notepad.cursor;
									}
									for (int i = 0; i < selectedWord.getLength(); i++) {
										notepad.insertChar(selectedWord[i]);
									}
									clearSuggestionsArea();
									notepad.printList();
									gotoxy(notepad.cursorX, notepad.cursorY);
								}
							}
							else {
								gotoxy(notepad.cursorX, notepad.cursorY);
								cout << ' ';
								gotoxy(notepad.cursorX, notepad.cursorY);
							}
						}
						else if (ch == '1') {
							if (!undoFlag) {
								undoFlag = true;
							}
							undo(notepad);
							notepad.printList();
							gotoxy(notepad.cursorX, notepad.cursorY);
						}
						else if (ch == '2') {
							if (!redoFlag) {
								redoFlag = true;
							}
							redo(notepad);
							notepad.printList();
							gotoxy(notepad.cursorX, notepad.cursorY);
						}
						break;
					}
				}

			} // end EventsRead loop

		}

	} // end program loop
	/*String str = "tal";
	int count = searchTree.getCount(str);
	cout << "Count: " << count << endl;
	String* words = searchTree.getWords(str);
	if (words) {
		for (int i = 0; i < count; i++) {
			if (words[i].getStr()) {
				cout << words[i].getStr() << "   ";
			}
		}
	}
	else {
		cout << "No matches found" << endl;
	}*/
	searchTree.visualizeNAryTree();

	return 0;
}
