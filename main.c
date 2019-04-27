// Header: Ã∞≥‘…ﬂ
// File Name: main.c
// Author: Kiyotaka233
// Date: 2019/4/27

#define _MAIN_C_
#include <ctype.h>
#include <math.h>
#include <stc15.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C:\Keil_v5\C51\INC\SiLABS\shared\si8051Base\stdbool.h"
#include "zimo.h"

#define RS P15
#define RW P16
#define E P17
#define CS1 P23
#define CS2 P24

#define S2RI 0x01
#define S2TI 0x02

#define PI 3.141592653589793

#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

typedef unsigned char uchar;
typedef unsigned int uint;

char buf[16];
char ram[32];

typedef struct Node {
  int x;
  int y;
  int dis;
} Body;

Body Snake[30];

Body food;

bit stop_c, busy, uart_c, move_c, dir_only;
uchar mode, func;
int level, buf_cnt, max_ms;
uint ms;

void Init() {
  int tmp, i;

  if (func == 0) {
    ClrLCD(0, 7, 0, 127, 0);
    DisLCD(2, 46, "Ã∞≥‘…ﬂ");
    DisLCD(4, 49, "Snake");
    printf("\f");
    dir_only = 0;
    max_ms = 80;
    for (i = 0; i < ARR_SIZE(Snake); i++) Snake[i].dis = 0;
    func++;
  } else if (func == 1) {
    if (uart_c) {
      if (strcmp(buf, "ks") == 0) {
        Snake[0].x = 64;
        Snake[0].y = 32;
        Snake[1].x = 63;
        Snake[1].y = 32;
        Snake[2].x = 62;
        Snake[2].y = 32;
        Snake[0].dis = Snake[1].dis = Snake[2].dis = 1;
        ClrLCD(0, 7, 0, 127, 0);
        mode++;
        func = 0;
      } else if (sscanf(buf, "%d", &tmp)) {
        max_ms = tmp;
        printf("…Ë÷√≥…π¶! \r\n");
        level = tmp;
      }
      uart_c = 0;
    }
  }
}

bool isDirCon(char *p) {
  if (p[0] == 'w' || p[0] == 'a' || p[0] == 's' || p[0] == 'd') {
    return true;
  } else {
    return false;
  }
}

void DrawBody() {
  int i;

  for (i = 0; i < ARR_SIZE(Snake); i++) {
    if (Snake[i].dis) {
      DrawPoint(Snake[i].x, Snake[i].y, 1);
    } else {
      DrawPoint(Snake[i].x, Snake[i].y, 0);
      break;
    }
  }
}

void Start() {
  int i;

  if (func == 0) {
    DrawBody();
    for (i = 0; i < ARR_SIZE(buf); i++) buf[i] = 0;
    func++;
  } else if (func == 1) {
    if (buf[0] != 0) {
      if (isDirCon(&buf) == true) {
        dir_only = 1;
        food.x = rand() % 128;
        food.y = rand() % 64;
        while (ReadPoint(food.x, food.y)) {
          food.x = rand() % 128;
          food.y = rand() % 64;
        }
        DrawPoint(food.x, food.y, 1);
        mode++;
        ms = move_c = func = 0;
      } else
        buf[0] = 0;
      buf_cnt = 0;
    }
  }
}

void Swap(Body in) {
  int i;

  for (i = ARR_SIZE(Snake) - 1; i >= 0; i--) {
    Snake[i + 1].x = Snake[i].x;
    Snake[i + 1].y = Snake[i].y;
    Snake[i + 1].dis = Snake[i].dis;
  }
  Snake[0].x = in.x;
  Snake[0].y = in.y;
  Snake[0].dis = in.dis;
}

int FindTail() {
  int i;

  for (i = 0; i < ARR_SIZE(Snake); i++) {
    if (Snake[i].dis == 0) break;
  }

  return i;
}

void Run() {
  Body New;

  if (move_c) {
    if (buf[0] == 'w') {
      New.x = Snake[0].x;
      New.y = Snake[0].y - 1;
      New.dis = 2;
    } else if (buf[0] == 's') {
      New.x = Snake[0].x;
      New.y = Snake[0].y + 1;
      New.dis = 2;
    } else if (buf[0] == 'a') {
      New.x = Snake[0].x - 1;
      New.y = Snake[0].y;
      New.dis = 1;
    } else if (buf[0] == 'd') {
      New.x = Snake[0].x + 1;
      New.y = Snake[0].y;
      New.dis = 1;
    }
    if (ReadPoint(New.x, New.y)) {
      if (New.x == food.x && New.y == food.y) {
        Swap(New);
        DrawBody();
        food.x = rand() % 128;
        food.y = rand() % 64;
        while (ReadPoint(food.x, food.y)) {
          food.x = rand() % 128;
          food.y = rand() % 64;
        }
        DrawPoint(food.x, food.y, 1);
      } else
        mode = func = 0;
    } else {
      Snake[FindTail() - 1].dis = 0;
      Swap(New);
      DrawBody();
    }
    move_c = ms = 0;
  }
}

void main() {
  InitSystem();
  while (1) {
    if (mode == 0)
      Init();
    else if (mode == 1)
      Start();
    else if (mode == 2)
      Run();
  }
}
