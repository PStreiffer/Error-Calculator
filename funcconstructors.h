#include "ErrorClass.h"

function<errval(errval, errval)> addition = [](errval a, errval b){return a+b;};
function<errval(errval,errval)> multiplication = [](errval a, errval b){return a*b;};
function<errval(errval,errval)> division = [](errval a, errval b){return a/b;};
function<errval(errval,errval)> power = [](errval a, errval b){return pow(a,b);};

function<function<errval(vector<errval>)>(function<errval(errval,errval)>, function<errval(errval,errval)>)> concat = 
[](function<errval(errval,errval)> a, function<errval(errval,errval)> b){return [a,b](vector<errval> c){return a(b(c[0],c[1]),c[2]);};}; //concatenates two functions together

function<errval(vector<errval>)> assigner(function<errval(vector<errval>)> func, vector<bool> isconst, vector<errval>constvals){ //assigns constant values to a function
    if (size(isconst)!=size(constvals)){cout<<"List of constants must be equal to number of constant bools";return 0;}
    function<vector<errval>(vector<errval>)> listmaker = [isconst, constvals](vector<errval> a ){
        int i = 0;
        vector<errval> result = {};
        for(int n = 0; n<size(isconst);n++){
            if(isconst[n]){
                result.push_back(constvals[n]);
            } else {
                result.push_back(a[i]);
                n++;
            }
        }
        return result;
    };
    return [func, listmaker](vector<errval> a){return func(listmaker(a));};
}