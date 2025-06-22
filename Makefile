CXX = g++
CXXFLAGS = -std=c++11 -Wall 

target: etapa5

etapa5: lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o tacs.o
	$(CXX) lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o tacs.o -o etapa5

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $< -c 

parser.tab.hpp: parser.ypp 
	bison parser.ypp -d

parser.tab.cpp: parser.ypp 
	bison parser.ypp

lex.yy.cpp: scanner.l parser.tab.hpp
	flex -o lex.yy.cpp scanner.l 

clean:
	rm etapa5 lex.yy.cpp parser.tab.cpp parser.tab.hpp *.o
