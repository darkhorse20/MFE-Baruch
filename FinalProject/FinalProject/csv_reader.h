#ifndef csv_reader_h
#define csv_reader_h

#include "csv_file.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include <map>
#include <Eigen\Core>

using namespace std;
using namespace Eigen;

void simple_read(string filename, MatrixXd &matrix, bool readColHeaders, bool readRowHeaders);
void simple_read(string filename, MatrixXd &matrix);

/*
A class that reads a contents of a file into the CsvPointer
*/
class CsvReader 
{
	//the class being built
	const CsvFilePtr csvFilePtr;
	const bool readColHeaders;
	const bool readRowHeaders;
		
	//private metholds
	/*
	builds a header when fed the first line in the file
	*/
	void buildHeader(string &line);
	/*
	builds a default header with column titles 1,2,3...
	*/
	void CsvReader::buildDefaultHeader(string &line);
	/*
	Adds a line to the CsvFile
	*/
	void addLine(string line);
	/*
	Splits a string when given the string and the separator.
	The results are stored in the passed in results pointer
	*/
	void splitString(string str, string separator, vector<string>* results);
	/*
	removes the quotes that might surround an element in the csv file
	*/
	string trimQuotes(string str);
	/*
	Converts an integer into a string
	*/
	string itos(int i);
public:
	/*
	Constructor builds a csv File.
	the csvFile is passed back to the user by way of the 'fileToBuild' parameter
	*/
	CsvReader(string fileName, CsvFilePtr fileToBuild, bool readColHeaders, bool readRowHeaders);

	CsvReader(string fileName, CsvFilePtr fileToBuild);
};

#endif