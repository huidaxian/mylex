#include "generate.h"

using namespace std;
void generate(dfa& d) {
    cout << R"(
#include <iostream>
#include <string>
#include <unordered_map>
#include "yy.tab.h"
using namespace std;
string yytext;
unsigned int yylength;
FILE* yyin = stdin;
//errno_t err = fopen_s(&yyin, "./test.txt", "r");
FILE* yyout = stdout;

)";
    vector<vector<int>>& table = d.transitionTable;
    int row = table.size();
    string transTable = "const int  transitionTable[" + to_string(row) + "]" + "[128] ={";
    for (int i = 0;i < row;i++) {
        transTable += "\n\t";
        for (int j : table[i]) {
            transTable += to_string(j) + ",";
        }
    }
    transTable.pop_back();
    transTable += "};\n";
    cout << transTable;

    auto& actionTable = d.actionTable;
    for (auto& kv : actionTable) {
        string func = "int accept" + to_string(kv.first) + "(){" + kv.second+"\n return 0;}\n";
        cout << func;
    }
    cout << R"(
int acceptDefault() {
cout << yytext << ":unknown" << endl;
return 0;
 }
)";
    cout << "unordered_map<int, int(*)()> actionMap = {\n";
    string actMap;
    for (auto& kv : d.actionTable) {
        actMap+="\n\t{" + to_string(kv.first) + ",accept" + to_string(kv.first) + "},";
    }
    actMap.pop_back();
    actMap += "\n};\n";
    cout << actMap;

    cout << "int yylex() {\n\t";
    cout << "int state = " + to_string(d.start) + ";";
    cout << R"(
    int nextChar;
    yytext = "";
    yylength = 0;
    int lastAccept = -1;
    unsigned int lastLength = 0;

    while (true) {
        if (stdin == yyin)
            nextChar = cin.get();
        else
            nextChar = fgetc(yyin);
        if (nextChar == EOF)
            if (0 == yylength)
                return -1;
            else
                state = -1;
        else
        {
            state = transitionTable[state][nextChar];
            yytext+=nextChar;
            yylength++;
            if (actionMap.find(state) != actionMap.end()) {
                lastAccept = state;
                lastLength = yylength;
            }
        }
        if (state == -1) {
            while (yylength > lastLength + (lastAccept == -1)) {
                if (stdin == yyin)
                    cin.unget();
                else
                    fseek(yyin, -1, SEEK_CUR);
                yytext.pop_back();
                yylength--;
            }
            if (lastAccept == -1)
                return acceptDefault();
            else
                return actionMap.at(lastAccept)();
        }
    }
    return 0;
}
)";
    
}