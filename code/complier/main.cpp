#include "Lexical/lexical.h"
#include "GrammarAnalyzer/Grammar.h"
#include "GrammarAction/GrammarAction.h"
#include <bits/stdc++.h>
static const string source=R"(..\Files\source6.txt)";
static const string middle=R"(..\Files\middle.txt)";
static const string tokenFile=R"(..\Files\tokenOutput.txt)";
using namespace std;
int main() {
    ofstream fout;
    fout.open(tokenFile);
   lexical lex(source);
   GrammarAction action(middle);
   Grammar grammar(lex,action);
   grammar.analyzer();
   return 0;
}