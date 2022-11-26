#include "types.h"
#include "user.h"
#include "proc_stat.h"
// #include "proc.h"
void testing()
{
    volatile int ro = 0;
    for (volatile int i = 0; i < 3  00000000; i++)
    {
        ro++;
        ro--;
    }
}
// int main()
// {
//     // for(int i = 0; i < 4; i++)
//     {
//         int child = fork();
//         if (child == 0)
//         {
//             SetPriority(1);
//             //sleep(100);
//             testing();
//             struct proc_stat a;
//             getpinfo(&a, 4);
//             exit();
//         }
//         else
//         {
//             child = fork();
//             if (child == 0)
//             {
//                 SetPriority(3);
//                 testing();
//                 struct proc_stat a;
//                 getpinfo(&a, 5);
//                 exit();
//             }
//             else
//             {
//                 child = fork();
//                 if (child == 0)
//                 {
//                     for (volatile int j = 0; j < 4; j++)
//                         testing();
//                     // sleep(100);
//                     exit();
//                 }
//                 else
//                 {

//                     child = fork();
//                     int t1, t2;
//                     if (child == 0)
//                     {
//                         testing();
//                         //sleep(1400);
//                         SetPriority(90);
//                         exit();
//                     }
//                     else
//                     {
//                         ;
//                         int pid = waitx(&t1, &t2);
//                         printf(0, "t1=%d ,t2=%d pid=%d\n", t1, t2, pid);
//                     }
//                     int pid = waitx(&t1, &t2);
//                     printf(0, "t1=%d ,t2=%d pid=%d\n", t1, t2, pid);
//                     ;
//                 }
//                 int t1, t2;
//                 int pid = waitx(&t1, &t2);
//                 printf(0, "t1=%d ,t2=%d pid=%d\n", t1, t2, pid);
//                 ;
//             }

//             int t1, t2;
//             int pid = waitx(&t1, &t2);
//             printf(0, "t1=%d ,t2=%d pid=%d\n", t1, t2, pid);
//             ;
//         };
//     }
//     exit();
// }
// void test()
// {
//     for (int i = 0; i < 50000000; i++)
//         ;
// }
int main()
{
    for (int i = 0; i < 4; i++)
    {
        int child = fork();
        if (child == 0)
        {
            switch (i)
            {
            case 1:
                testing();
                break;
            case 2:
                testing();
                break;
            case 3:
                testing();
                break;
            case 4:
                testing();
                break;
            default:
                testing();
                break;
            }
            exit();
        }
        else
        {
            ;
            // int t1, t2;
            // int pid = waitx(&t1, &t2);
            // printf(0, "t1=%d ,t2=%d pid=%d\n", t1, t2, pid);
        }
    }
    exit();
}