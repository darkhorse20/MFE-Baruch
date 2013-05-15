#include "csv_reader.h"

string CsvReader::itos(int i) 
{
	char a[100];
	itoa(i, a, 10);
	return string(a);
}

/*
 * Splits a string into its components based on the seperator
 */
void CsvReader::splitString(string str, string separator, vector<string>* results) {
    int found;
    found = str.find_first_of(separator);
    while(found != string::npos){
        if(found > 0){
            results->push_back(trimQuotes(str.substr(0,found)));
        }
        str = str.substr(found+1);
        found = str.find_first_of(separator);
    }
    if(str.length() > 0){
        results->push_back(trimQuotes(str));
    }
}

string CsvReader::trimQuotes(string str)
{
	if(str[0]=='"' && str[str.length()-1]=='"' && str.length() > 2)
		return str.substr(1, str.length()-2);
	return str;
}

void CsvReader::buildHeader(string &line)
{
	//build the header, using a buffer to remove the upper left column
	vector<string> buffer;
	splitString(line, ",", &buffer);

	vector<string>::iterator titleIter = buffer.begin();
	titleIter++;//dumping the the upper left column
	for(;titleIter != buffer.end(); titleIter++) {
		csvFilePtr->addColumn(*titleIter);
	}
}

void CsvReader::buildDefaultHeader(string &line)
{
	//build the header, using a buffer to store the split strings
	vector<string> buffer;
	splitString(line, ",", &buffer);
	vector<string>::iterator titleIter = buffer.begin();
	
	int i = 0;
	
	for(;titleIter != buffer.end(); titleIter++) {
		csvFilePtr->addColumn(itos(i++));
	}
}

void CsvReader::addLine(string line)
{
	cout<<"Line read: "<<line<<endl;
	vector<string> prices;
	splitString(line, ",", &prices);
	vector<string>::iterator priceIter = prices.begin();
	for(int i = 0; priceIter!=prices.end(); i++)
	{
		CsvColumnPtr col;
		if(readRowHeaders) {
			if(i==0) {
				csvFilePtr->addRowLabel(trimQuotes(*priceIter)); //read row label
			} else {
				col = csvFilePtr->getColumn(i-1);
			}
		} else {
			if(i==0)
				csvFilePtr->addRowLabel(itos(i));
			col = csvFilePtr->getColumn(i);
		}

		col->addValue(strtod((*priceIter).c_str(), NULL)); //read value
		priceIter++;

	}
}

CsvReader::CsvReader(string fileName, CsvFilePtr fileToBuild, bool readColumnHeaders_, bool readRowHeaders_)
	:csvFilePtr(fileToBuild), readColHeaders(readColumnHeaders_), readRowHeaders(readRowHeaders_)
{
	cout<<"Opening file "<<fileName<<endl;
	ifstream file(fileName);
	
	if(!file) 
	{
		throw "unable to find file " + fileName;
	}

	string line;
	
	file>>line;
	cout<<"First line "<<line<<endl;
	if(readColHeaders) {
		buildHeader(line);
		file>>line;
	} else {
		buildDefaultHeader(line);
	}

	while(!file.eof()) 
	{
		addLine(line);
		file>>line;
	}
	addLine(line);
	file.close();
}

CsvReader::CsvReader(string fileName, CsvFilePtr fileToBuild)
	:csvFilePtr(fileToBuild), readColHeaders(false), readRowHeaders(false)
{
	cout<<"Opening file "<<fileName<<endl;
	ifstream file(fileName);
	
	if(!file) 
	{
		throw "unable to find file " + fileName;
	}

	string line;
	
	file>>line;
	cout<<"First line "<<line<<endl;
	while(!file.eof()) 
	{
		addLine(line);
		file>>line;
		cout<<"Line "<<line<<endl;
	}

	file.close();
}

void simple_read(string filename, MatrixXd &matrix, bool readColHeaders, bool readRowHeaders)
{
	CsvFilePtr matrixFilePtr = new CsvFile();
	CsvReader matrixFileReader(filename, matrixFilePtr, readColHeaders, readRowHeaders);
	matrixFilePtr->getDataAsMatrix(matrix);
	delete matrixFilePtr;
}

void simple_read(string filename, MatrixXd &matrix)
{
	CsvFilePtr matrixFilePtr = new CsvFile();
	CsvReader matrixFileReader(filename, matrixFilePtr);
	matrixFilePtr->getDataAsMatrix(matrix);
	delete matrixFilePtr;
}