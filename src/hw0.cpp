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

//bool exit(char argv)
//{
//	string temp = "exit";

//	char* temp2[5];	

//	strcpy(temp2[0], temp.c_str());

//	if(argv == temp2) return true;
	
//	return false;
//}
void test(vector<string> cmd, vector<int> location, char** argv)
{
/*	if(type == 2){
		re_file = open(addr, O_APPEND | O_RDWR, S_IREAD | S_IWRITE);
		type--;
	}
	else if(type == 1)
		re_file = open(addr, O_TRUNC | O_RDWR, S_IREAD | S_IWRITE);
	else re_file = open(addr, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
	old_stdio = dup(type);
	dup2(re_file, type);
	close(re_file);
*/			
	int pid = fork();
	
	if(pid == -1) perror("forking error");
	else if(pid == 0){
	for(unsigned i = 0; i < location.size(); ++i)
	{
		if (cmd.at(location.at(i)) == "<"){
			int new_stdout = dup(1);
			close(1);
			const char* temp = cmd.at(location.at(i)+1).c_str();
			int fd0 = open(temp, O_RDONLY);
			dup2(fd0, STDIN_FILENO);
			close(fd0);
//			in = 0;
		}	

		//if(out)(
		//	
		//	int fd1 = creat(output, 0644);
		//	dup2(fd1, STDOUT_FILENO);
		//	close(fd1);
		//	out = 0;
			
		//}
		
		execvp(argv[0], argv);
		perror("execvp error");
		exit(1);
	}
	}	
	else{
		waitpid(pid, 0, 0);
//		free(res);
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

		char** argv = new char*[parsed.size()];
	//fills argv with correct sizes	
		for(unsigned int i = 0; i < parsed.size(); ++i)
		{
			argv[i] = new char[parsed.at(i).size()];
	//		argv[i] = NULL;
		}

		for(unsigned int i = 0; i < parsed.size(); ++i)
		{
			strcpy(argv[i], parsed.at(i).c_str());
			if(parsed.at(i) == ">" || parsed.at(i) == "<")
				redirec_location.push_back(i);
		}

		argv[parsed.size()] = NULL;
		
		if(redirec_location.size() !=  0)
		{	
			test(parsed,redirec_location,argv);
		}
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
			cout << parsed.at(i) << endl;
		}
		
			execute(commandstr, parsed);
	}
	return 0;
}
	
