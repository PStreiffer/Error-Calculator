#include <string.h>
#include <vector>
#include <iostream>
#include "mathfuncs.h"
#include <cmath>
#pragma once
using namespace std;

class errval{
    public:
        double val; //value used for calculations
        double err; //error value
        errval(double value = 0, double error = 0){ //initialize val and err; err is always +
            val = value;
            err = abs(error);
        }

        errval operator+(errval rhs){ //addition operator
            double val1 = val +  rhs.val; //add vals together - simple addition
            double err1 = vectorsum({err,rhs.err}); //take vectorsum, as addition always gives error equal to vectorsum
            return errval(val1,err1);
        }
        errval operator+(double rhs){
            return operator+(errval(rhs));
        }
        errval operator-(errval rhs){ //same as addition
            double val1 = val - rhs.val;
            double err1 = vectorsum({err,rhs.err});
            return errval(val1,err1);
        }
        errval operator-(double rhs){
            return operator-(errval(rhs));
        }
        errval operator*(errval rhs); //initialize * operator
        errval operator*(double rhs){
            return operator*(errval(rhs));
        }
        errval operator/(errval rhs); //initialize division
        errval operator/(double rhs){
            return operator/(errval(rhs));
        }
        
        errval operator+=(errval rhs){
            *this = *this + rhs;
            return *this;
        }
        errval operator-=(errval rhs){
            *this = *this - rhs;
            return *this;
        }
        errval operator*=(errval rhs){
            *this = *this * rhs;
            return *this;
        }
        errval operator /= (errval rhs){
            *this = *this / rhs;
            return *this;
        }

        bool operator> (errval rhs){
            return val > rhs.val;
        };
        bool operator> (double rhs){
            return val > rhs;
        };
        bool operator< (errval rhs){
            return val < rhs.val;
        };
        bool operator< (double rhs){
            return val < rhs;
        };
        bool operator!=(errval rhs){
            return (val != rhs.val && err != rhs.err);
        }
        bool operator!=(double rhs){
            return (val != rhs);
        }
        bool operator==(errval rhs){
            return (val == rhs.val && err == rhs.err);;
        }
        bool operator==(double rhs){
            return (val == rhs && err == 0);
        }
        friend std::ostream& operator<<(std::ostream& stream, const errval& errval){ //output stream
            stream << errval.val << " \u00b1 " << errval.err;
            return stream;
        }
};

double funcerr(double (*f)(vector<double>), vector<errval> values){ //function for taking error of function based on values, error
    vector<double> vals = {};
    //put all values into one vector
    for(int i = 0; i<size(values);i++){
        vals.push_back(values[i].val);
    }
    //calculate individual errors of function based on function & values + error
    vector<double> subterms = {};
    for(int i=0; i<size(vals);i++){
        subterms.push_back(partial(f,vals,i)*values[i].err); //add each partial to subterms
    }
    return vectorsum(subterms); //calculate vectorsum
}

errval errval::operator*(errval rhs){ //actual definiton of *
    double err1 = funcerr(
        [](vector<double> a){return a[0]*a[1];}, //define lambda function that multiplies two values
        {*this,rhs} //use errorvals as inputs to function
    ); 
    double val1 = val * rhs.val; //multiply values together
    return errval(val1,err1);
}
errval errval::operator/(errval rhs){ //actual definition of divison
    double err1 = funcerr(
        [](vector<double> a){return a[0]/a[1];}, //define lambda function that divides two values
        {*this, rhs}
    );
    
    return errval(val / rhs.val,err1);
}

errval sqrt(errval x){
    return errval(sqrt(x.val), funcerr([](vector<double> a){return sqrt(a[0]);}, {x}));
}

errval pow(errval x, errval y){
    return errval(pow(x.val,y.val), funcerr([](vector<double> a){return pow(a[0],a[1]);}, {x,y}));
}
// k = sqrt(partialfx*errorx + partialfy*errory)

errval sin(errval x){
    return errval(sin(x.val), funcerr([](vector<double> a){return sin(a[0]);}, {x}));
}
errval cos(errval x){
    return errval(cos(x.val), funcerr([](vector<double> a){return cos(a[0]);}, {x}));
}
errval tan(errval x){
    return errval(tan(x.val), funcerr([](vector<double> a){return tan(a[0]);}, {x}));
}