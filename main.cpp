#include <ncurses.h>
#include <unistd.h>

class Ball
{
public:
  int x;
  int y;
};

WINDOW *init_screen()
{
  WINDOW *win;                /* Okno gry */
  initscr();                  /* Włączamy tryb curses */
  win = newwin(30, 80, 1, 1); /* Tworzymy okno gry */
  refresh();                  /* Odświeżamy ekran */
  box(win, 0, 0);             /* Rysujemy ramkę */
  keypad(stdscr, TRUE);       /* Włączamy klawisze strzałek*/
  noecho();                   /* Nie wyświetlaj klawisza na ekran */
  curs_set(0);                /* Ukryj kursor */
  return win;                 /* Zwracamy okno gry */
}

void single_player(WINDOW *win)
{
  Ball ball1;
  ball1.x = 10;
  ball1.y = 10;
  mvwprintw(win, ball1.x, ball1.y, "o");
  wrefresh(win);
  getch();
}

int main()
{
  WINDOW *win = init_screen();
  single_player(win);
  return 0;
}