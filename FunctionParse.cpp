#include "ErrorClass.h"
#include <vector>
#include <variant>
#include <iostream>
#include <string>
#include "funcvarlist.h"
#include <algorithm>
using namespace std;


vector<vector<double>> functionparse(string input){
    sort(funclist.begin(),funclist.end(),[](createdfunc x, createdfunc y){return size(x.name)>size(y.name);});
    vector<char> arglist = {'+','-','*','/','^', '(', ')','='};
    vector<vector<double>> argpostype = {};
    //+-*/^ operator recognition & number concat
    for (int i = 0; i<input.length();i++){ //identify basic operators
        for(int argnum=0; argnum < arglist.size();argnum++){
            if(input[i]==arglist[argnum]){
                argpostype.push_back({static_cast<double>(arglist[argnum])});
            }
        }
        if(input[i] == ')'){ //paren matching
                    try{
                    for(int i2 = size(argpostype)-2;i2>=0;i2--){
                        if(argpostype[i2][0] == '(' && size(argpostype[i2])==1){
                            argpostype[i2].push_back(size(argpostype)-1);
                            break;
                        }
                        if(i2 == 0){
                            throw(i);
                        }
                    }
                    } catch (int x){
                        cout<<"Invalid closed parentheses\n";
                        return {{560}};
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
            for(int n = i+1;isalpha(input[n])&& input[n]!='E'&&input[n]!='e';n++){
                s+=static_cast<char>(input[n]);
            }
            if(input[i+size(s)]=='('){
            for(int funcnum = 0; funcnum <size(funclist);funcnum++){
                if(s.find(funclist[funcnum].name)==size(s)-size(funclist[funcnum].name) && size(funclist[funcnum].name)<=size(s)){
                    s.erase(size(s)-size(funclist[funcnum].name),size(s)-1);
                    argpostype.push_back({-1,static_cast<double>(funcnum)});
                }
            }
            }
            i+=s.size()-1;
        }
    }
    for(vector<double>arg: argpostype){
        if(arg[0]=='('&&size(arg)==1){
            cout<<"Invalid open parentheses\n";
        }
    }
    
    
    return argpostype;
}

int main(){
    string func = "(3+5-(cos(85.2)*(9*5))-2E8)";
    vector<vector<double>> c = functionparse(func);
    vector<createdfunc> d = funclist;
    return 0;
}