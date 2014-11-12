#include <iostream>
#include <fstream>
#include "Timer.h"
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <cstdlib>


using namespace std;

void method1(int argc, char* argv[]){

	ifstream check(argv[2]);
	if(check)
	{
		cout << "File \"" << argv[2] << "\" already exists.\n";
		exit(0);	
	}

	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);
	if(!infile.is_open())
	{
		cerr << "File \"" << argv[1] << "\" could not be open\n";
		exit(0);
	}
	if(!outfile.is_open())
	{
		cerr << "File \"" << argv[2] << "\" could not be open\n";
		exit(0);
	}	


	outfile << infile.rdbuf();

	outfile.close();
	infile.close();

	return;
}

void method3(int argc, char*argv[]){

	string input = argv[1];
	string output = argv[2];
	
	int inFile = open(input.c_str(), O_RDONLY);
	int exists = access(output.c_str(), F_OK);
	if(inFile == -1)
	{
		perror("There was an error with open(). ");
		exit(1);
	}
	if(exists == 0)
	{
		cerr << "Error:" <<  output << " already exists.\n";
		exit(1);
	}
	int outFile = open(output.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if(outFile == -1)
	{	
		perror("There was an error with open(). ");
		exit(1);
	}

	char buf[BUFSIZ];
	int n;
	int p;

	while((n = read(inFile, buf, BUFSIZ)) > 0)
	{
		if(n == -1)
		{	
			perror("There was an error with read(). ");
			exit(1);
		}	
		p = write(outFile, buf, n);
		if(p == -1)
		{
			perror("There was an error with write(). ");
			exit(1);
		}
	}
	return;	
}

void method2(int argc, char*argv[]){

       int fdi = open(argv[1],O_RDONLY);
       int fdo = open(argv[2],O_WRONLY | O_CREAT,S_IRWXU);

       if(fdi == -1 || fdo == -1){
                perror("open");
       }

                char buf[BUFSIZ];

                while(int readNum = read(fdi,buf,1) > 0){
                        int writeChk = write(fdo,buf,1);

                        if(readNum == -1){
                                perror("read");
                        }

                        if(writeChk == -1){
                                perror("write");
                        }
                }
	return ;
}

int main(int argc, char*argv[])
{
	if(argc < 3){
		cout << "Error: Too few arguements\n";
		return 1;
	}
	
	string flag;
	if(argc == 4)
	{
		flag = argv[3];
	}
	
	Timer t;
	double eTime;
	t.start();
	
	if(flag[0] == '-')
	{
		string input;
		cout << "Warning, this program will remove the file provided in argv[2]" << endl
		<< "press 'y' and then enter to continue: " << endl;
		cin >> input;
		if(input != "y")		//exit program if input does not equal y
			return 0;
		method1(argc,argv);
		t.elapsedWallclockTime(eTime);
		cout << "Wallclock Time: " << eTime << endl;
		
		t.elapsedUserTime(eTime);
		cout << "User Time: " << eTime << endl;

		t.elapsedSystemTime(eTime);
		cout << "System Time: " << eTime << endl;
		
		//clear file
		if( remove( argv[2] ) != 0 )
		    perror( "Error deleting file" );
		Timer t2;
		double eTime2;
		t2.start();
		
		method2(argc,argv);
		t.elapsedWallclockTime(eTime2);
		cout << "Wallclock Time: " << eTime2 << endl;
		
		t.elapsedUserTime(eTime2);
		cout << "User Time: " << eTime2 << endl;

		t.elapsedSystemTime(eTime2);
		cout << "System Time: " << eTime2 << endl;
		//clear file
		if( remove( argv[2] ) != 0 )
			perror( "Error deleting file" );
		Timer t3;
		double eTime3;
		t3.start();

		method3(argc,argv);
		t3.elapsedWallclockTime(eTime3);
		cout << "Wallclock Time: " << eTime3 << endl;
		
		t.elapsedUserTime(eTime3);
		cout << "User Time: " << eTime3 << endl;

		t.elapsedSystemTime(eTime);
		cout << "System Time: " << eTime3 << endl;
		
	}
	else{
		method3(argc,argv);
		return 0;
	}
	return 0;
}
