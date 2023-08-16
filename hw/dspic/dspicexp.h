#ifndef HW_DSPIC_DSPICEXPC_H
#define HW_DSPIC_DSPICEXPC_H
#include "target/dspic/cpu.h"
#include "qom/object.h"
#include "hw/sysbus.h"
#define TYPE_DSPICEXP_MCU "DSPICEXP" //This will be the CPU
#define TYPE_DSPICEXPS_MCU "DSPICEXPS" //This will be a system on chip (SOC)

typedef struct DSPICEXPMcuState DSPICEXPMcuState;
DECLARE_INSTANCE_CHECKER(DSPICEXPMcuState, DSPICEXP_MCU, TYPE_DSPICEXP_MCU)

//The Microcontroller needs a state
struct DSPICEXPMcuState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    //This is a reference to the CPU logic. We implement it in part 2.
    DSPICACPU cpu;
    //The flash that will contain the firmware.
    MemoryRegion flash;
};
#endif // HW_DSPIC_DSPICEXPC_H
