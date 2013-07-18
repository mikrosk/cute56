#ifndef DSP_DSPFIX_ASM_H
#define DSP_DSPFIX_ASM_H

/* dsp.h
 *
 * COPYRIGHT (c) 1998 by NoCrew Laboratories.
 *
 * Implements:
 *
 *     extern Int dsp_load_program(Byte *program, Int length);
 *
 * Where length is the number of DSP words (e.i. 3 byte tuples).
 * A return value of 1 means that the loading was sucessful. 0
 * means something went wrong.
 *
 * This module will always load the DSP program. It is compatible
 * with the Dsp_ExecProg loader. However, as it reloads the DSP
 * bootstrap code every time -- which the TOS version does not --
 * this one does not freeze up.
 */

extern int asm_dsp_load_program( const char* program, int length );

#endif
