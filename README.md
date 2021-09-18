# Video_Club
Simple film recording system


A program that implements a film recording system with c. The process begins with reading an archive in which there is information about the films. It is important that the information in the file is stored in a very specific way.
The program is only able to edit this particular mode.

The program first reads the file and passes all the information to structures. During execution the changes to the records are made on the structures keeping them always sorted. By terminating the code all changes are stored back in the file so that it is up to date.
  
 In the menu there are the following procedures :

    - Insert
    - Delete(with id)
    - Search Title
    - Search Director( Prints all the director's films )
    - Print the whole list


Complie

    gcc -o v videoclub.c
    
Execute
     
     ./v
