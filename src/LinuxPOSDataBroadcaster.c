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
#include<errno.h>
#include<sys/stat.h>

#define BUFLEN 1024 //Max length of buffer
#define VER  "v 1.02.0\n" //Version of software

void print_help(); //declare print help function

void do_tcp(int PORT, int REPEAT, char *SERVER, char *DATAFILE, int QUIET, int filebuffsize ); //Declare do_tcp structure

char * convertbadstring(char *message, int filebuffsize);

char *FixedMessage;

int filebuffsize;


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
    FILE *fp;
	struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    int z;
    int QUIET = 0;

    if (argc < 2 ) 	print_help(); //Do this if there are too few arguments then exit
    if (argc > 11 )  print_help(); //Too many arguments

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

// ===================================================Lets do some Simple error checking==================///

      	  if (PORT < 1)

      	  {
      		  	     	  printf("Need a Port to use !\n");
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

      if (strcmp(PROTOCOL , "tcp") == 0) //Should we do TCP connection ?

      {

    	  do_tcp(PORT, REPEAT, SERVER, DATAFILE, QUIET, filebuffsize); //if yes lets Do TCP

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

        fp = fopen(DATAFILE,"r"); //open the data file
    	struct stat st;
    	stat(DATAFILE, &st);
    	filebuffsize  = st.st_size;
    	filebuffsize = filebuffsize + 1; // We need to get the last character
    	char filebuff[filebuffsize];
    	fgets(filebuff, filebuffsize, (FILE*)fp);
    	char message[filebuffsize];

    	char NewFile[filebuffsize];

    			    	while (!feof(fp)){ //debug

    			    	fgets(filebuff, filebuffsize, (FILE*)fp);
    			    	strcat(NewFile,filebuff);

    			    	}

    	FixedMessage = convertbadstring(NewFile , filebuffsize);  //find and replace the Mnemonics
		strcpy(message , FixedMessage);	 //Copy fixed message back to message so I don't have to rewrite everything
		int NewFileLength = strlen(message); // Length of the New Fixed File
		char NewMessage[100];
		int ChunkFile = 100;

		while(ChunkFile < NewFileLength){

		strncpy(NewMessage,message + ChunkFile, 100); //<--- move through Variable chunking 150 characters until the end
		ChunkFile = ChunkFile + 100;
		sleep(1);


		if (REPEAT == 0 && ChunkFile > NewFileLength)

    			{

					close(s);
    				int fclose(FILE *fp );
    				printf("\nData sent successfully \n");
    				exit(0);

    			}

    	if (REPEAT == 1 && ChunkFile > NewFileLength){

    		ChunkFile = 100;

    	}

    	if(QUIET == 0)

    	{
    		printf("\nSending Data...\n %s\n",NewMessage);
    	}

    			if (sendto(s, NewMessage, strlen(NewMessage) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    						//Send Data through Socket
    			{
    				die("sendto()"); //Connection Can't be made
    			}

    }

    int fclose( FILE *fp );	//Lets Close the data file
    close(s);
    printf("Data Sent Successfully\n");	//Print out everything was transmitted OK
    return 0;						//Exit Clean
}


//Lets Do TCP Protocol ===================================================================================//

void do_tcp(int PORT, int REPEAT, char SERVER[], char DATAFILE[], int QUIET, int filebuffsize)

{

	FILE *fp;
	int sock;
		struct sockaddr_in server;

		        fp = fopen(DATAFILE,"r"); //open the data file
		    	struct stat st;
		    	stat(DATAFILE, &st);
		    	filebuffsize  = st.st_size;
		    	filebuffsize = filebuffsize + 1; // We need to get the last character

		    	char filebuff[filebuffsize];
		    	char message[filebuffsize];
		    	char NewFile[filebuffsize];

		    	while (!feof(fp)){

		    	fgets(filebuff, filebuffsize, (FILE*)fp);
		    	strcat(NewFile,filebuff);

		    	}


		    	FixedMessage = convertbadstring(NewFile , filebuffsize);  //find and replace the Mnemonics

		    	strcpy(message , FixedMessage);	 //Copy fixed message back to message so I don't have to rewrite everything
		    	int NewFileLength = strlen(message); // Length of the New Fixed File
		    	char NewMessage[100];
				int ChunkFile = 100;


		sock = socket(AF_INET , SOCK_STREAM , 0); //Create socket
		if (sock == -1)

		{
			printf("Could not create socket\n");
			int fclose(FILE *fp );
			exit(1);
		}

		server.sin_addr.s_addr = inet_addr(SERVER);
		server.sin_family = AF_INET;
		server.sin_port = htons( PORT );

		if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) //Connect to remote server

		{
			perror("connect failed. Error\n");
			int fclose(FILE *fp );
			exit (1);
		}

		printf("Connected\n\n"); //We are Connected


		while(ChunkFile < NewFileLength){

			strncpy(NewMessage,message + ChunkFile, 100); //<--- move through Variable chunking 150 characters until the end

			ChunkFile = ChunkFile + 100;
			sleep(1);

			if (REPEAT == 1 && ChunkFile > NewFileLength) //Do We Need to repeat the data file ?
			    	{

			    		ChunkFile = 100;

			    	}

	    	if (REPEAT == 0 && ChunkFile > NewFileLength
	    			)

			    	{
			    			close(sock);
			    			int fclose(FILE *fp );
			    			printf("\nData sent successfully \n");
			    		    exit(0);
			    	}


			if( send(sock , NewMessage , strlen(NewMessage) , 0) < 0)	//Send some data

         			{
							printf("\nSend failed\n");
							int fclose(FILE *fp );
							exit(1);
		        	}

			if(QUIET == 0)

			{
				printf("\nSending Data ...\n %s\n",NewMessage);
			}
		}

		close(sock);
		int fclose(FILE *fp );

		printf("\nData sent successfully \n");
  exit(0);

}

//Lets Fix Mnemonic String to Real String  =================================================================//

char * convertbadstring(char filebuffer[], int filebuffsize)

{


int dataIndex = 0;
int strIndex = 0;
//int outLen = 0;
//char outData[filebuffsize];

char *outData = malloc (sizeof (char)* filebuffsize);



char WordArray[162][7] = {
		"<NUL>", "<SOH>", "<STX>", "<ETX>", "<EOT>", "<ENQ>", "<ACK>", "<BEL>",
		"<BS>", "<HT>", "<LF>", "<VT>", "<FF>", "<CR>", "<SO>", "<SI>", "<DLE>",
		"<DC1>", "<DC2>", "<DC3>", "<DC4>", "<NAK>", "<SYN>", "<ETB>", "<CAN>",
		"<EM>", "<SUB>", "<ESC>", "<FS>", "<GS>", "<RS>", "<US>", "<SP>",
		"<DEL>", "<0x80>", "<0x81>", "<0x82>", "<0x83>", "<0x84>", "<0x85>",
		"<0x86>", "<0x87>", "<0x88>", "<0x89>", "<0x8A>", "<0x8B>", "<0x8C>",
		"<0x8D>", "<0x8E>", "<0x8F>", "<0x90>", "<0x91>", "<0x92>", "<0x93>",
		"<0x94>", "<0x95>", "<0x96>", "<0x97>", "<0x98>", "<0x99>", "<0x9A>",
		"<0x9B>", "<0x9C>", "<0x9D>", "<0x9E>", "<0x9F>", "<0xA0>", "<0xA1>",
		"<0xA2>", "<0xA3>", "<0xA4>", "<0xA5>", "<0xA6>", "<0xA7>", "<0xA8>",
		"<0xA9>", "<0xAA>", "<0xAB>", "<0xAC>", "<0xAD>", "<0xAE>", "<0xAF>",
		"<0xB0>", "<0xB1>", "<0xB2>", "<0xB3>", "<0xB4>", "<0xB5>", "<0xB6>",
		"<0xB7>", "<0xB8>", "<0xB9>", "<0xBA>", "<0xBB>", "<0xBC>", "<0xBD>",
		"<0xBE>", "<0xBF>", "<0xC0>", "<0xC1>", "<0xC2>", "<0xC3>", "<0xC4>",
		"<0xC5>", "<0xC6>", "<0xC7>", "<0xC8>", "<0xC9>", "<0xCA>", "<0xCB>",
		"<0xCC>", "<0xCD>", "<0xCE>", "<0xCF>", "<0xD0>", "<0xD1>", "<0xD2>",
		"<0xD3>", "<0xD4>", "<0xD5>", "<0xD6>", "<0xD7>", "<0xD8>", "<0xD9>",
		"<0xDA>", "<0xDB>", "<0xDC>", "<0xDD>", "<0xDE>", "<0xDF>", "<0xE0>",
		"<0xE1>", "<0xE2>", "<0xE3>", "<0xE4>", "<0xE5>", "<0xE6>", "<0xE7>",
		"<0xE8>", "<0xE9>", "<0xEA>", "<0xEB>", "<0xEC>", "<0xED>", "<0xEE>",
		"<0xEF>", "<0xF0>", "<0xF1>", "<0xF2>", "<0xF3>", "<0xF4>", "<0xF5>",
		"<0xF6>", "<0xF7>", "<0xF8>", "<0xF9>", "<0xFA>", "<0xFB>", "<0xFC>",
		"<0xFD>", "<0xFE>", "<0xFF>"
	};



		while (strIndex < strlen(filebuffer)) {



		if (filebuffer[strIndex] != '<') {



			outData[dataIndex] = filebuffer[strIndex];

			dataIndex++;
			strIndex++;
			continue;
		}

		    //Search for match of command string
		    int Len;
		    for (int i = 0; i != 162; i++) {
			Len = strlen(WordArray[i]);


			if (!strncmp(filebuffer + strIndex, WordArray[i], Len)) {
				if (i < 33) {
					outData[dataIndex] = i;
				}
				else {
					outData[dataIndex] = (i - 33) + 0x7F;
				}
				dataIndex++;
				strIndex += Len;
				break;
			}
			if ((i == 161) && (filebuffer[strIndex] == '<')) // we checked all the listed mnemonics and didn't find a match
			{
				outData[dataIndex] = filebuffer[strIndex];
				dataIndex++;
				strIndex++;
			}
		}
	}


	//outLen = dataIndex;
	outData[dataIndex] = 0;

	//free(outData);
	return outData;

	}

//Done Fixing Mnemonics in the data file =====================================================//


//Lets Print Help =========================================================================================//

void print_help()

    {
    	 	 	printf("\n");
    	    	printf("posclientemu by Mark Meadows %s \n", VER);
    	    	printf("a command line Point Of Sales Emulator for *nix \n");
    	    	printf("\n");
    	    	printf("-a, I.P. address to send data to\n");
    	        printf("-p, port to send data to\n");
    	        printf("-P, protocol to use tcp or udp\n");
    	        printf("-d, data file location Path to data file\n");
    	        printf("-l, loop data file Keep sending data in the data file over and over again\n");
    	        printf("-h, this help information\n");
    	        printf("-q, quiet used to prevent echoing status to the screen when using ""&"" to put program into the background\n");
    	        printf("\n");
    	        printf(" Example usage posclientemu -a 231.0.0.1 -p 20001 -P udp -d test.txt -l\n");
    	        printf("\n");
    	        exit(1);
    }





