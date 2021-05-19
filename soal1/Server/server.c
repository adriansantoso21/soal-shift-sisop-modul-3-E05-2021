#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <libgen.h>
#define PORT 8080


typedef struct akun {
	char id[1000];
	char pass[1000];
} akun;

typedef struct buku {
	char publisher[1000];
	char tahun[1000];
	char path[1000];
	char nama[1000];
	char extension[1000];
} buku;

int jumlah_akun = -1, masuk=0, jumlah_buku = -1;
char status[1000], message[2000], message1[2000];
akun client_akun[1000];
buku client_buku[1000];

char *FileExtension(char *filename) {
    char *temp = strrchr(filename, '.');
    return temp + 1;
}

void moveFiles(char file_path[], char nama_file[]){

	// Mengambil data dari file asal	
	FILE *file;
	char line[10000], all[10000];
	memset(line, 0, 10000);memset(all, 0, 10000);
	
	file = fopen(file_path, "r");
	
	// Membuat folder FILES
  	char pathtujuan[1000];
  	memset(pathtujuan, 0, 1000);
  	strcpy(pathtujuan, "/home/adr01/Documents/SesiLab3/Soal1/Server/FILES/");
  	strcat(pathtujuan, nama_file);
  	
  	// Menuliskan isi folder
	FILE *fp;
	fp = fopen (pathtujuan, "w");
	char temp = fgetc(file);
	while(temp != EOF){
		fputc(temp, fp);
		temp = fgetc(file);
	}
	fclose(file);
	fclose(fp);
}

void add_tsvfile(char message[]){
	char file[1000];
	int i = -1;
	
	memset( file, 0, strlen(file) );
	
	for(int c=1; c<strlen(message); c++) {
		file[++i] = message[c];
	}
	
	FILE *f1;
	f1 = fopen("files.tsv","a");
    fprintf(f1,"%s\n",file);
    fclose(f1);
}

void createDir(char *dir) {
	struct stat st = {0};

	if (stat(dir, &st) == -1) {
		mkdir(dir, 0777);
	}
}

void register_user(char id_user[], char pass_user[]){
	jumlah_akun++;
	
	strcpy(client_akun[jumlah_akun].id, id_user);
	strcpy(client_akun[jumlah_akun].pass, pass_user);
	
	FILE *fptr;
	fptr = fopen("akun.txt","a");
    fprintf(fptr,"%s : %s\n",client_akun[jumlah_akun].id, client_akun[jumlah_akun].pass);
    fclose(fptr);
}

bool login_user(char id_user[], char pass_user[]){
	int flag=0;
	for(int b=0; b<=jumlah_akun; b++){
		if( (strcmp(client_akun[b].id, id_user) ==0) && (strcmp(client_akun[b].pass, pass_user) ==0)) {
			masuk = 1;
			flag=1;
			break;
		}
	}
	if (flag==1) return true;
	return false;
}


// Handle multiple clients
void *multiple_connection (void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	bool status;
	memset( message, 0, strlen(message) );
	if(masuk) send(sock , "penuh" , 10 , 0 );
	else send(sock , "kosong" , 10 , 0 );
	memset( message, 0, strlen(message) );
	
	// Menerima pesan dari user apakah dia login / register
	while( (read_size = recv(sock , message , 2000 , 0)) > 0 )
	{
		
		if(message[0] == 'r' || message[0] == 'l') {
			int iter = -1, iter1 = -1, i, a;
			char id_user[1000], pass_user[1000];
			
			strcpy(id_user, "");strcpy(pass_user, "");
				
			for(i=1; i<=strlen(message); i++){
				if(message[i] != '\t') id_user[++iter] = message[i];
				else break;
			}
			
			for(a=i+1; a<=strlen(message); a++){
				pass_user[++iter1] = message[a];
			}
			
			// Jika pilihan register
			if(message[0] == 'r'){
				register_user(id_user, pass_user);
				memset( message, 0, strlen(message) );
				strcpy(message, "Proses register berhasil\n");
				send(sock , message , strlen(message) , 0 );
			}
			
			// Jika pilihan login
			else if (message[0] == 'l'){
				status = login_user(id_user, pass_user);
				
				if(status) send(sock , "true" , 1000 , 0 );
				else send(sock , "false" , 1000 , 0 );
				
				memset( message, 0, strlen(message) );
				recv(sock , message , 2000 , 0);
				if(message[0] == 'a') {
				
					// Mengambil data publisher, tahun, path
					int it1=-1, it2=-1, it3=-1, d, e, f;
					char publisher[1000], tahun[1000], path[1000], namaFile[1000], fileExt[1000], temp[1000];
					
					for(d=1; d<strlen(message); d++){
						if(message[d] != '\t') publisher[++it1] = message[d];
						else break;
					}
					
					for(e=d+1; e<=strlen(message); e++){
						if(message[e] != '\t') tahun[++it2] = message[e];
						else break;
					}
					
					for(f=e+1; f<=strlen(message); f++){
						path[++it3] = message[f];
					}
					
					strcpy(namaFile, basename(path));
					
					strcpy(fileExt, FileExtension(namaFile));
					
					++jumlah_buku;
					
					strcpy(client_buku[jumlah_buku].publisher, publisher);
					strcpy(client_buku[jumlah_buku].path, path);
					strcpy(client_buku[jumlah_buku].tahun, tahun);
					strcpy(client_buku[jumlah_buku].nama, namaFile);
					strcpy(client_buku[jumlah_buku].extension, fileExt);
					
					add_tsvfile(message);
					moveFiles(path, namaFile);
					send(sock , "File berhasil dimasukkan" , 1000 , 0 );
				}
				
				// Jika pilihan see
				if(message[0] == 's') {
					if(jumlah_buku == -1) send(sock , "File tidak ada yang ditampilkan\n" , 1000 , 0 );
					else {
						char publisher[1000], tahun[1000], path[1000], namaFile[1000], fileExt[1000], temp[1000];
						for(int a=0; a<=jumlah_buku; a++){
							memset( namaFile, 0, strlen(namaFile) );
							memset( publisher, 0, strlen(publisher) );
							memset( tahun, 0, strlen(tahun) );
							memset( fileExt, 0, strlen(fileExt) );
							memset( path, 0, strlen(path) );
							
							strcpy(namaFile, "Nama: ");strcat(namaFile, client_buku[a].nama);strcat(namaFile, "\n");
							
							strcpy(publisher, "Publisher: ");strcat(publisher, client_buku[a].publisher);strcat(publisher, "\n");
							
							strcpy(tahun, "Tahun publishing: ");strcat(tahun, client_buku[a].tahun);strcat(tahun, "\n");
							
							strcpy(fileExt, "Ekstensi file: ");strcat(fileExt, client_buku[a].extension);strcat(fileExt, "\n");
							
							strcpy(path, "Filepath: ");strcat(path, client_buku[a].path);strcat(path, "\n");
							
							printf("Ini nama : %s\n", namaFile);
							printf("Ini publisher : %s\n", publisher);
							printf("Ini tahun : %s\n", tahun);
							printf("Ini ext : %s\n", fileExt);
							printf("path : %s\n", path);
							
							send(sock , namaFile , 1000 , 0 );
							send(sock , publisher , 1000 , 0 );
							send(sock , tahun , 1000 , 0 );
							send(sock , fileExt , 1000 , 0 );
							send(sock , path , 1000 , 0 );
							send(sock , "\n" , 1000 , 0 );
						}
						send(sock , "File berhasil ditampilkan\n" , 1000 , 0 );
					}
				}
			}
			
			memset( message, 0, strlen(message) );
		}
		if(strcmp(message, "exit")){
			masuk = 0;
			fflush(stdout);
		}
	}
	
	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread, c, *new_sock, client_sock;
    struct sockaddr_in address, client;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    //Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

	// Prepare the sockaddr_in structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
      
    // Bind   
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // Membuat files.tsv
    FILE *filetsv;
    filetsv = fopen("files.tsv", "a+");
    fprintf(filetsv, "Publisher\tTahun publikasi\tFilepath\n");
    fclose(filetsv);
    
    // Membuat folder FILES
    char cwd[1000];
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/FILES");
    createDir(cwd);

	// Menerima banyak client
	c = sizeof(struct sockaddr_in);
	while( (client_sock = accept(server_fd, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Client baru diterima");
		
		pthread_t tid;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &tid , NULL ,  multiple_connection, (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}

	}

    return 0;
}
