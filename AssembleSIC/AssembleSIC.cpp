//
//  main.cpp
//  AssembleSIC
//
//  Created by YuMIng Haung on 2019/4/4.
//  Copyright © 2019 YuMIng Haung. All rights reserved.
//

#include <iostream>
#include<fstream>
#include <string>
#include<map>
#include <typeinfo>
#include <cstdlib>
#include<sstream>
#include<math.h>
#include <iomanip>
using namespace std;
map<string,pair<string,int>> table;
map<string,pair<string,int>>::iterator iter;
int flag=0;
// compare str
//search marker you want
int searchMarker(string a,int position){
    if(a.find(' ',position)!=-1){
        return a.find(' ',position);
    }else{
        flag = 1;
        return a.length();
    }
}
void deleteMark(string &str){
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
}

int strcompare(string a,string b){
    if(a.length()!=b.length()){
        return -1;
    }
    for(int i=0;i<a.length();i++){
        //        cout<<"a:"<<a[i]<<" b:"<<b[i]<<endl;
        if(a[i]!=b[i]){
            return -1;
        }
    }
    return 1;
}
// give first and second opcode
string searchMap(string a){
    for(iter=table.begin();iter!= table.end();iter++){
        //cout<<iter->first<<iter->second<<endl;
        if(strcompare(a,iter->first)==1){
            return iter->second.first;
        }
    }
    return "null";
}


//decide first or end
string Label(string label){
    if (strcompare(label,"START")==1){
        return "1";
    }else if(strcompare(label,"END")==1){
        return "0";
    }else{
        return "-1";
    }
    return "test";
}
//Calulate operationCode start position
int OperationCodeIsStart(string operationCode){
    if (operationCode.compare("START")==0){
        //cout<<"START!"<<endl;
        return 1;
    }else if(operationCode.compare("END")==0){
        //cout<<"END!"<<endl;
        return 0;
    }
    return -1;
}
//Recursively
//將一個十六進位數字字串進行加法運算
string covertHexadecimal(string a,int addition,int position){
    int number;
    //printf("a[position]:%d\n",a[position]);
    if(a[position]>64){
        number = a[position]-55;
    }else{
        number = a[position]-48;
    }
    number+=addition;
    //printf("number:%d\n",number);
    if(number<0){
        a = covertHexadecimal(a,-1,position-1);
        a = covertHexadecimal(a,addition+16,position);
    }else if(number<10 && number>=0){
        a[position] = number+48;
    }else if(number>9&&number<16){
        a[position] = number+55;
    }else{
        int carry = number/16;
        number = number%16;
        a = covertHexadecimal(a,carry,position-1);
        a[position] = number+48;
    }
    return a;
}
int convertDecimal(char a){
    if(a>64){
        return a-55;
    }else{
        return a-48;
    }
}
//十六進制轉十進制
int hexadecimalConvertDecimal(string a){
    return convertDecimal(a[0])*pow(16,3)+convertDecimal(a[1])*pow(16,2)+convertDecimal(a[2])*pow(16,1)
    +convertDecimal(a[3])*pow(16,0);
}
//十進制轉十六進制
string decimalConvertHexadecimal(int a){
    int i,j,answer;
    string str;
    if(a<0){
        str = "0FFF";
        answer = hexadecimalConvertDecimal(str)-(-1)*a;
        str = decimalConvertHexadecimal(answer);
    }else{
        str = "    ";
        for (i=0,j=3;j>=0;j--,i++,a/=16) {
            answer = a%16;
            if(answer>9){
                str[j] = answer+55;
            }else{
                str[j] = answer+48;
            }
        }
    }
//    cout<<str<<endl;
    return str;
}
string creatTcard(int length,int nowPosition,string *obj,string *location,int start){
    string str;
    str.append("T,");
    str.append(location[start].c_str());
    str.append(",");
    str.append(decimalConvertHexadecimal(length));
    if(length==0){
        return "";
    }
    for(int r=start;r<=nowPosition;r++){
        str.append(",");
        str.append(obj[r].c_str());
    }
    return str;
}
int main(int argc, const char * argv[]) {
    //------------------------------------------------------------------------------------------------------
    //declare
    int i = 0,j=0,length=0,addition,objLength = 0,k=0,max = hexadecimalConvertDecimal("FFFF");
    int min = hexadecimalConvertDecimal("0000");
    map<int,string> errorMessage;
    map<int,string>::iterator errorMessageIter;
    map<string,pair<int,string>> symbolTable;
    map<string,pair<int,string>>::iterator symbolTableIter;
    string Scard,Ecard;
    string *Tcard = new string[100];
    string str,temp;
    string *location = new string[100];
    string *lable = new string[100];
    string *operationCode = new string[100];
    string *operand = new string[100];
    string *obj = new string[100];
    //------------------------------------------------------------------------------------------------------
    //Create Table
    table["ADD"] = make_pair("18",3);table["DIV"] =  make_pair("24",3);table["MUL"] = make_pair("20",3);
    table["COMP"] = make_pair("28",3);table["J"] =  make_pair("3C",3);table["SUB"] = make_pair("1C",3);
    table["JEQ"] =  make_pair("30",3);table["JGT"] = make_pair("34",3);
    table["JLT"] = make_pair("38",3);table["JSUB"] = make_pair("48",3);
    table["LDA"] = make_pair("00",3);table["LDB"] = make_pair("68",3);table["LDCH"] = make_pair("50",3);
    table["LDF"] = make_pair("70",3);table["LDL"] = make_pair("08",3);table["LDS"] = make_pair("6C",3);
    table["LDT"] = make_pair("74",3);table["LDX"] = make_pair("04",3);table["RSUB"] = make_pair("4C",3);
    table["STA"] = make_pair("0C",3);table["STB"] = make_pair("78",3);table["STCH"] = make_pair("54",3);
    table["STF"] = make_pair("80",3);table["STI"] = make_pair("D4",3);table["STL"] = make_pair("14",3);
    table["STS"] = make_pair("7C",3);table["STSW"] = make_pair("E8",3);table["STT"] = make_pair("84",3);
    table["STX"] = make_pair("10",3);table["TD"] = make_pair("E0",3);table["RD"] = make_pair("D8",3);
    table["TIX"] = make_pair("2C",3);table["WD"] = make_pair("DC",3);
    //------------------------------------------------------------------------------------------------------
    table["ADDR"] = make_pair("90",2);table["CLEAR"] = make_pair("B4",2);table["COMPR"] = make_pair("A0",2);
    table["DIVR"] =  make_pair("9C",2);table["MULR"] = make_pair("98",2);table["SUBR"] = make_pair("94",2);
    //------------------------------------------------------------------------------------------------------
    table["START"] = make_pair("NULL",0);table["END"] = make_pair("NULL",0);//no have obj
    table["RESB"] = make_pair("NULL",0);table["RESW"] = make_pair("NULL",0);
    //------------------------------------------------------------------------------------------------------
    table["WORD"] = make_pair("00",3);table["BYTE"] = make_pair(" ",0);

    //------------------------------------------------------------------------------------------------------
    //Open three file: input,obj,ist
//    cout<<"Please input file:";
//    cin>>str;
    string path = "";
    ifstream input(path);
    ofstream list,card;
    card.open("path/card.txt");
    list.open("path/list.txt");
    if(!input){
        cout<<"open is fail\n";
    }
    //------------------------------------------------------------------------------------------------------
    while(getline(input,str)){
        flag=0;
        if(str==" "||str[0]=='.'){
            continue;
        }
//        printf("%-5d%-5d%-5d%-5d\n",str.length(),searchMarker(str,0),searchMarker(str,8),searchMarker(str,16));
        flag=0;
        if(searchMarker(str,0)<=str.length()){
            lable[i] = str.substr(0,7);//(position,lenght)
//            cout<<lable[i]<<endl;
//            cout<<"flag:"<<flag<<endl;
        }if((flag!=1) && (searchMarker(str,8)<=str.length())){
            operationCode[i] = str.substr(8,8);
//            cout<<operationCode[i]<<endl;
//            cout<<"flag:"<<flag<<endl;
        }if((flag!=1) && (searchMarker(str,16)<=str.length())){
            operand[i] = str.substr(16,8);
//            cout<<"flag:"<<flag<<endl;
        }else{
            operand[i] = "";
        }
        deleteMark(lable[i]);
        deleteMark(operationCode[i]);
        deleteMark(operand[i]);
        
        iter = table.find(operationCode[i].c_str());
        if(iter==table.end()){
            errorMessage[i] = operationCode[i];
        }
//        cout<<str<<endl;  // example_1.txt
//        printf("%-5d%-8s%-8s%-8s\n",i,lable[i].c_str(),operationCode[i].c_str(),operand[i].c_str());
        i++;
    }
    //------------------------------------------------------------------------------------------------------
//    確認總共有幾行指令
    length = i-1;
    printf("length:%d\n",length);
    //------------------------------------------------------------------------------------------------------
    int start = 0;
    location[start] = operand[start];
    for(j = start;j<=length;j++) {
//        printf("%-5d%-8s%-8s%-8s\n",j,lable[j].c_str(),operationCode[j].c_str(),operand[j].c_str());
        //start error
//        if(!operationCode[j].compare("START") && !lable[j].compare("")){
//            printf("START errors\n");
//            return 0;
//        }
//        if(!operationCode[j].compare("START")){
//            for(k=0;k<4;k++){
//                if(operand[j][k]>57 || operand[j][k]<48){
//                    printf("START error\n");
//                    return 0;
//                }
//            }
//        }
        
        if(!errorMessage.empty()){
            printf("Error!!\n");
            for(errorMessageIter = errorMessage.begin();errorMessageIter!=errorMessage.end();errorMessageIter++){
                printf("%-2d %-8s\n",errorMessageIter->first,errorMessageIter->second.c_str());
            }
            return 0;
        }else{
            obj[j] = searchMap(operationCode[j]);
//            不會生成機器碼的走這裡
            if(!table[operationCode[j].c_str()].first.compare("NULL")){//actual location
                if(!operationCode[j].compare("RESW")){
                    //RESW表示我們要定義一個字（word）型數據，並分配儲存空間。
                    addition =hexadecimalConvertDecimal(location[j]) + atoi(operand[j].c_str())*3;
                    location[j+1] = decimalConvertHexadecimal(addition);
                //RESB 表示我們要定義一個位元組（byte）型數據，並分配儲存空間。
                }else if(!operationCode[j].compare("RESB")){
                    addition =hexadecimalConvertDecimal(location[j]) + atoi(operand[j].c_str());
                    location[j+1] = decimalConvertHexadecimal(addition);
                }else{
                    location[j+1] = location[j];
                }
            }else{
//                operationCode[j]等於"BYTE"，則compare函數將傳回值0
                if(!operationCode[j].compare("BYTE")){
                    //字元型數據
                    if(operand[j][0]=='C'){
                        //減三個字元，像是C''
                        addition = hexadecimalConvertDecimal(location[j]) + (operand[j].length()-3);
                        location[j+1] = decimalConvertHexadecimal(addition);
                    }else{
                        //表示16進制
                        addition = hexadecimalConvertDecimal(location[j]) + (operand[j].length()-3)/2;
                        location[j+1] = decimalConvertHexadecimal(addition);
                    }
                }else{
                    addition = hexadecimalConvertDecimal(location[j]) + table[operationCode[j].c_str()].second;
                    location[j+1] = decimalConvertHexadecimal(addition);
                }
            }
            //沒有Label的加入symbolTable
            if(!lable[j].empty()){
                symbolTable[lable[j].c_str()] = make_pair(j,location[j].c_str());
//                printf("%s:%s\n",lable[j].c_str(),symbolTable[lable[j].c_str()].second.c_str());
            }
//            printf("%-2d%-8s%-10s%-8s%-8s\n",j,operationCode[j].c_str(),operand[j].c_str(),obj[j].c_str(),location[j].c_str());
        }
    }
    //------------------------------------------------------------------------------------------------------
    for(j = start;j<=length;j++) {
        // end error
//        if(!operationCode[j].compare("END")){
//            for(symbolTableIter=symbolTable.begin();symbolTableIter!= symbolTable.end();symbolTableIter++){
//                if(strcompare(operand[j].c_str(),symbolTableIter->first)==1){
//                    cout<<symbolTableIter->first<<endl;
//                    cout<<"operand:"<<operand[j].c_str()<<endl;
//                    flag = 3;
//                }
//            }
//            if(flag==3){
//                printf("END error\n");
//            }
//            return 0;
//        }
        if(!errorMessage.empty()){
            return 0;
        }else{
            // 用於產生程式的起始卡（Start Card）
            if(!operationCode[j].compare("START")){
                Scard.append("S,");
                Scard.append(operationCode[j].c_str());
                Scard.append(",");
                Scard.append(lable[j].c_str());
                Scard.append(",");
                int a = hexadecimalConvertDecimal(location[length].c_str());
                int b = hexadecimalConvertDecimal(location[j].c_str());
                Scard.append(decimalConvertHexadecimal(a-b));
//                cout<<Scard<<endl;
            }
            // 用於產生程式的結束卡（End Card）
            if(!operationCode[j].compare("END")){
                Ecard.append("E,");
                Ecard = Ecard + symbolTable[operand[j].c_str()].second;
//                cout<<Ecard<<endl;
            }
            
            // 不在table的operationCode沒有機器碼
            if(!table[operationCode[j].c_str()].first.compare("NULL")){
                obj[j] = "";
                
            //若 symbolTable.find(operand[j].c_str()) 找到了匹配的键，則回傳true
            }else if(symbolTable.find(operand[j].c_str()) != symbolTable.end()){
                obj[j].append(symbolTable[operand[j].c_str()].second,0,4);
//                printf("%-8s %-8s\n",lable[j].c_str(),obj[j].c_str());
            //算位元編碼
            }else if(!operationCode[j].compare("BYTE")){
                obj[j] = "";
                if(operand[j][0]=='C'){
                    for(int i=2;i<operand[j].length()-1;i++){
                        int oper = operand[j][i];
                        str = decimalConvertHexadecimal(oper);
                        while(str[0]=='0'){
                            str.erase(0,1);
                        }
                        obj[j] = obj[j]+str;
                    }
                }else{
                    obj[j] = obj[j].append(operand[j],2,operand[j].length()-3);
                }
            //RSUB：這是一個彙編指令，表示“Return from SUBroutine”，意思是從子程式返回。
            }else if(!operand[j].compare("RSUB")){
                //如果是其中之一，表示找到了跳躍或比較的指令。
                for(i=j;;i--){
                    if(operationCode[i].compare("JSUB") == 0 || operationCode[i].compare("J")==0 || operationCode[i].compare("COMP")==0){
                        break;
                    }
                }
                obj[j].append(location[i].c_str(),0,4);
            }else if(operand[j].find(",X")!=-1){
                int xPosition = operand[j].find(",X");
                temp = operand[j].substr(0,xPosition);
                printf("%s\n",temp.c_str());
                obj[j].append(symbolTable[temp].second,0,4);
                //它的目的是將字串 obj[j] 中的某個字元位置的十六進位值加上 8。*這裡不太確定
                obj[j] = covertHexadecimal(obj[j],8,2);
            }else{
                obj[j].append(location[j].c_str(),0,4);
            }
//           ----------------------------------------
            objLength+=obj[j].length();
            //T卡是一種用於表示程式目標程式碼的文字格式，通常由位址、指令或資料以及其他相關資訊組成。
            if(obj[j].length() == 0){//creat Tcard
                if(objLength>0){
                    Tcard[k] = creatTcard(objLength,j-1,obj,location,start++);
                    start = j+1;
                    k++;
                    objLength=0;
                }else{
                    start++;
                    continue;
                }
            }else if(objLength>=30 || obj[j].length()!=6){
                Tcard[k] = creatTcard(objLength,j,obj,location,start);
//                printf("Tcard %d:%s\n",k,Tcard[k].c_str());
                start = j+1;
                k++;
                objLength=0;
            }
        }
//        printf("%-2d%-8s%-10s%-8s%-8s\n",j,operationCode[j].c_str(),operand[j].c_str(),obj[j].c_str(),location[j].c_str());
    }
//
    for(i=0;i<length;i++){
        cout<<std::left<<setw(3)<<i<<setw(8)<<location[i]<<setw(8)<<lable[i]<<setw(8)<<operationCode[i]<<setw(10)<<operand[i]<<setw(8)<<obj[i]<<endl;
    }
    cout<<Scard<<endl;
    for(i=0;i<k;i++){
        if(!Tcard[i].empty()){
            cout<<std::left<<Tcard[i]<<endl;
        }
    }
    cout<<Ecard<<endl;
    
    for(i=0;i<length;i++){
        list<<std::left<<setw(3)<<i<<setw(8)<<location[i]<<setw(8)<<lable[i]<<setw(8)<<operationCode[i]<<setw(10)<<operand[i]<<setw(8)<<obj[i]<<endl;
    }
    card<<Scard<<endl;
    for(i=0;i<k;i++){
        if(!Tcard[i].empty()){
            card<<std::left<<Tcard[i]<<endl;
        }
    }
    card<<Ecard<<endl;
    
    input.close();
    list.close();
    card.close();
    return 0;
}
