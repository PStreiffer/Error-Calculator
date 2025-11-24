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
    int equalnum = string::npos;
    //+-*/^ operator recognition & number concat
    for (int i = 0; i<input.length();i++){ //identify basic operators
        for(int argnum=0; argnum < arglist.size();argnum++){
            if(isdigit(input[i])||isalpha(input[i])||input[i]=='.'){break;}
            if(input[i]==arglist[argnum]){
                argpostype.push_back({static_cast<double>(arglist[argnum])});
                break;
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
                    } catch (int x){ //closed paren verification - make sure enough open paren for closed
                        cout<<"Invalid closed parentheses\n";
                        return {{560}};
                    }
                }
        if(isdigit(input[i])|| input[i]=='.'){ //number concatenation
            string s = "";
            s+=input[i];
            int n = i+1;
            while(isdigit(input[n]) || input[n]=='.' ||input[n]=='E'){
                if(input[n]=='E'&& isdigit(input[n+1]) == false){
                    cout << "Invalid scientific notation";
                    return {{560}};
                }
                s+=static_cast<char>(input[n]);
                n++;

            } 
            i = n-1;
            
            argpostype.push_back({0,stod(s)});
        }
        if(isalpha(input[i])&& input[i]!='E'){ //alpha concatenation
            vector<double> funcinsert = {0};
            string s = "";
            s+=input[i];
            for(int n = i+1;isalpha(input[n])&& input[n]!='E';n++){
                s+=static_cast<char>(input[n]);
            }
            if (equalnum == string::npos){
            for(int charnum = 0; charnum < size(input);charnum++){
                if (input[charnum]=='='){equalnum = charnum; break;}
            } 
            }
                //function/var creation handling
                if(i+size(s) <= equalnum){
                    
                    if(input[i+size(s)]=='('){ //function creation
                        if(size(argpostype)>0){
                        cout<<"Does not support explicit functions\n";
                        return {{560}};
                        }
                        funclist.push_back(createdfunc(s,[](vector<errval> x){return 0;}));
                        argpostype.push_back({-3,static_cast<double>(size(funclist))});
                        i+=size(s);
                        s.erase();

                    } else if (input[i+1]==','){ //function variable creation
                        if(size(s)>1){cout<<"Does not support variables with multi-char names\n";return{{560}};}
                        varlist.push_back(createdvar(s[0],errval(0)));
                        argpostype.push_back({-4,static_cast<double>(size(varlist))});
                        i+=size(s);
                        s.erase();

                    } else { //new variable creation
                        if(size(s)>1){cout<<"Does not support variables with multi-char names\n";return{{560}};}
                        varlist.push_back(createdvar(s[0],errval(0)));
                        argpostype.push_back({-5,static_cast<double>(size(varlist))});
                        i+=size(s);
                        s.erase();
                    }
                    i+=size(s)-1;
                }
                
            
            char p = input[i];
            if(input[i+size(s)]=='('){ //function detection + removal - always a ( afterwards
            for(int funcnum = 0; funcnum <size(funclist);funcnum++){
                if(s.find(funclist[funcnum].name)==size(s)-size(funclist[funcnum].name) && size(funclist[funcnum].name)<=size(s)){
                    i+=s.size()-1;
                    s.erase(size(s)-size(funclist[funcnum].name),size(s));
                    funcinsert = {-1,static_cast<double>(funcnum)};
                    break;
                }
            }
            }   
            try{
            
            for(char c: s){ //try string chars against varlist
                 bool found = false;
                for ( int varnum = 0; varnum < size(varlist); varnum++){
                    if (c == varlist[varnum].name){
                        argpostype.push_back({-2,static_cast<double>(varnum)});
                        found = true;
                        break;
                    }
                    
                }
                if(!found){throw(c);}

            }
            } catch (char x){ //catch any invalid variables
                cout<<"\'"<< x <<"\' is an invalid variable";
                return {{560}};
            }
            if (funcinsert[0] != 0){ //add function into arglist
                argpostype.push_back(funcinsert);
            
            }
        
        }
    }
    for(vector<double>arg: argpostype){ //open paren verification - make sure every open paren has a closed one
        if(arg[0]=='('&&size(arg)==1){
            cout<<"Invalid open parentheses\n";
            return {{560}};
        }
    }
    return argpostype;
}

int main(){
    string func = "a= (3+5-(cos(85.2)*(9*5))-2E8)";
    vector<vector<double>> c = functionparse(func);
    return 0;
}