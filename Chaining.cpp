#include "Chaining.h"
using namespace std;

void Merge(vector<int> &arr, int start, int end){
    int a=start;
    int mid = (start + (end-start)/2);
    int b=mid; 
    int insertat=0;
    int thing[end-start];
    while(a!=(mid) && b!=(end)){
        if(arr[a]<=arr[b]){
            thing[insertat]=arr[a];
            a++; insertat++;
        }
        else{
            thing[insertat]=arr[b];
            b++; insertat++;
        }
    }
    if(a!=mid){
        for(int i=a; i<mid; i++){
            thing[insertat]=arr[a];
            a++; insertat++;
        }
    }
    else if(b!=end){
        for(int i=b; i<end; i++){
            thing[insertat]=arr[b];
            b++; insertat++;
        }
    }
    for(int i=start; i<end; i++){
        arr[i]=thing[i-start];
    }
}

void MergeSort(vector<int> &arr,int start, int end){
    if((end-start)!=1){
        MergeSort(arr,start,(start + (end-start)/2));
        MergeSort(arr,(start + (end-start)/2),end);
        Merge(arr,start,end);
    }
}

Chaining::Chaining(){
    p[0]=1;
    for(int i=1; i<21; i++){
        p[i]=((p[i-1]*31)%capacity);
    }
    bankStorage2d.resize(capacity);
}

int Chaining::hash(std::string id) {
    int ans = 0;
    for(int i=0; i<4; i++){
        ans+=((id[i]-65)*p[i]);
        ans = ans%capacity;
    }
    for(int i=4; i<11; i++){
        ans+=((id[i]-48)*p[i]);
        ans = ans%capacity;
    }
    for(int i=12; i<22; i++){
        ans+=((id[i]-48)*p[i-1]);
        ans = ans%capacity;
    }
    return ans; 
}

void Chaining::createAccount(std::string id, int count) {
    if(BaseSize>=(0.6 * capacity)){
        capacity=capacity*2;
        for(int i=1; i<21; i++){
            p[i]=((p[i-1]*31)%capacity);
        }
        vector<vector<Account>> tempHash;
        tempHash.resize(capacity);
        for(int i=0; i<bankStorage2d.size(); i++){
            if(bankStorage2d[i].size()>0){
                for(Account x: bankStorage2d[i]){
                    tempHash[hash(x.id)].push_back(x);
                }
            }
        }
        bankStorage2d = tempHash;
    }
    Account temp = {id,count};
    int hval = hash(id);
    bankStorage2d[hval].push_back(temp);
    BaseSize++;
}

std::vector<int> Chaining::getTopK(int k) {
    vector<int> Sortcheck;
    vector<int> ans;
    for(int i=0; i<capacity; i++){
        if(bankStorage2d[i].size()!=0){
            for(Account x: bankStorage2d[i]){
                Sortcheck.push_back(x.balance);
            }
        }
    }
    if(BaseSize==0){vector<int> v={}; return v;}
    MergeSort(Sortcheck,0,Sortcheck.size());
    if(Sortcheck.size()<=k){
        for(int i=(Sortcheck.size()-1); i>=0; i-- ){
            ans.push_back(Sortcheck[i]);
        }
    }
    else for(int i=(Sortcheck.size()-1); i>=(Sortcheck.size()-k); i--){
        ans.push_back(Sortcheck[i]);
    }
    return ans;
}

int Chaining::getBalance(std::string id) {
    int hval = hash(id);
    if(bankStorage2d[hval].size()!=0){
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].id==id){return bankStorage2d[hval][i].balance;}
        }
    }
    return -1;
}

void Chaining::addTransaction(std::string id, int count) {
    int hval = hash(id);
    if(bankStorage2d[hval].size()==0){
        createAccount(id,count);
    }
    else{
        int c = 0;
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].id==id){
                bankStorage2d[hval][i].balance+=count;
                break;
            }
            else{c++;}
        }
        if(c==bankStorage2d[hval].size()){
            createAccount(id,count);
        } 
    }
}

bool Chaining::doesExist(std::string id) {
    int hval = hash(id);
    if(bankStorage2d[hval].size()!=0){
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].id==id){return true;}
        }
    }
    return false; 
}

bool Chaining::deleteAccount(std::string id) {
    int hval = hash(id);
    if(bankStorage2d[hval].size()!=0){
        for(int i=0; i<bankStorage2d[hval].size(); i++){
            if(bankStorage2d[hval][i].id==id){
                bankStorage2d[hval].erase(bankStorage2d[hval].begin()+i); 
                BaseSize--;
                return true;
            }
        }
    }
    return false;
}
int Chaining::databaseSize() {
    return BaseSize;
}

