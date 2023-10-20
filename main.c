/**
 * 程序名称：LED摇摇棒显示96*16像素
 * 简要说明：外部中断方式INT0显示；
 * 取模方式：纵向取模、字节倒序
 * Created by Yalda on 2013/02
 */

#include "pov.h"

/**
 * 主函数
 */
void main(void)
{
    IT0 = 1;
    EX0 = 1;
    KY = 0;
    EA = 1; // 开中断，下降沿中断

    P0 = 0xff;
    P2 = 0xff;

    // 主程序中只检测按键
    while (1)
    {
        // 画面切换键按下
        if (KEY == 0)
        {
            DelayUs(10000); // 按键去抖
            if (KEY == 0)
            {
                pic++;
            }
        }
        if (pic > 3)
        {
            pic = 0;
        }
    }
}
