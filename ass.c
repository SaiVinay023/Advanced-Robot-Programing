// Code for pipe connections S,P,L,G


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include<signal.h>
#include <sys/time.h>
#include <math.h>

int go=1, dump=1;

void start_handler(int signo)
{
	printf("first signal used! %d\n", signo);
	go=0;
}

void stop_handler(int signo)
{
    	printf("second signal used! %d\n",signo);
    	go=1;
}

void dump_handler(int signo)
{
    	printf("for dump handler! %d\n",signo);
    	dump=0;
}

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 


int main(){
	
	delay(1000);
	start_handler(1);
 

  	
	int fd, fdS[2], fdL[2], fdG[2];
    	char buff[10], fdG_char[10], new_token_char[10], old_token_char[10], IP[45], gobuff[10], buffLF[100];
	char buffL[100];
	memset(buffL,0,sizeof(buffL));
	
    	struct timeval tv1, tv2;

    	if ((fd=open("configurationfile.txt", O_RDONLY))<0 )
        	perror ("open");
    
    	ssize_t ret=read(fd, &buff, sizeof(buff));
    	close(fd);
    	int RF=atoi(buff); 

	
	//RF value is taken from config file
//As we have taken only pipes connection, we didnt give the values of IP address, port and waiting time in configuration file
	// AS FOR DUMP LOG FILE BASED ON HOW MANY INFO THERE IS IN THE CONFIG FILE YOU MAKE 

    	
    	if ((pipe(fdS)) == -1)
        	printf(" error occurred in pipe S");
    	pid_t pidS=fork();
    	if (pidS == 0){ //S receives Posix signals from console (start/stop/dump log)
        	printf("code of S runs\n");
        	close(fdS[0]);
        
        	if ((signal(SIGUSR1,start_handler))==SIG_ERR)
            		printf("can't catch SIGUSR1");
        	if ((signal(SIGUSR2,stop_handler))==SIG_ERR)
            		printf("can't catch SIGUSR2");
        	if ((signal(SIGWINCH,dump_handler))==SIG_ERR)
            		printf("can't catch SIGWINCH");
        
        	fprintf(stderr, "Running process....(PID=%d)\n", (int) getpid());
         
		int fdlf=open("log2.txt", O_RDWR); 
        	if (fdlf < 0)
        		perror("open");
		while (1){
			//printf("go value in S: %d\n",go);
			printf("dump value in S: %d\n",dump);
			sprintf(gobuff,"%d",go);
        		write(fdS[1],gobuff,sizeof(gobuff)); //error handling?
        	
			if (dump == 0){
				printf("we print the contents of logfile: \n");
				int ret=100;
				while (ret!=0){
					ret=read(fdlf,buffLF,sizeof(buffLF));
					printf("ret= %d\n",ret);
					if(ret<0)
						perror("read dump");
					if((write(1,buffLF,sizeof(buffLF)))<0)
						perror("write dump");
					printf("\n");
					sleep(1);
				}
			}
			sleep(1);
        	}
        	close(fdS[1]);
    	} //S code ends
	
delay(1000);
	
    	if ((pipe(fdL)) == -1)
        	printf("error occurred in pipe L");
    
    	pid_t pidL=fork();
    	if (pidL == 0){
        	printf("we do the code ofL\n");
        	close(fdL[1]);
        	int fdlf=open("logfile.txt", O_RDWR); //open logfile, read&write permission, error handling
        	if (fdlf < 0)
            		perror("open");
        
        	while (1){
        		int k=read(fdL[0],buffL,sizeof(buffL));
        		if (k!=sizeof(buffL))
            			perror("read in L");
        		if ((write(fdlf,buffL,sizeof(buffL)))!=sizeof(buffL))
            			perror("write in L");
			sleep(10);
		}
	} //code L ends here

    	
delay(1000);
   	if ((pipe(fdG)) == -1)
        	printf("pipe G error occurred");
    	pid_t pidG=fork();
    	if (pidG == 0){
        	printf("we enter G\n");
        	close(fdG[0]);
        	sprintf(fdG_char,"%d",fdG[1]); 
        	if ((execl("./G",IP,fdG_char,NULL))<0)
            		perror("exec");
        	printf("this should never appear!\n");
    	} //code G ends here

    delay(1000);
    
    //Computing token
    
    	close(fdL[0]); //writing on L
    	close(fdS[1]); //reading from S
	int i=0;
	while (1){
    		read(fdS[0],gobuff,sizeof(gobuff)); //reading S pipe the flag
		go=atoi(gobuff);
    		//printf("go value in P: %d\n",go);
		
		if (go == 0){ //start signal
        		
        		read(fdG[0],&old_token_char,sizeof(old_token_char));
        		float old_token=atof(old_token_char);
			//instant value of token
			gettimeofday (&tv2, NULL);
    	
        		printf("i recieved as old token: %f",old_token);
			if (i==0){
				tv1.tv_sec=tv2.tv_sec;
				tv1.tv_usec=tv2.tv_usec;
			}

 			int DT=(tv2.tv_sec-tv1.tv_sec)*1000+(tv2.tv_usec-tv1.tv_usec);
			printf("computed DT= %d\n",DT);
        		float new_token = old_token + DT*(1-(old_token)*(old_token)/2)*2*M_PI*RF;
        		printf(" computed token: %f\n", new_token); 
	
			time_t t=time(NULL);
			struct tm *tm=localtime(&t);
			char tempo1[64];
			assert(strftime(tempo1,sizeof(tempo1),"%c",tm));
			char tempo2[64];
			strcpy(tempo2,tempo1);
        		char buff1[30]=" from G | from S ";
			
			sprintf(new_token_char,"%f",new_token);

			char* a=strcat(strcat((strcat(tempo1,buff1)),old_token_char),"\n");
			char* b=strcat(strcat(tempo2," "),new_token_char);
			char* c=strcat(a,b);
			
			puts(c);	
			
			sprintf(buffL,"%s",c);
        		
			if ((write(fdL[1],buffL,sizeof(buffL)))<0)
            			perror("write L");
			//delivery of token
			gettimeofday (&tv1, NULL);
       			i++;
			sleep(1);
		}

		else if (go == 1){ //stop signal or (waiting for start)
			printf("I'm on sleep\n");
			sleep(1);
		}
		
    	} //ends the infinite loop of P
	close(fdL[1]);
delay(1000);
    	
    return 0;
}



