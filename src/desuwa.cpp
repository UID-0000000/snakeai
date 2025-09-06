#define _CRT_SECURE_NO_WARNINGS

//ͷ�ļ�
#include <stdio.h>// ���������������
#include <stdlib.h>// �������������
#include <time.h>// ����ʱ�亯��
#include <vector>// ���ڶ�̬����
#include <windows.h>// ����Sleep����
#include <conio.h>// ����_getch����
#include <easyx.h>// EasyXͼ�ο�

// ��Ϸ��������
#define WIDTH 640// ���ڿ��
#define HEIGHT 480// ���ڸ߶�
#define GRID_SIZE 20// �����С

// ����ö��
enum Direction { UP, DOWN, LEFT, RIGHT };

// �����ṹ
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

// ̰������Ϸ��
class SnakeGame {
private:
    std::vector<Point> snake;// ����
    Point food;// ʳ��
    Direction dir;// ��ǰ����
    bool gameOver;// ��Ϸ������־
    int score;// ����

    // ���ĳ��λ���Ƿ�ȫ����ײǽ����ײ�Լ����壩
    bool isSafe(Point pos) {
        //����Ƿ�ײǽ,ʹ�����������ж�
        if (pos.x < 0 || pos.x >= WIDTH / GRID_SIZE ||
            pos.y < 0 || pos.y >= HEIGHT / GRID_SIZE) {
            return false;
        }

        //����Ƿ�ײ�Լ�������,β������Ϊβ�����ƶ�
        for (size_t i = 0; i < snake.size() - 1; ++i) {
            if (snake[i].x == pos.x && snake[i].y == pos.y) {
                return false;
            }
        }
        return true;
    }

    // ��ȡ���п��ܵİ�ȫ����,�ų��������ײ����
    std::vector<Direction> getSafeDirections() {
        std::vector<Direction> safeDirs;
        Point head = snake[0];

        // ��������,�ų���ǰ���µ����
        if (dir != DOWN) {
            Point nextPos = { head.x, head.y - 1 };
            if (isSafe(nextPos)) {
                safeDirs.push_back(UP);
            }
        }
        // ��������,�ų���ǰ���ϵ����
        if (dir != UP) {
            Point nextPos = { head.x, head.y + 1 };
            if (isSafe(nextPos)) {
                safeDirs.push_back(DOWN);
            }
        }
        // ��������,�ų���ǰ���ҵ����
        if (dir != RIGHT) {
            Point nextPos = { head.x - 1, head.y };
            if (isSafe(nextPos)) {
                safeDirs.push_back(LEFT);
            }
        }
        // ��������,�ų���ǰ��������
        if (dir != LEFT) {
            Point nextPos = { head.x + 1, head.y };
            if (isSafe(nextPos)) {
                safeDirs.push_back(RIGHT);
            }
        }
        return safeDirs;
    }

    // ����ĳ��������ʳ���"�ӽ���",ʹ���������پ��룬ֵԽСԽ�ӽ�
    int getDistanceScore(Direction dir) {
        Point head = snake[0];
        Point nextPos = head;
        switch (dir) {
        case UP:    nextPos.y--; break;
        case DOWN:  nextPos.y++; break;
        case LEFT:  nextPos.x--; break;
        case RIGHT: nextPos.x++; break;
        }
        return abs(nextPos.x - food.x) + abs(nextPos.y - food.y);
    }

public:
    SnakeGame() {
        initGame();
    }

    // ��ʼ����Ϸ
    void initGame() {
        snake = { Point(10, 10), Point(10, 11), Point(10, 12) };
        dir = UP;
        generateFood();
        gameOver = false;
        score = 0;
    }

    // ����ʳ��
    void generateFood() {
        srand(time(NULL));
        bool onSnake;
        do {
            onSnake = false;
            food.x = rand() % (WIDTH / GRID_SIZE);
            food.y = rand() % (HEIGHT / GRID_SIZE);

            // ���ʳ���Ƿ���������
            for (const auto& p : snake) {
                if (p.x == food.x && p.y == food.y) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);
    }

    // �ı䷽�򣨽��ڲ�AIʹ�ã�
    void changeDirection(Direction newDir) {
        // ��ֹ180��ת��
        if ((dir == UP && newDir != DOWN) ||
            (dir == DOWN && newDir != UP) ||
            (dir == LEFT && newDir != RIGHT) ||
            (dir == RIGHT && newDir != LEFT)) {
            dir = newDir;
        }
    }

    // �ƶ���
    void move() {
        Point newHead = snake[0];
        switch (dir) {
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
        }
        snake.insert(snake.begin(), newHead);

        // ����Ƿ�Ե�ʳ��
        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            generateFood();
        }
        else {
            snake.pop_back();
        }
    }

    // ��ײ���
    bool checkCollision() {
        Point head = snake[0];
        // ײǽ���
        if (head.x < 0 || head.x >= WIDTH / GRID_SIZE ||
            head.y < 0 || head.y >= HEIGHT / GRID_SIZE) {
            return true;
        }

        // ײ������
        for (size_t i = 1; i < snake.size(); i++) {
            if (head.x == snake[i].x && head.y == snake[i].y) {
                return true;
            }
        }
        return false;
    }

    // ������Ϸ
    void draw() {
        cleardevice();
        // ����
        setfillcolor(GREEN);
        for (const auto& p : snake) {
            fillrectangle(p.x * GRID_SIZE, p.y * GRID_SIZE,
                (p.x + 1) * GRID_SIZE - 1, (p.y + 1) * GRID_SIZE - 1);
        }

        // ��ʳ��
        setfillcolor(RED);
        fillcircle((food.x + 0.5) * GRID_SIZE, (food.y + 0.5) * GRID_SIZE, GRID_SIZE / 2);

        // ������
        settextcolor(BLACK);
        TCHAR text[20];
        _stprintf(text, _T("Score: %d"), score);
        outtextxy(10, 10, text);
    }

    // ������Ϸ״̬
    void update() {
        if (gameOver) return;
        move();
        if (checkCollision()) {
            gameOver = true;
        }
    }

    // AI�����߼�
    void aiControl() {
        if (gameOver) return;

        Point head = snake[0];
        // 1. ��ȡ���а�ȫ����
        std::vector<Direction> safeDirs = getSafeDirections();
        if (safeDirs.empty()) {
            // û�а�ȫ������Ϸ����
            gameOver = true;
            return;
        }

        // 2. �Ӱ�ȫ������ѡ����ӽ�ʳ��ķ���
        Direction bestDir = safeDirs[0];
        int minScore = getDistanceScore(bestDir);
        for (Direction d : safeDirs) {
            int score = getDistanceScore(d);
            if (score < minScore) {
                minScore = score;
                bestDir = d;
            }
        }

        // 3. ִ�����ŷ���
        changeDirection(bestDir);
    }

    bool isGameOver() { return gameOver; }
};

// ������
int main() {
    // ����ͼ�δ���
    initgraph(WIDTH, HEIGHT);
    setbkcolor(WHITE);
    cleardevice();

    // ������Ϸ����
    SnakeGame game;

    // ��Ϸ��ѭ��
    while (true) {
        // AI�Զ����ƣ�Ψһ���Ʒ�ʽ��
        game.aiControl();

        // ������Ϸ״̬
        game.update();

        // ������Ϸ
        game.draw();

        // ������Ϸ�ٶ�
        Sleep(100);

        // ��Ϸ��������
        if (game.isGameOver()) {
            settextcolor(RED);
            outtextxy(WIDTH / 2 - 60, HEIGHT / 2, _T("Game Over!"));
            outtextxy(WIDTH / 2 - 100, HEIGHT / 2 + 30, _T("Press any key to exit"));
            _getch();// �ȴ������˳�
            break;
        }
    }

    // �رմ���
    closegraph();
    return 0;
}
