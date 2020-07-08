#include "Lexical/lexical.h"
#include "GrammarAnalyzer/Grammar.h"
#include "GrammarAction/GrammarAction.h"
#include <bits/stdc++.h>
static const string source=R"(..\Files\source6.txt)";
static const string middle=R"(..\Files\middle.txt)";
using namespace std;
int main() {
   lexical lex(source);
   GrammarAction action(middle);
   Grammar grammar(lex,action);
   grammar.analyzer();
   return 0;
}