#include "FilterMain.h"
#include "stdio.h"
#include "FileUtil.h"
#include <vector>
#include "Minimizer.h"
#include "FilterDiagnostics.h"
#include "Filter.cpp"

FilterMain::FilterMain(void)
{
}


FilterMain::~FilterMain(void)
{
}

string datafile = "C:\\aya\\Documents\\MFE\\Baruch\\Volatility Filtering and Estimation\\Data\\data.csv";
//CsvFilePtr filePtr; 
//MatrixXd dataMtx;

int main(int argc, char *argv[] ) {

	double init_prx = 0.0;
	vector<double> stock_prices; 

	ifstream datafile = openForRead ("../../Data/Final-Project.csv");
	//int n_rows = getCol(&datafile, stock_prices, 1, true);
	int n_rows = getColUsingString(&datafile, stock_prices, 1, false);

	cout << "Rows read from input file: " << stock_prices.size() << "\n"; 
	for(int i=0; i<stock_prices.size(); i++) {
		stock_prices[i] = log(stock_prices[i]);
		//cout << "Row: " << i << " log stock price: " << stock_prices[i] << '\n';
	}
	
	
	int n = stock_prices.size();
	
	double *ln_prxs = &(stock_prices[0]);
	double *estims = new double[n];
	double *u = new double[n];
	double *v = new double[n];

	Minimizer min;
	
	EKF ekf;
	Filter *selectedFilter = &ekf;
	FilterDiagnostics diagnostics;


	///////////
	double p = 0.5;
	cout << "Filter with p = 0.5 (Square Root Model) \n"; 
	min.estimate_params(selectedFilter,ln_prxs, estims, u, v, n, p);
	
	double mpe = diagnostics.mpe(ln_prxs,estims, n);
	double rmse = diagnostics.rmse(ln_prxs,estims, n);

	cout << "MPE: " << mpe << ",    RMSE: " << rmse << "\n"; 

	diagnostics.histogram(ln_prxs,estims, n);

	//Specify test files
	string obs_file = "../../Data/ObsFileSqrt.csv";
	string est_file = "../../Data/EstFileSqrt.csv";
	string v_file = "../../Data/VarianceFileSQrt.csv";
	string u_file = "../../Data/MeanFileSQrt.csv";

	saveToFile(obs_file, ln_prxs, "", n);
	saveToFile(est_file, estims, "", n);
	saveToFile(v_file, v, "", n);
	saveToFile(u_file, u, "", n);

	//Save standard residuals to file for furhter diagnosis (done in R).
	diagnostics.std_residuals(ln_prxs,estims,u, v, n);

	double box_ljung_val = diagnostics.box_ljung(ln_prxs,estims,u,v, n);
	cout << "Box-Ljung Test stat: " << box_ljung_val << "\n\n\n";


	/////////////
	p = 1.0;
	cout << "Filter with p = 1.0 (GARCH Model) \n"; 
	min.estimate_params(selectedFilter,ln_prxs, estims, u, v, n, p);
	
	//FilterDiagnostics diagnostics;
	mpe = diagnostics.mpe(ln_prxs,estims, n);
	rmse = diagnostics.rmse(ln_prxs,estims, n);

	cout << "MPE: " << mpe << ",    RMSE: " << rmse << "\n"; 

	diagnostics.histogram(ln_prxs,estims, n);

	//Specify test files
	obs_file = "../../Data/ObsFileGARCH.csv";
	est_file = "../../Data/EstFileGARCH.csv";
	v_file = "../../Data/VarianceFileGARCH.csv";
	u_file = "../../Data/MeanFileGARCH.csv";

	saveToFile(obs_file, ln_prxs, "", n);
	saveToFile(est_file, estims, "", n);
	saveToFile(v_file, v, "", n);
	saveToFile(u_file, u, "", n);

	//Save standard residuals to file for furhter diagnosis (done in R).
	//string resfile = "C:/aya/Documents/MFE/Baruch/Volatility Filtering and Estimation/Data/StdResiduals.csv";
	diagnostics.std_residuals(ln_prxs,estims,u, v, n);

	box_ljung_val = diagnostics.box_ljung(ln_prxs,estims,u,v, n);
	cout << "Box-Ljung Test stat: " << box_ljung_val << "\n\n\n";

	////////////
	p = 1.5;
	cout << "Filter with p = 1.5 (3-2 Model) \n"; 
	min.estimate_params(selectedFilter,ln_prxs, estims, u, v, n, p);
	
	//FilterDiagnostics diagnostics;
	mpe = diagnostics.mpe(ln_prxs,estims, n);
	rmse = diagnostics.rmse(ln_prxs,estims, n);

	cout << "MPE: " << mpe << ",    RMSE: " << rmse << "\n"; 

	diagnostics.histogram(ln_prxs,estims, n);

	//Specify test files
	obs_file = "../../Data/ObsFile32.csv";
	est_file = "../../Data/EstFile32.csv";
	v_file = "../../Data/VarianceFile32.csv";
	u_file = "../../Data/MeanFile32.csv";

	saveToFile(obs_file, ln_prxs, "", n);
	saveToFile(est_file, estims, "", n);
	saveToFile(v_file, v, "", n);
	saveToFile(u_file, u, "", n);

	//Save standard residuals to file for furhter diagnosis (done in R).
	//string resfile = "C:/aya/Documents/MFE/Baruch/Volatility Filtering and Estimation/Data/StdResiduals.csv";
	diagnostics.std_residuals(ln_prxs,estims,u, v, n);

	box_ljung_val = diagnostics.box_ljung(ln_prxs,estims,u,v, n);
	cout << "Box-Ljung Test stat: " << box_ljung_val << "\n";


	int c = getchar();
	return 0;
}


