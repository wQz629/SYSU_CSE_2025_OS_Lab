#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;


extern "C" void setup_kernel()
{
    // 中断处理部件
    interruptManager.initialize();
    // 屏幕IO处理部件
    stdio.initialize();
    
    // 清空屏幕
    stdio.clear_screen(); // 这个是我在STDIO类中新加的 用于清空屏幕的函数
    
    // 先在屏幕中央输出 姓名 + 学号，这里没有使用中断来实现，而是单纯调用函数来实现
    char information[] = "YangPeikai 23336279";
    stdio.moveCursor(12 * 80 + 32);  // 把光标移动到屏幕中央
    for(int y = 0;y < 19;y++)
    {
    	stdio.print(information[y], 0x70);
    	stdio.moveCursor(12 * 80 + 32 + y + 1);  // 把光标向前移动一格
    } 
    stdio.moveCursor(0);  // 把光标移回到（0,0）
    
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler); // 设置 处理时钟中断 的中断向量号
    asm_enable_interrupt(); // 开中断，允许对下一次中断产生响应
    interruptManager.enableTimeInterrupt();  // 开启8259A主片时钟中断，能够捕捉8253芯片每个一段时间产生的时钟中断
                                             // 时钟中断不需要手动产生
    					     // 操作系统在检测到有时钟中断时，会 自动触发 时钟中断处理函数
    
    asm_halt();
}

