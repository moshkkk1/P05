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

int y_pad1;
int y_pad2;

int y_ball, x_ball;
int dx, dy;

int score1 = 0;  // счёт игрока 1 (слева)
int score2 = 0;  // счёт игрока 2 (справа)

/*
Инициализация ракеток, мяча и очков. В ракетках мы инициализируем верхнюю черту, а середину и дно будем рисовать
уже в самом поле.
Цифры не рандомные, а научно натыканные, чтоб ракетки стояли в середине.
*/
void init(void) {
    y_pad1 = (HEIGHT - 3) / 2;
    y_pad2 = (HEIGHT - 3) / 2;

    y_ball = HEIGHT / 2;
    x_ball = WIDTH / 2;
    dx = 1;
    dy = 1;

    score1 = 0;
    score2 = 0;
}

/*
Функция движения ракеток. В зависимости от букв выбираем куда ракетка пойдет.
Примичание! Если вписать список букв, то оно не будет выполненно одновременно.
Сначала первое действие, полет мяча, потом второе и опять полет мяча.
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
        if (y_pad1 < 21) {
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
        if (y_pad2 < 21) {
            y_pad2++;
        }
        break;
    case ' ':
        break;
    default:
        break;
    }
}

/*
Функция движения мяча. Всегда все одинаково, он летит вниз вправо.
*/
void move_ball(void) {
    x_ball += dx;
    y_ball += dy;

    if (y_ball <= 0) {
        y_ball = 1;
        dy = 1;
    }

    if (y_ball >= HEIGHT - 1) {
        y_ball = HEIGHT - 2;
        dy = -1;
    }

    /*
    Отскок от ракеток. Тут подробнее. Ракетка имеет три секции. Если мяч прилетел в верхнюю часть ракетки, то и отскок будет вверх, если
    в нижнюю часть, то вниз. Это добавлено специально, ибо без данного функционала, игра превращалась в бесконечный алгоритм повторяющихся действий.
    А так игра получает хоть небольшую дозу неопределенности.
    */
    // Левая ракетка
    if (x_ball == 1 && y_ball >= y_pad1 && y_ball < y_pad1 + 3) {
        dx = 1;
        int hit_pos = y_ball - y_pad1;
        if (hit_pos == 0) {
            dy = -1;
        }
        else if (hit_pos == 2) {
            dy = 1;
        }
    }
    // Правая ракетка
    else if (x_ball == WIDTH - 2 && y_ball >= y_pad2 && y_ball < y_pad2 + 3) {
        dx = -1;
        int hit_pos = y_ball - y_pad2;
        if (hit_pos == 0) {
            dy = -1;
        }
        else if (hit_pos == 2) {
            dy = 1;
        }
    }

    /*
    Голы. Но тут все по базе, прилетело в левую стену, очки второму игроку и наоборот.
    */
    if (x_ball < 0) {
        // Очки игроку 2
        score2++;
        x_ball = WIDTH / 2;
        y_ball = HEIGHT / 2;
        dx = 1;
        dy = 1;
    }
    else if (x_ball >= WIDTH) {
        // Очки игроку 1
        score1++;
        x_ball = WIDTH / 2;
        y_ball = HEIGHT / 2;
        dx = -1;
        dy = 1;
    }
}

/*
Функция чтения команды. Ожидает корректный ввод от игроков.
В пошаговом режиме один из игроков должен передвинуть свою ракетку или пропустить ход.
*/
char read_command(void) {
    char c;
    int valid_input = 0;
    
    while (!valid_input) {
        printf("\nВведите команду (A/Z - игрок 1, K/M - игрок 2, Space - пропуск ход): ");
        c = getchar();
        
        // Пропускаем символы переноса строки
        while (getchar() != '\n');
        
        // Проверяем корректность ввода
        if (c == 'A' || c == 'a' || c == 'Z' || c == 'z' || 
            c == 'K' || c == 'k' || c == 'M' || c == 'm' || 
            c == ' ') {
            valid_input = 1;
        } else {
            printf("Неверная команда! Используйте A, Z, K, M или Space.\n");
        }
    }
    
    return c;
}

void playground(void) {
    printf("=== Player 1: %d  |  Player 2: %d === (first to %d wins)\n", score1, score2, WIN);
    
    // Прямая отрисовка без массива
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char cell = ' ';
            
            // Границы поля
            if (i == 0 || i == HEIGHT - 1) {
                if (j == 0 || j == WIDTH - 1)
                    cell = '+';
                else
                    cell = '-';
            }
            else if (j == 0 || j == WIDTH - 1) {
                cell = '|';
            }
            else {
                // Левая ракетка (x = 1)
                if (j == 1 && i >= y_pad1 && i < y_pad1 + 3) {
                    cell = '|';
                }
                // Правая ракетка (x = WIDTH - 2)
                else if (j == WIDTH - 2 && i >= y_pad2 && i < y_pad2 + 3) {
                    cell = '|';
                }
                // Мяч
                else if (j == x_ball && i == y_ball) {
                    cell = 'O';
                }
            }
            
            printf("%c", cell);
        }
        printf("\n");
    }
}

/*
База. Тут вызывается сразу все функции выше, но в цикле.
По порядку:
1) Инициализация всего: ракеток, мяча, очков.
2) Сам цикл. Крутится пока кто-то не наберет 21.
3) Двигаем ракету! Одну.
4) Двигаем мяч.
5) Все крутится, пока не закончится.
*/
int main(void) {
    printf("=== ИГРА В ПОНГ ===\n");
    printf("Управление:\n");
    printf("Игрок 1 (левая ракетка): A - вверх, Z - вниз\n");
    printf("Игрок 2 (правая ракетка): K - вверх, M - вниз\n");
    printf("Пропуск хода: Space Bar\n");
    printf("Игра ведется в пошаговом режиме.\n");
    printf("После каждого действия происходит отрисовка поля.\n");
    printf("Первый игрок, набравший %d очков, побеждает!\n", WIN);
    printf("Нажмите Enter для начала игры...\n");
    getchar();  // Ждем нажатия Enter
    
    init();
    char cmd;
    while (score1 < WIN && score2 < WIN) {
        playground();
        cmd = read_command();
        move_pad(cmd);
        move_ball();
    }
    
    printf("\nИгра завершена!\n");
    printf("Player 1: %d очков\n", score1);
    printf("Player 2: %d очков\n", score2);
    
    if (score1 == WIN) {
        printf("Победитель: Player 1!\n");
    } else if (score2 == WIN) {
        printf("Победитель: Player 2!\n");
    }
    
    return 0;
}