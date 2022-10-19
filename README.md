# count_vowels
Count Vowels - Simple C Program - But not simple in anyway possible.

Basically this program is not simple and involves deep understanding of the multi-threaded concepts, file locking mechanisms of unix and inter-process communication techniques.

First things first --  would be to just read a file (Path specified in command line) and return number of vowels.

count_vowels:
Open specified file and get the file Pointer.\
Check if file Pointer is not null.\
Get each character from file using fgetc (FILE GET CHARACTER) until EOF (EOF) and process the obtained character in switch case statement.\
We have to increment the count for cases 'a', 'e', 'i', 'o', u and 'A', 'E', 'I', 'O', 'U' (the problem statement skipped mentioning Uppercase- seemed intentional).\
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
The Problem clearly states that we need to have to "think unix philosophy" and that the program would be run in a linux environment for testing.
One has to be very careful and read this twice because this opens up a lot of decisions that we must make in our program.

Unix systems generally offer two types of lock to a file.
1. Mandatory Locks
2. Advisory Locks.

Design choice to AVOID MANDATORY LOCKS:
1. Mandatory lock is a type of lock that enforces mutual exclusion amongst processses.
2. To make the code portable, we won't use mandatory locks as all unix based OSs will not support mandatory locks. Quick digression -- Windows uses Mandatory lock and is well implemented.
3. Furthermore, Mandatory locks can create the well known problem of denial of service attack -- If a process that holds the mandatory lock crashes and doesn't return the lock there is no way we could get access to the locked resource. This is a serious problem if implemented in a database.
4. Though Linux calls it Mandatory lock, it seems like one could play around with the setgid bit and can find ways to get access to the said file. This makes mandatory locks unreliable.
5. Linux Manual Page says "Warning: the Linux implementation of mandatory locking is unreliable.  See BUGS below.  Because of these bugs, and the fact
   that the feature is believed to be little used, since Linux 4.5, mandatory locking has been made an optional feature, governed by
   a configuration option (CONFIG_MANDATORY_FILE_LOCKING). **This is an initial step toward removing this feature completely.**
   
 ADIVSORY_LOCKS:
 1. Advisory locks are co-operative locks, meaning, they are not enforced by the system.
 2. When a process acquires a lock, another process can still have access by directly using the system call, this means that the process used the system call which is not a design choice (i.e. Non-cooperative way).
 3. Since Problem Statements recommends following POSIX Convention combined with unix philosophy, we are not left with many choices, flock and fcntl seem valid.
 4. The problem with flock is that when the process is forked the locks are also copied along with it (because that is what fork means really). This would mean there will not be any synchronization or concurrency among the child and parent processes.
 5. To overcome this trouble, we have fcntl() that allows us to lock files (UNIX philosophy, portable and following POSIX Convention).
 6. There is **another design constraint** that we need to know which is all processes must use fcntl to acquire locks because it may be possible that other processes use other system api such as flock to get locks and it is possible that they get that lock. This will also result in data race.
 
 Now, we address scalability and handling large files:
 
 








