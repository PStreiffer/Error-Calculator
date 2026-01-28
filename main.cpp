#include "ErrorClass.h"
#include "mathfuncs.h"
#include <iostream>
#include "funcvarlist.h"
#include "FunctionParse.h"
#include "FunctionExecute.h"
#include <string>
#include <iomanip>
using namespace std;

int main(){
    cout<<"Error Calculator version " <<"1.0"<<"\n";
    cout<<"developed by Phoebe Streiffer, 2025" <<"\n";
    cout<<"use precision(n) to set precision to n decimal places\n";
    cout<<"use exit() to exit, otherwise type expression to evaluate \n\n";
    string input = "";
    int precision = 2;
    while(true){
        cout<<setprecision(precision); //set correct precision
        input = "";
        getline(cin,input);
        if(input.find("exit()")!=string::npos){ //exit program
            return 0;
        }
        

        vector<vector<double>> parsedinput = functionparse(input); //parse input
        if(input.find("precision(")!=string::npos){ //add precision: # of decimal places
            precision = parsedinput[1][1];
            cout<<"Precision set to "<<precision<<" decimal places\n\n";
            continue;
        }
        if(parsedinput[0][0]!=560){ //return correct formatting
            evalresult result = functioneval(parsedinput);
            switch (result.resulttype){
                
                case 101: //'e' - errval
                    cout<<"= "<<result.erval<<"\n";
                    break;
                case 69: //'E' - ervalist
                    cout<<"= "<<result.erval<<"\n";
                    break;
                case 102: //'f' - func
                    cout<<result.funval.name << " created" <<"\n";
                    break;
                case 70: //'F': funcvalist
                    cout<<result.funval.name << " created" <<"\n";
                    break;
                case 118: //'v': variable
                    cout<<result.varval.name<<" = "<<result.varval.value<<"\n";
                    break;
            }
        }
        cout<<"\n";
    }
    return 0;
}