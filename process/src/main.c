#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/time.h>

struct shared_data{
    int written; // 可写标识符，0为只读，1为可写
    long long result;
};
// 输入的范围
long long MAX;
int N;
long AVE;
long long sum;

long long child_sum(int index);
long long process_sum();
double get_time();
int transToInt(char * str);
long long transToLong(char * str);
int main() {
    int i;
    char _MAX[20];
    char _N[20];
    long long SUM_RESULT;
    // 目录信息
    char out_file_name[100] = "\0";
    char input_file_name[100] = "\0";
    char ROOT_PATH[100] = "\0";
    // 获取当前路径
    getcwd(ROOT_PATH, sizeof(ROOT_PATH));
    strcat(input_file_name, ROOT_PATH);
    strcat(input_file_name, "/input.txt");
    strcat(out_file_name, ROOT_PATH);
    strcat(out_file_name, "/output.txt");
    // 重定向标准输入
    if(!freopen(input_file_name, "r",stdin)){
        printf("文件打开失败！\n");
        exit(1);
    }
    scanf("%s",_N);
    scanf("%s",_MAX);
    fclose(stdin);
    // 将文件中的内容解析成数字型
    MAX = transToLong(_MAX);
    N   = transToInt(_N);
    AVE = MAX/N;
    SUM_RESULT = process_sum();
    if(!freopen(out_file_name,"w",stdout)){
        printf("文件打开失败！\n");
        exit(1);
    }
    printf("%lld\n",SUM_RESULT);
    fclose(stdout);
    return 0;
}

long long process_sum()
{
    int i;
    double t1,t2;
    // 当前进程所处的分段编号
    int index = 0;
    int stat_val;
    pid_t pid;
    long long sum_result;
    // 设置信号量
    signal(SIGCHLD, SIG_IGN);
    // 共享内存指针
    int shmid;
    void * shm = NULL;
    struct shared_data *shared = NULL;
    // 初始化共享内存
    if((shmid = shmget((key_t)999, sizeof(struct shared_data), 0666 | IPC_CREAT)) == -1){
        printf("共享内存创建失败！\n");
        exit(1);
    }
    // 设置共享内存
    shm = shmat(shmid, (void *)0, 0);
    if(shm == (void *) - 1)
        printf("shmat fail !\n");
    shared = (struct shared_data *)shm;
    shared->written = 1;
    shared->result  = 0;
    t1 = get_time();
    // 循环创建进程
    for( i=0;i < N;i++ ){
        pid = fork();
        if(pid == 0){
            index = i;
            break;
        }
    }
    // 子进程执行
    if(pid == 0){
        /*如果可写标识符为0，则为不可写等待标识符改写成1*/
        while (shared->written == 0)
            break;
        /*可写之后，立刻将标识符改写成0，使其他进程不可写*/
        shared->written = 0;
        child_sum(index);
        shared->result += sum;
        /*写完以后，将标识符改为1，使其他进程可写*/
        shared->written = 1;
        /*求和结束后将共享内存分离*/
        if(shmdt(shm) == -1){
            printf("shmdt FAILED !\n");
        }
        exit(0);
    }
    else if(pid == -1){
        printf("fork 失败！\n");
        exit(1);
    }
    // 父进程执行
    else{
        //waitpid(pid, &stat_val, 0);
        wait(NULL);
        t2 = get_time();
        sum_result = shared->result;
        printf("\n求和上限为：%lld\n",MAX);
        printf("进  程数为：%d\n",N);
        printf("求和结果为：%lld\n",sum_result);
        printf("求和时间为：%f\n\n",t2 - t1);
        return sum_result;
    }
}

long long child_sum(int index)
{
    int n = index;
    long long start = n*AVE + 1;
    long long end = start + AVE - 1;
    long long i;
    sum = 0;
    if(n == N-1){
        end = MAX;
    }
    for(i=start; i <= end; i++){
        sum = sum + i;
    }
}

double get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec/1000000.0;
}
int transToInt(char* str)
{
    int result;
    char dest[20];
    strncpy(dest,str+2,17);
    result = atoi(dest);
    return result;
}
long long transToLong(char* str)
{
    long long result;
    char dest[20];
    strncpy(dest,str+2,17);
    result = atoll(dest);
    return result;

}
