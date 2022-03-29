output: ga_2.o ga_functions.o
	g++ ga_2.o ga_functions.o -o ga.out

main.o: ga_2.cpp
	g++ -c ga_2.cpp

ga_functions.o: ga_functions.cpp
	g++ -c ga_functions.cpp

clean:
	rm *.o output