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
                int i = 1;
                char next = dev[i];
                if(isupper(next)){
                    if(properties[next].firstSet.size() == 0) findFirst(properties, gramatica, next);
                    mergeFirsts(properties, nonTerminal, next);
                    while(properties[next].hasEpsilon && i < dev.size()){
                        i++;
                        next = dev[i];
                        if(isupper(next)){
                            if(properties[next].firstSet.size() == 0) findFirst(properties, gramatica, next);
                            mergeFirsts(properties, nonTerminal, next);
                        }else{
                            if(!findInFirst(first, next)) first.push_back(next);
                            break;
                        }
                    }
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

int findInProduction(string production, char value){
    for(int i=0;i<production.size();i++){
        if(production[i] == value) return i;
    }
    return -1;
}

void addCharToFollow(vector<char>& follow, char terminal){
    for(char val: follow){
        if(val == terminal) return;
    }
    follow.push_back(terminal);
}

void addFirstToFollow(map<char, Properties>& properties, char nonTerminal, char head){
    for(char terminal: properties[head].firstSet){
        if(terminal != 'e') addCharToFollow(properties[nonTerminal].followSet, terminal);
    }
}

void mergeFollows(vector<char>& followB, vector<char> followA){
    for(char inFollowA: followA){
        bool found = false;
        for(char inFollowB: followB){
            if(inFollowA == inFollowB){
                found = true;
                break;
            }
        }
        if(!found) followB.push_back(inFollowA);
    }
}

void findFollow(map<char, Properties>& properties, Gramatica gramatica, char nonTerminal){
    vector<char>& follow = properties[nonTerminal].followSet;
    for(char head: gramatica.order){
        for(string dev: gramatica.gramatica[head]){
            int pos = findInProduction(dev, nonTerminal);
            if(pos >= 0){
                if(pos+1 < dev.size()){
                    char val = dev[pos+1];
                    if(isupper(val)){
                        addFirstToFollow(properties, nonTerminal, val);
                        int i = 2;
                        while(properties[val].hasEpsilon && pos+i < dev.size()){
                            val = dev[pos+i];
                            i++;
                            if(isupper(val)){
                                addFirstToFollow(properties, nonTerminal, val);
                            }else{
                                addCharToFollow(follow, val);
                                break;
                            }
                        }
                        if(pos+i == dev.size() && properties[val].hasEpsilon){
                            if(properties[val].followSet.size() == 0){
                                findFollow(properties, gramatica, head);
                            }
                            mergeFollows(follow, properties[head].followSet);
                        }
                    }else{
                        addCharToFollow(follow, val);
                    }
                }else{
                    if(properties[head].followSet.size() == 0){
                        findFollow(properties, gramatica, head);
                    }
                    mergeFollows(follow, properties[head].followSet);
                }
            }
        }
    }
}

void getFollows(map<char, Properties>& properties, Gramatica gramatica){
    properties['S'].followSet.push_back('$');
    findFollow(properties, gramatica, 'S');
    for(char head: gramatica.order){
        if(properties[head].followSet.size() == 0){
            findFollow(properties, gramatica, head);
        }
    }
}

void printFirsts(map<char, Properties> properties, Gramatica gramatica){
    for(const char nonTerminal: gramatica.order){
        cout << "First(" << nonTerminal << ") = {";
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

void printFollows(map<char, Properties> properties, Gramatica gramatica){
    for(const char nonTerminal: gramatica.order){
        cout << "Follow(" << nonTerminal << ") = {";
        bool first = true;
        for (const char val : properties[nonTerminal].followSet) {
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
        getFollows(properties, gramatica);
        printFirsts(properties, gramatica);
        printFollows(properties, gramatica);
    }
    return 0;
}