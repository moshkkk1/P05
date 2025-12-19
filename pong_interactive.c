#include <math.h>
#include <ncurses.h>
#include <stdio.h>

/*
Инициализация глобальных переменных:
1) Для ракеток
2) Для мяча - координаты и направление
3) Для игрока, его очки
*/
#define WIDTH 80
#define HEIGHT 25
#define WIN 21
#define PADDLE_SIZE 3
#define BALL_SIZE 1

int y_pad1;
int y_pad2;

int y_ball, x_ball;
int dx, dy;

int score1 = 0; // счёт игрока 1 (слева)
int score2 = 0; // счёт игрока 2 (справа)

/*
Инициализация ракеток, мяча и очков.
Цифры научно подобраны, чтобы ракетки стояли в середине.
*/
void init(void) {
  y_pad1 = (HEIGHT - PADDLE_SIZE) / 2;
  y_pad2 = (HEIGHT - PADDLE_SIZE) / 2;

  y_ball = HEIGHT / 2;
  x_ball = WIDTH / 2;
  dx = 1;
  dy = 1;

  score1 = 0;
  score2 = 0;
}

/*
Функция движения ракеток. В зависимости от букв выбираем куда ракетка пойдет.
*/
void move_pad(char z) {
  switch (z) {
  case 'A':
  case 'a':
    if (y_pad1 > 1) {
      y_pad1--;
    }
    break;
  case 'Z':
  case 'z':
    if (y_pad1 < HEIGHT - PADDLE_SIZE - 1) {
      y_pad1++;
    }
    break;
  case 'K':
  case 'k':
    if (y_pad2 > 1) {
      y_pad2--;
    }
    break;
  case 'M':
  case 'm':
    if (y_pad2 < HEIGHT - PADDLE_SIZE - 1) {
      y_pad2++;
    }
    break;
  default:
    break;
  }
}

/*
Функция движения мяча с физикой отскока от ракеток и стен.
*/
void move_ball(void) {
  x_ball += dx;
  y_ball += dy;

  // Отскок от верхней и нижней границ
  if (y_ball <= 0) {
    y_ball = 1;
    dy = 1;
  }

  if (y_ball >= HEIGHT - 1) {
    y_ball = HEIGHT - 2;
    dy = -1;
  }

  // Отскок от левой ракетки
  if (x_ball == 1 && y_ball >= y_pad1 && y_ball < y_pad1 + PADDLE_SIZE) {
    dx = 1;
    int hit_pos = y_ball - y_pad1;
    if (hit_pos == 0) {
      dy = -1;
    } else if (hit_pos == PADDLE_SIZE - 1) {
      dy = 1;
    }
  }
  // Отскок от правой ракетки
  else if (x_ball == WIDTH - 2 && y_ball >= y_pad2 &&
           y_ball < y_pad2 + PADDLE_SIZE) {
    dx = -1;
    int hit_pos = y_ball - y_pad2;
    if (hit_pos == 0) {
      dy = -1;
    } else if (hit_pos == PADDLE_SIZE - 1) {
      dy = 1;
    }
  }

  // Голы - мяч вышел за границы поля
  if (x_ball < 0) {
    // Очки игроку 2
    score2++;
    x_ball = WIDTH / 2;
    y_ball = HEIGHT / 2;
    dx = 1;
    dy = 1;
  } else if (x_ball >= WIDTH) {
    // Очки игроку 1
    score1++;
    x_ball = WIDTH / 2;
    y_ball = HEIGHT / 2;
    dx = -1;
    dy = 1;
  }
}

/*
Простая задержка без системных вызовов
*/
void simple_delay(void) {
  for (int i = 0; i < 100000000; i++) {
    // Пустой цикл для создания задержки
  }
}

/*
Функция отрисовки игрового поля с использованием ncurses.
*/
void draw_playground(void) {
  clear(); // Очистка экрана

  // Заголовок со счетом
  mvprintw(0, (WIDTH - 30) / 2, "=== PONG GAME ===");
  mvprintw(1, (WIDTH - 25) / 2, "Player 1: %d  |  Player 2: %d", score1,
           score2);
  mvprintw(2, (WIDTH - 35) / 2, "First to %d wins! (Q to quit)", WIN);

  // Отрисовка игрового поля
  for (int i = 3; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIDTH; j++) {
      // Границы поля
      if (i == 3 || i == HEIGHT + 1) {
        if (j == 0 || j == WIDTH - 1)
          mvaddch(i, j, '+');
        else
          mvaddch(i, j, '-');
      } else if (j == 0 || j == WIDTH - 1) {
        mvaddch(i, j, '|');
      } else {
        // Левая ракетка (x = 1)
        if (j == 1 && i - 3 >= y_pad1 && i - 3 < y_pad1 + PADDLE_SIZE) {
          mvaddch(i, j, '|');
        }
        // Правая ракетка (x = WIDTH - 2)
        else if (j == WIDTH - 2 && i - 3 >= y_pad2 &&
                 i - 3 < y_pad2 + PADDLE_SIZE) {
          mvaddch(i, j, '|');
        }
        // Мяч
        else if (j == x_ball && i - 3 == y_ball) {
          mvaddch(i, j, 'O');
        }
      }
    }
  }

  // Информация об управлении
  mvprintw(HEIGHT + 3, 2, "Controls: A/Z - Player 1, K/M - Player 2, Q - Quit");

  refresh(); // Обновление экрана
}

/*
Основная функция интерактивной игры.
*/
int main(void) {
  // Инициализация ncurses
  initscr();
  cbreak(); // Немедленный ввод символов без ожидания Enter
  noecho(); // Не отображать введенные символы
  keypad(stdscr, TRUE); // Включить обработку специальных клавиш
  timeout(0); // Неблокирующий ввод

  printf("=== ИГРА В ПОНГ (ИНТЕРАКТИВНАЯ ВЕРСИЯ) ===\n");
  printf("Управление:\n");
  printf("Игрок 1 (левая ракетка): A - вверх, Z - вниз\n");
  printf("Игрок 2 (правая ракетка): K - вверх, M - вниз\n");
  printf("Выход из игры: Q\n");
  printf("Игра ведется в реальном времени.\n");
  printf("Мяч движется автоматически, вы управляете только ракетками!\n");
  printf("Нажмите любую клавишу для начала игры...\n");
  getch(); // Ждем нажатия любой клавиши

  init();

  int game_running = 1;

  while (game_running && score1 < WIN && score2 < WIN) {
    // Обработка ввода
    int ch = getch();

    if (ch != ERR) { // Если была нажата клавиша
      switch (ch) {
      case 'A':
      case 'a':
      case 'Z':
      case 'z':
      case 'K':
      case 'k':
      case 'M':
      case 'm':
        move_pad((char)ch);
        break;
      case 'Q':
      case 'q':
        game_running = 0;
        break;
      default:
        break;
      }
    }

    // Движение мяча
    move_ball();

    // Отрисовка поля
    draw_playground();

    // Простая задержка для контроля скорости игры
    simple_delay();
  }

  // Завершение ncurses
  endwin();

  printf("\n=== ИГРА ЗАВЕРШЕНА ===\n");
  printf("Player 1: %d очков\n", score1);
  printf("Player 2: %d очков\n", score2);

  if (score1 == WIN) {
    printf("Победитель: Player 1!\n");
  } else if (score2 == WIN) {
    printf("Победитель: Player 2!\n");
  }

  return 0;
}