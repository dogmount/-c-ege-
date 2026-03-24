#include <iostream>
using namespace std;
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

//------预定义
// 定义场景大小
#define WIN_WIDTH 1040
#define WIN_HEIGHT 640
// 定义食物以及蛇的正方形边长
#define SIZE 20
// 定义蛇的朝向
#define UP VK_UP
#define DOWN VK_DOWN
#define RIGHT VK_RIGHT
#define LEFT VK_LEFT
// 蛇身最大长度
#define MAXLEN 1600

typedef struct
{
    int x;
    int y;
} SnakeNode;

SnakeNode tmp[MAXLEN]; // 用另一个数组来存储蛇原本的位置
int speed = 150;       // 蛇的速度（用在delay_ms函数里）

// 前向声明---
// 先声明食物类，避免Snake类中使用Food类的函数编译不通过
class Food;

// 创建蛇的类
class Snake
{
    friend class Food;

public:
    Snake();             // 初始化
    void Move();         // 移动
    void Draw();         // 绘制蛇
    bool Eat(Food food); // 吃食物
    bool Defeat();       // 失败判定
private:
    int dirt;               // 朝向
    int length;             // 长度
    SnakeNode node[MAXLEN]; // 蛇的结点
};

// 创建食物的类
class Food
{
    friend class Snake;

public:
    int score;         // 分数
    Food(Snake snake); // 食物初始化
    void Draw();       // 绘制食物
private:
    int x, y; // 坐标
};

int main()
{
    // 记录分数
    int score = 0;

    /*
    如果是动画的话，需要设置为手动渲染模式， 这个设置可以减少绘图时的闪烁。
    设置方式为在 initgraph() 中的第三个参数加上 INIT_RENDERMANUAL，如：
    initgraph(640, 480, INIT_RENDERMANUAL)
    */

    // 初始化窗口,手动渲染模式
    initgraph(WIN_WIDTH, WIN_HEIGHT, INIT_RENDERMANUAL);

    Snake snake;
    // cleardevice(); // 清屏
table:
    Food food(snake);
    while (1)
    {
        snake.Draw();
        food.Draw();
        // 控制帧率为60FPS
        if (snake.Eat(food))
        {
            score += food.score;
            goto table;
            continue;
        }
        if (snake.Defeat())
        {
            break;
        }
        snake.Move();
        delay_fps(60);
    }

    // 提示失败信息
    // HWND window = GetHWnd();

    // 使用 std::wstring 替代 std::string‌
    // std::wstring 直接提供宽字符支持，c_str() 返回 LPCWSTR 兼容的指针
    wstring text = L"游戏结束，玩家得分为： " + to_wstring(score);
    // MessageBoxW(NULL, L"游戏结束", L"提示", MB_OK | MB_SYSTEMMODAL);
    MessageBoxW(NULL, text.c_str(), L"提示", MB_OK | MB_SYSTEMMODAL);
    return 0;
}

// 蛇的初始化
Snake::Snake()
{
    this->dirt = RIGHT; // 初始化蛇的方向为向右
    this->length = 3;   // 初始化蛇的长度为2

    // 设置节点结构体坐标值---即x,y轴坐标
    // 下标0为蛇的头部
    for (int i = 0; i < length; i++)
    {
        // this->node[i].x = 40 - ((i + 1) * SIZE);
        // this->node[i].y = 0;
        // 设置出生点在中间
        this->node[i].x = 560 - ((i + 1) * SIZE);
        this->node[i].y = 14 * SIZE;
    }
}

void Snake::Move()
{
    // 将原来的蛇身节点拷贝一份
    for (int i = 0; i < this->length; i++)
    {
        tmp[i].x = this->node[i].x;
        tmp[i].y = this->node[i].y;
    }

    int status = 0; // 用来判断是否点击了转向按键
    // 如果当前方向向右
    if (this->dirt == RIGHT)
    {
        // 判断是否转向
        if (GetAsyncKeyState(VK_UP) && status == 0)
        {
            this->dirt = UP;
            status = 1;
        }
        else if (GetAsyncKeyState(VK_DOWN) && status == 0)
        {
            this->dirt = DOWN;
            status = 1;
        }
        else
        {
            this->node[0].x += SIZE;
        }
    }

    // 如果当前方向向左
    if (this->dirt == LEFT)
    {
        // 判断是否转向
        if (GetAsyncKeyState(VK_UP) && status == 0)
        {
            this->dirt = UP;
            status = 1;
        }
        else if (GetAsyncKeyState(VK_DOWN) && status == 0)
        {
            this->dirt = DOWN;
            status = 1;
        }
        else
        {
            this->node[0].x -= SIZE;
        }
    }

    // 如果当前方向向上
    // 由于上方已经修改方向，在循环结束前先改变
    if (this->dirt == UP)
    {
        // 判断是否转向
        if (GetAsyncKeyState(VK_RIGHT) && status == 0)
        {
            this->dirt = RIGHT;
            this->node[0].x += SIZE;
            status = 1;
        }
        else if (GetAsyncKeyState(VK_LEFT) && status == 0)
        {
            this->dirt = LEFT;
            this->node[0].x -= SIZE;
            status = 1;
        }
        else
        {
            this->node[0].y -= SIZE;
        }
    }

    // 如果当前方向向下
    if (this->dirt == DOWN)
    {
        // 判断是否转向
        if (GetAsyncKeyState(VK_RIGHT) && status == 0)
        {
            this->dirt = RIGHT;
            this->node[0].x += SIZE;
            status = 1;
        }
        else if (GetAsyncKeyState(VK_LEFT) && status == 0)
        {
            this->dirt = LEFT;
            this->node[0].x -= SIZE;
            status = 1;
        }
        else
        {
            this->node[0].y += SIZE;
        }
    }

    // 移动蛇身节点
    for (int i = 1; i < this->length; i++)
    {
        this->node[i].x = tmp[i - 1].x;
        this->node[i].y = tmp[i - 1].y;
    }
    Sleep(speed);
}

// 绘制蛇身
void Snake::Draw()
{
    cleardevice(); // 清空原先的绘图
    // srand((unsigned)time(NULL)); // 设置随机数种子
    setfillcolor(RGB(201, 53, 67));
    fillrect(this->node[0].x, this->node[0].y, this->node[0].x + SIZE, this->node[0].y + SIZE);
    for (int i = 1; i < this->length; i++)
    {
        setfillcolor(RGB(177, 229, 170));
        fillrect(this->node[i].x, this->node[i].y, this->node[i].x + SIZE, this->node[i].y + SIZE);
    }
}

// 吃食物---尾部增长
bool Snake::Eat(Food food)
{
    // 将原来的蛇身节点拷贝一份
    int tmp_x = this->node[length - 1].x;
    int tmp_y = this->node[length - 1].y;

    if (food.x == this->node[0].x && food.y == this->node[0].y)
    {
        // if (this->node[length - 1].x - this->node[length - 2].x == 0 && this->node[length - 1].y - this->node[length - 2].y == -20)
        // {
        //     this->length++;
        //     this->node[length - 1].x = this->node[length - 2].x;
        //     this->node[length - 1].y = this->node[length - 2].y - SIZE;
        // }
        // if (this->node[length - 1].x - this->node[length - 2].x == 0 && this->node[length - 1].y - this->node[length - 2].y == 20)
        // {
        //     this->length++;
        //     this->node[length - 1].x = this->node[length - 2].x;
        //     this->node[length - 1].y = this->node[length - 2].y + SIZE;
        // }
        // if (this->node[length - 1].x - this->node[length - 2].x == 20 && this->node[length - 1].y - this->node[length - 2].y == 0)
        // {
        //     this->length++;
        //     this->node[length - 1].x = this->node[length - 2].x + SIZE;
        //     this->node[length - 1].y = this->node[length - 2].y;
        // }
        // if (this->node[length - 1].x - this->node[length - 2].x == -20 && this->node[length - 1].y - this->node[length - 2].y == 0)
        // {
        //     this->length++;
        //     this->node[length - 1].x = this->node[length - 2].x - SIZE;
        //     this->node[length - 1].y = this->node[length - 2].y;
        // }

        if (this->node[length - 1].x - this->node[length - 2].x == 0 && this->node[length - 1].y - this->node[length - 2].y == -20)
        {
            this->length++;
            tmp_x = this->node[length - 2].x;
            tmp_y = this->node[length - 2].y - SIZE;
        }
        if (this->node[length - 1].x - this->node[length - 2].x == 0 && this->node[length - 1].y - this->node[length - 2].y == 20)
        {
            this->length++;
            tmp_x = this->node[length - 2].x;
            tmp_y = this->node[length - 2].y + SIZE;
        }
        if (this->node[length - 1].x - this->node[length - 2].x == 20 && this->node[length - 1].y - this->node[length - 2].y == 0)
        {
            this->length++;
            tmp_x = this->node[length - 2].x + SIZE;
            tmp_y = this->node[length - 2].y;
        }
        if (this->node[length - 1].x - this->node[length - 2].x == -20 && this->node[length - 1].y - this->node[length - 2].y == 0)
        {
            this->length++;
            tmp_x = this->node[length - 2].x - SIZE;
            tmp_y = this->node[length - 2].y;
        }

        // 移动蛇身节点
        this->node[length - 1].x = tmp_x;
        this->node[length - 1].y = tmp_y;
        return true;
    }
    return false;
}

// 失败判定
bool Snake::Defeat()
{
    // 1.碰到边界
    if (this->node[0].x < 0 || this->node[0].x >= WIN_WIDTH || this->node[0].y < 0 || this->node[0].y >= WIN_HEIGHT)
    {
        return true;
    }
    // 2.碰到自己的身体
    for (int i = 1; i < this->length; i++)
    {
        if (this->node[0].x == this->node[i].x && this->node[0].y == this->node[i].y)
        {
            return true;
        }
    }
    return false;
}

// 食物类初始化
Food::Food(Snake snake)
{
    // bool flag = true;            // 判断食物点位是否和蛇身重叠 重叠为true
    srand((unsigned)time(NULL)); // 设置随机数种子
    this->x = (rand() % (WIN_WIDTH / SIZE)) * SIZE;
    this->y = (rand() % (WIN_HEIGHT / SIZE)) * SIZE;
    this->score = rand() % 10 + 1;

    // 循环判断
    // 设置变量i来逐个判断蛇身节点
    int i = 0;
    while (i < snake.length)
    // for (int i = 0; i < snake.length; i++)
    {
        if (snake.node[i].x == this->x && snake.node[i].y == this->y)
        {
            this->x = (rand() % (WIN_WIDTH / SIZE)) * SIZE;
            this->y = (rand() % (WIN_HEIGHT / SIZE)) * SIZE;
            this->score = rand() % 10 + 1;
            i = 0;
            continue;
        }
        i++;
    }

    // table:
    //     srand((unsigned)time(NULL));
    //     this->x = (rand() % (WIN_WIDTH / SIZE)) * SIZE;
    //     this->y = (rand() % (WIN_HEIGHT / SIZE)) * SIZE;
    //     this->score = rand() % 10 + 1;
    //     for (int i = 0; i < snake.length; i++)
    //     {
    //         if (snake.node[i].x == this->x && snake.node[i].y == this->y)
    //         {
    //             goto table;
    //         }
    //     }
}

// 绘制食物
void Food::Draw()
{
    setfillcolor(GREEN);
    fillrect(this->x, this->y, this->x + SIZE, this->y + SIZE);
}
