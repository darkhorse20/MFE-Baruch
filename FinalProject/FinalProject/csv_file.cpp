#include "csv_file.h"

CsvFile::CsvFile()
{
}

CsvFile::CsvFile(vector<string>* headers)
{
	for(vector<string>::iterator headerIter = headers->begin(); headerIter != headers->end(); headerIter++)
		addColumn(*headerIter);
}

CsvFile::~CsvFile()
{
	for(vector<CsvColumnPtr>::iterator colIter = columns.begin(); colIter!=columns.end(); colIter++)
		delete *colIter;
}

int CsvFile::getColumnCount()
{
	return columns.size();
}

CsvColumnPtr CsvFile::getColumn(int columnIndex)
{
	int i = 0;
	for(vector<CsvColumnPtr>::iterator colIter = columns.begin();
		colIter!=columns.end();
		colIter++, i++)
	{
		if(i==columnIndex)
			return *colIter;
	}

	throw "No column found at " + (columnIndex);
}

CsvColumnPtr CsvFile::getColumn(string colHeader) 
{
	vector<string>::iterator headerIter = columnHeaders.begin();
	vector<CsvColumnPtr>::iterator colIter = columns.begin();
	for(;headerIter!=columnHeaders.end(); headerIter++, colIter++)
	{
		if(*headerIter==colHeader)
			return *colIter;
	}

	throw "No column found for header " + colHeader;
}

vector<CsvColumnPtr>* CsvFile::getColumns()
{
	return &columns;
}

string CsvFile::getRowLabelAt(int rowIndex)
{
	vector<string>::iterator dateIter = rowLabels.begin();
	for(int i =0; 
		dateIter!=rowLabels.end(); 
		dateIter++, i++) {
		
		if(i==rowIndex)
			return *dateIter;
	}

	throw "No date found for index " + rowIndex;
}

void CsvFile::addColumn(string header) {
	columnHeaders.push_back(header);
	columns.push_back(new CsvColumn());
}

void CsvFile::addRowLabel(string label) {
	rowLabels.push_back(label);
}

vector<string>* CsvFile::getColumnHeaders() {
	return &columnHeaders;
}

vector<string>* CsvFile::getRowLabels() {
	return &rowLabels;
}

void CsvFile::getDataAsMatrix(MatrixXd& matrix) {

	int numberOfRows = getRowLabels()->size();
	int numberOfCols = getColumnCount();

	matrix = MatrixXd::Zero(numberOfRows, numberOfCols);

	for(int i = 0; i < getColumnCount(); i++)
	{
		CsvColumnPtr col = getColumn(i);
		for(int j = 0; j < col->getValues()->size(); j++)
		{
			matrix(j, i) = col->getValueAt(j);
		}
	}
}

void CsvFile::setMatrixData(MatrixXd& matrix, vector<string>& rowLabels, vector<string>& colLabels)
{
	setMatrixData(matrix, false, false);
	setRowLabels(rowLabels);
	setColLabels(colLabels);
}

void CsvFile::setMatrixData(MatrixXd& matrix, vector<string>& rowLabels, bool autoGenerateColLables)
{
	setMatrixData(matrix, false, false);
	setRowLabels(rowLabels);
	autoGenerateColLabels();
}

void CsvFile::setMatrixData(MatrixXd& matrix, bool autoGenerateRowLables, vector<string>& colLabels)
{
	setMatrixData(matrix, false, false);
	autoGenerateRowLabels();
	setColLabels(colLabels);
}

void CsvFile::setMatrixData(MatrixXd& matrix, bool autoGenerateRowLabels, bool autoGenerateCollLabels)
{
	
	for(int col = 0; col < matrix.cols(); col++)
	{
		CsvColumnPtr cc = new CsvColumn();
		columns.push_back(cc);
		for(int row = 0; row < matrix.rows(); row++)
		{
			cc->addValue(matrix(row, col));
		}
	}
	if(autoGenerateRowLabels)
		this->autoGenerateRowLabels();
	if(autoGenerateCollLabels)
		this->autoGenerateColLabels();
}

void CsvFile::setColLabels(vector<string>& cols) 
{
	vector<string>::iterator col = cols.begin();
	while(col!=cols.end()) {
		columnHeaders.push_back(*col);
		col++;
	}
}

void CsvFile::setRowLabels(vector<string>& rows)
{
	vector<string>::iterator row = rows.begin();
	while(row!=rows.end()) {
		rowLabels.push_back(*row);
		row++;
	}
}
	
void CsvFile::autoGenerateRowLabels()
{
	CsvColumnPtr col = *(columns.begin());
	
	for(int i = 0; i < col->getValues()->size(); i++)
	{
		rowLabels.push_back(itos(i));
	}
}

void CsvFile::autoGenerateColLabels()
{
	for(int i=0; i < columns.size(); i++)
	{
		columnHeaders.push_back(itos(i));
	}
}

string CsvFile::itos(int i)
{
	char c[10000];
	itoa(i, c, 10);
	return string(c);
}
