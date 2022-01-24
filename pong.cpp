#include <ncurses.h>
#include <unistd.h>

class Pong
{
public:
  int xPos = 5;
  int yPos = 5;
};

int main(int argc, char *argv[])
{
  Pong mainPong;

  WINDOW *okno;
  okno = initscr();
  nodelay(okno, 1);
  raw();                /* Line buffering disabled	*/
  keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
  noecho();             /* Don't echo() while we do getch */
  curs_set(0);
  mvprintw(mainPong.yPos, mainPong.xPos, "o");
  refresh();
  while (true)
  {
    int key = getch();
    if (key == 'k')
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
    else if (key == KEY_UP)
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
    }
    mvprintw(mainPong.yPos, mainPong.xPos, "o");
  }
  endwin();
}