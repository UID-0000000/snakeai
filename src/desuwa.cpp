#define _CRT_SECURE_NO_WARNINGS

//头文件
#include <stdio.h>// 用于输入输出函数
#include <stdlib.h>// 用于随机数生成
#include <time.h>// 用于时间函数
#include <vector>// 用于动态数组
#include <windows.h>// 用于Sleep函数
#include <conio.h>// 用于_getch函数
#include <easyx.h>// EasyX图形库

// 游戏常量定义
#define WIDTH 640// 窗口宽度
#define HEIGHT 480// 窗口高度
#define GRID_SIZE 20// 网格大小

// 方向枚举
enum Direction { UP, DOWN, LEFT, RIGHT };

// 坐标点结构
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

// 贪吃蛇游戏类
class SnakeGame {
private:
    std::vector<Point> snake;// 蛇身
    Point food;// 食物
    Direction dir;// 当前方向
    bool gameOver;// 游戏结束标志
    int score;// 分数

    // 检查某个位置是否安全（不撞墙、不撞自己身体）
    bool isSafe(Point pos) {
        //检查是否撞墙,使用网格坐标判断
        if (pos.x < 0 || pos.x >= WIDTH / GRID_SIZE ||
            pos.y < 0 || pos.y >= HEIGHT / GRID_SIZE) {
            return false;
        }

        //检查是否撞自己的身体,尾部，因为尾部会移动
        for (size_t i = 0; i < snake.size() - 1; ++i) {
            if (snake[i].x == pos.x && snake[i].y == pos.y) {
                return false;
            }
        }
        return true;
    }

    // 获取所有可能的安全方向,排除反向和碰撞方向
    std::vector<Direction> getSafeDirections() {
        std::vector<Direction> safeDirs;
        Point head = snake[0];

        // 尝试向上,排除当前向下的情况
        if (dir != DOWN) {
            Point nextPos = { head.x, head.y - 1 };
            if (isSafe(nextPos)) {
                safeDirs.push_back(UP);
            }
        }
        // 尝试向下,排除当前向上的情况
        if (dir != UP) {
            Point nextPos = { head.x, head.y + 1 };
            if (isSafe(nextPos)) {
                safeDirs.push_back(DOWN);
            }
        }
        // 尝试向左,排除当前向右的情况
        if (dir != RIGHT) {
            Point nextPos = { head.x - 1, head.y };
            if (isSafe(nextPos)) {
                safeDirs.push_back(LEFT);
            }
        }
        // 尝试向右,排除当前向左的情况
        if (dir != LEFT) {
            Point nextPos = { head.x + 1, head.y };
            if (isSafe(nextPos)) {
                safeDirs.push_back(RIGHT);
            }
        }
        return safeDirs;
    }

    // 计算某个方向与食物的"接近度",使用了曼哈顿距离，值越小越接近
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

    // 初始化游戏
    void initGame() {
        snake = { Point(10, 10), Point(10, 11), Point(10, 12) };
        dir = UP;
        generateFood();
        gameOver = false;
        score = 0;
    }

    // 生成食物
    void generateFood() {
        srand(time(NULL));
        bool onSnake;
        do {
            onSnake = false;
            food.x = rand() % (WIDTH / GRID_SIZE);
            food.y = rand() % (HEIGHT / GRID_SIZE);

            // 检查食物是否在蛇身上
            for (const auto& p : snake) {
                if (p.x == food.x && p.y == food.y) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);
    }

    // 改变方向（仅内部AI使用）
    void changeDirection(Direction newDir) {
        // 防止180度转向
        if ((dir == UP && newDir != DOWN) ||
            (dir == DOWN && newDir != UP) ||
            (dir == LEFT && newDir != RIGHT) ||
            (dir == RIGHT && newDir != LEFT)) {
            dir = newDir;
        }
    }

    // 移动蛇
    void move() {
        Point newHead = snake[0];
        switch (dir) {
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
        }
        snake.insert(snake.begin(), newHead);

        // 检查是否吃到食物
        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            generateFood();
        }
        else {
            snake.pop_back();
        }
    }

    // 碰撞检测
    bool checkCollision() {
        Point head = snake[0];
        // 撞墙检测
        if (head.x < 0 || head.x >= WIDTH / GRID_SIZE ||
            head.y < 0 || head.y >= HEIGHT / GRID_SIZE) {
            return true;
        }

        // 撞自身检测
        for (size_t i = 1; i < snake.size(); i++) {
            if (head.x == snake[i].x && head.y == snake[i].y) {
                return true;
            }
        }
        return false;
    }

    // 绘制游戏
    void draw() {
        cleardevice();
        // 画蛇
        setfillcolor(GREEN);
        for (const auto& p : snake) {
            fillrectangle(p.x * GRID_SIZE, p.y * GRID_SIZE,
                (p.x + 1) * GRID_SIZE - 1, (p.y + 1) * GRID_SIZE - 1);
        }

        // 画食物
        setfillcolor(RED);
        fillcircle((food.x + 0.5) * GRID_SIZE, (food.y + 0.5) * GRID_SIZE, GRID_SIZE / 2);

        // 画分数
        settextcolor(BLACK);
        TCHAR text[20];
        _stprintf(text, _T("Score: %d"), score);
        outtextxy(10, 10, text);
    }

    // 更新游戏状态
    void update() {
        if (gameOver) return;
        move();
        if (checkCollision()) {
            gameOver = true;
        }
    }

    // AI控制逻辑
    void aiControl() {
        if (gameOver) return;

        Point head = snake[0];
        // 1. 获取所有安全方向
        std::vector<Direction> safeDirs = getSafeDirections();
        if (safeDirs.empty()) {
            // 没有安全方向，游戏结束
            gameOver = true;
            return;
        }

        // 2. 从安全方向中选择最接近食物的方向
        Direction bestDir = safeDirs[0];
        int minScore = getDistanceScore(bestDir);
        for (Direction d : safeDirs) {
            int score = getDistanceScore(d);
            if (score < minScore) {
                minScore = score;
                bestDir = d;
            }
        }

        // 3. 执行最优方向
        changeDirection(bestDir);
    }

    bool isGameOver() { return gameOver; }
};

// 主函数
int main() {
    // 创建图形窗口
    initgraph(WIDTH, HEIGHT);
    setbkcolor(WHITE);
    cleardevice();

    // 创建游戏对象
    SnakeGame game;

    // 游戏主循环
    while (true) {
        // AI自动控制（唯一控制方式）
        game.aiControl();

        // 更新游戏状态
        game.update();

        // 绘制游戏
        game.draw();

        // 控制游戏速度
        Sleep(100);

        // 游戏结束处理
        if (game.isGameOver()) {
            settextcolor(RED);
            outtextxy(WIDTH / 2 - 60, HEIGHT / 2, _T("Game Over!"));
            outtextxy(WIDTH / 2 - 100, HEIGHT / 2 + 30, _T("Press any key to exit"));
            _getch();// 等待按键退出
            break;
        }
    }

    // 关闭窗口
    closegraph();
    return 0;
}
