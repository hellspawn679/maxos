#ifndef __MAXOS__HARDWARECOMMUNICATION__INTERRUPTMANAGER_H
#define __MAXOS__HARDWARECOMMUNICATION__INTERRUPTMANAGER_H

#include <gdt.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>


namespace maxos
{
    namespace hardwarecommunication
    {
        class InterruptManager;

        class InterruptHandler
        {
        protected:
            maxos::common::uint8_t interruptNumber;
            InterruptManager* interruptManager;
            InterruptHandler(maxos::common::uint8_t interruptNumber,InterruptManager* interruptManager);
            ~InterruptHandler();
        public:
            virtual maxos::common::uint32_t HandleInterrupt(maxos::common::uint32_t esp);

        };
        class InterruptManager
        {
            friend class InterruptHandler;
            InterruptHandler* handlers[256];
            protected:
                static InterruptManager* ActiveInterruptManager; // static ptr to interrupt manager 
                struct GateDescriptor
                {
                    maxos::common::uint16_t handlerAddressLowBits;
                    maxos::common::uint16_t gdt_codeSegmentSelector;
                    maxos::common::uint8_t reserved;
                    maxos::common::uint8_t access;
                    maxos::common::uint16_t handlerAddressHighBits;
                } __attribute__((packed));

                static GateDescriptor interruptDescriptorTable[256];

                struct InterruptDescriptorTablePointer
                {
                    maxos::common::uint16_t size;
                    maxos::common::uint32_t base;
                } __attribute__((packed));

                maxos::common::uint16_t hardwareInterruptOffset;
                //static InterruptManager* ActiveInterruptManager;
                static void SetInterruptDescriptorTableEntry(maxos::common::uint8_t interrupt,
                    maxos::common::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                    maxos::common::uint8_t DescriptorPrivilegeLevel, maxos::common::uint8_t DescriptorType);


                static void InterruptIgnore();

                static void HandleInterruptRequest0x00();
                static void HandleInterruptRequest0x01();
                static void HandleInterruptRequest0x02();
                static void HandleInterruptRequest0x03();
                static void HandleInterruptRequest0x04();
                static void HandleInterruptRequest0x05();
                static void HandleInterruptRequest0x06();
                static void HandleInterruptRequest0x07();
                static void HandleInterruptRequest0x08();
                static void HandleInterruptRequest0x09();
                static void HandleInterruptRequest0x0A();
                static void HandleInterruptRequest0x0B();
                static void HandleInterruptRequest0x0C();
                static void HandleInterruptRequest0x0D();
                static void HandleInterruptRequest0x0E();
                static void HandleInterruptRequest0x0F();
                static void HandleInterruptRequest0x31();

                static void HandleException0x00();
                static void HandleException0x01();
                static void HandleException0x02();
                static void HandleException0x03();
                static void HandleException0x04();
                static void HandleException0x05();
                static void HandleException0x06();
                static void HandleException0x07();
                static void HandleException0x08();
                static void HandleException0x09();
                static void HandleException0x0A();
                static void HandleException0x0B();
                static void HandleException0x0C();
                static void HandleException0x0D();
                static void HandleException0x0E();
                static void HandleException0x0F();
                static void HandleException0x10();
                static void HandleException0x11();
                static void HandleException0x12();
                static void HandleException0x13();

                static maxos::common::uint32_t HandleInterrupt(maxos::common::uint8_t interrupt, maxos::common::uint32_t esp);
                maxos::common::uint32_t DoHandlerInterrupt(maxos::common::uint8_t interrupt, maxos::common::uint32_t esp);

                Port8BitSlow programmableInterruptControllerMasterCommandPort;
                Port8BitSlow programmableInterruptControllerMasterDataPort;
                Port8BitSlow programmableInterruptControllerSlaveCommandPort;
                Port8BitSlow programmableInterruptControllerSlaveDataPort;

            public:
                InterruptManager(maxos::common::uint16_t hardwareInterruptOffset, maxos::GlobalDescriptorTable* globalDescriptorTable);
                ~InterruptManager();
                maxos::common::uint16_t HardwareInterruptOffset();
                void Activate();
                void Deactivate();
        };
    }
}
#endif