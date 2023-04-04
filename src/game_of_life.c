#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define M 80
#define N 25

void input_0graphic(char matrix[][M], int n, int m);
void output_graphic(char newmatrix[][M], int n, int m);
int finish(char newmatrix[][M], int n, int m);
void life_or_dead(char matrix[][M], char newmatrix[][M], int n, int m);
void start_menu();
int proverka(char matrix[][M], int n, int m);
void SPandEXIT(char komand, int *sleep, int *res);
int uslprov(int *Y_obhod, int *X_obhod);

int main() {
  int sleep = 300000;
  char matrix[N][M];
  char newmatrix[N][M];
  int n = N;
  int m = M;
  int res = 1;
  input_0graphic(newmatrix, n, m);
  if (!freopen("/dev/tty", "r", stdin)) {
    perror("/dev/tty");
    exit(1);
  }
  initscr();
  noecho();
  timeout(0);
  output_graphic(newmatrix, n, m);
  while (res != 0) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        matrix[i][j] = newmatrix[i][j];
      }
    }
    life_or_dead(matrix, newmatrix, n, m);
    output_graphic(newmatrix, n, m);
    res = finish(newmatrix, n, m);
    char komand = getch();
    SPandEXIT(komand, &sleep, &res);
    usleep(sleep);
  }
  endwin();
  return 0;
}

void SPandEXIT(char komand, int *sleep, int *res) {
  if ((komand == 'z' || komand == 'Z') && *sleep < 600000)
    *sleep += 25000;
  if ((komand == 'x' || komand == 'X') && *sleep > 25000)
    *sleep -= 25000;
  if (komand == 'f' || komand == 'F')
    *res = 0;
}

void input_0graphic(char matrix[][M], int n, int m) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      char term;
      scanf("%c%c", &matrix[i][j], &term);
    }
  }
}

void output_graphic(char newmatrix[][M], int n, int m) {
  move(0, 0);
  start_menu();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (j == 0)
        printw("|%c", newmatrix[i][j]);
      else
        printw(" %c", newmatrix[i][j]);
    }
    printw("|\n");
  }
  refresh();
}

void life_or_dead(char matrix[][M], char newmatrix[][M], int n, int m) {
  int str, stolb, ld = 0;
  for (str = 0; str < n; str++) {
    for (stolb = 0; stolb < m; stolb++) {
      int checkLife = 0;
      int checkDead = 0;
      if (matrix[str][stolb] == '#') {
        ld = 1;
        checkLife = proverka(matrix, str, stolb);
      } else {
        if (matrix[str][stolb] == '-') {
          ld = 0;
          checkDead = proverka(matrix, str, stolb);
        }
      }
      if (ld == 0) {
        if (checkDead > 2 && checkDead < 4)
          newmatrix[str][stolb] = '#';
        else
          newmatrix[str][stolb] = matrix[str][stolb];
      } else if (ld == 1) {
        if (checkLife == 2 || checkLife == 3)
          newmatrix[str][stolb] = matrix[str][stolb];
        else
          newmatrix[str][stolb] = '-';
      }
    }
  }
}

int proverka(char matrix[][M], int n, int m) {
  int kol = 0;
  for (int Y_obhod = -1; Y_obhod < 2; Y_obhod++) {
    for (int X_obhod = -1; X_obhod < 2; X_obhod++) {
      if ((Y_obhod != 0) || (X_obhod != 0)) {
        if ((n == 0) && (Y_obhod == -1)) {
          if (matrix[N - 1][m + X_obhod] == '#') {
            kol++;
          }
        } else if ((n == N - 1) && (Y_obhod == 1)) {
          if (matrix[0][m + X_obhod] == '#') {
            kol++;
          }
        } else if ((m == 0) && (X_obhod == -1)) {
          if (matrix[n + Y_obhod][M - 1] == '#') {
            kol++;
          }
        } else if ((m == M - 1) && (X_obhod == 1)) {
          if (matrix[n + Y_obhod][0] == '#') {
            kol++;
          }
        } else if ((n == 0) && (m == 0) && (X_obhod == -1) && (Y_obhod == -1) &&
                   (matrix[N - 1][M - 1] == '#')) {
          kol++;
        } else if ((n == 0) && (m == M - 1) && (X_obhod == 1) &&
                   (Y_obhod == -1) && (matrix[N - 1][0] == '#')) {
          kol++;
        } else if ((n == N - 1) && (m == 0) && (X_obhod == -1) &&
                   (Y_obhod == 1) && (matrix[0][M - 1] == '#')) {
          kol++;
        } else if ((n == N - 1) && (m == M - 1) && (X_obhod == 1) &&
                   (Y_obhod == 1) && (matrix[0][0] == '#')) {
          kol++;
        } else if (matrix[n + Y_obhod][m + X_obhod] == '#') {
          kol++;
        }
      }
    }
  }
  return kol;
}

int finish(char newmatrix[][M], int n, int m) {
  int i, j;
  int res = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      if (newmatrix[i][j] == '#')
        res++;
    }
  }
  return res;
}

void start_menu() {
  printw("+--------------------------------------------------------------------"
         "---------------------------------------------------------------------"
         "----------------------+\n");
  printw("|                                                                    "
         "                                                                     "
         "                      |\n");
  printw("|                                                                    "
         "     Game of Life                                                    "
         "                      |\n");
  printw("|                                                                    "
         "                                                                     "
         "                      |\n");
  printw("+--------------------------------------------------------------------"
         "---------------------------------------------------------------------"
         "----------------------+\n");
  printw("|                                                       You can "
         "change the speed on the buttons 'z' and 'x'                          "
         "                           |\n");
  printw("+--------------------------------------------------------------------"
         "---------------------------------------------------------------------"
         "----------------------+\n");
  printw("|                                                                   "
         "Press f to end the game                                              "
         "                       |\n");
  printw("+--------------------------------------------------------------------"
         "---------------------------------------------------------------------"
         "----------------------+\n");
}
