#include <iostream>
#include <fstream>
#include "Time.h"
#include <cstring>

using namespace std;

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
	
	ifstream check(argv[2]);
	if(check)
	{
		cout << "File \"" << argv[2] << "\" already exists.\n";
		return 0;
	}

	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);
	if(!infile.is_open())
	{
		cerr << "File \"" << argv[1] << "\" could not be open\n";
		return 1;
	}
	if(!outfile.is_open())
	{
		cerr << "File \"" << argv[2] << "\" could not be open\n";
		return 1;
	}	


	outfile << infile.rdbuf();

	outfile.close();
	infile.close();
	if(flag == "-t")
	{
		t.elapsedWallclockTime(eTime);
		cout << "Wallclock Time: " << eTime << endl;
		
		t.elapsedUserTime(eTime);
		cout << "User Time: " << eTime << endl;

		t.elapsedSystemTime(eTime);
		cout << "System Time: " << eTime << endl;
	}
	
	return 0;
}
