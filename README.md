# count_vowels
Count Vowels - Simple C Program - But not simple in anyway possible.

Basically this program is not simple and involves deep understanding of the multi-threaded concepts, file locking mechanisms of unix and inter-process communication techniques.

First things first --  would be to just read a file (Path specified in command line) and return number of vowels.

count_vowels:\
Open specified file and get the file Pointer.\
Check if file Pointer is not null.\
Get each character from file using fgetc (FILE GET CHARACTER) until EOF (End Of File macro) and process the obtained character in switch case statement.\
We have to increment the count for cases 'a', 'e', 'i', 'o', u and 'A', 'E', 'I', 'O', 'U' (the problem statement skipped mentioning Uppercase- seemed intentional).

1. We create a fairly small file and test this.
2. It probably works too.

But on a grand scheme of things, even if the developer misses this subtle uppercase information, there are so many other things to focus on to evaluate the assignment.


Now, we address the REAL PROBLEM:\
\
Imagine this file is in a database and there are multiple users trying to access the same file -- read, modify, write actions.\
The count_vowels will invariably produce wrong results at the end of the program.

It is simply because the data that is in the file is stored in memory, and memory is a resource.
When the program count_vowels is running and multiple threads try to access the same resource, we end up in a data race condition and the end result becomes ambiguous.\
This would mean we need some kind of synchronisation/ mutual exclusion to ensure good results (i.e. when we read the file no one modifies/writes to it).

LOCKING:\
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
 2. When a process acquires a lock on a file, another process can still gain access to the file by directly using the system call, this means that the process used the system call which is not a design choice (i.e. Non-cooperative way).
 3. Since Problem Statements recommends following POSIX Convention combined with unix philosophy, we are not left with many choices, flock and fcntl seem valid.
 4. The problem with flock() is that when the process is forked the locks are also copied along with it (because that is what fork means really). This would mean there will not be any synchronization or concurrency among the child and parent processes.
 5. To overcome this trouble, we have fcntl() that allows us to lock files (UNIX philosophy, portable and following POSIX Convention).
 6. There is **another design constraint** that we need to know which is all processes must use fcntl to acquire locks because it may be possible that other processes use other system api such as flock() to get locks and it is possible that they get that lock. This will also result in data race.
 7. Quick digression -- note that manual page will have indexed fcntl(2) and flock(2). 2 indicates they are system calls.
 
 Now, we address scalability and handling large files:
 
We could potentially create multiple threads based on the size of the files and read them in parallel and return the end result of sum of vowels of all threads, this would mean that very large files can be processed quicker than you would do it sequentially.\
fcntl() allows byte locks, where we set the start position and length -- (along with whence -- Reference position). We could obtain a read lock and then read those bytes and return to the main process which waits for the threads to complete the work.\
But for small files, we just need one worker thread. Deciding on number (tuning) of worker threads and where to create more threads seems beyond the scope of the assignment.

FINDING THE FILE:\
The simple C states that the fileName will be provided as a command line argument. This doesn't explicitly state that the absolute path will be a part of the fileName. File systems start at the root folder for almost all unix systems. Files are n-ary trees data structure wise. A binary tree has utmost 2 children. An n-ary would have n children possible. To traverse a tree we have bfs and dfs. (Preorder, postorder and inorder traversal). Thus we need some kind of a file tree walk as files and directories are stored as trees, if we wanted to do this as a separate assignment. We have nftw(3) supporting file tree walk, but we will not be using this as this directly implements the search. We will have to perform bfs from the root to reach the possible file. To read a directory and get the possible list of directories we require readdir(3). 

Having said this, we will assume that the absolute path is available to us as input and proceed. As the evaluator thinks, I should be focusing on parsing the file not the implementation of finding the file.

TYPE OF FILE:\
The input file could be of any format as specfied in the assignment, this would mean that the file can be text file; it could be binary format also.
Processing of text files are different from binary files as the text files store data in chars and binary store data in bits. In text file format each character's ASCII will be stored.

Also Text file has EOF entry which is a macro that returns -1 in the end of the file, but this is not the case for binary file.\
Hence it is better if we used open(2) for opening the file, read(2) for reading the file and close(2) for closing the file.\
Use lseek() to change the position of file descriptor. Use fstat() to determine the size of the file.

We also assume that the size requested to read by a single thread is **less than SSIZE_MAX macro** defined which is approximately 2GB in **32 BIT SYSTEMS**. If this is larger then we assume that the number of threads we use is significantly large and well handled by threads so that we never need more than 2Gig. I have made some attempt at explaining this in the code. I have a 64 bit system.

So, I introduced multi-threaded using advisory locks and found that for a large file such as 2.4Gb of data it took around 4 seconds to parse with 20 threads. The time did not improve much after that because of the overhead cost of creating the thread and copying and processing the data was not fast enough to offset the raw computation power of the threads already in place. The results were consistent when I increased the number of threads, which is something very important to test i.e.(NUM_THREADS set to 12 should result in the same vowels count as NUM_THREADS set to 20).

The NUM_THREADS is a macro that can be changed in the code.

Also, I have used eclipse IDE for development, I have also set the compilation flag _FILE_OFFSET_BITS=64 to support proper processing of large files.
I am also assuming that the file we parse supports lseek().

MMAP VS MALLOC: *[IMPORTANT]*

Final thing to ponder is why use read() and not mmap().

Most important thing to know about mmap() is that reading from and writing to a memory mapped file avoids the excessive copy that occurs when using the read() or write() system calls, where the data must be copied to and from a user space buffer.

mmap() is very fast because it has no context switch overhead or doesn't incur any system call.

But, the problem is mmap() reserves the address space for the entire page, this would mean that in 32 bit systems with a 32 bit address space, a very large number of memory mapps can result in fragmentation of the address space very soon. Though, this problem is less obvious in 64 bit systems. Our goal was to create a portable code, hence I have avoided mmap() in the implementation.

A small files such as 10 bytes will have serious issue of memory wasted. This is because a 10 byte mapping can still cost a reservation of 4089 bytes, this difference of memory is called slack space and it is wasted in case of mmap(). this is because the basic granularity of the mmap is in Pages, so even if a file is 10 bytes a full page is reserved. 

I also tried (implementation)creating an mmap in the beginning of worker thread and unmap in the end of the worker thread's code and have the implementation/code as a local copy not in git hub, I did not see any significant improvement in terms of speed, what I did was to do an mmap until integral multiples of 4089 bytes i.e. if file size to read for the worker thread is k bytes I would do an integral multiple of 4089 until it reaches k or less than k and the rest of the bytes in char and read it using read and lseek. This did not improve the speed at all, infact it was the same 4 seconds to process it. If you think this code is more correct I can upload that to git hub. Please note that the file size is N bytes having 20 worker threads mean each worker thread will read n/20 bytes which is  k here.

Another question that might come is why not mmap the whole file and have multiple threads read, well this could improve the speed but I was worried about using the whole address space for large files (I have not tried this idea).

I also happened to fumble upon one of these links which was replied by Linus Trovald himself on mmaps()
https://marc.info/?l=linux-kernel&m=95496636207616&w=2

**Quote Begin Linus Trovald:**

People love mmap() and other ways to play with the page tables to
optimize away a copy operation, and sometimes it is worth it.

HOWEVER, playing games with the virtual memory mapping is very expensive
in itself.  It has a number of quite real disadvantages that people tend
to ignore because memory copying is seen as something very slow, and
sometimes optimizing that copy away is seen as an obvious improvment. 

Downsides to mmap:
 - quite noticeable setup and teardown costs. And I mean _noticeable_.
   It's things like following the page tables to unmap everything
   cleanly. It's the book-keeping for maintaining a list of all the
   mappings. It's The TLB flush needed after unmapping stuff. 
 - page faulting is expensive. That's how the mapping gets populated,
   and it's quite slow. 
   
**Quote End Linus Trovald.**


There are other things that I can discuss but due to time constraints we are leaving at that here.








