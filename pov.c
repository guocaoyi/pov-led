/**
程序名称：LED摇摇棒显示96*16像素
简要说明：外部中断方式INT0显示；
取模方式：纵向取模、字节倒序
 Created by Yalda on 2013/02
*/

#include "pov.h"
#include <AT89X52.h>
#define uchar unsigned char
#define uint unsigned int

// 定义画面切换按键
#define KEY P3_0
// KY作用在后面说明
uchar KY;
// 显示汉字指针
uchar disp;
// pic为按键次数；num为中断次数
uchar pic=0,num=0;

uchar capricorn[]={
    // spacer
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    // capricorn icon
    0x18,0x00,0x08,0x00,0xF8,0x3F,0xF0,0x1F,
    0x00,0x0C,0x00,0x06,0x00,0x03,0x80,0x31,
    0xC0,0x60,0x40,0x40,0xFC,0x40,0xE6,0x60,
    0xE2,0x73,0x62,0x3E,0x7E,0x18,0x3C,0x00,

    // spacer
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

uchar hzqian[]={
    // spacer
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    // character '前'
    0x08,0x00,0x08,0x00,0xE8,0x7F,0xA8,0x04,
    0xA9,0x24,0xAE,0x44,0xEA,0x3F,0x08,0x00,
    0x08,0x00,0xC8,0x1F,0x0C,0x40,0x0B,0x80,
    0xEA,0x7F,0x08,0x00,0x08,0x00,0x00,0x00,

    // character '程'
    0x10,0x04,0x12,0x03,0xD2,0x00,0xFE,0xFF,
    0x91,0x00,0x11,0x41,0x80,0x44,0xBF,0x44,
    0xA1,0x44,0xA1,0x7F,0xA1,0x44,0xA1,0x44,
    0xBF,0x44,0x80,0x44,0x00,0x40,0x00,0x00,

    // character '似'
    0x80,0x00,0x40,0x00,0xF8,0x7F,0x17,0x00,
    0x02,0x00,0xF8,0x1F,0x00,0x48,0x01,0x44,
    0x0E,0x22,0x04,0x21,0x00,0x10,0x00,0x0C,
    0xFF,0x03,0x00,0x0C,0x00,0x70,0x00,0x00,

    // character '锦'
    0x40,0x01,0x30,0x01,0x2F,0x01,0xE4,0x7F,
    0x24,0x21,0x24,0x11,0x00,0x00,0x7C,0x3F,
    0x54,0x01,0x56,0x01,0xD5,0xFF,0x54,0x01,
    0x54,0x11,0x7C,0x21,0x00,0x1F,0x00,0x00,

    // spacer
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

uchar hztian[]={
    // spacer
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    // character '天'
    0x00,0x00,0x40,0x80,0x42,0x40,0x42,0x20,
    0x42,0x10,0x42,0x08,0x42,0x06,0xFE,0x01,
    0x42,0x02,0x42,0x04,0x42,0x08,0x42,0x10,
    0x42,0x30,0x42,0x60,0x40,0x20,0x00,0x00,

    // character '天'
    0x00,0x00,0x40,0x80,0x42,0x40,0x42,0x20,
    0x42,0x10,0x42,0x08,0x42,0x06,0xFE,0x01,
    0x42,0x02,0x42,0x04,0x42,0x08,0x42,0x10,
    0x42,0x30,0x42,0x60,0x40,0x20,0x00,0x00,

    // character '开'
    0x40,0x00,0x42,0x40,0x42,0x20,0x42,0x10,
    0x42,0x0C,0xFE,0x03,0x42,0x00,0x42,0x00,
    0x42,0x00,0x42,0x00,0xFE,0x7F,0x42,0x00,
    0x42,0x00,0x42,0x00,0x42,0x00,0x00,0x00,

    // character '心'
    0x00,0x04,0x80,0x03,0x00,0x00,0x00,0x00,
    0xF0,0x3F,0x00,0x40,0x01,0x40,0x02,0x40,
    0x1C,0x40,0x08,0x40,0x00,0x40,0x40,0x78,
    0x80,0x00,0x00,0x07,0x00,0x02,0x00,0x00,

    // spacer
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/**
 battery icon
 */
uchar dianchi[]={
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xFE,0x7F,0x03,0xC0,0xF9,0x9F,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,
    0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
    0x03,0xC0,0xFE,0x7F,0x00,0x00,0xF0,0x0F,
    0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x81,
    0x80,0x61,0xC0,0x39,0xE0,0x1F,0xF0,0x0F,
    0xF8,0x07,0x9C,0x03,0x86,0x01,0x81,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


void display1(void);
void display2(void);
void display3(void);
void display4(void);

/**
 延时子程序
 */
void DelayUs(uint N)
{
    uint x;
    for(x=0;x<=N;x++);
}

void intersvr0(void) interrupt 0 using 1    //外部中断0服务程序
{
    KY=~KY;
    // Each swinging mercury switch produces a falling edge break at each end of the swing, extracting only one of them
    // i.e. swinging from left to right to display
    if(KY==0)
    {
        num++; // count interrupt times
        switch(pic) // choose screen
        {
            case 0:{display1();}break;
            case 1:{display2();}break;
            case 2:{display3();}break;
            case 3:{display4();}break;
            default:{display1();}
        }
    }
}


/**
 show screen - 1
 */
void display1(void)
{
 uchar i;
 DelayUs(4000);
 for(i=0;i<64;i++)
 {
     P0=~capricorn[i*2];
     P2=~capricorn[i*2+1];
     DelayUs(120);
 }
}

/**
 show screen - 2
 */
void display2(void)
{
 uchar i;
 DelayUs(4000);
 for(i=0;i<64;i++)
 {
     P0=~hzqian[i*2];
     P2=~hzqian[i*2+1];
     DelayUs(120);
 }
}

/**
 show screen - 3
 */
void display3(void)
{
 uchar i;
 DelayUs(4000);
 for(i=0;i<64;i++)
 {
     P0=~hztian[i*2];
     P2=~hztian[i*2+1];
     DelayUs(120);
 }
}
                   
/**
 show screen - 4
 */
void display4(void)
{
 uchar i;
 DelayUs(4000);
 for(i=0;i<64;i++)
 {
     P0=~dianchi[i*2];
     P2=~dianchi[i*2+1];
     DelayUs(120);
 }
}


/**
 主函数
 */
void main(void)
{
    IT0=1;
    EX0=1;
    KY=0;
    EA=1;// 开中断，下降沿中断

    P0=0xff;
    P2=0xff;
    
    // 主程序中只检测按键
    while(1)
    {
        // 画面切换键按下
        if(KEY==0)
        {
            DelayUs(10000); // 按键去抖
            if(KEY==0);
            pic++;
        }
        if(pic>3)pic=0;
    }
}
