#include <ncurses.h>
#include <unistd.h>

class Ball
{
public:
  int x;
  int y;
};

class Paddle
{
public:
  int x, y, width;
};

WINDOW *
init_screen()
{
  WINDOW *win; /* Okno gry */

  initscr();                  /* Włączamy tryb curses */
  win = newwin(30, 80, 1, 1); /* Tworzymy okno gry */
  refresh();                  /* Odświeżamy ekran */
  box(win, 0, 0);             /* Rysujemy ramkę */
  keypad(stdscr, TRUE);       /* Włączamy klawisze strzałek*/
  noecho();                   /* Nie wyświetlaj klawisza na ekran */
  curs_set(0);                /* Ukryj kursor */
  nodelay(stdscr, TRUE);      /* Nie czekaj na wprowadzenie znaku */
  return win;                 /* Zwracamy okno gry */
}

/**
 * @brief Funkcja odpowiedzialna za sterowanie paletka
 *
 * @param win - okno gry w którym będzie się poruszać paletka
 * @param paddle - paletka, którą sterujemy
 * @return int - zwraca 1 gdy użytkownik chce opuścić grę
 */
int move_paddle(WINDOW *win, Paddle &paddle)
{
  // pętla odpowiedzialna za tworzenie dodatkowych pól w kształcie paletki
  // np. jeśli nasza paletka ma szerokość 5 to musimy stworzyć 5 pól
  for (int i = 0; i < paddle.width; i++)
  {
    mvwprintw(win, paddle.y, paddle.x + i, "-");
  }
  // Pobieramy klawisz sterowania od użytkownika,
  // w przypadku klawisza 'q' opuszczamy grę zwracając 1
  int ch;
  ch = getch();
  switch (ch)
  {
  case 'q':
    return 1;
  // Przesuwamy paletkę o jeden w lewo po naciśnięciu klawisza strzałki w lewo `<-`
  case KEY_LEFT:
    if (paddle.x > getbegx(win))
    {
      paddle.x--;
      // Pętla odpowiedzialna za tworzenie dodatkowych pól w kształcie paletki
      // np. jeśli nasza paletka ma szerokość 5 to musimy stworzyć 5 pól
      // Dodatkowo usuwamy pola paletki z poprzedniego miejsca
      for (int i = 0; i < paddle.width; i++)
      {
        mvwprintw(win, paddle.y, paddle.x + i, "-");
        mvwprintw(win, paddle.y, paddle.x + i + 1, " ");
      }
    }
    break;
  // Przesuwamy paletkę o jeden w prawo po naciśnięciu klawisza strzałki w prawo `->`
  case KEY_RIGHT:
    if (paddle.x < getmaxx(win) - 1 - paddle.width)
    {
      paddle.x += 1;
      // Pętla odpowiedzialna za tworzenie dodatkowych pól w kształcie paletki
      // np. jeśli nasza paletka ma szerokość 5 to musimy stworzyć 5 pól
      // Dodatkowo usuwamy pola paletki z poprzedniego miejsca
      for (int i = 0; i < paddle.width; i++)
      {
        mvwprintw(win, paddle.y, paddle.x + i, "-");
        // W tym warunku sprawdzamy czy nasza paletka nie znajduje się przy krawedzi ekranu
        // Jeśli tak to nie usuwamy znaku sprzed paletki
        // W przeciwnym wypadku zmazywalibyśmy znak krawedzi ekranu
        if (paddle.x - i != getbegx(win) - 1)
          mvwprintw(win, paddle.y, paddle.x - i, " ");
      }
    }

    break;
  }
  return 0;
}

void single_player(WINDOW *win)
{
  Ball ball1;
  ball1.x = 10;
  ball1.y = 10;
  mvwprintw(win, ball1.x, ball1.y, "o");
  Paddle paddle1;
  paddle1.x = getmaxx(win) / 2;
  paddle1.y = getmaxy(win) - 2;
  paddle1.width = 5;
  while (true)
  {
    int quit = move_paddle(win, paddle1);
    if (quit == 1)
    {
      break;
    }
    usleep(500);
    wrefresh(win);
  }
}

int main()
{

  WINDOW *win = init_screen();
  single_player(win);
  endwin();
  return 0;
}