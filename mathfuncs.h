#include <cmath>
#include <limits>
#include <functional>
#include <vector>
#pragma once
using namespace std;
const double h = sqrt(std::numeric_limits<double>::epsilon());

 double ddx(double (*f)(double),double x){
    return (f(x+h)-f(x-h))/(2*h);
};

double partial(double (*f)(vector<double>),vector<double> values, int varnum=0){
    vector<double> s1 = values;
    vector<double> s2 = values;
    s1[varnum]+=h;
    s2[varnum]+=-h;
    return (f(s1)-f(s2))/(2*h);
}

double vectorsum(vector<double> vals){
    double sum = 0;
    for(int i=0; i<size(vals);i++){
        sum+=pow(vals[i],2);
    }
    return sqrt(sum);
}
