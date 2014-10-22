#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <cstdlib>
#include <stdio.h>
#include <sys/wait.h>
#include <vector>

using namespace std;

//bool exit(char argv)
//{
//	string temp = "exit";

//	char* temp2[5];	

//	strcpy(temp2[0], temp.c_str());

//	if(argv == temp2) return true;
	
//	return false;
//}
		

void execute(char x[], vector<string> &parsed)
{


	


	string temp = x;

	//deals with connectors
	
	pid_t childpid;

	childpid = fork();

	if(childpid == 0){

		char** argv = new char*[parsed.size()];
		
		for(unsigned int i = 0; i < parsed.size(); ++i)
		{
			argv[i] = new char[parsed.at(i).size()];
	//		argv[i] = NULL;
		}

		for(unsigned int i = 0; i < parsed.size(); ++i)
		{
			strcpy(argv[i], parsed.at(i).c_str());
		}

	//	for(unsigned int i = 0; i < parsed.size(); ++i)
	//	{
	//		if(argv[i] == "") argv[i] = NULL;
	//	}
		argv[parsed.size()] = NULL;
		


		int r = execvp(argv[0], argv);

		if(r == -1)
		{
			perror("Error");
			exit(1);
		}
		exit(0);
	} 

	else if(childpid == -1)
	{
		perror("Broke while forking");
		exit(1);
	}
	
	else{
		wait(0);
		return;
	}

}

int main()
{
	//Username stuff
	string UserName;
	char HostName[1024];
	UserName = getlogin();
        gethostname(HostName,1024);


	string command;
	char* commandstr = new char[command.size()];

	strcpy(commandstr, command.c_str());

	
	while(true){	
		cout << UserName << "@" << HostName << " $ ";
		getline(cin,command);

		if(command == "exit")
		{
			exit(0);
		}
		
		vector<string> parsed;	
	
		if(command.find('#') != string::npos)
		{
			command = command.substr(0, command.find('#'));
		}

		unsigned i = 0;
	
		char **argv = new char*[command.size()];

		while(i != command.size())
		{
			argv[i] = NULL;
			++i;
		}

		char *temp = new char[command.length() + 1];
		strcpy(temp, command.c_str());

		char *p = strtok(temp, " ");

		while(p != 0)
		{
			parsed.push_back(p);
			p = strtok(NULL," ");
		}
		
			execute(commandstr, parsed);
	}
	return 0;
}
	
