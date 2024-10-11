#include <iostream>
#include <Windows.h>
#include "Stack.h"
using namespace std;

// Function prototypes
void gotoxy(int x, int y);
void initWindow();
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

void undo(Notepad& notepad) {
	redoStack.push(notepad);
	notepad = undoStack.pop();
}

void redo(Notepad& notepad) {
	undoStack.push(notepad);
	notepad = redoStack.pop();
}



// End Helper Functions


int main(int argc, char* argv[]) {

	system("cls");
	// initialize window
	initWindow();

	// create notepad object
	Notepad notepad;
	Notepad notepad2;


	HANDLE  rhnd = GetStdHandle(STD_INPUT_HANDLE);  // handle to read console

	DWORD Events = 0;     // Event count
	DWORD EventsRead = 0; // Events read from console

	bool Running = true;
	char deletedChar;

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

					case VK_CONTROL: //control, print all list at position
						//notepad.generateDOT();
						break;

					case VK_RETURN:
						notepad.createNewLine();
						notepad.printList();
						gotoxy(notepad.cursorX, notepad.cursorY);
						break;

					case VK_BACK:
						deletedChar = notepad.deleteChar();
						notepad.printList();
						gotoxy(notepad.cursorX, notepad.cursorY);
						if (deletedChar == ' ') {
							undoStack.push(notepad);
						}
						break;

					case VK_ESCAPE: //escape
						Running = false;
						notepad2 = undoStack.peek();
						notepad.writeToFile("notepad_output.txt");
						notepad2.writeToFile("notepad2_output.txt");
						break;

					default:
						char ch = eventBuffer->Event.KeyEvent.uChar.AsciiChar;
						if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ') {
							notepad.insertChar(ch);
							notepad.printList();
							gotoxy(notepad.cursorX, notepad.cursorY);
							if (ch == ' ') {
								undoStack.push(notepad);
							}
						}
						else if (ch == '1') {
							undo(notepad);
							notepad.printList();
							gotoxy(notepad.cursorX, notepad.cursorY);
						}
						else if (ch == '2') {
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