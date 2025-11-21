#include "ErrorClass.h"
#include <vector>
#include <variant>
using namespace std;

errval functiondo(errval (*f)(vector<errval>), vector<errval> x){
    return f(x);
}

errval functionparse(string input){
    vector<vector<variant<string,int>>> args ={};
    while(input.find("+")!=-1){
        args.push_back({input.find("+"),1});
    }
}