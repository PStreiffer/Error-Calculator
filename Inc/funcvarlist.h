#ifndef funcvarlist__
#define funcvarlist__

#include "main.h"
#include "ErrorClass.h"
class createdfunc{
    public:
        string name;
        int argnum;
        function<errval(vector<errval>)> namedfunc;

        createdfunc();

        createdfunc(string,int, function<errval(vector<errval>)>);

        createdfunc(string, int, function<errval()>);

        errval operator()(vector<errval>);
        
};

class createdvar{
    public:
    char name;
    errval value;
    createdvar();
    createdvar(char, errval);
};

extern vector<createdfunc> funclist;
extern vector<createdvar> varlist;

#endif