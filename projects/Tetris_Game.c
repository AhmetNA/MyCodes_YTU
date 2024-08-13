#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printboard(int x, int y, int matris[][y]) {                
    printf("Tahta : \n");
    for (int i = 1; i < y+1; i++) {
        printf("| %d ", i);
    }
    printf("|\n");
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("| %d ", matris[i][j]);
        }
        printf("|\n");
    }
}

void initialize_board(int x, int y, int board[x+1][y]) {
    for (int i = 0; i < x+1; i++) {
        for (int j = 0; j < y; j++) {
            board[i][j] = 0;
        }
    }
    for (int i = 0; i < y; i++) {
        board[x][i] = 1;
    }
}

void generate_block(int bloksize, int blok[4][4]) {
    for (int i = 0; i < bloksize; i++) {
        for (int j = 0; j < bloksize; j++) {
            blok[i][j] = 0;
        }
    }
    int a = 0, x = 0, y = 0;
    blok[x][y] = 1;
    while (a < bloksize - 1) {
        int grow = rand() % 4;
        if (grow == 0) x++;
        else if (grow == 1) y++;
        else if (grow == 2 && y != 0 && blok[x][y-1] == 0) y--;
        else if (grow == 3 && x != 0 && blok[x-1][y] == 0) x--;
        blok[x][y] = 1;
        a++;
    }
}

void rotate_block(int bloksize, int blok[4][4], int rotate) {
    int newblok[4][4];
    for (int k = 0; k < rotate; k++) {
        for (int i = 0; i < bloksize; i++) {
            for (int j = 0; j < bloksize; j++) {
                newblok[j][i] = blok[i][j];
            }
        }
        for (int i = 0; i < bloksize; i++) {
            for (int j = 0; j < bloksize; j++) {
                blok[i][j] = newblok[i][bloksize-1-j];
            }
        }
    }
}

void clear_full_rows(int x, int y, int board[x+1][y], int *score) {
    for (int i = 0; i < x; i++) {
        int tmp = 0;
        for (int j = 0; j < y; j++) {
            tmp += board[i][j];
        }
        if (tmp == y) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < y; j++) {
                    board[k][j] = board[k-1][j];
                }
            }
            for (int j = 0; j < y; j++) {
                board[0][j] = 0;
            }
            *score += y;
            printboard(x, y, board);
        }
    }
}

int main() {
    srand(time(NULL));
    char secim[2], nickname[20] = "Misafir Oyuncu";
    int basla = 0, highscore[] = {0,0}, quit = 0;

    do {
        while (basla == 0) {
            printf("Oyna\nNickname girin: \nBilgi\nCikis\nTercihinizi yapiniz :");
            scanf("%s", secim);
            if (secim[0] == 'C' || secim[0] == 'c') return 0;
            if (secim[0] == 'O' || secim[0] == 'o') { printf("Oyun Basliyor\n"); basla = 1; }
            if (secim[0] == 'B' || secim[0] == 'b') {
                printf("Oyunun amaci bloklari en alt satira kadar indirmek...\nMenuye donuldu\n");
            }
            if (secim[0] == 'N' || secim[0] == 'n') {
                printf("Nickname : \n");
                scanf("%s", nickname);
                printf("Nickname tercihiniz kaydedildi. Menuye donuldu\n");
            }
        }

        int bsizex, bsizey;
        printf("HOSGELDINIZ %s\nEglenceye hazir miyiz? ", nickname);
        printf("Minimum 5x5 Maximum 10x10 olacak sekilde tahtanin boyutlarini giriniz\nX: ");
        do { scanf("%d", &bsizey); } while (bsizey < 5 || bsizey > 10);
        do { printf("Y: "); scanf("%d", &bsizex); } while (bsizex < 5 || bsizex > 10);

        int board[bsizex+1][bsizey];
        initialize_board(bsizex, bsizey, board);
        printboard(bsizex, bsizey, board);

        int gameover = 0, score = 0;
        do {
            int bloksize = rand() % 4 + 1;
            int blok[4][4];
            generate_block(bloksize, blok);

            printf("BLOK : \n");
            for (int i = 0; i < bloksize; i++) {
                for (int j = 0; j < bloksize; j++) {
                    if (blok[i][j] == 1) printf("%d ", blok[i][j]);
                    else printf("  ");
                }
                printf("\n");
            }

            int ones = 0;
            for (int i = 0; i < bloksize; i++) {
                for (int j = 0; j < bloksize; j++) {
                    if (blok[i][j] == 1) ones++;
                }
            }

            int rotate = 0;
            if (ones > 1) {
                printf("Kac kere 90 derece dondurmek istiyorsunuz? ");
                scanf("%d", &rotate);
                rotate_block(bloksize, blok, rotate);
            }

            printboard(bsizex, bsizey, board);

            int by;
            do {
                printf("Bloku nereye koymak istersiniz? X kordinatini giriniz :");
                scanf("%d", &by);
                if (by <= 0 || by + bloksize > bsizey) {
                    printf("Tahtanin disina cikamazsiniz\n");
                }
            } while (by <= 0 || by + bloksize > bsizey);
            by--;

            int full = 0, dead = 0, indirrow = 1;
            while (dead == 0 && indirrow <= bsizex+1) {
                for (int i = 0; i < bloksize; i++) {
                    for (int j = 0; j < bloksize; j++) {
                        if (board[i+indirrow][j+by] != 0 && blok[i][j] == 1) {
                            dead = 1;
                        }
                    }
                }
                indirrow++;
            }
            indirrow -= 2;

            for (int i = 0; i < bloksize; i++) {
                for (int j = 0; j < bloksize; j++) {
                    if (blok[i][j] == 1) {
                        board[i+indirrow][j+by] = blok[i][j];
                    }
                }
            }

            printboard(bsizex, bsizey, board);
            clear_full_rows(bsizex, bsizey, board, &score);

            for (int i = 0; i < bsizey; i++) {
                if (board[0][i] == 1) {
                    full++;
                }
            }
            if (full != 0) {
                gameover = 1;
                printf("Oyun bitti\n");
            }
            if (gameover != 1) {
                printf("Devam etmek icin 0'a basin. Cikmak icin 1'e basin. ");
                scanf("%d", &gameover);
                highscore[1] = score;
            }
        } while (gameover < 1);

        if (highscore[0] == 0 || highscore[1] >= highscore[0]) {
            if (highscore[1] > 0) printf("Tebrikler rekoru kirdiniz\n");
            highscore[0] = highscore[1];
        }
        printf("Highscore : %d\nYour score : %d\nYeni oyuna gecmek icin 0'e basin. Cikmak icin 1'e basin.\n", highscore[0], highscore[1]);
        scanf("%d", &quit);
    } while (quit < 1);

    return 0;
}
