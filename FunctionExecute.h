#include <vector>
#include <iostream>
#include <string>
#include <numeric>
#include "ErrorClass.h"
#include "funcvarlist.h"
#include "functionparse.h"
#include "funcconstructors.h"
#pragma once
class evalresult{
    public:
    int resulttype;
    errval erval;
    createdfunc funval;
    createdvar varval;
    vector<errval> errvalist;
    vector<createdfunc> funvalist;
    evalresult(vector<errval> input){
        resulttype = 'E';
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
        funvalist = {input};
    }
    evalresult(createdvar input){
        resulttype = 'v';
        varval = input;
    }
    evalresult(vector<createdfunc> input){
        resulttype = 'F';
        funvalist = input;
        funval = funvalist[0];
    }
};

/* Workflow:
- determine if function/var defition, or if evaluation
Var:
1. Determine if there is a pm: if so, then one side is a function to be evaluated, the other is an error; if not, only an eval
2. Evaluate the function and set it equal to the value + add the error if there is any
3. Evaluate the error by taking vector sum of function error and user defined error
4. Add variable to varlist

Function: same as Evaluation, but instead of numbers, composes functions

Evaluation:
1. Evaluate Parentheses, put values into calargs & set arglist position to pointer to calargs - use recursion
2. Evaluate numbers, functions, and variables, put into calargs & set arglist position to pointer to calargs
3. Evaluate ^, *, /: both sides should have -9 as code, so directly modify left hand one in calargs
    - evaluate powers first, then multiplication & division
    - evaluate left to right
4. evaluate +: keep accumulator, return final value
*/  

evalresult functioneval(vector<vector<double>> arglist, bool funcdef = false, int funcvarstartin = -2){
    errcorrection = false;
    vector<evalresult> calargs = {}; //stores subterms of values; for eval, each individual term will be added together at end unless resulttype = 0
    int funcvarstart = funcvarstartin; //vector of which variables are function variables in varlist; erase all at end

    if (arglist[0][0] == -3){ 
        //special handling for new functions - defines setup to move into evaluation
        funcdef = true;
        if(funcvarstart==-2){ //determine where function variables begin
        for(int i = 2;arglist[i][0]!=')';i++){
            if(arglist[i][0]==-4){
            funcvarstart = arglist[i][1];
            break;
            }
        }
        }
        for(int i = 0; i<size(arglist);i++){ //remove function definition junk from beginning: start argument eval from after = sign
            if(arglist[i][0]=='='){
                arglist.erase(arglist.begin(),arglist.begin()+i+1);
                break;
            }
        }

            } if(arglist[0][0]==-5){
        //special handling for new variables - evaluate expression then add error
        bool found = false;
        errval value = 0;
        for(int i = 0; i<size(arglist); i++){
            
            if(arglist[i][0] == -15695){ //determine if error included: if so, evaluate before ± then vectorsum errors - i is location of ±
                value = functioneval(vector<vector<double>>(arglist.begin()+2,arglist.begin()+i)).erval;
                if(size(arglist) >= i+3){
                    if(arglist[i+2][0] == '%'){
                        arglist[i+1][1]*=0.01;
                        arglist[i+1][1]*=value.val;
                    }
                }
                value.err = vectorsum({value.err,arglist[i+1][1]});
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
    int funcargnum = size(varlist)-funcvarstart; //number of function variables
    vector<int> upperassignlist(funcargnum); //list of variables assigned in numeric order, to use for assignment later
    if(funcdef){ //add numeric ordered variables
        iota(upperassignlist.begin(),upperassignlist.end(),0);
    }
    for(int i = 0;i<size(arglist);i++){ //parentheses eval
        if (arglist[i][0] == '('){ //paren recursion - if paren detected, input enclosed arguments into functioneval
            vector<vector<double>>tempargs (arglist.begin()+i+1,arglist.begin()+i+arglist[i][1]); 
            vector<int> commanum = {}; //indices of commas in paren
            int infunctil = -1; //flag for how long inside another function , to evaluate as block 
            for(int arg = 0; arg<size(tempargs);arg++){ //check how many args
                if(tempargs[arg][0] == -1 && arg>infunctil){
                    infunctil = arg + 1 + tempargs[arg+1][1];
                }
                if(tempargs[arg][0] == ',' && arg>infunctil){
                    commanum.push_back(arg);
                }
            }
            if(size(commanum)>0){ //if multiple args, evaluate as list
                commanum.push_back(size(tempargs));
                calargs.push_back(evalresult({functioneval(vector<vector<double>>(tempargs.begin(),tempargs.begin()+commanum[0]),funcdef,funcvarstart)})); //recursive element: evaluates subfunction then adds it to list
                for(int n = 1; n<size(commanum);n++){ //add onto error value list
                    if(funcdef){ //add on extra elements for function def
                        calargs[size(calargs)-1].funvalist.push_back(functioneval(vector<vector<double>>(tempargs.begin()+commanum[n-1]+1,tempargs.begin()+commanum[n]),funcdef,funcvarstart).funval);
                    } else{ //add on extra elements for regular eval
                        calargs[size(calargs)-1].errvalist.push_back(functioneval(vector<vector<double>>(tempargs.begin()+commanum[n-1]+1,tempargs.begin()+commanum[n]),funcdef,funcvarstart).erval);
                    }
                }
            } else { //evaluate just one arg
                calargs.push_back(functioneval(tempargs,funcdef,funcvarstart));
            }
            arglist.erase(arglist.begin()+i+1,arglist.begin()+i+1+arglist[i][1]);
            arglist[i] = {-9,static_cast<double>(size(calargs)-1)}; //-9 used to inicate value has been evaluated, and is in calargs
        }
    }


    for(int i = 0; i<size(arglist);i++){ //number replacement, variable replacement, function eval
        if (arglist[i][0] == 0){ //number replacement --> errval
            if(funcdef){
                errval val(arglist[i][1]);
                calargs.push_back(evalresult(createdfunc("constant",funcargnum,[val](vector<errval> a){return val;})));
            } else {
                calargs.push_back(evalresult(errval(arglist[i][1])));
            }
            arglist[i] = {-9, static_cast<double>(size(calargs)-1)};

        } else if(arglist[i][0]==-2){ //variable replacement --> variable value as errval
            if(arglist[i][1]< funcvarstart || funcvarstart < 0){ //test to see if function variable: if not, eval as normal
                if(funcdef){
                    errval val = varlist[arglist[i][1]].value;
                    calargs.push_back(evalresult(createdfunc("constant",funcargnum,[val](vector<errval> a){return val;})));
                } else{
                    calargs.push_back(evalresult(varlist[arglist[i][1]].value));
                }
                arglist[i] = {-9,static_cast<double>(size(calargs)-1)};
            } else { //if is, add as function instead, returning same value out as appropriate index 
                const int arg = arglist[i][1]-funcvarstart;
                calargs.push_back(evalresult(createdfunc(
                    "funcvar",
                    funcargnum,
                    [arg](vector<errval> a){return a[arg];})));

                arglist[i] = {-9, static_cast<double>(size(calargs)-1)};
            }
        } else if(arglist[i][0] == -1){ //functions - already have a -9 as input since paren right after
            if(calargs[arglist[i+1][1]].resulttype=='e'||calargs[arglist[i+1][1]].resulttype=='E'){ //general use case determinant if errval or func
                calargs[arglist[i+1][1]]=evalresult(funclist[arglist[i][1]](calargs[arglist[i+1][1]].errvalist));
            } else {
                calargs[arglist[i+1][1]]=evalresult(compose(funclist[arglist[i][1]],calargs[arglist[i+1][1]].funvalist));
                vector<int> assignlist = {}; //create assignment list; make same size as number of args to assign
                for(createdfunc a: calargs[arglist[i+1][1]].funvalist){
                    for (int arg: upperassignlist){
                        assignlist.push_back(arg);
                    }
                }
                calargs[arglist[i+1][1]].funval = assigner(calargs[arglist[i+1][1]].funval,assignlist); //assign args appropriately
            }
            arglist.erase(arglist.begin()+i);
        }
    }

    for(int i = 0; i<size(arglist);i++){ //powers
        if (arglist[i][0] == '^'){ 
            if((calargs[arglist[i+1][1]].resulttype=='e'||calargs[arglist[i+1][1]].resulttype=='E')&&(calargs[arglist[i-1][1]].resulttype=='e'||calargs[arglist[i-1][1]].resulttype=='E')){
                if(arglist[i-1][0] != -9 && arglist[i+1][0]!=-9){cout<<"Invalid operation: taking exponent of nonnumber"; return evalresult(errval(560));} //make sure arguments are correct - -9 on both sides
                
                calargs[arglist[i-1][1]].erval = pow(calargs[arglist[i-1][1]].erval,calargs[arglist[i+1][1]].erval);
            } else {
                calargs[arglist[i-1][1]].funval = compose(power, {calargs[arglist[i-1][1]].funval,calargs[arglist[i+1][1]].funval});
                calargs[arglist[i-1][1]].resulttype = 'f';
                vector<int> assignlist = upperassignlist;
                for (int arg: upperassignlist){
                    assignlist.push_back(arg);
                }
                calargs[arglist[i-1][1]].funval = assigner(calargs[arglist[i-1][1]].funval,assignlist);
                
            }
            calargs[arglist[i+1][1]].resulttype = 0;
            arglist.erase(arglist.begin()+i,arglist.begin()+i+1);
        }
        
    }
    
    for(int i = 0; i<size(arglist);i++){ //multiplication/division
        if(arglist[i][0] == '*'){ //multiplication
            if(arglist[i-1][0] != -9 && arglist[i+1][0]!=-9){cout<<"Invalid operation: multiplying nonnumber"; return evalresult(errval(560));} //make sure arguments are correct - -9 on both sides
            if((calargs[arglist[i+1][1]].resulttype=='e'||calargs[arglist[i+1][1]].resulttype=='E')&&(calargs[arglist[i-1][1]].resulttype=='e'||calargs[arglist[i-1][1]].resulttype=='E')){
                calargs[arglist[i-1][1]].erval*=calargs[arglist[i+1][1]].erval;
            } else {
                calargs[arglist[i-1][1]].funval = compose(multiplication, {calargs[arglist[i-1][1]].funval,calargs[arglist[i+1][1]].funval});
                calargs[arglist[i-1][1]].resulttype = 'f';
                vector<int> assignlist = upperassignlist;
                for (int arg: upperassignlist){
                    assignlist.push_back(arg);
                }
                calargs[arglist[i-1][1]].funval = assigner(calargs[arglist[i-1][1]].funval,assignlist);
            }
            calargs[arglist[i+1][1]].resulttype=0;
            arglist.erase(arglist.begin()+i,arglist.begin()+i+2);
            i--;
        
        } else if(arglist[i][0] == '/'){ //division
            if(arglist[i-1][0] != -9 && arglist[i+1][0]!=-9){cout<<"Invalid operation: multiplying nonnumber"; return evalresult(errval(560));} //make sure arguments are correct - -9 on both sides
            if((calargs[arglist[i+1][1]].resulttype=='e'||calargs[arglist[i+1][1]].resulttype=='E')&&(calargs[arglist[i-1][1]].resulttype=='e'||calargs[arglist[i-1][1]].resulttype=='E')){
                calargs[arglist[i-1][1]].erval/=calargs[arglist[i+1][1]].erval;
            } else {
                calargs[arglist[i-1][1]].funval = compose(division, {calargs[arglist[i-1][1]].funval,calargs[arglist[i+1][1]].funval});
                calargs[arglist[i-1][1]].resulttype = 'f';
                vector<int> assignlist = upperassignlist;
                for (int arg: upperassignlist){
                    assignlist.push_back(arg);
                }
                calargs[arglist[i-1][1]].funval = assigner(calargs[arglist[i-1][1]].funval,assignlist);
            }
            calargs[arglist[i+1][1]].resulttype=0;
            arglist.erase(arglist.begin()+i,arglist.begin()+i+2);
            i--;
        }
    }

    errval sum = errval(0); //accumulator
    createdfunc sumfunc; //function accumulator
    sumfunc.argnum = size(varlist)-funcvarstart;
    for(int i = 0; i<size(arglist);i++){ //addition
        if(arglist[i][0]==-9){
            if(calargs[arglist[i][1]].resulttype!=0){ //check to see if already incorporated
                if(calargs[arglist[i][1]].resulttype == 'e'|| calargs[arglist[i][1]].resulttype == 'E'){ //accumulate regular vals
                    sum+=calargs[arglist[i][1]].erval;
                } else { //accumulate functions
                    if(sumfunc.name == ""){ //assign sumfunc if not assigned
                        sumfunc.namedfunc = calargs[arglist[i][1]].funval.namedfunc;
                        sumfunc.name = calargs[arglist[i][1]].funval.name;
                    } else { //compose sumfunc with next function, assign variables to next function - this is never a constant, since would be added to sum instead
                        sumfunc = compose(addition, {sumfunc, calargs[arglist[i][1]].funval});
                        vector<int> assignlist = upperassignlist;
                        for (int arg: upperassignlist){
                            assignlist.push_back(arg);
                        }
                        sumfunc = assigner(sumfunc,assignlist);
                    }
                }
            }
        }
    }
    if(sum!=0 && funcdef){ //add constants to funcdef at end by adding sum as constant - only for pure constant terms
                    if (sumfunc.name ==""){
                        sumfunc = createdfunc("constant",funcargnum,[sum](){return sum;});
                    } else {
                        sumfunc = compose(addition,{sumfunc,createdfunc("constant",funcargnum,[sum](){return sum;})});
                    }
                    vector<int> assignlist = upperassignlist;
                    for (int arg: upperassignlist){
                        assignlist.push_back(arg);
                    }
                    sumfunc = assigner(sumfunc,assignlist);
                }

    if(funcdef){ //determine whether to output sum or sumfunc, and erase funcvariables if necessary
        if(funcvarstartin==-2){
            sumfunc.name = funclist[size(funclist)-1].name;
            funclist[size(funclist)-1]=sumfunc;
            varlist.erase(varlist.begin()+funcvarstart,varlist.begin()+funcvarstart+funcargnum); //erase function variables from arglist to free up variable names
        }
        
        return evalresult(sumfunc);
    } else{
        return evalresult(sum);
    }
    }

return(evalresult(errval(560))); //error code if no return value for some reason
}
//test code
/*int main(){
    functioneval(functionparse("a = 2+-1"));
    auto c = functionparse("f(x,y,z)=sqrt(xy)+acos(x+2)-2xz");
    functioneval(c);
    auto d = functioneval(functionparse("f(5,5,2)"));
    cout<<d.erval;
    return 0;
}
    */