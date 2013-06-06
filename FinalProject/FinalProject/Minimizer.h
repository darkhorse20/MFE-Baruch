#pragma once
#include "EKF.h"
#include "Filter.h"

class Minimizer
{
public:
	Minimizer(void);
	~Minimizer(void);
	void Minimizer::estimate_params(Filter *filter, double *prxs, double *estimates, double* u, double *v, int n, double p);
};

