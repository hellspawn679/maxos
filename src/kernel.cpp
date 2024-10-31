

#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace maxos;
using namespace maxos::common;
using namespace maxos::drivers;
using namespace maxos::hardwarecommunication;

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;
    
    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }
        
        if(x >= 80)
        {
            x = 0;
            y++;
        }
        
        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[11] = hex[(key >> 4) & 0xF];
    foo[12] = hex[key & 0xF];
    printf(foo);
}


class PrintfKeyBoardEventHandler : public KeyboardEventHandler
{
    public: 
        void OnKeyDown( char c)
        {
            char* foo =" ";
            foo[0]=c;
            printf(foo);
        }
};


class MouseToConsole : public MouseEventHandler
{
    int8_t x,y;

public:
    MouseToConsole()
    {
        x=40;
        y=12;
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x]= ((VideoMemory[80*y+x] & 0xF000) >> 4)|
                                ((VideoMemory[80*y+x] & 0x0F00) << 4)|
                                ((VideoMemory[80*y+x] & 0x00FF));
    }
    void OnMouseMove(int xoffset,int yoffset)
    {
         static uint16_t* VideoMemory = (uint16_t*)0xb8000;

            VideoMemory[80*y+x]= ((VideoMemory[80*y+x] & 0xF000) >> 4)|
                                ((VideoMemory[80*y+x] & 0x0F00) << 4)|
                                ((VideoMemory[80*y+x] & 0x00FF));

            x+=xoffset;
            if(x<0) x=0;
            if(x>=80) x=79;
            y+=yoffset;
            if (y<0) y=0;
            if(y>=25) y=24;

            //VideoMemory[80*y+x]= (VideoMemory[80*y+x] & 0xF000) | ((VideoMemory[80*y+x] + 1) & 0x0FFF);
            VideoMemory[80*y+x]= ((VideoMemory[80*y+x] & 0xF000) >> 4)|
                                ((VideoMemory[80*y+x] & 0x0F00) << 4)|
                                ((VideoMemory[80*y+x] & 0x00FF));
    }
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Welcome to BREAK OS this os made from ground up to learn how os works \nThis is made by  mehul gautam student of IIIT Gwalior \n");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    printf("Initializing Hardware, Stage 1\n");

    DriverManager drvManager;
        PrintfKeyBoardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
        drvManager.AddDriver(&keyboard);
        

        MouseToConsole mousehandler;
        MouseDriver mouse(&interrupts, &mousehandler);
        drvManager.AddDriver(&mouse);


    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();
    printf("Initializing Hardware, Stage 3\n");
    interrupts.Activate();
    while(1);
}