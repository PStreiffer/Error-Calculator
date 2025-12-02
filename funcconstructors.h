#include "ErrorClass.h"
#include "funcvarlist.h"

createdfunc addition("addition",2,[](vector<errval>a){return a[0]+a[1];});
createdfunc multiplication("multiplication",2,[](vector<errval>a){return a[0]*a[1];});
createdfunc division("division",2,[](vector<errval>a){return a[0]/a[1];});
createdfunc power("power",2,[](vector<errval>a){return pow(a[0],a[1]);});

createdfunc compose(const createdfunc upperfunc, const vector<createdfunc> argfuncs) {
    //take a parent function and functions as arguments and composes upperfunc of argfuncs
    //returns composition of upperfunc as createdfunc w/ argnum equal to sum of argnums of composites
    vector<vector<int>> argindex = {};
    for(createdfunc arg: argfuncs){
        if(size(argindex)==0){
            argindex.push_back({0,arg.argnum-1});
            continue;
        }
        argindex.push_back({argindex[size(argindex)-1][1]+1,argindex[size(argindex)-1][1]+arg.argnum});
    }
    return createdfunc(
        upperfunc.name + " concat", //name
        argindex[size(argindex)-1][1]+1, //size
        [upperfunc, argindex, argfuncs](const vector<errval> a){
        vector<errval> argsols = {};
        for(int i = 0; i<size(argindex);i++){ //create vector of arguments for upperfunc
            argsols.push_back(argfuncs[i].namedfunc(vector<errval>(a.begin()+argindex[i][0],a.begin()+argindex[i][1]+1)));
        }
        return upperfunc.namedfunc(argsols);
    }
    );
};

createdfunc assigner(createdfunc func, vector<int> assigntype){
    //takes in function and list of assignments and outputs function where each argument is mapped to arg in assigntype
    //acts as translation: args of new function --> args of original function
    //argnum is maximum argnum reached by assigntype
    int totalargs = 0;
    for (int type: assigntype){ //determine maximum argnum --> new argnum
            totalargs = type;
    }
    return createdfunc(
        func.name, //name
        totalargs+1, //argnum
        [assigntype,func](vector<errval> a){
            vector<errval> input = {};
            for (int type: assigntype){ //assign values in a to inputs
                if(type>=0){
                    input.push_back(a[type]);
                }
            }
            return func.namedfunc(input);
        }
    );
    
};
/*test code
int main(){
    auto c = concat(addition, {multiplication, division});
    cout<<c({1,2,3,4})<<"\n";
    auto d = concat(division,{multiplication,division});
    cout<<d({1,2,3,4})<<"\n";
    cout<<c({1,2,3,4})<<"\n";
    return 0;
}
*/