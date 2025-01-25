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

IMAGE bk; // ����ͼƬ
IMAGE airplane[2]; // ��ҷɻ�ͼƬ


// ͼƬ��ʼ��
void loadImage() {
    loadimage(&bk, "./Resource/background.png"); // ����ͼƬ��ʼ��
    loadimage(&airplane[0], "./Resource/me1.png"); //�ɻ�ͼƬ��ʼ��
}

//������ͼ
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

    // �ṹ��ĵ�������Ա��ʾ�����͸���ȣ�0 ��ʾȫ͸����255 ��ʾ��͸����
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

//��Ϸ��ͼ
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
        if (_kbhit()) { // ����������
            char ch = _getch();
            if (ch == 27) { // �� ESC �˳�
                closegraph();
                exit(0);
            }
        }
        Sleep(16); // ����֡��
    }
}

// ��ʾ�˵�
void displayMenu() {
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(30, 0, _T("Arial"));
    outtextxy(190, 300, _T("��ʼ��Ϸ"));
    outtextxy(190, 360, _T("��ʷ���а�"));
    outtextxy(190, 420, _T("�˳���Ϸ"));
}


// ����˵�ѡ��
void handleMenuSelection(int selection) {
    switch (selection) {
    case 1:
        gameStart();
        break;
    case 2:
        printf("�鿴��ʷ���а�\n");
        break;
    case 3:
        printf("�˳���Ϸ\n");
        closegraph();
        exit(0); // ȷ�������˳�
    }
}

// ����˵����
void handleMenu() {
    int selections[3] = { 300, 360, 420 }; // ÿ���˵���� y ����

    BeginBatchDraw(); 
    while (1) {
        cleardevice(); // ����
        backgroundDraw();
        displayMenu(); 
        FlushBatchDraw(); 

        if (MouseHit()) { // �������¼�
            MOUSEMSG msg = GetMouseMsg(); // ��ȡ�����Ϣ
            if (msg.uMsg == WM_LBUTTONDOWN) { // �����������¼�
                for (int i = 0; i < 3; ++i) {
                    if (msg.y >= selections[i] && msg.y <= selections[i] + 40 &&
                        msg.x >= 150 && msg.x <= 330) { // ����x���귶Χ
                        handleMenuSelection(i + 1);
                        return; // �˳� handleMenu()
                    }
                }
            }
        }

        Sleep(16); // ����֡�ʣ���ֹCPU����
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
