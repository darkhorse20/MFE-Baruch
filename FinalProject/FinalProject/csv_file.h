#ifndef csv_file_h
#define csv_file_h

#include <vector>
#include <cstdlib>
#include <Eigen/Core>

#include "csv_column.h"

USING_PART_OF_NAMESPACE_EIGEN

using namespace std;

class CsvFile;
typedef CsvFile* CsvFilePtr;

/**
	An object representation of a Csv Timeseries file
*/
class CsvFile
{
	//Private variables
	vector<string> columnHeaders;
	vector<string> rowLabels;
	vector<CsvColumnPtr> columns;

	void setColLabels(vector<string>& cols);
	void setRowLabels(vector<string>& rows);
	void autoGenerateRowLabels();
	void autoGenerateColLabels();
	string itos(int i);

public:
	/*
	Default Constructor
	*/
	CsvFile();
	
	/*
	Construtor used when the file is to be built around a particular column structure
	*/
	CsvFile(vector<string>* headers);
	
	/*
	destroys the data in the columns variable
	*/
	virtual ~CsvFile();
	/*
	returns the column count
	*/
	int getColumnCount();
	/*
	gets a column at a particular index
	*/
	CsvColumnPtr getColumn(int columnIndex);
	/*
	gets a column that corresponds to a particular header
	*/
	CsvColumnPtr getColumn(string colHeader);
	/*
	gets a pointer to the vector that contains all the columns
	*/
	vector<CsvColumnPtr>* getColumns();
	/*
	gets the date at a particular index
	*/
	string getRowLabelAt(int rowIndex);
	/*
	add a column with a header
	*/
	void addColumn(string header);
	/*
	adds a date
	*/
	void addRowLabel(string rowLabel);
	/*
	returns a pointer to a vector of column headers
	*/
	vector<string>* getColumnHeaders();
	/*
	returns a pointer to a vector of Dates
	*/
	vector<string>* getRowLabels();
	/*
	Gets the data as a matrix
	*/
	void getDataAsMatrix(MatrixXd& matrix);
	/*
	Create a file from Matrix Data using the given rowLabels and colLabels
	*/
	void CsvFile::setMatrixData(MatrixXd& matrix, vector<string>& rowLabels, vector<string>& colLabels);
	/*
	Create a file from Matrix Data using the given rowLabels and auto generating the column labels
	*/
	void CsvFile::setMatrixData(MatrixXd& matrix, vector<string>& rowLabels, bool autoGenerateColLables);
	/*
	Create a file from Matrix Data using the given colLabels and auto generating the row labels
	*/
	void CsvFile::setMatrixData(MatrixXd& matrix, bool autoGenerateRowlLables, vector<string>& colLabels);
	/*
	Create a file from Matrix Data, auto generating the row and column labels
	*/
	void CsvFile::setMatrixData(MatrixXd& matrix, bool autoGenerateRowlLables, bool autoGenerateCollLables);
};

#endif