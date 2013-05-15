#include "csv_column.h"

CsvColumn::CsvColumn() 
{
}

double CsvColumn::getValueAt(int rowIndex)
{
	vector<double>::iterator it = values.begin();
	it+=rowIndex;
	return *it;
}

void CsvColumn::addValue(double value)
{
	values.push_back(value);
}

vector<double>* CsvColumn::getValues()
{
	return &values;
}
