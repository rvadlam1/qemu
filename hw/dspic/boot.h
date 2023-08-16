#ifndef HW_DSPIC_BOOT_H
#define HW_DSPIC_BOOT_H

#include "hw/boards.h"
#include "cpu.h"

bool dspic_load_firmware(DSPICACPU *cpu, MachineState *ms,
                         MemoryRegion *mr, const char *firmware);

#endif // HW_DSPIC_BOOT_H
