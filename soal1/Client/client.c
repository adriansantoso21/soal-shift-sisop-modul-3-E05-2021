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
    
    while(1)
	{
		printf("Perintah :\n 1. Register\n 2. Login\n");
		printf("Masukkan perintah:");
		scanf("%d" , &perintah);
		strcpy(message, "");
		strcpy(id_pass, "");
		
		if(perintah == 1) {
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
			
		}
		else if (perintah == 2) {
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
			}
			else if(strcmp(message, "true")== 0 ){
				printf("Proses login sukses\n");
				
				after_login:;
				memset( perintah2, 0, strlen(perintah2) );
				printf("Selamat datang di aplikasi buku\n");
				printf("Perintah :1. add\n4. see\n");
				printf("Masukkan pilihan : ");
				getchar();
				scanf("%s", perintah2);
				
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
					
				}
				
			}
			
		}
		

	}

	close(sock);
    return 0;
}
