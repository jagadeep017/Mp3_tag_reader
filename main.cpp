#include <iostream>
#include "fun.h"
#include <string.h>
using namespace std;

int main(int argc,char **argv){
    int res=operation_type(argc,argv);  //getting the operation type
    mp3 obj;                            //creating object of mp3 class
    if(res!=HELP){                      //if the operation type is not help
        strcpy(obj.name,argv[2]);
    }
    switch (res) {
    case NO_OPERATION:                  //if no operation is passed or invalid operation is passed
        cout<<"ERROR: INVALID ARGUMENT"<<endl;
        cout<<"USAGE:"<<endl;
        cout<<"To view please pass like: ./a.out -v <.mp3>"<<endl;
        cout<<"To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <.mp3>"<<endl;
        break;
    case HELP:                          //if help is passed
        help();
        break;
    case VIEW:                          //if view is passed
        view(argc,argv,&obj);
        break;
    case EDIT:                          //if edit is passed
        edit(argc,argv);
        break;
    }
    return 0;
}