/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 * 
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching 
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Ref.h"
#include "Verse.h"
#include "Bible.h"

using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

using namespace cgicc;

int main() {
  /* A CGI program must send a response header with content type
   * back to the web client before any other output.
   * For an AJAX request, our response is not a complete HTML document,
   * so the response type is just plain text to insert into the web page.
   */
  cout << "Content-Type: text/plain\n\n";
  
  Cgicc cgi;  // create object used to access CGI request data

  // GET THE INPUT DATA
  // browser sends us a string of field name/value pairs from HTML form
  // retrieve the value for each appropriate field name
  //form_iterator st = cgi.getElement("search_type");
  form_iterator book = cgi.getElement("book");
  form_iterator chapter = cgi.getElement("chapter");
  form_iterator verse = cgi.getElement("verse");
  form_iterator nv = cgi.getElement("num_verse");

  // Convert and check input data
  bool validInput = false;
  bool validChap = false;
  bool validBook = false;
  bool validVerse = false;
  bool validVerseNum = false;

  int bookNumber = 0;
  int chapterNumber = 0;
  int verseNumber = 0;
  int multipleVerseNumbers = 0;

  if (chapter != cgi.getElements().end()) {
	 int chapterNumber = chapter->getIntegerValue();
	 if (chapterNumber > 150) {
		 cout << "<p>The chapter number (" << chapterNumber << ") is too high.</p>" << endl;
	 }
	 else if (chapterNumber <= 0) {
		 cout << "<p>The chapter must be a positive number.</p>" << endl;
	 }
	 else
		 validChap = true;
  }
  else {
	  cout << "<p> No chapter number input. Please input a chapter number between 1 and 150.</p>" << endl;
  }
  if (book != cgi.getElements().end()) {
	  if (bookNumber > 66 || bookNumber < 1) {
		  cout << "<p> Invalid Book Input: Book Number ( " << bookNumber << " ) is too high. </p>" << endl;
	  }
	  else if (bookNumber < 1) {
		  cout << "<p> Invalid Book Input: Book Number  ( " << bookNumber << " ) is not a positive number. </p>" << endl;
	  }
	  else{
		  validBook = true;
	  }
  }
  else {
	  cout << "<p> No book number input. Please input a book number between 1 and 66.</p>" << endl;
  }
  
  /* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */
  if(verse != cgi.getElements().end()){
	  if (verseNumber <= 0) {
		  cout << "<p> Invalid Verse Input: Verse Number ( " << verseNumber << " ) is not a positive number. </p>" << endl;
	  }
	  else {
		  validVerse = true;
	  }
  }
  else {
	  cout << "<p> No verse number input. Please input a verse number between ---- and ----.</p>" << endl;
  }
  if (validVerse && validBook && validChap) {
	  validInput = true;
  }
  
  /* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
   *        TO LOOK UP THE REQUESTED VERSES
   */
  Bible webBible("/home/class/csc3004/Bibles/web-complete");
  
  LookupResult result;

  Ref refObj(bookNumber, chapterNumber, verseNumber);
  Verse verseObj;
  if (multipleVerseNumbers >= 1) {
	  for (int i = 0; i < multipleVerseNumbers; i++) {
		  if (validInput) {
			  // cout << "Search Type: <b>" << **st << "</b>" << endl;
			  cout << "<p>Your result: "
				  << refObj.getBookName() << " " << **chapter << ":" << **verse
				  << " "
				  << **nv
				  << verseObj.getVerse() << "</p>" << endl;
			  verseObj = webBible.nextVerse(result);
		  }
		  if (result != OTHER) {
			  cout << webBible.error(result) << endl;
		  }
	  }
	  
	  //Check and make sure the BOOK, CHAPTER, VERSE are correctly entered, 
	  //As well as translation later.
	  //Need to make sure you know what 
  }
  

  /* SEND BACK THE RESULTS
   * Finally we send the result back to the client on the standard output stream
   * in HTML text format.
   * This string will be inserted as is inside a container on the web page, 
   * so we must include HTML formatting commands to make things look presentable!
   */


 // if (validInput) {
	//cout << "Search Type: <b>" << **st << "</b>" << endl;
	//cout << "<p>Your result: "
	//	 << **book << " " << **chapter << ":" << **verse 
	//	 << " " 
	//	// This is to display the number of verses, its italicized << **nv

	//	 <<  verseObj.getVerse() <<"</p>" << endl;
 // }
 // else {
	//  cout << "<p>Invalid Input: <em>report the more specific problem.</em></p>" << endl;
 // }
  return 0;
}
