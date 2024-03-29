#include<iostream>
using namespace std;
#include<string.h>

/*  P - PREMISE
    * - AND
    + - OR
    ~ - NOT
    > - IMPLIES
    e - ELIMINATION
    i - INTRODUCTION
    MT - MODUS TOLLENS
*/

/**
 * @brief Function to divide input line into 4 parts Proof Line, Proof Line, Line 1,Line 2
 * @param s string to be divided
 * @param delim Delimiter
 * @return string* returns string array
 */
string * divide(string s,char delim){
    string *arr = new string[5];
    int part=0;
    string s1,s2,s3,s4;
    for(char c : s){
        if(c==delim){
            part++;
            continue;
        }
        if(part==0){
            s1+=c;
        }
        else if (part==1){
            s2+=c;
        }
        else if (part==2){
            s3+=c;
        }
        else if (part==3){
            s4+=c;
        }
    }
    arr[0]=s1;arr[1]=s2;
    arr[2]=s3;arr[3]=s4;
    return arr;
}

/// Custom class to map each line 
class map{
    public:
        int lineno;
        string pl,rule,l1,l2;
        map(int i,string a){
            /// @brief Sets Line No,proof line,proof rule
            /// @param i Line no
            /// @param a proof input line

            lineno=i; //Sets Line No
            string *p = divide(a,'/');
            pl=p[0]; // Sets proof line
            rule=p[1]; // Sets proof rule
            l1=p[2];l2=p[3]; // Stores line nos used for proof line
        }
};
/// Custom Stack class 
class stack{
    public:
        int top,size;
        char *arr=NULL;
        /// @brief Constructor
        /// @param cap cap- max capacity of stack
        stack(int cap){
            size = cap; 
            top=-1;
            arr =new char[size];
        }
        
        bool isEmpty(){
            /// To check if stack is empty 
            if (top<=-1){
                return 1;
            }
            else{
                return 0;
            }
        }
        bool isFull(){
            /// To check if stack is Full 
            if (top>=size-1){
                return 1;
            }
            else{
                return 0;
            }
        }
        void push(char item){
            /// Pushing a new item into the stack 
            if (isFull()){
                /// Checking for overflow condition
                cout<<"Overflow: Cant push";
            }
            else{
                top+=1;
                arr[top] = item;
            }
        }
        void pop(){
            /// Removing item at top 
            if (isEmpty()){
                /// Checking for underflow condition
                cout<<"Underflow: Cant pop";
            }
            else{
                //cout<<arr[top];
                top-=1;
            }
        }
        char peek(){
            /// Function to return topmost element of stack 
            if (!isEmpty()){
                return arr[top];}
            else{return '\0';}
        }
};
/**
 * @brief  Divide the proof line into 2 parts operand before operator and operand after operator
 * @param ptr 
 * @param str 
 * @param delim Delimiter
 * @return string* 
 */
string *getElements(stack *ptr,string str,char delim){
    string *arr = new string[2];
    string a1,a2;
    int pt=0,j=0;
    for(int i=1;i<str.length()-1;i++){
        if(str[i]=='('){
            ptr->push('(');
        }
        if((ptr->isEmpty()) && (j!=0)){
            pt++;
            j=0;
            continue;
        }
        if(str[i]==')'){
            ptr->pop();
        }
        if(pt==0){
            a1+=str[i];
        }
        if(pt==1){
            a2+=str[i];
        }
        j++;
    }
    arr[0]=a1;
    arr[1]=a2;
    return arr;
}

int main(){
    cout<<"Enter no of lines in proof"<<endl;
    int n; 
    map *maparr[100];
    cin>>n;
    cout<<"Enter proof"<<endl;
    for (int i=0;i<n;i++){
        string p;
        cout<<i+1<<" ";
        cin>>p;
        /// @brief Creating a map pointer & an array of map type objects
        /// @return 
        map *m = new map(i+1,p); // Creating a map pointer
        maparr[i]=m; // Creating an array of map type objects
    }
    stack *st = new stack(100);
    for (int i=0;i<n;i++){
         /// @brief PREMISE
         /// @return 
        if(((maparr[i]->rule).compare("P"))==0){
            continue;
        }
         /// @brief AND INTRODUCTION
         /// @return 
         
        else if(((maparr[i]->rule).compare("*i"))==0){
            string *m = getElements(st,maparr[i]->pl,'*');
            int line1=stoi(maparr[i]->l1);
            int line2=stoi(maparr[i]->l2);
            if(!m[0].compare(maparr[line1-1]->pl)==0 || !m[1].compare(maparr[line2-1]->pl)==0){
                cout<<endl<<"Invalid proof at line "<<i+1<<endl;
                return 0;
            }
        }
        /// @brief OR INTRODUCTION
        /// @return 
        else if((((maparr[i]->rule).compare("+i1"))==0) || (((maparr[i]->rule).compare("+i2"))==0)){
            string *m = getElements(st,maparr[i]->pl,'+');
            int line1=stoi(maparr[i]->l1);
            if(((maparr[i]->rule).compare("+i1")==0)){
                if(!m[0].compare(maparr[line1-1]->pl)==0){
                    cout<<endl<<"Invalid proof at line "<<i+1<<endl;
                    return 0;
                }
            }
            if(((maparr[i]->rule).compare("+i2")==0)){
                if(!m[1].compare(maparr[line1-1]->pl)==0){
                    cout<<endl<<"Invalid proof at line "<<i+1<<endl;
                    return 0;
                }
            }
        }
        /// @brief AND ELIMINATION
        /// @return 
        else if(((maparr[i]->rule).compare("*e1")==0)){
            int line1=stoi(maparr[i]->l1);
            string *m = getElements(st,maparr[line1-1]->pl,'*');
            if(!(m[0].compare(maparr[i]->pl)==0)){
                cout<<endl<<"Invalid proof at line "<<i+1<<endl;
                return 0;
            }
        }
        else if(((maparr[i]->rule).compare("*e2")==0)){
            int line1=stoi(maparr[i]->l1);
            string *m = getElements(st,maparr[line1-1]->pl,'*');
            if(!(m[1].compare(maparr[i]->pl)==0)){
                cout<<endl<<"Invalid proof at line "<<i+1<<endl;
                return 0;
            }
        }
        /// @brief IMPLICATION ELIMINATION
        /// @return  
        else if(((maparr[i]->rule).compare(">e")==0)){
            int line1=stoi(maparr[i]->l1);
            int line2=stoi(maparr[i]->l2);
            string *m = getElements(st,maparr[line2-1]->pl,'>');
            //cout<<m[0]<<" "<<m[1]<<maparr[line1-1]->pl<<endl;
            if(((!(maparr[i]->pl).compare(m[1]))==0) || ((!(maparr[line1-1]->pl).compare(m[0]))==0)){
                cout<<endl<<"Invalid proof at line "<<i+1<<endl;
                return 0;
            }
        }
        /// @brief MODUS TOLLENS
        /// @return  
        else if(((maparr[i]->rule).compare("MT")==0)){
            int line1=stoi(maparr[i]->l1);
            int line2=stoi(maparr[i]->l2);
            string *m = getElements(st,maparr[line1-1]->pl,'>');
            if(((!(maparr[i]->pl).compare("(~"+m[0]+")"))==0) || ((!(maparr[line2-1]->pl).compare("(~"+m[1]+")"))==0)){
                cout<<endl<<"Invalid proof at line "<<i+1<<endl;
                return 0;
            }
        }
        else{
            cout<<endl<<"Invalid proof rule used"<<endl;
            return 0;
        }
    }
    cout<<endl<<"Valid Proof"<<endl;
    return 0;
}