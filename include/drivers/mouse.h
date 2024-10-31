#ifndef __MAXOS__DRIVERS__MOUSE_H
#define __MAXOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>
namespace maxos
{
    namespace drivers
    {
        class MouseEventHandler
        {
            
        public:
            MouseEventHandler();
            ~MouseEventHandler();

            virtual void OnActivate(); 
            virtual void OnMouseDown(maxos::common::uint8_t button);
            virtual void OnMouseUp(maxos::common::uint8_t button);
            virtual void OnMouseMove(int x,int y); 
        };
        class MouseDriver : public maxos::hardwarecommunication::InterruptHandler , public Driver
        {
            maxos::hardwarecommunication::Port8Bit dataport;
            maxos::hardwarecommunication::Port8Bit commandport;

            maxos::common::uint8_t buffer[3];
            maxos::common::uint8_t offset;
            maxos::common::uint8_t buttons;

            MouseEventHandler* handler;
        
        public:
            MouseDriver(maxos::hardwarecommunication::InterruptManager* manager,MouseEventHandler* handler);
            ~MouseDriver();
            virtual maxos::common::uint32_t HandleInterrupt(maxos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif