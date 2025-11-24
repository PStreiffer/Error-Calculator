#include <vector>
#include <iostream>
#include <string>
#include "funcvarlist.h"
#include "functionparse.h"

vector<double> functioneval(vector<vector<double>> arglist){
    vector<errval> storederrvals = {};
    vector<vector<double>> activeargs = {};
    char p;
    for(int i = 0; i<size(arglist);i++){
        p = arglist[i][0];
        if (arglist[i][0] == '('){ //paren recursion - if paren detected, input enclosed arguments into functioneval
            activeargs = vector<vector<double>>(arglist.begin()+i+1,arglist.begin()+i+arglist[i][1]);
            activeargs.erase(arglist.begin()+i+1,arglist.begin()+i+1+arglist[i][1]);
            activeargs[i]=functioneval(activeargs);
        }
    }
    return {0};
}
//test code
int main(){
    auto c = functionparse("89.5+e-(cos(2)+7e)*((8)(2-6))");
    auto e = functioneval(c);
    return 0;
}