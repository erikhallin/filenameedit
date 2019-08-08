#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <stdio.h>

using namespace std;

int main()
{
    cout << "Startup" << endl;

    int max_digits=3;

    //read local filenames
    vector<string> vec_filenames;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (".")) != NULL)
    {
      while ((ent = readdir (dir)) != NULL)
      {
        if(ent->d_name[0]!='.')
            vec_filenames.push_back(ent->d_name);
      }
      closedir (dir);
    }
    else
    {
        cout<<"ERROR: Problems with directory\n";
        return 1;
    }

    //find {
    int counter=0;
    for(int i=0;i<(int)vec_filenames.size();i++)
    {
        bool file_modified=false;

        for(int c=0;c<(int)vec_filenames[i].length();c++)
        {
            if(file_modified) break;

            if(vec_filenames[i][c]=='{')
            {
                if((int)vec_filenames[i].length()<c+max_digits) break;

                //count numbers
                for(int n=max_digits;n>0;n--)
                {
                    if(vec_filenames[i][c+n]=='}')
                    {
                        //add n zeros
                        string new_name=string(vec_filenames[i],0,c+1);
                        string end_name=string(vec_filenames[i],c+1);
                        //cout<<new_name<<endl;
                        new_name.append(max_digits-n+1,'0');
                        new_name.append(end_name);
                        //cout<<new_name<<endl;

                        ofstream newfile(new_name.c_str());
                        ifstream oldfile(vec_filenames[i].c_str());
                        if(newfile==0 || oldfile==0)
                        {
                            cout<<"ERROR with files\n\n";
                            return 1;
                        }
                        // copy data
                        string line;
                        while(getline(oldfile,line))
                        {
                            newfile<<line<<endl;
                        }
                        newfile.close();
                        oldfile.close();

                        //remove old file
                        counter++;
                        file_modified=true;
                        remove(vec_filenames[i].c_str());
                        break;
                    }
                }
            }
        }
    }

    cout<<"Done\n\n";
    cout<<counter<<" files modified\n\n";

    return 0;
}
