#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FLAG_a 1
#define FLAG_l 2
#define FLAG_r 3
#define flag_R 4

int main(int argc, char** argv)
{
	int flags = 0;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			for (int j=1; argv[i][j] !=0; ++j)
			{
				if(argv[i][j] == 'a')
					flags |= FLAG_a;
			//unsure about this
				else if(argv[i][j] == 'l')
					flags |= FLAG_l;
				else if(argv[i][j] == 'r')
					flags |= FLAG_r;
				else if(argv[i][k] == 'R')
					flags |= FLAG_R;
			}
		}
	}

	//main code
	
	return 0;
}
