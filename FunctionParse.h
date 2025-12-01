#include "ErrorClass.h"
#include <vector>
#include <iostream>
#include <string>
#include "funcvarlist.h"
#include <algorithm>
using namespace std;


vector<vector<double>> functionparse(string input){
    int inversepos;
    sort(funclist.begin(),funclist.end(),[](createdfunc x, createdfunc y){return size(x.name)>size(y.name);});
    for (int funcnum = 0; funcnum <size(funclist);funcnum++){
        if(funclist[funcnum].name=="inverse"){
            inversepos = funcnum;
            break;
        }
    }
    vector<char> arglist = {'*','+','/','^', '(', ')','=',','};

    vector<vector<double>> argpostype = {};
    signed int equalnum = -1;
    for (int i = 0; i<input.length();i++){ //identify per-character operators
        for(int argnum=0; argnum < arglist.size();argnum++){ //basic operators in arglist
            
            if(isdigit(input[i])||isalpha(input[i])||input[i]=='.'){break;}
            //arg modification - plus minus addition
            if(input[i]=='+' && input[i+1]=='-'){
                argpostype.push_back({-15695});
                i++;
                break;
            }
            if(input[i]=='-'){ //convert - signs into inversions
                argpostype.push_back({'+'});
                argpostype.push_back({-1,static_cast<double>(inversepos)});
                if(input[i+1]!='('){
                    argpostype.push_back({'(',2,2});
                }
                break;
            }
            if(input[i]==arglist[argnum]){
                argpostype.push_back({static_cast<double>(arglist[argnum])});
                break;
            }
            
        }
        if(input[i] == ')'){ //paren matching
                    try{
                    for(int i2 = size(argpostype)-2;i2>=0;i2--){
                        if(argpostype[i2][0] == '(' && size(argpostype[i2])==1){
                            argpostype[i2].push_back(size(argpostype)-1-i2);
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
                    if(input[i+1]=='('){
                        argpostype.push_back({'*'});
                    }
                }
        else if(isdigit(input[i])|| input[i]=='.'){ //number concatenation
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
            if(input[i+1]=='('){
                argpostype.push_back({'*'});
            }
            argpostype.push_back({0,stod(s)});
        }
        else if(isalpha(input[i])&& input[i]!='E'){ //alpha concatenation
            vector<double> funcinsert = {0};
            string s = "";
            s+=input[i];
            for(int n = i+1;isalpha(input[n])&& input[n]!='E';n++){
                s+=static_cast<char>(input[n]);
            }
            if (equalnum == -1){
            equalnum = input.find("=");
            }
            //function/var creation handling
            if(static_cast<signed int>(i+size(s)) <= equalnum){   
                int argnum = 0;        
                if(input[i+size(s)]=='('){ //function creation
                    if(size(argpostype)>0){
                    cout<<"Does not support explicit functions\n";
                    return {{560}};
                    }
                    funclist.push_back(createdfunc(s,1,[](vector<errval> x){return 0;}));
                    argpostype.push_back({-3,static_cast<double>(size(funclist)-1)});
                    i+=size(s);
                    s.erase();

                } else if (input[i+1]==',' || input[i+1]==')'){ //function variable creation
                    if(size(s)>1){cout<<"Does not support variables with multi-char names\n";return{{560}};}
                    varlist.push_back(createdvar(s[0],errval(0)));
                    argpostype.push_back({-4,static_cast<double>(size(varlist)-1)});
                    i+=size(s);
                    s.erase();
                    argnum++;
                } else { //new variable creation
                    if(size(s)>1){cout<<"Does not support variables with multi-char names\n";return{{560}};}
                    varlist.push_back(createdvar(s[0],errval(0)));
                    argpostype.push_back({-5,static_cast<double>(size(varlist)-1)});
                    i+=size(s);
                    s.erase();
                }
                funclist[size(funclist)-1].argnum=argnum;
                i+=size(s)-1;
            }  else{ 
                if(input[i+size(s)]=='('){ //function detection + removal - always a ( afterwards
                for(int funcnum = 0; funcnum <size(funclist);funcnum++){
                    if(string(s.end()-size(funclist[funcnum].name),s.end())==funclist[funcnum].name){
                        i+=size(funclist[funcnum].name);
                        s.erase(size(s)-size(funclist[funcnum].name),size(s));
                        funcinsert = {-1,static_cast<double>(funcnum)};
                        break;
                    }
                }
                }   
                try{
                for(char c: s){ //variable detection - all vars are one char, so no need to delete b/c iterate thru list
                    bool found = false;
                    for ( int varnum = 0; varnum < size(varlist); varnum++){
                        if (c == varlist[varnum].name){
                            if(size(argpostype)>0){
                            if(argpostype[size(argpostype)-1][0]==')'||argpostype[size(argpostype)-1][0]<=0){ //add implicit multiplication
                                argpostype.push_back({'*'});
                            }
                            }
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
                    if(size(argpostype)>0){
                    if(argpostype[size(argpostype)-1][0]==')'||argpostype[size(argpostype)-1][0]<=0){//add implicit multiplication
                                argpostype.push_back({'*'});
                    }
                    }  
                    argpostype.push_back(funcinsert);
                
                }
                i+=size(s)-1;
            }
        
        }
    }
    for(int argnum = 0; argnum<size(argpostype);argnum++){ //open paren verification - make sure every open paren has a closed one
        if(argpostype[argnum][0]=='('){
            if(size(argpostype[argnum])==3){ //insert floating closed paren from inversion addition earlier
                argpostype.insert(argpostype.begin()+argnum+argpostype[argnum][2],{')'});
                argpostype[argnum] = {'(',argpostype[argnum][1]};
            }
            if(size(argpostype[argnum])==1){ //invalid open paren detection
            cout<<"Invalid open parentheses\n";
            return {{560}};
        }
    }

    }
    return argpostype;
}
/* test cases

int main(){
    string funcin = "89.5+e-(cos(2)+7e)*((8)(2-6))";
    vector<vector<double>> c = functionparse(funcin);
    return 0;
}
*/