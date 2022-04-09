/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief Główny plik programu
 * @version 0.1
 * @date 2022-04-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <ncurses.h>
#include <unistd.h>

/**
 * @brief Klasa reprezentująca piłkę, którą gracz odbija w trakcie gry
 *
 */
class Ball
{
public:
  int x;       /**< Współrzędna x piłki */
  int y;       /**< Współrzędna y piłki */
  int x_speed; /**< Prędkość piłki w osi X */
  int y_speed; /**< Prędkość piłki w osi Y */
};

/**
 * @brief Klasa reprezentująca paletkę, którą sterujemy w trakcie gry
 *
 */
class Paddle
{
public:
  int x;     /**< Współrzędna x paletki */
  int y;     /**< Współrzędna y paletki */
  int width; /**< Szerokość paletki */
};

/**
 * @brief Funkcja inicjalizująca ncurses
 *
 * @return WINDOW* Wskaźnik na okno główne
 */
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

/**
 * @brief Funkcja odpowiedzialna za odbijanie piłki od paletki i ścian
 *
 * @param win wskaźnik na okno gry
 * @param ball referencja do obiektu piłki
 * @param paddle referencja do obiektu paletki
 * @param score referencja do obiektu wyniku
 */
void ball_bounce(WINDOW *win, Ball &ball, Paddle &paddle, int &score)
{
  if (ball.y == getmaxy(win) - 3)
  {
    for (int i = paddle.x; i < paddle.x + paddle.width; i++)
    {
      if (ball.x == i)
      {
        ball.y_speed = -1;
        score++;
      }
    }
  }
  if (ball.y == getbegx(win))
  {
    ball.y_speed = 1;
  }
  if (ball.x == getmaxx(win) - 2)
  {
    ball.x_speed = -1;
  }
  if (ball.x == getbegx(win))
  {
    ball.x_speed = 1;
  }
}

/**
 * @brief Funkcja odpowiedzialna za gre dla jednego gracza
 *
 * @param win - okno, w którym odbywa się gra
 *
 * @return int - zwraca 1 gdy użytkownik przegrał
 */
int single_player(WINDOW *win)
{
  // Deklaracja piłki oraz jej początkowej pozycji i prędkości
  Ball ball1;
  ball1.x = 10;
  ball1.y = 10;
  ball1.x_speed = 1;
  ball1.y_speed = 1;
  mvwprintw(win, ball1.y, ball1.x, "o");
  // Deklaracja paletki, jej pozycji w połowie ekranu na osi X oraz na przedostatniej linii osi Y
  Paddle paddle1;
  paddle1.x = getmaxx(win) / 2;
  paddle1.y = getmaxy(win) - 2;
  paddle1.width = 5;

  /* Counter pozwala na przesuwanie piłki z opóźnieniem */
  int counter = 0;
  int score = 0;
  // Funkcja w której znajduje się cała logika gry
  while (true)
  {
    counter++;
    mvwprintw(win, 0, 0, "Score: %d", score / 125);
    // Co 125 klatek przesuwamy piłkę, usuwając stary ślad
    if (counter % 125 == 0)
    {
      mvwprintw(win, ball1.y, ball1.x, " ");
      ball1.x += ball1.x_speed;
      ball1.y += ball1.y_speed;
      mvwprintw(win, ball1.y, ball1.x, "o");
    }

    /* Odbijanie piłki */
    ball_bounce(win, ball1, paddle1, score);
    if (ball1.y == getmaxy(win) - 1)
    {
      wclear(win);
      box(win, 0, 0);
      mvwprintw(win, getmaxy(win) / 2, getmaxx(win) / 2, "GAME OVER");
      mvwprintw(win, getmaxy(win) / 2 + 1, getmaxx(win) / 2, "Your score: %d", score / 125);
      mvwprintw(win, getmaxy(win) / 2 + 2, getmaxx(win) / 2, "Press any key to continue");
      mvwprintw(win, getmaxy(win) / 2 + 3, getmaxx(win) / 2, "Press q to quit");
      wrefresh(win);
      nodelay(stdscr, FALSE);
      int ch;
      ch = getch();

      if (ch == 'q' || ch == 'Q')
        return 1;
      else
      {
        wclear(win);
        box(win, 0, 0);
        nodelay(stdscr, TRUE);
        return single_player(win);
      }
    }
    // Funkcja odpowiedzialna za przesuwanie paletki
    int quit = move_paddle(win, paddle1);
    if (quit == 1)
    {
      break;
    }
    // Usypiamy program na 500 mikrosekund i odświeżamy okno z nowymi danymi
    usleep(500);
    wrefresh(win);
  }
  return 0;
}

/**
 * @brief Główna pętla programu
 *
 * @return int - 0 gdy program się zakończy poprawnie
 */
int main()
{

  WINDOW *win = init_screen();
  single_player(win);
  endwin();
  return 0;
}