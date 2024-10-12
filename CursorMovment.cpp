#include <iostream>
#include <Windows.h>
#include "Stack.h"
using namespace std;

// Function prototypes
void gotoxy(int x, int y);
void initWindow();
void loadFile(Notepad& notepad, filesystem::path& filePath);
void undo(Notepad& notepad);
void redo(Notepad& notepad);

// Global variables
Stack undoStack;
Stack redoStack;
int undoCount = 5;
int redoCount = 5;


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



// End Helper Functions


int main(int argc, char* argv[]) {
	// create notepad object
	Notepad notepad;

	system("cls");

	// menu
	char choice;
	filesystem::path filePath;
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
	//programs main loop
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
							if (ch == ' ') {
								undoStack.push(notepad);
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

	return 0;
}