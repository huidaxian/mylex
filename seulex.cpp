

#include <iostream>
#include "dfa.h"
#include "parseLex.h"
#include "generate.h"
#include <time.h>
using namespace std;


int main(int argc,char* argv[]) 
{
    try {
        clock_t start, end;
        double totaltime;
        start = clock();
        LexFile lf(argv[1]);
        Integratednfa a(lf.regExps);
        dfa d(a);
        d = minimize(d);
        ofstream fout1("./output/yy.tab.h");
        cout.rdbuf(fout1.rdbuf());
        cout << lf.headers;
        ofstream fout2("./output/lex.yy.cpp");
        cout.rdbuf(fout2.rdbuf());
        generate(d);
        cout << lf.userSegment;
        ofstream fout3(stdout);
        cout.rdbuf(fout3.rdbuf());
        end = clock();
        totaltime = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "success!"<< "程序运行时间为：" << totaltime << "秒!" << endl;;
    }
    catch (const char* e) {
        cout << e;
    }
    
    
    return 0;

}

