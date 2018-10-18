#include<iostream>
#include<set>
#include<list>
#include<algorithm>

using namespace std;


string NUMFirstRE[] = {"1", "2", "3", "4", "5", "6", "7"
        , "8", "9"};

string NUMRE[] = {"0" ,"1", "2", "3", "4", "5", "6", "7"
        , "8", "9"};

set<string> NUMFirstRESet (NUMFirstRE, NUMFirstRE+9);
set<string> NUMRESet (NUMRE, NUMRE+10);

string unexpectedToken;

string expr(list<string>& tokenStr, list<string>::iterator& currentIt);
string expr_tail(list<string>& tokenStr, list<string>::iterator& currentIt);
string term(list<string>& tokenStr, list<string>::iterator& currentIt);
string term_tail(list<string>& tokenStr, list<string>::iterator& currentIt);
string factor(list<string>& tokenStr, list<string>::iterator& currentIt);

list<string> SliceAdj(string str, list<string>& subString)
{
    list<string> ans;
    for(int i=0;i<str.length();i++)
    {
        if(str.substr(i, 1).compare("\(")==0){
            ans.push_back("LPR");
            subString.push_back("\(");
        }
        else if(str.substr(i, 1).compare("\)")==0){
            ans.push_back("RPR");
            subString.push_back("\)");
        }
        else if(str.substr(i, 1).compare("\/")==0){
            ans.push_back("DIV");
            subString.push_back("/");
        }
        else if(str.substr(i, 1).compare("\*")==0){
            ans.push_back("MUL");
            subString.push_back("*");
        }
        else if(str.substr(i, 1).compare("-")==0){
            ans.push_back("MINUS");
            subString.push_back("-");
        }
        else if(str.substr(i, 1).compare("+")==0){
            ans.push_back("PLUS");
            subString.push_back("+");
        }
        else if(str.substr(i, 1).compare("0")==0){
            ans.push_back("NUM");
            subString.push_back("0");
        }
        else if(NUMFirstRESet.count(str.substr(i, 1))!=0){
            int tempStr=i;
            if((i+1) < str.length())
            {
                i++;
            }
            while(NUMRESet.count(str.substr(i, 1))!=0)
            {
                i++;
                if(i>=str.length())
                    break;
            }
            //cout<<"tempStr: "<<tempStr<<endl;
            //cout<<"i: "<<i;
            ans.push_back("NUM");
            subString.push_back(str.substr(tempStr, i-tempStr));
            i--;
        }
        else
        {
            subString.push_back(str.substr(i, 1));
            ans.push_back("ERROR");
        }
    }
    return ans;
}

string factor(list<string>& tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"factor\n";
    string ans="PASS";
    if(currentIt==tokenStr.end())
        return "ERROR";
    if((*currentIt).compare("LPR")==0)
    {
        currentIt++;
        ans=expr(tokenStr, currentIt);
        if(ans.compare("ERROR")==0) return "ERROR";
        if((*currentIt).compare("RPR")==0)
        {
            currentIt++;
            ans="PASS";
        }
        else
            {return "ERROR"; unexpectedToken=*currentIt;}
    }
    else if((*currentIt).compare("PLUS")==0)
    {
        currentIt++;
        if((*currentIt).compare("NUM")==0)
        {
            currentIt++;
            ans="PASS";
        }
        else {ans="ERROR"; unexpectedToken=*currentIt;}
    }
    else if((*currentIt).compare("MINUS")==0)
    {
        currentIt++;
        if((*currentIt).compare("NUM")==0)
        {
            currentIt++;
            ans="PASS";
            //cout<<"*currentIt: "<<*currentIt;
        }
        else {ans="ERROR";unexpectedToken=*currentIt;}
    }
    else if((*currentIt).compare("NUM")==0)
    {
        currentIt++;
        ans="PASS";
    }
    else{
        unexpectedToken=*currentIt;
        ans="ERROR";
    }
    //cout<<"ans: "<<ans<<endl;
    //cout<<"*IT: "<<*currentIt<<endl;
    return ans;
}

string term_tail(list<string>& tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"term_tail\n";
    string ans="PASS";
    if(currentIt==tokenStr.end())
        return "ERROR";
    if((*currentIt).compare("MUL")==0)
    {
        currentIt++;
        ans=factor(tokenStr, currentIt);
        if(ans.compare("ERROR")==0) return "ERROR";
        ans=term_tail(tokenStr, currentIt);
    }
    else if((*currentIt).compare("DIV")==0)
    {
        currentIt++;
        ans=factor(tokenStr, currentIt);
        if(ans.compare("ERROR")==0) return "ERROR";
        ans=term_tail(tokenStr, currentIt);
    }
    else if((*currentIt).compare("PLUS")==0
            || (*currentIt).compare("MINUS")==0
            || (*currentIt).compare("EOF")==0
            || (*currentIt).compare("RPR")==0)
    {
        /* lambda*/
    }
    else{
        unexpectedToken=*currentIt;
        ans="ERROR";
    }
    //cout<<"ans: "<<ans<<endl;
    //cout<<"*IT: "<<*currentIt<<endl;
    return ans;
}

string term(list<string>& tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"term\n";
    string ans="PASS";
    if(currentIt==tokenStr.end())
        return "ERROR";
    ans=factor(tokenStr, currentIt);
    if(ans=="ERROR") return "ERROR";
    ans=term_tail(tokenStr, currentIt);
    return ans;
}

string expr_tail(list<string>& tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"expr_tail\n";
    string ans="PASS";
    if(currentIt==tokenStr.end())
        return "ERROR";
    if((*currentIt).compare("PLUS")==0)
    {
        currentIt++;
        ans=term(tokenStr, currentIt);
        if(ans.compare("ERROR")==0) return "ERROR";
        ans=expr_tail(tokenStr, currentIt);
    }
    else if((*currentIt).compare("MINUS")==0)
    {
        currentIt++;
        ans=term(tokenStr, currentIt);
        if(ans.compare("ERROR")==0) return "ERROR";
        ans=expr_tail(tokenStr, currentIt);
    }
    else if((*currentIt).compare("EOF")==0
            || (*currentIt).compare("RPR")==0){/* lambda*/}
    else
    {
        unexpectedToken=*currentIt;
        ans="ERROR";
    }

    //cout<<"ans: "<<ans<<endl;
    //cout<<"*IT: "<<*currentIt<<endl;
    return ans;
}

string expr(list<string>& tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"expr\n";
    string ans="PASS";
    ans=term(tokenStr, currentIt);
    if(ans=="ERROR") return "ERROR";
    ans=expr_tail(tokenStr, currentIt);
    return ans;
}

int main()
{
    unexpectedToken="";
    list<string> tokenList;
    list<string> stringList;
    string str;
    char inputChar;

    //cout<<endl;
    while(cin>>inputChar)
    {
        //cout<<inputChar<<" ";
        str.push_back(inputChar);
    }


    tokenList=SliceAdj(str, stringList);
    // q1 output
    for(list<string>::iterator it=tokenList.begin()
        , its=stringList.begin()
        ;its!=stringList.end()
        ;it++, its++)
    {
        cout<<*it<<" ";
        if(NUMRESet.count( (*its).substr(0, 1) )!=0)
            cout<<*its<<"\n";
        else
            cout<<endl;
    }

    // q2 start
    stringList.push_back("$");
    tokenList.push_back("EOF");   // Add end sign "$"
    list<string>::iterator tokenListIt=tokenList.begin();
    string ans=expr(tokenList, tokenListIt);


    //q4
    bool hasOut=false;

    for(list<string>::iterator it=tokenList.begin()
        , its=stringList.begin()
        ;its!=stringList.end()
        ;it++, its++)
    {
        if((*it).compare("ERROR")==0)
        {
            cout<<"Error: Unrecognized token '"<<*its<<"'."<<endl;
            hasOut=true;
        }
    }
    if(hasOut==false)
    {
        size_t result=str.find("\/0");
        if(result!=string::npos)
        {
            cout<<"Error: Divided by Zero.\n";
            hasOut=true;
        }
    }
    if(hasOut==false)
    {
        if(unexpectedToken.length()!=0)
        {
            if(unexpectedToken=="PLUS") unexpectedToken="+";
            if(unexpectedToken=="MINUS") unexpectedToken="-";
            if(unexpectedToken=="MUL") unexpectedToken="*";
            if(unexpectedToken=="DIV") unexpectedToken="\/";
            if(unexpectedToken=="LPR") unexpectedToken="\(";
            if(unexpectedToken=="RPR") unexpectedToken="\)";
            cout<<"Error: Unexpected token '"<<unexpectedToken
            <<"'.\n";
            hasOut=true;
        }
        else if(ans!="PASS")
        {
            cout<<"Error: Unexpected token '"<<"EOF"
            <<"'.\n";
            hasOut=true;
        }
    }
    if(ans=="PASS"&&hasOut==false)
    {
        cout<<"Correct.";
        hasOut=true;
    }
    if(hasOut==false)
    {
        cout<<ans<<endl;
    }


}
