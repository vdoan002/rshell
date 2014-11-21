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
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

void redirection(vector<string> cmd, vector<int> location, char** argv)
{
	int pid = fork();
	
	if(pid == -1) perror("forking error");
	else if(pid == 0){
	for(unsigned int i = 0; i < location.size(); ++i)
	{
		if (cmd.at(location.at(i)) == "<"){
			//error check if there is something actually after!!
			const char* temp = cmd.at(location.at(i) + 1).c_str();
			int fd0 = open(temp, O_RDWR | O_CREAT | O_TRUNC);
			
			if(fd0 == -1)
			{
				perror("failed to open");
				exit(1);
			}
			if(close(0)){
				perror("failed to close");
				exit(1);
			}
			if(dup(fd0)){
				perror("failed to dup");
				exit(1);
			}
		}
			

		else if(cmd.at(location.at(i)) == ">"){
			const char* temp = cmd.at(location.at(i) + 1).c_str();
			int fd = open(temp, O_RDWR | O_CREAT);
			close(1);
			dup(fd);
		}

		char** argv_temp;

	
		execvp(argv[0], argv);
		perror("execvp error");
		exit(1);
		}
	}
		
	else{
		wait(0);
	}
	
}	

void pipe(){
	int pfds[2];
	pipe(pfds);
	if (!fork()) {
		close(1);       /* close normal stdout */
    		dup(pfds[1]);   /* make stdout same as pfds[1] */
    		close(pfds[0]); /* we don't need this */
    		execlp("ls", "ls", NULL);
	} 
	else {
    		close(0);       /* close normal stdin */
    		dup(pfds[0]);   /* make stdin same as pfds[0] */
    		close(pfds[1]); /* we don't need this */
    		execlp("grep", "SOMETHING", NULL);
	}
}

void execute(char x[], vector<string> &parsed)
{
	
	vector<unsigned int> redirec_location;
	string temp = x;

	//deals with connectors
	
	pid_t childpid;

	childpid = fork();

	if(childpid == 0){
		vector<int> redirec_location;


		int argv_size = 0
		char** argv = new char*[parsed.size()];
	//fills argv with correct sizes	
		for(unsigned int i = 0; i < parsed.size(); ++i)
		{
			argv[i] = new char[parsed.at(i).size()];
		}

		for(unsigned int i = 0; i < parsed.size(); ++i)
		{
			cout << "Parsed.at: " << i << " " << parsed.at(i) << endl;
			if(parsed.at(i) == ">" || parsed.at(i) == "<"){
				redirec_location.push_back(i);
				parsed.erase(parsed.begin() + (i));
				--i;
				
			}

			else{
			++argv;
			strcpy(argv[i], parsed.at(i).c_str());
			}
		}

		argv[parsed.size()] = NULL;
		
		if(redirec_location.size() !=  0)
		{	
			redirection(parsed,redirec_location,argv);
		}
		else{
		int r = execvp(argv[0], argv);

		if(r == -1)
		{
			perror("Error");
			exit(1);
		}
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

//void piping(){}
	


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

		for(unsigned i = 0; i < parsed.size(); ++i)
		{
			if(parsed.at(i) == "exit") exit(0);
			cerr << parsed.at(i) << endl; 
		}
		
			execute(commandstr, parsed);
	}
	return 0;
}
	
