#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <graphics.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#pragma comment(lib, "MSIMG32.LIB")
#pragma region definitions

IMAGE bk; // 背景图片
IMAGE airplane[2]; // 玩家飞机图片


// 图片初始化
void loadImage() {
    loadimage(&bk, "./Resource/background.png"); // 背景图片初始化
    loadimage(&airplane[0], "./Resource/me1.png"); //飞机图片初始化
}

//贴背景图
void backgroundDraw() {
    loadImage();
    putimage(0, 0, &bk);
    FlushBatchDraw(); 
}

void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();

    // 结构体的第三个成员表示额外的透明度，0 表示全透明，255 表示不透明。
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

//游戏贴图
void gameDraw() {
    loadImage();
    transparentimage(NULL,190,700 , &airplane[0]);
    FlushBatchDraw();
}

void gameStart() {
    initgraph(480, 850);
    while (1) {
        backgroundDraw();
        gameDraw();
        if (_kbhit()) { // 检测键盘输入
            char ch = _getch();
            if (ch == 27) { // 按 ESC 退出
                closegraph();
                exit(0);
            }
        }
        Sleep(16); // 限制帧率
    }
}

// 显示菜单
void displayMenu() {
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(30, 0, _T("Arial"));
    outtextxy(190, 300, _T("开始游戏"));
    outtextxy(190, 360, _T("历史排行榜"));
    outtextxy(190, 420, _T("退出游戏"));
}


// 处理菜单选择
void handleMenuSelection(int selection) {
    switch (selection) {
    case 1:
        gameStart();
        break;
    case 2:
        printf("查看历史排行榜\n");
        break;
    case 3:
        printf("退出游戏\n");
        closegraph();
        exit(0); // 确保程序退出
    }
}

// 处理菜单点击
void handleMenu() {
    int selections[3] = { 300, 360, 420 }; // 每个菜单项的 y 坐标

    BeginBatchDraw(); 
    while (1) {
        cleardevice(); // 清屏
        backgroundDraw();
        displayMenu(); 
        FlushBatchDraw(); 

        if (MouseHit()) { // 检测鼠标事件
            MOUSEMSG msg = GetMouseMsg(); // 获取鼠标消息
            if (msg.uMsg == WM_LBUTTONDOWN) { // 检测左键按下事件
                for (int i = 0; i < 3; ++i) {
                    if (msg.y >= selections[i] && msg.y <= selections[i] + 40 &&
                        msg.x >= 150 && msg.x <= 330) { // 修正x坐标范围
                        handleMenuSelection(i + 1);
                        return; // 退出 handleMenu()
                    }
                }
            }
        }

        Sleep(16); // 限制帧率，防止CPU过载
    }
    EndBatchDraw();
}

int main() {
    initgraph(480, 850);
    backgroundDraw();
    handleMenu();
    closegraph();
    return 0;
}
