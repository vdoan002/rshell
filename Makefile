all:
	if [ ! -d "./bin" ];\
	then mkdir bin;\
	fi
	g++ -Wall -Werror -ansi -pedantic src/hw0.cpp -o bin/rshell
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -o bin/ls
	g++ -Wall -Werror -ansi -pedantic src/cp.cpp -o bin/cp
rshell:
	g++ -Wall -Werror -ansi -pedantic src/hw0.cpp -o bin/rshell
ls:
	g++ -Wall -Werror -ansi -pedantic src/ls.cpp -o bin/ls
cp:
	g++ -Wall -Werror -ansi -pedantic src/cp.cpp -o bin/cp
