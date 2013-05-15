#include "nr3.h"
#include "mins_ndim.h"

#pragma once

class EKFFilter
{

private:
	double *log_stock_prices;
	int no_stock_prices;

public:
	EKFFilter(void);
	EKFFilter(double *stock_prices, int n_stock_prices);
	~EKFFilter(void);
	void estimate_extended_kalman_parameters_1_dim(
		double *log_stock_prices,
		double muS,
		int n_stock_prices,
		double omega,
		double theta,
		double xi,
		double rho,
		double *u,
		double *v,
		double *estimates);
	
Doub unc(VecDoub_I x);
void estimate_params();

};

