#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

vector<string> getProductions(string prod){
    vector<string> derivations;
    for(int i=0;i<prod.size();i++){
        string dev = "";
        while(prod[i] != ' ' && i < prod.size()){
            dev = dev + prod[i];
            cout << dev << endl;
            i++;
        }
        derivations.push_back(dev);
    }
    return derivations;
}

int main(){
    int cases; cin >> cases;
    while(cases--){
        int gLines; cin >> gLines; //Lineas de gramática
        map<string, vector<string>> gramatica; //Diccionario que va a tener la gramatica.
        cin.ignore();
        while(gLines--){
            string production; getline(cin, production);
            string head = production.substr(0,1);
            gramatica[head] = getProductions(production.substr(2));
        }
    }
    return 0;
}