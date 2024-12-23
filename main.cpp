#include <iostream>
#include "fun.h"
#include <string.h>
using namespace std;

int main(int argc,char **argv){
    int res=operation_type(argc,argv);        //getting the operation type
    char *index=is_valid(argc,argv);    //address of the argument which has the file name
    if(index==NULL){                        //checking if a valid file name is passed
        cout<<"ERROR: no mp3 file name found"<<endl<<endl;
        help();                             //displaying help
        return 0;
    }
    switch (res) {
        case NO_OPERATION:                  //if no operation is passed or invalid operation is passed
            cout<<"ERROR: INVALID ARGUMENT"<<endl;
            cout<<"USAGE:"<<endl;
            cout<<"To get help or manual please pass like : ./a.out -h"<<endl;
            cout<<"To view tags please pass like          : ./a.out -v <.mp3>"<<endl;
            cout<<"To edit tags please pass like          : ./a.out -e -t/-a/-A/-m/-y/-c <.mp3>"<<endl;
            break;
        case HELP:                          //if help is passed
            help();
            break;
        case VIEW:                          //if view is passed
            view(index);
            break;
        case EDIT:                          //if edit is passed
            edit(argc,argv);
            break;
    }
    return 0;
}