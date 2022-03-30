output: ga_main.o ga_functions.o ga_heft.o
	g++ -g3 ga_main.o ga_functions.o ga_heft.o -o ga.out

main.o: ga_main.cpp
	g++ -c -g3 ga_main.cpp

ga_functions.o: ga_functions.cpp
	g++ -c -g3 ga_functions.cpp

ga_heft.o: ga_heft.cpp
	g++ -c -g3 ga_heft.cpp

clean:
	rm *.o