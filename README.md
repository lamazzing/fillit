# fillit
Fillit is a variation of the game Tetris. 
The program takes a file as parameter which contains a list of Tetriminos (pieces) and arranges them to create the smallest square possible. The main goal is to find this smallest square in the minimal amount of time, despite an exponentially growing number of possibilities each time a piece is added.
This is the second version of the project, I decided to retry it with a friend (eferrand) using a more unusual method to write the algorithm. We decided to use bitwise shifting and operators to significantly improve the speed and reduce the number of functions and files (first project ~25 functions, this project 10 functions).  
Bitwise shifting is quite useful to clearly understand how data is stored in the memory, and how to use casting and bitwise operators to manipulate the variables.

usage:
```
$ ./fillit tetris_file
```
