#include "ErrorClass.h"
#include <vector>
using namespace std;

errval functiondo(errval (*f)(vector<errval>), vector<errval> x){
    return f(x);
}

errval functionparse(string input){
    vector<string> args ={};
    while(input.find("+")!=-1){
        
    }
}