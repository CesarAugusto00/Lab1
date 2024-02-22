
/* A simple UDP client which measures round trip delay */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>


#define SERVER_UDP_PORT 5000
#define MAXLEN 4096 /* maximum data length */
#define DEFLEN 64 /* default length */
#define PACKET_SIZE 1000


long delay(struct timeval t1, struct timeval t2);

bool file_exists(const char *filename);

size_t getFileSize(const char *fileName);

size_t getItinerationFileSize(const char *fileName);


int main(int argc, char **argv)
    {
    int data_size = DEFLEN, port = SERVER_UDP_PORT;
    int i, j, sd, server_len;
    char *pname, *host, rbuf[4], sbuf[4];
    struct hostent *hp;
    struct sockaddr_in server;
    struct timeval start, end;
    bool check;
    const char* inputString[256];
    char word[100];
    char word2[100];
    const char* confirming = "yes";
    pname = argv[0];
    argc--;
    argv++;
    if (argc > 0 && (strcmp(*argv, "-s") == 0)) {
        if (--argc > 0 && (data_size = atoi(*++argv))) {
            argc--;
            argv++;
        }
        else {
            fprintf (stderr, "Usage: %s [-s data_size] host [port]\n", pname);
            exit(1);
        }
    }
    if (argc > 0) {
        host = *argv;
        if (--argc > 0)
            port = atoi(*++argv);
    }
    else {
        fprintf(stderr, "Usage: %s [-s data_size] host [port]\n", pname);
        exit(1);
    }
    /* Create a datagram socket */
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        fprintf(stderr, "Can’t create a socket\n");
        exit(1);
    }
    /* Store server’s information */
    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if ((hp = gethostbyname(host)) == NULL) {
        fprintf(stderr, "Can’t get server’s IP address\n");
        exit(1);
    }
    bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    if (data_size > MAXLEN) {
        fprintf(stderr, "Data is too big\n");
        close(sd);
        exit(1);
    }

    // Here it will send the value
    // fgets(inputString, sizeof(inputString), stdin);
    // word = strtok(inputString, " ");
    // word2 = strtok(NULL, " ");
    // printf("%s\n", word2);
    // word2[strcspn(word2, "\n")] = '\0';
    scanf("%s", word);
    scanf("%s", word2);
    check = file_exists(word2); 
    if(check == true){
        strcpy(sbuf, word);
    }
    else{
        printf("File does not exist");
        exit(1);
    }


    gettimeofday(&start, NULL); /* start delay measure */

    /* transmit data */
    server_len = sizeof(server);
    if (sendto(sd, sbuf, 4, 0, (struct sockaddr *) &server, server_len) == -1) {
        fprintf(stderr, "sendto error\n");
        exit(1);
    }
    /* receive data */
    if (recvfrom(sd, rbuf, 4, 0, (struct sockaddr *) &server, &server_len) < 0) {
        fprintf(stderr, "recvfrom error\n");
        exit(1);
    }  
    else{
        printf("HERE IS RBUF %s \n", rbuf);
        if(strcmp(rbuf, "yes") == 0 ){
            printf("A file transfer can start.\n");
        }

        else if(strcmp(rbuf, "no")  == 0){
            printf("The sever responded with no \n");
            exit(1);
        }
        else{
            printf("No confirmation\n");
            exit(1);
        }
    }



    gettimeofday(&end, NULL); /* end delay measure */
    printf ("Round-trip delay = %ld ms.\n",
    delay(start, end));
    //if (strncmp(sbuf, rbuf, data_size) != 0)
    //    printf("Data is corrupted\n");
        close(sd);
    return(0);
    }

/*
* Compute the delay between t1 and t2 in milliseconds
*/
long delay (struct timeval t1, struct timeval t2)
{
    long d;
    d = (t2.tv_sec - t1.tv_sec) * 1000;
    d += ((t2.tv_usec - t1.tv_usec + 500) / 1000);
    return(d);
}

bool file_exists(const char *filename)
{
    //function extracted from the web https://www.learnc.net/c-tutorial/c-file-exists/
    FILE *fp = fopen(filename, "r");
    bool doesExist = false;
    if (fp != NULL)
    {
        doesExist = true;
        fclose(fp); // close the file
    }
    return doesExist;
}


size_t getFileSize(const char *fileName) {
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);  // Move the file pointer to the end
    size_t fileSize = ftell(file);  // Get the position, which is the file size
    fclose(file);

    return fileSize;
}

size_t getItinerationFileSize(const char *fileName) {
    //FOR A REASON i CAN'T USE CEIL. Might be the order. 
    FILE *file = fopen(fileName, "rb");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);  // Move the file pointer to the end
    size_t fileSize = ftell(file);  // Get the position, which is the file size
    fclose(file);
    return (fileSize/1000);

}