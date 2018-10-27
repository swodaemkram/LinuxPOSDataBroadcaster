/*
 ============================================================================
 Name        : LinuxPOSDataBroadcaster.c
 Author      : Mark Meadows
 Version     :
 Copyright   : copyright 2018
 Description : LinuxPOSDataBrodcaster C, Ansi-style
 ============================================================================
 */

#include<stdio.h>                                                //printf
#include<string.h>                                              //memset
#include<stdlib.h>                                             //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "233.0.0.1"
#define BUFLEN 512                                            //Max length of buffer
#define PORT 8888                                             //The port on which to send data
#define VER  "v 0.0.1\n"                                      //Version of software

void print_help();                                            //declare print help function
void die(char *s)                                             //declare die function
{
    perror(s);
    exit(1);
}


int main(int argc, char *argv[])
{

	struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if (argc < 2 ) 	print_help();                              //Do this if there are too few arguments then exit
    if (argc > 5 )  print_help();							//Too many arguments
                                                              //Clean up arguments from the command line
                                                               //*******************************************************************************
    														//********* Here is were Im working ************************************************
     int z;

      for (z = 1; z < argc; z++)  /* Skip argv[0] (program name). */
     {
                     printf("%s\n", argv[z]);

      }
        														//*********************************************************************
                                                             //*******************************************************************************
                                                              // Finished cleaning up arguments from the command line and I have passed them to their variables

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) //if the socket cant be made then Die
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");          //if we cant open a socket program fails with error 1
        exit(1);
    }

    while(1)
    {
        printf("Enter message : ");                       //enter something to send
        gets(message);

        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1) //Send Data through Socket
        {
            die("sendto()");
        }

    }


    close(s);
    return 0;
}


    void print_help()

    {
    	 printf("\n");
    	    	printf("LinuxPOSDataBroadcaster By Mark Meadows v 0.0.1\n");
    	    	printf("\n");
    	    	printf("-a, address to use\n");
    	        printf("-p, port to use\n");
    	        printf("-P, protocal to use tcp or udp\n");
    	        printf("-d, data file location Path to data file\n");
    	        printf("-l, loop data file Keep sending data in the data file over and over again\n");
    	        printf("-h, this help information\n");
    	        printf("-v, Version info\n");
    	        printf("\n");
    	        printf(" Example usage LinuxPOSDataBrodcaster -a 231.0.0.1 -p 20001 -P udp -d test.dat -l\n");
    	        printf("\n");
    	        exit(1);
    }




