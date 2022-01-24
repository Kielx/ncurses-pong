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
  int counter = 0;
  WINDOW *okno;         /* Game window */
  okno = initscr();     /* Start curses mode */
  nodelay(okno, 1);     /* Dont wait for input */
  raw();                /* Line buffering disabled	*/
  keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
  noecho();             /* Don't echo() while we do getch */
  curs_set(0);          /* Invisible cursor */

  Pong mainPong; /* The main pong */
  /* Set default position of main pong */
  mainPong.yPos = LINES - 1;
  mainPong.xPos = COLS / 2;
  mvprintw(mainPong.yPos, mainPong.xPos, "o");

  Ball ball;
  ball.xPos = COLS / 2;
  ball.yPos = 0;
  ball.xVel = 1;
  ball.yVel = 1;
  mvprintw(ball.yPos, ball.xPos, "o");
  refresh();

  /* Steering loop */
  while (true)
  {
    counter++;
    int key = getch();
    if (key == 'k' || key == 'q')
    {
      break;
    }
    if (key == KEY_LEFT)
    {
      mainPong.xPos--;
      refresh();
      clear();
    }
    else if (key == KEY_RIGHT)
    {
      mainPong.xPos++;
      refresh();
      clear();
    }
    /* else if (key == KEY_UP)
    {
      mainPong.yPos--;
      refresh();
      clear();
    }
    else if (key == KEY_DOWN)
    {
      mainPong.yPos++;
      refresh();
      clear();
    } */
    clear();
    if (counter % 20 == 0)
    {
      ball.xPos += ball.xVel;
      ball.yPos += ball.yVel;
    }
    if (ball.yPos == LINES - 2)
    {
      for (int i = mainPong.xPos; i < mainPong.xPos + mainPong.width; i++)
      {
        if (ball.xPos == i)
        {
          ball.yVel = -1;
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
      mvprintw(mainPong.yPos, mainPong.xPos + i, "o");
    }

    usleep(5 * 1000);

    refresh();
  }

  endwin();
}