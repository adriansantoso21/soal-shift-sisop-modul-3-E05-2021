#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#define PORT 8080

char currPath[255]; //path untuk "../Server/"
char txtPath[255]; //path untuk akun.txt

char buffer[1024] = ""; //untuk read
char msg[1024] = ""; //untuk send

bool isClientExist = false;


int _getNumberOfLine(char *path) {
    FILE *fp = fopen(path, "r");

    int count_lines = 0;
    char chr = getc(fp);
    while (chr != EOF) {
        //Count whenever new line is encountered
        if (chr == '\n')
            count_lines++;
        //take next character from file.
        chr = getc(fp);
    }

    fclose(fp);
    return count_lines;
}

int _isUserExist(char *user, char *path) {
    FILE *fp = fopen(path, "r");

    char buff[255];
    int iter = _getNumberOfLine(path);
    while(iter--) {
        fscanf(fp, "%[^:]s", buff);
        if(strcmp(buff, user) == 0) {
            fclose(fp);
            return 1;
        }
        fscanf(fp, "%[^\n]s", buff);
        getc(fp);
    }

    fclose(fp);
    return 0;
}

/*
 * Check if a file exist using fopen() function
 * return 1 if the file exist otherwise return 0
 */
int _isFileExists(const char *fPath){
    /* try to open file to read */
    FILE *file;
    if (file = fopen(fPath, "rb")){
        fclose(file);
        return 1;
    }
    return 0;
}

char *_getFilenameExt(const char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

char *_getFileName(char *path) {
    char *path2 = strdup(path);
    char *bname = basename(path2);
    return bname;
}

// gunakan hanya di main
char *strCt(char *str1, char *str2) {
    static char buffer[255];
    sprintf(buffer, "%s%s", str1, str2);
    return buffer;
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


// fungsi A-H: 

// create socket, retrun fd (server)
int createServerSocket(struct sockaddr_in *address, int *addrlen) {
    int fd, opt = 1;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);
    *addrlen = sizeof(*address);

    if (bind(fd, (struct sockaddr *)address, *addrlen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return fd;
}

// butuh: _isUserExist()
void registerUser(char *path, int socket) {
    FILE *fp = fopen(path, "a+");

    char username[100], password[100];

    strcpy(msg, "Username:\n");
    send(socket, msg, strlen(msg), 0);
    
    memset(username, 0, sizeof(username));
    read(socket, username, sizeof(username));

    strcpy(msg, "Password:\n");
    send(socket, msg, strlen(msg), 0);

    memset(password, 0, sizeof(password));
    read(socket, password, sizeof(password));

    if(!_isUserExist(username, path)) {
        fprintf(fp, "%s:%s\n", username, password);
        strcpy(msg, "Account registered.\n");
        send(socket, msg, strlen(msg), 0);
    } else {
        strcpy(msg, "Username sudah ada.\n");
        send(socket, msg, strlen(msg), 0);
    }

    fclose(fp);
}

// butuh: _getNumberOfLine(),
char *loginUser(char *path, int socket) {
    FILE *fp = fopen(path, "r");

    char username[100], password[100];
    static char userpass[255];
    
    strcpy(msg, "Username:\n");
    send(socket, msg, strlen(msg), 0);
    
    memset(username, 0, sizeof(username));
    read(socket, username, sizeof(username));

    strcpy(msg, "Password:\n");
    send(socket, msg, strlen(msg), 0);
    
    memset(password, 0, sizeof(password));
    read(socket, password, sizeof(password));

    sprintf(userpass, "%s:%s", username, password);

    char buff[255];
    int iter = _getNumberOfLine(path);
    while(iter--) {
        fscanf(fp, "%[^\n]s", buff);
        if(strcmp(userpass, buff) == 0) {
            strcpy(msg, "Login berhasil.\n");
            send(socket, msg, strlen(msg), 0);
            fclose(fp);
            return userpass;
        }
        getc(fp);
    }

    strcpy(msg, "Login gagal.\n");
    send(socket, msg, strlen(msg), 0);
    fclose(fp);
    return NULL;
}

// butuh: _isFileExist()
int copyFile(char *pathIn, char *pathOut) {
    // pathIn: path file yang dicopy
    if(!_isFileExists(pathIn)) {
        return 0; // return 0 jika tidak ada file
    }

    FILE *exein, *exeout;
    exein = fopen(pathIn, "rb");
    if (exein == NULL) {
        /* handle error */
        perror("file open for reading");
        exit(EXIT_FAILURE);
    }
    exeout = fopen(pathOut, "wb");
    if (exeout == NULL) {
        /* handle error */
        perror("file open for writing");
        exit(EXIT_FAILURE);
    }

    size_t n, m;
    unsigned char buff[8192];
    do {
        n = fread(buff, 1, sizeof buff, exein);
        if (n)
            m = fwrite(buff, 1, n, exeout);
        else
            m = 0;
    } while ((n > 0) && (n == m));

    if (m) perror("copy");
    if (fclose(exeout)) perror("close output file");
    if (fclose(exein)) perror("close input file");

    return 1; // return 1 jika berhasil
}

// butuh: _getFileName(), _getFilenameExt(), copyFile()
void addFiles(char *tsvPath, char* userpass, int socket) {
    char pub[100], thn[10], fpath[255];
    strcpy(msg, "Publisher: ");
    send(socket, msg, strlen(msg), 0);
    memset(pub, 0, sizeof(pub));
    read(socket, pub, sizeof(pub));

    strcpy(msg, "Tahun Publikasi: ");
    send(socket, msg, strlen(msg), 0);
    memset(thn, 0, sizeof(thn));
    read(socket, thn, sizeof(thn));

    strcpy(msg, "Filepath: ");
    send(socket, msg, strlen(msg), 0);
    memset(fpath, 0, sizeof(fpath));
    read(socket, fpath, sizeof(fpath));

    char *fname, *fext;
    fname = _getFileName(fpath);
    fext = _getFilenameExt(fname);

    // copy file dari client ke server
    char fserverPath[255];
    sprintf(fserverPath, "%s/%s/%s", currPath, "FILES", fname);
    if(_isFileExists(fserverPath) == 1) {
        // file sudah ada di server
        strcpy(msg, "file sudah ada di server.\n");
        send(socket, msg, strlen(msg), 0);
        return;
    } else if(copyFile(fpath, fserverPath) == 0) {
        // file tidak ada di client
        strcpy(msg, "file tidak ada di client.\n");
        send(socket, msg, strlen(msg), 0);
        return;
    }

    // update files.tsv
    FILE *fp = fopen(tsvPath, "a+");
    fprintf(fp, "%s\t%s\t%s\t%s\t%s\n", fname, pub, thn, fext, fpath);

    sprintf(msg, "file berhasil ditambahkan.\n");
    send(socket, msg, strlen(msg), 0);
    fclose(fp);

    // update running.log
    char rlogPath[255];
    sprintf(rlogPath, "%s/%s", currPath, "running.log");
    fp = fopen(rlogPath, "a+");
    fprintf(fp, "Tambah: %s (%s)\n", fname, userpass);
    fclose(fp);
}

// butuh: copyFile()
void downloadFile(char *fname, int socket) {
    // send " " (supaya ga ngebug karena read 2x sekaligus)
    send(socket, " ", strlen(" "), 0);

    char fserverPath[255] = "";
    sprintf(fserverPath, "%s/%s/%s", currPath, "FILES", fname);

    char fclientPath[255] = "";
    memset(fclientPath, 0, sizeof(fclientPath));
    read(socket, fclientPath, sizeof(fclientPath));
    sprintf(fclientPath, "%s/%s", fclientPath, fname);

    if(copyFile(fserverPath, fclientPath) == 0) {
        strcpy(msg, "file tidak ada di server\n");
        send(socket, msg, strlen(msg), 0);
    } else {
        strcpy(msg, "download berhasil\n");
        send(socket, msg, strlen(msg), 0);
    }
}

// butuh: _getNumberOfLine(), _isFileExists()
void deleteFile(char *fname, char* login, int socket) {
    // cek apakah file ada?
    char fPath[255], cwd[255], tsvPath[255];
    getcwd(cwd, sizeof(fPath));
    sprintf(fPath, "%s/%s/%s", cwd, "FILES", fname);
    sprintf(tsvPath, "%s/%s", cwd, "files.tsv");

    if(!_isFileExists(fPath)) {
        strcpy(msg, "file tidak ada.\n");
        send(socket, msg, strlen(msg), 0);
        return; // return jika tidak ada
    }

    // delete line di files.tsv
    FILE *fp = fopen(tsvPath, "r");
    char buff[255];
    char newtsv[1024] = "";
    int iter = _getNumberOfLine(tsvPath);
    while(iter--) {
        fscanf(fp, "%[^\t]s", buff);
        if(strcmp(buff, fname) != 0) {
            strcat(newtsv, buff);
            fscanf(fp, "%[^\n]s", buff);
            sprintf(buff, "%s\n", buff);
            strcat(newtsv, buff);
            getc(fp);
        } else {
            fscanf(fp, "%[^\n]s", buff);
            getc(fp);
        }
    }
    fclose(fp);

    fp = fopen(tsvPath, "w");
    fprintf(fp, "%s", newtsv);
    fclose(fp);

    // rename file: old-fname
    char oldname[255];
    strcpy(oldname, fPath);
    char newname[255];
    sprintf(newname, "%s/%s/old-%s", cwd, "FILES", fname);

    if(rename(oldname, newname) == 0) {
        strcpy(msg, "delete/rename berhasil.\n");
        send(socket, msg, strlen(msg), 0);
    } else {
        strcpy(msg, "delete/rename gagal.\n");
        send(socket, msg, strlen(msg), 0);
    }

    // update running.log
    char rlogPath[255];
    sprintf(rlogPath, "%s/%s", cwd, "running.log");
    fp = fopen(rlogPath, "a+");
    fprintf(fp, "Hapus: %s (%s)\n", fname, login);
    fclose(fp);
}

// save to array and then print
// butuh: _getNumberOfLine(),
void seeTsv(char *tsvPath, int socket) {
    FILE *fp = fopen(tsvPath, "r");

    char buff[255];
    char buff2[255];
    char tsv[1024] = "";
    int iter = _getNumberOfLine(tsvPath);
    while(iter--) {
        fscanf(fp, "%[^\t]s", buff);
        sprintf(buff2, "Nama: %s\n", buff);
        strcat(tsv, buff2);
        getc(fp);

        fscanf(fp, "%[^\t]s", buff);
        sprintf(buff2, "Publisher: %s\n", buff);
        strcat(tsv, buff2);
        getc(fp);

        fscanf(fp, "%[^\t]s", buff);
        sprintf(buff2, "Tahun publishing: %s\n", buff);
        strcat(tsv, buff2);
        getc(fp);

        fscanf(fp, "%[^\t]s", buff);
        sprintf(buff2, "Ekstensi File: %s\n", buff);
        strcat(tsv, buff2);
        getc(fp);

        fscanf(fp, "%[^\n]s", buff);
        sprintf(buff2, "Filepath: %s\n\n", buff);
        strcat(tsv, buff2);
        getc(fp);
    }
    fclose(fp);

    // semuanya di array tsv
    if(strcmp(tsv, "") == 0) {
        strcpy(msg, "tidak ditemukan hasil.\n");
        send(socket, msg, strlen(msg), 0);
    }
    else {
        send(socket, tsv, strlen(tsv), 0);
    }
}

// save to array and then print
// butuh: _getNumberOfLine(),
void findFromTsv(char *tsvPath, char *word, int socket) {
    FILE *fp = fopen(tsvPath, "r");

    char buff[255];
    char buff2[255];
    char tsv[1024] = "";
    int iter = _getNumberOfLine(tsvPath);
    while(iter--) {
        fscanf(fp, "%[^\t]s", buff);
        if(strstr(buff, word) == NULL) {
            fscanf(fp, "%[^\n]s", buff);
            getc(fp);
            continue;
        }
        sprintf(buff2, "Nama: %s\n", buff);
        strcat(tsv, buff2);
        getc(fp);

        fscanf(fp, "%[^\t]s", buff);
        sprintf(buff2, "Publisher: %s\n", buff);
        strcat(tsv, buff2);
        getc(fp);

        fscanf(fp, "%[^\t]s", buff);
        sprintf(buff2, "Tahun publishing: %s\n", buff);
        strcat(tsv, buff2);
        getc(fp);

        fscanf(fp, "%[^\t]s", buff);
        sprintf(buff2, "Ekstensi File: %s\n", buff);
        strcat(tsv, buff2);
        getc(fp);

        fscanf(fp, "%[^\n]s", buff);
        sprintf(buff2, "Filepath: %s\n\n", buff);
        strcat(tsv, buff2);
        getc(fp);
    }
    fclose(fp);

    // semuanya di array tsv
    if(strcmp(tsv, "") == 0) {
        strcpy(msg, "tidak ditemukan hasil.\n");
        send(socket, msg, strlen(msg), 0);
    }
    else {
        send(socket, tsv, strlen(tsv), 0);
    }
}

void* main_service(void *arg) {
    int socket = *(int *)arg;

    // cek ada client / tidak
    if(isClientExist) {
        strcpy(msg, "server_penuh");
        send(socket, msg, strlen(msg), 0);
        return NULL;
    } else {
        strcpy(msg, "server_kosong");
        send(socket, msg, strlen(msg), 0);
        isClientExist = true;
    }

    char req[100];
    bool exit = false;

    while(!exit) {
        memset(req, 0, sizeof(req));
        read(socket, req, sizeof(req));

        printf("*%s*\n", req);

        if(strcmp(req, "exit") == 0 || strcmp(req, "") == 0) {
            exit = true;
            isClientExist = false;
        }
        else if(strcmp(req, "register") == 0) {
            registerUser(txtPath, socket);
        }
        else if(strcmp(req, "login") == 0) {
            char *login = loginUser(txtPath, socket); // sukses-> "user:pass", gagal-> NULL

            while(login != NULL && !exit) {
                char longreq[100];
                memset(longreq, 0, sizeof(longreq));
                read(socket, longreq, sizeof(longreq));
                strcpy(req, getFirstWord(longreq));

                printf("**%s**\n", longreq);

                if(strcmp(req, "add") == 0) {
                    addFiles(strCt(currPath, "/files.tsv"), login, socket);
                }
                else if(strcmp(req, "download") == 0) {
                    char fname[50] = "";
                    if(getAllNextWord(longreq) != NULL){
                        strcpy(fname, getAllNextWord(longreq));
                        downloadFile(fname, socket);
                    }
                }
                else if(strcmp(req, "delete") == 0) {
                    char fname[50];
                    if(getAllNextWord(longreq) != NULL){
                        strcpy(fname, getAllNextWord(longreq));
                        deleteFile(fname, login, socket);
                    }
                }
                else if(strcmp(req, "see") == 0) {
                    seeTsv(strCt(currPath, "/files.tsv"), socket);
                }
                else if(strcmp(req, "find") == 0) {
                    char word[50];
                    if(getAllNextWord(longreq) != NULL){
                        strcpy(word, getAllNextWord(longreq));
                        findFromTsv(strCt(currPath, "/files.tsv"), word, socket);
                    }
                }
                else if(strcmp(req, "exit") == 0 || strcmp(req, "") == 0) {
                    exit = true;
                    isClientExist = false;
                }
            }
        }
    }

    return NULL;
}

int main() {
    // mendapatkan path saat ini
    getcwd(currPath, sizeof(currPath));
    sprintf(txtPath, "%s/%s", currPath, "/akun.txt");

    // membuat akun.txt
    FILE *fp = fopen(txtPath, "a+");
    fclose(fp);

    // membuat files.tsv
    fp = fopen(strCt(currPath, "/files.tsv"), "a+");
    fclose(fp);

    // membuat dir FILES
    int result = mkdir(strCt(currPath, "/FILES"), 0777);

    // membuat running.log
    fp = fopen(strCt(currPath, "/running.log"), "a+");
    fclose(fp);

    // membuat socket
    struct sockaddr_in address;
    int client_socket, addrlen;

    // menerima banyak client
    int server_fd = createServerSocket(&address, &addrlen);
    while((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0) {
        printf("client tersambung\n");

        // membuat thread
        pthread_t tid;
        int err = pthread_create(&(tid), NULL, &main_service, (void *)&client_socket);

        if(err != 0) printf("can't create thread : [%s]",strerror(err));
        else printf("create thread success\n");
    }

    return 0;
}