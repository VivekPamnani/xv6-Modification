Waitx, rtime, ctime, wtime, etime - Implementation :

    When allocating process, ctime is set
    When process is exiting, etime is set
    Every time we get timer interrupt, ticks count is increased. This is done in trap.c.
    There we are also increasing the rtime of the current process (myproc() returns that)
    In waitx(int *wtime, int *rtime) we get wtime dynamically when requested. We use that
    wtime = (etime - ctime) - rtime
    Testing watix can be done by running “waitxTest” in prompt. File is “waitxTest..c”

First Come First Serve (FCFS)
    Iterate through table and find the process which is RUNNABLE with minimum
    Run that process chosen in first step
    Disabled yield() for FCFS
     Chosen pid instead of ctime because two processes can have the same ctime
    but pid are always different as kernel does pid++ every time it allocates a
process

Priority Based (PBS)

Set priority of a new process to default value 60. (Except that init(pid 1) &
sh(pid 2) have priority 0). Each process has priority variable in struct proc

 Iterate through table and find the value of highest priority i.e, the minimum
priority value set in struct proc

 Iterate again through the table and run all RUNNABLE processes which have
the same priority value in the struct proc as the value found in previous step

 The previous step ensures that processes which have the same priority are
executed in round robin fashion


Running:
        Single_CPU;
        test1 : 
            FCFS = 815 ; PBS = 768 ; MLFQ = 728 ; DEFAULT = 924 .

        test2 :
            FCFS = 1574 ; PBS = 1283 ; MLFQ = 1530 ; DEFAULT = 1417 .


Analysis : 

    -> Usually there should be no difference btw PBS and  