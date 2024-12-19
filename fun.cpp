#include "fun.h"
#include <cstdio>
#include <ostream>
#include <string.h>
#include <iostream>

using namespace std;
int operation_type(int argc, char **argv){
    if(argc==1){
        return NO_OPERATION;
    }
    else if(strcmp(argv[1],"-h")==0){
        return HELP;
    }
    else if(strcmp(argv[1],"-v")==0){
        return VIEW;
    }
    else if(strcmp(argv[1],"-e")==0){
        return EDIT;
    }
    return NO_OPERATION;
}

void help(){
    cout<<"usage: ./a.out {options} filename<.mp3>"<<endl;
    cout<<"options:"<<endl;
    cout<<"-h: help(Displayes this help info)"<<endl;
    cout<<"-v: view(Prints info in the tags)"<<endl;
    cout<<"-e: edit"<<endl;
    cout<<"\t-t: Modifies Title tag"<<endl;
    cout<<"\t-a: Modifies Artist tag"<<endl;
    cout<<"\t-A: Modifies Album tag"<<endl;
    cout<<"\t-y: Modifies Year tag"<<endl;
    cout<<"\t-c: Modifies Comment tag"<<endl;
}

void view(int argc, char **argv,mp3 *obj){
    char buffer[6];
    char temp;
    char *ptr=strstr(argv[2],".");
    if(strcmp(ptr,".mp3")!=0){
        cout<<"ERROR: INVALID FILE TYPE"<<endl;
        cout<<"File must be <.mp3> type"<<endl;
        return;
    }
    FILE *fptr=fopen(argv[2],"r");
    if(fptr==NULL){ 
        cout<<"ERROR: FILE NOT FOUND"<<endl;
        return;
    }
    fgets(buffer,4,fptr);
    if(strcmp(buffer,"ID3")!=0){
        cout<<"ERROR: INVALID FILE FORMAT"<<endl;
        return;
    }
    cout<<"MP3 tag reader and editer for ";
    cout<<buffer;
    obj->ver[0]=fgetc(fptr);
    obj->ver[1]=fgetc(fptr);
    cout<<"v"<<(int)(obj->ver[0])<<endl;
    cout<<"---------------------------------------------------------------------"<<endl;
    obj->flags=fgetc(fptr);
    fseek(fptr,10,SEEK_SET);
    
    read(&obj->buffer,fptr);
    cout<<"Title: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);
    cout<<"Artist: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);
    cout<<"Album: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);
    cout<<"Year: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);
    cout<<"Music: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);
    cout<<"comment: "<<obj->buffer<<endl;
    delete obj->buffer;
    cout<<"---------------------------------------------------------------------"<<endl;
    fclose(fptr);
}

void read(char **output,FILE *fptr){
    char buffer[6];
    fgets(buffer,5,fptr);
    fgetc(fptr),fgetc(fptr),fgetc(fptr);
    int size=0;
    char *ptr=(char *)&size;
    for(int i=0;i<4;i++){
        ptr[i]=fgetc(fptr);
    }
    *output=new char[size+1];
    fread(*output,1,size-1,fptr);
}


void edit(int argc, char **argv){
    if(argv[2][0]!='-'){
        cout<<"ERROR: INVALID ARGUMENT"<<endl;
        return;
    }
    char *ptr=strstr(argv[4],".");
    if(ptr&&strcmp(ptr,".mp3")){
        cout<<"ERROR: File must be <.mp3> type"<<endl;
        return;
    }
    FILE *fptr=fopen(argv[4],"r");
    if(fptr==NULL){
        cout<<"ERROR: unable open the "<<argv[4]<<" File"<<endl;
        return;
    }
    char buffer[20];
    unsigned int size=0,skip=0;
    ptr=(char *)&size;
    fread(buffer,1,3,fptr);
    if(strcmp(buffer,"ID3")){
        cout<<"ERROR: INVALID FILE FORMET"<<endl;
        return;
    }
    fseek(fptr,0,SEEK_SET);
    FILE *fptr2=fopen("temp.mp3","w");
    if(fptr2==NULL){
        cout<<"ERROR: unable to open a temp file ijn writing mode"<<endl;
        return;
    }
    fread(buffer,1,10,fptr);
    cout<<buffer<<endl;
    fwrite(buffer,1,10,fptr2);
    switch(argv[2][1]){
        case 't':
            skip=0;
            break;
        case 'a':
            skip=1;
            break;
        case 'A':
            skip=2;
            break;
        case 'm':
            skip=4;
            break;
        case 'y':
            skip=3;
            break;
        case 'c':
            skip=5;
            break;
        default:
            cout<<"ERROR: INVALID ARGUMENT"<<endl;
            return;
    }
    for(int i=0;i<skip;i++){
        cpy_tag(fptr,fptr2);
    }
    fread(buffer,1,4,fptr);
    cout<<buffer<<endl;
    fwrite(buffer,1,4,fptr2);
    for(int i=0;i<3;i++){
        fputc(fgetc(fptr),fptr2);
    }
    for(int i=0;i<4;i++){
        ptr[i]=fgetc(fptr);
    }
    printf("%d\n",size);
    fseek(fptr,size-1,SEEK_CUR);
    size=strlen(argv[3])+1;
    printf("%d\n",size);
    fwrite(&size,4,1,fptr2);
    fwrite(argv[3],1,size-1,fptr2);
    cpy_remaining(fptr,fptr2);
}

void cpy_remaining(FILE *fptr,FILE *fptr2){
    while(feof(fptr)==0){
        fputc(fgetc(fptr),fptr2);
    }
}

int cpy_tag(FILE *fptr,FILE *fptr2){
    char buffer[20];
    int size=0;
    char *ptr=(char *)&size;
    fread(buffer,1,4,fptr);
    fwrite(buffer,1,4,fptr2);
    for(int i=0;i<3;i++){
        fputc(fgetc(fptr),fptr2);
    }
    ptr=(char *)&size;
    for(int i=0;i<4;i++){
        ptr[i]=fgetc(fptr);
    }
    fwrite(&size,4,1,fptr2);
    for(int i=0;i<size-1;i++){
        fputc(fgetc(fptr),fptr2);
    }
    return size;
}
