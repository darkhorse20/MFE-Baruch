#pragma once
class Filter
{
public:
	virtual void applyFilter(double *log_stock_prices,
	double muS,
	int n_stock_prices,
	double p,
	double kappa,
	double bigV,
	double xi,
	double rho,
	double *u,
	double *v,
	double *estimates);
};

