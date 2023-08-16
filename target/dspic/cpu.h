#ifndef QEMU_DSPICA_CPU_H
#define QEMU_DSPICA_CPU_H

#include "cpu-qom.h"
#include "exec/cpu-defs.h"

#define DSPIC_EXP 0x100
#define DSPIC_EXP_S    DSPIC_EXP | 0x30

//DSPIC CPUs have 16 general purpose registers, and PC.
#define DSPICA_REG_PAGE_SIZE 16 // w0 - w15

//The CPU need a definition
struct DSPICACPUDef {
    const char* name;
    const char* parent_microarch; // DSPICA or DSPICB, two variants of the DSPIC instruction set.
    size_t core_type;
    size_t series_type;
    size_t clock_speed;
};
#define DSPICA_CPU_TYPE_SUFFIX "-" TYPE_DSPICA_CPU
#define DSPICA_CPU_TYPE_NAME(name) (name DSPICA_CPU_TYPE_SUFFIX)
#define CPU_RESOLVING_TYPE TYPE_DSPICA_CPU

