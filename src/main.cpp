#include <easyx.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

// ��Ϸ��������
#define WIDTH 640
#define HEIGHT 480
#define GRID_SIZE 20

// ����ö��
enum Direction { UP, DOWN, LEFT, RIGHT };

// ����ڵ�ṹ
struct Point {
    int x;
    int y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

// ��Ϸ��
class SnakeGame {
private:
    std::vector<Point> snake;  // ����
    Point food;                // ʳ��
    Direction dir;             // ��ǰ����
    bool gameOver;             // ��Ϸ������־
    int score;                 // ����

public:
    SnakeGame() {
        // ��ʼ����Ϸ
        initGame();
    }

    // ��ʼ����Ϸ
    void initGame() {
        // ��ʼ����
        snake.clear();
        snake.push_back(Point(10, 10));
        snake.push_back(Point(10, 11));
        snake.push_back(Point(10, 12));

        // ��ʼ����
        dir = UP;

        // ����ʳ��
        generateFood();

        // ��ʼ����Ϸ״̬
        gameOver = false;
        score = 0;
    }

    // ����ʳ���ȫʾ����
    void generateFood() {
        srand(time(NULL));
        bool onSnake;
        do {
            onSnake = false;
            // �������ʳ��λ�ã�ȷ���������ڣ�
            food.x = rand() % (WIDTH / GRID_SIZE);
            food.y = rand() % (HEIGHT / GRID_SIZE);
            // ����Ƿ��������ص�
            for (const auto& p : snake) {
                if (p.x == food.x && p.y == food.y) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);  // ���ص�����������
    }

    // �ı䷽�򣨲�ȫʾ����
    void changeDirection(Direction newDir) {
        // ��ֹ180��ת���統ǰ����ʱ����ֱ�����£�
        if ((dir == UP && newDir != DOWN) ||
            (dir == DOWN && newDir != UP) ||
            (dir == LEFT && newDir != RIGHT) ||
            (dir == RIGHT && newDir != LEFT)) {
            dir = newDir;
        }
    }

    // �ƶ��ߣ���ȫʾ����
    void move() {
        // ����ͷ = ����ͷ + ����ƫ��
        Point newHead = snake[0];
        switch (dir) {
        case UP: newHead.y--; break;
        case DOWN: newHead.y++; break;
        case LEFT: newHead.x--; break;
        case RIGHT: newHead.x++; break;
        }
        // ������ͷ��������
        snake.insert(snake.begin(), newHead);
        // ���Ե�ʳ����ӷ֣������Ƴ�β�����߳��Ȳ��䣩
        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            generateFood();  // ��������ʳ��
        }
        else {
            snake.pop_back();
        }
    }

    // ��ײ��⣨��ȫʾ����
    bool checkCollision() {
        Point head = snake[0];
        // ���ײǽ
        if (head.x < 0 || head.x >= WIDTH / GRID_SIZE ||
            head.y < 0 || head.y >= HEIGHT / GRID_SIZE) {
            return true;
        }
        // ���ײ����
        for (size_t i = 1; i < snake.size(); i++) {
            if (head.x == snake[i].x && head.y == snake[i].y) {
                return true;
            }
        }
        return false;
    }

    // ������Ϸ����ȫʾ����
    void draw() {
        cleardevice();  // �����Ļ
        // ��������
        setfillcolor(GREEN);
        for (const auto& p : snake) {
            fillrectangle(p.x * GRID_SIZE, p.y * GRID_SIZE,
                (p.x + 1) * GRID_SIZE - 1, (p.y + 1) * GRID_SIZE - 1);
        }
        // ����ʳ��
        setfillcolor(RED);
        fillcircle((food.x + 0.5) * GRID_SIZE, (food.y + 0.5) * GRID_SIZE, GRID_SIZE / 2);
        // ���Ʒ���
        settextcolor(BLACK);
        char scoreText[20];
        sprintf(scoreText, "Score: %d", score);
        outtextxy(10, 10, scoreText);
    }

    // ������Ϸ״̬
    void update() {
        if (!gameOver) {
            move();
            if (checkCollision()) {
                gameOver = true;
            }
        }
    }

    // AI����
    void aiControl() {
        // ʵ��AI�߼����Զ������ߵ��ƶ�
        // ...
    }

    // ��Ϸ�Ƿ����
    bool isGameOver() {
        return gameOver;
    }
};

int main() {
    // ��������
    initgraph(WIDTH, HEIGHT);
    setbkcolor(WHITE);
    cleardevice();

    // ������Ϸ����
    SnakeGame game;

    // ��Ϸ��ѭ��
    while (true) {
        // ��������
        if (_kbhit()) {
            char key = _getch();
            // �����������
        }

        // AI����
        game.aiControl();

        // ������Ϸ״̬
        game.update();

        // ������Ϸ
        game.draw();

        // ������Ϸ�ٶ�
        Sleep(100);

        // �����Ϸ����
        if (game.isGameOver()) {
            // ��Ϸ��������
            break;
        }
    }

    // �ر�ͼ�ν���
    closegraph();
    return 0;
}