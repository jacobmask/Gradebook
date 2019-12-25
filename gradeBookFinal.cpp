/* Author: <PUT YOUR NAME HERE>
   Description:  Grade book program that allows a user to interactively
   save and restore class scores to/from a text file.
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>

using namespace std;

// Global Constant
const int CLASS_SIZE = 100;
const int MAX_SCORES = 50;

// Prototypes
void readScoresFromFile(string& fileName, string students[], string columnTitles[],
	double scores[][MAX_SCORES], int& studentCount, int& scoreCount);
void printScores(string students[], string columnTitles[], double scores[][MAX_SCORES],
	int studentCount, int scoresCount);
void gradeBookMenu(string students[], string columnTitles[], double scores[][MAX_SCORES],
	int& studentCount, int& scoreCount);
double averageScores(double scores[], int scoreCount);
void possiblySaveScoresToFile(string students[], string columnTitles[],
	double scores[][MAX_SCORES], int& studentCount, int& scoreCount);
void saveScoresToFile(string fileName, string students[], string columnTitles[],
	double scores[][MAX_SCORES], int& studentCount, int& scoreCount);
void displayMenu();
void listSearch(string target, string stringList[], int stringCount, bool& found, int& locationFound);
double averageColumn(double scores[][MAX_SCORES], int columnNumber, int studentCount, int scoreCount);
void add_column_of_scores(string columnTitles[], double scores[][MAX_SCORES], int studentCount, int& scoreCount);
void add_new_student(string students[], string columnTitles[], double scores[][MAX_SCORES], int& studentCount, int scoreCount);
void change_score(string students[], string columnTitles[], double scores[][MAX_SCORES], int studentCount, int scoreCount);

int main() {
	double scores[CLASS_SIZE][MAX_SCORES];
	string studentNames[CLASS_SIZE], columnTitles[MAX_SCORES];
	string fileName;
	int numberOfScores, numberOfStudents;

	readScoresFromFile(fileName, studentNames, columnTitles, scores, numberOfStudents, numberOfScores);

	printScores(studentNames, columnTitles, scores, numberOfStudents, numberOfScores);

	gradeBookMenu(studentNames, columnTitles, scores, numberOfStudents, numberOfScores);

	possiblySaveScoresToFile(studentNames, columnTitles, scores, numberOfStudents, numberOfScores);

} // end main


/* This function reads a grade book file into the arrays */
void readScoresFromFile(string& fileName, string students[], string columnTitles[],
	double scores[][MAX_SCORES], int& studentCount, int& scoreCount) {
	int row, column;
	double score;
	ifstream inFile;

	do  //ask the user to enter the grade book file name for reading in. Loop until the file can be opened successfully.
	{
		cout << "Enter file name: ";
		getline(cin, fileName);
		inFile.open(fileName.c_str());

		if (!inFile)
		{
			cout << "Wrong file name. Error opening file <" << fileName << "> for reading!\n";
		}
		else
		{
			inFile >> studentCount;     //obtain the total number of students
			inFile >> scoreCount;		//obtain the total number of columns for each student
			inFile.ignore(100, '\n');   //ignore the rest until a new line

			//read score column titles
			for (column = 0; column < scoreCount; column++)
			{
				getline(inFile, columnTitles[column]);
			} // end for (column...

			for (row = 0; row < studentCount; row++)
			{
				getline(inFile, students[row]);  //read in student names

				for (column = 0; column < scoreCount; column++)
				{
					inFile >> score;
					scores[row][column] = score;
				} // end for (column...

				inFile.ignore(100, '\n');
			} // end for (row...      

			inFile.close();
		} // end if
	} while (!inFile);

	cout << "read sucessfully\n";

} // end readScoresFromFile


/* This function prints grade book scores in arrays to console in a nice format */
void printScores(string students[], string columnTitles[], double scores[][MAX_SCORES],
	int studentCount, int scoreCount) {
	int row, column;

	cout << "There are " << studentCount << " students with " << scoreCount << " score columns." << endl << endl;

	// Print Column Titles
	cout << left << setw(17) << "Student Names";
	for (column = 0; column < scoreCount; column++) {
		cout << right << setw(6) << columnTitles[column] << " ";
	} // end for (column...
	cout << endl;

	// Print student names,  scores, and average score
	for (row = 0; row < studentCount; row++) {
		cout << left << setw(15) << students[row];
		cout << ": ";
		for (column = 0; column < scoreCount; column++) {
			cout << right << setw(6) << scores[row][column] << " ";
		}
		cout << "Average Score = " << fixed << setprecision(1) << averageScores(scores[row], scoreCount) << endl;
	}

	// print column averages
	cout << left << setw(17) << "Column Averages";
	for (column = 0; column < scoreCount; column++) {
		cout << right << setw(6) << averageColumn(scores, column, studentCount, scoreCount) << " ";
	} // end for (column...
	cout << endl << endl;

} // end printScores

/* Returns the average of a row in the 2D scores array */
/* Add new function */
double averageScores(double scores[], int scoreCount) {
	double average = 0;
	//obtaint the average score of a column
	int column;
	double total;

	if (scoreCount <= 0) {
		cout << "Error: To calculate the average, you must have one or more scores!" << endl;
		exit(EXIT_FAILURE);
	} // end if

	  // obtain the total for a column
	total = 0;
	for (column = 0; column < scoreCount; column++) {
		total += scores[column];
	}

	average = total / scoreCount;

	return average;
} // end averageScores


/* Returns the average of a column in the 2D scores array */
double averageColumn(double scores[][MAX_SCORES], int columnNumber, int studentCount, int scoreCount) {
	double total;
	int row;

	if (studentCount <= 0) {
		cout << "Error: To calculate the average, you must have one or more scores!" << endl;
		exit(EXIT_FAILURE);
	} // end if

	  // obtain the total for a column
	total = 0;
	for (row = 0; row < studentCount; row++) {
		total += scores[row][columnNumber];
	}

	return (total / studentCount);

} // end averageColumn

/* Add new column of scores*/
void add_column_of_scores(string columnTitles[], double scores[][MAX_SCORES], int studentCount, int& scoreCount)
{
	string subjectName;
	int column;
	do  //ask the user to enter the grade book file name for reading in. Loop until the file can be opened successfully.
	{
		cout << "Enter the subject name: ";
		getline(cin, subjectName);

	} while (subjectName.empty());

	scoreCount++;	// increase a column of scores
	if( scoreCount > MAX_SCORES )
	{
		scoreCount = MAX_SCORES;
		cout << "It can't add a new column of the score. Maximum size exceeded" << endl;;
		return;
	}

	columnTitles[scoreCount - 1] = subjectName;
	for (column = 0; column < studentCount; column++) {
		scores[column][scoreCount-1] = 0;	// initialize
	}

	cout << "add successfuly!";
	cout << endl;

}
/* Add new student */
void add_new_student(string students[], string columnTitles[], double scores[][MAX_SCORES], int &studentCount, int scoreCount)
{
	int bad_input;
	float fScore;
	int x, y;
	string studentName;
	int row;
	do  //ask the user to enter the grade book file name for reading in. Loop until the file can be opened successfully.
	{
		cout << "Enter the student name: ";
		getline(cin, studentName);

	} while (studentName.empty());


	studentCount++;
	if (studentCount > CLASS_SIZE)
	{
		studentCount = CLASS_SIZE;
		cout << "It can't add a new student. Maximum size exceeded" << endl;;
		return;
	}

	students[studentCount - 1] = studentName;


	for (row = 0; row < scoreCount; row++)
	{
		cout << studentName << ":" << columnTitles[row] << "'s score: ";
		do {
			bad_input = 0;
			cin >> fScore;
			if (!cin)
			{
				bad_input = 1;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Input the right number" << endl;
			}

		} while (bad_input);

		scores[studentCount - 1][row] = fScore;
	}
	cout << studentName << "'s score ";
	cout << "added successfuly!";
	cout << endl;
}

void change_score(string students[], string columnTitles[], double scores[][MAX_SCORES], int studentCount, int scoreCount)
{
	char str[100];
	float fScore;
	int x, y;
	int  bad_input;
	string studentName;
	int row;
	int column;
	do  //ask the user to enter the grade book file name for reading in. Loop until the file can be opened successfully.
	{
		cout << "Enter student name for updating the score in first: ";
		getline(cin, studentName);

	} while (studentName.empty());

	for (row = 0; row < studentCount; row++)
	{
		if( studentName.compare(students[row]) )
			continue;
		break;
	}
	if (row >= studentCount)
	{
		cout << "Student name is not exist" << endl;
		return;
	}

	

	do  //ask the user to enter the grade book file name for reading in. Loop until the file can be opened successfully.
	{
		cout << "Enter the subject name of " << students[row] << ": ";
		getline(cin, studentName);

	} while (studentName.empty());


	for (column = 0; column < scoreCount; column++)
	{
		if( studentName.compare(columnTitles[column]) )
			continue;
		break;

	}
	if (column >= scoreCount)
	{
		cout << "subject name is not exist" << endl;;
		return;
	}

	
	do {
		bad_input = 0;
		cout << "Enter the score: ";
		cin >> fScore;
		if (!cin)
		{
			bad_input = 1;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Input the right number" << endl;
		}

	} while (bad_input);
	scores[row][column]= fScore;

	cout << "Update Successfully!";
	cout << endl;
}

// Repeatedly process Grade Book requests until user decides to Quit.
void gradeBookMenu(string students[], string columnTitles[], double scores[][MAX_SCORES],
	int& studentCount, int& scoreCount) {
	string response;
	bool found;
	int row, column;

	while (true) {
		displayMenu();
		cout << endl << "Enter your choice: ";
		getline(cin, response);
		if (response == "D" || response == "d") {
			cout << endl << "Display of all scores:" << endl << endl;
			printScores(students, columnTitles, scores, studentCount, scoreCount);

		}
		else if (response == "A" || response == "a") {
			//call the function to add column of scores
			add_column_of_scores(columnTitles, scores, studentCount, scoreCount);

		}
		else if (response == "N" || response == "n") {
			//call the function to add a new student name and prompt to enter their scores using column-headings
			add_new_student(students, columnTitles, scores, studentCount, scoreCount);

		}
		else if (response == "C" || response == "c") {
			//call the function to change a single socre by prompting sutdnet name and score column-heading
			change_score(students, columnTitles, scores, studentCount, scoreCount);
		}
		else if (response == "E" || response == "e") {
			break;   //exit the menu 
		}
		else {
			cout << "Please select from the following." << endl << endl;
		} // end if
	} // end while

} // end gradeBookMenu


// Displays the grade book program menu choices
void displayMenu() {
	cout << "Grade Book Menu" << endl;
	cout << "(D)isplay all scores" << endl;
	cout << "(A)dd column of scores" << endl;
	cout << "(N)ew student" << endl;
	cout << "(C)hange a score" << endl;
	cout << "(E)xit program" << endl;
} // end displayMenu


// Search the stringList for the target.  If the target is in the stringList, the function returns with found equal to 
// true and locationFound specifying the index where it was found in stringList. If the target is not in the list, then
// the function returns with found equal false.
void listSearch(string target, string stringList[], int stringCount, bool& found, int& locationFound) {
	int Index;


} // end listSearch




/* This function is to determines if the user wants to save the grade book information to a file */
void possiblySaveScoresToFile(string students[], string columnTitles[],
	double scores[][MAX_SCORES], int& studentCount, int& scoreCount) {
	string response, fileName;

	while (true) {
		cout << "Would you like to save the scores to a file (y or n)?: ";
		getline(cin, response);
		if (response == "n" || response == "N") {
			cout << "Scores not saved" << endl;
			break;
		}
		else if (response == "y" || response == "Y") {
			cout << "Enter the file name where you want the scores saved: ";
			getline(cin, fileName);
			saveScoresToFile(fileName, students, columnTitles, scores,
				studentCount, scoreCount);
			break;
		}
		else {
			cout << "Invalid response!  Please enter only f or i. " << endl;
		} // end if
	} // end while
} // end possiblySaveScoresToFile


/* Saves the complete grade book to a text file */
void saveScoresToFile(string fileName, string students[], string columnTitles[],
	double scores[][MAX_SCORES], int& studentCount, int& scoreCount) {
	int row, column;
	ofstream outFile;

	outFile.open(fileName.c_str());
	outFile << studentCount << " " << scoreCount << endl;

	//save score column headings
	for (column = 0; column < scoreCount; column++) {
		outFile << columnTitles[column] << endl;
	}

	//save each student name followed by scores
	for (row = 0; row < studentCount; row++) {
		outFile << students[row] << endl;
		for (column = 0; column < scoreCount; column++) {
			outFile << scores[row][column] << " ";
		} // end for (column...
		outFile << endl;
	} // end for (row...

	outFile.close();
} // end saveScoresToFile


