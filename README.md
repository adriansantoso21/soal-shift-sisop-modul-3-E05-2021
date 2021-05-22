# soal-shift-sisop-modul-3-E05-2021
## Soal nomor 1
Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini.

Di dalam proyek itu, Keverk diminta:

**a.** Pada saat client tersambung dengan server, terdapat dua pilihan pertama, yaitu register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. User juga dapat melakukan login. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem ini juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan id dan password pada file bernama **akun.txt** dengan format:

**akun.txt**
```
id:password
id2:password2
```

**b.** Sistem memiliki sebuah database yang bernama **files.tsv**. Isi dari **files.tsv** ini adalah **path file saat berada di server, publisher**, dan **tahun publikasi**. Setiap penambahan dan penghapusan file pada folder file yang bernama **FILES** pada server akan memengaruhi isi dari **files.tsv**. Folder **FILES** otomatis dibuat saat server dijalankan.

**c.** Tidak hanya itu, Keverk juga diminta membuat fitur agar client dapat menambah file baru ke dalam server. Direktori **FILES** memiliki struktur direktori di bawah ini:

    Direktori **FILES**
    ```
    File1.ekstensi
    File2.ekstensi
    ```

    Pertama client mengirimkan input ke server dengan struktur sebagai berikut :
    Contoh Command Client:
    ```
    add
    ```

    Output Client Console:
    ```
    Publisher:
    Tahun Publikasi:
    Filepath:
    ```

    Kemudian, dari aplikasi client akan dimasukan data buku tersebut (perlu diingat bahwa Filepath ini merupakan **path file yang akan dikirim ke server**). Lalu client nanti akan melakukan pengiriman file ke aplikasi server dengan menggunakan socket. Ketika file diterima di server, maka row dari files.tsv akan bertambah sesuai dengan data terbaru yang ditambahkan.

**d.** Dan client dapat mendownload file yang telah ada dalam folder FILES di server, sehingga sistem harus dapat mengirim file ke client. Server harus melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. Jika tidak valid, maka mengirimkan pesan error balik ke client. Jika berhasil, file akan dikirim dan akan diterima ke client di **folder client** tersebut.

Contoh Command client
```
download TEMPfile.pdf
```

**e.** Setelah itu, client juga dapat menghapus file yang tersimpan di server. Akan tetapi, Keverk takut file yang dibuang adalah file yang penting, maka file hanya akan diganti namanya menjadi ‘old-NamaFile.ekstensi’. Ketika file telah diubah namanya, maka row dari file tersebut di file.tsv akan terhapus.

Contoh Command Client:
```
delete TEMPfile.pdf
```

**f.** Client dapat melihat semua isi **files.tsv** dengan memanggil suatu perintah yang bernama see. Output dari perintah tersebut keluar dengan format.

Contoh Command Client :
```
see
```

Contoh Format Output pada Client:
```
Nama:
Publisher:
Tahun publishing:
Ekstensi File :
Filepath :
```

**g.** Aplikasi client juga dapat melakukan pencarian dengan memberikan suatu string. Hasilnya adalah semua nama file yang mengandung string tersebut. Format output seperti format output f.

Contoh Client Command:
```
find TEMP
```

**h.** Dikarenakan Keverk waspada dengan pertambahan dan penghapusan file di server, maka Keverk membuat suatu log untuk server yang bernama running.log. Contoh isi dari log ini adalah

**running.log**
```
Tambah : File1.ektensi (id:pass)
Hapus : File2.ektensi (id:pass)
```

Akan tetapi, kevek saat itu masih belum paham dengan sistem server dan client. Bantulah Keverk untuk menyelesaikan proyek ini!

Note:
- Dilarang menggunakan system() dan execv(). Silahkan dikerjakan sepenuhnya
dengan thread dan socket programming.
- Untuk download dan upload silahkan menggunakan file teks dengan ekstensi dan isi bebas (yang ada isinya bukan touch saja dan tidak kosong) dan requirement untuk benar adalah percobaan dengan minimum 5 data.

Struktur Direktori:
```
├── Client
│ ├── client.c
│ ├── File2.extensi
│ └── File1.extensi
└── Server
├── akun.txt
├── files.tsv
├── server.c
├── running.log
└── FILES
├── File2.extensi
└── File1.ekstensi
```

## Penjelasan penyelesaian soal
### 1a

### 1b

### 1c

### 1d

### 1e

### 1f

### 1g

### 1h


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

## Soal nomor 3
Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

**a.** Program menerima opsi -f seperti contoh di atas, jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna.
```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
```
**b.** Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin.  
```$ ./soal3 -d /path/to/directory/```  
Perintah di atas akan mengkategorikan file di /path/to/directory, lalu hasilnya
akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory).
Output yang dikeluarkan adalah seperti ini :
```
Ketentuan:
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“
```
**c.** Selain menerima opsi-opsi di atas, program ini menerima opsi *, contohnya ada di bawah ini:  
```$ ./soal3 \*```  
Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.

**d.** Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.

**e.** Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

Catatan: 
- Kategori folder tidak dibuat secara manual, harus melalui program C
- Program ini tidak case sensitive. Contoh: JPG dan jpg adalah sama
- Jika ekstensi lebih dari satu (contoh “.tar.gz”) maka akan masuk ke folder dengan titik terdepan (contoh “tar.gz”)
- Dilarang juga menggunakan fork-exec dan system()
- Bagian b dan c berlaku rekursif

## Penjelasan penyelesaian soal
### 3a
Pertama, kita perlu mengambil perintah dari argumen. Kemudian, untuk tiap file nya kita membuat thread. Kemudian, kita memanggil fungsi MovetoDir. Jika hasil bernilai 0 maka file berhasil dikategorikan selain itu file gagal dikategorikan. Terakhir, gabungkan semua thread dengan pthread_join.
```c
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
```  

Di fungsi MovetoDir, kita menerima parameter berupa path dari file. Kemudian, kita mengambil path dari direktori saat ini dengan ```getcwd()```. Setelah itu, kita dapatkan nama file nya dengan memanggil fungsi ```basename()```. ```FileExtension()``` digunakan untuk membuat nama folder sesuai ekstensi dari file. Karena persyaratan dari soal meminta tidak case sensitive sehingga kita membuat semua nama ekstensi menjadi huruf kecil dengan fungsi ```toLower()```. Kemudian, kita membuat direktori dengan ```createDir()```. Terakhir, kita memindahkan file ke dalam direktori yang baru saja dibuat dengan ```rename()```.
```c
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
```  

FileExtension digunakan untuk menentukan ekstensi file. Jika karakter pertama berupa "." maka tipe nya adalah hidden. Fungsi ```strchr()``` digunakan untuk mengembalikan posisi karakter ketika suatu karakter pertama kali ditemukan. Jika null, maka tipe nya unknown sedangkan jika ditemukan maka kembalikan posisi karakter + 1.
```c
char *FileExtension(char *filename) {
    if(filename[0] == '.') return "Hidden"; 
    char *temp = strchr(filename, '.');
    if(!temp) return "Unknown";
    return temp + 1;
}
```  

createDir digunakan untuk membuat folder dengan memasukkan direktori dari folder yang ingin dibuat
```c
void createDir(char *dir) {
	struct stat st = {0};

	if (stat(dir, &st) == -1) {
		mkdir(dir, 0777);
	}
}
```

Testing sebelum percobaan :

![1](https://user-images.githubusercontent.com/65168221/119237648-16e48500-bb68-11eb-929e-bcbbf04d9eac.jpg)


Testing sesudah percobaan :

![2](https://user-images.githubusercontent.com/65168221/119237650-1a780c00-bb68-11eb-8afe-0d538a7d10b8.jpg)


### 3b
Pertama, kita perlu mengambil perintah dari argumen. Jika argumen lebih dari 3 tidak bisa karena hanya boleh memasukkan 1 direktori. Jika tidak error, maka menampilkan pesan "Direktori sukses disimpan" tetapi jika error maka mencetak "Yah, gagal disimpan". Kita memanggil fungsi ```Folder``` untuk mengkategorikan direktori.
```c
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
```

Pertama, kita membuka folder dari argumen yang dimasukkan user. Kemudian kita membaca satu per satu isi dari folder selama tidak NULL. Jika tipe dari item adalah DT_REG / file maka kita bisa langsung mengkategorikan dengan fungsi MovetoDir ( langkah nya sama dengan soal 3a ) dengan membuat path dari file nya terlebih dahulu. Setiap membuat file, kita membuat thread untuk tiap file nya. Tetapi jika tipe dari item adalah DT_DIR / folder maka kita perlu memanggil fungsi Folder kembali. Kita membuka folder nya terlebih dahulu dan panggil kembali fungsi ```Folder()``` dengan parameter path dari folder yang ingin dibuka 
```c
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
```

Testing sebelum percobaan :

![3](https://user-images.githubusercontent.com/65168221/119237661-2ebc0900-bb68-11eb-9deb-5ce1736c24c5.jpg)


Testing sesudah percobaan :

![4](https://user-images.githubusercontent.com/65168221/119237668-354a8080-bb68-11eb-8c47-d66955ca0afb.jpg)


### 3c
Pertama, kita perlu mengambil perintah dari argumen. Karena ingin mengkategorikan isi dari current kategori maka kita kirim parameter berupa direktori saat ini dengan fungsi ```getcwd()```. Kemudian, kita panggil fungsi ```Folder()``` dan untuk sisa cara kerjanya sama dengan soal 3b.
```c
else if (strcmp(argv[1], "*") == 0) {
		char cwd[300];
		getcwd(cwd, sizeof(cwd));
		Folder(cwd);
		
	}
```

Testing sebelum percobaan :

![5](https://user-images.githubusercontent.com/65168221/119237681-44313300-bb68-11eb-919e-a198feec2513.jpg)


Testing sesudah percobaan :

![6](https://user-images.githubusercontent.com/65168221/119237687-485d5080-bb68-11eb-85f0-fef8dba24b18.jpg)


### 3d
Untuk persyaratan 3d, terdapat pada fungsi FileExtension. Jika pada fungsi ```strchr()``` bernilai null / tidak memiliki ekstensi maka dikembalikan string "Unknown" dan jika karakter pertama berupa "." maka itu menunjukkan bahwa ia file hidden dan dikembalikan string "Hidden".
```c
char *FileExtension(char *filename) {
    if(filename[0] == '.') return "Hidden"; 
    char *temp = strchr(filename, '.');
    if(!temp) return "Unknown";
    return temp + 1;
}
```  

### 3e
Untuk persyaratan 3e, setiap file yang akan dioperasikan harus menggunakan thread. Maka kita bisa buat dengan code di bawah ini
```c
pthread_t thread[1000];
pthread_create(&thread[++it], NULL, MovetoDir, (void *) path);
pthread_join(thread[it], NULL);
```  

Kendala :
- Kesulitan ketika soal b dan c berlaku rekursif
- Kesulitan ketika penamaan folder saat nama ekstensi file lebih dari 1
- Kesulitan saat mengoperasikan tiap thread untuk masing - masing file
