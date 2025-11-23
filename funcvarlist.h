#include <vector>
#include <variant>
#include <string>
#include "ErrorClass.h"
#include <cmath>
using namespace std;

class createdfunc {
    public:
    string name;
    function<errval(vector<errval>)> namedfunc;
    createdfunc(string namein, function<errval(vector<errval>)> functionin){
        name = namein;
        namedfunc = functionin;
    }
    errval operator()(vector<errval> input) {
        return namedfunc(input);
    }
};

vector<createdfunc> funclist = {createdfunc("sqrt",[](vector<errval> a){return sqrt(a[0]);}),
                                createdfunc("sin",[](vector<errval> a){return sin(a[0]);}),
                                createdfunc("cos",[](vector<errval> a){return cos(a[0]);}),
                                createdfunc("tan",[](vector<errval> a){return tan(a[0]);})
                            };