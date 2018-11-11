/*
 ============================================================================
 Name        : posclientemu.c
 Author      : Mark Meadows
 Version     :
 Copyright   : copyright 2018
 Description : posclientemu C, Ansi-style
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


#define BUFLEN 512 //Max length of buffer
#define VER  "v 0.98.1\n" //Version of software

void print_help(); //declare print help function

void do_tcp(int PORT, int REPEAT, char *SERVER, char *DATAFILE, int QUIET );
//Declare do_tcp structure

void die(char *s) //declare die function

{
    perror(s);
    int fclose(FILE *fp );
    exit(1);
}


int main(int argc, char *argv[])

{

	char SERVER[] = "233.0.0.1";
	int PORT = 8888;
	char PROTOCOL[] = "udp";
    char DATAFILE[] = "default.dat";
    int REPEAT =  0;
    char filebuff[255]; //File Buffer
    FILE *fp;

	struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char message[BUFLEN];
    int z;
    int QUIET = 0;
    if (argc < 2 ) 	print_help(); //Do this if there are too few arguments then exit
    if (argc < 10 )  print_help(); //Too many arguments

//Clean up arguments from the command line=====================================================================//

      for (z = 1; z < argc; z++) //* Skip argv[0] (program name). */

      {
                   if (argv[z][0] == '-') //Fixes the Command Line Argument Problem !
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

                   case 'l'  :
                    		 REPEAT = 1;
                    	     break;

                   case 'q' :
                	        QUIET = 1;
                	        break;

                   case 'L' :
                	        printf("-L is an invalid argument !\n");
                	   	    print_help();
                	   	    break;

                    }

                   }

      }

// Finished cleaning up arguments from the command line and I have passed them to their variables ========//

// ===================================================Lets do some Simple error checking==================//

      if (!SERVER)

      {
    	  printf("Need to know what server to send data to!\n");
    	  print_help();

      }

      if (PORT < 1)

      {
    	  printf("Need a Port to use !\n");
    	  print_help();
      }

      if (!PROTOCOL)

      {
    	  printf("Need to know what protocol to use!\n");
    	  print_help();
      }

      if (!DATAFILE)

      {
    	  printf("Need Data File to send!\n");
    	  print_help();
      }

      if (fopen(DATAFILE, "r") == 0)

      {
    	  	  	  printf("\n");
    	  	  	  printf("Could not Open File %s ! \n", DATAFILE);
    	  	  	  print_help();
      }


//======================= Finished With Simple Error Checking may add more in the future ==================//

      printf("\n");
      printf("Sending data to I.P. Address %s on port %d using the %s protocol with the data file %s Repeat = %d Quiet = %d\n\n",SERVER, PORT, PROTOCOL, DATAFILE,REPEAT, QUIET);



     fp = fopen(DATAFILE,"r"); //open the data file

      if (strcmp(PROTOCOL , "tcp") == 0) //Should we do TCP connection ?

      {
     	      	  do_tcp(PORT, REPEAT, SERVER, DATAFILE, QUIET); //if yes lets Do TCP
      }

      	  	  	  	  	  	  	  	  	  	  	  	  	  	  	//otherwise lets do UDP
      if  ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) //if the socket can't be made then Die

      {
        die("socket");
        int fclose(FILE *fp );
      }



    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)

    {
        fprintf(stderr, "inet_aton() failed\n"); //if we cant open a socket program fails with error 1
        int fclose(FILE *fp );
        exit(1);
    }


    while(!feof(fp) ) //Send Data until the End of the data file

    {

    	sleep(1); //Lets Slow down just a little
    	fgets(filebuff, 255, (FILE*)fp);
    	strcpy(message , filebuff);


    	if (REPEAT == 0 && feof(fp))

    			{
    				close(s);
    				int fclose(FILE *fp );
    				printf("\nData sent successfully \n");
    			    exit(0);
    			}

    											//send the message
    	if (REPEAT == 1 && feof(fp)) //Do We Need to repeat the data file ?

    	        {
    	        	int fclose(FILE *fp ); //Close the data file
    	        	fp = fopen(DATAFILE,"r"); //Reopen File So We Can Repeat it again
    	        	fgets(filebuff, 255, (FILE*)fp);
    	        	strcpy(message , filebuff);

    	        }


    	if(QUIET == 0)

    	{
    		printf("Sending Data... \n");
    	}

    		if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    						//Send Data through Socket
        {
            die("sendto()"); //Connection Can't be made
        }



    }

    int fclose( FILE *fp );	//Lets Close the data file
    close(s);
    printf("Data Sent Successfully\n");	//Print out everthing was transmitted OK
    return 0;						//Exit Clean
}


//Lets Do TCP Protocol ===================================================================================//

void do_tcp(int PORT, int REPEAT, char SERVER[], char DATAFILE[], int QUIET)

{


	FILE *fp;
	int sock;
		struct sockaddr_in server;
		char message[1000];	//, server_reply[2000];
		char filebuff[255];

		fp = fopen(DATAFILE,"r");



							//Create socket
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1)

		{
			printf("Could not create socket\n");
			int fclose(FILE *fp );
			exit(1);
		}

							//printf("Socket created\n");

		server.sin_addr.s_addr = inet_addr(SERVER);
		server.sin_family = AF_INET;
		server.sin_port = htons( PORT );

							//Connect to remote server
		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)

		{
			perror("connect failed. Error\n");
			int fclose(FILE *fp );
			exit (1);
		}

		printf("Connected\n\n"); //We are Connected

								//keep communicating with server
		while(!feof(fp))

		{


			sleep(1); //Lets Slow down just a little

			            fgets(filebuff, 255, (FILE*)fp); //Gets data to send from file
						strcpy(message , filebuff);


			    	if (REPEAT == 1 && feof(fp)) //Do We Need to repeat the data file ?

			    	{
			    			int fclose(FILE *fp ); //Close the data file
			    			fp = fopen(DATAFILE,"r"); //Reopen File So We Can Repeat it again
			    									// Debug printf("end of file");
			    			fgets(filebuff, 255, (FILE*)fp); //Gets data to send from file
			    			strcpy(message , filebuff);

			    	}


			    	if (REPEAT == 0 && feof(fp))

			    	{
			    			close(sock);
			    			int fclose(FILE *fp );
			    			printf("\nData sent successfully \n");
			    		    exit(0);
			    	}


			if( send(sock , message , strlen(message) , 0) < 0)	//Send some data

			{
				printf("Send failed\n");
				int fclose(FILE *fp );
				exit(1);
			}


			if(QUIET == 0)

			{
				printf("Sending Data ...\n");
			}
		}

		close(sock);
		int fclose(FILE *fp );

		printf("\nData sent successfully \n");
  exit(0);

}

//Lets Print Help =========================================================================================//

void print_help()

    {
    	 	 	printf("\n");
    	    	printf("posclientemu by Mark Meadows %s ", VER);
    	    	printf("a command line Point Of Sales Emulator for *nix \n");
    	    	printf("\n");
    	    	printf("-a, I.P. address to send data to\n");
    	        printf("-p, port to send data to\n");
    	        printf("-P, protocal to use tcp or udp\n");
    	        printf("-d, data file location Path to data file\n");
    	        printf("-l, loop data file Keep sending data in the data file over and over again\n");
    	        printf("-h, this help information\n");
    	        printf("-q, quiet used to prevent echoing status to the screen when using ""&"" to put program into the background\n");
    	        printf("\n");
    	        printf(" Example usage posclientemu -a 231.0.0.1 -p 20001 -P udp -d test.txt -l\n");
    	        printf("\n");
    	        exit(1);
    }





