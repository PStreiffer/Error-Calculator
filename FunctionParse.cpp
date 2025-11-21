#include "ErrorClass.h"
#include <vector>
#include <variant>
#include <iostream>
#include <string>
using namespace std;

errval functiondo(errval (*f)(vector<errval>), vector<errval> x){
    return f(x);
}

vector<vector<int>> functionparse(string input){
    vector<char> arglist = {'+','-','*','/','^'};
    vector<vector<int>> argpostype = {};
    for (int i = 0; i<input.length();i++){
        for(int argnum=0; argnum < arglist.size();argnum++){
            if(input[i]==arglist[argnum]){
                argpostype.push_back({i,argnum});
            }
        }
        
    }
    return argpostype;
}

int main(){
    string func = "2 + 2 / 3 * 5 -83";
    vector<vector<int>> c = functionparse(func);
    return 0;
}