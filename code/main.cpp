#include "../GrammarAnalyzer/Grammar.h"
#include "../Optimizer/Optimizer.h"
int main(){
    lexical la("../main/test.txt");
    GrammarAction ga("../main/code.txt");
    Grammar gm(la,ga);
    gm.analyzer();
    ifstream in("../main/code.txt");
    Optimizer opt("../main/optimize.txt");
    opt.optimize(in);
    in.close();
    return 0;
}