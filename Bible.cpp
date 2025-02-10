// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	instream = ifstream(infile);
	currentLine = "";
	isOpen = true;
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; instream = ifstream(infile); isOpen = true; }

string Bible::getLine() { return currentLine; }

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
	
	string refToFind = ref.toString();

	status = OTHER;
	currentRef = ref;
	currentVerse = Verse();
	
	//Translate the ref into book that will exist regardless of chap and verse
	//Do the same but for a chapter in said book

	string bookCheck = to_string(ref.getBook()) + ":1:1";
	string chCheck = to_string(ref.getBook()) + ":" + to_string(ref.getChap()) + ":1";
	
	//66:22:21 - marks end of file
	// Booleans to check for the existence of book, chapter, verse.ma
	bool bookExists=false;
	bool chapExists=false;
	bool verseExists=false;

	
	if (checkIfOpen()) {
		while (getline (instream, currentLine) ){
			if (currentLine.find(refToFind, 0) != string::npos) {
				currentVerse = Verse(getCurrentLine());
				currentRef = currentVerse.getRef();
				currentVerse.getRef() = currentRef;
				status = SUCCESS;
				return(currentVerse);
			}
			if (currentLine.find(bookCheck, 0) != string::npos) {
				bookExists = true;
			}
			if (currentLine.find(chCheck, 0) != string::npos) {
				chapExists = true;
			}

		}
	}
	
	//At this point it will have gone through the whole Bible, if the variables have changed then they do exist
	if (chapExists && bookExists && !verseExists) {
		status = NO_VERSE;
	}
	if (!chapExists && !verseExists && !bookExists) {
		status = NO_BOOK;
	}
	if (bookExists && !chapExists && !verseExists) {
		status = NO_CHAPTER;
	}
	
	//Check if the reference exists
	//First check if the # for the book is within the 1-66, then check the specific book to find the chapter, then verse
	
	// create and return the verse object
	// that is constructed from a line in the file.
	
	//Use a series of if statements to check for the truth values of the booleans.
	
	
	                
    return(currentVerse);
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status) {
	if (status == SUCCESS) {


		if (checkIfOpen()) {
			if (currentLine.find("66:22:21", 0) == string::npos) {
				getline(instream, currentLine);

				currentVerse = Verse(getCurrentLine());
				currentRef = currentVerse.getRef();
			}
			else {
				ifstream firstlineStream(infile);
				string firstVerse;
				getline(firstlineStream, firstVerse);
				currentVerse = Verse(firstVerse);
				currentRef = currentVerse.getRef();
			}

		}
		else {
			ifstream firstlineStream(infile);
			string firstVerse;
			getline(firstlineStream, firstVerse);
			currentVerse = Verse(firstVerse);
			currentRef = currentVerse.getRef();
		}
	}
	return currentVerse;
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status) {
	string output;
	
	if (status == NO_BOOK) {
		output = "Error: no such book " + to_string(currentRef.getBook());
	}
	if (status == NO_CHAPTER) {
		output = "Error: no such chapter " + to_string(currentRef.getChap()) + " in " + currentRef.getBookName();
	}
	if (status == NO_VERSE) {
		output = "Error: no such verse " + to_string(currentRef.getVerse()) + " in " + currentRef.getBookName() + " " + to_string(currentRef.getChap());
	}
	if (status == OTHER) {
		output = "Other Error";
	}
	if (status == SUCCESS) {
		output = "SUCCESS";
	}
	return output;
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}

bool Bible::checkIfOpen() {
	if (instream.is_open()) {
		return true;
	}
	return false;
}

void Bible::openFile() {instream.open(infile, std::ifstream::in);}

void Bible::closeFile() {instream.close();}

Ref Bible::getCurrentRef() { return currentRef; }

Verse Bible::getCurrentVerse() { return currentVerse; }

string Bible::getCurrentLine() { return currentLine; }
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
//Ref Bible::next(const Ref ref, LookupResult& status) {}
//
//// OPTIONAL: Return the reference before the given ref
//Ref Bible::prev(const Ref ref, LookupResult& status) {}
