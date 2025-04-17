#ifndef STDIO_H
#define STDIO_H

#include "os_type.h"

class STDIO
{
private:
    uint8 *screen;

public:

    // 记录每次输出的字符
    uint number;
    // 记录每次输出的字符的颜色
    uint color;
    
    
    STDIO();
    // 初始化函数
    void initialize();
    // 打印字符c，颜色color到位置(x,y)
    void print(uint x, uint y, uint8 c, uint8 color);
    // 打印字符c，颜色color到光标位置
    void print(uint8 c, uint8 color);
    // 打印字符c，颜色默认到光标位置
    void print(uint8 c);
    // 移动光标到一维位置
    void moveCursor(uint position);
    // 移动光标到二维位置
    void moveCursor(uint x, uint y);
    // 获取光标位置
    uint getCursor();
    // 清空屏幕
    void clear_screen(); // 这个函数是我自己加的
    // 修改每次输出的字符
    void change_char(); // 这个函数是我自己加的
    
private:
    // 滚屏
    void rollUp();
};

#endif
