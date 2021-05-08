#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int arr[4][6] = {0};

int fact(int n, int lim)
{
    if (n == lim)
        return 1;
    return n*fact(n-1, lim);
}

void *permutation(void *arg)
{
    int *val = (int *) arg;
    int i;
    if (val[2] == 0)
        arr[val[0]][val[1]] = 0;
    else
    {
        int a = arr[val[0]][val[1]];
        if (a >= val[2])
        {
            arr[val[0]][val[1]] = fact(a, a-val[2]);
        }
        else if ( val[2] > a)
        {
            arr[val[0]][val[1]] = fact(a, 0);
        }
    }
}

int main()
{
    // access shared memory
    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = (int *) shmat(shmid, NULL, 0);

    int i, j;
    int max = value[0] * value[1];

    int row = 0, col = 0;

    // print shared matrix
    printf("Shared matrix:\n");
    for (i = 0; i < max; i++)
    {
        printf("%-3d ", value[i+2]);
        int num = value[i+2];
        arr[row][col] = num;
        //printf("%-3d ", arr[row][col]);
        if((i + 1) % 6 == 0)
        {
            row++;
            col = 0;
        }
        else
            col++;
        
        if ((i + 1) % value[1] == 0)
            printf("\n");
    }

    // create new matrix
    int newMat[4][6];
    int r1 = 4;
    int c1 = 6;

    // input by user
    printf("New matrix:\n");
    for (i = 0; i < r1; i++)
        for (j = 0; j < c1; j++)
            scanf("%d", &newMat[i][j]);

    // make thread array
    int count;
    pthread_t t_id[r1 * c1];

    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c1; j++)
        {
            int val[3];
            val[0] = i;
            val[1] = j;
            val[2] = newMat[i][j];

            pthread_create(&t_id[count], NULL, permutation, (void *)val);

            pthread_join(t_id[count++], NULL);
        }
    }

    printf("Result:\n");
    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c1; j++)
        {
            
            printf("%-3d ", arr[i][j]);
        }
        printf("\n");
    }
    
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}