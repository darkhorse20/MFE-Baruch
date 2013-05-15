#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Filter.h"

using namespace std;

bool filter(string* ln) {
	string line = *ln;
	 unsigned pos1 = line.find(",");
	  unsigned pos2 = line.rfind(",");
      string datestr = line.substr(0,pos1);
	  string  spreadstr = line.substr(pos1+1,pos2 - (pos1+1));
	  string volstr = line.substr(pos2+1, line.length() - pos2);

	  //cout << "Date: " << datestr << " Spread: " << spreadstr << " Volume: " << volstr << "\n";

	  if(filterDuplicate(&line)) return true;
	  if(filterOutlierSpread(&spreadstr)) return true;
	  if(filterOutlierVolume(&volstr)) return true;
	  if(filterZeros(&spreadstr)) return true;

	  return false;
}

bool filterDuplicate(string* line) {
	return false;
}

bool filterZeros(string* val) {
	if(stof(*val) == 0.0f) {
		return true;
	}
	return false;
}

bool filterOutlierSpread(string* val) {
	if(stof(*val) == 0.0f || stof(*val) > 5.0f || stof(*val) < -5.0f) {
		return true;
	}
	return false;
}

bool filterOutlierVolume(string* val) {
	if(stoi(*val) <= 0 || stoi(*val) > 100000000) {
		return true;
	}
	return false;
}
