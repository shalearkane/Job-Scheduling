#include<bits/stdc++.h>
using namespace std;

struct gene
{
    int task;
    int p;
};

int avg_cost(int t, vector<vector<pair<int,int>>>& chromosomes){
    
}

int main(){
    int t,p;cin>>t>>p;


    srand(time(0));
    vector<vector<pair<int,int>>> chromosomes(20);
    for(int j=0;j<20;j++){
        for(int i=0;i<10;i++){
            int a = ( rand() % 10 ) +1;
            int b = ( rand() % 3 ) +1;

            chromosomes[j].push_back({a,b});
        }
    }
    avg_cost(t,chromosomes);
    // problem - all tasks need to be unique

    for(int j=0;j<20;j++){
        for(int i=0;i<10;i++){
            cout<<chromosomes[j][i].first<<" - "<<chromosomes[j][i].second<<"\n";
        }
        cout<<"\n";
    }
    
    return 0;
}