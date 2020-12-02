# cpu_scheduler

cpu_scheduler is a simple java program that illustrates the behavior of several schedulers for a user-entered list of processes(jobs), gets the average turnaround, response, wait times.

Available Schedulersare: 
1. FCFS(First COme, First Served)
2. SJF (Shortest Job First)
3. RR (Round Robin)

## Before Installation

It's java program, so you should be sure that 
* You have a working java runtime and development environments.
* CLASSPATH is correctly configured, including you working directory.

You may need to use command line to run it, start your command line then follow these steps to make sure your environment is valid:
1. Check java installed version:

        > java -version
    you should see this output, with possibly different version 

         java version "11.0.6" 
    if you get bad or unrecognised command, So java may not be installed or not configured, check the problem then come back here..

2. Check java compiler installed version:

        > javac -version
    you should get the same version number you have seen in step 1.
     if you get bad or unrecognized command, So java compiler may not be installed or not configured, check the problem then come back here..

## Installation

Install cpu_scheduler following these steps:

        1. Create a directory folder in which you need to install the program (D:\OS\sched)
            C:\WINDOWS> D:
            D:\> mkdir OS
            D:\> cd OS
            D:\OS> mkdir sched
            D:\OS> cd sched
            D:\OS\sched>
        2. Download the .zip file and extract it in the new created D:\OS\sched
        3. type D:\OS\sched> javac cpu_scheduler.java
        4. type D:\OS\sched> java Cpu_scheduler
        5. Follow what is displayed on the screen and see how your entered jobs are scheduled in these basic schedulers

## Contact

 Any suggestions, corrections, comments on
    [Ikhlas](mailto:ikhlasjaydi@gmail.com?subject=[GitHub]%15cpu%15Scheduling)
