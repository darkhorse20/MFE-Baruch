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
	

	void estimate_particle_unscented_kalman_parameters_1_dim(
		double *log_stock_prices,
		double muS,
		int n_stock_prices,
		double omega,
		double theta,
		double xi,
		double rho,
		double *ll,
		double *estimates)
	{
		Normaldist norm(0,1);
		
		int i1, i2, i3, i4;
		int na=3;
		double x0, P0;
		double Wm[7], Wc[7];
		int M=1000;
		double x[1000], xx[1000], x1[1000], x2[1000],
			zz[1000], Z[1000][7];
		double X[1000][7], Xa[1000][3][7];
		double xa[1000][3], prod[1000];
		double P[1000], P1[1000], U[1000], K[1000],
			W[1000], Pzz[1000];
		double w[1000], u[1000], c[1000];
		double ***Pa, ***proda;
		double q, pz, px, s, m, l, z;
		double delt=1.0/252.0;
		long idum=-1;
		Ran ran2(idum);
		int ret;
		double a=0.001 , b=0.0, k=0.0, lambda;
		proda= new double ** [M];
		Pa = new double ** [M];
		for (i2=0;i2<M;i2++)
		{
			Pa[i2]= new double * [na];
			proda[i2]= new double * [na];
			for (i1=0;i1<na;i1++)
			{
				Pa[i2][i1]= new double [na];
				proda[i2][i1]= new double [na];
			}
		}
		for (i2=0;i2<M;i2++)
		{
			for (i1=0;i1<na;i1++)
			{
				for (i3=0;i3<na;i3++)
				{
					proda[i2][i1][i3]=0.0;
				}
			}
		}
		lambda = a*a*(na +k)-na;
		Wm[0]=lambda/(na+lambda);
		Wc[0]=lambda/(na+lambda) + (1-a*a+b);
		for (i3=1;i3<(2*na+1);i3++)
		{
			Wm[i3]=Wc[i3]=1/(2*(na+lambda));
		}
		x0 = 0.04;
		P0 = 0.000001;
		for (i2=0; i2<M; i2++)
		{
			x[i2] = x0 + sqrt(P0)* norm.invcdf(ran2.doub());
			P[i2] = P0;
			xa[i2][0]=x[i2];
			xa[i2][1]=xa[i2][2]=0.0;
			Pa[i2][0][0]= P[i2];
			Pa[i2][1][1]= Pa[i2][2][2] = 1.0;
			Pa[i2][1][0]= Pa[i2][0][1]= Pa[i2][1][2] =
				Pa[i2][2][1] =
				Pa[i2][0][2] = Pa[i2][2][0] = 0.0;
		}
		*ll=0.0;
		for (i1=1;i1<n_stock_prices-1;i1++)
		{
			l = 0.0;
			estimates[i1+1]=0.0;
			for (i2=0; i2<M; i2++)
			{
				/* UKF for the proposal distribution */
				for (i3=0;i3<na;i3++)
				{
					Xa[i2][i3][0]= xa[i2][i3];
				}
					for (i3=0;i3<na;i3++)
					{
						for (i4=0;i4<na;i4++)
						{
							if (i3==i4)
							{
								if (Pa[i2][i3][i4] < 1.0e-10)
									Pa[i2][i3][i4]= 1.0e-10;
							}
							else
							{
								if (Pa[i2][i3][i4] < 1.0e-10)
									Pa[i2][i3][i4] = 0.0;
							}
						}
					}
					ret = sqrt_matrix(Pa[i2],proda[i2],na);
					for (i3=1;i3<(1+na);i3++)
					{
						for (i4=0;i4<na;i4++)
						{
							Xa[i2][i4][i3]= xa[i2][i4] + sqrt(na+lambda) *
								proda[i2][i4][i3-1];
						}
					}
					for (i3=(1+na);i3<(2*na+1);i3++)
					{
						for (i4=0;i4<na;i4++)
						{
							Xa[i2][i4][i3]= xa[i2][i4] - sqrt(na+lambda) *
								proda[i2][i4][i3-na-1];
						}
					}
					for (i3=0;i3<(2*na+1);i3++)
					{
						if (Xa[i2][0][i3]<0) Xa[i2][0][i3]=0.0001;
						X[i2][i3]= Xa[i2][0][i3] + (omega-muS*rho*xi -
							(theta-0.5*rho*xi) *Xa[i2][0][i3])*delt +
							rho*xi* (log_stock_prices[i1]-
							log_stock_prices[i1-1]) +
							xi*sqrt((1-rho*rho)*delt*Xa[i2][0][i3])*
							Xa[i2][1][i3];
					}
					x1[i2] = 0;
					for (i3=0;i3<(2*na+1);i3++)
					{
						x1[i2] += Wm[i3]*X[i2][i3];
					}
					P1[i2]=0.0;
					for (i3=0;i3<(2*na+1);i3++)
					{
						P1[i2] += Wc[i3]*(X[i2][i3]-x1[i2])*(X[i2][i3]-
							x1[i2]);
					}
					zz[i2]=0;
					for (i3=0;i3<(2*na+1);i3++)
					{
						if (X[i2][i3]<0) X[i2][i3]=0.00001;
						Z[i2][i3] = log_stock_prices[i1] +
							(muS-0.5*X[i2][i3])*delt + sqrt(X[i2][i3]*delt)*Xa[i2][2][i3];
						zz[i2] += Wm[i3]*Z[i2][i3];
					}
					Pzz[i2]=0;
					for (i3=0;i3<(2*na+1);i3++)
					{
						Pzz[i2] += Wc[i3]*(Z[i2][i3]-zz[i2])*(Z[i2][i3]-
							zz[i2]);
					}
					prod[i2]=0.0;
					for (i3=0;i3<(2*na+1);i3++)
					{
						prod[i2] += Wc[i3]*(X[i2][i3]-x1[i2])* (Z[i2][i3]-
							zz[i2]);
					}
					K[i2]= prod[i2]/Pzz[i2];
					z = log_stock_prices[i1+1];
					estimates[i1+1] += zz[i2]/M;
					x2[i2] = x1[i2] + K[i2]*(z - zz[i2]);
					P[i2] = P1[i2] - K[i2]*K[i2] * Pzz[i2];
					xa[i2][0]=x2[i2];
					Pa[i2][0][0] = P[i2];
					if (x2[i2]<0) x2[i2]=0.0001;
					Pa[i2][1][0]= Pa[i2][0][1]= Pa[i2][1][2]
					=Pa[i2][2][1]= Pa[i2][0][2]=Pa[i2][2][0]=[0];
					/* sample */
					xx[i2] = x2[i2] + sqrt(P[i2])*
						norm.invcdf(ran2(&idum));
					if (xx[i2]<0) xx[i2]=0.00001;
					/* calculate weights */
					m = x2[i2];
					s = sqrt(P[i2]);
					q = 0.39894228/s * exp( - 0.5* (xx[i2] - m)*
						(xx[i2] - m)/(s*s) );
					m= log_stock_prices[i1] + (muS-0.5*xx[i2])*delt;
					s= sqrt(xx[i2]*delt);
					pz= 0.39894228/s * exp( - 0.5* (z - m)*
						(z - m)/(s*s) );
					m= x[i2] + ( omega-rho*xi*muS -
						(theta-0.5*rho*xi) * x[i2]) * delt +
						rho*xi* (log_stock_prices[i1]-
						log_stock_prices[i1-1]);
					s= xi*sqrt((1-rho*rho) * x[i2] * delt);
					px= 0.39894228/s * exp( - 0.5* (xx[i2] - m)*
						(xx[i2] - m)/(s*s) );
					w[i2]= MAX(pz, 1.0e-10) *
						MAX(px, 1.0e-10) / MAX(q, 1.0e-10);
					l += w[i2];
			}
			*ll += log(l);
			
			/* normalize weights */
			for (i2=0; i2<M; i2++)
					w[i2] /= l;

			/* resample and reset weights */
			c[0]=0;
			
			for (i2=1; i2<M; i2++)
				c[i2] = c[i2-1] + w[i2];
			i2=0;
			u[0] = 1.0/M * ran2.doub();
			
			for (i3=0; i3<M; i3++)
			{
				u[i3] = u[0] + 1.0/M *i3;
				while (u[i3] > c[i2])
					i2++;
				x[i3]= xx[i2];
				w[i3]=1.0/M;
			}
		}
		*ll *= -1.0;
		for (i2=0;i2<M;i2++)
		{
			for (i1=0;i1<na;i1++)
			{
				delete [] Pa[i2][i1];
				delete [] proda[i2][i1];
			}
		}
		for (i2=0;i2<M;i2++)
		{
			delete [] Pa[i2];
			delete [] proda[i2];
		}
		delete [] Pa;
		delete [] proda;
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

			estimate_particle_unscented_kalman_parameters_1_dim(
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
