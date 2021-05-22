#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#define PORT 8080

char buffer[1024];
char msg[1024];
bool isLogin = false;

int createClientSocket(struct sockaddr_in *address, struct sockaddr_in *serv_addr) {
    int fd;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(serv_addr, '0', sizeof(*serv_addr));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &(serv_addr->sin_addr)) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(fd, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    return fd;
}

const char *getFirstWord(char *str) {
    int i;
    char word[255] = "";
    for(i = 0; i < strlen(str); i++) {
        if(str[i] == ' ') break;
        word[i] = str[i];
    }
    char *wordptr = word;
    return wordptr;
}

const char *getAllNextWord(char *str) {
    int x = 0, isSpaceExist = 0;
    while(x < strlen(str)) {
        if(str[x] == ' ') {
            isSpaceExist = 1;
            break;
        }
        x++;
    }
    if(!isSpaceExist) return NULL;

    int i = 0, j = 0;
    char word[255] = "";

    while(str[i++] != ' ');

    while(i < strlen(str)) {
        word[j] = str[i];
        i++;
        j++;
    }

    if(strcmp(word, "") != 0) {
        char *wordptr = word;
        return wordptr;
    } else {
        return NULL;
    }
}

void command(char *msg, int client_fd) {
    int iter;
    if(!isLogin && (strcmp(msg, "login") == 0 || strcmp(msg, "register") == 0)) {
        iter = 3;
        
        // send: login / register
        send(client_fd, msg, strlen(msg), 0);
        
        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, 1024);
        printf("%s", buffer);

        //loop = 3-1
        while(--iter) {
            scanf(" %[^\n]s", msg);
            send(client_fd, msg, strlen(msg), 0);

            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, 1024);
            printf("%s", buffer);

            if(strcmp(buffer, "Login berhasil.\n") == 0)  {
                isLogin = true;
            }
        }
    }
    else if(isLogin && strcmp(msg, "add") == 0) {
        iter = 4;
        
        // send: add
        send(client_fd, msg, strlen(msg), 0);
        
        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, 1024);
        printf("%s", buffer);

        while(--iter) {
            scanf(" %[^\n]s", msg);
            send(client_fd, msg, strlen(msg), 0);

            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, 1024);
            printf("%s", buffer);
        }
    }
    else if(isLogin && strcmp(getFirstWord(msg), "download") == 0 && getAllNextWord(msg) != NULL) {
        // send: download [sesuatu] 
        send(client_fd, msg, strlen(msg), 0);

        // read " "
        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, 1024);

        // send: path client
        char cwd[255];
        getcwd(cwd, sizeof(cwd));
        send(client_fd, cwd, strlen(cwd), 0);

        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, 1024);
        printf("%s", buffer);
    }
    else if(isLogin && strcmp(getFirstWord(msg), "delete") == 0 && getAllNextWord(msg) != NULL) {
        // send: delete [sesuatu] 
        send(client_fd, msg, strlen(msg), 0);

        // delete gagal/berhasil ?
        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, 1024);
        printf("%s", buffer);
    }
    else if(isLogin && strcmp(msg, "see") == 0 || (strcmp(getFirstWord(msg), "find") == 0 && getAllNextWord(msg) != NULL)) {
        // send: see / find [sesuatu] 
        send(client_fd, msg, strlen(msg), 0);

        // list hasil see / find
        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, 1024);
        printf("%s", buffer);
    }
    else if(strcmp(msg, "exit") == 0) {
        // send: see / find [sesuatu] 
        send(client_fd, msg, strlen(msg), 0);
    }
    else {
        printf("command salah.\n");
    }
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in address, serv_addr;
    int client_fd = createClientSocket(&address, &serv_addr);

    // cek server penuh / kosong
    memset(buffer, 0, sizeof(buffer));
    read(client_fd, buffer, 1024);

    if(strcmp(buffer, "server_penuh") == 0) {
        printf("Mohon maaf server penuh\n");
        return 0;
    }
    
    // minta command
    while(strcmp(msg, "exit") != 0) {
        scanf(" %[^\n]s", msg);
        command(msg, client_fd);
    }

    return 0;
}