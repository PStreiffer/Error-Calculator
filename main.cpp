#include "ErrorClass.h"
#include "mathfuncs.h"
#include <iostream>
using namespace std;

int main(){
    errval x(2,2);
    errval y(5,6);
    errval z = sqrt(x*2*y);
    cout <<z;
    return;
}