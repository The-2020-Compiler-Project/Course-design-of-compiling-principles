#include "Lexical/lexical.h"
#include "GrammarAnalyzer/Grammar.h"
#include "GrammarAction/GrammarAction.h"
#include "Optimizer/Optimizer.h"
#include "TargetCodeGenerator/TargetCodeGenerator.h"
#include <bits/stdc++.h>
static const string source=R"(..\Files\digui.txt)";
static const string tokenOutput=R"(..\Files\tokenOutput.txt)";
static const string middle=R"(..\Files\middle.txt)";
static const string tokenFile=R"(..\Files\tokenOutput.txt)";
static const string exMiddle=R"(..\Files\exMiddle.txt)";

using namespace std;
int main() {
    lexical lex(source,tokenFile);
    GrammarAction action(middle);
    Grammar grammar(lex, action);

    grammar.analyzer();
    ifstream in;
    in.open(middle);
    Optimizer optimizer(exMiddle);
    optimizer.optimize(in);
    in.close();

    TargetCodeGenerator tarCode;
    tarCode.read(exMiddle);
    tarCode.spiltMidCode();
    tarCode.generateCode();

    return 0;
}