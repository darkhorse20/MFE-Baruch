#ifndef csv_column_h
#define csv_column_h

#include<vector>
#include<string>

using namespace std;

class CsvColumn;
typedef CsvColumn* CsvColumnPtr;

/*
A wrapper class around the data that would represent a csv column
*/
class CsvColumn 
{
	vector<double> values;
public:
	/*
	default constructor
	*/
	CsvColumn();
	/*
	returns a pointer to the vector that contains the values in the column
	*/
	vector<double>* getValues();
	/*
	returns the value at a particular row
	*/
	double getValueAt(int rowIndex);
	/*
	add a value to end of the list of values
	*/
	void addValue(double value);
};

#endif