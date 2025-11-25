#include <vector>
#include <iostream>
#include <string>
#include "ErrorClass.h"
#include "funcvarlist.h"
#include "functionparse.h"
class evalresult{
    public:
    int resulttype;
    errval erval;
    createdfunc funval;
    createdvar varval;
    evalresult(errval input){
        resulttype = 'e';
        erval = input;
    }
    evalresult(createdfunc input){
        resulttype = 'f';
        funval = input;
    }
    evalresult(createdvar input){
        resulttype = 'v';
        varval = input;
    }
};

evalresult functioneval(vector<vector<double>> arglist){
    vector<evalresult> calargs = {};
    char p;
    if (arglist[0][0] == -3 ){
        //special handling for new functions            
    }
    if(arglist[0][0]=-5){
        //special handling for new variables
    }
    for(int i = 0;i<size(arglist);i++){
        if (arglist[i][0] == '('){ //paren recursion - if paren detected, input enclosed arguments into functioneval
            calargs.push_back(functioneval(vector<vector<double>>(arglist.begin()+i+1,arglist.begin()+i+arglist[i][1])));
            arglist.erase(arglist.begin()+i+1,arglist.begin()+i+1+arglist[i][1]);
            arglist[i] = {-9,static_cast<double>(size(calargs)-1)};
        }
    }
    for(int i = 0; i<size(arglist);i++){ //multiplication, number replacement, variable replacement
        p = arglist[i][0];
        if (arglist[i][0] == 0){ //number replacement --> errval
            calargs.push_back(evalresult(errval(arglist[i][1])));
            arglist[i] = {-9, static_cast<double>(size(calargs)-1)};
        } else if(arglist[i][0]==-2){ //variable replacement --> variable value as errval
            calargs.push_back(evalresult(varlist[arglist[i][1]].value));
            arglist[i] = {-9,static_cast<double>(size(calargs)-1)};
        } else if(arglist[i][0]=='*' || arglist[i][0]=='/'){ //multiplication
            if(arglist[i+1][0]==-1){ //function: eval then multiply
                switch (static_cast<int>(arglist[i][0])){
                case '*':
                    calargs[size(calargs)-1]=evalresult(calargs[size(calargs)-1].erval*(funclist[arglist[i+1][1]]({calargs[arglist[i+2][1]].erval})));
                case '/':
                    calargs[size(calargs)-1]=evalresult(calargs[size(calargs)-1].erval/(funclist[arglist[i+1][1]]({calargs[arglist[i+2][1]].erval})));
                arglist.erase(arglist.begin()+i+2);
            }} else if(arglist[i+1][0] == -9){ //number, already converted to evalresult: multiply then delete
                switch (static_cast<int>(arglist[i][0])){
                case '*':
                    calargs[size(calargs)-1]=evalresult(calargs[size(calargs)-1].erval*calargs[arglist[i+1][1]].erval);
                case '/':
                    calargs[size(calargs)-1]=evalresult(calargs[size(calargs)-1].erval/calargs[arglist[i+1][1]].erval);
                }
                arglist.erase(arglist.begin()+i+2);
            } else if(arglist[i+1][0] == 0){ //number, not converted: convert to errval then multiply then delete
                switch (static_cast<int>(arglist[i][0])){
                case '*':
                    calargs[size(calargs)-1]=evalresult(calargs[size(calargs)-1].erval*errval(arglist[i+1][1]));
                case '/':
                    calargs[size(calargs)-1]=evalresult(calargs[size(calargs)-1].erval/errval(arglist[i+1][1]));
                }
                arglist.erase(arglist.begin()+i+2);
            } else if(arglist[i+1][0]==-2){ //variable, not converted: convert to errval then multiply then delete
                switch (static_cast<int>(arglist[i][0])){
                case '*':
                    calargs[size(calargs)-1]=evalresult(calargs[size(calargs)-1].erval*varlist[arglist[i+1][1]].value);
                case '/':
                    calargs[size(calargs)-1]=evalresult(calargs[size(calargs)-1].erval/varlist[arglist[i+1][1]].value);
                }
                arglist.erase(arglist.begin()+i+2);
            }
        }
    }
    return evalresult(errval(0));
}
//test code
int main(){
    auto c = functionparse("89.5+e-(cos(2)+7e)*((8)(2-6))");
    auto e = functioneval(c);
    return 0;
}