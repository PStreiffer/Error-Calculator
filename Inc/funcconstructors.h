#ifndef funcconstructors__
#define funcconstructors__
#include "main.h"
#include "funcvarlist.h"

extern createdfunc addition;
extern createdfunc multiplication;
extern createdfunc division;
extern createdfunc power;

createdfunc compose(const createdfunc, const vector<createdfunc>);

createdfunc assigner(createdfunc, vector<int>);



#endif