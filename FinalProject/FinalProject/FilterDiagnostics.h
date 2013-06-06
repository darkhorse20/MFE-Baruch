#include <string>


class FilterDiagnostics
{
private:
	double mean_prx_err, root_mean_sq_err, mean, sum_diff;
	void FilterDiagnostics::do_sum_diff(double *observations, double *estimates, int size);
public:
	FilterDiagnostics(void);
	~FilterDiagnostics(void);
	double mpe(double *observations, double *estimates, int size);
	double rmse(double *observations, double *estimates, int size);
	double box_ljung(double *observations, double *estimates, double *u, double *v, int size);
	double chi_squared_test(double *observations, double *estimates, double *u, double *v, int size);
	void histogram(double *observations, double *estimates, int size);
	void std_residuals(double *observations, double *estimates, double *u, double* v, int size);
};

