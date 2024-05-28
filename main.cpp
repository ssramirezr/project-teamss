#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct Properties{
    vector<char> firstSet;
    vector<char> followSet; 
    bool hasEpsilon = false; //Solo aplicable para set de firsts.
};

struct Gramatica{
    map<char, vector<string>> gramatica;
    vector<char> order;
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
Gramatica getGrammar(int gLines){
    Gramatica gramatica;
    while(gLines--){
        string production; getline(cin, production);
        char head = production[0];
        vector<string> prods = getProductions(production.substr(2));
        gramatica.order.push_back(head);
        gramatica.gramatica[head] = prods;
    }
    return gramatica;
}

bool findInFirst(vector<char> first, char terminal){
    bool exist = false;
    for(int i=0;i<first.size() && !exist;i++){
        if(first[i] == terminal){
            exist = true;
        }
    }
    return exist;
}

void mergeFirsts(map<char, Properties>& properties, char head, char value){
    for(char terminal: properties[value].firstSet){
        if(!findInFirst(properties[head].firstSet, terminal) && terminal != 'e'){
            properties[head].firstSet.push_back(terminal);
        }
    }
}

void findFirst(map<char, Properties>& properties, Gramatica gramatica, char nonTerminal){
    vector<char>& first = properties[nonTerminal].firstSet;
    bool hasNext = false;
    for(string dev: gramatica.gramatica[nonTerminal]){
        int i=0;
        char value = dev[i];
        if(value == nonTerminal){
            if(i+1 < dev.size()) hasNext = true;
            continue;   
        }
        if(isupper(value)){
            if(properties[value].firstSet.size() == 0){
                findFirst(properties, gramatica, value);
            }
            mergeFirsts(properties, nonTerminal, value);
            i++;
            while(properties[value].hasEpsilon && i < dev.size()){
                value = dev[i];
                if(!isupper(value)){
                    if(!findInFirst(first, value)) first.push_back(value);
                    break;
                }else{
                    if(properties[value].firstSet.size() == 0){
                        findFirst(properties, gramatica, value);
                    }
                    mergeFirsts(properties, nonTerminal, value);
                    i++;
                }
                if(i == dev.size() && properties[value].hasEpsilon){
                    first.push_back('e');
                    properties[nonTerminal].hasEpsilon = true;
                }
            }
        }else{
            if(!findInFirst(first, value)) first.push_back(value);
            if(value == 'e') properties[nonTerminal].hasEpsilon = true;
        }
    }
    if(properties[nonTerminal].hasEpsilon && hasNext){
        for(string dev: gramatica.gramatica[nonTerminal]){
            if(dev[0] == nonTerminal && dev.size() > 1){
                char next = dev[1];
                if(isupper(next)){
                    if(properties[next].firstSet.size() == 0) findFirst(properties, gramatica, next);
                    mergeFirsts(properties, nonTerminal, next);
                }else{
                    if(!findInFirst(first, next)) first.push_back(next);
                }
            }
        }
    }
}

void getFirsts(map<char, Properties>& properties, Gramatica gramatica){
    for(char head: gramatica.order){
        if(properties[head].firstSet.size() == 0){
            findFirst(properties, gramatica, head);
        }
    }
}

void printFirsts(map<char, Properties> properties, Gramatica gramatica){
    for(const char nonTerminal: gramatica.order){
        cout << "FIRST(" << nonTerminal << ") = {";
        bool first = true;
        for (const char val : properties[nonTerminal].firstSet) {
            if (!first) {
                cout << ", ";
            }
            cout << val;
            first = false;
        }
        cout << "}" << endl;
    }
}


int main(){
    int cases; 
    cin >> cases;
    while(cases--){
        int gLines; 
        cin >> gLines; //Num of grammar lines.
        cin.ignore();
        Gramatica gramatica  = getGrammar(gLines);
        map<char, Properties> properties;
        getFirsts(properties, gramatica);
        printFirsts(properties, gramatica);
    }
    return 0;
}