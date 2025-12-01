#include <vector>
#include <iostream>
#include <string>
#include "ErrorClass.h"
#include "funcvarlist.h"
#include "functionparse.h"
#include "funcconstructors.h"
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

evalresult functioneval(vector<vector<double>> arglist, bool subargument = false, vector<int> funcvarlocsin = {}){
    vector<evalresult> calargs = {}; //stores subterms of values; for eval, each individual term will be added together at end unless resulttype = 0
/* Workflow:
- determine if function/var defition, or if evaluation
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
    if (arglist[0][0] == -3 ||subargument == true){
        //special handling for new functions
        /*Workflow:
            1. separate by addition - create vector of vector of vector of doubles for each addition
            2. determine if there is a function variable in subvector - if no, evaluate as normal
            3. if there is a function variable, construct a function from bottom up
        */
        vector<int> funcvarlocs = funcvarlocsin; //vector of which variables are function variables in varlist; erase all at end
        if(size(funcvarlocs)==0){
        for(int i = 2;arglist[i][0]!=')';i++){
            if(arglist[i][0]==-4)
            funcvarlocs.push_back(arglist[i][1]);
        }
        }
        vector<vector<vector<double>>> funcevalargs = {{}}; //arglist subdivided by addition to combine later
        int n = 0;
        int inparen = 0;
        bool funcarg = false; //test if there is a function variable in the subvector: if not, evaluate immediately
        arglist.push_back({'+'}); //add extra + to make sure last argument is read
        for(int i = 4+funclist[arglist[0][1]].argnum; i<size(arglist);i++){
            if(arglist[i][0]=='+'&&i>inparen){
                if(funcarg == false){
                    calargs.push_back(functioneval(funcevalargs[n]));
                    funcevalargs[n] = {{-9,static_cast<double>(size(calargs)-1)}};
                }
                inparen=0;
                funcevalargs.push_back({});
                n+=1;
                funcarg=false;
            } else {
                if(arglist[i][0]==-2){
                    for(int argnum: funcvarlocs){
                        if(arglist[i][1]==argnum){
                            funcarg=true;
                            break;
                        }
                    }
                } else if(arglist[i][0]=='('&&inparen==0){
                    inparen=arglist[i][1]+i;
                }
                funcevalargs[n].push_back(arglist[i]);
            }
        }
        funcevalargs.erase(funcevalargs.end()-1);
        createdfunc foundation("foundation");
        for(vector<vector<double>> argset: funcevalargs){ //base case: no paren, no addition --> only functions, mulitiplication, division, powers
            for(int argnum = 0;argnum<size(argset);argnum++){
                if(argset[argnum][0] == -1){
                    
                }
            }
        }
    
            } else if(arglist[0][0]==-5){
        //special handling for new variables - evaluate expression then add error
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
            vector<int> commanum = {};
            for(int arg = 0; arg<size(tempargs);arg++){ //check how many args
                if(tempargs[arg][0] == ','){
                    commanum.push_back(arg);
                    break;
                }
            }
            if(size(commanum)>0){ //if multiple args, evaluate as list
                commanum.push_back(size(tempargs));
                calargs.push_back(evalresult({functioneval(vector<vector<double>>(tempargs.begin(),tempargs.begin()+commanum[0]))}));
                for(int n = 1; n<size(commanum);n++){
                    calargs[size(calargs)-1].errvalist.push_back(functioneval(vector<vector<double>>(tempargs.begin()+commanum[n-1]+1,tempargs.begin()+commanum[n])).erval);
                }

            } else { //evaluate just one arg
            calargs.push_back(functioneval(tempargs));
            }
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
    auto c = functionparse("f(x) = sqrt(2x)");
    functioneval(c);
    return 0;
}