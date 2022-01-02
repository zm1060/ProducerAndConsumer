#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    //windows下用windows.h
#include <pthread.h>   // 实现多线程的头文件
#include <semaphore.h> // 实现信号量定义的头文件
#include <time.h>

#define producerNumber 2   // 生产者的数目
#define consumerNumber 2   // 消费者的数目
#define M 20 // 车位




sem_t empty_sem; // 信号量的数据类型为结构sem_t，它本质上是一个长整型的数,同步信号量， 当满了时阻止停车
sem_t full_sem;   // 同步信号量
pthread_mutex_t mutex; // 互斥信号量， 一次只有一个线程访问缓冲

int count = 20;

/* 生产者方法 */
void *product1()
{
    while(1)
    {
        // 用sleep的数量可以调节生产和消费的速度，便于观察
        sleep(1 + rand()%3);
        if(count<=0){
            printf("[入口1]:车位满,请等待\t");
        }
        sem_wait(&empty_sem); // 满不放；
        pthread_mutex_lock(&mutex); // 实现互斥
        printf("[入口1]:来车请进\t");
        count--;
        printf("[空余车位]: %d\n",count);
        pthread_mutex_unlock(&mutex);
        sem_post(&full_sem);
    }
}


/* 生产者方法 */
void *product2()
{

    while(1)
    {
        // 用sleep的数量可以调节生产和消费的速度，便于观察
        sleep(1 + rand()%3);
        if(count<=0){
            printf("[入口2]:车位满,请等待\t");
        }
        sem_wait(&empty_sem); 
        pthread_mutex_lock(&mutex); // 实现互斥
        printf("[入口2]:来车请进\t");
        count--;
        printf("[空余车位]: %d\n",count);
        pthread_mutex_unlock(&mutex);
        sem_post(&full_sem);
    }
}

/* 消费者方法 */
void *prochase1()
{

    while(1)
    {
        // 用sleep的数量可以调节生产和消费的速度，便于观察
        sleep(1 + rand()%3);

        sem_wait(&full_sem); // 空不取
        pthread_mutex_lock(&mutex); // 实现互斥
        printf("[出口1]:请缴费出场\t");
        count++;
        printf("[空余车位]: %d\n",count);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_sem);
    }
}

/* 消费者方法 */
void *prochase2()
{

    while(1)
    {
        // 用sleep的数量可以调节生产和消费的速度，便于观察
        sleep(1 + rand()%3);

        sem_wait(&full_sem); // 空不取
        pthread_mutex_lock(&mutex); // 实现互斥
        printf("[出口2]:请缴费出场\t");
        count++;
        printf("[空余车位]: %d\n",count);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_sem);
    }
}

int main()
{
    srand((unsigned)time(NULL));
    pthread_t id1[producerNumber]; // 声明生产者线程的ID数组
    pthread_t id2[consumerNumber]; // 声明消费者线程的ID数组
    int i;
    int ret1[producerNumber];
    int ret2[consumerNumber];

    // 初始化同步信号量
    int ini1 = sem_init(&empty_sem, 0, M);
    // 原型为:extern int sem_init __P ((sem_t *__sem, int __pshared, unsigned int __value));
    // 第一个参数：sem为指向信号量结构的一个指针；
    // 第二个参数：pshared==0确保只能为当前的进程的所有线程共享
    // 第三个参数：value为信号量的初始值
    int ini2 = sem_init(&full_sem, 0, 0);
    if(ini1 && ini2 != 0) //初始化失败
    {
        printf("sem init failed \n");
    }

    //初始化互斥信号量的函数pthread_mutex_init();
    int ini3 = pthread_mutex_init(&mutex, NULL);
    // 抽象的锁类型的结构；
    //函数原型：int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr); 以动态方式创建互斥锁
    // 第一个参数：为指向互斥信号量结构的一个指针
    // 第二个参数： 指定了新建互斥锁的属性，NULL表明使用默认的互斥锁属性（快速互斥锁）
    if(ini3 != 0)
    {
        printf("mutex init failed \n");
    }
    //创建生产者线程
    ret1[0] = pthread_create(&id1[0], NULL, product1, NULL);
    ret1[1] = pthread_create(&id2[1], NULL, product2, NULL);
    if(ret1[0] && ret1[1] != 0)
    {
        printf("product%d creation failed \n", i);
    }
    //创建消费者线程
    ret2[0] = pthread_create(&id2[0], NULL, prochase1, NULL);
    ret2[1] = pthread_create(&id2[1], NULL, prochase2, NULL);
    if(ret2[0] && ret2[1] != 0)
    {
        printf("prochase%d creation failed \n", i);
    }

    //销毁线程
    for(i = 0; i < producerNumber; i++)
    {
        pthread_join(id1[i],NULL);
    }

    for(i = 0; i < consumerNumber; i++)
    {
        pthread_join(id2[i],NULL);
    }

    exit(0);
}
