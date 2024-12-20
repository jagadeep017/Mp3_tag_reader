MP3 Tag Editor
This project is a command-line tool for viewing and editing tags in MP3 files. It allows users to view and modify various tags such as Title, Artist, Album, Year, and Comment.

Usage
./a.out {options} filename<.mp3>

Options
-h: Display help information.
-v: View the tags in the MP3 file.
-e: Edit the tags in the MP3 file.
-t: Modify the Title tag.
-a: Modify the Artist tag.
-A: Modify the Album tag.
-y: Modify the Year tag.
-c: Modify the Comment tag.

Examples
Display help information:
./a.out -h

View tags in an MP3 file:
./a.out -v example.mp3

Edit the Title tag in an MP3 file:
./a.out -e -t "New Title" example.mp3

Files
fun.cpp  : Contains the implementation of the functions for viewing and editing MP3 tags.
fun.h    : Header file with function declarations and type definitions.
main.cpp : Contains the main function and handles command-line arguments.

Compilation:
To compile the project, use the makefile to get excutable file.
