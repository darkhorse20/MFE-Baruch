#include "nr3.h"
#include "mins_ndim.h"

Doub f(VecDoub_I & x)
{
    Doub retval;

    //Calculate return value, retval,  as a function of x[0], x[1], ...

    return retval;
}

int main()
{
    VecDoub x(2); // Make its dimension whatever your function is defined for
    VecDoub xx;   // Its size will be set in the return from minimize()

    x[0] = 0.0;
    x[1] = 0.0;

    // Feed the function to the constructor
    Powell <Doub(VecDoub_I &)> powell(f);

    // Use the minimize member function
    xx = powell.minimize(x);

    // Print the following if you are interested
    //cout << "Number of iterations =  " << powell.iter << endl << endl;

    cout << scientific;

    cout << "Minimum function value of f = "
         << powell.fret
         << ", found at: " << endl;
    for (Int i = 0; i < xx.size(); i++) {
        cout << "  x[" << i << "] = " << setw(13) << xx[i] << endl;
    }
    cout << endl << endl;

    return 0;
}