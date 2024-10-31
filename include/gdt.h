#ifndef __maxos__GDT_H
#define __maxos__GDT_H

#include <common/types.h>

namespace maxos
{
    class GlobalDescriptorTable
    {
        public:

            class SegmentDescriptor
            {
                private:
                    maxos::common::uint16_t limit_lo;
                    maxos::common::uint16_t base_lo;
                    maxos::common::uint8_t base_hi;
                    maxos::common::uint8_t type;
                    maxos::common::uint8_t limit_hi;
                    maxos::common::uint8_t base_vhi;

                public:
                    SegmentDescriptor(maxos::common::uint32_t base, maxos::common::uint32_t limit, maxos::common::uint8_t type);
                    maxos::common::uint32_t Base();
                    maxos::common::uint32_t Limit();
            } __attribute__((packed));

        private:
            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:

            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            maxos::common::uint16_t CodeSegmentSelector();
            maxos::common::uint16_t DataSegmentSelector();
    };
}
#endif