#include <math.h>
#include "Filter.h"
#include "erf.h"
#include "ran2.h"

class UKF: public Filter
{

private:

public:
	UKF(void) {};
	~UKF(void) {};

	void estimate_unscented_kalman_parameters_1_dim(
		double *log_stock_prices,
		double muS,
		int n_stock_prices,
		double omega,
		double theta, double xi,
		double rho,
		double *u,
		double *v,
		double *estimates)
	{
		int i1,i2, i3, t1;
		int ret;
		int na=3;
		double x, xa[3];
		double X[7], Xa[3][7];
		double Wm[7], Wc[7], Z[7];
		double x1;
		double prod, prod1;
		double P, P1;
		double **Pa, **proda;
		double z, U, Pzz, K;
		double delt=1.0/252.0;
		double a=0.001 , b=0.0, k=0.0, lambda;
		double eps=0.00001;
		lambda = a*a*(na +k)-na;
		proda= new double * [na];
		Pa = new double * [na];
		for (i1=0;i1<na;i1++)
		{
			Pa[i1]= new double [na];
			proda[i1]= new double [na];
		}
		xa[1]=xa[2]=0.0;
		x= 0.04;
		u[0]=u[n_stock_prices-1]=0.0;
		v[0]=v[n_stock_prices-1]=1.0;
		estimates[0]=estimates[1]=log_stock_prices[0]+eps;
		xa[0]=x;
		Pa[0][0]= Pa[1][1]= Pa[2][2] = 1.0;
		Pa[1][0]= Pa[0][1]= Pa[1][2]=Pa[2][1]= Pa[0][2]=Pa[2][0]=0;
		for (i1=0;i1<na;i1++)
		{
			for (i2=0;i2<na;i2++)
			{
				proda[i1][i2]=0.0;
			}
		}
		Wm[0]=lambda/(na+lambda);
			Wc[0]=lambda/(na+lambda) + (1-a*a+b);
		for (i3=1;i3<(2*na+1);i3++)
		{
			Wm[i3]=Wc[i3]=1/(2*(na+lambda));
		}
		for (t1=1;t1<n_stock_prices-1;t1++)
		{
			for (i1=0;i1<na;i1++)
			{
				Xa[i1][0]= xa[i1];
			}
			for (i1=0;i1<na;i1++)
			{
				for (i2=0;i2<na;i2++)
				{
					if (i1==i2)
					{
						if (Pa[i1][i2] < 1.0e-10)
							Pa[i1][i2]= 1.0e-10;
					}
					else
					{
						if (Pa[i1][i2] < 1.0e-10)
							Pa[i1][i2]= 0.0;
					}
				}
			}
			ret = sqrt_matrix(Pa,proda,na);
			for (i3=1;i3<(1+na);i3++)
			{
				for (i1=0;i1<na;i1++)
				{
					Xa[i1][i3]= xa[i1] + sqrt(na+lambda) * proda[i1][i3-1];
				}
			}
			for (i3=(1+na);i3<(2*na+1);i3++)
			{
				for (i1=0;i1<na;i1++)
				{
					Xa[i1][i3]= xa[i1] - sqrt(na+lambda) * proda[i1][i3-na-1];
				}
			}
			for (i3=0;i3<(2*na+1);i3++)
			{
				if (Xa[0][i3]<0) Xa[0][i3]=0.0001;
				X[i3]= Xa[0][i3] + (omega-muS*rho*xi -
					(theta-0.5*rho*xi) *Xa[0][i3])*delt +
					rho*xi* (log_stock_prices[t1]-log_stock_prices[t1-1]) +
					xi*sqrt((1-rho*rho)*delt*Xa[0][i3])*Xa[1][i3];
			}
			x1 = 0;
			for (i3=0;i3<(2*na+1);i3++)
			{
				x1 += Wm[i3]*X[i3];
			}
			P1=0.0;
			for (i3=0;i3<(2*na+1);i3++)
			{
				P1 += Wc[i3]*(X[i3]-x1)*(X[i3]-x1);
			}
			z=0;
			for (i3=0;i3<(2*na+1);i3++)
			{
				if (X[i3]<0) X[i3]=0.00001;
				Z[i3] = log_stock_prices[t1] + (muS-0.5*X[i3])*delt +
					sqrt(X[i3]*delt)*Xa[2][i3];
				z += Wm[i3]*Z[i3];
			}
			Pzz=0;
			for (i3=0;i3<(2*na+1);i3++)
			{
				Pzz += Wc[i3]*(Z[i3]-z)*(Z[i3]-z);
			}
			prod=0.0;
			for (i3=0;i3<(2*na+1);i3++)
			{
				prod += Wc[i3]*(X[i3]-x1)* (Z[i3]-z);
			}
			94 INSIDE VOLATILITY ARBITRAGE
				K= prod/Pzz;
			u[t1] = log_stock_prices[t1+1] - z;
			v[t1] = Pzz;
			estimates[t1+1] = z;
			x = x1 + K*(log_stock_prices[t1+1] - z);
			P = P1 - K*K * Pzz;
			xa[0]=x;
			Pa[0][0] = P;
			if (x<0) x=0.0001;
			Pa[1][0]= Pa[0][1]= Pa[1][2]=Pa[2][1]= Pa[0][2]=Pa[2][0]=0;
		}
		for (i1=0;i1<na;i1++)
		{
			delete [] Pa[i1];
			delete [] proda[i1];
		}
		delete [] Pa;
		delete [] proda;
	}
	// the routine sqrt_matrix() can be constructed via the Cholesly
	//decomposition
	// also available as choldc() in the Numerical Recipes in C


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

			estimate_unscented_kalman_parameters_1_dim(
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
