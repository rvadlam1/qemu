#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/units.h"
#include "qapi/error.h"
#include "exec/memory.h"
#include "exec/address-spaces.h"
#include "sysemu/sysemu.h"
#include "hw/qdev-properties.h"
#include "hw/sysbus.h"
#include "qom/object.h"
#include "hw/misc/unimp.h"
#include "dspicexp.h"

struct DSPICEXPMcuClass {
    /*< private >*/
    SysBusDeviceClass parent_class;

    /*< public >*/
    const char *cpu_type;

    size_t flash_size;
};
typedef struct DSPICEXPMcuClass DSPICEXPMcuClass;
DECLARE_CLASS_CHECKERS(DSPICEXPMcuClass, DSPICEXP_MCU,
        TYPE_DSPICEXP_MCU)

// This functions sets up the device
static void dspicexp_realize(DeviceState *dev, Error **errp)
{
    //We create a state for the microcontroller form the generic state
    DSPICEXPMcuState *s = DSPICEXP_MCU(dev);
    //And we create a class from the state
    const DSPICEXPMcuClass *mc = DSPICEXP_MCU_GET_CLASS(dev);

    // The DSPIC CPU was defined in the DSPICEXPMcuState
    object_initialize_child(OBJECT(dev), "cpu", &s->cpu, mc->cpu_type);
    //Set the CPU object to realized
    object_property_set_bool(OBJECT(&s->cpu), "realized", true, &error_abort);

    //Init the flash memory region
    memory_region_init_rom(&s->flash, OBJECT(dev),
                           "flash", mc->flash_size, &error_fatal);
    //Here we set the start address of the memory region 
    memory_region_add_subregion(get_system_memory(),
                                0xd0000000, &s->flash);
}

static void dspicexp_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    //Set the actuall setup function
    dc->realize = dspicexp_realize;
    dc->user_creatable = false;
}

static void dspicexps_class_init(ObjectClass *oc, void *data){

    DSPICEXPMcuClass* dspicexp = DSPICEXP_MCU_CLASS(oc);

    dspicexp->cpu_type = DSPICA_CPU_TYPE_NAME("DSPICEXPC");
    dspicexp->flash_size = 1024 * KiB;
}

static const TypeInfo dspicexp_mcu_types[] = {
        {
                .name           = TYPE_DSPICEXPS_MCU,
                .parent         = TYPE_DSPICEXP_MCU,
                .class_init     = dspicexps_class_init,
        }, {
                .name           = TYPE_DSPICEXP_MCU,
                .parent         = TYPE_SYS_BUS_DEVICE,
                .instance_size  = sizeof(DSPICEXPMcuState),
                .class_size     = sizeof(DSPICEXPMcuClass),
                .class_init     = dspicexp_class_init,
                .abstract       = true,
        }
};

DEFINE_TYPES(dspicexp_mcu_types)

