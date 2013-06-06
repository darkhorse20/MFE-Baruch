#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

ifstream openForRead(string file);

ofstream openForWrite(string file);

void write(ofstream* file, string *line);

bool readLine(ifstream* file, string* line);

void closeFile(ifstream* file);

void closeFile(ofstream* file);

int getCol(ifstream* file, double* d_vals, int d_col, bool skip_hdr);

int getColUsingString(ifstream* file, vector<double> &d_vals, int d_col, bool skip_hdr);

void saveToFile(string file, double* data_array, string separator, int size);