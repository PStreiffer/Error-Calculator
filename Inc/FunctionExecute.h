#ifndef functionexecute__
#define functionexecute__
#include "main.h"
#include "ErrorClass.h"
#include "funcvarlist.h"
#include "funcconstructors.h"



class evalresult{
    public:
    int resulttype;
    errval erval;
    createdfunc funval;
    createdvar varval;
    vector<errval> errvalist;
    vector<createdfunc> funvalist;
    evalresult(vector<errval>);
    evalresult(errval);
    evalresult(createdfunc);
    evalresult(createdvar);
    evalresult(vector<createdfunc>);
};

evalresult functioneval(vector<vector<double>> arglist, bool funcdef = false, int funcvarstartin = -2);

#endif
