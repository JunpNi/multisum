#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
long MAX; // 整数范围
int N;   // 创建N个子线程求和
long AVE;

long long *sum = NULL;

double get_time();
long long THREAD_SUM();
void thread_sum(void* arg);
int transToInt(char * str);
long transToLong(char * str);
int main() {
    long long result;
    char _MAX[20];
    char _N[20];
    //目录信息
    char ROOT_PATH[100]        = "\0";   // 当前目录路径
    char input_file_path[100]  = "\0";     // 输入txt路径
    char output_file_path[100] = "\0";  // 输出路径

    // 当前路径ROOT_PATH
    // 计算数值从ROOTPATH/input.txt中导入
    // 输出到ROOTPATH/output.txt中
    getcwd(ROOT_PATH, sizeof(ROOT_PATH));
    strcat(input_file_path,ROOT_PATH);
    strcat(input_file_path,"/input.txt");
    strcat(output_file_path,ROOT_PATH);
    strcat(output_file_path,"/output.txt");
    if(!freopen(input_file_path,"r",stdin)){
        printf("文件打开失败...");
        exit(1);
    }
    // 通过重定向输入求和上限 MAX 和线程数 N
    scanf("%s",_N);
    scanf("%s",_MAX);
    fclose(stdin);
    // 得到 MAX 和 N 的值,得到 AVE
    MAX = transToLong(_MAX);
    N = transToInt(_N);
    AVE = MAX/N;
    result = THREAD_SUM();
    if(!freopen(output_file_path,"w",stdout)){
        printf("output.txt 文件打开失败！");
        exit(1);
    }
    printf("%lld\n",result);
    fclose(stdout);
    return 0;
}
long long THREAD_SUM()
{
    int i;
    long AVE;
    double t1,t2;
    long long result = 0;
    pthread_t *pthread_id = NULL;
    // 申请线程id和求和记录数组
    pthread_id = (pthread_t*)malloc(N * sizeof(pthread_t));
    sum = (long long*)malloc(N * sizeof(long long));
    // 计算开始时间
    t1 = get_time();

    // 创建N个子线程
    for(i=0;i<N;i++){
        pthread_create(pthread_id+i,NULL,thread_sum,i);
    }
    // 等待每一个线程结束，并把结果汇总
    for(i=0;i<N;i++){
        pthread_join(pthread_id[i],NULL);
        result += sum[i];
    }
    // 计算结束时间
    t2 = get_time();
    printf("\n求和上限：%ld\n",MAX);
    printf("线程数量：%d\n",N);
    printf("求和结果：%lld\n",result);
    printf("求和时间：%f\n\n",t2 - t1);
    free(pthread_id);
    free(sum);
    return  result;
}

double get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec/1000000.0;
}

void thread_sum(void* arg)
{
    int n = (int)arg;
    long long start = n*AVE + 1;
    long long end = start + AVE - 1;
    long long i;
    sum[n] = 0;
    if(n == N-1){
        end = MAX;
    }
    for(i=start; i <= end; i++){
        sum[n] = sum[n] + i;
    }
    pthread_exit(0);
}

int transToInt(char* str)
{
    int result;
    char dest[20];
    strncpy(dest,str+2,17);
    result = atoi(dest);
    return result;
}
long transToLong(char* str)
{
    long result;
    char dest[20];
    strncpy(dest,str+2,17);
    result = atol(dest);
    return result;

}
