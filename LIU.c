#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <fcntl.h>
#include <time.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include<sys/time.h>


int main(int argc, const char * argv[]) {
    pid_t pid;   //process id of child process
    int pipefd[2];
	int n;
    int i, j, k;    

    char array_ticker_symbol[100];
    char array_price_traded[100];
    char array_change_amount[100];
    char stock_price[20] = {0};   //store 0 in the price array
	clock_t start, end, total;
	start = clock();
  
    if ((pipe(pipefd)) == -1) {  // if fail to create pipe 

        fprintf(stderr, "Create pipe failed.\n");
        exit(1);

    }
   
    if ((pid = fork()) == -1) { // if fail to create a child process
        fprintf(stderr, "Create fork failed.\n");
        exit(1);
    }



    else if (pid > 0) // parent process
	{        
		close(pipefd[0]);  // not enter pipe 0 ,enter pipe 1
        for (n = 0; n < 20; n++) {
            char stock_price[20] = {0};   // initialize the value for stock_price         
            for (i = 0; i <= 3; i++){
                array_ticker_symbol[i]= rand() % 26 + 65; //random pick a symbol
            }
            for (j = 0; j <= 5; j++){
                array_price_traded[j]= rand() % 10 + 48; //random pick a price 
            }

            for (k = 0; k <= 5; k++){
                array_change_amount[k]= rand() % 10 + 48;
            }
			// check random number is even or odd number
            if (rand() % 2 == 0)    
			// if even number, put "+" at the 1st position
                array_change_amount[0] = (char)43;    
			// if odd, put "-" at the 1 position
            else
                array_change_amount[0] = (char)45;    		
			// put "." to the 4th poisition of price trade array
            array_price_traded[3] = (char)46;        			
			//  put "." to the 4th position of change amount array
			array_change_amount[3] = (char)46;      
			

                  /*put result into array stock_price*/
            strcat(stock_price,array_ticker_symbol);
            strcat(stock_price, " ");
            strcat(stock_price,array_price_traded);
            strcat(stock_price, " ");
            strcat(stock_price,array_change_amount);            
            sleep(1);   //wait 
			write(pipefd[1], stock_price, sizeof(stock_price)); // write in array stock
            fprintf(stderr, "Producer writes %s\n", stock_price); //print result 

        }       
        close(pipefd[1]);  //close pipe 1
    }

    

    else {  // child process
		close(pipefd[1]); // not enter pipe 1, enter pipe 0
        while ( pipefd [0] ) {
            read(pipefd[0], stock_price, sizeof(stock_price));
            fprintf(stderr, "Consumer reads %s\n", stock_price);
        }
        close(pipefd[0]);   //not enter pipe 0, enter pipe 1
		end = clock();    //record end time
		total = (float)(end - start) / CLOCKS_PER_SEC;  // calculate the total time
		printf("TOTAL TIME is: %f\n", total);    //print the total time
    }
}