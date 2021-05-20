# soal-shift-sisop-modul-3-E05-2021

## Soal nomor 2
Soal no 2 meminta kita untuk melakukan tugas sebagai berikut:\
**a.** Membuat program perkalian matriks 4x3 dengan matriks 3x6 dan menampilkan hasilnya dengan catatan semua matriks berasal dari input ke program. Matriks nantinya akan berisi angka 1-20.\
**b.** Membuat program menggunakan matriks output dari program soal 2a. Kemudaian matriks hasil tersebut akan dilakukan perhitungan dengan matriks baru (input user). Perhitungannya adalah setiap cel matriks A menjadi angka untuk faktorial , lalu cel dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil). 
```
Ketentuan:
If a>=b -> a!/(a-b)!
If b>a -> a!
If 0 -> 0
```
**c.** Membuat program untuk mengecek 5 proses teratas apa saja yang memakan resource komputer dengan command `ps aux | sort -nrk 3,3 | head -5` dan `harus menggunakan IPC Pipes.`
## Penjelasan penyelesaian soal
### 2a
Soal 2a adalah program yang menghitung perkalian matriks dengan setiap perkaliannya dijalankan menggunakan `thread` kemudian memasukkan hasilnya  ke `shared memory`. Karena soal ini berhubungan dengan soal 2b dimana output dari 2a akan dipakai oleh 2b. Fungsi utama dari program ini adalah `mult` yang mana fungsi ini akan melakukan perkalian pada setiap baris matriks pertama dengan setiap kolom matriks kedua. Fungsi tersebut menerima inputan berupa array yang mempunyai data bertipe integer yang berisi panjang baris matriks pertama, panjan kolom matriks kedua, elemen pada kolom pertama sepanjang baris pada matriks pertama, dan elemen pada baris pertama sepanjang kolom pada matriks kedua.
```
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
```
Kemudian setiap elemen pada matriks merupakan hasil input user ke program. Kodenya sebagai berikut:
```
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

    ...

    return 0;
}
```
Lalu, kami membuat variabel array void data bertujuan untuk menampung setiap elemen pada baris i matriks pertama dan kolom j matriks kedua. Selanjutnya `thread` dibuat dan menjalankan fungsi `mult` dengan passing variabel data untuk setiap i dan j. Kodenya sebagai berikut:
```
int main()
{

    ...
    
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
    
    ...
    
    return 0;
}
```
Selanjutnya kami menggunakan `shared memory` agar bisa menjawab soal 2b dimana memory yang dishare adalah array pointer to integer `value`. Kodenya sebagai berikut:
```
int main()
{
    
    ...
    
    // access shared memory
    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = (int *) shmat(shmid, NULL, 0);

    value[0] = row1;
    value[1] = col2;

    ...
    
    return 0;
}
```
Kemudian thread dijoin satu persatu lalu ditampung ke variable `void *k`. Selanjutnya mencetak hasil dari perkalian kedua matriks tersebut. Berikut kode programnya:
```
int main()
{
    
    ...

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
```
![image](https://drive.google.com/uc?export=view&id=1TkTjyz9v4kRfbSwIssApS1VG8YeLLrBg)
### 2b
Soal 2b menerima hasil output dari soal 2a dengan cara membaca `shared memory` kemudian meminta input matriks baru dan melakukan operasi sesuai dengan ketentuan soal. Pertama, kami membuat fungsi `fact` untuk mengoperasikan factorial. Berikut kodenya:
```
unsigned long long fact(int n, int lim)
{
    if (n == lim)
        return 1;
    return n*fact(n-1, lim);
}
```
Selanjutnya, kami membuat fungsi `permutation` yang mengoperasikan ketentuan-ketentuan yang diminta soal. Fungsi ini menerima inputan berupa array pointer to integer `val`. Hasil dari operasi akan disimpan dalam variabel array berukuran 4x6. Berikut kodenya:
```
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
```
Pada fungsi main hal pertama yang dilakukan adalah mengambil shared memory dari output soal 2a kemudian menyimpannya di variabel pointer to integer `value`. Selanjutnya matriks hasil shared memory tersebut akan ditampilkan. Kodenya sebagai berikut:
```
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

    ...

    return 0;
}
```
Langkah selanjutnya yaitu menginputkan matriks baru yang akan digunakan sebagai batas maksimal faktorialnya. Kemudian untuk setiap elemen pada matriksnya akan dioperasikan menggunakan fungsi `permutation` dengan menggunakan thread dan selanjutnya threadnya dijoin. Berikut kodenya:
```
int main()
{
    
    ...

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

   ...

    return 0;
}
```
Terakhir, mencetak hasil dari operasi matriks tersebut. Berikut kodenya:
```
int main()
{
    
    ...

    printf("Result:\n");
    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c1; j++)
        {
            printf("%-3llu ", arr[i][j]);
        }
        printf("\n");
    }
    
    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
```
![image](https://drive.google.com/uc?export=view&id=1CgGyskHWU4pBPqrAqpGZlxa-F_CUofbs)
### 2c
Soal 2c meminta kita membuat program untuk mengecek 5 proses teratas apa saja yang memakan resource komputer menggunakan `pipe` dan `fork-exec` dengan command `ps aux | sort -nrk 3,3 | head -5`. Terdapat 2 pipe dan 3 fork untuk mengeksekusi perintah ini. Pertama create pipe1 selanjutnya melakukan fork untuk menjalankan command `ps aux`. 
```
void main() 
{
  pid_t pid1, pid2, pid3;
  int pipe1[2], pipe2[2];

  if (pipe(pipe1) == -1) 
  {
    perror("bad pipe1");
    exit(1);
  }
  
  pid1 = fork();
  if (pid1 == -1) 
  {
    perror("fork1 error");
    exit(1);
  } 

  else if (pid1 == 0) 
  {
    dup2(pipe1[1], 1);
    close(pipe1[0]);
    close(pipe1[1]);
    char *argv1[] = {"ps", "aux", NULL};
          execv("/bin/ps", argv1);
    perror("ps aux execute doesn't work");
    _exit(1);
  }

  ...
  
}
```
Kedua create pipe2 dan melakukan fork untuk menjalankan command `sort -nrk 3,3`. 
```
void main() 
{

  ...

  if (pipe(pipe2) == -1) 
  {
    perror("bad pipe2");
    exit(1);
  }

  pid2 = fork();
  if (pid2 == -1) 
  {
    perror("fork2 error");
    exit(1);
  } 

  else if (pid2 == 0) 
  {
    dup2(pipe1[0], 0);
    dup2(pipe2[1], 1);
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    char *argv2[] = {"sort", "-nrk", "3,3", NULL};
          execv("/bin/sort", argv2);
    perror("sort execute doesn't work");
    _exit(1);
  }

  close(pipe1[0]);
  close(pipe1[1]);

  ...
  
}
```
Ketiga melakukan fork untuk menjalankan command `head -5`. Berikut kode programnya:
```
void main() 
{
  ...

  pid3 = fork();
  if (pid3 == -1) 
  {
    perror("fork3 error");
    exit(1);
  } 
  
  else if (pid3 == 0) 
  {
    dup2(pipe2[0], 0);
    close(pipe2[0]);
    close(pipe2[1]);
    char *argv3[] = {"head", "-5", NULL};
          execv("/bin/head", argv3);
    perror("head execute doesn't work");
    _exit(1);
  }
  _exit(0);
}
```
![image](https://drive.google.com/uc?export=view&id=1z1phTbLJxxRbuGRD8ipKOy9mBzUeg4fG)

Kendala yang dialami:
1. Mengalami kesulitan saat mengimplementasikan thread dan shared memory saat mengerjakan soal 2a dan 2b.
2. Beberapa error terjadi saat mengerjakan soal 2c karena salah saat mengimplementasikan pipe.
