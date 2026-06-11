#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
/*结构定义和有序链表实现*/
typedef struct 
{
    int pid;     // 进程标志符
    int pri;     // 进程优先级
    int cputime; // cpu 时间统计
    int alltime; // 运行所需时间
    int state;   // 进程状态
} PROC;          // 进程控制块结构
typedef struct Node
{
    PROC info;         // 进程控制块信息
    struct Node *next; // 下一个结点指针
} Node;                // 链表结点结构
typedef Node *LinkList;
// 初始化有序链表，返回一个链表的头结点
Node *InitSortedList()
{
    Node *head = (Node *)malloc(sizeof(Node));
    head->next = NULL;
    return head;
}
// 按优先级高低次序，把进程 Proc 插入到有序链表 L 中
void InsertSortedList(LinkList L, PROC Proc)
{
    Node *pre = L;
    Node *p = pre->next;
    Node *NewProc = (Node *)malloc(sizeof(Node));
    NewProc->info = Proc;
    while (p != NULL && Proc.pri <= (p->info).pri)
    { // 寻找合适的插入点
        pre = p;
        p = p->next;
    }
    if (p == NULL)
    {
        pre->next = NewProc;
        NewProc->next = NULL;
    }
    else
    {
        NewProc->next = p;
        pre->next = NewProc;
    }
}
// 摘取有序链表的第一个结点（这个结点从链表中移出）
int GetHeadElement(LinkList L,PROC *p)
{
     Node *pp; 
    if (L->next == NULL)
        return -1;
    else
    {
        *p = ((L->next)->info);
        pp=L->next;
        L->next = (L->next)->next;
        free(pp);
        return 0;
    }
}

// 打印进程控制块信息
void PrintPROC(PROC *Proc)
{
    if (Proc == NULL)
    {
        printf("\nProcess Point is NULL!");
    }
    else
    {
        printf("\nPid: %d", Proc->pid);
        printf(" Pri: %d", Proc->pri);
        printf(" CPUTime: %d", Proc->cputime);
        printf(" AllTime: %d", Proc->alltime);
        printf(" State: %d", Proc->state);
    }
}
// 打印有序链表内容
void PrintSortedList(LinkList L)
{
    Node *p = L->next;
    if (p == NULL)
        printf("\nThis SortedList Has No Node !\n");
    else
    {
        printf("\nShow The Node(s) Of The SortedList: ");
        while (p != NULL)
        {
            PrintPROC((PROC *)&(p->info));
            p = p->next;
        }
    }
}

/******以上部分可以略看，知道每个函数的功能即可*********/
/***************进程调度程序，仔细理解*********************/
int main()
{
    int i, count;
    int ret;
    PROC p[5];
    PROC pp;
    LinkList L = InitSortedList(); /* 初始化一个有序链表 */
    /*产生五个随机不同优先级的进程*/
    for (i = 0; i < 5; i++)
    {
        p[i].pid = i + 1;
        p[i].pri = rand() % 20;                    // 优先级最高 20
        p[i].cputime = rand() % 30;                // cpu 时间统计限定在 30 以内
        p[i].alltime = p[i].cputime + rand() % 20; // 运行完成所需全部时间
        p[i].state = 1;
    }
    /* 插入进程, 产生就绪队列 */
    for (i = 0; i < 5; i++)
        InsertSortedList(L, p[i]);
    PrintSortedList(L); /* 打印就绪队列 */
    count = 1;          // 调度次数计数器
    while (L->next != NULL)
    {                           // 就绪队列不空，调度进程运行
        ret = GetHeadElement(L,&pp); // 取得优先级最高的进程
        if(ret < 0)
            break;
        printf("\n/*******Schedule #%d******************/", count);
        printf("\nProcess %d info before running:", pp.pid);
        PrintPROC(&pp);
        pp.pri = (pp.pri) - 2; // 每运行一次优先级降低 2
        if (pp.pri < 0)
        {
            pp.pri = 0;
        }
        pp.cputime = pp.cputime + 4;  // 每运行一次 CPU 时间增加 4
        if (pp.cputime >= pp.alltime) // 运行完成，退出
        {
            printf("\nProcess %d is over!\n", pp.pid);
        }
        else
        { // 还没有运行完成，继续插入就绪队列，等待调度
            printf("\nProcess %d info after running:", pp.pid);
            PrintPROC(&pp);
            InsertSortedList(L, pp);
        }
         PrintSortedList(L); /* 打印就绪队列 */
        count++;
    }
    free(L);
    return 0;
}