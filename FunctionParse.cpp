#include "ErrorClass.h"
#include <vector>
#include <variant>
#include <iostream>
#include <string>
using namespace std;


vector<vector<double>> functionparse(string input){
    vector<char> arglist = {'+','-','*','/','^', '(', ')','='};
    vector<vector<double>> argpostype = {};
    //+-*/^ operator recognition & number concat
    for (int i = 0; i<input.length();i++){ //identify basic operators
        for(int argnum=0; argnum < arglist.size();argnum++){
            if(input[i]==arglist[argnum]){
                argpostype.push_back({static_cast<double>(arglist[argnum])});
            }
        }
        if(isdigit(input[i])|| input[i]=='.'){ //number concatenation
            string s = "";
            s+=input[i];
            int n = i+1;
            while(isdigit(input[n]) || input[n]=='.' ||input[n]=='E'){
                s+=static_cast<char>(input[n]);
                n++;
            } 
            i = n-1;
            
            argpostype.push_back({0,stod(s)});
        }
        if(isalpha(input[i])&& input[i]!='E'&&input[i]!='e'){ //alpha concatenation
            string s = "";
            s+=input[i];
            int n = i+1;
            while(isalpha(input[n])&& input[n]!='E'&&input[n]!='e'){
                s+=static_cast<char>(input[n]);
                n++;
            } if (input[n]=='('){
            }
            i = n-1;
            
            argpostype.push_back({0,});
        }
    }
    int parennum  = 0;
    try{
    for (int i = 0; i<size(argpostype);i++){ //parentheses validation
        int closednum = 0;
        if (argpostype[i][0] == '('){
            parennum+=1;
            for(int i1 = size(argpostype)-1; i1>=0;i1--){
                if(argpostype[i1][0]== ')'){
                    closednum+=1;
                    if(closednum==parennum){
                    argpostype[i].push_back(i1);
                    break;
                    }
                }
            }
            if(parennum!=closednum){
                    throw 1;
                    break;
                }
        }
    
    }
    } catch (int s){
        cout<<"Mismatched Parentheses";
    }
    
    
    return argpostype;
}

int main(){
    string func = "(3+5-(85.2*(9*5))-2E8)";
    vector<vector<double>> c = functionparse(func);
    return 0;
}