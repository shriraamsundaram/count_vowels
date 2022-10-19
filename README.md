# count_vowels
Count Vowels - Simple C Program - But not simple in anyway possible.

Basically this program is not simple and involves deep understanding of the multi-threaded concepts, file locking mechanisms of unix and inter-process communication techniques.

First things first --  would be to just read a file (Path specified in command line) and return number of vowels.

count_vowels:
Open specified file and get the file Pointer.
Check if file Pointer is not null.
Get each character from file using fgetc (FILE GET CHARACTER) until EOF (EOF) and process the obtained character in switch case statement.
We have to increment the count for cases 'a', 'e', 'i', 'o', u and 'A', 'E', 'I', 'O', 'U' (the problem statement skipped mentioning Uppercase- seemed intentional).
But on a grand scheme of things, even if the developer misses this subtle uppercase information, there are so many other things to focus on to evaluate the assignment.

1. We create a fairly small file and test this.
2. It probably works too.


Now, we address the REAL PROBLEM:

Imagine this file is in a database and there are multiple users trying to access the same file -- read, modify, write actions.
The count_vowels will invariably produce wrong results at the end of the program. 

It is simply because the data that is in the file is stored in memory, and memory is a resource.
When the program count_vowels is running and multiple threads try to access the same resource, we end up in a data race condition and the end result becomes ambiguous.
This would mean we need some kind of synchronisation/ mutual exclusion to ensure good results (i.e. when we read the file no one modifies/writes to it).

LOCKING:
The Problem clearly states that we need to have "UNIX school of thought" and that the program would be run in a linux environment for testing.
One has to be very careful and read this twice because this opens up a lot of decisions that we must make in our program.

Unix systems generally offer two types of lock to a file.
1. Mandatory Locks
2. Advisory Locks.

Design choice to AVOID MANDATORY LOCKS:





