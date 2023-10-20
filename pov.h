#ifndef POV_H
#define POV_H

#define uchar unsigned char
#define uint unsigned int

// 定义画面切换按键
#define KEY P3_0

extern uchar KY;
extern uchar disp;
extern uchar pic;
extern uchar num;

extern uchar capricorn[];
extern uchar hzqian[];
extern uchar hztian[];
extern uchar dianchi[];

void display1(void);
void display2(void);
void display3(void);
void display4(void);

void DelayUs(uint N);
void intersvr0(void);

#endif
