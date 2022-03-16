#include<bits/stdc++.h>
using namespace std;

struct gene
{
    int task;
    int p;
};



int main(){
    int n;cin>>n;
    srand(time(0));
    vector<vector<pair<int,int>>> chromosomes(n);
    for(int j=0;j<n;j++){
        for(int i=0;i<10;i++){
            int t = ( rand() % 10 ) +1;
            int p = ( rand() % 3 ) +1;
            chromosomes[j][i].first = t;
            chromosomes[j][i].second = p;
        }
    }
    
    for(int j=0;j<n;j++){
        for(int i=0;i<10;i++){
            cout<<chromosomes[j][i].first<<" - "<<chromosomes[j][i].second<<"\n";
        }
        cout<<"\n";
    }
    
    return 0;
}