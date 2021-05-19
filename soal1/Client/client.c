#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread, perintah;
    struct sockaddr_in serv_addr;
    char server_reply[2000], id_pass[1000], id[1000], pass[1000], message[1000], perintah2[1000];
    char buffer[1024] = {0};
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

	// Connect to remote server  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    char status[100];
	recv(sock , status , 100 , 0);
	if(strcmp(status, "penuh")==0){
		printf("Mohon maaf telah ada user yang login. Tunggu lagi nanti\n");
		memset(status, 0, strlen(status));
	}
	else{
		    while(1)
		{
			first_login:;
			printf("Perintah :\n 0. register\n 1. login\n");
			printf("Masukkan perintah:");
			memset( message, 0, strlen(message) );
			memset( id_pass, 0, strlen(id_pass) );
			scanf("%s", message);
			
			if(strcmp(message, "register")== 0 ) {
				strcpy(id_pass, "r");
				printf("Masukkan id : ");
				getchar();
				scanf("%s", id);
				strcat(id_pass, id);
				strcat(id_pass, "\t");
				printf("Masukkan password : ");
				getchar();
				scanf("%s", pass);
				strcat(id_pass, pass);
				
				send(sock, id_pass, strlen(id_pass), 0);
				recv(sock , message , 1000 , 0);
				printf("%s\n", message);
				
				goto first_login;
			}
			else if (strcmp(message, "login")== 0) {
				
				
				strcpy(id_pass, "l");
				printf("Masukkan id : ");
				getchar();
				scanf("%s", id);
				strcat(id_pass, id);
				strcat(id_pass, "\t");
				printf("Masukkan password : ");
				getchar();
				scanf("%s", pass);
				strcat(id_pass, pass);
				
				send(sock, id_pass, strlen(id_pass), 0);
				recv(sock , message , 1000 , 0);
				
				if(strcmp(message, "false")== 0 ){
					printf("Proses login gagal\n");
					goto first_login;
				}
				else if(strcmp(message, "true")== 0 ){
					printf("Proses login sukses\n");
					
					after_login:;
					memset( perintah2, 0, strlen(perintah2) );
					printf("\nSelamat datang di aplikasi buku\n");
					printf("Perintah :\n0.exit\n1. add\n4. see\n");
					printf("Masukkan pilihan : ");
					getchar();
					scanf("%s", perintah2);
					
					if(strcmp(perintah2, "exit") == 0 ) {
						send(sock, "exit", 10, 0);
						printf("Terima kasih telah menggunakan aplikasi\n");
						close(sock);
						//goto first_login;
					}
					
					if(strcmp(perintah2, "add") == 0 ) {
						char publisher[1000],filepath[1000], c_message[1000], tahun[1000];
						
						strcpy(c_message, "a");
						printf("Masukkan Publisher : ");
						getchar();
						scanf("%s", publisher);
						strcat(c_message, publisher);
						strcat(c_message, "\t");
						printf("Masukkan Tahun publikasi : ");
						getchar();
						scanf("%s", tahun);
						strcat(c_message, tahun);
						strcat(c_message, "\t");
						printf("Masukkan Filepath : ");
						getchar();
						scanf("%s", filepath);
						
						strcat(c_message, filepath);
						
						printf("ini message add : %s\n", c_message);
						
						send(sock, c_message, strlen(c_message), 0);
						recv(sock , c_message , 1000 , 0);
						printf("ini c_message : %s\n", c_message);
						goto after_login;
					}
					
					if(strcmp(perintah2, "see") == 0) {
						char c_message[1000];
						memset(c_message, 0, 1000);
						strcpy(c_message, "s");
						send(sock, c_message, strlen(c_message), 0);
						recv(sock , c_message , 1000 , 0);
						
						while( (strcmp(c_message,"File berhasil ditampilkan") != 0) && (strcmp(c_message,"File tidak ada yang ditampilkan") != 0) ) {
							printf("%s",c_message);
							memset(c_message, 0, 1000);
							recv(sock , c_message , 1000 , 0);
						}
						printf("%s", c_message);
						goto after_login;
					}
					
				}
				
			}
			else {
				printf("Perintah yang dimasukkan salah\n");
				goto first_login;
			}

		}
	}
    


	close(sock);
    return 0;
}
