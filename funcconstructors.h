#include "ErrorClass.h"
#include "funcvarlist.h"

createdfunc addition("addition",2,[](vector<errval>a){return a[0]+a[1];});
createdfunc multiplication("multiplication",2,[](vector<errval>a){return a[0]*a[1];});
createdfunc division("division",2,[](vector<errval>a){return a[0]/a[1];});
createdfunc power("power",2,[](vector<errval>a){return pow(a[0],a[1]);});

createdfunc concat(const createdfunc upperfunc, const vector<createdfunc> argfuncs) {
    //take a parent function and functions are arguments and composes upperfunc of argfuncs
    //returns concat of upperfunc as createdfunc w/ correct argnum
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
        for(int i = 0; i<size(argindex);i++){
            argsols.push_back(argfuncs[i].namedfunc(vector<errval>(a.begin()+argindex[i][0],a.begin()+argindex[i][1]+1)));
        }
        return upperfunc.namedfunc(argsols);
    }
    );
};

void assigner(createdfunc func, vector<errval> constants, vector<int> assigntype){
    /*assigntype dictates what each variable is:
        - <0 - constant: does not take in value, constant number equal to number in assigntype plus one
        - >=0 - variable: takes in value, variable number equal to number in assigntype
        if assigntype > 0, constant in that slot is not used
    */
    int totalargs = 0;
    for (int type: assigntype){
        if (type>totalargs){
            totalargs = type;
        }
    }
    func = createdfunc(
        func.name, //name
        totalargs+1, //argnum
        [constants,assigntype,func](vector<errval> a){
            vector<errval> input = {};
            for (int type: assigntype){
                if(type>=0){
                    input.push_back(a[type]);
                } else {
                    input.push_back(constants[-type-1]);
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