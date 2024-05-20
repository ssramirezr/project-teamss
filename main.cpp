#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

struct First{
    vector<string> firstSet;
    bool hasEpsilon = false;
};

vector<string> getProductions(string prod){
    vector<string> derivations;
    for(int i=0;i<prod.size();i++){
        string dev = "";
        while(prod[i] != ' ' && i < prod.size()){
            dev = dev + prod[i];
            i++;
        }
        derivations.push_back(dev);
    }
    return derivations;
}

bool findInFirst(vector<string> first, string value){
    bool exist = false;
    for(int i=0;i<first.size() && !exist;i++){
        if(first[i] == value){
            exist = true;
        }
    }
    return exist;
}

void mergeFirsts(map<string, First>& firsts, string x, string y){
    for(string terminal: firsts[y].firstSet){
        if(!findInFirst(firsts[x].firstSet, terminal) && terminal != "e"){
            firsts[x].firstSet.push_back(terminal);
        }
    }
}

void calcFirst(map<string, First>& firsts, map<string, vector<string>> grammar, string nonTerminal){
    for(string dev: grammar[nonTerminal]){
        int i = 0;
        string value = dev.substr(i, 1);
        if(dev[i] >= 65 && dev[i] <= 90 ){
            if(firsts[value].firstSet.size() == 0){
                calcFirst(firsts, grammar, value);
            }
            mergeFirsts(firsts, nonTerminal, value);
            i++;
            while(firsts[value].hasEpsilon && i < dev.size()){
                value = dev.substr(i, 1);
                if(dev[i] < 65 || dev[i] > 90){
                    firsts[nonTerminal].firstSet.push_back(value);
                    break;
                }else{
                    if(firsts[value].firstSet.size() == 0){
                        calcFirst(firsts, grammar, value);
                    }
                    mergeFirsts(firsts, nonTerminal, value);
                    i++;
                }
            }
            if(i == dev.size()){
                firsts[nonTerminal].firstSet.push_back("e");
                firsts[nonTerminal].hasEpsilon = true;
            }

        }else{
            firsts[nonTerminal].firstSet.push_back(value);
            if(value == "e") firsts[nonTerminal].hasEpsilon = true;
        }
    }
}

void getFirsts(map<string, First>& firsts, map<string, vector<string>> grammar){
    for(pair<string, vector<string>> rule: grammar){
        if(firsts[rule.first].firstSet.size() == 0){
            calcFirst(firsts, grammar, rule.first);
        }
    }
}

int main(){
    int cases; 
    cin >> cases;
    while(cases--){
        int gLines; 
        cin >> gLines; //Lineas de gramática
        map<string, vector<string>> gramatica; //Diccionario que va a tener la gramatica.
        map<string, First> firsts; //Diccionario que almacenará los firsts.
        map<string, vector<string>> follows; //Diccionario que almacenará los follows.
        cin.ignore();
        while(gLines--){
            string production; getline(cin, production);
            string head = production.substr(0,1);
            gramatica[head] = getProductions(production.substr(2));
            firsts[head]; //Inicialización del mapa de firsts en el no terminal.
            follows[head]; //Inicialización del mapa de follows en el no terminal
        }
        getFirsts(firsts, gramatica);
        for(auto& val: firsts){
            cout << "Head: " << val.first << endl;
            for(auto& t: val.second.firstSet){
                cout << t << endl;
            }
        }
    }
    return 0;
}
