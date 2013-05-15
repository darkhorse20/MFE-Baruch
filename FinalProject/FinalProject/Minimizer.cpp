#include "Minimizer.h"
#include "nr3.h"
#include "mins_ndim.h"


Minimizer::Minimizer(void)
{
}


Minimizer::~Minimizer(void)
{
}

struct Func {
Doub operator()(VecDoub_I &x);
};

struct Funcd { 
	Doub operator() (VecDoub_I &x)
	{
		return x[0]*x[0]+x[1]*x[1];
	}

	void df(VecDoub_I &x, VecDoub_O &deriv)
	{
		deriv[0]=2.0*x[0];
		deriv[1]=2.0*x[1];
	}

	void do_minimization() {


		Funcd funcd;
		VecDoub p;

		Func func;
		Powell<Func> powell(func);
		p=powell.minimize(p);

		Frprmn<Funcd> frprmn(funcd);
		p=frprmn.minimize(p); 
	}
};

