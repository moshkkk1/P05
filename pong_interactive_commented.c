#include <ncurses.h>  // Подключаем библиотеку ncurses для работы с консольным интерфейсом
#include <stdio.h>    // Подключаем стандартную библиотеку ввода-вывода

// Определяем константы размеров игрового поля
#define FIELD_WIDTH 80     // Ширина игрового поля в символах
#define FIELD_HEIGHT 25    // Высота игрового поля в строках
#define PADDLE_SIZE 3      // Размер ракетки (высота в символах)
#define BALL_SIZE 1        // Размер мяча (всегда 1 символ)

// Переменные для позиции и движения мяча
int ball_x = FIELD_WIDTH / 2;  // Текущая позиция мяча по горизонтали (в центре поля)
int ball_y = FIELD_HEIGHT / 2; // Текущая позиция мяча по вертикали (в центре поля)
int ball_vx = 1;               // Скорость мяча по горизонтали (1 = движется вправо)
int ball_vy = 1;               // Скорость мяча по вертикали (1 = движется вниз)

// Переменные для позиции ракеток
int left_paddle_y = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2;  // Позиция левой ракетки по вертикали
int right_paddle_y = FIELD_HEIGHT / 2 - PADDLE_SIZE / 2; // Позиция правой ракетки по вертикали

// Счетчики очков игроков
int left_score = 0;   // Счет игрока 1 (левая ракетка)
int right_score = 0;  // Счет игрока 2 (правая ракетка)
int game_running = 1; // Флаг, показывающий, продолжается ли игра (1 = да, 0 = нет)

// Переменные для управления ракетками (для простого управления)
int left_paddle_up = 0;    // Флаг движения левой ракетки вверх
int left_paddle_down = 0;  // Флаг движения левой ракетки вниз
int right_paddle_up = 0;   // Флаг движения правой ракетки вверх
int right_paddle_down = 0; // Флаг движения правой ракетки вниз

// Функция обновления позиции и движения мяча
void update_ball() {
    // Мяч движется согласно своей скорости
    ball_x += ball_vx;  // Двигаем мяч по горизонтали
    ball_y += ball_vy;  // Двигаем мяч по вертикали

    // Проверяем столкновение с верхней и нижней границами поля
    if (ball_y <= 1 || ball_y >= FIELD_HEIGHT - 2) {
        ball_vy = -ball_vy;    // Меняем направление по вертикали (отскок от стенки)
        ball_y += ball_vy;     // Немного двигаем мяч, чтобы он не застрял в стенке
    }

    // Проверяем столкновение с левой ракеткой
    if (ball_x <= 3 && ball_vx < 0 && ball_y >= left_paddle_y && ball_y < left_paddle_y + PADDLE_SIZE) {
        ball_vx = -ball_vx;    // Меняем направление по горизонтали (отскок от ракетки)
        ball_x = 4;            // Немного отодвигаем мяч от ракетки

        // Вычисляем место попадания мяча по ракетке для изменения вертикальной скорости
        int paddle_center = left_paddle_y + PADDLE_SIZE / 2;  // Центр ракетки
        int hit_position = ball_y - paddle_center;            // Позиция попадания относительно центра
        ball_vy += hit_position / 2;                          // Изменяем вертикальную скорость
    }

    // Проверяем столкновение с правой ракеткой
    if (ball_x >= FIELD_WIDTH - 4 && ball_vx > 0 && ball_y >= right_paddle_y &&
        ball_y < right_paddle_y + PADDLE_SIZE) {
        ball_vx = -ball_vx;    // Меняем направление по горизонтали
        ball_x = FIELD_WIDTH - 5; // Немного отодвигаем мяч от ракетки

        // Аналогично левой ракетке - вычисляем место попадания
        int paddle_center = right_paddle_y + PADDLE_SIZE / 2;
        int hit_position = ball_y - paddle_center;
        ball_vy += hit_position / 2;
    }

    // Проверяем, вышел ли мяч за границы поля (гол)
    if (ball_x <= 1) {
        right_score++;  // Очко получает правый игрок

        // Сбрасываем мяч в центр поля
        ball_x = FIELD_WIDTH / 2;
        ball_y = FIELD_HEIGHT / 2;
        ball_vx = -1;   // Мяч будет двигаться влево
        ball_vy = 1;    // Мяч будет двигаться вниз
    } else if (ball_x >= FIELD_WIDTH - 2) {
        left_score++;   // Очко получает левый игрок

        // Сбрасываем мяч в центр поля
        ball_x = FIELD_WIDTH / 2;
        ball_y = FIELD_HEIGHT / 2;
        ball_vx = 1;    // Мяч будет двигаться вправо
        ball_vy = 1;    // Мяч будет двигаться вниз
    }
}

// Функция обновления позиций ракеток (для простого управления)
void update_paddles() {
    // Движение левой ракетки вверх
    if (left_paddle_up && left_paddle_y > 1) {
        left_paddle_y--;  // Уменьшаем позицию Y (движение вверх)
    }
    // Движение левой ракетки вниз
    if (left_paddle_down && left_paddle_y < FIELD_HEIGHT - PADDLE_SIZE - 2) {
        left_paddle_y++;  // Увеличиваем позицию Y (движение вниз)
    }

    // Движение правой ракетки вверх
    if (right_paddle_up && right_paddle_y > 1) {
        right_paddle_y--;
    }
    // Движение правой ракетки вниз
    if (right_paddle_down && right_paddle_y < FIELD_HEIGHT - PADDLE_SIZE - 2) {
        right_paddle_y++;
    }
}

// Функция движения левой ракетки
void move_left_paddle(int direction) {
    if (direction < 0) {  // Если direction меньше 0, двигаемся вверх
        if (left_paddle_y > 1) {  // Проверяем, что не выходим за верхнюю границу
            left_paddle_y--;      // Двигаем ракетку вверх
        }
    } else if (direction > 0) {  // Если direction больше 0, двигаемся вниз
        if (left_paddle_y < FIELD_HEIGHT - PADDLE_SIZE - 2) {  // Проверяем нижнюю границу
            left_paddle_y++;      // Двигаем ракетку вниз
        }
    }
}

// Функция движения правой ракетки (аналогично левой)
void move_right_paddle(int direction) {
    if (direction < 0) {  // Движение вверх
        if (right_paddle_y > 1) {
            right_paddle_y--;
        }
    } else if (direction > 0) {  // Движение вниз
        if (right_paddle_y < FIELD_HEIGHT - PADDLE_SIZE - 2) {
            right_paddle_y++;
        }
    }
}

// Функция отрисовки игрового поля и всех объектов
void draw_field() {
    int i, j;  // Счетчики для циклов

    clear();  // Очищаем экран

    // Рисуем верхнюю границу поля
    for (i = 0; i < FIELD_WIDTH; i++) {
        printw("-");  // Печатаем символ границы
    }
    printw("\n");  // Переходим на новую строку

    // Рисуем внутреннюю часть поля
    for (j = 1; j < FIELD_HEIGHT - 1; j++) {
        printw("|");  // Рисуем левую границу строки

        // Рисуем содержимое строки
        for (i = 1; i < FIELD_WIDTH - 1; i++) {
            int printed = 0;  // Флаг, показывающий, напечатали ли что-то в этой позиции

            // Проверяем, находится ли здесь мяч
            if (i == ball_x && j == ball_y) {
                printw("O");  // Рисуем мяч символом 'O'
                printed = 1;  // Отмечаем, что что-то напечатано
            }
            // Проверяем, находится ли здесь левая ракетка
            else if (i == 2 && j >= left_paddle_y && j < left_paddle_y + PADDLE_SIZE) {
                printw("|");  // Рисуем ракетку символом '|'
                printed = 1;
            }
            // Проверяем, находится ли здесь правая ракетка
            else if (i == FIELD_WIDTH - 3 && j >= right_paddle_y && j < right_paddle_y + PADDLE_SIZE) {
                printw("|");  // Рисуем ракетку символом '|'
                printed = 1;
            }
            // Рисуем центральную разделительную линию
            else if (i == FIELD_WIDTH / 2 && j % 2 == 0) {
                printw("|");  // Вертикальная линия по центру поля
                printed = 1;
            }

            // Если ничего не напечатано, рисуем пробел
            if (!printed) {
                printw(" ");
            }
        }

        printw("|\n");  // Рисуем правую границу строки и переходим на новую строку
    }

    // Рисуем нижнюю границу поля
    for (i = 0; i < FIELD_WIDTH; i++) {
        printw("-");
    }
    printw("\n");

    // Выводим счет и инструкции по управлению
    printw("Igrok 1 (A/Z): %d | Igrok 2 (K/M): %d\n", left_score, right_score);
    printw("Upravlenie: A/Z - Igrok 1, K/M - Igrok 2, Q - Vihod\n");

    refresh();  // Обновляем экран, показывая все напечатанное
}

// Главная функция программы
int main() {
    int ch;  // Переменная для хранения нажатой клавиши

    // Инициализация ncurses
    initscr();           // Начинаем работу с ncurses
    cbreak();            // Режим немедленного ввода символов (без ожидания Enter)
    noecho();            // Не отображать вводимые символы на экране
    keypad(stdscr, TRUE); // Разрешаем обработку специальных клавиш (стрелки и т.д.)
    timeout(100);        // Ожидание ввода не более 100 миллисекунд

    // Выводим стартовое сообщение
    printf("PONG - Interaktivnaya virsiya\n");
    printf("Inizializaziya ncurses...\n");

    // Основной игровой цикл
    while (game_running) {
        ch = getch();  // Получаем нажатую клавишу

        // Обрабатываем нажатую клавишу
        switch (ch) {
            case 'a':
            case 'A':  // Игрок 1 двигает ракетку вверх
                move_left_paddle(-1);
                break;
            case 'z':
            case 'Z':  // Игрок 1 двигает ракетку вниз
                move_left_paddle(1);
                break;
            case 'k':
            case 'K':  // Игрок 2 двигает ракетку вверх
                move_right_paddle(-1);
                break;
            case 'm':
            case 'M':  // Игрок 2 двигает ракетку вниз
                move_right_paddle(1);
                break;
            case 'q':
            case 'Q':  // Выход из игры
                game_running = 0;
                break;
            case KEY_UP:    // Стрелка вверх (игрок 1)
                move_left_paddle(-1);
                break;
            case KEY_DOWN:  // Стрелка вниз (игрок 1)
                move_left_paddle(1);
                break;
            default:  // Любая другая клавиша - ничего не делаем
                break;
        }

        update_ball();    // Обновляем позицию и движение мяча
        draw_field();     // Рисуем игровое поле

        // Проверяем, закончилась ли игра (кто-то набрал 10 очков)
        if (left_score >= 10 || right_score >= 10) {
            clear();
            printw("Igra zakonchilas!\n");
            if (left_score >= 10) {
                printw("Igrok 1 pobedil!\n");  // Победа игрока 1
            } else {
                printw("Igrok 2 pobedil!\n");  // Победа игрока 2
            }
            printw("Nazhmite lybuyu knopku chtobi viyti...");
            refresh();
            getch();  // Ждем нажатия любой клавиши
            game_running = 0;  // Завершаем игру
        }
    }

    endwin();  // Завершаем работу с ncurses

    return 0;  // Программа завершена успешно
}
