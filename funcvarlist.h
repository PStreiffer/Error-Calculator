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
    function<errval(vector<errval>)> namedfunc;
    createdfunc(string namein = " ", function<errval(vector<errval>)> functionin = [](vector<errval> x){return 0;}){
        name = namein;
        namedfunc = functionin;
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
                                createdfunc("sqrt",[](vector<errval> a){return sqrt(a[0]);}),
                                createdfunc("sin",[](vector<errval> a){return sin(a[0]);}),
                                createdfunc("cos",[](vector<errval> a){return cos(a[0]);}),
                                createdfunc("tan",[](vector<errval> a){return tan(a[0]);}),
                                createdfunc("inverse",[](vector <errval> a){return a[0]*(-1.0);})
                            };

vector<createdvar> varlist = {
                                createdvar('e',errval(exp(1.0),0.0))
                            };