#include <easyx.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

// 游戏常量定义
#define WIDTH 640
#define HEIGHT 480
#define GRID_SIZE 20

// 方向枚举
enum Direction { UP, DOWN, LEFT, RIGHT };

// 蛇身节点结构
struct Point {
    int x;
    int y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

// 游戏类
class SnakeGame {
private:
    std::vector<Point> snake;  // 蛇身
    Point food;                // 食物
    Direction dir;             // 当前方向
    bool gameOver;             // 游戏结束标志
    int score;                 // 分数

public:
    SnakeGame() {
        // 初始化游戏
        initGame();
    }

    // 初始化游戏
    void initGame() {
        // 初始化蛇
        snake.clear();
        snake.push_back(Point(10, 10));
        snake.push_back(Point(10, 11));
        snake.push_back(Point(10, 12));

        // 初始方向
        dir = UP;

        // 生成食物
        generateFood();

        // 初始化游戏状态
        gameOver = false;
        score = 0;
    }

    // 生成食物（补全示例）
    void generateFood() {
        srand(time(NULL));
        bool onSnake;
        do {
            onSnake = false;
            // 随机生成食物位置（确保在网格内）
            food.x = rand() % (WIDTH / GRID_SIZE);
            food.y = rand() % (HEIGHT / GRID_SIZE);
            // 检查是否与蛇身重叠
            for (const auto& p : snake) {
                if (p.x == food.x && p.y == food.y) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);  // 若重叠则重新生成
    }

    // 改变方向（补全示例）
    void changeDirection(Direction newDir) {
        // 禁止180度转向（如当前向上时不能直接向下）
        if ((dir == UP && newDir != DOWN) ||
            (dir == DOWN && newDir != UP) ||
            (dir == LEFT && newDir != RIGHT) ||
            (dir == RIGHT && newDir != LEFT)) {
            dir = newDir;
        }
    }

    // 移动蛇（补全示例）
    void move() {
        // 新蛇头 = 旧蛇头 + 方向偏移
        Point newHead = snake[0];
        switch (dir) {
        case UP: newHead.y--; break;
        case DOWN: newHead.y++; break;
        case LEFT: newHead.x--; break;
        case RIGHT: newHead.x++; break;
        }
        // 将新蛇头加入蛇身
        snake.insert(snake.begin(), newHead);
        // 若吃到食物则加分，否则移除尾部（蛇长度不变）
        if (newHead.x == food.x && newHead.y == food.y) {
            score += 10;
            generateFood();  // 重新生成食物
        }
        else {
            snake.pop_back();
        }
    }

    // 碰撞检测（补全示例）
    bool checkCollision() {
        Point head = snake[0];
        // 检测撞墙
        if (head.x < 0 || head.x >= WIDTH / GRID_SIZE ||
            head.y < 0 || head.y >= HEIGHT / GRID_SIZE) {
            return true;
        }
        // 检测撞自身
        for (size_t i = 1; i < snake.size(); i++) {
            if (head.x == snake[i].x && head.y == snake[i].y) {
                return true;
            }
        }
        return false;
    }

    // 绘制游戏（补全示例）
    void draw() {
        cleardevice();  // 清空屏幕
        // 绘制蛇身
        setfillcolor(GREEN);
        for (const auto& p : snake) {
            fillrectangle(p.x * GRID_SIZE, p.y * GRID_SIZE,
                (p.x + 1) * GRID_SIZE - 1, (p.y + 1) * GRID_SIZE - 1);
        }
        // 绘制食物
        setfillcolor(RED);
        fillcircle((food.x + 0.5) * GRID_SIZE, (food.y + 0.5) * GRID_SIZE, GRID_SIZE / 2);
        // 绘制分数
        settextcolor(BLACK);
        char scoreText[20];
        sprintf(scoreText, "Score: %d", score);
        outtextxy(10, 10, scoreText);
    }

    // 更新游戏状态
    void update() {
        if (!gameOver) {
            move();
            if (checkCollision()) {
                gameOver = true;
            }
        }
    }

    // AI控制
    void aiControl() {
        // 实现AI逻辑，自动控制蛇的移动
        // ...
    }

    // 游戏是否结束
    bool isGameOver() {
        return gameOver;
    }
};

int main() {
    // 创建窗口
    initgraph(WIDTH, HEIGHT);
    setbkcolor(WHITE);
    cleardevice();

    // 创建游戏对象
    SnakeGame game;

    // 游戏主循环
    while (true) {
        // 处理输入
        if (_kbhit()) {
            char key = _getch();
            // 处理键盘输入
        }

        // AI控制
        game.aiControl();

        // 更新游戏状态
        game.update();

        // 绘制游戏
        game.draw();

        // 控制游戏速度
        Sleep(100);

        // 检查游戏结束
        if (game.isGameOver()) {
            // 游戏结束处理
            break;
        }
    }

    // 关闭图形界面
    closegraph();
    return 0;
}