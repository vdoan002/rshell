all:
	if[ ! -d "./bin"];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic src/hw0.cpp bin/rshell
rshell:
	if [ ! -d "./bin"];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic src/hwo0.cpp bin/rshell
