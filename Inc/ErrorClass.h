#ifndef errorclass__
#define errorclass__
#include "main.h"
#include "mathfuncs.h"

extern bool errcorrection;

class errval{
    public:
    double val;
    double err;

    errval(double value = 0, double err = 0);

    errval operator+(errval);
    errval operator+(double);
    errval operator-(errval);
    errval operator-(double);
    errval operator*(errval);
    errval operator*(double);
    errval operator/(errval);
    errval operator/(double);
    errval operator+=(errval);
    errval operator-=(errval);
    errval operator*=(errval);
    errval operator/=(errval);
    bool operator>(errval);
    bool operator>(double);
    bool operator<(errval);
    bool operator<(double);
    bool operator!=(errval);
    bool operator!=(double);
    bool operator==(errval);
    bool operator==(double);
    friend std::ostream& operator<<(std::ostream&, const errval&);
};
double funcerr(double (*)(vector<double>), vector<errval>);

errval sqrt(errval);
errval pow(errval,errval);
errval sin(errval);
errval cos(errval);
errval tan(errval);
errval asin(errval);
errval acos(errval);
errval atan(errval);
errval log(errval);
errval log10(errval);
errval log(errval, errval);

#endif