#include <sys/types.h>
#include <time.h>
#include <grp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include <errno.h>
#include <iostream>
#include <pwd.h>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <vector>
using namespace std;

void print_long(vector<string> list, int flag, const char* dir){
	int max = 4;
	for(unsigned int i = 0; i < list.size();++i)
        {
            struct stat statbuf;
            string words = list.at(i);
            string path = "/";
            string combined_path = dir + path + words;
            lstat(combined_path.c_str(), &statbuf);
           /* if((words == "." || words == "..") && !flag)
            {
                continue;
            }
            if(words[0] == '.' && !flag)
            {
                continue;
            }
           */ if(S_ISDIR(statbuf.st_mode))
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
           
	    if(words[0] == '.')
            {
                if(S_ISLNK(statbuf.st_mode))
                {
                    cout << "\033[1;100;36m" << words << "\033[0m" << ' ';

                }
                else if(S_ISDIR(statbuf.st_mode))
                {
                    cout << "\033[1;100;34m" << words << "\033[0m" << ' ';
                }
                else if((S_IXUSR & statbuf.st_mode) || (S_IXGRP & statbuf.st_mode) || (S_IXOTH &statbuf.st_mode))
                {
                    cout << "\033[100;32m" << words << "\033[0m" << ' ';

                }
                else cout << words << ' ';
            }
            else
            {
                if(S_ISLNK(statbuf.st_mode))
                {
                    cout << "\033[1;36m" << words << "\033[0m" << ' ';

                }
                else if(S_ISDIR(statbuf.st_mode))
                {
                    cout << "\033[1;34m" << words << "\033[0m" << ' ';
                }

                else if((S_IXUSR & statbuf.st_mode) || (S_IXGRP & statbuf.st_mode) || (S_IXOTH & statbuf.st_mode))
                {
                    cout << "\033[1;32m" << words << "\033[0m" << ' ';

                }
                else cout << words << ' ';
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
    unsigned int jalapenos = row.size();
    if(jalapenos == 0)
    row.push_back(column);
    for(unsigned i = 0; i < row.size();++i)
    {
        for(unsigned x= 0 ; x < row.at(i).size();++x)
        {

            struct stat statbuf;
            string word = row.at(i).at(x);
            string path = "/";
            string newpath = dir + path + word;
            lstat(newpath.c_str(), &statbuf);
            if(word[0] == '.')
            {
                if(S_ISLNK(statbuf.st_mode))
                {
                    cout << "\033[1;100;36m" << row.at(i).at(x) << "\033[0m" << ' ';

                }
                else if(S_ISDIR(statbuf.st_mode))
                {
                    cout << "\033[1;100;34m" << row.at(i).at(x) << "\033[0m" << ' ';
                }
                else if((S_IXUSR & statbuf.st_mode) || (S_IXGRP & statbuf.st_mode) || (S_IXOTH & statbuf.st_mode))
                {
                    cout << "\033[100;32m" << row.at(i).at(x) << "\033[0m" << ' ';

                }
                else cout << row.at(i).at(x) << ' ';
            }
            else
            {
                if(S_ISLNK(statbuf.st_mode))
                {
                    cout << "\033[1;36m" << row.at(i).at(x) << "\033[0m" << ' ';

                }
                else if(S_ISDIR(statbuf.st_mode))
                {
                    cout << "\033[1;34m" << row.at(i).at(x) << "\033[0m" << ' ';
                }

                else if((S_IXUSR & statbuf.st_mode) || (S_IXGRP & statbuf.st_mode) || (S_IXOTH & statbuf.st_mode))
                {
                    cout << "\033[1;32m" << row.at(i).at(x) << "\033[0m" << ' ';

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


void ls_R(const char* dir, int flaga, int flagl)
{
     string dirName = dir;
     cout << dir << ": " << endl;

     vector <string>t;
     if(flagl)
     {
         ls_l(dir, flaga);
     }
     DIR *dirp = opendir(dir);
     if(dirp == NULL)
         error("Error opening directory");
     
     dirent *direntp;
     vector <string> list;
     while ((direntp = readdir(dirp)))
     {
         if(direntp == NULL)
             error("Error");
         
         char * word = direntp->d_name;
         string words = direntp->d_name;
         string patherino = "/";
         string newpatherino = dirName + patherino + word;
         struct stat buf;
            if(stat(newpatherino.c_str(), &buf))
                error("Error");
         if(words == "." || words  == "..");
         else{
            if(word[0] == '.' && !flaga)
            {
                continue;
            }
            list.push_back(words);
            if(S_ISDIR(buf.st_mode))
            {
                string path ="/";
                string newpath = dirName + path + word;
                t.push_back(newpath);
            }
         }

     }
     sort(list.begin(), list.end(), locale("en_US.UTF-8"));
     sort(t.begin(), t.end(), locale("en_US.UTF-8"));

         if(!flagl)
            output(list, dir);
     cout << endl;
     for(unsigned i = 0; i < t.size();++i)
     {

         string tmp = t.at(i);

                ls_R(tmp.c_str(), flaga, flagl);
     }
   if(closedir(dirp))
	error("Error closedir");

 }




int main(int argc, char *argv[])
{

     int flag_a = 0;
     int flag_l = 0;
     int flag_r = 0;
    vector<char*> files1;
    for(int i = 1; i < argc ; ++i)
    {
        if(argv[i][0] == '-')
        {
             for(unsigned int k = 1; k < strlen(argv[i]);++k)
             {
                if(argv[i][k] == 'a')
                {
                    flag_a = 1;
                }
                else if(argv[i][k] == 'l')
                {
                    flag_l = 1;
                }
                else if(argv[i][k] == 'R')
                {
                    flag_r = 1;
                }
                else
                {
                    cerr << "Error: LS: Command not found" << endl;
                    return 0;
                }
             }
        }
        else
        {
            files1.push_back(argv[i]);
        }
     }
     vector<char*> files;
     vector<char*> goodfiles;
     for(unsigned int i = 0; i < files1.size();++i)
      {
            struct stat buf;
            if(stat(files1.at(i), &buf))
            {
                files.push_back(files1.at(i));
            }
            else
                goodfiles.push_back(files1.at(i));


      }
      for(unsigned int i = 0; i < goodfiles.size();++i)
          files.push_back(goodfiles.at(i));
     if(flag_a + flag_r + flag_l == 0)
     {
         if(files.size() == 0)
         {
            string foo= ".";
            ls(foo.c_str(), 0);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
                 ls(files.at(i), 0);
             }
         }
     }
    if(flag_a && flag_r + flag_l == 0)
    {

         if(files.size() == 0)
         {
            string foo= ".";
            ls(foo.c_str(), flag_a);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
                 ls(files.at(i), flag_a);
                 cout << endl;
             }
         }
    }
    if(flag_r)
    {
         if(files.size() == 0)
         {
            string a = ".";
            ls_R(a.c_str(), flag_a, flag_l);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
                 ls_R(files.at(i), flag_a, flag_l);
                 cout << endl;
             }
         }
    }
    if(flag_l && flag_a + flag_r == 0)
    {
         if(files.size() == 0)
         {
            string foo= ".";
            ls_l(foo.c_str(), flag_a);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
                 ls_l(files.at(i), flag_a);
                 cout << endl;
             }
         }
    }
    if(flag_a + flag_r == 2 && !flag_l)
    {
        cout << "a + r" << endl;
    }
    if(flag_l + flag_r == 2 && !flag_a)
    {
        cout << "l + r" << endl;
        if(files.size() == 0)
         {
            string foo= ".";
            ls_R(foo.c_str(), flag_a, flag_l);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
                 ls_R(files.at(i), flag_a, flag_l);
                 cout << endl;
             }
         }
    }
    if(flag_a + flag_l == 2 && !flag_r)
    {
         if(files.size() == 0)
         {
            string foo= ".";
            ls_l(foo.c_str(), flag_a);
         }
         else
         {
             for(unsigned int i = 0; i < files.size(); ++i)
             {
                 ls_l(files.at(i), flag_a);
                 cout << endl;
             }
         }
    }
    return 0;
}
