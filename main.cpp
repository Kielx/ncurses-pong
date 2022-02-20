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

WINDOW *init_screen()
{
  WINDOW *win;                /* Game window pointer */
  initscr();                  /* Turn on curses mode */
  win = newwin(30, 80, 1, 1); /* Create new game window */
  nodelay(stdscr, TRUE);      /* Don't wait for keypress */
  refresh();                  /* Refreshing screen */
  box(win, 0, 0);             /* Draw a box around the window */
  keypad(stdscr, TRUE);       /* Turn on arrow keys*/
  noecho();                   /* Don't print key pressed to the screen */
  curs_set(0);                /* Hide cursor */

  return win; /* Return game window */
}

/**
 * @brief Function that moves the paddle
 *
 * @param win - game window in which the paddle is located
 * @param paddle - paddle to be moved
 * @return int - returns 1 if user wants to quit the game, 0 otherwise
 */
int move_paddle(WINDOW *win, Paddle &paddle)
{
  // loop responsible for creating paddle blocks
  // e.g if our paddle is of width 5 - then we need to create 5 separate 'blocks'
  for (int i = 0; i < paddle.width; i++)
  {
    mvwprintw(win, paddle.y, paddle.x + i, "-");
  }
  // Get a steering key from the user
  // If it's the 'q' key, return 1 to indicate that the user wants to quit the game
  int ch;
  ch = getch();
  switch (ch)
  {
  case 'q':
    return 1;
  // Move the paddle to the left on left arrow key press `<-`
  case KEY_LEFT:
    if (paddle.x > getbegx(win))
    {
      paddle.x--;
      // loop responsible for creating paddle blocks
      // e.g if our paddle is of width 5 - then we need to create 5 separate 'blocks'
      // Remove the previous paddle blocks
      for (int i = 0; i < paddle.width; i++)
      {
        mvwprintw(win, paddle.y, paddle.x + i, "-");
        mvwprintw(win, paddle.y, paddle.x + i + 1, " ");
      }
    }
    break;
    // Move the paddle to the right on right arrow key press `->`
  case KEY_RIGHT:
    if (paddle.x < getmaxx(win) - 1 - paddle.width)
    {
      paddle.x += 1;
      // loop responsible for creating paddle blocks
      // e.g if our paddle is of width 5 - then we need to create 5 separate 'blocks'
      // Remove the previous paddle blocks
      for (int i = 0; i < paddle.width; i++)
      {
        mvwprintw(win, paddle.y, paddle.x + i, "-");
        // In this conditional if statement, we check if the paddle is at the edge of the window. If it is, we don't want to print a space character
        // We would be printing a space character on the edge of the window, removing our border
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