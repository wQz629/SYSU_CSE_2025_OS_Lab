#include "interrupt.h"
#include "os_type.h"
#include "os_constant.h"
#include "asm_utils.h"
#include "stdio.h"

extern STDIO stdio;

int times = 0;

InterruptManager::InterruptManager()
{
    initialize();
}

void InterruptManager::initialize()
{
    // 初始化中断计数变量
    times = 0;
    
    // 初始化IDT
    IDT = (uint32 *)IDT_START_ADDRESS;
    asm_lidt(IDT_START_ADDRESS, 256 * 8 - 1);

    for (uint i = 0; i < 256; ++i)
    {
        setInterruptDescriptor(i, (uint32)asm_unhandled_interrupt, 0);
    }

    // 初始化8259A芯片
    initialize8259A();
}

void InterruptManager::setInterruptDescriptor(uint32 index, uint32 address, byte DPL)
{
    IDT[index * 2] = (CODE_SELECTOR << 16) | (address & 0xffff);
    IDT[index * 2 + 1] = (address & 0xffff0000) | (0x1 << 15) | (DPL << 13) | (0xe << 8);
}

void InterruptManager::initialize8259A()
{
    // ICW 1
    asm_out_port(0x20, 0x11);
    asm_out_port(0xa0, 0x11);
    // ICW 2
    IRQ0_8259A_MASTER = 0x20;
    IRQ0_8259A_SLAVE = 0x28;
    asm_out_port(0x21, IRQ0_8259A_MASTER);
    asm_out_port(0xa1, IRQ0_8259A_SLAVE);
    // ICW 3
    asm_out_port(0x21, 4);
    asm_out_port(0xa1, 2);
    // ICW 4
    asm_out_port(0x21, 1);
    asm_out_port(0xa1, 1);

    // OCW 1 屏蔽主片所有中断，但主片的IRQ2需要开启
    asm_out_port(0x21, 0xfb);
    // OCW 1 屏蔽从片所有中断
    asm_out_port(0xa1, 0xff);
}

void InterruptManager::enableTimeInterrupt()
{
    uint8 value;
    // 读入主片OCW
    asm_in_port(0x21, &value);
    // 开启主片时钟中断，置0开启
    value = value & 0xfe;
    asm_out_port(0x21, value);
}

void InterruptManager::disableTimeInterrupt()
{
    uint8 value;
    asm_in_port(0x21, &value);
    // 关闭时钟中断，置1关闭
    value = value | 0x01;
    asm_out_port(0x21, value);
}

void InterruptManager::setTimeInterrupt(void *handler)
{
    setInterruptDescriptor(IRQ0_8259A_MASTER, (uint32)handler, 0);
}

// 中断处理函数
extern "C" void c_time_interrupt_handler()
{
    // stdio.number 是用来记录 每次输出的数字
    // stdio.color 是用来记录 每次输出的字符的颜色
    //  position  是用来记录当前光标位置的变量
    uint position = stdio.getCursor();
    
    // 每次调用时钟中断处理函数就输出一个字符
    stdio.print(stdio.number + '0',stdio.color);
    
    // 修改输出字符的 背景色 和 前景色
    stdio.color += 1;
    stdio.color %= 0xff;
    
    // 修改下一次要输出的字符
    stdio.change_char();
    
    // 修改下一次输出字符的光标位置
    // 如果要在上边缘输出字符
    if(position >= 0 && position < 79)
    {
    	stdio.moveCursor(position + 1);
    }
    // 如果要在右边缘输出字符
    else if((position + 1) % 80 == 0 && position != 1999)
    {
    	stdio.moveCursor(position + 80);
    }
    // 如果要在下边缘输出字符
    else if(position > 1920 && position <= 1999)
    {
    	stdio.moveCursor(position - 1);
    }
    // 如果要在左边缘输出字符
    else if(position % 80 == 0 && position != 0)
    {
    	stdio.moveCursor(position - 80);
    }
}
