//FileName: multi_thread_example.cpp  
//Compile: g++ -lpthread -o multi_thread_example.out multi_thread_example.cpp   
//Run: ./multi_thread_example.out  
  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
  
void *function(void *arg);  
  
//定义共享锁  
pthread_mutex_t mutex;  
  
int main(int argc, char *argv[])  
{  
    int rc1,rc2;  
  
    char *str1="aaaaaaaaaaaaaaaaa";  
    char *str2="bbbbbbbbbbbbbbbbb";  
    pthread_t thread1,thread2;  
  
    //初始化共享锁   
    pthread_mutex_init(&mutex,NULL);  
  
    //创建线程1，创建后立刻运行  
    if(rc1 = pthread_create(&thread1,NULL,function,str1))  
    {  
        fprintf(stdout,"thread 1 create failed: %d\n",rc1);  
    }  
  
    //创建线程2，创建后立刻运行   
    if(rc2=pthread_create(&thread2,NULL,function,str2))  
    {  
        fprintf(stdout,"thread 2 create failed: %d\n",rc2);  
    }  
  
    //等待线程1和线程2运行结束；如果不加pthread_join函数，主线程会直接运行结束，从而导致子线程未运行完就也被结束。  
    pthread_join(thread1,NULL);  
    pthread_join(thread2,NULL);  
  
    return 0;  
}  
  
//线程函数   
void *function(void *arg)  
{  
    char *m;  
    m = (char *)arg;  
  
    //使用共享锁隔离关键区域；以下区域打印字符，如果不隔离的话，2个线程会交替打印出类似ababab...的字符串  
    pthread_mutex_lock(&mutex);  
  
    while(*m != '\0')  
    {  
        printf("%c",*m);  
        fflush(stdout);  
        m++;  
        sleep(1); /*模拟长时间处理情况*/  
    }  
    printf("\n");  
  
    //释放共享锁，以让其他线程可以继续执行该段代码  
    pthread_mutex_unlock(&mutex);  
}
