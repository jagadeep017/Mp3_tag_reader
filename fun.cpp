#include "fun.h"
#include <cstdio>
#include <ostream>
#include <string.h>
#include <iostream>

using namespace std;

//takes command line arguments and returns the operation type
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

//function to display options and usage
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

//function to view the tags in the mp3 file
void view(int argc, char **argv,mp3 *obj){
    char buffer[6];         //buffer to store some datas of the file
    char *ptr=strstr(argv[2],".");      //pointer to the occurance of '.' in the file name
    if(strcmp(ptr,".mp3")!=0){          //checking if the file name consists of .mp3 extension
        cout<<"ERROR: INVALID FILE TYPE"<<endl;
        cout<<"File must be <.mp3> type"<<endl;
        return;
    }
    FILE *fptr=fopen(argv[2],"r");      //opening the file in read mode
    if(fptr==NULL){                                     //checking if the file is opened successfully    
        cout<<"ERROR: FILE NOT FOUND"<<endl;
        return;
    }
    fread(buffer,1,3,fptr);                   //reading the first 3 bytes of the file
    buffer[3]='\0';                                 //adding null character at the end of the buffer
    if(strcmp(buffer,"ID3")!=0){                 //checking if the file of mp3 format
        cout<<"ERROR: INVALID FILE FORMAT "<<buffer<<endl;
        return;
    }
    cout<<"MP3 tag reader and editer for ";
    cout<<buffer;
    obj->ver[0]=fgetc(fptr);                    //reading the version info
    obj->ver[1]=fgetc(fptr);
    cout<<"v"<<(int)(obj->ver[0])<<endl;
    cout<<"---------------------------------------------------------------------"<<endl;
    obj->flags=fgetc(fptr);
    fseek(fptr,10,SEEK_SET);        //skipping the first 10 bytes
    
    read(&obj->buffer,fptr);                    //reading the title tag
    cout<<"Title: "<<obj->buffer<<endl;
    delete obj->buffer;                                 //deleting the buffer to free the memory to avoid memory leak
    read(&obj->buffer,fptr);                    //reading the artist tag
    cout<<"Artist: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);                    //reading the album tag
    cout<<"Album: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);                    //reading the year tag
    cout<<"Year: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);                    //reading the genre tag
    cout<<"Genre: "<<obj->buffer<<endl;
    delete obj->buffer;
    read(&obj->buffer,fptr);                    //reading the comment tag
    cout<<"comment: "<<obj->buffer<<endl;
    delete obj->buffer;
    cout<<"---------------------------------------------------------------------"<<endl;
    fclose(fptr);
}

//function to read the tags from the file
void read(char **output,FILE *fptr){
    char buffer[6];             //buffer to store some datas of the file
    fgets(buffer,5,fptr);    //reading the first 4 bytes of the file
    fgetc(fptr),fgetc(fptr),fgetc(fptr);    //skipping the next 3 bytes
    int size=0;                 //variable to store the size of the tag
    char *ptr=(char *)&size;    //pointer to the lsb of thesize variable
    for(int i=0;i<4;i++){
        ptr[i]=fgetc(fptr);    //reading the size of the tag
    }
    *output=new char[size+1];       //allocating memory
    fread(*output,1,size-1,fptr);   //reading the tag
    (*output)[size-1]='\0';         //adding null character at the end of the tag
}


//function to edit the tags in the mp3 file
void edit(int argc, char **argv){
    if(argv[2][0]!='-'){                                //checking if the argument is valid
        cout<<"ERROR: INVALID ARGUMENT"<<endl;
        return;
    }
    char *ptr=strstr(argv[4],".");      //pointer to the occurance of '.' in the file name
    if(ptr&&strcmp(ptr,".mp3")){                    //checking if the file name consists of .mp3 extension
        cout<<"ERROR: File must be <.mp3> type"<<endl;
        return;
    }
    FILE *fptr=fopen(argv[4],"r");          //opening the file in read mode
    if(fptr==NULL){                                             //checking if the file is opened successfully
        cout<<"ERROR: unable open the "<<argv[4]<<" File"<<endl;
        return;
    }
    char buffer[11];        //buffer to store some datas of the file
    unsigned int size=0,skip=0;     //variable to store the size of the tag and the number of tags to skip
    ptr=(char *)&size;              //pointer to the lsb of the size variable
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
            cout<<"ERROR: INVALID ARGUMENT"<<endl;   //if the argument is invalid just return
            return;
    }
    for(int i=0;i<skip;i++){        //skipping the tags
        cpy_tag(fptr,fptr2);
    }
    fread(buffer,1,4,fptr);     //reading the 4 bytes from the file to the buffer
    fwrite(buffer,1,4,fptr2);       //writing the 4 bytes to the temp file
    for(int i=0;i<3;i++){
        fputc(fgetc(fptr),fptr2);    //writing the next 3 bytes to the temp file
    }
    for(int i=0;i<4;i++){
        ptr[i]=fgetc(fptr);     //reading the size of the tag
    }
    fseek(fptr,size-1,SEEK_CUR);    //skipping the tag
    size=strlen(argv[3])+1;
    fwrite(&size,4,1,fptr2);    //writing the size of the new tag
    fwrite(argv[3],1,size-1,fptr2); //writing the new tag
    cpy_remaining(fptr,fptr2);  //copying the remaining tags
    fclose(fptr);       //closing the file
    fclose(fptr2);      //closing the temp file
    remove(argv[4]);    //removing the original file
    rename("temp.mp3",argv[4]);   //renaming the temp file to the original file
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
