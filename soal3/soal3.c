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

int iter = 1;

void createDir(char *dir) {
	struct stat st = {0};

	if (stat(dir, &st) == -1) {
		mkdir(dir, 0777);
	}
}

char *FileExtension(char *filename) {
    char *temp = strrchr(filename, '.');
    if(!temp || temp == filename) return "Unknown";
    return temp + 1;
}

void *MovetoDir(char *argv){
	char *path, cwd[300], fileExt[300], temp1[300], fix[300]; 
	path = (char *)argv;
	
	printf("path1 = %s\n", path);
	
	strcpy(cwd, "/home/adr01/Documents/SesiLab3/Soal3");
	
	printf("cwd = %s\n", cwd);

	char* file = basename(path);

	strcpy(temp1, FileExtension(file));
	
	printf("temp1 = %s\n", temp1);
	
	for(int i = 0; temp1[i]; i++){
		fileExt[i] = tolower(temp1[i]);
	}
	
	strcat(fix, cwd);
	strcat(fix, "/");
	strcat(fix,fileExt);
	
	printf("fileExt = %s\n", fix);
	
	createDir(fix);

	strcat(fix, "/");
	strcat(fix, file);
	
	printf("fileExt = %s\n", fix);
	
	int hasil = rename(path , fix); 

	if ( hasil == 0 ){
		printf ( "File %d : Berhasil Dikategorikan\n",iter);
		iter = iter+1;
	}
	    
	else{
		printf( "File %d : Sad, gagal :(\n", iter );
		iter = iter + 1;
	}	
}

void Folder(char *argv){
	int it=0; 
	struct dirent *dp;
   	DIR *dir = opendir(argv);

	if (!dir) return;
	
	pthread_t thread[500];
	
	while ((dp = readdir(dir)) != NULL) {
		char path[300];
		strcat(path, argv);
		strcat(path, dp->d_name);
		
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) continue;
		else if(strcmp(dp->d_name, __FILE__) == 0) continue;
		
		struct stat path_stat;
		stat(path, &path_stat);

		if(S_ISREG(path_stat.st_mode)) continue;
		
		it = it+1;
		
		pthread_create(&thread[it], NULL, MovetoDir,  (void *) path);
	}
	
	for (int b = 0; b < it; b++) pthread_join(thread[b], NULL);

	closedir(dir);
}

int main(int argc, char* argv[])
{
	if (strcmp(argv[1], "-f") == 0){
		int a;
		for(a=2; a<argc; a++){
			MovetoDir(argv[a]);
		}
	
	}
	else if(strcmp(argv[1], "-d") == 0){
		if(argc > 3){
			printf("Maaf hanya boleh 1 direktori\n");
		}
		else {
			Folder(argv[2]);
		}
	}
	else if (strcmp(argv[1], "*") == 0) {
		char cwd[300];
		getcwd(cwd, sizeof(cwd));
		strcat(cwd, "/");
		Folder(cwd);
		
	}
	else {
		printf("Maaf argumen salah\n");	
	}
}
