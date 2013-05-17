#include "FilterMain.h"
#include "csv_reader.h"
#include "stdio.h"
#include "FileUtil.h"
#include <vector>
#include "Minimizer.h"

FilterMain::FilterMain(void)
{
}


FilterMain::~FilterMain(void)
{
}

string datafile = "C:\\aya\\Documents\\MFE\\Baruch\\Volatility Filtering and Estimation\\Data\\data.csv";
CsvFilePtr filePtr; 
MatrixXd dataMtx;

int main(int argc, char *argv[] ) {
	//CsvReader csv_r(datafile, filePtr);
	
	//simple_read(datafile, dataMtx, false, false);
	double init_prx = 0.0;
	//double *stock_prices;
	vector<double> stock_prices; 
	//stock_prices = &init_prx;

	ifstream datafile = openForRead ("C:\\aya\\Documents\\MFE\\Baruch\\Volatility Filtering and Estimation\\Data\\datafull.csv");
	//int n_rows = getCol(&datafile, stock_prices, 2, true);
	int n_rows = getColUsingString(&datafile, stock_prices, 2, true);

	cout << "Rows returned from getCol: " << stock_prices.size() << "\n"; 
	//for(int i=0; i<stock_prices.size(); i++) {
	//	stock_prices[i] = log(stock_prices[i]);
	//	cout << "Row: " << i << " log stock price: " << stock_prices[i] << '\n';
	//}
	cout << "Calling the EKF now ... \n";
	
	int n = stock_prices.size();
	double *prxs = &(stock_prices[0]);
	Minimizer min;
	min.estimate_params(prxs,n);

	int c = getchar();
	return 0;
}


