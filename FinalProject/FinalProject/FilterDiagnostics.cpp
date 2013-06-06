#include "FilterDiagnostics.h"
#include <math.h>
#include "FileUtil.h"
#include <sstream>
#include "erf.h"
#include "stattests.h"

FilterDiagnostics::FilterDiagnostics(void)
{
}

FilterDiagnostics::~FilterDiagnostics(void)
{
}

void FilterDiagnostics::do_sum_diff(double *observations, double *estimates, int size) {
	sum_diff = 0.0;
	for(int i=0; i < size; i++) {
		sum_diff += (observations[i] - estimates[i]);
	}
}

double FilterDiagnostics::mpe(double *observations, double *estimates, int size) {
	//double sum_diff = 0.0;
	//for(int i=0; i < size; i++) {
	//	sum_diff += (observations[i] - estimates[i]);
	//}
	do_sum_diff(observations, estimates, size);
	mean_prx_err = (sum_diff/size);
	return mean_prx_err;
}

double FilterDiagnostics::rmse(double *observations, double *estimates, int size) {

	double t_rmse = 0.0;
	for(int i=0; i < size; i++) {
		t_rmse += pow(((observations[i] - estimates[i]) - mean_prx_err ),2.0);
	}

	root_mean_sq_err = sqrt(t_rmse/size);

	return root_mean_sq_err;
}

double FilterDiagnostics::box_ljung(double *observations, double *estimates, double *u, double *v, int size) {
	if(sum_diff == 0.0) do_sum_diff(observations, estimates, size);
	mean = sum_diff/size;
	int k =24;
	double numer = 0.0, denom = 0.0;


	for(int k =0; k < size; k++) {
		denom += pow(((observations[k] - estimates[k]) - mean),2.0);
	}

	double r_k = 0.0;
	double sum_chi = 0.0;

	for(int j=0; j <k; j++) {
		for(int i=0; i < size - k; i++) {
			numer += ((observations[i] - estimates[i]) - mean)*((observations[i+k] - estimates[i+k]) - mean);
		}


		r_k = numer/denom;
		sum_chi += r_k*r_k/(size-k);
	}
	sum_chi = sum_chi*(size)*(size +2);

	return sum_chi;
}

void FilterDiagnostics::histogram(double *observations, double *estimates, int size) {
	string res = "";
	ostringstream stream;
	ofstream file = openForWrite("../../Data/ResultAnalysis.csv");
	for(int i=0; i < size; i++) {
		stream << (observations[i] - estimates[i]) << "\n";
		//cout << "Writing to histogram: " << (observations[i] - estimates[i]) << "\n";

	}
	write(&file, &stream.str());
	closeFile(&file);
}

void FilterDiagnostics::std_residuals(double *observations, double *estimates, double *u, double* v, int size) {
	string res = "";
	ostringstream stream;
	ofstream file = openForWrite("../../Data/StdResiduals.csv");
	for(int i=0; i < size; i++) {
		stream << ((observations[i] - estimates[i])-u[i])/sqrt(v[i]) << "\n";
		//cout << "Writing to histogram: " << (observations[i] - estimates[i]) << "\n";

	}
	write(&file, &stream.str());
	closeFile(&file);
}

double FilterDiagnostics::chi_squared_test(double *observations, double *estimates,  double *u, double *v, int size) {

	double *std_residuals = new double[size];

	const int inter = 25;
	double min=-10.00, max=8.00;
	double delt = (max - min)/inter;

	double *bin_pts = new double[inter+1];
	double test_bin_cnts[inter] = {};
	double norm_bin_cnts[inter] = {};

	for(int i=0; i <= inter; i++) {
		bin_pts[i] = min + i*delt;
		//cout << "Bin Pt: " << bin_pts[i] << "\n";
	}

	//Initialize the bin counts arrays
	//for(int i=0; i < size; i++) {
	//	test_bin_cnts[i] = 0.0;
	//	norm_bin_cnts[i] = 0.0;
	//	//cout << "Init Bin Counts: " << test_bin_cnts[i] << "\n";
	//}

	unsigned int bin_idx = 0;
	double doub_idx;

	for(int i=0; i < size; i++) {
		std_residuals[i] = ((observations[i]-estimates[i]) - u[i])/sqrt(v[i]);
		cout << "Residual: " << std_residuals[i] << "\n";
		doub_idx = (std_residuals[i] - min)/delt;
		bin_idx = static_cast<unsigned int>(doub_idx);
		cout << "Bin Indx Calc: " << bin_idx << "\n";
		test_bin_cnts[bin_idx] += 1; 
	}

	cout << "Setting up Normal Dist bin counts\n";

	double N_i = 0.0;
	double chi_sq_value = 0.0;
	Normaldist norm(0,1);
	double less_df = 0.0;

	for(int i=0; i <= inter; i++) {
		//printf("Normal cdf for : %12.6f is : %12.6f\n", bin_pts[i+1], norm.cdf(bin_pts[i+1]));

		N_i = (norm.cdf(bin_pts[i+1]) - norm.cdf(bin_pts[i]))*size;
		cout << "Exp Normal Cnts is: " << N_i << "\n";
		if(test_bin_cnts[i] <= 0) {
			less_df++;
			//cout << "test bin cnt is zero\n";
		} else {
			chi_sq_value += (N_i - test_bin_cnts[i])*(N_i - test_bin_cnts[i])/test_bin_cnts[i];
			//cout << "Chi Sq. Value is : " << chi_sq_value << "\n";
		}
		norm_bin_cnts[i] = N_i;
	}

	//string obs_cnt_file = "C:/aya/Documents/MFE/Baruch/Volatility Filtering and Estimation/Data/ObsBinCntsFile.csv";
	//string norm_cnt_file = "C:/aya/Documents/MFE/Baruch/Volatility Filtering and Estimation/Data/NormBinCntsFile.csv";
	//
	//saveToFile(obs_cnt_file, &test_bin_cnts[0], "",inter);
	//saveToFile(norm_cnt_file, &norm_bin_cnts[0], "",inter);

	//VecDoub_I bins(inter, &(test_bin_cnts[0]));
	//VecDoub_I ebins(inter, &(norm_bin_cnts[0]));

	//cout << "Calculated Chi Squared Value: " << chi_sq_value << "\n";
	//Doub df = inter -1 - less_df;
	Doub p_value = 0.0f;
	//chsone(bins, ebins, df,
	//chi_sq_value, p_value, 1);

	return p_value;
}

