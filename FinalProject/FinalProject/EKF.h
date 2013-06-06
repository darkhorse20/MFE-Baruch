#include <math.h>
#include "Filter.h"

class EKF: public Filter
{

public:
	EKF(void) {};
	~EKF(void) {};

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

	void estimate_extended_kalman_parameters_1_dim_fin_proj(
		double *log_stock_prices,
		double muS,
		int n_stock_prices,
		double p,
		double kappa,
		double bigV,
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
		double omega;
		double theta;
		double pow_xi;
		omega = kappa*bigV; //K(V - v_k) = w - theta*v_k = theta(w/theta - v_k)
		theta = kappa;

		x = 0.04;
		P=0.01;
		u[0]=u[n_stock_prices-1]=0.0;
		v[0]=v[n_stock_prices-1]=1.0;
		estimates[0]=estimates[1]=log_stock_prices[0]+eps;

		if(omega < 0.0) omega = 0.2;
		if(theta < 0.0) theta = 8.0;
		if(rho > 1.0 || rho < -1.0 ) rho = 0.5;
		if(xi < 0.0) xi = 0.04;

		for (i1=1;i1<n_stock_prices-1;i1++)
		{
			if (x<0) x=0.00001;

			pow_xi = pow(x,p-0.5);

			x1 = x + ( omega - (theta-0.5*rho*xi*pow_xi) * x) * delt +
				rho*xi* pow_xi * (log_stock_prices[i1]-log_stock_prices[i1-1]); 
			// xi* sqrt( 1 - pow(rho,2.0)) * pow(x,p)* sqrt(delt) * ;

			A = 1.0- (theta-0.5*rho*xi*(p+0.5)*pow(x, (p-0.5) ))*delt 
				+ (p - 0.5)*rho*xi*pow(x, p - 1.5)*(log_stock_prices[i1]-log_stock_prices[i1-1]) ;

			W = xi*sqrt((1-rho*rho) * delt) * pow(x,p);

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


	void applyFilter(double *log_stock_prices,
		double muS,
		int n_stock_prices,
		double p,
		double kappa,
		double bigV,
		double xi,
		double rho,
		double *u,
		double *v,
		double *estimates) { 

			estimate_extended_kalman_parameters_1_dim_fin_proj(
				log_stock_prices,
				muS,
				n_stock_prices,
				p,
				kappa,
				bigV,
				xi,
				rho,
				u,
				v,
				estimates);
	}

};
