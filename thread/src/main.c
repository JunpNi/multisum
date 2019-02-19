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
void thread_sum(void* arg);
int transToInt(char * str);
long transToLong(char * str);
int main() {
    int i,j;
    double t1,t2;
    pthread_t *pthread_id = NULL;
    long long result = 0;
    long long outResult[20];
    char _MAX[20];
    char _N[20];
    //目录信息
    DIR* directory;
    struct dirent * ptr;
    char directBuffer[80]; // 当前目录路径
    char fileDir[100];     // txt路径
    char outFileDir[100];  // 输出路径
    char outTxtDir[100];

    //获取当前路径 测试样例在当前路径/testfile/xx.txt
    getcwd(directBuffer, sizeof(directBuffer));
    strcat(outFileDir,directBuffer);
    strcat(directBuffer,"/testfile/");
    strcat(outFileDir,"/outfile/");
    if(!(directory=opendir(directBuffer))){
        printf("路径进入失败...");
        exit(1);
    }
    puts(directBuffer);
    j = 0;
    while ((ptr=readdir(directory)) != NULL){
        if(ptr->d_type == 8 && (strstr(ptr->d_name,".txt")!=NULL)){
            fileDir[0] = '\0';
            outTxtDir[0] = '\0';
            strcat(fileDir,directBuffer);
            strcat(fileDir,ptr->d_name);
            strcat(outTxtDir,outFileDir);
            strcat(outTxtDir,ptr->d_name);
            puts(ptr->d_name);
            puts(fileDir);
        } else
            continue;
        if(!freopen(fileDir,"r",stdin)){
            printf("文件打开失败...");
            exit(1);
        }

        // 通过重定向输入求和上限 MAX 和线程数 N
        scanf("%s",_N);
        scanf("%s",_MAX);
        fclose(stdin);
        //得到 MAX 和 N 的值
        MAX = transToLong(_MAX);
        N = transToInt(_N);

        //申请线程id和求和记录数组
        pthread_id = (pthread_t*)malloc(N * sizeof(pthread_t));
        sum = (long long*)malloc(N * sizeof(long long));
        AVE = MAX/N;

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
        printf("求和结果：%lld\n",result);
        printf("求和时间：%f\n",t2 - t1);
        puts(outTxtDir);
        //把结果记录到输出数组
        outResult[j] = result;
        j++;
        // 重置result 并释放内存
        result = 0;
        free(pthread_id);
        free(sum);
    }
    closedir(directory);
    if(!(directory=opendir(directBuffer))){
        printf("路径进入失败...");
        exit(1);
    }
    j = 0;
    while ((ptr=readdir(directory)) != NULL) {
        if (ptr->d_type == 8 && (strstr(ptr->d_name, ".txt") != NULL)) {
            outTxtDir[0] = '\0';
            strcat(outTxtDir, outFileDir);
            strcat(outTxtDir, ptr->d_name);
        } else
            continue;
        // 定向输出到outfile内容
        freopen(outTxtDir,"w",stdout);
        printf("%lld\n",outResult[j]);
        j++;
    }
    closedir(directory);
    return 0;
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
