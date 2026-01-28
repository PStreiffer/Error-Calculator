#include <vector>
#include <variant>
#include <string>
#include "ErrorClass.h"
#include <cmath>
#pragma once
using namespace std;

class createdfunc {
    public:
    string name;
    int argnum;
    function<errval(vector<errval>)> namedfunc;
    createdfunc(string namein, int argnumin, function<errval(vector<errval>)> functionin){
        name = namein;
        argnum = argnumin;
        namedfunc = functionin;
    }
    createdfunc(string namein = "",int argnumin = 0, function<errval()> functionin = [](){return 0;}){
        name = namein;
        argnum = argnumin;
        namedfunc = [functionin](vector<errval> a){return functionin();};
    }
    errval operator()(vector<errval> input) {
        return namedfunc(input);
    }
};
class createdvar {
    public:
    char name;
    errval value;
    createdvar(char namein = ' ', errval valin = 0){
        name = namein;
        value = valin;
    }
};

vector<createdfunc> funclist = {
                                createdfunc("sqrt",1,[](vector<errval> a){return sqrt(a[0]);}),
                                createdfunc("sin",1,[](vector<errval> a){return sin(a[0]);}),
                                createdfunc("cos",1,[](vector<errval> a){return cos(a[0]);}),
                                createdfunc("tan",1,[](vector<errval> a){return tan(a[0]);}),
                                createdfunc("inverse",1,[](vector <errval> a){return a[0]*(-1.0);}),
                                createdfunc("ln",1,[](vector<errval> a){return log(a[0]);}),
                                createdfunc("log",1,[](vector<errval> a){return log10(a[0]);}),
                                createdfunc("loga",2,[](vector<errval> a){return log(a[0],a[1]);}),
                                createdfunc("arccos",1,[](vector<errval>a){return acos(a[0]);}),
                                createdfunc("arcsin",1,[](vector<errval>a){return asin(a[0]);}),
                                createdfunc("arctan",1,[](vector<errval>a){return atan(a[0]);})
                            };

vector<createdvar> varlist = {
                                createdvar('e',errval(exp(1.0),0.0)),
                                createdvar('g',errval(9.80665, 0.0)),
                                createdvar('c',errval(300000000,0.0)),
                                createdvar('h',errval(6.62607015E-34,0.0))
                            };