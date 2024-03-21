//Calvin Brooks
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int threads;//holds threads, global because will be used in process function
int numberLimit;//program is done when number reaches numberCounter
int happyNumbers = 0;//globally represents tally of happy numbers  ( total - happyNumbers = sad numbers )
pthread_mutex_t lock;//mutex initiation and destruction, used for critical section with locks
pthread_t thd[4];//threads
  
//thread process finds happy numbers within evenly divided range based on thread amount
void* ourProcess(void* arg)
{
    int threadLimit;
    int *iptr = (int *)arg;//*iptr holds number to start with
    
    
    if(*iptr == 1)//deal w/ remainders
    {
    	threadLimit = ((numberLimit/threads) + (numberLimit % threads));
    }
    else
    {
    	threadLimit = (*iptr + (numberLimit/threads)) - 1;//finding portion of work for thread
    }
    for(int i = *iptr; i <= threadLimit; i++)
    {
    	int check = i;
    	int rem = 0;
    	int sum = 0;
    	while(sum != 1 && sum != 4)//determines happy or sad #
    	{
        	sum = 0; 
        	while(check > 0) 
        	{ 
            	rem = check % 10; 
            	sum += (rem * rem); 
            	check = check/10;  
        	} 
        	check = sum; 
    	}
    	
    	if(sum == 1)
    	{
        	   pthread_mutex_lock(&lock);
        	   happyNumbers++;//adjusting our global variable within the critical section
        	   pthread_mutex_unlock(&lock);
    	}
    }
		return NULL;
}
  
//example execution: ./happyNums   -t        3        -l     10000000 (must be at least 100)
//                    argv[0]    argv[1]  argv[2]  argv[3]   argv[4]
int main(int argc,char* argv[])
{
    if(argc!=5)//not correct # of commands
    {
        printf("\nError, invalid command line arguments.\n");
        return 0;  
    }
    if(strcmp(argv[1], "-t") != 0 || strcmp(argv[3], "-l") != 0)//not correct 'keywords'
    {
        printf("\nError, invalid command line arguments.\n");
        return 0;
    }
    else if(strcmp(argv[1], "-t") == 0 && strcmp(argv[3], "-l") == 0)//correct keywords & # of args
    {
        if(strcmp(argv[2], "1") == 0 || strcmp(argv[2], "2") == 0 || strcmp(argv[2], "3") == 0 || strcmp(argv[2], "4") == 0)//correct # of threads too
        {
            numberLimit = atoi(argv[4]);//global numberLimit holds our limit to count to
            if(numberLimit <= 100)
            {
                printf("\nError, limit must be > 100.\n");
                return 0;
            }
            //done checking commands, proceed with threads
            else
            {
                //lock mutex_t is initated right before creation, while destroyed after joining threads
                if(pthread_mutex_init(&lock, NULL) != 0) 
                {
                    printf("\nMutex initialization one has failed.\n");
                    return 1;
                }
                
                
                threads = atoi(argv[2]);
                printf("\nCount of happy and sad numbers from 1 to %d", numberLimit);
                printf("\nPlease wait. Running...\n");
                
                int arr[threads];//holds the sections we'll pass our threads to divide work
                
                for(int i = 0; i < threads; i++)
                {
                	arr[i] = (0 + ((numberLimit/threads)*i))+ 1;
                }
                
                for(int i = 0; i < threads; i++)//create threads
                {
                    int threadCheck = pthread_create(&thd[i], NULL, &ourProcess, &arr[i]);
                    if(threadCheck != 0)
                    {
                        printf("\nThread %d has failed to create.\n", (i + 1));
                        return 1;
                    }
                }
                //waiting for threads
                for(int i = 0; i < threads; i++)
                {
                    pthread_join(thd[i], NULL); 
                }

            }
        }
        else
        {
           printf("\nError, invalid thread count.\n");
           return 0;
        }
    }
    
    printf("\nCount of Happy Numbers = %d", happyNumbers);
    printf("\nCount of Sad Numbers = %d", (numberLimit - happyNumbers));
    pthread_mutex_destroy(&lock);
    return 0;

}
