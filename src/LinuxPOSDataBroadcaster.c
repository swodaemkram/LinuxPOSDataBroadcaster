/*
 ============================================================================
 Name        : LinuxPOSDataBroadcaster.c
 Author      : Mark Meadows
 Version     :
 Copyright   : copyright 2018
 Description : LinuxPOSDataBrodcaster C, Ansi-style
 ============================================================================
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include <errno.h>


#define BUFLEN 512                                            //Max length of buffer
#define VER  "v 0.0.1\n"                                      //Version of software

void print_help();                                            //declare print help function

void do_tcp(int PORT, char *SERVER, char *DATAFILE);			//Declare do_tcp structure

void die(char *s)                                             //declare die function
{
    perror(s);
    exit(1);
}


int main(int argc, char *argv[])

{

	char SERVER[] = "233.0.0.1";
	int PORT = 8888;
	char PROTOCOL[] = "udp";
    char DATAFILE[] = "default.dat";
    int REPEAT =  0;
    char filebuff[255];                                             //File Buffer
    FILE *fp;

	struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char message[BUFLEN];
    int z;

    if (argc < 2 ) 	print_help();                                  //Do this if there are too few arguments then exit
    if (argc < 9 )  print_help();							       //Too many arguments
                                                                   //Clean up arguments from the command line

      for (z = 1; z < argc; z++)                                  //* Skip argv[0] (program name). */
     {
                   if (argv[z][0] == '-')						  //Fixes the Command Line Argument Problem !
                   {
                   switch (argv[z][1])
                    {
                    case 'a':
                       	    strcpy (SERVER, argv[z+1]);
                    		break;

                   case 'p' :

                    		PORT = atoi(argv[z+1]);
                    		break;

                   case 'P'  :

                    		strcpy (PROTOCOL, argv[z+1]);
                    		break;

                   case 'd'  :

                    		strcpy (DATAFILE, argv[z+1]);
                    		break;

                   case 'L'  :
                    		 REPEAT = 1;
                    	     break;

                    }
                   }
     }

// Finished cleaning up arguments from the command line and I have passed them to their variables ======================================================

      printf("\n");
      printf("Sending data to I.P. Address %s on port %d using the %s protocol with the data file %s Repeat = %d \n\n",SERVER, PORT, PROTOCOL, DATAFILE,REPEAT);

      	  	  	  	  	  //Lets Open The Data File

      fp = fopen(DATAFILE,"r");                                    //open the data file

      if (strcmp(PROTOCOL , "tcp") == 0)                           //Should we do TCP connection ?
      {
     	      	  do_tcp(PORT, SERVER, DATAFILE);					//if yes lets to TCP
      }

      	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	//otherwise lets do UDP
      if  ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) //if the socket can't be made then Die
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


    while(!feof(fp) )                                        //Send Data until the End of the data file

    {

    	sleep(1);                                            //Lets Slow down just a little
    	fgets(filebuff, 255, (FILE*)fp);
    	strcpy(message , filebuff);


    	//send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1) //Send Data through Socket

        {
            die("sendto()");                                //Connection Can't be made
        }


        if (REPEAT == 1 && feof(fp))                      //Do We Need to repeat the data file ?

        {

        	int fclose(FILE *fp );						//Close the data file
        	fp = fopen(DATAFILE,"r");                   //Reopen File So We Can Repeat it again

        }


    }

    int fclose( FILE *fp ); 							//Lets Close the data file
    close(s);
    printf("Data Sent Successfully\n");					//Print out everthing was transmitted OK
    return 0;											//Exit Clean
}


//Lets Do TCP Protocol =============================================================================================================================//

void do_tcp(int PORT, char SERVER[], char DATAFILE[])

{



	int sock;
		struct sockaddr_in server;


		char message[1000] , server_reply[2000];

		//Create socket
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)
		{
			printf("Could not create socket");
		}
		puts("Socket created");

		server.sin_addr.s_addr = inet_addr(SERVER);
		server.sin_family = AF_INET;
		server.sin_port = htons( PORT );

		//Connect to remote server
		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
		{
			perror("connect failed. Error");
			exit (1);
		}

		puts("Connected\n");

		//keep communicating with server
		while(1)
		{
			printf("Enter message : ");
			scanf("%s" , message);

			//Send some data
			if( send(sock , message , strlen(message) , 0) < 0)
			{
				puts("Send failed");
				exit(1);
			}

			//Receive a reply from the server
			if( recv(sock , server_reply , 2000 , 0) < 0)
			{
				puts("recv failed");
				break;
			}

			puts("Server reply :");
			puts(server_reply);
		}

		close(sock);



  exit(0);

}

//Lets Print Help ====================================================================================================================================//

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





