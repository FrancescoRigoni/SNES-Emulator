
//#ifndef SNESEMU_BUILDCONFIG_HPP_H
//#define SNESEMU_BUILDCONFIG_HPP_H


/**
 * The following define enables building the Cpu65816 for 65C02 emulation.
 * Note that this will enable 65C02 emulation but NOT R65C02 emulation.
 * The R65C02 cpu has several different opcodes and is not compatible
 * with the 65C02.
 *
 * This define enables some minor differences in cycles counting
 * for some opcodes.
 * */
//#define EMU_65C02

//#endif SNESEMU_BUILDCONFIG_HPP_H