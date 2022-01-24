#include <ncurses.h>
#include <unistd.h>

class Pong
{
public:
  int xPos;
  int yPos;
  int width = 8;
};

class Ball
{
public:
  int xPos;
  int yPos;
  int xVel;
  int yVel;
};

int main(int argc, char *argv[])
{

  WINDOW *okno;         /* Game window */
  okno = initscr();     /* Start curses mode */
  nodelay(okno, 1);     /* Dont wait for input */
  raw();                /* Line buffering disabled	*/
  keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
  noecho();             /* Don't echo() while we do getch */
  curs_set(0);          /* Invisible cursor */

  /* Counter allows us to move the ball with delay */
  int counter = 0;
  Pong mainPong; /* The main pong */
  /* Set default position of main pong */
  mainPong.yPos = LINES - 1;
  mainPong.xPos = COLS / 2;

  /* Set default position of ball */
  Ball ball;
  ball.xPos = COLS / 2;
  ball.yPos = 0;
  ball.xVel = 1;
  ball.yVel = 1;

  unsigned score = 0;

  /* Main loop */
  while (true)
  {
    mvprintw(1, 1, "Score: %d", score);
    counter++;
    int key = getch();
    if (key == 'k' || key == 'q')
    {
      break;
    }
    /* Move main pong */
    if (key == KEY_LEFT && mainPong.xPos > 0)
    {
      mainPong.xPos--;
    }
    else if (key == KEY_RIGHT && mainPong.xPos < COLS - mainPong.width)
    {
      mainPong.xPos++;
    }

    clear();
    /* Move ball with delay from counter */
    if (counter % 30 == 0)
    {
      ball.xPos += ball.xVel;
      ball.yPos += ball.yVel;
    }
    /* Bounce the ball */
    if (ball.yPos == LINES - 2)
    {
      for (int i = mainPong.xPos; i < mainPong.xPos + mainPong.width; i++)
      {
        if (ball.xPos == i)
        {
          ball.yVel = -1;
          score++;
        }
      }
    }
    if (ball.yPos == 0)
    {
      ball.yVel = 1;
    }
    if (ball.xPos == 0)
    {
      ball.xVel = 1;
    }
    if (ball.xPos == COLS - 1)
    {
      ball.xVel = -1;
    }

    mvprintw(ball.yPos, ball.xPos, "o");
    for (int i = 0; i < mainPong.width; i++)
    {
      mvprintw(mainPong.yPos, mainPong.xPos + i, "-");
    }

    usleep(5000);

    refresh();
  }

  endwin();
}