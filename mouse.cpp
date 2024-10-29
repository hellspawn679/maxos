#include "mouse.h"




MouseEventHandler::MouseEventHandler()
{

}
void MouseEventHandler::OnActivate()
{

}
void MouseEventHandler::OnMouseDown(uint8_t button)
{

}
void MouseEventHandler::OnMouseUp(uint8_t button)
{

}
void MouseEventHandler::OnMouseMove(int x,int y)
{

}




MouseDriver::MouseDriver(InterruptManager* manager,MouseEventHandler* handler) : InterruptHandler(0x2C,manager),
dataport(0x60),
commandport(0x64)
{

   this->handler=handler;
}


MouseDriver::~MouseDriver()
{

}

void printf(char*);

void MouseDriver::Activate()
{
    offset=0;
    buttons=0;

   
    //read from here https://wiki.osdev.org/Interrupts
    commandport.Write(0xA8); // activate interrupts for mouse
    commandport.Write(0x20); // get current state
    uint8_t status = dataport.Read() | 2 ; //new state and 2th bit to true
    commandport.Write(0x60); // set state 
    dataport.Write(status);

    commandport.Write(0xD4); // write to second PS/2 port
    dataport.Write(0xF4); // activate mouse
    dataport.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandport.Read();
    //if the 6th bit is 1 one then only we can read the data
    if((!(status & 0x20)) || handler == 0 )
        return esp;
    
    //x=40,y=12;
    buffer[offset] =dataport.Read();
    offset = (offset+1)%3;
    
    if(offset==0)
    {
        //buffer[1] is movement in x 
        //buffer[2] is movement in y
        if(buffer[1]!=0 || buffer[2]!=0)
       { 
            handler->OnMouseMove(buffer[1],-buffer[2]);
           
        }

        for(uint8_t i=0;i<3;i++)
        {
            if((buffer[0])& (0x01 <<i)!= (buttons & (0x01 <<i)))
            {
                if(buttons & (0x1<<i)){
                    handler->OnMouseUp(i+1);
                }
                else 
                    handler->OnMouseDown(i+1);
                
            }
        }
        buttons =buffer[0];

    }
    return esp;
}