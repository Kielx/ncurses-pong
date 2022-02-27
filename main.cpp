#include <ncurses.h>
#include <unistd.h>

class Ball
{
public:
  int x, y;
  int x_speed, y_speed;
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
 * @brief A function responsible for a single player game
 *
 * @param win - the window where the game takes place
 *
 * @return int - returns 1 if the user has lost
 */
int single_player(WINDOW *win)
{
  // Declare the ball and its initial position and speed
  Ball ball1;
  ball1.x = 10;
  ball1.y = 10;
  ball1.x_speed = 1;
  ball1.y_speed = 1;
  mvwprintw(win, ball1.y, ball1.x, "o");
  // Declare the paddle, its position in the middle of the screen on the X axis and on the penultimate line of the Y axis
  Paddle paddle1;
  paddle1.x = getmaxx(win) / 2;
  paddle1.y = getmaxy(win) - 2;
  paddle1.width = 5;

  // Counter allows you to move the ball with a delay
  int counter = 0;
  int score = 0;
  // Loop that contains all the logic of the single player game
  while (true)
  {
    counter++;
    mvwprintw(win, 0, 0, "Score: %d", score / 300);
    // Every 300 frames we move the ball, removing the old trail
    if (counter % 300 == 0)
    {
      mvwprintw(win, ball1.y, ball1.x, " ");
      ball1.x += ball1.x_speed;
      ball1.y += ball1.y_speed;
      mvwprintw(win, ball1.y, ball1.x, "o");
    }

    // Ball bounce function is called every frame
    ball_bounce(win, ball1, paddle1, score);
    if (ball1.y == getmaxy(win) - 1)
    {
      wclear(win);
      box(win, 0, 0);
      mvwprintw(win, getmaxy(win) / 2, getmaxx(win) / 2, "GAME OVER");
      mvwprintw(win, getmaxy(win) / 2 + 1, getmaxx(win) / 2, "Your score: %d", score / 300);
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
    // Function responsible for moving the paddle is called every frame
    int quit = move_paddle(win, paddle1);
    if (quit == 1)
    {
      break;
    }
    // We put the program to sleep for 500 microseconds and refresh the window with new data
    usleep(500);
    wrefresh(win);
  }
  return 0;
}

int main()
{

  WINDOW *win = init_screen();
  single_player(win);
  endwin();
  return 0;
}