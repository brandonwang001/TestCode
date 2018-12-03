#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/time.h>
#include<pthread.h>
#include<vector>
#include <atomic>

#include<errno.h>  //错误
using namespace std;
#define MAXLINE 128
#define SER_PORT 44556


std::atomic<int64_t> succ_nums(0);
std::atomic<int64_t> fail_nums(0);

int64_t getCurrentTime()      //直接调用这个函数就行了，返回值最好是int64_t，long long应该也可以
{
  struct timeval tv;
  gettimeofday(&tv,NULL);    //该函数在sys/time.h头文件中
  //return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
  return tv.tv_sec;
}


void * count(void * args)
{
  int64_t start_time = getCurrentTime();
  
  int64_t last_succ_nums = 0;
  
  while(1)
  {
    if((succ_nums - last_succ_nums) > 10000 && (getCurrentTime()-start_time) > 1)
    {
      int64_t succ_qps = succ_nums / (getCurrentTime()-start_time);
      int64_t fail_qps = fail_nums / (getCurrentTime()-start_time);
      printf("succ_qps=%lu, fail_qps=%lu\n", succ_qps, fail_qps);
      last_succ_nums = succ_nums.load();
      
    }
  }
}

void * sender(void *args)
{
  struct sockaddr_in servaddr;
  bzero(&servaddr,sizeof(servaddr));
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  servaddr.sin_family = AF_INET; 
  inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
  servaddr.sin_port = htons(SER_PORT);
  
  if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
  {
    printf("connet error:%s\n",strerror(errno)); 
  }
 
  while(1)
  {
    string buf = "hello world";
    write(sockfd,buf.data(),buf.size());
    char buftemp[1024];
    int n = read(sockfd,buftemp,1024);
    buftemp[n] = '\0';
    string sBuf(buftemp); 
    if(buf == sBuf)
    {
       succ_nums.fetch_add(1);      
    }
    else
    {
      fail_nums.fetch_add(1);
    }
  }
  close(sockfd); 
}

int main(int argc,char *argv[]){

    pthread_t count_pid;
    if(pthread_create(&count_pid, NULL, count, (void *)NULL) != 0)
    {
      return -1;
    }
  
    std::vector<pthread_t> pids;
    int thread_nums = 10;
    pids.resize(thread_nums); 
    for(int i=0; i<thread_nums; i++)
    {
      if(pthread_create(&pids[i], NULL, sender, (void *)NULL) != 0)
      {
        printf("error : %s", "Fail to create pthread");
        return -1;
      }
    }

    pthread_join(count_pid, NULL);
    for(int i=0; i< thread_nums; i++)
    {
      pthread_join(pids[i], NULL);
    }   
    return 0;
}

