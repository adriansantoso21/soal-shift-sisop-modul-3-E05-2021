#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Perkalian untuk tiap baris dan tiap kolom
void *mult(void *arg)
{
    int *data = (int *)arg;
    int k = 0, i = 0;

    int x = data[0];
    for (i = 1; i <= x; i++)
        k += data[i] * data[i+x];
    
    int *p = (int*)malloc(sizeof(int));
    *p = k;

    pthread_exit(p);
}

int main()
{
    int matA[4][3];
    int matB[3][6];
    int row1 = 4;
    int col1 = 3;
    int row2 = 3;
    int col2 = 6;

    int i, j, k;    // variabel untuk looping

    printf("First matrix:\n");
    for (i = 0; i < row1; i++)
        for (j = 0; j < col1; j++)
            scanf("%d", &matA[i][j]);    
        
    printf("Second matrix:\n");
    for (i = 0; i < row2; i++)
        for (j = 0; j < col2; j++)
            scanf("%d", &matB[i][j]); 
    
    int max = row1*col2;

    // make thread array
    pthread_t *threads;
    threads = (pthread_t*)malloc(max*sizeof(pthread_t));

    int count = 0;
    int *data = NULL;
    for (i = 0; i < row1; i++)
    {
        for (j = 0; j < col2; j++)
        {
            data = (int *)malloc((20)*sizeof(int));
            data[0] = col1;

            for (k = 0; k < col1; k++)
                data[k+1] = matA[i][k];
            
            for (k = 0; k < row2; k++)
                data[k+col1+1] = matB[k][j];

            pthread_create(&threads[count++], NULL, mult, (void *)(data));
        }
    }
    
    // access shared memory
    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = (int *) shmat(shmid, NULL, 0);

    value[0] = row1;
    value[1] = col2;

    // result matrix
    printf("Multiplication result:\n");
    for (i = 0; i < max; i++)
    {
        void *k;
        
        pthread_join(threads[i], &k);

        int *p = (int *)k;
        value[i+2] = *p;
        printf("%-3d ", value[i+2]);
        if ((i + 1) % col2 == 0)
            printf("\n");
    }
    shmdt((void *) value);
    return 0;
}