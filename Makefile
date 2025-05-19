CXX = g++
CXXFLAGS = -std=c++11 -Wall 

target: etapa4

etapa4: lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o
	$(CXX) lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o -o etapa4

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $< -c 

parser.tab.hpp: parser.ypp 
	bison parser.ypp -d

parser.tab.cpp: parser.ypp 
	bison parser.ypp

lex.yy.cpp: scanner.l parser.tab.hpp
	flex -o lex.yy.cpp scanner.l 

clean:
	rm etapa4 lex.yy.cpp parser.tab.cpp parser.tab.hpp *.o
