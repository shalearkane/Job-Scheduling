output: ga_main.o ga_functions.o ga_heft.o
	g++ ga_main.o ga_functions.o ga_heft.o -o ga.out

main.o: ga_main.cpp
	g++ -c ga_main.cpp

ga_functions.o: ga_functions.cpp
	g++ -c ga_functions.cpp

ga_heft.o: ga_heft.cpp
	g++ -c ga_heft.cpp

clean:
	rm *.o output