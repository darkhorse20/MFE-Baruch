#include "Minimizer.h"
#include "nr3.h"
#include "mins_ndim.h"

struct Func {
	
	Func(double *ln_stock_prxs, int nr_prxs): prxs(ln_stock_prxs), n(nr_prxs) {	}
	Doub operator()(VecDoub_I & x) {
		
		double *u = new double[n];
		double *v = new double[n];
		double *estimates = new double[n];

		estimate_extended_kalman_parameters_1_dim(prxs, 0.025, n,
			x[0],x[1],x[2],x[3], u, v, estimates);
		
		Doub neg_lli_hood = 0.0;
		for(int i=0; i < n; i++) {
			neg_lli_hood += log(v[i])+u[i]*u[i]/v[i];
		}

		cout << "Log likelihood is: " << neg_lli_hood << "\n";
		return neg_lli_hood;

		//return (x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);

	}

	double *prxs;
	int n;

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
	double *estimates)
{
	int i1;
	double x, x1, W, H, A;
	double P, P1, z, U, K;
	double delt=1.0/252.0;
	double eps=0.00001;
	x = 0.04;
	P=0.01;
	u[0]=u[n_stock_prices-1]=0.0;
	v[0]=v[n_stock_prices-1]=1.0;
	estimates[0]=estimates[1]=log_stock_prices[0]+eps;
	for (i1=1;i1<n_stock_prices-1;i1++)
	{
		if (x<0) x=0.00001;
		x1 = x + ( omega-rho*xi*muS - (theta-0.5*rho*xi) * x) * delt +
			rho*xi* (log_stock_prices[i1]-log_stock_prices[i1-1]);
		A = 1.0-(theta-0.5*rho*xi)*delt;
		W = xi*sqrt((1-rho*rho) * x * delt);
		P1 = W*W + A*P*A;
		if (x1<0) x1=0.00001;
		H = -0.5*delt;
		U = sqrt(x1*delt);
		K = P1*H/( H*P1*H + U*U);
		z = log_stock_prices[i1+1];
		x = x1 + K * (z - (log_stock_prices[i1] + (muS-0.5*x1)*delt));
		u[i1] = z - (log_stock_prices[i1] + (muS-0.5*x1)*delt);
		v[i1] = H*P1*H + U*U;
		estimates[i1+1] = log_stock_prices[i1] + (muS-0.5*x1)*delt;
		P=(1.0-K*H)*P1;
	}
}
};

Minimizer::Minimizer(void)
{
}


Minimizer::~Minimizer(void)
{
}

void Minimizer::estimate_params(double *prxs, int n) {

	cout << " estimating params\n";
	Func filter(prxs,n);
	Int nr_of_params = 4;

	Int i;
    
	VecDoub pinit(nr_of_params , 0.0); 
	pinit[0]= 0.15;
	pinit[1]= 10.0;
	pinit[2]= 0.02;
	pinit[3]=-0.51;

	VecDoub pmin;
	Doub tol = 0.001;
	cout << "calling powell minimization";

	Powell <Func> powell(filter, tol);
	pmin = powell.minimize(pinit);

	cout << "Number of Powell iterations  =  " << powell.iter << endl << endl;
    cout << "After Powell, minimum value  = ";
    cout << setw(13) << powell.fret << " at: ";
    for (i = 0; i < nr_of_params; i++) {
        cout << setw(13) << pmin[i] << " ";
    }
    cout << endl << endl;

};

