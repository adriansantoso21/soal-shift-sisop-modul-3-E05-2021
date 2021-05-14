#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <limits.h>
#include <dirent.h> 
#include <libgen.h>
#include <ctype.h>
#include <pthread.h>

int sukses, hasil;

void createDir(char *dir) {
	struct stat st = {0};

	if (stat(dir, &st) == -1) {
		mkdir(dir, 0777);
	}
}

char *FileExtension(char *filename) {
    if(filename[0] == '.') return "Hidden"; 
    char *temp = strchr(filename, '.');
    if(!temp) return "Unknown";
    if(temp[1] == '.') return "Hidden"; 
    return temp + 1;
}

void *MovetoDir(void *argv){
	char *path, cwd[300], fileExt[300], temp1[300], fix[300]; 
	
	path = (char *)argv;	
	getcwd(cwd, sizeof(cwd));

	char* file = basename(path);
	strcpy(fileExt, FileExtension(file));
	
	for(int i = 0; i < strlen(fileExt); i++){
		fileExt[i] = tolower(fileExt[i]);
	}
	
	strcpy(fix,"");strcat(fix, cwd);strcat(fix, "/");strcat(fix,fileExt);
	createDir(fix);
	strcat(fix, "/");strcat(fix, file);
	
	hasil = rename(path , fix); 
}

void Folder(char *argv){
	int it=-1; 
	struct dirent *dp;
   	DIR *dir = opendir(argv);
	
	pthread_t thread[1000];
	
	while ((dp = readdir(dir)) != NULL) {
		char path[300];
		
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
			if(dp->d_type == DT_REG) {
				
				strcpy(path,"");strcat(path, argv);strcat(path, "/");strcat(path, dp->d_name);
				pthread_create(&thread[++it], NULL, MovetoDir, (void *) path);
				pthread_join(thread[it], NULL);
			}
			else if(dp->d_type == DT_DIR) {
				struct dirent *dp0;
   				DIR *dir0 = opendir(argv);
				char path0[300];
				strcpy(path0,"");strcat(path0, argv);strcat(path0, "/");strcat(path0, dp->d_name);
				Folder(path0);
   				closedir(dir0);
			}
		}
	}

	closedir(dir);
}

int main(int argc, char* argv[])
{
	// Untuk file
	if (strcmp(argv[1], "-f") == 0){
		int a,itr = -1;
		pthread_t tid[argc-2];
		for(a=2; a<argc; a++){
			pthread_create(&tid[++itr], NULL, MovetoDir, (void *) argv[a]);
			
			if ( hasil == 0 ){
				printf ( "File %d : Berhasil Dikategorikan\n",a-1);
			}
			    
			else{
				printf( "File %d : Sad, gagal :(\n", a-1 );
			}
		}
		for(int a=0; a<=itr; a++) pthread_join(tid[a],NULL);
	
	}
	
	// Untuk direktori
	else if(strcmp(argv[1], "-d") == 0){
		if(argc > 3){
			printf("Maaf hanya boleh 1 direktori\n");
		}
		else {
			if( ENOENT != errno ) {
				Folder(argv[2]);
				printf("Direktori sukses disimpan!\n");
			}
			else {
				printf("Yah, gagal disimpan :(\n");
			}
		}
	}
	
	// Untuk file dalam current directory
	else if (strcmp(argv[1], "*") == 0) {
		char cwd[300];
		getcwd(cwd, sizeof(cwd));
		Folder(cwd);
		
	}
	
	// JIka semua argumen tidak ada yang memenuhi
	else {
		printf("Maaf argumen salah\n");	
	}
}
