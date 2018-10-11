#include<iostream>
#include<set>

using namespace std;

string idRE[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"
        , "k", "l", "m", "n", "o", "p", "q", "r", "s", "t"
        , "u", "v", "w", "x", "y", "z", "A", "B", "C", "D"
        , "E", "F", "G", "H", "I", "J", "K", "L", "M", "N"
        , "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X"
        , "Y", "Z"};

set<string> idRESet (idRE, idRE+52);

string inumRE[]={"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

set<string> inumRESet (inumRE, inumRE+10);

string Scanner(string str) //returns a token
{
    string ans;
    if(idRESet.count(str.substr(0, 1))!=0)
        ans="id";
    else if(inumRESet.count(str.substr(0, 1))!=0
            || str.substr(0, 1).compare("-")==0)
        ans="inum";
    else if(str.substr(0, 1).compare("=")==0)
        ans="assign";
    else
        return "ERROR";

    for(int i=1;i<str.length();i++)
    {
        if(idRESet.count(str.substr(i, 1))!=0){
            if(ans.compare("id")!=0)
                return "ERROR";
        }
        else if(inumRESet.count(str.substr(i, 1))!=0){
            if(ans.compare("inum")!=0)
                return "ERROR";
        }
        else
            return "ERROR";
    }

    return ans;
}

string Val(string strArray[], int currentIndex, int arrayLen)
{
    string ans="TA is handsome.";
    if(currentIndex>=arrayLen)
        return "ERROR";
    if(strArray[currentIndex].compare("id")==0)
    {
        if(currentIndex+1>=arrayLen)
            return "ERROR";
        if(strArray[currentIndex+1].compare("assign")==0)
        {
            ans=Val(strArray, currentIndex+2, arrayLen);
        }
        else
            return "ERROR";
    }
    else if(strArray[currentIndex].compare("inum")==0)
    {
        if(currentIndex==arrayLen-1)
        {
            return ans;
        }
        else
        {
            return "ERROR";
        }
    }
    else
        return "ERROR";

    return ans;
}

string Procs(string strArray[], int currentIndex, int arrayLen)
{
    string ans;
    ans=Val(strArray, currentIndex, arrayLen);
    return ans;
}

int main()
{
    string tokenStr[1000];
    int tokenStrLen=0;
    string ans;
    string str;
    while(cin>>str)
    {
        string scannerStr=Scanner(str);
        cout<<"A input "<<scannerStr<<"\n";
        if(ans.compare("ERROR")!=0)
        {
            ans=scannerStr;
        }


        tokenStr[tokenStrLen]=scannerStr;
        tokenStrLen=tokenStrLen+1;
    }

    for(int i=0;i<tokenStrLen;i++)
    {
        cout<<tokenStr[i]<<" ";
    }
    cout<<"\ntokenStrLen: "<<tokenStrLen;

    if(ans.compare("ERROR")==0)
    {
        cout<<"\n"<<"Token mismatch.";
    }

    //parse
    if(ans.compare("ERROR")!=0)
    {
        ans=Procs(tokenStr, 0, tokenStrLen);
        if(ans.compare("ERROR")==0)
            ans="Grammar mismatch.";
        cout<<"\n"<<ans<<"\n";
    }

}


