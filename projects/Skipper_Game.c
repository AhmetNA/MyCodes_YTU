//                                       AHMET NURI AYDEMIR 22011005
//                                     YAPISAL PROGRAMLAMA PROJE ODEVI
//                                              SKIPPITY OYUNU
//                                         https://youtu.be/700GrZIYe64
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// Skipper'in degerini ve konumunu saklamak icin struct
typedef struct {
    int x;
    int y;
    char value;
} Skipper;

void kirmizi();
void sari();
void reset();
void yesil();
void mavi();
void beyaz();

// Fonksiyon prototipleri
void print_board(char **board, int N);
void print_scoreboard(int player1_s[5], int player2_s[5]);
int create_board(char ***board, int *N);
int get_skipper(char **board, int N, Skipper *current);
char get_move(char **board, char **temp, int N, int player, int player1_s[5], int player2_s[5], Skipper *current);
int play_1vs1(char **board, char **temp, int N, int player1_s[5], int player2_s[5]);
void skore_board(char color, int player1_s[5], int player2_s[5], int player);
void remove_score(char color, int player1_s[5], int player2_s[5], int player);
char check_skipper(char **board, int N, int x1, int y1, char move);
int copy_board(char ***board, char ***temp, int N);
int menu();
void start_game();
int undo(char **board, char **temp, int N, int player, int player1_s[5], int player2_s[5], char last_eaten, Skipper *current);
int redo(char **board, char **temp, int N, int player, int player1_s[5], int player2_s[5], char last_eaten, Skipper *current);
void save_game(char **board, int N, int player1_s[5], int player2_s[5], int player, int game_mode);
void load_game(char ***board, int *N, int player1_s[5], int player2_s[5], int *player, int *game_mode);

int play_computer(char **board, char **temp, int N, int player1_s[6], int player2_s[6]);



int main() {
    start_game();
    return 0;
}



// Oyunu baslatmak için fonksiyon
void start_game() {
    int N, i, secim = menu();
    char **board, **temp;
    srand(time(NULL));
    int player1_s[6] = {0, 0, 0, 0, 0, 0}, player2_s[6] = {0, 0, 0, 0, 0, 0}, current_player = 0;
    // 1:1vs1, 2:Computer
    int game_mode = 0;
    // Skipper structi  
    Skipper current; 

    if (secim == 4) {
        load_game(&board, &N, player1_s, player2_s, &current_player, &game_mode);
        printf("Y: Yesil, S: Sari, K: Kirmizi, M: Mavi, B: Beyaz\n");
    } else {
        create_board(&board, &N);
        printf("Y: Yesil, S: Sari, K: Kirmizi, M: Mavi, B: Beyaz\n");
    }
    // allocation
    temp = (char **)malloc(N * sizeof(char *));
    for (i = 0; i < N; i++) temp[i] = (char *)malloc(N * sizeof(char));

    if (secim == 1 || (secim == 4 && game_mode == 1)) {
        play_1vs1(board, temp, N, player1_s, player2_s);
    } else if (secim == 2 || (secim == 4 && game_mode == 2)) {
        printf("Bilgisayar ile oyun\n");
        play_computer(board, temp, N, player1_s, player2_s);
    }
    //free hafiza
    for (i = 0; i < N; i++) {
        free(board[i]);
        free(temp[i]);
    }
    free(board);
    free(temp);
}

// Oyun tahtasını olusturma fonksiyonu
int create_board(char ***board, int *N) {
    int i, j, Y, S, K, M, B, random;
    char input[20];
    printf("Oyun tahtasinin boyutu (10/20)?\n");
    do {
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", N);
    } while (*N != 10 && *N != 20);

    *board = (char **)malloc(*N * sizeof(char *));
    for (i = 0; i < *N; i++) (*board)[i] = (char *)malloc(*N * sizeof(char));

    for (i = 0; i < *N; i++) for (j = 0; j < *N; j++) (*board)[i][j] = '.';

    int renk_sayisi = 5;
    if (*N == 10) Y = S = K = M = B = 20;
    else Y = S = K = M = B = 80;

    //rastgele renk secip tahtaya yerlestirme ve kullanilani eksiltme
    for (i = 0; i < *N; i++) {
        for (j = 0; j < *N; j++) {
            random = rand() % renk_sayisi;
            if (random == 0 && Y > 0) (*board)[i][j] = 'Y',         Y--;
            else if (random == 1 && S > 0) (*board)[i][j] = 'S',    S--;
            else if (random == 2 && K > 0) (*board)[i][j] = 'K',    K--;
            else if (random == 3 && M > 0) (*board)[i][j] = 'M',    M--;
            else if (random == 4 && B > 0) (*board)[i][j] = 'B',    B--;
            else j--;
        }
    }
    (*board)[*N / 2][*N / 2] = (*board)[*N / 2][*N / 2 - 1] = (*board)[*N / 2 - 1][*N / 2] = (*board)[*N / 2 - 1][*N / 2 - 1] = '.';
    print_board(*board, *N);
    return 0;
}

// Menu fonksiyonu
int menu() {
    int secim;
    kirmizi(); printf("\t*****************HOSGELDINIZZZZZZZ*****************\n");
    sari(); printf("\t\t****************SKIPPITY****************\n\n");
    yesil(); printf("Oyunun amaci en fazla puani toplamaktir\nOyunda skipperlarinizla diğer skipperlari yiyerek puan toplayabilirsiniz\n");
    kirmizi(); printf("HADI BASLAYALIM\n");
    reset(); printf("1. Arkadasinla Oyna\n2. Bilgisayara Karsi Oyna\n3. Cikis\n4. Kaydedilmis Oyunu Yukle\nSeciminizi yapin: ");
    scanf("%d", &secim);
    return secim;
}

// Tahtayi yazdirma fonksiyonu
void print_board(char **board, int N) {
    int i, j;
    printf("   ");
    for (i = 0; i < N; i++) printf("%d%s", i + 1, i < 9 ? "  " : " "); 
    printf("\n%s\n", N == 10 ? "-------------------------------------" : "--------------------------------------------------------------------------");
    for (i = 0; i < N; i++) {
        printf("%d%s", i + 1, i < 9 ? " |" : "|"); 
        for (j = 0; j < N; j++) {
            switch (board[i][j]) {
                case 'Y': yesil();   break;
                case 'S': sari();    break;
                case 'K': kirmizi(); break;
                case 'M': mavi();    break;
                case 'B': beyaz();   break;
                default: reset();    break;
            }
            printf("%c  ", board[i][j]);
            reset();
        }
        printf("\n");
    }
}

// Skor tahtasini yazdirma fonksiyonu
void print_scoreboard(int player1_s[5], int player2_s[5]) {
    printf("Oyuncu 1\n");
    yesil(); printf("Y: %d ", player1_s[0]);
    sari(); printf("S: %d ", player1_s[1]);
    kirmizi(); printf("K: %d ", player1_s[2]);
    mavi(); printf("M: %d ", player1_s[3]);
    beyaz(); printf("B: %d ", player1_s[4]);
    reset(); printf("Skor: %d\nOyuncu 2\n", player1_s[5]);
    yesil(); printf("Y: %d ", player2_s[0]);
    sari(); printf("S: %d ", player2_s[1]);
    kirmizi(); printf("K: %d ", player2_s[2]);
    mavi(); printf("M: %d ", player2_s[3]);
    beyaz(); printf("B: %d ", player2_s[4]);
    reset(); printf("Skor: %d\n", player2_s[5]);
    if(player1_s[5] > player2_s[5]) {printf("Oyuncu 1 KAZANIYOR\n");}
    else if(player1_s[5] < player2_s[5]) {printf("Oyuncu 2 KAZANIYOR\n");}
    else {printf("Berabere\n");}
}

// Kullanicidan skipper'in konumunu almak icin fonksiyon
int get_skipper(char **board, int N, Skipper *current) {
    int x1, y1, i, can_move = 0;
    char moves[4] = {'w', 'a', 's', 'd'};
    printf("Tasimak istediginiz skipper'in koordinatlarini girin\n");
    scanf("%d %d", &x1, &y1);
    // 1 10 arasinda almaliyiz ama index 0 dan basladigi icin 1 cikartiyoruz
    x1--; y1--;
    for (i = 0; i < 4; i++) if (check_skipper(board, N, x1, y1, moves[i]) != 'n') can_move = 1;
    if (can_move == 0 || x1 < 0 || x1 >= N || y1 < 0 || y1 >= N || board[x1][y1] == '.') {
        printf("Gecersiz nokta, baska bir nokta secin\n");
        return get_skipper(board, N, current);
    } else {
        current->x = x1; current->y = y1; current->value = board[x1][y1];
        print_board(board, N);
        printf("Sectiginiz skipper: %c\n", board[x1][y1]);
        return 0;
    }
}

// Kullanicidan hareket almak icin fonksiyon
char get_move(char **board, char **temp, int N, int player, int player1_s[5], int player2_s[5], Skipper *current) {
    int i, can_move = 0, x1 = current->x, y1 = current->y;
    char move, eaten, moves[4] = {'w', 'a', 's', 'd'};
    for (i = 0; i < 4; i++) if (check_skipper(board, N, x1, y1, moves[i]) != 'n') can_move = 1;
    if (can_move == 0) {
        printf("Siraniz bitti\nOyuncu %d'nin sirasi\n", player + 1);
        return 'n';
    }
    printf("Skipper'i tasimak icin w, a, s, d tuslarina basin\n");
    getchar();
    move = getchar();
    // eger kullanici dogru hamle girmezse tekrar al
    if (move != 'w' && move != 'a' && move != 's' && move != 'd') {
        printf("Gecersiz hareket 1\n");
        return get_move(board, temp, N, player, player1_s, player2_s, current);
    } else if (check_skipper(board, N, x1, y1, move) == 'n') {
        printf("Gecersiz hareket 2\n");
        return get_move(board, temp, N, player, player1_s, player2_s, current);
    }
    // aldigimiz tum hamleleri degerlendiriyoruz hamle yapmadan once tahtanin kopyasini aliyoruz 
    // ve hamleyi yapip tahtayi guncelliyoruz tas yersek skoru arttiriyoruz ve '.' ile yer degistiriyoruz
    if (check_skipper(board, N, x1, y1, move) == 'w') {
        eaten = board[x1 - 1][y1];
        skore_board(eaten, player1_s, player2_s, player);
        copy_board(&board, &temp, N);
        board[x1][y1] = board[x1 - 1][y1] = '.';
        board[x1 - 2][y1] = current->value;
        current->x = x1 - 2;
    } else if (check_skipper(board, N, x1, y1, move) == 'a') {
        eaten = board[x1][y1 - 1];
        skore_board(eaten, player1_s, player2_s, player);
        copy_board(&board, &temp, N);
        board[x1][y1] = board[x1][y1 - 1] = '.';
        board[x1][y1 - 2] = current->value;
        current->y = y1 - 2;
    } else if (check_skipper(board, N, x1, y1, move) == 's') {
        eaten = board[x1 + 1][y1];
        skore_board(eaten, player1_s, player2_s, player);
        copy_board(&board, &temp, N);
        board[x1][y1] = board[x1 + 1][y1] = '.';
        board[x1 + 2][y1] = current->value;
        current->x = x1 + 2;
    } else if (check_skipper(board, N, x1, y1, move) == 'd') {
        eaten = board[x1][y1 + 1];
        skore_board(eaten, player1_s, player2_s, player);
        copy_board(&board, &temp, N);
        board[x1][y1] = board[x1][y1 + 1] = '.';
        board[x1][y1 + 2] = current->value;
        current->y = y1 + 2;
    } else {
        printf("Gecersiz hareket\n");
        return get_move(board, temp, N, player, player1_s, player2_s, current);
    }
    print_board(board, N);
    // hamleyi oynadiktan sonra undo ve redo istiyor mu cagiriyoruz
    undo(board, temp, N, player, player1_s, player2_s, eaten, current);
}

// Geri alma islemi icin fonksiyon
int undo(char **board, char **old_board, int N, int player, int player1_s[6], int player2_s[6], char last_eaten, Skipper *current) {
    int i, j;
    char secim, temp;
    printf("Hamlenizi geri almak istiyor musunuz?\nEvet: y\nHayir: n\n");
    getchar();
    scanf("%c", &secim);
    if (secim == 'n') return 0;
    // eger evet derse tahtayi eski haline getiriyoruz
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++) {
        temp = board[i][j], board[i][j] = old_board[i][j]; 
        old_board[i][j] = temp;
        }
    }
    remove_score(last_eaten, player1_s, player2_s, player);
    print_board(board, N);
    if (redo(board, old_board, N, player, player1_s, player2_s, last_eaten, current) != 1) {
        get_skipper(board, N, current);
        get_move(board, old_board, N, player, player1_s, player2_s, current);
    }
    return 0;
}

// Yeniden yapma islemi icin fonksiyon
int redo(char **board, char **temp, int N, int player, int player1_s[6], int player2_s[6], char last_eaten, Skipper *current) {
    int i, j;
    char secim;
    printf("Hamlenizi yeniden yapmak istiyor musunuz?\nEvet: y\nHayir: n\n");
    getchar();
    scanf("%c", &secim);
    if (secim == 'y') {
        for (i = 0; i < N; i++){ 
            for (j = 0; j < N; j++) {
                board[i][j] = temp[i][j];
            }
        }
        print_board(board, N);
        skore_board(last_eaten, player1_s, player2_s, player);
        return 1;
    } else return 0;
}

// Skipper icin gecerli hareket olasiliklarini kontrol etme fonksiyonu
char check_skipper(char **board, int N, int x1, int y1, char move) {
    if (move == 'w') {
        if (x1 > 1 && board[x1 - 1][y1] != '.' && board[x1 - 2][y1] == '.') {return 'w';} 
        else {return 'n';}
    }
    else if (move == 'a') {
        if (y1 > 1 && board[x1][y1 - 1] != '.' && board[x1][y1 - 2] == '.') {return 'a';} 
        else {return 'n';}
    }
    else if (move == 's') {
        if (x1 < N - 2 && board[x1 + 1][y1] != '.' && board[x1 + 2][y1] == '.') {return 's';} 
        else {return 'n';}
    }
    else if (move == 'd') {
        if (y1 < N - 2 && board[x1][y1 + 1] != '.' && board[x1][y1 + 2] == '.') {return 'd';} 
        else {return 'n';}
    }
    return 'n';
}

// 1vs1 oyun modu icin fonksiyon
int play_1vs1(char **board, char **temp, int N, int player1_s[6], int player2_s[6]) {
    int quit = 0, player = 1, cant_play = 0;
    char color, canplay;
    Skipper current; // Mevcut skipper için yerel değişken
    while (!quit) {
        system("cls");
        print_board(board, N);
        get_skipper(board, N, &current);
        cant_play = 0;
        while (!cant_play) {
            canplay = get_move(board, temp, N, player, player1_s, player2_s, &current);
            if (canplay == 'n') cant_play = 1;
            else skore_board(canplay, player1_s, player2_s, player);
        }
        print_scoreboard(player1_s, player2_s);
        player = (player + 1) % 2;
        printf("Oyunu bitirmek istiyor musunuz?\nEvet: y\nHayir: n\n");
        getchar();
        if (getchar() == 'y') {
            quit = 1;
            printf("Oyun Bitti\n");
            if (player1_s[5] > player2_s[5]) printf("Oyuncu 1 kazandi\n");
            else if (player1_s[5] < player2_s[5]) printf("Oyuncu 2 kazandi\n");
            else printf("Berabere\n");
            save_game(board, N, player1_s, player2_s, player, 1);
        }
        print_scoreboard(player1_s, player2_s);
    }
    return 0;
}

// Bilgisayara karsi oynamak icin fonksiyon
int play_computer(char **board, char **temp, int N, int player1_s[6], int player2_s[6]) {
    int quit = 0, player = 1, cant_play = 0;
    char color, canplay;
    Skipper current; 
    int x, y, m; 
    while (!quit) {
        system("cls");
        print_board(board, N);
        get_skipper(board, N, &current);
        cant_play = 0;
        while (!cant_play) {
            canplay = get_move(board, temp, N, player, player1_s, player2_s, &current);
            if (canplay == 'n') cant_play = 1;
            else skore_board(canplay, player1_s, player2_s, player);
        }
        print_scoreboard(player1_s, player2_s);
        player = (player + 1) % 2;

        // Bilgisayarın sırası
        if (player == 0) {
            //en iyi hamleyi bulcaz
            int best_x = -1, best_y = -1, best_score = -1;
            char best_move = 'n';
            char moves[4] = {'w', 'a', 's', 'd'};
            // oyun tahtasi tum hamleleri dener her tas icin 4 hamle yapar
            for (x = 0; x < N; x++) {
                for (y = 0; y < N; y++) {
                    if (board[x][y] != '.') {
                        for (m = 0; m < 4; m++) {
                            int score = 0;
                            if (check_skipper(board, N, x, y, moves[m]) != 'n') {
                                int temp_x = x, temp_y = y;
                                while (check_skipper(board, N, temp_x, temp_y, moves[m]) != 'n') {
                                    if (moves[m] == 'w') temp_x -= 2;
                                    else if (moves[m] == 'a') temp_y -= 2;
                                    else if (moves[m] == 's') temp_x += 2;
                                    else if (moves[m] == 'd') temp_y += 2;
                                    score++;
                                }
                                // en iyi skor yapan hamle icin yerleri tutar
                                if (score > best_score) {
                                    best_score = score;
                                    best_x = x;
                                    best_y = y;
                                    best_move = moves[m];
                                }
                            }
                        }
                    }
                }
            }

            if (best_x != -1 && best_y != -1) {
                current.x = best_x;
                current.y = best_y;
                current.value = board[best_x][best_y];
                char eaten;
                printf("Bilgisayar (%d, %d) konumundan ", best_x + 1, best_y + 1); 
                if (best_move == 'w') {
                    while (check_skipper(board, N, current.x, current.y, best_move) != 'n') {
                        eaten = board[current.x - 1][current.y];
                        skore_board(eaten, player1_s, player2_s, player);
                        board[current.x][current.y] = board[current.x - 1][current.y] = '.';
                        board[current.x - 2][current.y] = current.value;
                        current.x -= 2;
                    }
                } else if (best_move == 'a') {
                    while (check_skipper(board, N, current.x, current.y, best_move) != 'n') {
                        eaten = board[current.x][current.y - 1];
                        skore_board(eaten, player1_s, player2_s, player);
                        board[current.x][current.y] = board[current.x][current.y - 1] = '.';
                        board[current.x][current.y - 2] = current.value;
                        current.y -= 2;
                    }
                } else if (best_move == 's') {
                    while (check_skipper(board, N, current.x, current.y, best_move) != 'n') {
                        eaten = board[current.x + 1][current.y];
                        skore_board(eaten, player1_s, player2_s, player);
                        board[current.x][current.y] = board[current.x + 1][current.y] = '.';
                        board[current.x + 2][current.y] = current.value;
                        current.x += 2;
                    }
                } else if (best_move == 'd') {
                    while (check_skipper(board, N, current.x, current.y, best_move) != 'n') {
                        eaten = board[current.x][current.y + 1];
                        skore_board(eaten, player1_s, player2_s, player);
                        board[current.x][current.y] = board[current.x][current.y + 1] = '.';
                        board[current.x][current.y + 2] = current.value;
                        current.y += 2;
                    }
                }
                printf("(%d, %d) konumuna tasiyor\n", current.x + 1, current.y + 1); // Bitiş koordinatlarını yazdır
                print_board(board, N);
            }

            print_scoreboard(player1_s, player2_s);
            printf("Oyunu bitirmek istiyor musunuz?\nEvet: y\nHayir: n\n");
            getchar();
            if (getchar() == 'y') {
                quit = 1;
                printf("Oyun Bitti\n");
                if (player1_s[5] > player2_s[5]) printf("Oyuncu 1 kazandi\n");
                else if (player1_s[5] < player2_s[5]) printf("Oyuncu 2 kazandi\n");
                else printf("Berabere\n");
                save_game(board, N, player1_s, player2_s, player, 2);
            }
            player = (player + 1) % 2;
        }

        printf("Oyunu bitirmek istiyor musunuz?\nEvet: y\nHayir: n\n");
        getchar();
        if (getchar() == 'y') {
            quit = 1;
            printf("Oyun Bitti\n");
            if (player1_s[5] > player2_s[5]) printf("Oyuncu 1 kazandi\n");
            else if (player1_s[5] < player2_s[5]) printf("Oyuncu 2 kazandi\n");
            else printf("Berabere\n");
            save_game(board, N, player1_s, player2_s, player, 2);
        }
        print_scoreboard(player1_s, player2_s);
    }
    return 0;
}

// Skor tahtasini guncelleme fonksiyonu
void skore_board(char color, int player1_s[6], int player2_s[6], int player) {
    int i, min = 5;
    if (player == 1) {
        if (color == 'n') return;
        if (color == 'B') player1_s[4]++;
        else if (color == 'K') player1_s[2]++;
        else if (color == 'M') player1_s[3]++;
        else if (color == 'S') player1_s[1]++;
        else if (color == 'Y') player1_s[0]++;
        for (i = 0; i < 5; i++) if (player1_s[i] < min) min = player1_s[i];
        player1_s[5] = min;
    } else if (player == 0) {
        if (color == 'B') player2_s[4]++;
        else if (color == 'K') player2_s[2]++;
        else if (color == 'M') player2_s[3]++;
        else if (color == 'S') player2_s[1]++;
        else if (color == 'Y') player2_s[0]++;
        for (i = 0; i < 5; i++) if (player2_s[i] < min) min = player2_s[i];
        player2_s[5] = min;
    }
}

// Skor tahtasindan puan silme fonksiyonu
void remove_score(char color, int player1_s[6], int player2_s[6], int player) {
    int i, min = 5;
    if (player == 1) {
        if (color == 'n') return;
        if (color == 'B') player1_s[4]--;
        else if (color == 'K') player1_s[2]--;
        else if (color == 'M') player1_s[3]--;
        else if (color == 'S') player1_s[1]--;
        else if (color == 'Y') player1_s[0]--;
        for (i = 0; i < 5; i++) if (player1_s[i] < min) min = player1_s[i];
        player1_s[5] = min;
    } else if (player == 0) {
        if (color == 'B') player2_s[4]--;
        else if (color == 'K') player2_s[2]--;
        else if (color == 'M') player2_s[3]--;
        else if (color == 'S') player2_s[1]--;
        else if (color == 'Y') player2_s[0]--;
        for (i = 0; i < 5; i++) if (player2_s[i] < min) min = player2_s[i];
        player2_s[5] = min;
    }
}

// Mevcut tahta durumunu gecici tahtaya kopyalama fonksiyonu
int copy_board(char ***board, char ***temp, int N) {
    int i, j;
    for (i = 0; i < N; i++) for (j = 0; j < N; j++) (*temp)[i][j] = (*board)[i][j];
    return 0;
}

// Oyun durumunu bir dosyaya kaydetme fonksiyonu
void save_game(char **board, int N, int player1_s[5], int player2_s[5], int player, int game_mode) {
    int i, j;
    char secim;
    printf("Oyunu kaydetmek istiyor musunuz?\nEvet: y\nHayir: n\n");
    getchar();
    scanf("%c", &secim);
    if (secim == 'y') {
        FILE *file = fopen("saveskippity.txt", "w");
        if (file == NULL) {
            printf("Hata\nOyun kaydedilmedi\n");
            return;
        }
        fprintf(file, "%d\n", N);
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) fprintf(file, "%c", board[i][j]);
            fprintf(file, "\n");
        }
        for (i = 0; i < 5; i++) fprintf(file, "%d ", player1_s[i]);
        fprintf(file, "%d\n", player1_s[5]);
        for (i = 0; i < 5; i++) fprintf(file, "%d ", player2_s[i]);
        fprintf(file, "%d\n", player2_s[5]);
        fprintf(file, "%d\n", player);
        fprintf(file, "%d\n", game_mode); // Oyun modunu kaydet
        fclose(file);
        printf("Oyun kaydedildi\n");
    }
}

// Oyun durumunu bir dosyadan yukleme fonksiyonu
void load_game(char ***board, int *N, int player1_s[5], int player2_s[5], int *player, int *game_mode) {
    int i, j;
    char secim;
    printf("Oyunu yuklemek istiyor musunuz?\nEvet: y\nHayir: n\n");
    getchar();
    scanf("%c", &secim);
    if (secim == 'n') return;
    FILE *file = fopen("saveskippity.txt", "r");
    if (file == NULL) {
        printf("Hata\nOyun yuklenmedi\n");
        return;
    }
    fscanf(file, "%d\n", N);
    *board = (char **)malloc(*N * sizeof(char *));
    for (i = 0; i < *N; i++) (*board)[i] = (char *)malloc(*N * sizeof(char));
    for (i = 0; i < *N; i++) for (j = 0; j < *N; j++) fscanf(file, " %c", &(*board)[i][j]);
    for (i = 0; i < 6; i++) fscanf(file, "%d", &player1_s[i]);
    for (i = 0; i < 6; i++) fscanf(file, "%d", &player2_s[i]);
    fscanf(file, "%d", player);
    fscanf(file, "%d", game_mode);
    fclose(file);
    printf("Oyun yüklendi\n");
    print_board(*board, *N);
}

// Renk fonksiyonları
void kirmizi() { printf("\033[1;31m"); }
void sari() { printf("\033[1;33m"); }
void yesil() { printf("\033[1;32m"); }
void mavi() { printf("\033[1;34m"); }
void beyaz() { printf("\033[1;37m"); }
void reset() { printf("\033[0m"); }
