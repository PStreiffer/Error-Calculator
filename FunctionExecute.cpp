#include <vector>
#include <iostream>
#include <string>
#include <numeric>
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

evalresult functioneval(vector<vector<double>> arglist, bool funcdef = false, int funcvarstartin = -2){
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
    int funcvarstart = funcvarstartin;
    if (arglist[0][0] == -3){
        funcdef = true;
        funcvarstart = funcvarstartin; //vector of which variables are function variables in varlist; erase all at end
        if(funcvarstart==-2){
        for(int i = 2;arglist[i][0]!=')';i++){
            if(arglist[i][0]==-4){
            funcvarstart = arglist[i][1];
            break;
            }
        }
        }
        for(int i = 0; i<size(arglist);i++){
            if(arglist[i][0]=='='){
                arglist.erase(arglist.begin(),arglist.begin()+i+1);
                break;
            }
        }
        //special handling for new functions
        /*Workflow:
            1. separate by addition - create vector of vector of vector of doubles for each addition
            2. determine if there is a function variable in subvector - if no, evaluate as normal
            3. if there is a function variable, construct a function from bottom up
        */

            } if(arglist[0][0]==-5){
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
    int funcargnum = size(varlist)-funcvarstart;
     vector<int> upperassignlist(funcargnum);
        if(funcdef){
            iota(upperassignlist.begin(),upperassignlist.end(),0);
        }
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
                calargs.push_back(evalresult({functioneval(vector<vector<double>>(tempargs.begin(),tempargs.begin()+commanum[0]),funcdef,funcvarstart)}));
                for(int n = 1; n<size(commanum);n++){
                    calargs[size(calargs)-1].errvalist.push_back(functioneval(vector<vector<double>>(tempargs.begin()+commanum[n-1]+1,tempargs.begin()+commanum[n]),funcdef,funcvarstart).erval);
                }
            } else { //evaluate just one arg
                calargs.push_back(functioneval(tempargs,funcdef,funcvarstart));
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
            if(arglist[i][1]< funcvarstart){
                calargs.push_back(evalresult(varlist[arglist[i][1]].value));
                arglist[i] = {-9,static_cast<double>(size(calargs)-1)};
            } else {
                calargs.push_back(evalresult(createdfunc("funcvar",funcargnum,
                [arglist,i](vector<errval> a ){return a[arglist[i][1]];})));
                arglist[i] = {-9, static_cast<double>(size(calargs)-1)};
                assigner(calargs[size(calargs)-1].funval,{},upperassignlist);
            }
        } else if(arglist[i][0] == -1){ //functions - already have a -9 as input since paren right after
            if(calargs[arglist[i+1][1]].resulttype=='e'||calargs[arglist[i+1][1]].resulttype=='E'){
                calargs[arglist[i+1][1]]=evalresult(funclist[arglist[i][1]](calargs[arglist[i+1][1]].errvalist));
            } else {
                calargs[arglist[i+1][1]]=evalresult(concat(funclist[arglist[i][1]],calargs[arglist[i+1][1]].funvalist));
                assigner(calargs[arglist[i+1][1]].funval,{},upperassignlist);
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
                calargs[arglist[i-1][1]].funval = concat(power, {calargs[arglist[i-1][1]].funval,calargs[arglist[i+1][1]].funval});
                calargs[arglist[i-1][1]].resulttype = 'f';
                assigner(calargs[arglist[i-1][1]].funval,{},upperassignlist);
                
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
                calargs[arglist[i-1][1]].funval = concat(multiplication, {calargs[arglist[i-1][1]].funval,calargs[arglist[i+1][1]].funval});
                calargs[arglist[i-1][1]].resulttype = 'f';
                assigner(calargs[arglist[i-1][1]].funval,{},upperassignlist);
            }
            calargs[arglist[i+1][1]].resulttype=0;
            arglist.erase(arglist.begin()+i,arglist.begin()+i+2);
            i--;
        
        } else if(arglist[i][0] == '/'){ //division
            if(arglist[i-1][0] != -9 && arglist[i+1][0]!=-9){cout<<"Invalid operation: multiplying nonnumber"; return evalresult(errval(560));} //make sure arguments are correct - -9 on both sides
            if((calargs[arglist[i+1][1]].resulttype=='e'||calargs[arglist[i+1][1]].resulttype=='E')&&(calargs[arglist[i-1][1]].resulttype=='e'||calargs[arglist[i-1][1]].resulttype=='E')){
                calargs[arglist[i-1][1]].erval/=calargs[arglist[i+1][1]].erval;
            } else {
                calargs[arglist[i-1][1]].funval = concat(division, {calargs[arglist[i-1][1]].funval,calargs[arglist[i+1][1]].funval});
                calargs[arglist[i-1][1]].resulttype = 'f';
                assigner(calargs[arglist[i-1][1]].funval,{},upperassignlist);
            }
            calargs[arglist[i+1][1]].resulttype=0;
            arglist.erase(arglist.begin()+i,arglist.begin()+i+2);
            i--;
        }
    }

    errval sum = errval(0); //accumulator
    createdfunc sumfunc; //function accumulator
    sumfunc.argnum = size(varlist)-funcvarstart;
    vector<int> assignlist(sumfunc.argnum);
    iota(assignlist.begin(),assignlist.end(),0);
    for(int i = 0; i<size(arglist);i++){ //addition
        assignlist = vector<int>(assignlist.begin(),assignlist.begin()+sumfunc.argnum);
        if(arglist[i][0]==-9){
            if(calargs[arglist[i][1]].resulttype!=0){ //check to see if already incorporated
                if(calargs[arglist[i][1]].resulttype == 'e'|| calargs[arglist[i][1]].resulttype == 'E'){ //accumulate regular vals
                    sum+=calargs[arglist[i][1]].erval;
                } else { //accumulate functions
                    if(sumfunc.name == ""){ //assign sumfunc if not assigned
                        sumfunc.namedfunc = calargs[arglist[i][1]].funval.namedfunc;
                        sumfunc.name = calargs[arglist[i][1]].funval.name;
                    } else { //concat sumfunc with next function, assign variables to next function - this is never a constant, since would be added to sum instead
                        sumfunc = concat(addition, {sumfunc, calargs[arglist[i][1]].funval});
                        for(int i = 0; i<calargs[arglist[i][1]].funval.argnum;i++){
                            assignlist.push_back(i);
                        }
                        assigner(sumfunc,{},assignlist);
                    }
                }
            }
        }
    }
    if(sum!=0 && funcdef){ //add constants to funcdef at end by adding sum as constant, no need to assign constant function
                    if (sumfunc.name ==""){
                        sumfunc = createdfunc("constant",0,[sum](){return sum;});
                    } else {
                        sumfunc = concat(addition,{sumfunc,createdfunc("constant",0,[sum](){return sum;})});
                    }
                    assigner(sumfunc,{},assignlist);
                }
    if(funcdef){
        if(funcvarstartin==-2){
            sumfunc.name = funclist[size(funclist)-1].name;
            funclist[size(funclist)-1]=sumfunc;
        }
        return evalresult(sumfunc);
    } else{
        return evalresult(sum);
    }
    }
return(evalresult(errval(560)));
}
//test code
int main(){
    auto c = functionparse("f(x) = sqrt(2x)");
    functioneval(c);
    auto d = functioneval(functionparse("f(2)"));
    cout<<d.erval;
    return 0;
}