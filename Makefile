CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11

battle: battle.o
	${CXX} -o $@ $^

stat:   stat.o
		${CXX} -o $@ $^

catch:  catch.o
		${CXX} -o $@ $^

%.o: %.cpp $(shell echo *.h)
	${CXX} ${CXXFLAGS} -c $<
