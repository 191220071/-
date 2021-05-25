#include "lib.h"
#include "types.h"
#define N 5 

void think(int index){
	printf("Philosopher %d : think\n",index);
	sleep(128);
}

void eat(int index){
	printf("Philosopher %d : eat\n",index);
	sleep(128);
}

void philosopher(sem_t *forks,int index){
	int i=2;
	sem_t* left_fork = forks+index;
	sem_t* right_fork= forks+(index+1)%N;
	while(i-->0){
		think(index);
		if(index%2==0){
			sem_wait(left_fork);
			sleep(128);
			sem_wait(right_fork);
			sleep(128);
		}
		else{
			sem_wait(right_fork);
			sleep(128);
			sem_wait(left_fork);
			sleep(128);
		}
		eat(index);
		sem_post(left_fork);
		sleep(128);
		sem_post(right_fork);
		sleep(128);
	}
}

int one() {
	printf("philosopher\n");
	sem_t forks[5];
	for(int i=0;i<5;++i)
		sem_init(forks+i,1);
	for(int i=0,ret=0;i<N;++i){
		ret = fork();
		if(ret == 0){
			philosopher(forks,i);
			exit();
		}

	}

	exit();
	return 0;
}
int two(){
	 printf("[Start Test Producer & Consumer Program]\n");

    sem_t buffer, lock;
    int fork_time = 6;
    int ret = 0;
    int k = 1;
    int j = 0;
    int i = 0;
    
    sem_init(&buffer, 0); sem_init(&lock, 1);

    for (int i = 0; i < fork_time; ++i)
    {
        ret = fork();
        if (ret == -1)
            exit();
    }
    
    if (getpid() == 2 || getpid() == 3)
    {
        j = getpid() - 1; // 编号从1开始进行编号
        i = getpid();

        for ( ; k <= 8; )
        {
            sleep(64);
            printf("pid %d, producer %d, try lock, product %d\n", i, j, k);
            sem_wait(&lock);
            printf("pid %d, producer %d, locked\n", i, j);
            //sleep(16);
            sem_post(&buffer);
            sleep(12);
            printf("pid %d, prodecer %d, produce %d\n", i, j, k);
            sem_post(&lock);
            printf("pid %d, producer %d, unlock\n", i, j);
            ++k;
        }
        exit();
    }

    else if (getpid() > 3 && getpid() < 8)
    {
        j = getpid() - 3; // 编号从1开始进行编号
        i = getpid();

        for ( ; k <= 4; )
        {
            printf("pid %d, consumer %d, try consume, product %d\n", i, j, k);
            sem_wait(&buffer);
            printf("pid %d, consumer %d, try lock, product %d\n", i, j, k);
            sem_wait(&lock);
            printf("pid %d, consumer %d, locked\n", i, j);
            sleep(16);
            printf("pid %d, consumer %d, consumed, product %d\n", i, j, k);
            sem_post(&lock);
            printf("pid %d, consumer %d, unlock\n", i, j);
            sleep(160);
            ++k;
        }
        exit();
    }
    exit();
	return 0;
}
int three()
{
	printf("reader_writer\n");

	int ret = 0;
	int Rcount = 0;
	sem_t WriteMutex, CountMutex;
	ret = sem_init(&WriteMutex, 1);
	if(ret == -1){
		printf("Init semaphone error!\n");  
		return -1;  
	}
	ret = sem_init(&CountMutex, 1);
	if(ret == -1){
		printf("Init semaphone error!\n");  
		return -1;  
	}

	for(int i = 0; i < 5; ++i){ //To get six process by fork, three reader(pid = 1~3) and three writer(pid = 4~6)
		ret = fork();
		if(ret == -1){
			printf("Fork error!\n");  
			return -1;  
		}
		if(ret == 0){ //Child process
			break;
		}
	}

	int id = 0;
	if(getpid() >= 1 && getpid() <= 3){  //reader process
		while(1){
			sem_wait(&CountMutex);
			sleep(128);
			if(Rcount == 0){
				id = getpid();
				sem_wait(&WriteMutex);
			}
			Rcount++;
			sem_post(&CountMutex);
			id = getpid();
			printf("Reader %d: read, total %d reader\n", id, Rcount);
			sleep(128);
			sem_wait(&CountMutex);
			sleep(128);
			Rcount--;
			if(Rcount == 0){
				sem_post(&WriteMutex);
			}
			sem_post(&CountMutex);
			//printf("Reader %d read over, total %d reader\n", id, Rcount);
		}
	}
	else if(getpid() >= 4 && getpid() <= 6){  //writer process
		while(1){
			sem_wait(&WriteMutex);
			id = getpid();		
			printf("Writer %d: write\n", id);
			sleep(128);
			sem_post(&WriteMutex);
			//printf("Write over\n");
		}
	}

	exit();
	return 0;
}

int uEntry(void) {
	// // For lab4.1
	// // Test 'scanf' 
	int dec = 0;
	int hex = 0;
	char str[6];
	char cha = 0;
	int ret = 0;
	while(1){
		printf("Input:\" Test %%c Test %%6s %%d %%x\"\n");
		ret = scanf(" Test %c Test %6s %d %x", &cha, str, &dec, &hex);
		printf("Ret: %d; %c, %s, %d, %x.\n", ret, cha, str, dec, hex);
		if (ret == 4)
			break;
	}
	
	// For lab4.2
	// Test 'Semaphore'
	int i = 4;

	sem_t sem;
	printf("Father Process: Semaphore Initializing.\n");
	ret = sem_init(&sem, 2);
	if (ret == -1) {
		printf("Father Process: Semaphore Initializing Failed.\n");
		exit();
	}

	ret = fork();
	if (ret == 0) {
		while( i != 0) {
			i --;
			printf("Child Process: Semaphore Waiting.\n");
			sem_wait(&sem);
			printf("Child Process: In Critical Area.\n");
		}
		printf("Child Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}
	else if (ret != -1) {
		while( i != 0) {
			i --;
			printf("Father Process: Sleeping.\n");
			sleep(128);
			printf("Father Process: Semaphore Posting.\n");
			sem_post(&sem);
		}
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}

	// For lab4.3
	// TODO: You need to design and test the philosopher problem.
	// Note that you can create your own functions.
	// Requirements are demonstrated in the guide.
	char ch;
	printf("1 - philisopher\n       2 - for customer_and_producer\n       3 - for reader_writer\n");
	scanf("%c", &ch);

	switch (ch)
	{
	case '1':
		one();
		break;
	case '2':
		two();
		break;
	case '3':
		three();
		break;
	default:
		break;
	}
	exit();
	return 0;
}
