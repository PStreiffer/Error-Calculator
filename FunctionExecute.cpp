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
    vector<errval> errvalist;
    evalresult(vector<errval> input){
        resulttype = 'l';
        errvalist = input;
        erval = input[0];
    }
    evalresult(errval input){
        resulttype = 'e';
        erval = input;
        errvalist = {input};
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
/* Workflow:
- determine if function/var definition, or if evaluation
Function/var:
TBD - already separated out tho
Evaluation:
1. Evaluate Parentheses, put values into calargs & set arglist position to pointer to calargs - use recursion
2. Evaluate numbers, functions, and variables, put into calargs & set arglist position to pointer to calargs
3. Evaluate ^, *, /: both sides should have -9 as code, so directly modify left hand one in calargs
    - evaluate powers first, then multiplication & division
    - evaluate left to right
4. evaluate +: keep accumulator, return final value
*/
    if (arglist[0][0] == -3 ){
        //special handling for new functions            
    } else if(arglist[0][0]==-5){
        //special handling for new variables
        bool found = false;
        errval value = 0;
        for(int i = 0; i<size(arglist); i++){
            
            if(arglist[i][0] == -15695){ //determine if error included: if so, evaluate before ± then vectorsum errors
                value = functioneval(vector<vector<double>>(arglist.begin()+2,arglist.begin()+i)).erval;
                value.err = vectorsum({value.err,arglist[size(arglist)-1][1]});
                found = true;
                break;
            }
            
        }   
            if(!found){ //if no ±, then evaluate expression and set variable equal to result
             value = functioneval(vector<vector<double>>(arglist.begin()+2,arglist.end())).erval;
            }
            varlist[arglist[0][1]].value=value;
            return(varlist[arglist[0][1]]); //returns newly created variable
            
        
    } else { //evaluation
    for(int i = 0;i<size(arglist);i++){ //parentheses
        if (arglist[i][0] == '('){ //paren recursion - if paren detected, input enclosed arguments into functioneval
            vector<vector<double>>tempargs (arglist.begin()+i+1,arglist.begin()+i+arglist[i][1]);
            calargs.push_back(functioneval(tempargs));
            arglist.erase(arglist.begin()+i+1,arglist.begin()+i+1+arglist[i][1]);
            arglist[i] = {-9,static_cast<double>(size(calargs)-1)};
        }
    }


    for(int i = 0; i<size(arglist);i++){ //number replacement, variable replacement, function eval
        if (arglist[i][0] == 0){ //number replacement --> errval
            calargs.push_back(evalresult(errval(arglist[i][1])));
            arglist[i] = {-9, static_cast<double>(size(calargs)-1)};

        } else if(arglist[i][0]==-2){ //variable replacement --> variable value as errval
            calargs.push_back(evalresult(varlist[arglist[i][1]].value));
            arglist[i] = {-9,static_cast<double>(size(calargs)-1)};

        } else if(arglist[i][0] == -1){ //functions - already have a -9 as input since paren right after
            calargs[arglist[i+1][1]]=evalresult(funclist[arglist[i][1]](calargs[arglist[i+1][1]].errvalist));
            arglist.erase(arglist.begin()+i);
        }
    }

    for(int i = 0; i<size(arglist);i++){ //powers
        if (arglist[i][0] == '^'){ 
            if(arglist[i-1][0] != -9 && arglist[i+1][0]!=-9){cout<<"Invalid operation: taking exponent of nonnumber"; return evalresult(errval(560));} //make sure arguments are correct - -9 on both sides
            calargs[arglist[i-1][1]].erval = pow(calargs[arglist[i-1][1]].erval,calargs[arglist[i+1][1]].erval);
            calargs[arglist[i+1][1]].resulttype=0;
            arglist.erase(arglist.begin()+i,arglist.begin()+i+1);
        }
        
    }
    
    for(int i = 0; i<size(arglist);i++){ //multiplication/division
        if(arglist[i][0] == '*'){ //multiplication
            if(arglist[i-1][0] != -9 && arglist[i+1][0]!=-9){cout<<"Invalid operation: multiplying nonnumber"; return evalresult(errval(560));} //make sure arguments are correct - -9 on both sides
            calargs[arglist[i-1][1]].erval*=calargs[arglist[i+1][1]].erval;
            calargs[arglist[i+1][1]].resulttype=0;
            arglist.erase(arglist.begin()+i,arglist.begin()+i+2);
            i--;
        
        } else if(arglist[i][0] == '/'){ //division
            if(arglist[i-1][0] != -9 && arglist[i+1][0]!=-9){cout<<"Invalid operation: multiplying nonnumber"; return evalresult(errval(560));} //make sure arguments are correct - -9 on both sides
            calargs[arglist[i-1][1]].erval/=calargs[arglist[i+1][1]].erval;
            calargs[arglist[i+1][1]].resulttype=0;
            calargs[arglist[i+1][1]].resulttype=0;
            arglist.erase(arglist.begin()+i,arglist.begin()+i+2);
            i--;
        }
    }

    errval sum = errval(0); //accumulator
    for(int i = 0; i<size(arglist);i++){ //addition
        if(arglist[i][0]==-9){
            if(calargs[arglist[i][1]].resulttype!=0){
            sum+=calargs[arglist[i][1]].erval;
            }
        }
    }
    return(evalresult(sum));
    }
return(evalresult(errval(560)));
}
//test code
int main(){
    auto c = functionparse("a=2*(90)+-2");
    functioneval(c);
    auto d = functionparse("aecos(ae)");
    auto e = functioneval(d).erval;
    cout<<e;
    return 0;
}