#include<iostream>
#include<set>
#include<list>
#include<algorithm>

using namespace std;

string IDFirstRE[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"
        , "k", "l", "m", "n", "o", "p", "q", "r", "s", "t"
        , "u", "v", "w", "x", "y", "z", "A", "B", "C", "D"
        , "E", "F", "G", "H", "I", "J", "K", "L", "M", "N"
        , "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X"
        , "Y", "Z", "_"};

string IDRE[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"
        , "k", "l", "m", "n", "o", "p", "q", "r", "s", "t"
        , "u", "v", "w", "x", "y", "z", "A", "B", "C", "D"
        , "E", "F", "G", "H", "I", "J", "K", "L", "M", "N"
        , "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X"
        , "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7"
        , "8", "9", "_"};

set<string> IDFirstRESet (IDFirstRE, IDFirstRE+53);
set<string> IDRESet (IDRE, IDRE+63);

string exp(list<string> tokenStr, list<string>::iterator& currentIt);

list<string> SliceAdj(string str, list<string>& subString)
{
    list<string> ans;
    for(int i=0;i<str.length();i++)
    {
        if(str.substr(i, 1).compare("\(")==0){
            ans.push_back("LBR");
            subString.push_back("\(");
        }
        else if(str.substr(i, 1).compare("\)")==0){
            ans.push_back("RBR");
            subString.push_back("\)");
        }
        else if(str.substr(i, 1).compare("\.")==0){
            ans.push_back("DOT");
            subString.push_back("\.");
        }
        else if(str.substr(i, 1).compare(";")==0){
            ans.push_back("SEMICOLON");
            subString.push_back(";");
        }
        else if(IDFirstRESet.count(str.substr(i, 1))!=0){
            int tempStr=i;
            if((i+1) < str.length())
            {
                i++;
            }
            while(IDRESet.count(str.substr(i, 1))!=0)
            {
                i++;
                if(i>=str.length())
                    break;
            }
            ans.push_back("ID");
            subString.push_back(str.substr(tempStr, i-tempStr));
            i--;
        }
        //can use string.find() will be more easier
        else if(str.substr(i, 1).compare("\"")==0){
            int tempStr=i;  //first " appear index
            size_t found=-1;

            if(i+1!=str.length())
                found=str.find("\"", i+1, 1);
            tempStr=(int)found;

            if(found==string::npos || i==str.length()-1)
            {
                subString.push_back("ERROR");
                ans.push_back("ERROR");
            }
            else
            {
                subString.push_back(str.substr(i, tempStr-i+1));
                ans.push_back("STRLIT");
                i=(int)found;
            }
        }
        else
        {
            subString.push_back("ERROR");
            ans.push_back("ERROR");
        }
    }
    return ans;
}

list<string> Slice(string str, list<string>& subString)
{
    list<string> ans;
    for(int i=0;i<str.length();i++)
    {
        if(str.substr(i, 1).compare("\(")==0){
            ans.push_back("LBR");
            subString.push_back("\(");
        }
        else if(str.substr(i, 1).compare("\)")==0){
            ans.push_back("RBR");
            subString.push_back("\)");
        }
        else if(str.substr(i, 1).compare("\.")==0){
            ans.push_back("DOT");
            subString.push_back("\.");
        }
        else if(str.substr(i, 1).compare(";")==0){
            ans.push_back("SEMICOLON");
            subString.push_back(";");
        }
        else if(IDFirstRESet.count(str.substr(i, 1))!=0){
            int tempStr=i;
            if((i+1) < str.length())
            {
                i++;
            }
            while(IDRESet.count(str.substr(i, 1))!=0)
            {
                i++;
                if(i>=str.length())
                    break;
            }
            ans.push_back("ID");
            subString.push_back(str.substr(tempStr, i-tempStr));
            i--;
        }
        //can use string.find() will be more easier
        else if(str.substr(i, 1).compare("\"")==0){
            int tempStr=i;
            if((i+1) < str.length())
            {
                i++;
            }
            while(str.substr(i, 1).compare("\"")!=0){
                i++;
                if(i>=str.length())
                {
                    i--;
                    break;
                }
            }
            if(str.substr(i, 1).compare("\"")!=0
               || tempStr==str.length()-1)
            {
                subString.push_back("ERROR");
                ans.push_back("ERROR");
            }
            else
            {
                subString.push_back(str.substr(tempStr, i-tempStr+1));
                ans.push_back("STRLIT");
            }
        }
        else
        {
            subString.push_back("ERROR");
            ans.push_back("ERROR");
        }
    }
    return ans;
}

string primary_tail(list<string> tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"\nPrimary_tail"<<*currentIt;
    string ans="PASS";
    if(currentIt==tokenStr.end())
        return "ERROR";
    if((*currentIt).compare("DOT")==0)
    {
        currentIt++;
        if((*currentIt).compare("ID")==0)
        {
            currentIt++;
            ans=primary_tail(tokenStr, currentIt);
        }
        else
            return "ERROR";
    }
    else if((*currentIt).compare("LBR")==0)
    {
        currentIt++;
        ans=exp(tokenStr, currentIt);
        if((*currentIt).compare("RBR")==0)
        {
            currentIt++;
            ans=primary_tail(tokenStr, currentIt);
        }
        else
            ans="ERROR";
    }
    else if((*currentIt).compare("SEMICOLON")==0){}
    else
        return "ERROR";
    return ans;
}

string primary(list<string> tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"\nPrimary"<<*currentIt;
    string ans="";
    if(currentIt==tokenStr.end())
        return "ERROR";
    if((*currentIt).compare("ID")==0)
    {
        currentIt++;
        ans=primary_tail(tokenStr, currentIt);
    }
    else
        ans="ERROR";
    return ans;
}

string exp(list<string> tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"\nexp "<<*currentIt;
    string ans="PASS";
    if(currentIt==tokenStr.end())
        return "ERROR";
    if((*currentIt).compare("ID")==0)
        ans=primary(tokenStr, currentIt);
    else if(( *currentIt).compare("STRLIT")==0)
    {
        currentIt++;
        ans="PASS";
    }
    else if((*currentIt).compare("SEMICOLON")==0
            ||(*currentIt).compare("RBR")==0)
            {
                /* do nothing for lambda*/
            }
    else
        ans="ERROR";
    return ans;
}

string stmt(list<string> tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"\nStmt "<<*currentIt;
    string ans="";
    if(currentIt==tokenStr.end())
        return "ERROR";
    if((*currentIt).compare("ID")==0 || (*currentIt).compare("STRLIT")==0)
    {
        ans=exp(tokenStr, currentIt);
        if((*currentIt).compare("SEMICOLON")!=0)
            ans="ERROR";
        currentIt++;
    }
    else
        ans="ERROR";
    return ans;
}

string stmts(list<string> tokenStr, list<string>::iterator& currentIt)
{
    //cout<<"\nStmts "<<*currentIt;
    string ans="PASS";
    if(currentIt==tokenStr.end())
        return "ERROR";
    if((*currentIt).compare("ID")==0 || (*currentIt).compare("STRLIT")==0)
    {
        ans=stmt(tokenStr, currentIt);
        if(ans.compare("ERROR")==0) return "ERROR";
        ans=stmts(tokenStr, currentIt);
    }
    else if((*currentIt).compare("$")==0){}
    else
        ans="ERROR";
    return ans;
}

string program(list<string> tokenStr)
{
    //cout<<"\nProgram";
    string ans="PASS";
    list<string>::iterator currentIt=tokenStr.begin();
    ans=stmts(tokenStr, currentIt);
    return ans;
}

int main()
{
    string str;
    list<string> tokenList;
    list<string> stringList;
    string errorStr="ERROR";

    while(cin>>str)
    {
        tokenList=SliceAdj(str, stringList);
        list<string> tokenList2(tokenList);
        stringList.push_back("EOF");
        tokenList.push_back("$");   // Add end sign "$"

        list<string>::iterator findIter=find(tokenList.begin(),
                                            tokenList.end(),
                                            errorStr);

        string ans=program(tokenList);
        //cout<<"\n"<<ans;
        if(findIter==tokenList.end() && ans.compare("ERROR")!=0)
        {
            for(list<string>::iterator it=tokenList2.begin(),
            its=stringList.begin();
            it!=tokenList2.end();
            ++it, ++its)
            {
                cout<<*it<<" "<<*its<<"\n";
            }
        }
        else
        {
            cout<<"invalid input\n";
        }
        stringList.clear();
    }

    /*string ans=program(tokenList);
    cout<<"\n"<<ans;*/
}

/*string Scanner(string str)    //returns a token
{
    string ans;

    if(IDFirstRESet.count(str)!=0)
        ans="ID";
    else if(str.compare("\(")==0)
        ans="LBR";
    else if(str.compare("\)")==0)
        ans="RBR";
    else if(str.compare("\.")==0)
        ans="DOT";
    else if(str.compare(";")==0)
        ans="SEMICOLON";
    else if(str.compare("\"")==0)
        ans="STRLIT";
    else
        ans="ERROR";

    int STRLITCount=0;
    for(int i=1;i<str.length();i++)
    {
        if(STRLITCount>0)
            ans="ERROR";
        if(IDRESet.count(str)!=0){
            if(ans!="ID")
                ans="ERROR";
        }
        else if(str.compare("\"")==0){
            if(ans!="STRLIT")
                ans="ERROR";
            STRLITCount++;
        }
        else
            ans="ERROR";
    }
    return ans;
}*/
