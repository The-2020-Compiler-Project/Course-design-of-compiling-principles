cmake_minimum_required(VERSION 3.14)
project(complier)

set(CMAKE_CXX_STANDARD 11)

add_executable(complier main.cpp tokenType/tokenType.h STable/STable.cpp STable/STable.h
        STable/Sheet.cpp STable/Sheet.h
        XTable/FunSheet.cpp XTable/FunSheet.h XTable/TypeSheet.cpp
        XTable/TypeSheet.h type/type.h type/cat.h XTable/ElemSheet.cpp
        XTable/ElemSheet.h type/quatName.h GrammarAction/GrammarAction.cpp
        GrammarAction/GrammarAction.h GrammarAnalyzer/Grammar.cpp GrammarAnalyzer/Grammar.h
        Lexical/lexical.cpp  Lexical/lexical.h Lexical/lexical.cpp
        Optimizer/Optimizer.cpp Optimizer/Optimizer.h
        TargetCodeGenerator/StackManager.h TargetCodeGenerator/TargetCodeGenerator.cpp TargetCodeGenerator/TargetCodeGenerator.h TargetCodeGenerator/x86Code.h)