FILE = mytest

all: $(FILE)

$(FILE): $(FILE).cpp
	g++ -Wall $(FILE).cpp solitaire.cpp -o $(FILE)

run: $(FILE)
	./$(FILE)

val:
	valgrind./$(FILE)
