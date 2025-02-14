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
  form_iterator type = cgi.getElement("Bible_type");
  // Convert and check input data
  bool validInput = false;
  bool validChap = false;
  bool validBook = false;
  bool validVerse = false;
  bool validVerseNum = false;

  int bookNumber = -5;
  int chapterNumber = -5;
  int verseNumber = -5;
  
  

  if (chapter != cgi.getElements().end()) {
	 chapterNumber = chapter->getIntegerValue();
	 if (chapterNumber > 150) {
		 cout << "<p>The chapter number (" << chapterNumber << ") is too high.</p>" << endl;
	 }
	 else if (chapterNumber <= 0) {
		 cout << "<p>The chapter must be a positive number.</p>" << endl;
	 }
	 else
		 validChap = true;
  }
  else{
	  cout << "<p> No chapter number input. Please input a chapter number between 1 and 150.</p>" << endl;
  }
  if (book != cgi.getElements().end()) {
	  bookNumber = book->getIntegerValue();
	  if (bookNumber > 66) {
		  cout << "<p> Invalid Book Input: Book Number ( " << bookNumber << " ) is too high. </p>" << endl;
	  }
	  else if (bookNumber < 1) {
		  cout << "<p> Invalid Book Input: Book Number  ( " << bookNumber << " ) is not a positive number. </p>" << endl;
	  }
	  else{
		  validBook = true;
	  }
  }
  else{
	  cout << "<p> No book number input. Please input a book number between 1 and 66.</p>" << endl;
  }
  
  /* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */
  if(verse != cgi.getElements().end()){
	  verseNumber = verse->getIntegerValue();
	  if (verseNumber < 0) {
		  cout << "<p> Invalid Verse Input: Verse Number ( " << verseNumber << " ) is not a positive number. </p>" << endl;
	  }
	  else if (verseNumber > 176) {
		  cout << "<p> Invalid Verse Input: Verse Number ( " << verseNumber << " ) is too high. </p>" << endl;
	  }
	  else {
		  validVerse = true;
	  }
  }
  else{
	  cout << "<p> No verse number input. Please input a verse number between ---- and ----.</p>" << endl;
  }
  if (validVerse && validBook && validChap) {
	  validInput = true;
  }

  
  /* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
   *        TO LOOK UP THE REQUESTED VERSES
   */

  
  
  LookupResult result;

  int multipleVerseNumbers = nv->getIntegerValue();
  if(nv == cgi.getElements().end()) {
	  cout << "<p> No number of verses. Please enter an amount of verses.</p>" << endl;
  }
  if (multipleVerseNumbers < 1) {
	  cout << "<p> Invalid Input: Number of verses. Please enter a positive amount of verses.</p>" << endl;
  }

  Bible typeBible;
  if (**type == "World English Bible") {
	  typeBible = Bible("/home/class/csc3004/Bibles/web-complete");
  }
  if (**type == "King James Version") {
	  typeBible = Bible("/home/class/csc3004/Bibles/kjv-complete");
  }
  if (**type == "Darby Translation") {
	  typeBible = Bible("/home/class/csc3004/Bibles/dby-complete");
  }
  if (**type == "Young's Literal Translation") {
	  typeBible = Bible("/home/class/csc3004/Bibles/ylt-complete");
  }
  if (**type == "Webster Translation") {
	  typeBible = Bible("/home/class/csc3004/Bibles/webster-complete");
  }

  Ref refObj(bookNumber, chapterNumber, verseNumber);
  Verse verseObj = typeBible.lookup(refObj,result);
  if (multipleVerseNumbers >= 1) {
	  for (int i = 0; i < multipleVerseNumbers; i++) {
		  if (validInput) {
			  if (result != OTHER && result != SUCCESS) {
				  cout << typeBible.error(result) << endl;
				  break;
			  }
			  // cout << "Search Type: <b>" << **st << "</b>" << endl;
			  cout << "<p>"
				  << typeBible.getCurrentRef().getBookName() << " " << typeBible.getCurrentRef().getChap() << ":" << typeBible.getCurrentRef().getVerse()
				  << " "
				  << verseObj.getVerse() << "</p>" << endl;
			  verseObj = typeBible.nextVerse(result);
		  
			  
		  }
		  
	  }
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
