// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "FileUtil.h"
#include <sstream>
#include <vector>


using namespace std;

ifstream openForRead(string file) {
	ifstream myfile (file);
	return myfile;
}

ofstream openForWrite(string file) {
	ofstream myfile (file);
	return myfile;
}

bool readLine(ifstream* file, string* line) {
	if ((*file).is_open())
	{
		if ( (*file).good() && !(*file).eof())
		{
			getline ((*file),(*line));
			return true;

		} else {
			return false;
		}
	} else {
		return false;
	}

}

void closeFile(ifstream* file) {
	(*file).close();
}

void closeFile(ofstream* file) {
	(*file).flush();
	(*file).close();
}

void write(ofstream* file, string* line) {
	char* buffer = new char[(*line).size() + 2];
	buffer[(*line).size()  ] = '\n';
	buffer[(*line).size() +1 ] = 0;
	memcpy(buffer, (*line).c_str(), (*line).size());
	(*file).write(buffer,(*line).size() + 2);
	(*file).flush();

	delete[] buffer;
}
void clearPtrMemory() {
}

int getColUsingString(ifstream* file, vector<double> &d_vals, int d_col, bool skip_hdr) {
	char c;
	int col = 1, len = 0;
	int rows =0;
	char colVal[30];
	char *data;
	string line;
	string word;

	if ((*file).is_open())
	{
		if ( (*file).good() && !(*file).eof())
		{
			getline ((*file),line);

			if(skip_hdr) {
				//skip first line
				getline ((*file),line);

			}

			while(!(*file).eof()) {
				
				stringstream stream(line);
				
				while( getline(stream, word, ',') ) {
					if(col == d_col) {
						//cout << "Adding: " << word << ", to row: " << rows <<"\n";
						d_vals.push_back(atof(word.c_str()));
					}
					col++;
				}

				getline ((*file),line);
				rows++;
				col = 1;
			}
		} 
		return rows;
	}
}

int getCol(ifstream* file, double* d_vals, int d_col, bool skip_hdr) {
	char c;
	int col = 1, len = 0;
	int rows =0;
	char colVal[30];
	char *data;

	if ((*file).is_open())
	{
		if ( (*file).good() && !(*file).eof())
		{
			data = 	&(colVal[0]);
			c = (*file).get();

			if(skip_hdr) {
				while(c !=EOF && c != '\n') {
					c = (*file).get();
					//cout << "Skipping char in header: " << c << "\n"; 
				}
			}

			c = (*file).get();
			while(c != EOF) {
				if(c == '\n') {

					if(col == d_col) {
						data[len]='\0';
						d_vals[rows]  = atof(colVal);
						//cout << "Line ending, adding: " << colVal << "\n";
						len = 0;
						data = 	&(colVal[0]);
					}

					col = 1;
					rows++;

				} else if(c == ',') {
					if(col == d_col) {
						data[len]='\0';
						d_vals[rows]  = atof(colVal);
						cout << "Adding: " << colVal << "Row: " << rows << "\n";
						len = 0;
						
						//delete[] data;
						data = NULL;

						data = 	&(colVal[0]);

					}
					col++;

				} else {
					if(col == d_col) { //This is the col we want to extract
						data[len] = c;
						//cout << "Adding char : " << c << "to col data" << "\n";
						len++;
					}
				}
				c = (*file).get();

			}

			if(c == EOF) {
				rows++;
			}

		} 
		return rows;
	} 

}