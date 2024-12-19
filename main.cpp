#include <iostream>
#include "fun.h"
#include <string.h>
using namespace std;

int main(int argc,char **argv){
    int res=operation_type(argc,argv);
    mp3 obj;
    if(res!=HELP){
        strcpy(obj.name,argv[2]);
    }
    switch (res) {
    case NO_OPERATION:
        cout<<"ERROR: INVALID ARGUMENT"<<endl;
        cout<<"USAGE:"<<endl;
        cout<<"To view please pass like: ./a.out -v <.mp3>"<<endl;
        cout<<"To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c <.mp3>"<<endl;
        break;
    case HELP:
        help();
        break;
    case VIEW:
        view(argc,argv,&obj);
        break;
    case EDIT:
        edit(argc,argv);
        break;
    }
    return 0;
}