#include "fun.h"
#include <cstdio>
#include <ostream>
#include <string.h>
#include <iostream>

using namespace std;

//function takes command line arguments and returns the operation type
//argc : command line arguments count
//argv : command line argumnets vector
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

//function to check if atleast one argument is a valid mp3 file name
//argc : command line arguments count
//argv : command line argumnets vector
char *is_valid(int argc,char **argv){
    char* index=argv[1];    
    char*ptr;
    for(int i=2;index!=NULL;i++){
        ptr=strstr(index,".");
        if(ptr&&strcmp(ptr,".mp3")==0){
            return index;
        }
        index=argv[i];
    }
    return index;
}

//function to display options and usage
void help(){
    cout<<"usage: ./a.out {options} filename.mp3"<<endl;
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

//function to view the tags in the mp3 file
//name : name of the mp3 file
void view(const char* name){
    char *buffer = new char[6];         //buffer to store some datas of the file
    char *tag;                          //tag magic string
    FILE *fptr=fopen(name,"r");      //opening the file in read mode
    if(fptr==NULL){                                     //checking if the file is opened successfully    
        cout<<"ERROR: FILE NOT FOUND"<<endl;
        return;
    }
    fread(buffer,1,3,fptr);                   //reading the first 3 bytes of the file
    buffer[3]='\0';                                     //adding null character at the end of the buffer
    if(strcmp(buffer,"ID3")!=0){                 //checking if the file of mp3 format
        cout<<"This mp3 file does not contain any tags"<<endl;
        return;
    }
    cout<<"MP3 tag reader and editer for ";
    cout<<buffer;
    buffer[0]=fgetc(fptr);                    //reading the version info
    buffer[1]=fgetc(fptr);
    cout<<"v"<<(int)(buffer[0])<<endl;
    delete[] buffer;
    cout<<"---------------------------------------------------------------------"<<endl;
    fseek(fptr,10,SEEK_SET);        //skipping the first 10 bytes
    
    tag=read(&buffer,fptr);                    //reading the title tag
    if(strcmp(tag,"TIT2")==0){                 //checking if Title tag is present
        cout<<"Title    : "<<buffer<<endl;
        delete[] buffer;                                 //deleting the buffer to avoid memory leak
        delete[] tag;
        tag=read(&buffer,fptr);                    //reading the artist tag
    }
    else{
        cout<<"Title    : Nill"<<endl;
    }
    if(strcmp(tag,"TPE1")==0){                 //checking if Artist tag is present
        cout<<"Artist   : "<<buffer<<endl;
        delete[] buffer;                                 //deleting the buffer to avoid memory leak
        delete[] tag;
        tag=read(&buffer,fptr);                    //reading the album tag
    }
    else{
        cout<<"Artist   : Nill"<<endl;
    }
    if(strcmp(tag,"TALB")==0){                 //checking if Album tag is present
        cout<<"Album    : "<<buffer<<endl;
        delete[] buffer;                                 //deleting the buffer to avoid memory leak
        delete[] tag;
        tag=read(&buffer,fptr);                    //reading the year tag
    }
    else{
        cout<<"Album    : Nill"<<endl;
    }
    if(strcmp(tag,"TYER")==0){                 //checking if Year tag is present
        cout<<"Year     : "<<buffer<<endl;
        delete[] buffer;                                 //deleting the buffer to avoid memory leak
        delete[] tag;
        tag=read(&buffer,fptr);                    //reading the genre tag
    }
    else{
        cout<<"Year     : Nill"<<endl;
    }
    if(strcmp(tag,"TCON")==0){                 //checking if Genre tag is present
        cout<<"Genre    : "<<buffer<<endl;
        delete[] buffer;                                 //deleting the buffer to avoid memory leak
        delete[] tag;
        tag=read(&buffer,fptr);                    //reading the comment tag

    }
    else{
        cout<<"Genre    : Nill"<<endl;
    }
    if(strcmp(tag,"COMM")==0){                 //checking if Comment tag is present
        cout<<"comment  : "<<buffer<<endl;
        delete[] buffer;                                 //deleting the bufferto avoid memory leak
        delete[] tag;
    }
    else{
        cout<<"comment  : Nill"<<endl;
    }
    cout<<"---------------------------------------------------------------------"<<endl;
    fclose(fptr);
}

//function to read the tags from the file
char *read(char **output,FILE *fptr){
    char *tag=new char[5];             //buffer to store some datas of the file
    fgets(tag,5,fptr);    //reading the first 4 bytes of the file(tag magic string)
    tag[4]='\0';
    int size=0;                 //variable to store the size of the tag
    char *ptr=(char *)&size;    //pointer to the lsb of thesize variable
    for(int i=3;i>=0;i--){
        ptr[i]=fgetc(fptr);    //reading the size of the tag
    }
    fgetc(fptr),fgetc(fptr),fgetc(fptr);    //skipping the next 3 bytes
    *output=new char[size+1];       //allocating memory
    fread(*output,1,size-1,fptr);   //reading the tag
    (*output)[size-1]='\0';         //adding null character at the end of the tag
    return tag;
}


//function to edit the tags in the mp3 file
//argc : command line arguments count
//argv : command line argumnets vector
//name : name of the mp3 file
void edit(int argc, char **argv,const char* name){
    if(argv[2][0]!='-'){                                //checking if the argument is valid
        cout<<"ERROR: INVALID ARGUMENT"<<endl;
        cout<<"Please pass tag argiments like: -t/-a/-A/-m/-y/-c"<<endl;
        cout<<"Ex:- ./a.out -e -y 2023 name.mp3"<<endl;
        return;
    }
    char *ptr;
    FILE *fptr=fopen(name,"r");          //opening the file in read mode
    if(fptr==NULL){                                             //checking if the file is opened successfully
        cout<<"ERROR: unable open the "<<name<<" File"<<endl;
        return;
    }
    char buffer[11];        //buffer to store some datas of the file
    unsigned int size=0,skip=0;     //variable to store the size of the tag and the number of tags to skip
    fread(buffer,1,3,fptr);        //reading the first 3 bytes of the file
    buffer[3]='\0';                     //adding null character at the end of the buffer
    if(strcmp(buffer,"ID3")){                   //checking if the file of mp3 format
        cout<<"ERROR: INVALID FILE FORMET"<<endl;
        return;
    }
    fseek(fptr,0,SEEK_SET);         //setting the file pointer to the start of the file
    FILE *fptr2=fopen("temp.mp3","w");    //opening a temp file in write mode
    if(fptr2==NULL){                                        //checking if the file is opened successfully
        cout<<"ERROR: unable to open a temp file ijn writing mode"<<endl;
        return;
    }
    fread(buffer,1,10,fptr);        //reading the first 10 bytes of the file to the buffer
    fwrite(buffer,1,10,fptr2);           //writing the first 10 bytes of the file to the temp file
    
    switch(argv[2][1]){                     //assaigning the number of tags to skip based on the argument
        case 't':
            cout<<"Modifying Title Tag to ";
            skip=0;
            break;
        case 'a':
            cout<<"Modifying Artist Tag to ";
            skip=1;
            break;
        case 'A':
            cout<<"Modifying Album Tag to ";
            skip=2;
            break;
        case 'm':
            cout<<"Modifying Genre Tag to ";
            skip=4;
            break;
        case 'y':
            cout<<"Modifying Year Tag to ";
            skip=3;
            break;
        case 'c':
            cout<<"Modifying Comment Tag to ";
            skip=5;
            break;
        default:
            cout<<"ERROR: INVALID ARGUMENT"<<endl;   //if the argument is invalid just return
            return;
    }
    ptr=argv[3];
    cout<<"\"";
    for(int i=4;name!=ptr;i++){
        cout<<ptr<<" ";
        ptr=argv[i];
    }
    cout<<"\"";
    cout<<endl;
    for(int i=0;i<skip;i++){        //skipping the tags
        cpy_tag(fptr,fptr2);
    }
    fread(buffer,1,4,fptr);     //reading the 4 bytes from the file to the buffer
    fwrite(buffer,1,4,fptr2);       //writing the 4 bytes to the temp file
    ptr=(char *)&size;              //pointer to the lsb of the size variable
    for(int i=3;i>=0;i--){
        ptr[i]=fgetc(fptr);     //reading the size of the tag
    }
    int len=size;
    size=0;
    for(int i=3;strcmp(argv[i],name);i++){
        size+=strlen(argv[i])+1;        //adding the size of the words to the size variable
    }
    for(int i=3;i>=0;i--){                //writing the size of the tag to the temp file
        fputc(ptr[i],fptr2);
    }
    for(int i=0;i<3;i++){
        fputc(fgetc(fptr),fptr2);    //writing the next 3 bytes to the temp file
    }
    fseek(fptr,len-1,SEEK_CUR);    //skipping the tag in the original file
    for(int i=3;strcmp(argv[i],name);i++){  //writing the new tags till the file name
    if(i!=3){
        fputc(' ',fptr2);               //writing a space after each word
    }
        fwrite(argv[i],1,strlen(argv[i]),fptr2);    //writing the word to the temp file
    }
    cpy_remaining(fptr,fptr2);  //copying the remaining tags
    fclose(fptr);       //closing the file
    fclose(fptr2);      //closing the temp file
    remove(name);    //removing the original file
    rename("temp.mp3",name);   //renaming the temp file to the original file
}

//function to copy the remaining tags and data from the file
void cpy_remaining(FILE *fptr,FILE *fptr2){
    while(feof(fptr)==0){
        fputc(fgetc(fptr),fptr2);
    }
}

//function to copy the tag from the file
int cpy_tag(FILE *fptr,FILE *fptr2){
    char buffer[20];
    int size=0;
    char *ptr=(char *)&size;
    fread(buffer,1,4,fptr);
    fwrite(buffer,1,4,fptr2);
    ptr=(char *)&size;
    for(int i=3;i>=0;i--){
        ptr[i]=fgetc(fptr);
        fputc(ptr[i],fptr2);
    }
    for(int i=0;i<3;i++){
        fputc(fgetc(fptr),fptr2);
    }
    for(int i=0;i<size-1;i++){
        fputc(fgetc(fptr),fptr2);
    }
    return size;
}
