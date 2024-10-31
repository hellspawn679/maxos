#ifndef __MAXOS__DRIVERS__KEYBOARD_H
#define __MAXOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace maxos
{
    namespace drivers
    {
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();
            ~KeyboardEventHandler();
            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public maxos::hardwarecommunication::InterruptHandler , public Driver
        {
            maxos::hardwarecommunication::Port8Bit dataport;
            maxos::hardwarecommunication::Port8Bit commandport;

            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(maxos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
            ~KeyboardDriver();
            virtual maxos::common::uint32_t HandleInterrupt(maxos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif