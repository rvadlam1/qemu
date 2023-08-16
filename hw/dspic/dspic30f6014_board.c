#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qapi/error.h"
#include "dspicexp.h"
#include "boot.h"
#include "qom/object.h"
#include "hw/boards.h"

struct DSPICExampleBoardMachineState {
    MachineState parent_obj;
    DSPICEXPMcuState mcu;
};
typedef struct DSPICExampleBoardMachineState DSPICExampleBoardMachineState;

struct DSPICExampleBoardMachineClass {
    MachineClass parent_class;
};

#define TYPE_DSPICEXAMPLE_BOARD_BASE_MACHINE MACHINE_TYPE_NAME("dspicexample-board-base")
#define TYPE_DSPICEXAMPLE_BOARD_MACHINE MACHINE_TYPE_NAME("dspicexample-board")

DECLARE_OBJ_CHECKERS(DSPICExampleBoardMachineState, DSPICExampleBoardMachineClass,
        DSPICEXAMPLE_BOARD_MACHINE, TYPE_DSPICEXAMPLE_BOARD_MACHINE)

//The generic MachineState is passed by QEMU
static void dspicexample_board_init(MachineState *machine)
{
    //Make a specific MachineState out of the generic one
    DSPICExampleBoardMachineState* m_state = DSPICEXAMPLE_BOARD_MACHINE(machine);

    //We initialize the mocrocontroller that is part of the board
    object_initialize_child(OBJECT(machine), "mcu", &m_state->mcu, TYPE_DSPICEXPS_MCU);
    //And we connect it via QEMUs SYSBUS.
    sysbus_realize(SYS_BUS_DEVICE(&m_state->mcu), &error_abort);

    //Here we load the firmware file with a load function that we will implment in boot.c
    if (machine->firmware) {
        if (!dspic_load_firmware(&m_state->mcu.cpu, machine,
                                 &m_state->mcu.flash, machine->firmware)) {
            exit(1);
        }
    }
}


//Generic Objectc is passed by QEMU
static void dspicexample_board_class_init(ObjectClass *oc, void *data)
{
    //The generic machine class from object
    MachineClass *mc = MACHINE_CLASS(oc);
    mc->desc = "DSPIC Example Board";
    mc->alias = "dspicexample-board";
    
    //Notice that we tell QEMU what function is used to initialize our board here.
    mc->init = dspicexample_board_init;
    mc->default_cpus = 1;
    mc->min_cpus = mc->default_cpus;
    mc->max_cpus = mc->default_cpus;
    // Our board does not have any media drive
    mc->no_floppy = 1;
    mc->no_cdrom = 1;
    //We also will not have threads
    mc->no_parallel = 1;
}

static const TypeInfo dspicexample_board_machine_types[] = {
        {
                                //Notice that this is the TYPE that we defined above.
                .name           = TYPE_DSPICEXAMPLE_BOARD_MACHINE,
                                //Our machine is a direct child of QEMU generic machine
                .parent         = TYPE_MACHINE,
                .instance_size  = sizeof(DSPICExampleBoardMachineState),
                .class_size     = sizeof(DSPICExampleBoardMachineClass),
                //We need to register the class init function 
                .class_init     = dspicexample_board_class_init,
        }
};
DEFINE_TYPES(dspicexample_board_machine_types)
