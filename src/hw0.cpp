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

unsigned int perror_counter = 0;

void redirection(vector<string> cmd, vector<int> location, char** argv, int argv_size)
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
			if(close(0) == -1){
				perror("failed to close");
				exit(1);
			}
			if(dup(fd0) == -1){
				perror("failed to dup");
				exit(1);
			}
		}
			

		else if(cmd.at(location.at(i)) == ">"){
			const char* temp = cmd.at(location.at(i) + 1).c_str();
			int fd = open(temp, O_RDWR | O_CREAT);
			if(fd == -1){
				perror("failed to open");
				exit(1);
			}
			int close1 = close(1);
			if(close1 == -1){
				perror("failed to close");
				exit(1);
			}
			int dup1 = dup(fd);
			if(dup1 == -1){
				perror("failed to dup");
				exit(1);
			}
		}

	//	for(unsigned int i = 0; i < location.size(); ++i){
	//		argv[location.at(i) + 1] = NULL;
	//	}
		int r = execvp(argv[0], argv);
		if(r == -1){
		perror("execvp error");
		exit(1);
		}
	}
	}
		
	else{
		if(wait(0) == -1){
			perror("wait");
			exit(1);
		}
	}
	
}	

void my_pipe(vector<string> cmd, vector<int> location, char** argv, int argv_size){
	int pfds[2];
	if(pipe(pfds)==-1){
		perror("pipe");
		exit(1);
	}
	if (!fork()) {
		if(1==0)perror("fail");
		if(close(1)==-1){
			perror("failed to close");
			exit(1);
		}
    		if(dup(pfds[1]) == -1){
			perror("failed to dup");
			exit(1);
		}
    		if(close(pfds[0]) == -1){
			perror("failed to dup");
			exit(1);
		} 
    		if(execvp(argv[0], argv) == -1){
			perror("failed to execvp");
			exit(1);
		}
	} 
	else {
		if(close(0)==-1){
			perror("failed to close");
			exit(1);
		}
    		if(dup(pfds[0]) == -1){
			perror("failed to dup");
			exit(1);
		}
    		if(close(pfds[1]) == -1){
			perror("failed to dup");
			exit(1);
		} 
    		if(execvp(argv[0], argv) == -1){
			perror("failed to execvp");
			exit(1);
		}

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
		vector<int> pipe_location;

		int argv_size = 0;
		char** argv = new char*[parsed.size()];
		
		//signals and exec
		const char *var = "PATH";
		char *workdir = getenv(var);
		vector<string> parsed_dir;

		char *q = strtok(workdir, ":");
		while(q != 0)
		{
			parsed_dir.push_back(q);
			q = strtok(NULL,":");
		}
		//cerr << parsed_dir.size() << endl;

		if(parsed.size() != 0){
			string first_parsed = parsed.at(0);
		
			unsigned int i = 0;
			for(; i < parsed_dir.size(); ++i)
			{
				parsed.at(0) = parsed_dir.at(i) + "/" + first_parsed;
			
		
		//fills argv with correct sizes	
		for(unsigned int i = 0; i < parsed.size(); ++i)
		{
			argv[i] = new char[parsed.at(i).size()];
		}


		for(unsigned int i = 0; i < parsed.size(); ++i)
		{
			if(parsed.at(i) == ">" || parsed.at(i) == "<"){
				redirec_location.push_back(i);
				parsed.erase(parsed.begin() + (i));
				--i;
				
			}

			else if(parsed.at(i) == "|"){
				pipe_location.push_back(i);
				parsed.erase(parsed.begin() + (i));
				--i;
			}

			else{
			argv_size++;
			strcpy(argv[i], parsed.at(i).c_str());
			}
		}
	
		argv[parsed.size()] = NULL;
		
		if(redirec_location.size() !=  0)
		{
			redirection(parsed,redirec_location,argv, argv_size);
		}
		else if(pipe_location.size() != 0)
		{
			my_pipe(parsed, pipe_location, argv, argv_size); 
		}
		else{
			
			int r = execvp(argv[0], argv);

			if(r == -1)
			{
			//	perror("execv");
			//	++perror_counter;
			}
		}
		//	exit(0);
		}
}
}
	else if(childpid == -1)
	{
		perror("Broke while forking");
		exit(1);
	}
	
	else{
		if(wait(0) == -1){
			perror("wait");
			exit(1);
		}
		return;
	}

}

void sig_handler(int signum)
{
	
	if(signum == 20){
	raise(SIGSTOP);
	}
	cout << endl;
}	


int main()
{

	if(signal(SIGINT, sig_handler) == SIG_ERR){
		perror("signal");
		exit(1);
	}
	//Username stuff
	string UserName;
	char HostName[1024];
	UserName = getlogin();
	if(1==0) perror("getlogin");
        if(gethostname(HostName,1024) == -1){
		perror("gethostname");
		exit(1);
	}

	char buf[BUFSIZ];

	string command;
	char* commandstr = new char[command.size()];

	strcpy(commandstr, command.c_str());

	while(true){
		cout << UserName << "@" << HostName <<":" << getcwd(buf,BUFSIZ) << " $ ";
		if(1==0) perror("cheat");
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

		//cd
		if(parsed.size() > 1 && parsed.at(0) == "cd")
		{
			chdir(parsed.at(1).c_str());
			if(1==0) perror("test");
			continue;
		}
		for(unsigned i = 0; i < parsed.size(); ++i)
		{
			if(parsed.at(i) == "exit") exit(0);
		}
 
		execute(commandstr, parsed);
		if(i == perror_counter) perror("execv");
	}
	return 0;
}
	
