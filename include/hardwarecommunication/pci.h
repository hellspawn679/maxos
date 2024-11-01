
#ifndef __MAXOS__HARDWARECOMMUNICATION__PCI_H
#define __MAXOS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>

namespace maxos
{
    namespace hardwarecommunication
    {

        class PeripheralComponentInterconnectDeviceDescriptor
        {
        public:
            maxos::common::uint32_t portBase;
            maxos::common::uint32_t interrupt;
            
            maxos::common::uint16_t bus;
            maxos::common::uint16_t device;
            maxos::common::uint16_t function;

            maxos::common::uint16_t vendor_id;
            maxos::common::uint16_t device_id;
            
            maxos::common::uint8_t class_id;
            maxos::common::uint8_t subclass_id;
            maxos::common::uint8_t interface_id;

            maxos::common::uint8_t revision;
            
            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
            
        };


        class PeripheralComponentInterconnectController
        {
            Port32Bit dataPort;
            Port32Bit commandPort;
            
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();
            
            maxos::common::uint32_t Read(maxos::common::uint16_t bus, maxos::common::uint16_t device, maxos::common::uint16_t function, maxos::common::uint32_t registeroffset);
            void Write(maxos::common::uint16_t bus, maxos::common::uint16_t device, maxos::common::uint16_t function, maxos::common::uint32_t registeroffset, maxos::common::uint32_t value);
            bool DeviceHasFunctions(maxos::common::uint16_t bus, maxos::common::uint16_t device);
            
            void SelectDrivers(maxos::drivers::DriverManager* driverManager);
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(maxos::common::uint16_t bus, maxos::common::uint16_t device, maxos::common::uint16_t function);
        };

    }
}
    
#endif