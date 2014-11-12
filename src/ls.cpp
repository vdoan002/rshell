#include <iostream>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <grp.h>
#include <dirent.h>
#include <stdio.h>


using namespace std;

void print_long(vector<string> list, int flag, const char* dir){
	int max = 4; //will be used for later
	for(unsigned int i = 0; i < list.size();++i)
        {
            struct stat statbuf;
            string current_file = list.at(i);
            string temp = "/";
	
		//concatinates the new directory by
		//adding them together with a fowardslash in middle
            string combined_path = dir + temp + current_file;
            lstat(combined_path.c_str(), &statbuf);

            if(S_ISDIR(statbuf.st_mode))
            {
                    cout << 'd';
            }
	    else cout << "-";	
            
	    if(S_IRUSR& statbuf.st_mode){
                cout << 'r';
            }   else cout << '-';
            if(S_IWUSR & statbuf.st_mode){
                cout << 'w';
            }   else cout << '-';
            if(S_IXUSR & statbuf.st_mode){
                cout << 'x';
            }   else cout << '-';
            if(S_IRGRP& statbuf.st_mode){
                cout << 'r';
            }   else cout << '-';
              
	    if(S_IWGRP & statbuf.st_mode){
                cout << 'w';
            }   else cout << '-';
            if(S_IXGRP & statbuf.st_mode){
                cout << 'x';
            }   else cout << '-';
            
	    if(S_IROTH& statbuf.st_mode){
                cout << 'r';
            }   else cout << '-';
            if(S_IWOTH & statbuf.st_mode){
                cout << 'w';
            }   else cout << '-';
            if(S_IXOTH & statbuf.st_mode){
                cout << "x ";
            }   else cout << "- ";
            
	    cout <<  statbuf.st_nlink << ' ';
            
	    struct passwd *pw;          //refer to stackoverflow for better understanding
            uid_t uid = statbuf.st_uid;
            pw = getpwuid(uid);
            cout << pw->pw_name << ' ';
           
	    struct group *gp;
            gid_t gid = statbuf.st_gid;
            gp = getgrgid(gid);
            cout << gp->gr_name << ' ';
           
	    if(statbuf.st_size> 10000 && max <5)
                max = 5;
	    else if(statbuf.st_size > 100000 && max <6)
		max = 6;
	    else if(statbuf.st_size > 1000000 && max <7)
		max = 7;
	    else if(statbuf.st_size > 10000000 && max <8)
		max = 8;
           
	    cout << setw(max)  << right << statbuf.st_size << ' ';
            
	    char timestamp[20];
            strftime(timestamp, 20, "%b %e %R", localtime(&statbuf.st_mtime));
            cout << timestamp << ' ';
           
	    if(current_file[0] == '.')
            {
                if(S_ISDIR(statbuf.st_mode))
                {
                    cout << "\033[34;100m" << current_file << "\033[0m" << ' ';
                }
  
                else if(S_ISLNK(statbuf.st_mode))
                {
                    cout << "\033[35;100m" << current_file << "\033[0m" << ' ';
                }
               else if((S_IXOTH &statbuf.st_mode) || (S_IXGRP & statbuf.st_mode) || (S_IXUSR & statbuf.st_mode))
                {
                    cout << "\033[32;100m" << current_file << "\033[0m" << ' ';
                }
                else cout << current_file << ' ';
            }
            else
            {
                if(S_ISDIR(statbuf.st_mode))
                {
                    cout << "\033[34m" << current_file << "\033[0m" << ' ';
                }
                else if(S_ISLNK(statbuf.st_mode))
                {
                    cout << "\033[35m" << current_file << "\033[0m" << ' ';
                }

                else if((S_IXOTH &statbuf.st_mode) || (S_IXGRP & statbuf.st_mode) || (S_IXUSR & statbuf.st_mode))
                {
                    cout << "\033[32m" << current_file << "\033[0m" << ' ';
                }

                else cout << current_file << ' ';
            }
            cout << endl;
        }
}

void output(vector<string> list, const char* dir)
{
    unsigned int max = 80; //comes from GNU ls
    unsigned int cur = 0;

    vector<string> column;
    vector<vector<string> > row;
    for(unsigned int i = 0; i < list.size();++i)
    {
	//for a max of 80 char per row
	//if over, move onto next row
        cur+= (list.at(i).size() - 1);
        if(cur < max)
        {
            column.push_back(list.at(i));
        }
        else
        {
            cur = 0;
            row.push_back(column);
            column.clear();
        }
    }
    unsigned int current_directory= row.size();
    if(current_directory == 0)
    	row.push_back(column);
    for(unsigned i = 0; i < row.size();++i)
    {
        for(unsigned x= 0 ; x < row.at(i).size();++x)
        {

            struct stat statbuf;
            string word = row.at(i).at(x);
            string temp = "/";
            string newpath = dir + temp + word;
            lstat(newpath.c_str(), &statbuf);
            if(word[0] == '.')
            {
                if(S_ISLNK(statbuf.st_mode))
                {
                    cout << "\033[36;100m" << row.at(i).at(x) << "\033[0m" << ' ';
                }
                else if(S_ISDIR(statbuf.st_mode))
                {
                    cout << "\033[34;100m" << row.at(i).at(x) << "\033[0m" << ' ';
                }
                else if((S_IXUSR & statbuf.st_mode) || (S_IXGRP & statbuf.st_mode) || (S_IXOTH & statbuf.st_mode))
                {
                    cout << "\033[32;100m" << row.at(i).at(x) << "\033[0m" << ' ';
                }
                else cout << row.at(i).at(x) << ' ';
            }
            else
            {
                if(S_ISLNK(statbuf.st_mode))
                {
                    cout << "\033[36m" << row.at(i).at(x) << "\033[0m" << ' ';
                }
                else if(S_ISDIR(statbuf.st_mode))
                {
                    cout << "\033[34m" << row.at(i).at(x) << "\033[0m" << ' ';
                }

                else if((S_IXUSR & statbuf.st_mode) || (S_IXGRP & statbuf.st_mode) || (S_IXOTH & statbuf.st_mode))
                {
                    cout << "\033[32m" << row.at(i).at(x) << "\033[0m" << ' ';

                }
                else cout << word << ' ';
            }

        }
        cout << endl;
    }
}
int blocksize(vector<string> list)
{
    int total = 0;
    struct stat buf;
    for(unsigned int i = 0; i < list.size();++i)
    {
        string current = list.at(i);
        stat(current.c_str(), &buf);
        total += buf.st_blocks;

    }
    total = total/2;
    return total;

}

void error(const char* msg){
	perror(msg);
	exit(1);
}

void ls(const char* dir, int flag)
{
     const char* dirName = dir;
     DIR *dirp = opendir(dirName);
     if(dirp == NULL)
     {
         error("Error: invalid file name");
     }

     vector <string> list;

     dirent *direntp;
     while ( (direntp = readdir(dirp)) )
     {
	 if(direntp == NULL)
		error("Error at readdir");
         string name = direntp->d_name;
         if(!flag) // if flag isn't -a, don't show hidden files
         {
            if(name[0] != '.')
            list.push_back(name);
         }
         else // for -a flag
             list.push_back(name);
     }
     sort(list.begin(), list.end(), locale("en_US.UTF-8")); //sort alphabeticaly
     output(list, dirName);

if(closedir(dirp))
	error("closedir error");

}



void ls_l(const char* dir, int flag)
{
        const char *dirName = dir;
        DIR *dirp = opendir(dirName);
        if (dirp == NULL)
        {
            error("Broke opening");
        }

        vector <string> list;
	dirent *direntp;
        while((direntp = readdir(dirp)))
            {
                if(direntp == NULL)
                {
                    error("Error on -l");
                }
                string word = direntp->d_name;
                if((word[0] == '.') && !flag);
                else
            		list.push_back(word);

        }
        
	int totalsize = blocksize(list);
        cout << "total: " << totalsize << endl;
        sort(list.begin(), list.end(), locale("en_US.UTF-8"));
	       
	print_long(list, flag, dir);
	
        if(closedir(dirp))
		error("error closing");
}


void ls_R(const char* dir, int flag_a, int flag_l)
{
     cout << endl << "Currently not implemented." << endl;
     exit(1);
 }




int main(int argc, char *argv[])
{

    int FLAG_a = 0;
    int FLAG_l = 0;
    int FLAG_R = 0;

    vector<char*> files;

    for(int i = 1; i < argc ; ++i)
    {
        if(argv[i][0] == '-')
        {
             for(unsigned int k = 1; k < strlen(argv[i]);++k)
             {
                if(argv[i][k] == 'a')
                {
                    FLAG_a = 1;
		//	flags |= FLAG_a
                }
                else if(argv[i][k] == 'l')
                {
                    FLAG_l = 1;
		//	flags |= FLAG_l
                }
                else if(argv[i][k] == 'R')
                {
                    FLAG_R = 1;
			//flags |= FLAG_R
                }
                else
                {
                    return 0;
                }
             }
        }
	else
        {
            files.push_back(argv[i]);
        }
     }
    if(FLAG_a + FLAG_l == 2 && !FLAG_R)  //ls -l -a
    {
	FLAG_a = 1;
         if(files.size() == 0)
         {
            string begin= ".";
            ls_l(begin.c_str(), FLAG_a);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
		 string begin = files.at(i);
                 ls_l(begin.c_str(), FLAG_a);
                 cout << endl;
             }
         }
    }
	if(FLAG_a + FLAG_R + FLAG_l == 0)
     {
         if(files.size() == 0)
         {
            string begin = ".";
            ls(begin.c_str(), 0);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
		 string begin = ".";
                 ls(begin.c_str(), 0);
             }
         }
     }
    if(FLAG_a && FLAG_R + FLAG_l == 0)
    {

         if(files.size() == 0)
         {
            string begin = ".";
            ls(begin.c_str(), FLAG_a);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
		 string begin = files.at(i);
                 ls(begin.c_str(), FLAG_a);
                 cout << endl;
             }
         }
    }
    
    if(FLAG_l && FLAG_a + FLAG_R == 0)
    {
         if(files.size() == 0)
         {
            string begin= ".";
            ls_l(begin.c_str(), FLAG_a);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
		string begin = files.at(i);
                 ls_l(begin.c_str(), FLAG_a);
                 cout << endl;
             }
         }
    }
    if(FLAG_R)
    {
         if(files.size() == 0)
         {
            string begin = ".";
            ls_R(begin.c_str(), FLAG_a, FLAG_l);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
		 string begin = files.at(i);
                 ls_R(begin.c_str(), FLAG_a, FLAG_l);
                 cout << endl;
             }
         }
    }
    if(FLAG_l + FLAG_R == 2 && !FLAG_a) //ls -l -r
    {
	FLAG_l = 1;
	FLAG_a = 0;	
        if(files.size() == 0)
         {
            string begin= ".";
            ls_R(begin.c_str(), FLAG_a, FLAG_l);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
		 string begin = files.at(i);
                 ls_R(begin.c_str(), FLAG_a, FLAG_l);
                 cout << endl;
             }
         }
    }

    
    return 0;
}
