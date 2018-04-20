/*
** This file has been pre-processed with DynASM.
** http://luajit.org/dynasm.html
** DynASM version 1.3.0, DynASM x64 version 1.3.0
** DO NOT EDIT! The original file is in "src/jit/x64/emit.dasc".
*/

#line 1 "src/jit/x64/emit.dasc"
/* -*-C-*- */
#include "moar.h"
#include "jit/internal.h"
#include "dasm_x86.h"

#ifdef _MSC_VER
#pragma warning( disable : 4129 )
#endif

/**
 * CONVENTIONS

 * Much of this file contains snippets of assembly code, which are concatenated
 * at runtime in order to form a single executable routine. It is essential for
 * the correctness of the result that each of the snippets behaves
 * nicely. Because you can't be expected to know what that is, it is documented
 * here.

 * REGISTERS:

 * Register and calling conventions differ between POSIX and windows
 * systems. The registers rax, rcx, rdx, r8, r9, r10 and r11 are caller-saved,
 * meaning that you are free to overwrrite them, and functions you may call are
 * free to do thesame. Hence you should save their values on stack, if you wish
 * to keep them after calling. In contrast, rbx, rsp, rbp, and r12-r15 are
 * callee-saved, meaning that their values before entering and after returning
 * from a function must be the same. POSIX also makes rdi and rsi caller-saved,
 * windows makes them callee-saved. For this reason we avoid using them.  The
 * first 4 (windows) or 6 (POSIX) function call arguments are placed in
 * registers. These registers sets are not the same between windows and POSIX,
 * but they're always caller-saved.

 * To deal with the ambiguity, register names have been aliased.

 * + RV stands for 'return value', and is aliased to rax
 * + TMP1-6 are the 6 shared caller-saved registers
 * + ARG1-4 are (different) aliases for argument registers
 * + TC, CU, WORK are registers that hold interpreter variables; these are callee-
 *   saved registers set up at entry and restored at exit
 * + TMP5 (r10) is also aliased as FUNCTION; it never conflicts with an argument
 *   register, and neither does TMP6.
 * + The suffixes d, w, and b stand for the 4, 2, and 1 byte-width value of the
 *   registers.

 * Note that the current convention for function calls is to load the function
 * pointer as a 64 bit value in a register from the machine code, and call on
 * that register. This is not ideal, but call doesn't natively take 64 bit
 * values, and it is neccesary to ensure that the function lives within 32 bits
 * distance from the function otherwise. Other methods are being considered.

 * LABELS:

 * Don't use dynamic labels in this code, unless they have been passed to you
 * from outside. Dynamic labels need to be allocated and not conflict, hence
 * just picking one is typically unsafe. You are allowed to use in a snippet
 * the local labels 1-5; the labels 6-9 are reserved by special constructs like
 * THROWISH_PRE and INVOKISH.

 * WRITE BARRIERS:

 * Use of write barriers is tricky, because they may involve a function call, and
 * that may or may not mean you have to store your temporaries on the stack.
 * Hence, a write barrier (MVM_ASSIGN_REF) is split into two parts:

 * + check_wb (root, value, label)
 * + hit_wb (root)

 * You should have the label parameter point somewhere after hit_wb, and save
 * and restore your temporaries around the hib_wb.
 **/


//|.arch x64
#if DASM_VERSION != 10300
#error "Version mismatch between DynASM and included encoding engine"
#endif
#line 74 "src/jit/x64/emit.dasc"
//|.actionlist actions
static const unsigned char actions[4670] = {
  254,0,85,72,137,229,255,72,129,250,236,0,1,0,0,255,76,137,117,250,248,76,
  137,109,250,240,72,137,93,250,232,255,68,139,159,231,68,137,93,224,255,73,
  137,250,254,73,137,250,245,77,139,158,231,73,139,155,231,255,250,255,226,
  255,246,10,255,76,139,117,250,248,76,139,109,250,240,72,139,93,250,232,255,
  72,137,250,236,93,195,255,72,185,235,235,72,137,139,231,255,72,199,131,231,
  235,255,73,139,141,231,72,139,137,231,72,137,139,231,255,73,139,142,231,72,
  139,137,231,72,137,139,231,255,72,139,139,231,72,139,137,231,255,72,139,137,
  231,73,139,150,231,72,139,146,231,72,133,201,72,15,68,202,255,77,139,158,
  231,255,77,139,155,231,255,77,139,147,231,255,77,139,146,231,255,77,133,210,
  15,133,242,247,255,76,137,250,247,76,137,222,72,199,194,235,254,1,246,5,235,
  235,254,0,250,255,21,242,5,255,73,137,194,246,1,255,76,137,147,231,255,76,
  139,155,231,77,139,155,231,255,76,137,250,247,255,73,139,181,231,72,139,182,
  231,72,199,194,235,254,1,250,255,21,242,5,72,133,192,15,132,242,247,72,139,
  0,72,137,131,231,250,233,242,248,246,1,77,139,134,231,77,139,128,231,76,137,
  131,231,246,2,255,73,139,142,231,255,72,139,145,231,76,139,131,231,76,137,
  130,231,255,102,250,247,129,231,234,15,132,242,248,77,133,192,15,132,242,
  248,102,65,250,247,128,231,234,15,133,242,248,72,137,206,76,137,250,247,254,
  1,250,255,21,242,5,246,2,255,73,139,142,231,72,139,137,231,72,139,137,231,
  72,137,139,231,255,72,139,139,231,72,141,145,231,76,139,137,231,77,141,145,
  231,77,133,201,73,15,69,210,255,76,139,2,77,133,192,255,15,133,242,249,255,
  77,139,134,231,77,139,128,231,246,3,255,76,139,2,255,77,133,192,15,133,242,
  250,255,77,139,134,231,77,139,128,231,77,139,128,231,255,102,250,247,129,
  231,234,15,132,242,249,77,133,192,15,132,242,249,102,65,250,247,128,231,234,
  15,133,242,249,72,137,85,216,76,137,69,208,72,139,179,231,76,137,250,247,
  254,1,250,255,21,242,5,76,139,69,208,72,139,85,216,246,3,255,76,137,2,246,
  4,255,76,139,2,77,133,192,15,133,242,250,255,76,137,250,247,73,139,182,231,
  72,139,182,231,72,139,182,231,254,1,250,255,21,242,5,73,137,192,255,250,255,
  21,242,5,76,139,69,208,72,139,85,216,246,3,76,137,2,255,76,137,131,231,255,
  72,139,139,231,72,139,147,231,255,102,250,247,129,231,234,15,132,242,248,
  72,133,210,15,132,242,248,102,250,247,130,231,234,15,133,242,248,255,250,
  255,21,242,5,72,139,139,231,72,139,147,231,246,2,255,72,137,145,231,255,72,
  139,139,231,72,139,145,231,72,137,147,231,255,72,139,139,231,72,139,147,231,
  72,139,137,231,72,137,17,255,72,139,139,231,76,139,129,231,73,139,16,72,137,
  147,231,255,72,139,139,231,72,139,147,231,76,141,129,231,73,131,184,231,0,
  15,132,242,247,77,139,128,231,246,1,255,102,250,247,129,231,234,15,132,242,
  248,72,133,210,15,132,242,248,102,250,247,130,231,234,15,133,242,248,72,137,
  85,216,76,137,69,208,72,139,179,231,76,137,250,247,254,1,250,255,21,242,5,
  76,139,69,208,72,139,85,216,246,2,255,73,137,144,231,255,72,139,139,231,72,
  137,139,231,255,72,139,139,231,73,137,142,231,255,73,139,142,231,72,131,250,
  249,0,15,132,242,248,73,139,150,231,72,131,250,250,0,15,132,242,247,77,139,
  134,231,77,139,128,231,76,57,194,15,133,242,248,246,1,72,137,139,231,73,199,
  134,231,235,250,233,242,249,246,2,73,139,142,231,72,139,137,231,72,137,139,
  231,246,3,255,73,139,142,231,72,139,137,231,72,133,201,15,132,242,247,72,
  139,137,231,246,1,72,137,139,231,255,73,139,141,231,255,72,199,131,231,0,
  0,0,0,255,72,139,139,231,72,133,201,15,148,210,72,15,182,210,72,137,147,231,
  255,72,129,131,231,237,255,72,129,171,231,237,255,72,129,139,231,237,255,
  72,129,163,231,237,255,72,129,179,231,237,255,72,139,131,231,255,72,1,131,
  231,255,72,41,131,231,255,72,9,131,231,255,72,33,131,231,255,72,49,131,231,
  255,72,129,192,237,255,72,129,250,232,237,255,72,129,200,237,255,72,129,224,
  237,255,72,129,250,240,237,255,72,137,131,231,255,72,3,131,231,255,72,43,
  131,231,255,72,11,131,231,255,72,35,131,231,255,72,51,131,231,255,72,15,175,
  131,231,255,138,139,231,72,211,224,255,138,139,231,72,211,250,232,255,72,
  139,131,231,72,139,139,231,72,131,250,249,0,15,133,242,247,76,137,250,247,
  72,190,235,235,254,1,250,255,21,242,5,246,1,255,15,156,214,72,131,250,248,
  0,15,156,210,48,250,242,68,15,182,194,72,153,72,250,247,250,249,72,133,210,
  15,149,209,65,32,200,255,76,41,192,72,137,131,231,255,250,255,21,242,5,246,
  1,72,153,72,250,247,250,249,72,137,147,231,255,72,131,131,231,1,255,72,131,
  171,231,1,255,72,139,139,231,72,250,247,209,72,137,139,231,255,72,139,139,
  231,72,250,247,217,72,137,139,231,255,72,139,131,231,72,152,72,137,131,231,
  255,72,139,139,231,255,137,202,72,137,147,231,255,250,242,15,16,131,231,255,
  250,242,15,88,131,231,255,250,242,15,92,131,231,255,250,242,15,89,131,231,
  255,250,242,15,94,131,231,255,250,242,15,17,131,231,255,250,242,72,15,42,
  131,231,250,242,15,17,131,231,255,250,242,72,15,44,131,231,72,137,131,231,
  255,72,199,193,1,0,0,0,72,193,225,63,72,139,147,231,72,49,202,72,137,147,
  231,255,72,59,131,231,255,15,148,208,255,15,149,208,255,15,156,208,255,15,
  158,208,255,15,159,208,255,15,157,208,255,72,15,182,192,72,137,131,231,255,
  72,59,139,231,255,15,159,210,72,15,182,210,65,15,156,208,77,15,182,192,76,
  41,194,72,137,147,231,255,72,199,194,1,0,0,0,255,72,199,194,250,255,250,255,
  250,255,250,255,255,72,199,194,0,0,0,0,255,72,59,147,231,255,72,199,193,0,
  0,0,0,255,72,199,193,1,0,0,0,255,250,242,15,16,131,231,102,15,46,131,231,
  255,15,147,209,255,15,155,210,72,15,68,202,255,15,154,210,72,15,68,202,255,
  15,151,209,255,72,15,182,201,72,137,139,231,255,250,242,15,16,131,231,250,
  242,15,16,139,231,102,15,46,193,15,151,209,72,15,182,193,102,15,46,200,15,
  151,209,72,15,182,201,72,41,200,72,137,131,231,255,72,199,199,235,72,139,
  179,231,72,139,147,231,254,1,72,129,250,248,237,15,148,208,255,72,129,250,
  248,237,15,149,208,255,72,139,139,231,72,133,201,15,132,242,247,72,139,137,
  231,72,139,137,231,72,129,185,231,237,15,133,242,247,72,199,131,231,1,0,0,
  0,250,233,242,248,246,1,72,199,131,231,0,0,0,0,246,2,255,72,139,139,231,72,
  139,145,231,72,131,194,1,76,139,129,231,255,76,57,194,15,156,209,72,15,182,
  201,72,137,139,231,255,72,139,139,231,72,139,145,231,72,133,210,15,149,210,
  72,15,182,210,72,137,147,231,255,76,139,155,231,77,133,219,15,132,242,247,
  73,139,179,231,76,139,150,231,77,139,146,231,76,137,250,247,65,250,255,210,
  76,15,183,152,231,246,1,76,137,155,231,255,76,139,155,231,77,133,219,15,132,
  242,247,73,139,179,231,76,139,150,231,77,139,146,231,76,137,250,247,65,250,
  255,210,76,15,183,152,231,77,133,219,15,132,242,247,76,15,183,152,231,246,
  1,76,137,155,231,255,72,139,139,231,72,133,201,15,149,210,77,139,134,231,
  77,139,128,231,76,57,193,65,15,149,208,68,32,194,72,15,182,210,72,137,147,
  231,255,72,139,139,231,72,133,201,15,148,210,77,139,134,231,77,139,128,231,
  76,57,193,65,15,148,208,68,8,194,72,15,182,210,72,137,147,231,255,76,137,
  250,247,72,199,198,235,254,1,250,255,21,242,5,73,139,142,231,72,139,137,231,
  72,139,137,231,72,137,136,231,102,199,128,231,234,65,139,142,231,137,136,
  231,72,137,131,231,255,76,139,147,231,77,133,210,255,15,132,242,247,102,65,
  250,247,130,231,234,255,15,133,242,247,77,139,154,231,77,139,155,231,77,133,
  219,255,15,132,242,247,76,137,250,247,76,137,214,72,141,147,231,77,139,147,
  231,65,250,255,210,250,233,242,248,246,1,255,76,137,147,231,246,2,255,72,
  139,139,231,72,133,201,15,132,242,247,72,139,137,231,72,139,137,231,72,133,
  201,246,1,15,149,209,72,15,182,201,72,137,139,231,255,73,139,142,231,72,139,
  137,231,72,139,137,231,72,139,147,231,72,139,146,231,72,57,209,15,133,242,
  247,77,139,134,231,77,139,128,231,77,139,128,231,76,137,131,231,250,233,242,
  248,246,1,255,73,139,134,231,76,141,29,242,248,76,137,152,231,255,76,137,
  250,247,72,139,179,231,255,73,139,149,231,72,139,146,231,72,199,193,235,76,
  141,155,231,77,137,216,254,1,250,255,21,242,5,72,133,192,255,15,133,242,10,
  246,2,255,72,139,139,231,72,133,201,15,132,242,247,102,250,247,129,231,234,
  15,133,242,247,72,199,131,231,1,0,0,0,250,233,242,248,246,1,72,199,131,231,
  0,0,0,0,246,2,255,73,139,142,231,72,137,139,231,73,199,134,231,0,0,0,0,255,
  73,139,142,231,72,133,201,15,132,242,247,72,139,137,231,72,137,139,231,250,
  233,242,248,246,1,73,139,142,231,72,139,137,231,72,137,139,231,246,2,255,
  65,139,142,231,131,193,1,65,137,142,231,72,137,139,231,255,65,139,142,231,
  131,250,233,1,65,137,142,231,72,137,139,231,255,72,139,179,231,76,139,150,
  231,77,139,146,231,77,133,210,15,133,242,247,76,137,250,247,72,190,235,235,
  254,1,250,255,21,242,5,246,1,76,137,250,247,72,139,147,231,255,65,250,255,
  210,255,76,137,250,247,72,139,179,231,72,199,194,235,254,1,250,255,21,242,
  5,72,133,192,15,132,242,247,72,139,0,246,1,72,137,131,231,255,73,139,182,
  231,72,141,182,231,102,68,139,150,231,102,68,59,150,231,15,132,242,247,76,
  137,250,247,254,1,72,139,139,231,72,133,201,15,133,242,247,76,137,250,247,
  254,1,76,137,250,247,73,139,182,231,72,139,182,231,72,199,194,235,254,1,76,
  137,250,247,73,139,182,231,72,139,182,231,72,139,182,231,72,199,194,235,254,
  1,72,139,139,231,139,145,231,77,49,192,131,250,250,0,15,142,242,247,77,139,
  134,231,77,139,128,231,77,139,4,208,77,139,128,231,246,1,76,137,131,231,255,
  102,250,247,129,231,234,15,133,242,247,72,139,145,231,72,139,146,231,129,
  186,231,237,15,132,242,248,246,1,255,76,137,250,247,72,139,179,231,72,49,
  210,254,1,72,137,198,255,72,141,187,231,77,139,158,231,73,137,187,231,255,
  65,198,131,231,233,255,73,139,190,231,72,139,63,73,137,187,231,255,76,137,
  250,247,72,139,147,231,72,139,138,231,72,139,145,231,72,139,137,231,76,139,
  150,231,77,139,146,231,65,250,255,210,255,250,255,21,242,5,246,2,72,139,145,
  231,72,15,183,146,231,72,137,147,231,255,250,255,21,242,5,246,2,72,139,145,
  231,102,68,139,130,231,102,139,146,231,102,68,57,194,15,149,208,72,15,182,
  192,72,137,131,231,255,77,139,134,231,255,250,255,21,242,5,72,137,131,231,
  255,72,139,145,231,72,139,146,231,129,186,231,237,15,132,242,247,255,250,
  255,21,242,5,246,1,72,139,147,231,72,137,145,231,255,77,139,134,231,76,137,
  131,231,255,76,137,250,247,72,139,183,231,72,141,182,231,72,199,194,235,254,
  1,76,137,250,247,72,139,183,231,72,141,182,231,254,1,76,137,250,247,72,139,
  179,231,72,139,147,231,72,139,139,231,76,141,131,231,76,139,150,231,77,139,
  146,231,77,139,146,231,65,250,255,210,255,76,137,250,247,72,139,179,231,76,
  139,150,231,77,139,146,231,77,139,146,231,65,250,255,210,72,137,131,231,255,
  76,137,250,247,72,139,179,231,72,139,147,231,76,139,150,231,77,139,146,231,
  77,139,146,231,65,250,255,210,255,77,137,250,243,255,77,137,250,235,255,77,
  139,158,231,77,141,155,231,255,77,139,158,231,77,139,155,231,255,76,139,155,
  231,255,76,141,155,231,255,77,139,157,231,77,139,155,231,255,73,199,195,235,
  255,73,187,235,235,255,76,139,155,231,77,141,155,231,255,77,139,150,231,77,
  139,18,77,49,219,102,69,139,154,231,78,139,28,219,255,76,141,29,243,255,76,
  139,93,216,255,77,139,158,231,77,139,155,231,77,139,155,231,255,77,139,158,
  231,77,139,155,231,77,139,155,231,77,139,155,231,255,76,137,223,255,76,137,
  222,255,76,137,218,255,76,137,217,255,77,137,216,255,77,137,217,255,102,73,
  15,110,195,255,102,73,15,110,203,255,102,73,15,110,211,255,102,73,15,110,
  219,255,102,73,15,110,227,255,102,73,15,110,250,235,255,102,73,15,110,250,
  243,255,102,73,15,110,250,251,255,68,136,156,251,36,231,255,102,68,137,156,
  251,36,231,255,76,137,156,251,36,231,255,72,139,8,72,137,139,231,255,72,139,
  139,231,72,137,8,255,73,139,142,231,72,139,9,72,49,210,102,139,145,231,72,
  137,4,211,255,73,131,190,231,0,15,132,242,247,76,137,250,247,254,1,250,233,
  242,10,255,250,233,243,255,72,139,131,231,72,133,192,15,133,243,255,72,139,
  131,231,72,133,192,15,132,243,255,102,72,15,110,131,231,102,15,87,201,102,
  15,46,193,15,138,243,15,133,243,255,102,72,15,110,131,231,102,15,87,201,102,
  15,46,193,15,138,242,247,15,133,242,247,250,233,243,246,1,255,76,137,250,
  247,72,139,179,231,254,1,72,139,139,231,72,133,201,15,132,242,247,73,139,
  150,231,72,139,146,231,72,57,209,15,132,242,247,250,233,243,246,1,255,76,
  137,250,247,72,139,179,231,72,139,147,231,255,73,139,141,231,72,139,137,231,
  254,1,72,131,250,248,250,255,255,15,142,243,255,15,140,243,255,15,141,243,
  255,15,143,243,255,72,139,139,231,73,139,150,231,72,139,146,231,72,139,146,
  231,255,72,133,201,15,132,242,247,255,72,59,145,231,15,133,242,247,255,102,
  250,247,129,231,234,255,102,250,247,129,231,234,15,133,242,247,255,76,139,
  129,231,77,139,128,231,73,129,184,231,237,15,133,242,247,72,59,145,231,15,
  133,242,247,255,76,137,250,247,72,199,198,235,72,199,194,235,254,1,250,233,
  242,10,246,2,255,77,139,150,231,77,137,154,231,255,65,198,130,231,233,255,
  73,199,130,231,235,255,72,141,147,231,73,137,146,231,255,73,139,150,231,72,
  139,18,73,137,146,231,255,72,141,21,243,73,137,146,231,255,76,139,139,231,
  77,137,138,231,255,73,185,235,235,77,137,138,231,255,77,139,141,231,77,139,
  137,231,77,137,138,231,255,76,137,85,216,76,137,93,208,255,76,137,250,247,
  72,139,179,231,72,141,85,208,76,137,209,73,199,192,0,0,0,0,254,1,76,139,93,
  208,76,139,85,216,255,76,137,250,247,72,137,198,76,137,218,76,137,209,255,
  76,139,144,231,77,139,146,231,65,250,255,210,255,76,137,250,247,72,139,179,
  231,76,137,218,72,199,193,235,254,1,72,139,139,231,72,131,250,249,0,15,140,
  242,248,72,129,250,249,237,15,141,242,248,72,107,201,8,72,141,21,242,247,
  72,1,202,250,255,226,248,7,246,1,255,247,250,233,243,248,7,255,65,139,134,
  231,59,69,224,15,132,242,255,250,233,242,10,246,9,255,72,141,5,242,247,73,
  252,139,134,251,238,10,231,72,252,137,128,251,238,8,231,246,1,255,72,252,
  141,5,238,2,242,255,73,139,134,231,72,252,137,128,251,238,10,231,255,65,139,
  134,231,59,69,224,15,133,242,254,255,73,139,134,231,250,255,160,231,255,246,
  8,250,233,242,10,255,205,3,255,64,252,138,128,251,238,10,238,8,231,255,102,
  253,139,128,251,238,10,238,8,231,255,72,252,139,128,251,238,10,238,8,231,
  255,64,252,136,128,251,238,10,238,8,231,255,102,253,137,128,251,238,10,238,
  8,231,255,72,252,137,128,251,238,10,238,8,231,255,72,252,137,192,238,14,238,
  12,255,144,255,72,137,69,216,255,72,252,141,128,251,238,10,238,8,231,255,
  72,252,141,132,251,238,10,192,238,15,238,13,231,255,72,252,184,238,0,235,
  235,255,72,252,199,192,238,12,235,255,253,138,128,251,238,10,238,8,231,255,
  64,252,138,132,251,238,10,192,238,15,238,13,231,255,102,253,139,132,251,238,
  10,192,238,15,238,13,231,255,72,252,139,132,251,238,10,192,238,15,238,13,
  231,255,64,252,136,132,251,238,10,192,238,15,238,13,231,255,102,253,137,132,
  251,238,10,192,238,15,238,13,231,255,72,252,137,132,251,238,10,192,238,15,
  238,13,231,255,102,64,252,15,190,192,238,14,238,12,255,253,15,191,192,238,
  14,238,12,255,72,252,15,190,192,238,14,238,12,255,72,252,15,191,192,238,14,
  238,12,255,253,137,192,238,14,72,152,72,252,137,192,238,12,255,102,64,252,
  15,182,192,238,14,238,12,255,253,15,183,192,238,14,238,12,255,72,252,15,182,
  192,238,14,238,12,255,72,252,15,183,192,238,14,238,12,255,253,137,192,238,
  14,238,12,255,72,252,137,192,238,14,255,72,252,1,192,238,14,238,12,255,72,
  184,235,235,72,252,1,192,238,12,255,72,252,129,192,238,12,237,255,72,252,
  184,238,0,235,235,72,252,1,192,238,14,238,12,255,72,252,137,192,238,14,238,
  12,72,252,129,192,238,12,237,255,64,252,2,128,251,238,10,238,8,231,255,102,
  253,3,128,251,238,10,238,8,231,255,72,252,3,128,251,238,10,238,8,231,255,
  64,252,2,132,251,238,10,192,238,15,238,13,231,255,102,253,3,132,251,238,10,
  192,238,15,238,13,231,255,72,252,3,132,251,238,10,192,238,15,238,13,231,255,
  72,252,33,192,238,14,238,12,255,72,184,235,235,72,252,33,192,238,12,255,72,
  252,129,224,238,12,237,255,72,252,184,238,0,235,235,72,252,33,192,238,14,
  238,12,255,72,252,137,192,238,14,238,12,72,252,129,224,238,12,237,255,64,
  252,34,128,251,238,10,238,8,231,255,102,253,35,128,251,238,10,238,8,231,255,
  72,252,35,128,251,238,10,238,8,231,255,64,252,34,132,251,238,10,192,238,15,
  238,13,231,255,102,253,35,132,251,238,10,192,238,15,238,13,231,255,72,252,
  35,132,251,238,10,192,238,15,238,13,231,255,72,252,41,192,238,14,238,12,255,
  72,184,235,235,72,252,41,192,238,12,255,72,252,129,250,232,238,12,237,255,
  72,184,235,235,72,252,137,192,238,14,238,12,72,252,41,192,238,12,255,72,252,
  137,192,238,14,238,12,72,252,129,250,232,238,12,237,255,64,252,42,128,251,
  238,10,238,8,231,255,102,253,43,128,251,238,10,238,8,231,255,72,252,43,128,
  251,238,10,238,8,231,255,64,252,42,132,251,238,10,192,238,15,238,13,231,255,
  102,253,43,132,251,238,10,192,238,15,238,13,231,255,72,252,43,132,251,238,
  10,192,238,15,238,13,231,255,64,252,132,192,238,14,238,12,255,102,253,133,
  192,238,14,238,12,255,72,252,133,192,238,14,238,12,255,64,252,128,184,251,
  238,8,231,0,255,102,253,131,184,251,238,8,231,0,255,72,252,131,184,251,238,
  8,231,0,255,64,252,128,188,251,192,238,15,238,13,231,0,255,102,253,131,188,
  251,192,238,15,238,13,231,0,255,72,252,131,188,251,192,238,15,238,13,231,
  0,255,64,252,250,246,192,238,12,233,255,102,253,250,247,192,238,12,234,255,
  253,250,247,192,238,12,235,255,72,252,250,247,192,238,12,235,255,72,184,235,
  235,72,252,133,192,238,12,255,64,252,250,246,128,251,238,8,231,233,255,102,
  253,250,247,128,251,238,8,231,234,255,253,250,247,128,251,238,8,231,235,255,
  72,252,250,247,128,251,238,8,231,235,255,72,184,235,235,72,252,133,128,251,
  238,8,231,255,64,252,56,192,238,14,238,12,255,102,253,57,192,238,14,238,12,
  255,72,252,57,192,238,14,238,12,255,64,252,15,156,208,238,12,255,64,252,15,
  158,208,238,12,255,64,252,15,148,208,238,12,255,64,252,15,149,208,238,12,
  255,64,252,15,157,208,238,12,255,64,252,15,159,208,238,12,255,72,252,141,
  5,238,2,243,255,253,250,255,208,238,12,255,253,250,255,144,251,238,8,231,
  255
};

#line 75 "src/jit/x64/emit.dasc"
//|.section code, data
#define DASM_SECTION_CODE	0
#define DASM_SECTION_DATA	1
#define DASM_MAXSECTION		2
#line 76 "src/jit/x64/emit.dasc"
//|.globals MVM_JIT_LABEL_
enum {
  MVM_JIT_LABEL_exit,
  MVM_JIT_LABEL__MAX
};
#line 77 "src/jit/x64/emit.dasc"

/* type declarations */
//|.type REGISTER, MVMRegister
#define Dt1(_V) (int)(ptrdiff_t)&(((MVMRegister *)0)_V)
#line 80 "src/jit/x64/emit.dasc"
//|.type FRAME, MVMFrame
#define Dt2(_V) (int)(ptrdiff_t)&(((MVMFrame *)0)_V)
#line 81 "src/jit/x64/emit.dasc"
//|.type ARGCTX, MVMArgProcContext
#define Dt3(_V) (int)(ptrdiff_t)&(((MVMArgProcContext *)0)_V)
#line 82 "src/jit/x64/emit.dasc"
//|.type CALLSITEPTR, MVMCallsite*
#define Dt4(_V) (int)(ptrdiff_t)&(((MVMCallsite* *)0)_V)
#line 83 "src/jit/x64/emit.dasc"
//|.type CAPTURE, MVMCallCapture
#define Dt5(_V) (int)(ptrdiff_t)&(((MVMCallCapture *)0)_V)
#line 84 "src/jit/x64/emit.dasc"
//|.type CAPTUREBODY, MVMCallCaptureBody
#define Dt6(_V) (int)(ptrdiff_t)&(((MVMCallCaptureBody *)0)_V)
#line 85 "src/jit/x64/emit.dasc"
//|.type ARGPROCCONTEXT, MVMArgProcContext
#define Dt7(_V) (int)(ptrdiff_t)&(((MVMArgProcContext *)0)_V)
#line 86 "src/jit/x64/emit.dasc"
//|.type STATICFRAME, MVMStaticFrame
#define Dt8(_V) (int)(ptrdiff_t)&(((MVMStaticFrame *)0)_V)
#line 87 "src/jit/x64/emit.dasc"
//|.type P6OPAQUE, MVMP6opaque
#define Dt9(_V) (int)(ptrdiff_t)&(((MVMP6opaque *)0)_V)
#line 88 "src/jit/x64/emit.dasc"
//|.type P6OBODY, MVMP6opaqueBody
#define DtA(_V) (int)(ptrdiff_t)&(((MVMP6opaqueBody *)0)_V)
#line 89 "src/jit/x64/emit.dasc"
//|.type MVMITER, MVMIter
#define DtB(_V) (int)(ptrdiff_t)&(((MVMIter *)0)_V)
#line 90 "src/jit/x64/emit.dasc"
//|.type MVMINSTANCE, MVMInstance
#define DtC(_V) (int)(ptrdiff_t)&(((MVMInstance *)0)_V)
#line 91 "src/jit/x64/emit.dasc"
//|.type MVMACTIVEHANDLERS, MVMActiveHandler
#define DtD(_V) (int)(ptrdiff_t)&(((MVMActiveHandler *)0)_V)
#line 92 "src/jit/x64/emit.dasc"
//|.type OBJECT, MVMObject
#define DtE(_V) (int)(ptrdiff_t)&(((MVMObject *)0)_V)
#line 93 "src/jit/x64/emit.dasc"
//|.type STOOGE, MVMObjectStooge
#define DtF(_V) (int)(ptrdiff_t)&(((MVMObjectStooge *)0)_V)
#line 94 "src/jit/x64/emit.dasc"
//|.type VMARRAY, MVMArray
#define Dt10(_V) (int)(ptrdiff_t)&(((MVMArray *)0)_V)
#line 95 "src/jit/x64/emit.dasc"
//|.type COLLECTABLE, MVMCollectable
#define Dt11(_V) (int)(ptrdiff_t)&(((MVMCollectable *)0)_V)
#line 96 "src/jit/x64/emit.dasc"
//|.type STABLE, MVMSTable
#define Dt12(_V) (int)(ptrdiff_t)&(((MVMSTable *)0)_V)
#line 97 "src/jit/x64/emit.dasc"
//|.type REPR, MVMREPROps
#define Dt13(_V) (int)(ptrdiff_t)&(((MVMREPROps *)0)_V)
#line 98 "src/jit/x64/emit.dasc"
//|.type STRING, MVMString*
#define Dt14(_V) (int)(ptrdiff_t)&(((MVMString* *)0)_V)
#line 99 "src/jit/x64/emit.dasc"
//|.type OBJECTPTR, MVMObject*
#define Dt15(_V) (int)(ptrdiff_t)&(((MVMObject* *)0)_V)
#line 100 "src/jit/x64/emit.dasc"
//|.type CONTAINERSPEC, MVMContainerSpec
#define Dt16(_V) (int)(ptrdiff_t)&(((MVMContainerSpec *)0)_V)
#line 101 "src/jit/x64/emit.dasc"
//|.type STORAGESPEC, MVMStorageSpec
#define Dt17(_V) (int)(ptrdiff_t)&(((MVMStorageSpec *)0)_V)
#line 102 "src/jit/x64/emit.dasc"
//|.type HLLCONFIG, MVMHLLConfig;
#define Dt18(_V) (int)(ptrdiff_t)&(((MVMHLLConfig *)0)_V)
#line 103 "src/jit/x64/emit.dasc"
//|.type SCREFBODY, MVMSerializationContextBody
#define Dt19(_V) (int)(ptrdiff_t)&(((MVMSerializationContextBody *)0)_V)
#line 104 "src/jit/x64/emit.dasc"
//|.type NFGSYNTH, MVMNFGSynthetic
#define Dt1A(_V) (int)(ptrdiff_t)&(((MVMNFGSynthetic *)0)_V)
#line 105 "src/jit/x64/emit.dasc"
//|.type CODE, MVMCode
#define Dt1B(_V) (int)(ptrdiff_t)&(((MVMCode *)0)_V)
#line 106 "src/jit/x64/emit.dasc"
//|.type U8, MVMuint8
#define Dt1C(_V) (int)(ptrdiff_t)&(((MVMuint8 *)0)_V)
#line 107 "src/jit/x64/emit.dasc"
//|.type U16, MVMuint16
#define Dt1D(_V) (int)(ptrdiff_t)&(((MVMuint16 *)0)_V)
#line 108 "src/jit/x64/emit.dasc"
//|.type U32, MVMuint32
#define Dt1E(_V) (int)(ptrdiff_t)&(((MVMuint32 *)0)_V)
#line 109 "src/jit/x64/emit.dasc"
//|.type U64, MVMuint64
#define Dt1F(_V) (int)(ptrdiff_t)&(((MVMuint64 *)0)_V)
#line 110 "src/jit/x64/emit.dasc"


/* Static allocation of relevant types to registers. I pick
 * callee-save registers for efficiency. It is likely we'll be calling
 * quite a C functions, and this saves us the trouble of storing
 * them. Moreover, C compilers preferentially do not use callee-saved
 * registers, and so in most cases, these won't be touched at all. */
//|.type TC, MVMThreadContext, r14
#define Dt20(_V) (int)(ptrdiff_t)&(((MVMThreadContext *)0)_V)
#line 118 "src/jit/x64/emit.dasc"
/* Alternative base pointer. I'll be using this often, so picking rbx
 * here rather than the extended registers will lead to smaller
 * bytecode */
//|.type WORK, MVMRegister, rbx
#define Dt21(_V) (int)(ptrdiff_t)&(((MVMRegister *)0)_V)
#line 122 "src/jit/x64/emit.dasc"
//|.type CU, MVMCompUnit, r13
#define Dt22(_V) (int)(ptrdiff_t)&(((MVMCompUnit *)0)_V)
#line 123 "src/jit/x64/emit.dasc"




const MVMint32 MVM_jit_support(void) {
#ifdef __i386__
    /* Usually, this file should only be compiled only on a amd64
       platform; but when compiling 'fat' or 'universal' binaries, we
       may compile it for other platform. In this case we use the
       runtime check to disable the JIT */
    return 0;
#else
    return 1;
#endif
}

const unsigned char * MVM_jit_actions(void) {
    return actions;
}

const unsigned int MVM_jit_num_globals(void) {
    return MVM_JIT_LABEL__MAX;
}


/* C Call argument registers */
//|.if WIN32
//|.define ARG1, rcx
//|.define ARG2, rdx
//|.define ARG3, r8
//|.define ARG4, r9
//|.else
//|.define ARG1, rdi
//|.define ARG2, rsi
//|.define ARG3, rdx
//|.define ARG4, rcx
//|.define ARG5, r8
//|.define ARG6, r9
//|.endif

/* C call argument registers for floating point */
//|.if WIN32
//|.define ARG1F, xmm0
//|.define ARG2F, xmm1
//|.define ARG3F, xmm2
//|.define ARG4F, xmm3
//|.else
//|.define ARG1F, xmm0
//|.define ARG2F, xmm1
//|.define ARG3F, xmm2
//|.define ARG4F, xmm3
//|.define ARG5F, xmm4
//|.define ARG6F, xmm5
//|.define ARG7F, xmm6
//|.define ARG8F, xmm7
//|.endif

/* Special register for the function to be invoked
 * (chosen because it isn't involved in argument passing
 *  and volatile) */
//|.define FUNCTION, r10
/* all-purpose temporary registers */
//|.define TMP1, rcx
//|.define TMP2, rdx
//|.define TMP3, r8
//|.define TMP4, r9
//|.define TMP5, r10
//|.define TMP6, r11
/* same, but 32 bits wide */
//|.define TMP1d, ecx
//|.define TMP2d, edx
//|.define TMP3d, r8d
//|.define TMP4d, r9d
//|.define TMP5d, r10d
//|.define TMP6d, r11d
/* and 16 bits wide */
//|.define TMP1w, cx
//|.define TMP2w, dx
//|.define TMP3w, r8w
//|.define TMP4w, r9w
//|.define TMP5w, r10w
//|.define TMP6w, r11w
/* and 8 bits for good measure */
//|.define TMP1b, cl
//|.define TMP2b, dl
//|.define TMP3b, r8b
//|.define TMP4b, r9b
//|.define TMP5b, r10b
//|.define TMP6b, r11b


/* return value */
//|.define RV, rax
//|.define RVd, eax
//|.define RVF, xmm0
//|.define SAVED_RV, [rbp-0x28]


//|.macro callp, funcptr
//|.data
//|5:
//|.dword (MVMuint32)((uintptr_t)(funcptr)), (MVMuint32)((uintptr_t)(funcptr) >> 32);
//|.code
//| call qword [<5];
//|.endmacro


//|.macro check_wb, root, ref, lbl;
//| test word COLLECTABLE:root->flags, MVM_CF_SECOND_GEN;
//| jz lbl;
//| test ref, ref;
//| jz lbl;
//| test word COLLECTABLE:ref->flags, MVM_CF_SECOND_GEN;
//| jnz lbl;
//|.endmacro;

//|.macro hit_wb, obj
//| mov ARG2, obj;
//| mov ARG1, TC;
//| callp &MVM_gc_write_barrier_hit;
//|.endmacro

//|.macro get_spesh_slot, reg, idx;
//| mov reg, TC->cur_frame;
//| mov reg, FRAME:reg->effective_spesh_slots;
//| mov reg, OBJECTPTR:reg[idx];
//|.endmacro

//|.macro get_vmnull, reg
//| mov reg, TC->instance;
//| mov reg, MVMINSTANCE:reg->VMNull;
//|.endmacro

//|.macro get_cur_op, reg
//| mov reg, TC->interp_cur_op
//| mov reg, [reg]
//|.endmacro

//|.macro get_string, reg, idx
//|| MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, idx);
//| mov reg, CU->body.strings;
//| mov reg, STRING:reg[idx];
//|.endmacro

//|.macro is_type_object, reg
//| test word OBJECT:reg->header.flags, MVM_CF_TYPE_OBJECT
//|.endmacro

//|.macro gc_sync_point
//| cmp qword TC->gc_status, 0;
//| je >1;
//| mov ARG1, TC;
//| callp &MVM_gc_enter_from_interrupt;
//|1:
//|.endmacro

//|.macro throw_adhoc, msg
//| mov ARG1, TC;
//| mov64 ARG2, (uintptr_t)(msg);
//| callp &MVM_exception_throw_adhoc;
//|.endmacro

//|.define FRAME_NR, dword [rbp-0x20]

/* A function prologue is always the same in x86 / x64, because
 * we do not provide variable arguments, instead arguments are provided
 * via a frame. All JIT entry points receive a prologue. */

void MVM_jit_emit_prologue(MVMThreadContext *tc, MVMJitCompiler *compiler,
                           MVMJitGraph *jg) {
    //|.code
    dasm_put(Dst, 0);
#line 294 "src/jit/x64/emit.dasc"
    /* Setup stack */
    //| push rbp; // nb, this aligns the stack to 16 bytes again
    //| mov rbp, rsp;
    dasm_put(Dst, 2);
#line 297 "src/jit/x64/emit.dasc"
    /* allocate stack space: 0x100 bytes = 256 bytes
     *
     * layout: [ a: 0x20 | b: 0x40 | c: 0xa0 | d: 0x20 ]
     * a: space for 4 callee-save registers
     * b: small scratch space
     * c: space for stack arguments to c calls
     * d: reserve space for GPR registers to c calls (win64) or more space for
     * stack arguments (posix) */
    //| sub rsp, 0x100;
    dasm_put(Dst, 7);
#line 306 "src/jit/x64/emit.dasc"
    /* save callee-save registers */
    //| mov [rbp-0x8],  TC;
    //| mov [rbp-0x10], CU;
    //| mov [rbp-0x18], WORK;
    dasm_put(Dst, 16);
#line 310 "src/jit/x64/emit.dasc"
    /* store the current frame number for cheap comparisons */
    //| mov TMP6d, dword TC:ARG1->current_frame_nr;
    //| mov FRAME_NR, TMP6d;
    dasm_put(Dst, 32, Dt20(->current_frame_nr));
#line 313 "src/jit/x64/emit.dasc"
    /* setup special frame variables */
    //| mov TC,   ARG1;
    //| mov CU,   ARG2;
    //| mov TMP6, TC->cur_frame;
    //| mov WORK, FRAME:TMP6->work;
    dasm_put(Dst, 41, Dt20(->cur_frame), Dt2(->work));
#line 318 "src/jit/x64/emit.dasc"
    /* ARG3 contains our 'entry label' */
    //| jmp ARG3
    dasm_put(Dst, 58);
#line 320 "src/jit/x64/emit.dasc"
}

/* And a function epilogue is also always the same */
void MVM_jit_emit_epilogue(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg) {
    //| ->exit:
    dasm_put(Dst, 62);
#line 325 "src/jit/x64/emit.dasc"
    /* restore callee-save registers */
    //| mov TC, [rbp-0x8];
    //| mov CU, [rbp-0x10];
    //| mov WORK, [rbp-0x18];
    dasm_put(Dst, 65);
#line 329 "src/jit/x64/emit.dasc"
    /* Restore stack */
    //| mov rsp, rbp;
    //| pop rbp;
    //| ret;
    dasm_put(Dst, 81);
#line 333 "src/jit/x64/emit.dasc"
}

static MVMuint64 try_emit_gen2_ref(MVMThreadContext *tc, MVMJitCompiler *compiler,
                                   MVMJitGraph *jg, MVMObject *obj, MVMint16 reg) {
    if (!(obj->header.flags & MVM_CF_SECOND_GEN))
        return 0;
    //| mov64 TMP1, (uintptr_t)obj;
    //| mov WORK[reg], TMP1;
    dasm_put(Dst, 88, (unsigned int)((uintptr_t)obj), (unsigned int)(((uintptr_t)obj)>>32), Dt21([reg]));
#line 341 "src/jit/x64/emit.dasc"
    return 1;
}

static MVMint64 fits_in_32_bit(MVMint64 number) {
    /* Used to determine if a 64 bit integer can be safely used as a
     * 32 bit constant for immediate mode access */
    return (number >= INT32_MIN) && (number <= INT32_MAX);
}

/* compile per instruction, can't really do any better yet */
void MVM_jit_emit_primitive(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                            MVMJitPrimitive * prim) {
    MVMSpeshIns *ins = prim->ins;
    MVMuint16 op = ins->info->opcode;
    MVM_jit_log(tc, "emit opcode: <%s>\n", ins->info->name);
    /* Quite a few of these opcodes are copies. Ultimately, I want to
     * move copies to their own node (MVMJitCopy or such), and reduce
     * the number of copies (and thereby increase the efficiency), but
     * currently that isn't really feasible. */
    switch (op) {
    case MVM_OP_const_i64_16:
    case MVM_OP_const_i64_32: {
        MVMint32 reg = ins->operands[0].reg.orig;
        /* Upgrade to 64 bit */
        MVMint64 val = (op == MVM_OP_const_i64_16 ? (MVMint64)ins->operands[1].lit_i16 :
                        (MVMint64)ins->operands[1].lit_i32);
        //| mov qword WORK[reg], val;
        dasm_put(Dst, 97, Dt21([reg]), val);
#line 368 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_const_i64: {
        MVMint32 reg = ins->operands[0].reg.orig;
        MVMint64 val = ins->operands[1].lit_i64;
        //| mov64 TMP1, val;
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 88, (unsigned int)(val), (unsigned int)((val)>>32), Dt21([reg]));
#line 375 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_const_n64: {
        MVMint16 reg = ins->operands[0].reg.orig;
        MVMint64 valbytes = ins->operands[1].lit_i64;
        MVM_jit_log(tc, "store const %f\n", ins->operands[1].lit_n64);
        //| mov64 TMP1, valbytes;
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 88, (unsigned int)(valbytes), (unsigned int)((valbytes)>>32), Dt21([reg]));
#line 383 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_inf:
    case MVM_OP_neginf:
    case MVM_OP_nan: {
        MVMint16 reg = ins->operands[0].reg.orig;
        MVMRegister tmp;
        if (op == MVM_OP_nan)
            tmp.n64 = MVM_num_nan(tc);
        else if (op == MVM_OP_inf)
            tmp.n64 = MVM_num_posinf(tc);
        else if (op == MVM_OP_neginf)
            tmp.n64 = MVM_num_neginf(tc);
        //| mov64 TMP1, tmp.i64;
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 88, (unsigned int)(tmp.i64), (unsigned int)((tmp.i64)>>32), Dt21([reg]));
#line 398 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_const_s: {
         MVMint16 reg = ins->operands[0].reg.orig;
         MVMuint32 idx = ins->operands[1].lit_str_idx;
         MVMStaticFrame *sf = jg->sg->sf;
         MVMString * s = MVM_cu_string(tc, sf->body.cu, idx);
         if (!try_emit_gen2_ref(tc, compiler, jg, (MVMObject*)s, reg)) {
             //| get_string TMP1, idx;
              MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, idx);
#line 407 "src/jit/x64/emit.dasc"
             //| mov WORK[reg], TMP1;
             dasm_put(Dst, 103, Dt22(->body.strings), Dt14([idx]), Dt21([reg]));
#line 408 "src/jit/x64/emit.dasc"
         }
         break;
    }
    case MVM_OP_null: {
        MVMint16 reg = ins->operands[0].reg.orig;
        //| get_vmnull TMP1;
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 116, Dt20(->instance), DtC(->VMNull), Dt21([reg]));
#line 415 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_getwhat:
    case MVM_OP_getwho: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[obj];
        //| mov TMP1, OBJECT:TMP1->st;
        dasm_put(Dst, 129, Dt21([obj]), DtE(->st));
#line 423 "src/jit/x64/emit.dasc"
        if (op == MVM_OP_getwho) {
            //| mov TMP1, STABLE:TMP1->WHO;
            //| get_vmnull TMP2;
            //| test TMP1, TMP1;
            //| cmovz TMP1, TMP2;
            dasm_put(Dst, 138, Dt12(->WHO), Dt20(->instance), DtC(->VMNull));
#line 428 "src/jit/x64/emit.dasc"
        } else {
            //| mov TMP1, STABLE:TMP1->WHAT;
            dasm_put(Dst, 133, Dt12(->WHAT));
#line 430 "src/jit/x64/emit.dasc"
        }
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 92, Dt21([dst]));
#line 432 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_getlex:
    case MVM_OP_sp_getlex_o:
    case MVM_OP_sp_getlex_ins: {
        MVMuint16 *lexical_types;
        MVMStaticFrame * sf = jg->sg->sf;
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 idx = ins->operands[1].lex.idx;
        MVMint16 out = ins->operands[1].lex.outers;
        MVMint16 i;
        //| mov TMP6, TC->cur_frame;
        dasm_put(Dst, 158, Dt20(->cur_frame));
#line 444 "src/jit/x64/emit.dasc"
        for (i = 0; i < out; i++) {
            /* I'm going to skip compiling the check whether the outer
             * node really exists, because if the code has run N times
             * correctly, then the outer frame must have existed then,
             * and since this chain is static, it should still exist
             * now.  If it doesn't exist, that means we crash.
             *
             * NB: inlining /might/ make this all wrong! But, if that
             * happens, the interpreter will panic even without JIT */
            //| mov TMP6, FRAME:TMP6->outer;
            dasm_put(Dst, 163, Dt2(->outer));
#line 454 "src/jit/x64/emit.dasc"
            sf = sf->body.outer;
        }
        /* get array of lexicals */
        //| mov TMP5, FRAME:TMP6->env;
        dasm_put(Dst, 168, Dt2(->env));
#line 458 "src/jit/x64/emit.dasc"
        /* read value */
        //| mov TMP5, REGISTER:TMP5[idx];
        dasm_put(Dst, 173, Dt1([idx]));
#line 460 "src/jit/x64/emit.dasc"
        /* it seems that if at runtime, if the outer frame has been inlined,
         * this /could/ be wrong. But if that is so, the interpreted instruction
         * would also be wrong, because it'd refer to the wrong lexical. */
        lexical_types = (!out && jg->sg->lexical_types ?
                         jg->sg->lexical_types :
                         sf->body.lexical_types);
        MVM_jit_log(tc, "Lexical type of register: %d\n", lexical_types[idx]);
        if (lexical_types[idx] == MVM_reg_obj) {
            MVM_jit_log(tc, "Emit lex vifivy check\n");
            /* if it is zero, check if we need to auto-vivify */
            //| test TMP5, TMP5;
            //| jnz >1;
            dasm_put(Dst, 178);
#line 472 "src/jit/x64/emit.dasc"
            /* setup args */
            //| mov ARG1, TC;
            //| mov ARG2, TMP6;
            //| mov ARG3, idx;
            //| callp &MVM_frame_vivify_lexical;
            dasm_put(Dst, 186, idx);
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_frame_vivify_lexical)), (MVMuint32)((uintptr_t)(&MVM_frame_vivify_lexical) >> 32));
            dasm_put(Dst, 205);
#line 477 "src/jit/x64/emit.dasc"
            /* use return value for the result */
            //| mov TMP5, RV;
            //|1:
            dasm_put(Dst, 211);
#line 480 "src/jit/x64/emit.dasc"
        }
        /* store the value */
        //| mov WORK[dst], TMP5;
        dasm_put(Dst, 217, Dt21([dst]));
#line 483 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_getlexvia_o:
    case MVM_OP_sp_getlexvia_ins: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 idx = ins->operands[1].lit_ui16;
        MVMint16 out = ins->operands[2].lit_ui16;
        MVMint16 via = ins->operands[3].reg.orig;
        MVMint16 i;
        /* Resolve the frame. */
        //| mov TMP6, WORK[via];
        //| mov TMP6, CODE:TMP6->body.outer;
        dasm_put(Dst, 222, Dt21([via]), Dt1B(->body.outer));
#line 495 "src/jit/x64/emit.dasc"
        for (i = 1; i < out; i++) /* From 1 as we are already at outer */
            //| mov TMP6, FRAME:TMP6->outer;
            dasm_put(Dst, 163, Dt2(->outer));
#line 497 "src/jit/x64/emit.dasc"
        /* get array of lexicals */
        //| mov TMP5, FRAME:TMP6->env;
        dasm_put(Dst, 168, Dt2(->env));
#line 499 "src/jit/x64/emit.dasc"
        /* read value */
        //| mov TMP5, REGISTER:TMP5[idx];
        dasm_put(Dst, 173, Dt1([idx]));
#line 501 "src/jit/x64/emit.dasc"
        if (op == MVM_OP_sp_getlexvia_o) {
            MVM_jit_log(tc, "Emit lex vifivy check for via code-ref lookup\n");
            /* if it is zero, check if we need to auto-vivify */
            //| test TMP5, TMP5;
            //| jnz >1;
            dasm_put(Dst, 178);
#line 506 "src/jit/x64/emit.dasc"
            /* setup args */
            //| mov ARG1, TC;
            //| mov ARG2, TMP6;
            //| mov ARG3, idx;
            //| callp &MVM_frame_vivify_lexical;
            dasm_put(Dst, 186, idx);
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_frame_vivify_lexical)), (MVMuint32)((uintptr_t)(&MVM_frame_vivify_lexical) >> 32));
            dasm_put(Dst, 205);
#line 511 "src/jit/x64/emit.dasc"
            /* use return value for the result */
            //| mov TMP5, RV;
            //|1:
            dasm_put(Dst, 211);
#line 514 "src/jit/x64/emit.dasc"
        }
        /* store the value */
        //| mov WORK[dst], TMP5;
        dasm_put(Dst, 217, Dt21([dst]));
#line 517 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_getlex_no:
    case MVM_OP_sp_getlex_no: {
        MVMint16  dst = ins->operands[0].reg.orig;
        MVMuint32 idx = ins->operands[1].lit_str_idx;
        //| mov ARG1, TC;
        //| get_string ARG2, idx;
        dasm_put(Dst, 231);
         MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, idx);
#line 525 "src/jit/x64/emit.dasc"
        //| mov ARG3, MVM_reg_obj;
        //| callp &MVM_frame_find_lexical_by_name;
        dasm_put(Dst, 236, Dt22(->body.strings), Dt14([idx]), MVM_reg_obj);
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_frame_find_lexical_by_name)), (MVMuint32)((uintptr_t)(&MVM_frame_find_lexical_by_name) >> 32));
#line 527 "src/jit/x64/emit.dasc"
        //| test RV, RV;
        //| jz >1;
        //| mov RV, [RV];
        //| mov WORK[dst], RV;
        //| jmp >2;
        //|1:
        //| get_vmnull TMP3;
        //| mov WORK[dst], TMP3;
        //|2:
        dasm_put(Dst, 250, Dt21([dst]), Dt20(->instance), DtC(->VMNull), Dt21([dst]));
#line 536 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_bindlex: {
        MVMuint16 *lexical_types;
        MVMStaticFrame *sf = jg->sg->sf;
        MVMint16 idx = ins->operands[0].lex.idx;
        MVMint16 out = ins->operands[0].lex.outers;
        MVMint16 src = ins->operands[1].reg.orig;
        MVMint16 i;
        //| mov TMP1, TC->cur_frame;
        dasm_put(Dst, 290, Dt20(->cur_frame));
#line 546 "src/jit/x64/emit.dasc"
        for (i = 0; i < out; i++) {
            //| mov TMP1, FRAME:TMP1->outer;
            dasm_put(Dst, 133, Dt2(->outer));
#line 548 "src/jit/x64/emit.dasc"
            sf = sf->body.outer;
        }
        lexical_types = (!out && jg->sg->lexical_types ?
                         jg->sg->lexical_types :
                         sf->body.lexical_types);
        //| mov TMP2, FRAME:TMP1->env;
        //| mov TMP3, WORK[src];
        //| mov REGISTER:TMP2[idx], TMP3;
        dasm_put(Dst, 295, Dt2(->env), Dt21([src]), Dt1([idx]));
#line 556 "src/jit/x64/emit.dasc"
        if (lexical_types[idx] == MVM_reg_obj ||
            lexical_types[idx] == MVM_reg_str) {
            //| check_wb TMP1, TMP3, >2;
            //| hit_wb TMP1;
            dasm_put(Dst, 308, Dt11(->flags), MVM_CF_SECOND_GEN, Dt11(->flags), MVM_CF_SECOND_GEN);
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit)), (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit) >> 32));
#line 560 "src/jit/x64/emit.dasc"
            //|2:
            dasm_put(Dst, 345);
#line 561 "src/jit/x64/emit.dasc"
        }
        break;
    }
    case MVM_OP_sp_getarg_o:
    case MVM_OP_sp_getarg_n:
    case MVM_OP_sp_getarg_s:
    case MVM_OP_sp_getarg_i: {
        MVMint32 reg = ins->operands[0].reg.orig;
        MVMuint16 idx = ins->operands[1].callsite_idx;
        //| mov TMP1, TC->cur_frame;
        //| mov TMP1, FRAME:TMP1->params.args;
        //| mov TMP1, REGISTER:TMP1[idx];
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 353, Dt20(->cur_frame), Dt2(->params.args), Dt1([idx]), Dt21([reg]));
#line 574 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_p6oget_i:
    case MVM_OP_sp_p6oget_n:
    case MVM_OP_sp_p6oget_s:
    case MVM_OP_sp_p6oget_o:
    case MVM_OP_sp_p6ogetvc_o:
    case MVM_OP_sp_p6ogetvt_o: {
        MVMint16 dst    = ins->operands[0].reg.orig;
        MVMint16 obj    = ins->operands[1].reg.orig;
        MVMint16 offset = ins->operands[2].lit_i16;
        MVMint16 body   = offsetof(MVMP6opaque, body);
        /* load address and object */
        //| mov TMP1, WORK[obj];
        //| lea TMP2, [TMP1 + (offset + body)];
        //| mov TMP4, P6OPAQUE:TMP1->body.replaced;
        //| lea TMP5, [TMP4 + offset];
        //| test TMP4, TMP4;
        //| cmovnz TMP2, TMP5;
        dasm_put(Dst, 370, Dt21([obj]), (offset + body), Dt9(->body.replaced), offset);
#line 593 "src/jit/x64/emit.dasc"
        /* TMP2 now contains address of item */
        if (op == MVM_OP_sp_p6oget_o) {
            //| mov TMP3, [TMP2];
            //| test TMP3, TMP3;
            dasm_put(Dst, 394);
#line 597 "src/jit/x64/emit.dasc"
            /* Check if object doesn't point to NULL */
            //| jnz >3;
            dasm_put(Dst, 401);
#line 599 "src/jit/x64/emit.dasc"
            /* Otherwise load VMNull */
            //| get_vmnull TMP3;
            //|3:
            dasm_put(Dst, 406, Dt20(->instance), DtC(->VMNull));
#line 602 "src/jit/x64/emit.dasc"
        } else if (op == MVM_OP_sp_p6ogetvt_o) {
            /* vivify as type object */
            MVMint16 spesh_idx = ins->operands[3].lit_i16;
            //| mov TMP3, [TMP2];
            dasm_put(Dst, 417);
#line 606 "src/jit/x64/emit.dasc"
            /* check for null */
            //| test TMP3, TMP3;
            //| jnz >4;
            dasm_put(Dst, 421);
#line 609 "src/jit/x64/emit.dasc"
            /* if null, vivify as type object from spesh slot */
            //| get_spesh_slot TMP3, spesh_idx;
            dasm_put(Dst, 429, Dt20(->cur_frame), Dt2(->effective_spesh_slots), Dt15([spesh_idx]));
#line 611 "src/jit/x64/emit.dasc"
            /* need to hit write barrier? */
            //| check_wb TMP1, TMP3, >3;
            //| mov qword [rbp-0x28], TMP2; // address
            //| mov qword [rbp-0x30], TMP3; // value
            //| hit_wb WORK[obj]; // write barrier for header
            dasm_put(Dst, 442, Dt11(->flags), MVM_CF_SECOND_GEN, Dt11(->flags), MVM_CF_SECOND_GEN, Dt21([obj]));
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit)), (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit) >> 32));
#line 616 "src/jit/x64/emit.dasc"
            //| mov TMP3, qword [rbp-0x30];
            //| mov TMP2, qword [rbp-0x28];
            //|3:
            dasm_put(Dst, 488);
#line 619 "src/jit/x64/emit.dasc"
            /* store vivified type value in memory location */
            //| mov qword [TMP2], TMP3;
            //|4:
            dasm_put(Dst, 504);
#line 622 "src/jit/x64/emit.dasc"
        } else if (op == MVM_OP_sp_p6ogetvc_o) {
            MVMint16 spesh_idx = ins->operands[3].lit_i16;
            //| mov TMP3, [TMP2];
            //| test TMP3, TMP3;
            //| jnz >4;
            dasm_put(Dst, 510);
#line 627 "src/jit/x64/emit.dasc"
            /* vivify as clone */
            //| mov ARG1, TC;
            //| get_spesh_slot ARG2, spesh_idx;
            //| callp &MVM_repr_clone;
            dasm_put(Dst, 521, Dt20(->cur_frame), Dt2(->effective_spesh_slots), Dt15([spesh_idx]));
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_repr_clone)), (MVMuint32)((uintptr_t)(&MVM_repr_clone) >> 32));
#line 631 "src/jit/x64/emit.dasc"
            //| mov TMP3, RV;
            dasm_put(Dst, 539);
#line 632 "src/jit/x64/emit.dasc"
            /* reload object and address */
            //| mov TMP1, WORK[obj];
            //| lea TMP2, [TMP1 + (offset + body)];
            //| mov TMP4, P6OPAQUE:TMP1->body.replaced;
            //| lea TMP5, [TMP4 + offset];
            //| test TMP4, TMP4;
            //| cmovnz TMP2, TMP5;
            dasm_put(Dst, 370, Dt21([obj]), (offset + body), Dt9(->body.replaced), offset);
#line 639 "src/jit/x64/emit.dasc"
            /* assign with write barrier */
            //| check_wb TMP1, TMP3, >3;
            //| mov qword [rbp-0x28], TMP2; // address
            //| mov qword [rbp-0x30], TMP3; // value
            //| hit_wb WORK[obj]; // write barrier for header
            dasm_put(Dst, 442, Dt11(->flags), MVM_CF_SECOND_GEN, Dt11(->flags), MVM_CF_SECOND_GEN, Dt21([obj]));
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit)), (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit) >> 32));
#line 644 "src/jit/x64/emit.dasc"
            //| mov TMP3, qword [rbp-0x30];
            //| mov TMP2, qword [rbp-0x28];
            //|3:
            //| mov qword [TMP2], TMP3;
            dasm_put(Dst, 548);
#line 648 "src/jit/x64/emit.dasc"
            /* done */
            //|4:
            dasm_put(Dst, 507);
#line 650 "src/jit/x64/emit.dasc"
        } else {
            /* the regular case */
            //| mov TMP3, [TMP2];
            dasm_put(Dst, 417);
#line 653 "src/jit/x64/emit.dasc"
        }
        /* store in local register */
        //| mov WORK[dst], TMP3;
        dasm_put(Dst, 567, Dt21([dst]));
#line 656 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_bind_i64:
    case MVM_OP_sp_bind_n:
    case MVM_OP_sp_bind_s:
    case MVM_OP_sp_bind_o: {
        MVMint16 obj    = ins->operands[0].reg.orig;
        MVMint16 offset = ins->operands[1].lit_i16;
        MVMint16 val    = ins->operands[2].reg.orig;
        //| mov TMP1, WORK[obj];            // object
        //| mov TMP2, WORK[val];            // value
        dasm_put(Dst, 572, Dt21([obj]), Dt21([val]));
#line 667 "src/jit/x64/emit.dasc"
        if (op == MVM_OP_sp_bind_o || op == MVM_OP_sp_bind_s) {
            /* check if we should hit write barrier */
            //| check_wb TMP1, TMP2, >2;
            dasm_put(Dst, 581, Dt11(->flags), MVM_CF_SECOND_GEN, Dt11(->flags), MVM_CF_SECOND_GEN);
#line 670 "src/jit/x64/emit.dasc"
            /* note: it is uneccesary to store pointers, because they
               can just be loaded from memory */
            //| hit_wb WORK[obj];
            dasm_put(Dst, 478, Dt21([obj]));
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit)), (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit) >> 32));
#line 673 "src/jit/x64/emit.dasc"
            //| mov TMP1, aword WORK[obj]; // reload object
            //| mov TMP2, aword WORK[val]; // reload value
            //|2: // done
            dasm_put(Dst, 609, Dt21([obj]), Dt21([val]));
#line 676 "src/jit/x64/emit.dasc"
        }
        //| mov qword [TMP1+offset], TMP2; // store value into body
        dasm_put(Dst, 625, offset);
#line 678 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_get_i64:
    case MVM_OP_sp_get_n:
    case MVM_OP_sp_get_s:
    case MVM_OP_sp_get_o: {
        MVMint16 dst    = ins->operands[0].reg.orig;
        MVMint16 obj    = ins->operands[1].reg.orig;
        MVMint16 offset = ins->operands[2].lit_i16;
        //| mov TMP1, WORK[obj];            // object
        //| mov TMP2, qword [TMP1+offset];   // get value from body
        //| mov WORK[dst], TMP2;
        dasm_put(Dst, 630, Dt21([obj]), offset, Dt21([dst]));
#line 690 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_deref_bind_i64:
    case MVM_OP_sp_deref_bind_n: {
        MVMint16 obj    = ins->operands[0].reg.orig;
        MVMint16 val    = ins->operands[1].reg.orig;
        MVMint16 offset = ins->operands[2].lit_i16;
        //| mov TMP1, WORK[obj];            // object
        //| mov TMP2, WORK[val];            // value
        //| mov TMP1, qword [TMP1+offset];  // find address of target
        //| mov qword [TMP1], TMP2;
        dasm_put(Dst, 643, Dt21([obj]), Dt21([val]), offset);
#line 701 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_deref_get_i64:
    case MVM_OP_sp_deref_get_n: {
        MVMint16 dst    = ins->operands[0].reg.orig;
        MVMint16 obj    = ins->operands[1].reg.orig;
        MVMint16 offset = ins->operands[2].lit_i16;
        //| mov TMP1, WORK[obj];            // object
        //| mov TMP3, qword [TMP1+offset];  // get value pointer from body
        //| mov TMP2, qword [TMP3];         // deref the pointer
        //| mov WORK[dst], TMP2;
        dasm_put(Dst, 659, Dt21([obj]), offset, Dt21([dst]));
#line 712 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_p6obind_i:
    case MVM_OP_sp_p6obind_n:
    case MVM_OP_sp_p6obind_s:
    case MVM_OP_sp_p6obind_o: {
        MVMint16 obj    = ins->operands[0].reg.orig;
        MVMint16 offset = ins->operands[1].lit_i16;
        MVMint16 val    = ins->operands[2].reg.orig;
        //| mov TMP1, WORK[obj];            // object
        //| mov TMP2, WORK[val];            // value
        //| lea TMP3, P6OPAQUE:TMP1->body;  // body
        //| cmp qword P6OBODY:TMP3->replaced, 0;
        //| je >1;
        //| mov TMP3, P6OBODY:TMP3->replaced; // replaced object body
        //|1:
        dasm_put(Dst, 675, Dt21([obj]), Dt21([val]), Dt9(->body), DtA(->replaced), DtA(->replaced));
#line 728 "src/jit/x64/emit.dasc"
        if (op == MVM_OP_sp_p6obind_o || op == MVM_OP_sp_p6obind_s) {
            /* check if we should hit write barrier */
            //| check_wb TMP1, TMP2, >2;
            //| mov qword [rbp-0x28], TMP2; // store value
            //| mov qword [rbp-0x30], TMP3; // store body pointer
            //| hit_wb WORK[obj];
            dasm_put(Dst, 703, Dt11(->flags), MVM_CF_SECOND_GEN, Dt11(->flags), MVM_CF_SECOND_GEN, Dt21([obj]));
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit)), (MVMuint32)((uintptr_t)(&MVM_gc_write_barrier_hit) >> 32));
#line 734 "src/jit/x64/emit.dasc"
            //| mov TMP3, qword [rbp-0x30]; // restore body pointer
            //| mov TMP2, qword [rbp-0x28]; // restore value
            //|2: // done
            dasm_put(Dst, 748);
#line 737 "src/jit/x64/emit.dasc"
        }
        //| mov [TMP3+offset], TMP2; // store value into body
        dasm_put(Dst, 764, offset);
#line 739 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_getwhere:
    case MVM_OP_set: {
         MVMint32 reg1 = ins->operands[0].reg.orig;
         MVMint32 reg2 = ins->operands[1].reg.orig;
         //| mov TMP1, WORK[reg2];
         //| mov WORK[reg1], TMP1;
         dasm_put(Dst, 769, Dt21([reg2]), Dt21([reg1]));
#line 747 "src/jit/x64/emit.dasc"
         break;
    }
    case MVM_OP_sp_getspeshslot: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 spesh_idx = ins->operands[1].lit_i16;
        //| get_spesh_slot TMP1, spesh_idx;
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 353, Dt20(->cur_frame), Dt2(->effective_spesh_slots), Dt15([spesh_idx]), Dt21([dst]));
#line 754 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_setdispatcher: {
        MVMint16 src = ins->operands[0].reg.orig;
        //| mov TMP1, aword WORK[src];
        //| mov aword TC->cur_dispatcher, TMP1;
        dasm_put(Dst, 778, Dt21([src]), Dt20(->cur_dispatcher));
#line 760 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_takedispatcher: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, aword TC->cur_dispatcher;
        //| cmp TMP1, 0;
        //| je >2;
        //| mov TMP2, aword TC->cur_dispatcher_for;
        //| cmp TMP2, 0;
        //| je >1;
        //| mov TMP3, TC->cur_frame;
        //| mov TMP3, FRAME:TMP3->code_ref;
        //| cmp TMP2, TMP3;
        //| jne >2;
        //|1:
        //| mov aword WORK[dst], TMP1;
        //| mov aword TC->cur_dispatcher, NULL;
        //| jmp >3;
        //|2:
        //| get_vmnull TMP1;
        //| mov aword WORK[dst], TMP1;
        //|3:
        dasm_put(Dst, 787, Dt20(->cur_dispatcher), Dt20(->cur_dispatcher_for), Dt20(->cur_frame), Dt2(->code_ref), Dt21([dst]), Dt20(->cur_dispatcher), NULL, Dt20(->instance), DtC(->VMNull), Dt21([dst]));
#line 782 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_curcode: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, TC->cur_frame;
        //| mov TMP1, aword FRAME:TMP1->code_ref;
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 116, Dt20(->cur_frame), Dt2(->code_ref), Dt21([dst]));
#line 789 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_getcode: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMuint16 idx = ins->operands[1].coderef_idx;
        //| mov TMP1, aword CU->body.coderefs;
        //| mov TMP1, aword OBJECTPTR:TMP1[idx];
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 103, Dt22(->body.coderefs), Dt15([idx]), Dt21([dst]));
#line 797 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_callercode: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, TC->cur_frame;
        //| mov TMP1, aword FRAME:TMP1->caller;
        //| test TMP1, TMP1;
        //| jz >1;
        //| mov TMP1, aword FRAME:TMP1->code_ref;
        //|1:
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 860, Dt20(->cur_frame), Dt2(->caller), Dt2(->code_ref), Dt21([dst]));
#line 808 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_hllboxtype_n:
    case MVM_OP_hllboxtype_s:
    case MVM_OP_hllboxtype_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, CU->body.hll_config;
        dasm_put(Dst, 886, Dt22(->body.hll_config));
#line 815 "src/jit/x64/emit.dasc"
        if (op == MVM_OP_hllboxtype_n) {
            //| mov TMP1, aword HLLCONFIG:TMP1->num_box_type;
            dasm_put(Dst, 133, Dt18(->num_box_type));
#line 817 "src/jit/x64/emit.dasc"
        } else if (op == MVM_OP_hllboxtype_s) {
            //| mov TMP1, aword HLLCONFIG:TMP1->str_box_type;
            dasm_put(Dst, 133, Dt18(->str_box_type));
#line 819 "src/jit/x64/emit.dasc"
        } else {
            //| mov TMP1, aword HLLCONFIG:TMP1->int_box_type;
            dasm_put(Dst, 133, Dt18(->int_box_type));
#line 821 "src/jit/x64/emit.dasc"
        }
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 92, Dt21([dst]));
#line 823 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_null_s: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov qword WORK[dst], 0;
        dasm_put(Dst, 891, Dt21([dst]));
#line 828 "src/jit/x64/emit.dasc"
        break;
     }
    case MVM_OP_isnull_s: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[src];
        //| test TMP1, TMP1;
        //| setz TMP2b;
        //| movzx TMP2, TMP2b;
        //| mov qword WORK[dst], TMP2;
        dasm_put(Dst, 900, Dt21([src]), Dt21([dst]));
#line 838 "src/jit/x64/emit.dasc"
        break;
    }

    case MVM_OP_add_i:
    case MVM_OP_sub_i:
    case MVM_OP_bor_i:
    case MVM_OP_band_i:
    case MVM_OP_bxor_i: {
        MVMint32 reg_a = ins->operands[0].reg.orig;
        MVMint32 reg_b = ins->operands[1].reg.orig;
        MVMint32 reg_c = ins->operands[2].reg.orig;

        MVMSpeshFacts *operand_facts = MVM_spesh_get_facts(tc, jg->sg, ins->operands[2]);

        if (reg_a == reg_b) {
            if (operand_facts->flags & MVM_SPESH_FACT_KNOWN_VALUE &&
                fits_in_32_bit(operand_facts->value.i)) {
                MVMint64 value = operand_facts->value.i;
                MVM_jit_log(tc, "accumulator for %s stayed in memory and "
                            " constant value %"PRId64" used\n", ins->info->name, value);
                switch(ins->info->opcode) {
                case MVM_OP_add_i:
                    //| add qword WORK[reg_a], qword value;
                    dasm_put(Dst, 919, Dt21([reg_a]), value);
#line 861 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_sub_i:
                    //| sub qword WORK[reg_a], qword value;
                    dasm_put(Dst, 925, Dt21([reg_a]), value);
#line 864 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_bor_i:
                    //| or qword WORK[reg_a], qword value;
                    dasm_put(Dst, 931, Dt21([reg_a]), value);
#line 867 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_band_i:
                    //| and qword WORK[reg_a], qword value;
                    dasm_put(Dst, 937, Dt21([reg_a]), value);
#line 870 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_bxor_i:
                    //| xor qword WORK[reg_a], qword value;
                    dasm_put(Dst, 943, Dt21([reg_a]), value);
#line 873 "src/jit/x64/emit.dasc"
                    break;
                }
            } else {
                MVM_jit_log(tc, "accumulator for %s stayed in memory\n", ins->info->name);
                //| mov rax, WORK[reg_c];
                dasm_put(Dst, 949, Dt21([reg_c]));
#line 878 "src/jit/x64/emit.dasc"
                switch(ins->info->opcode) {
                case MVM_OP_add_i:
                    //| add WORK[reg_a], rax;
                    dasm_put(Dst, 954, Dt21([reg_a]));
#line 881 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_sub_i:
                    //| sub WORK[reg_a], rax;
                    dasm_put(Dst, 959, Dt21([reg_a]));
#line 884 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_bor_i:
                    //| or WORK[reg_a], rax;
                    dasm_put(Dst, 964, Dt21([reg_a]));
#line 887 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_band_i:
                    //| and WORK[reg_a], rax;
                    dasm_put(Dst, 969, Dt21([reg_a]));
#line 890 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_bxor_i:
                    //| xor WORK[reg_a], rax;
                    dasm_put(Dst, 974, Dt21([reg_a]));
#line 893 "src/jit/x64/emit.dasc"
                    break;
                }
            }
        } else {
            if (operand_facts->flags & MVM_SPESH_FACT_KNOWN_VALUE &&
                fits_in_32_bit(operand_facts->value.i)) {
                MVMint64 value = operand_facts->value.i;
                MVM_jit_log(tc, "constant value %"PRId64" used for %s\n",
                            value, ins->info->name);
                //| mov rax, WORK[reg_b];
                dasm_put(Dst, 949, Dt21([reg_b]));
#line 903 "src/jit/x64/emit.dasc"
                switch(ins->info->opcode) {
                case MVM_OP_add_i:
                    //| add rax, qword value;
                    dasm_put(Dst, 979, value);
#line 906 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_sub_i:
                    //| sub rax, qword value;
                    dasm_put(Dst, 984, value);
#line 909 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_bor_i:
                    //| or rax, qword value;
                    dasm_put(Dst, 990, value);
#line 912 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_band_i:
                    //| and rax, qword value;
                    dasm_put(Dst, 995, value);
#line 915 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_bxor_i:
                    //| xor rax, qword value;
                    dasm_put(Dst, 1000, value);
#line 918 "src/jit/x64/emit.dasc"
                    break;
                }
                //| mov WORK[reg_a], rax;
                dasm_put(Dst, 1006, Dt21([reg_a]));
#line 921 "src/jit/x64/emit.dasc"
            } else {
                //| mov rax, WORK[reg_b];
                dasm_put(Dst, 949, Dt21([reg_b]));
#line 923 "src/jit/x64/emit.dasc"
                switch(ins->info->opcode) {
                case MVM_OP_add_i:
                    //| add rax, WORK[reg_c];
                    dasm_put(Dst, 1011, Dt21([reg_c]));
#line 926 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_sub_i:
                    //| sub rax, WORK[reg_c];
                    dasm_put(Dst, 1016, Dt21([reg_c]));
#line 929 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_bor_i:
                    //| or rax, WORK[reg_c];
                    dasm_put(Dst, 1021, Dt21([reg_c]));
#line 932 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_band_i:
                    //| and rax, WORK[reg_c];
                    dasm_put(Dst, 1026, Dt21([reg_c]));
#line 935 "src/jit/x64/emit.dasc"
                    break;
                case MVM_OP_bxor_i:
                    //| xor rax, WORK[reg_c];
                    dasm_put(Dst, 1031, Dt21([reg_c]));
#line 938 "src/jit/x64/emit.dasc"
                    break;
                }
                //| mov WORK[reg_a], rax;
                dasm_put(Dst, 1006, Dt21([reg_a]));
#line 941 "src/jit/x64/emit.dasc"
            }
        }
        break;
    }
    case MVM_OP_mul_i:
    case MVM_OP_blshift_i:
    case MVM_OP_brshift_i: {
        MVMint32 reg_a = ins->operands[0].reg.orig;
        MVMint32 reg_b = ins->operands[1].reg.orig;
        MVMint32 reg_c = ins->operands[2].reg.orig;
        //| mov rax, WORK[reg_b];
        dasm_put(Dst, 949, Dt21([reg_b]));
#line 952 "src/jit/x64/emit.dasc"
        switch(ins->info->opcode) {
        case MVM_OP_mul_i:
            //| imul rax, WORK[reg_c];
            dasm_put(Dst, 1036, Dt21([reg_c]));
#line 955 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_blshift_i:
            //| mov cl, byte WORK[reg_c];
            //| shl rax, cl;
            dasm_put(Dst, 1042, Dt21([reg_c]));
#line 959 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_brshift_i:
            //| mov cl, byte WORK[reg_c];
            //| shr rax, cl;
            dasm_put(Dst, 1049, Dt21([reg_c]));
#line 963 "src/jit/x64/emit.dasc"
            break;
        }
        //| mov WORK[reg_a], rax;
        dasm_put(Dst, 1006, Dt21([reg_a]));
#line 966 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_div_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 a   = ins->operands[1].reg.orig;
        MVMint16 b   = ins->operands[2].reg.orig;
        //| mov rax, WORK[a];
        //| mov rcx, WORK[b];
        //| cmp rcx, 0;
        //| jnz >1;
        //| throw_adhoc "Division by zero";
        dasm_put(Dst, 1057, Dt21([a]), Dt21([b]), (unsigned int)((uintptr_t)("Division by zero")), (unsigned int)(((uintptr_t)("Division by zero"))>>32));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc)), (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc) >> 32));
#line 977 "src/jit/x64/emit.dasc"
        //|1:
        dasm_put(Dst, 1084);
#line 978 "src/jit/x64/emit.dasc"
        /* either num < 0, or denom < 0, but not both */
        //| setl dh;
        //| cmp rax, 0;
        //| setl dl;
        //| xor dl, dh;
        //| movzx r8d, dl;
        //| cqo;
        //| idiv rcx;
        //| test rdx, rdx;
        //| setnz cl;
        //| and r8b, cl;
        dasm_put(Dst, 1092);
#line 989 "src/jit/x64/emit.dasc"
        /* r8 = bias = (modulo != 0) & ((num < 0) ^ (denom < 0)) */
        //| sub rax, r8;
        //| mov WORK[dst], rax;
        dasm_put(Dst, 1127, Dt21([dst]));
#line 992 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_mod_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 a   = ins->operands[1].reg.orig;
        MVMint16 b   = ins->operands[2].reg.orig;
        //| mov rax, WORK[a];
        //| mov rcx, WORK[b];
        //| cmp rcx, 0;
        //| jnz >1;
        //| throw_adhoc "Division by zero";
        dasm_put(Dst, 1057, Dt21([a]), Dt21([b]), (unsigned int)((uintptr_t)("Division by zero")), (unsigned int)(((uintptr_t)("Division by zero"))>>32));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc)), (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc) >> 32));
#line 1003 "src/jit/x64/emit.dasc"
        //|1:
        //| cqo;
        //| idiv rcx;
        //| mov WORK[dst], rdx;
        dasm_put(Dst, 1135, Dt21([dst]));
#line 1007 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_inc_i: {
         MVMint32 reg = ins->operands[0].reg.orig;
         //| add qword WORK[reg], 1;
         dasm_put(Dst, 1154, Dt21([reg]));
#line 1012 "src/jit/x64/emit.dasc"
         break;
    }
    case MVM_OP_dec_i: {
        MVMint32 reg = ins->operands[0].reg.orig;
        //| sub qword WORK[reg], 1;
        dasm_put(Dst, 1160, Dt21([reg]));
#line 1017 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_bnot_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[src];
        //| not TMP1;
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 1166, Dt21([src]), Dt21([dst]));
#line 1025 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_neg_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[src];
        //| neg TMP1;
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 1179, Dt21([src]), Dt21([dst]));
#line 1033 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_extend_i32: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov RV, WORK[src];
        //| cdqe
        //| mov WORK[dst], RV;
        dasm_put(Dst, 1192, Dt21([src]), Dt21([dst]));
#line 1041 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_trunc_i32: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[src];
        dasm_put(Dst, 1203, Dt21([src]));
#line 1047 "src/jit/x64/emit.dasc"
        /* x86-64 auto-truncates the upper 32 bits when using registers in 32
         * bit mode */
        //| mov TMP2d, TMP1d;
        //| mov WORK[dst], TMP2;
        dasm_put(Dst, 1208, Dt21([dst]));
#line 1051 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_add_n:
    case MVM_OP_sub_n:
    case MVM_OP_mul_n:
    case MVM_OP_div_n: {
        MVMint16 reg_a = ins->operands[0].reg.orig;
        MVMint16 reg_b = ins->operands[1].reg.orig;
        MVMint16 reg_c = ins->operands[2].reg.orig;
        /* Copying data to xmm (floating point) registers requires
         * a special move instruction */
        //| movsd xmm0, qword WORK[reg_b];
        dasm_put(Dst, 1215, Dt21([reg_b]));
#line 1063 "src/jit/x64/emit.dasc"
        switch(ins->info->opcode) {
        case MVM_OP_add_n:
            //| addsd xmm0, qword WORK[reg_c];
            dasm_put(Dst, 1222, Dt21([reg_c]));
#line 1066 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_sub_n:
            //| subsd xmm0, qword WORK[reg_c];
            dasm_put(Dst, 1229, Dt21([reg_c]));
#line 1069 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_mul_n:
            //| mulsd xmm0, qword WORK[reg_c];
            dasm_put(Dst, 1236, Dt21([reg_c]));
#line 1072 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_div_n:
            //| divsd xmm0, qword WORK[reg_c];
            dasm_put(Dst, 1243, Dt21([reg_c]));
#line 1075 "src/jit/x64/emit.dasc"
            break;
        }
        //| movsd qword WORK[reg_a], xmm0;
        dasm_put(Dst, 1250, Dt21([reg_a]));
#line 1078 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_coerce_in: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        /* convert simple integer to double precision */
        //| cvtsi2sd xmm0, qword WORK[src];
        //| movsd qword WORK[dst], xmm0;
        dasm_put(Dst, 1257, Dt21([src]), Dt21([dst]));
#line 1086 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_coerce_ni: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        /* convert double precision to simple intege */
        //| cvttsd2si rax, qword WORK[src];
        //| mov WORK[dst], rax;
        dasm_put(Dst, 1271, Dt21([src]), Dt21([dst]));
#line 1094 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_neg_n: {
        /* Negation is xor-ing the highest byte. Pretty simple right */
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP1, 1;
        //| sal TMP1, 63;
        //| mov TMP2, qword WORK[src];
        //| xor TMP2, TMP1;
        //| mov qword WORK[dst], TMP2;
        dasm_put(Dst, 1283, Dt21([src]), Dt21([dst]));
#line 1105 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_eq_i:
    case MVM_OP_eqaddr:
    case MVM_OP_ne_i:
    case MVM_OP_lt_i:
    case MVM_OP_le_i:
    case MVM_OP_gt_i:
    case MVM_OP_ge_i: {
        MVMint32 reg_a = ins->operands[0].reg.orig;
        MVMint32 reg_b = ins->operands[1].reg.orig;
        MVMint32 reg_c = ins->operands[2].reg.orig;
        //| mov rax, WORK[reg_b];
        dasm_put(Dst, 949, Dt21([reg_b]));
#line 1118 "src/jit/x64/emit.dasc"
        /* comparison result in the setting bits in the rflags register */
        //| cmp rax, WORK[reg_c];
        dasm_put(Dst, 1306, Dt21([reg_c]));
#line 1120 "src/jit/x64/emit.dasc"
        /* copy the right comparison bit to the lower byte of the rax
           register */
        switch(ins->info->opcode) {
        case MVM_OP_eqaddr:
        case MVM_OP_eq_i:
            //| sete al;
            dasm_put(Dst, 1311);
#line 1126 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_ne_i:
            //| setne al;
            dasm_put(Dst, 1315);
#line 1129 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_lt_i:
            //| setl al;
            dasm_put(Dst, 1319);
#line 1132 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_le_i:
            //| setle al;
            dasm_put(Dst, 1323);
#line 1135 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_gt_i:
            //| setg al;
            dasm_put(Dst, 1327);
#line 1138 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_ge_i:
            //| setge al;
            dasm_put(Dst, 1331);
#line 1141 "src/jit/x64/emit.dasc"
            break;
        }
        /* zero extend al (lower byte) to rax (whole register) */
        //| movzx rax, al;
        //| mov WORK[reg_a], rax;
        dasm_put(Dst, 1335, Dt21([reg_a]));
#line 1146 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_cmp_i : {
        MVMint32 reg_a = ins->operands[0].reg.orig;
        MVMint32 reg_b = ins->operands[1].reg.orig;
        MVMint32 reg_c = ins->operands[2].reg.orig;
        //| mov TMP1, WORK[reg_b];
        dasm_put(Dst, 1203, Dt21([reg_b]));
#line 1153 "src/jit/x64/emit.dasc"
        /* comparison result in the setting bits in the rflags register */
        //| cmp TMP1, WORK[reg_c];
        dasm_put(Dst, 1344, Dt21([reg_c]));
#line 1155 "src/jit/x64/emit.dasc"
        /* copy the right comparison bit to the lower byte of the rax
           register */
        //| setg TMP2b;
        //| movzx TMP2, TMP2b;
        //| setl TMP3b;
        //| movzx TMP3, TMP3b;
        //| sub TMP2, TMP3;
        //| mov WORK[reg_a], TMP2;
        dasm_put(Dst, 1349, Dt21([reg_a]));
#line 1163 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_gt_s:
    case MVM_OP_ge_s:
    case MVM_OP_lt_s:
    case MVM_OP_le_s: {
        /* src/jit/graph.c already put a call to the MVM_string_compare
           function into the graph, so here we just have to deal with the
           returned integers. */
        MVMint32 reg = ins->operands[0].reg.orig;
        switch(ins->info->opcode) {
            case MVM_OP_gt_s:
                //| mov TMP2, 1;
                dasm_put(Dst, 1372);
#line 1176 "src/jit/x64/emit.dasc"
                break;
            case MVM_OP_lt_s:
                //| mov TMP2, -1;
                dasm_put(Dst, 1380);
#line 1179 "src/jit/x64/emit.dasc"
                break;
            case MVM_OP_ge_s: case MVM_OP_le_s:
                //| mov TMP2, 0;
                dasm_put(Dst, 1392);
#line 1182 "src/jit/x64/emit.dasc"
                break;
        }
        //| cmp TMP2, WORK[reg];
        dasm_put(Dst, 1400, Dt21([reg]));
#line 1185 "src/jit/x64/emit.dasc"
        /* Mind the reversedness of the constant and the value as compared to
         * interp.c */
        switch(ins->info->opcode) {
            case MVM_OP_gt_s:
                //| sete al;
                dasm_put(Dst, 1311);
#line 1190 "src/jit/x64/emit.dasc"
                break;
            case MVM_OP_ge_s:
                //| setle al;
                dasm_put(Dst, 1323);
#line 1193 "src/jit/x64/emit.dasc"
                break;
            case MVM_OP_lt_s:
                //| sete al;
                dasm_put(Dst, 1311);
#line 1196 "src/jit/x64/emit.dasc"
                break;
            case MVM_OP_le_s:
                //| setge al;
                dasm_put(Dst, 1331);
#line 1199 "src/jit/x64/emit.dasc"
                break;
        }
        //| movzx rax, al;
        //| mov WORK[reg], rax;
        dasm_put(Dst, 1335, Dt21([reg]));
#line 1203 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_not_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[src];
        //| test TMP1, TMP1;
        //| setz TMP2b;
        //| movzx TMP2, TMP2b;
        //| mov WORK[dst], TMP2;
        dasm_put(Dst, 900, Dt21([src]), Dt21([dst]));
#line 1213 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_eq_n:
    case MVM_OP_ne_n:
    case MVM_OP_le_n:
    case MVM_OP_lt_n:
    case MVM_OP_ge_n:
    case MVM_OP_gt_n: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 a   = ins->operands[1].reg.orig;
        MVMint16 b   = ins->operands[2].reg.orig;
        if (op == MVM_OP_eq_n) {
            //| mov TMP1, 0;
            dasm_put(Dst, 1405);
#line 1226 "src/jit/x64/emit.dasc"
        } else if (op == MVM_OP_ne_n) {
            //| mov TMP1, 1;
            dasm_put(Dst, 1413);
#line 1228 "src/jit/x64/emit.dasc"
        }
        if (op == MVM_OP_lt_n || op == MVM_OP_le_n) {
            //| movsd xmm0, qword WORK[b];
            //| ucomisd xmm0, qword WORK[a];
            dasm_put(Dst, 1421, Dt21([b]), Dt21([a]));
#line 1232 "src/jit/x64/emit.dasc"
        } else {
            //| movsd xmm0, qword WORK[a];
            //| ucomisd xmm0, qword WORK[b];
            dasm_put(Dst, 1421, Dt21([a]), Dt21([b]));
#line 1235 "src/jit/x64/emit.dasc"
        }

        if (op == MVM_OP_le_n || op == MVM_OP_ge_n) {
            //| setae TMP1b;
            dasm_put(Dst, 1433);
#line 1239 "src/jit/x64/emit.dasc"
        } else if (op == MVM_OP_eq_n) {
            //| setnp TMP2b; // zero if either is NaN, 1 otherwise
            //| cmove TMP1, TMP2; // if equal, overwrite 0 with 1
            dasm_put(Dst, 1437);
#line 1242 "src/jit/x64/emit.dasc"
        } else if (op == MVM_OP_ne_n) {
            //| setp TMP2b; // 1 if either is NaN (in which case they can't be equal)
            //| cmove TMP1, TMP2; // if equal, overwrite 1 with IsNan(a) | IsNaN(b)
            dasm_put(Dst, 1445);
#line 1245 "src/jit/x64/emit.dasc"
        } else {
            //| seta TMP1b;
            dasm_put(Dst, 1453);
#line 1247 "src/jit/x64/emit.dasc"
        }
        //| movzx TMP1, TMP1b;
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 1457, Dt21([dst]));
#line 1250 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_cmp_n: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 a   = ins->operands[1].reg.orig;
        MVMint16 b   = ins->operands[2].reg.orig;
        //| movsd xmm0, qword WORK[a];
        //| movsd xmm1, qword WORK[b];
        //| ucomisd xmm0, xmm1
        //| seta TMP1b;
        //| movzx rax, TMP1b;
        //| ucomisd xmm1, xmm0
        //| seta TMP1b;
        //| movzx TMP1, TMP1b;
        //| sub rax, TMP1;
        //| mov WORK[dst], rax;
        dasm_put(Dst, 1466, Dt21([a]), Dt21([b]), Dt21([dst]));
#line 1266 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_eq_I:
    case MVM_OP_ne_I:
    case MVM_OP_lt_I:
    case MVM_OP_le_I:
    case MVM_OP_gt_I:
    case MVM_OP_ge_I: {
        MVMint32 reg_a = ins->operands[0].reg.orig;
        MVMint32 reg_b = ins->operands[1].reg.orig;
        MVMint32 reg_c = ins->operands[2].reg.orig;
        /* Call the bigint comparison function. */
        //| mov ARG1, tc;
        //| mov ARG2, WORK[reg_b];
        //| mov ARG3, WORK[reg_c];
        //| callp &MVM_bigint_cmp;
        dasm_put(Dst, 1508, tc, Dt21([reg_b]), Dt21([reg_c]));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_bigint_cmp)), (MVMuint32)((uintptr_t)(&MVM_bigint_cmp) >> 32));
        dasm_put(Dst, 205);
#line 1282 "src/jit/x64/emit.dasc"
        /* Handle result by opcode. */
        switch(ins->info->opcode) {
        case MVM_OP_eq_I:
            //| cmp RV, MP_EQ
            //| sete al;
            dasm_put(Dst, 1522, MP_EQ);
#line 1287 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_ne_I:
            //| cmp RV, MP_EQ
            //| setne al;
            dasm_put(Dst, 1531, MP_EQ);
#line 1291 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_lt_I:
            //| cmp RV, MP_LT
            //| sete al;
            dasm_put(Dst, 1522, MP_LT);
#line 1295 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_le_I:
            //| cmp RV, MP_GT
            //| setne al;
            dasm_put(Dst, 1531, MP_GT);
#line 1299 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_gt_I:
            //| cmp RV, MP_GT
            //| sete al;
            dasm_put(Dst, 1522, MP_GT);
#line 1303 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_ge_I:
            //| cmp RV, MP_LT
            //| setne al;
            dasm_put(Dst, 1531, MP_LT);
#line 1307 "src/jit/x64/emit.dasc"
            break;
        }
        /* zero extend al (lower byte) to rax (whole register) */
        //| movzx rax, al;
        //| mov WORK[reg_a], rax;
        dasm_put(Dst, 1335, Dt21([reg_a]));
#line 1312 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_isint:
    case MVM_OP_isnum:
    case MVM_OP_isstr:
    case MVM_OP_islist:
    case MVM_OP_ishash: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        MVMint32 reprid = op == MVM_OP_isint ? MVM_REPR_ID_P6int :
                          op == MVM_OP_isnum ? MVM_REPR_ID_P6num :
                          op == MVM_OP_isstr ? MVM_REPR_ID_P6str :
                          op == MVM_OP_islist ? MVM_REPR_ID_VMArray :
                     /*  op == MVM_OP_ishash */ MVM_REPR_ID_MVMHash;
        //| mov TMP1, aword WORK[obj];
        //| test TMP1, TMP1;
        //| jz >1;
        //| mov TMP1, OBJECT:TMP1->st;
        //| mov TMP1, STABLE:TMP1->REPR;
        //| cmp qword REPR:TMP1->ID, reprid;
        //| jne >1;
        //| mov qword WORK[dst], 1;
        //| jmp >2;
        //|1:
        //| mov qword WORK[dst], 0;
        //|2:
        dasm_put(Dst, 1540, Dt21([obj]), DtE(->st), Dt12(->REPR), Dt13(->ID), reprid, Dt21([dst]), Dt21([dst]));
#line 1338 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_boolify_iter_arr: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[obj];
        //| mov TMP2, MVMITER:TMP1->body.array_state.index;
        //| add TMP2, 1;
        //| mov TMP3, MVMITER:TMP1->body.array_state.limit;
        dasm_put(Dst, 1593, Dt21([obj]), DtB(->body.array_state.index), DtB(->body.array_state.limit));
#line 1347 "src/jit/x64/emit.dasc"
        /* index - limit will give a carry flag when index < limit */
        //| cmp TMP2, TMP3;
        //| setl TMP1b;
        //| movzx TMP1, TMP1b;
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 1610, Dt21([dst]));
#line 1352 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_boolify_iter_hash: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[obj];
        //| mov TMP2, MVMITER:TMP1->body.hash_state.next;
        //| test TMP2, TMP2;
        //| setnz TMP2b;
        //| movzx TMP2, TMP2b;
        //| mov aword WORK[dst], TMP2;
        dasm_put(Dst, 1625, Dt21([obj]), DtB(->body.hash_state.next), Dt21([dst]));
#line 1363 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_objprimspec: {
        MVMint16 dst  = ins->operands[0].reg.orig;
        MVMint16 type = ins->operands[1].reg.orig;
        //| mov TMP6, aword WORK[type];
        //| test TMP6, TMP6;
        //| jz >1;
        //| mov ARG2, OBJECT:TMP6->st;
        //| mov FUNCTION, STABLE:ARG2->REPR;
        //| mov FUNCTION, REPR:FUNCTION->get_storage_spec;
        //| mov ARG1, TC;
        //| call FUNCTION;
        //| movzx TMP6, word STORAGESPEC:RV->boxed_primitive;
        //|1:
        //| mov aword WORK[dst], TMP6;
        dasm_put(Dst, 1648, Dt21([type]), DtE(->st), Dt12(->REPR), Dt13(->get_storage_spec), Dt17(->boxed_primitive), Dt21([dst]));
#line 1379 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_objprimbits: {
        MVMint16 dst  = ins->operands[0].reg.orig;
        MVMint16 type = ins->operands[1].reg.orig;
        //| mov TMP6, aword WORK[type];
        //| test TMP6, TMP6;
        //| jz >1;
        //| mov ARG2, OBJECT:TMP6->st;
        //| mov FUNCTION, STABLE:ARG2->REPR;
        //| mov FUNCTION, REPR:FUNCTION->get_storage_spec;
        //| mov ARG1, TC;
        //| call FUNCTION;
        //| movzx TMP6, word STORAGESPEC:RV->boxed_primitive;
        //| test TMP6, TMP6;
        //| jz >1;
        //| movzx TMP6, word STORAGESPEC:RV->bits;
        //|1:
        //| mov aword WORK[dst], TMP6;
        dasm_put(Dst, 1691, Dt21([type]), DtE(->st), Dt12(->REPR), Dt13(->get_storage_spec), Dt17(->boxed_primitive), Dt17(->bits), Dt21([dst]));
#line 1398 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_isnonnull: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[obj];
        //| test TMP1, TMP1;
        //| setnz TMP2b;
        //| get_vmnull TMP3;
        //| cmp TMP1, TMP3;
        //| setne TMP3b;
        //| and TMP2b, TMP3b;
        //| movzx TMP2, TMP2b;
        //| mov WORK[dst], TMP2;
        dasm_put(Dst, 1746, Dt21([obj]), Dt20(->instance), DtC(->VMNull), Dt21([dst]));
#line 1412 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_isnull: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[obj];
        //| test TMP1, TMP1;
        //| setz TMP2b;
        //| get_vmnull TMP3;
        //| cmp TMP1, TMP3;
        //| sete TMP3b;
        //| or TMP2b, TMP3b;
        //| movzx TMP2, TMP2b;
        //| mov WORK[dst], TMP2;
        dasm_put(Dst, 1783, Dt21([obj]), Dt20(->instance), DtC(->VMNull), Dt21([dst]));
#line 1426 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_fastcreate: {
        MVMint16 dst       = ins->operands[0].reg.orig;
        MVMuint16 size     = ins->operands[1].lit_i16;
        MVMint16 spesh_idx = ins->operands[2].lit_i16;
        //| mov ARG1, TC;
        //| mov ARG2, size;
        //| callp &MVM_gc_allocate_zeroed;
        dasm_put(Dst, 1820, size);
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_gc_allocate_zeroed)), (MVMuint32)((uintptr_t)(&MVM_gc_allocate_zeroed) >> 32));
#line 1435 "src/jit/x64/emit.dasc"
        //| get_spesh_slot TMP1, spesh_idx;
        //| mov aword OBJECT:RV->st, TMP1;  // st is 64 bit (pointer)
        //| mov word OBJECT:RV->header.size, size; // object size is 16 bit
        //| mov TMP1d, dword TC->thread_id;  // thread id is 32 bit
        //| mov dword OBJECT:RV->header.owner, TMP1d; // does this even work?
        //| mov aword WORK[dst], RV; // store in local register
        dasm_put(Dst, 1830, Dt20(->cur_frame), Dt2(->effective_spesh_slots), Dt15([spesh_idx]), DtE(->st), DtE(->header.size), size, Dt20(->thread_id), DtE(->header.owner), Dt21([dst]));
#line 1441 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_decont:
    case MVM_OP_sp_decont: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP5, WORK[src];
        //| test TMP5, TMP5;
        dasm_put(Dst, 1868, Dt21([src]));
#line 1449 "src/jit/x64/emit.dasc"
        // obj is null
        //| jz >1;
        //| is_type_object TMP5;
        dasm_put(Dst, 1876, DtE(->header.flags), MVM_CF_TYPE_OBJECT);
#line 1452 "src/jit/x64/emit.dasc"
        // object is type object (not concrete)
        //| jnz >1;
        //| mov TMP6, OBJECT:TMP5->st;
        //| mov TMP6, STABLE:TMP6->container_spec;
        //| test TMP6, TMP6;
        dasm_put(Dst, 1888, DtE(->st), Dt12(->container_spec));
#line 1457 "src/jit/x64/emit.dasc"
        // container spec is zero
        //| jz >1;
        //| mov ARG1, TC;
        //| mov ARG2, TMP5;      // object
        //| lea ARG3, WORK[dst]; // destination register
        //| mov FUNCTION, CONTAINERSPEC:TMP6->fetch; // get function pointer
        //| call FUNCTION;
        //| jmp >2;
        //|1:
        dasm_put(Dst, 1904, Dt21([dst]), Dt16(->fetch));
#line 1466 "src/jit/x64/emit.dasc"
        // otherwise just move the object into the register
        //| mov WORK[dst], TMP5;
        //|2:
        dasm_put(Dst, 1934, Dt21([dst]));
#line 1469 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_iscont: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[obj];
        //| test TMP1, TMP1;
        //| jz >1;
        //| mov TMP1, OBJECT:TMP1->st;
        //| mov TMP1, STABLE:TMP1->container_spec;
        //| test TMP1, TMP1;
        //|1:
        //| setnz TMP1b;
        //| movzx TMP1, TMP1b;
        //| mov qword WORK[dst], TMP1;
        dasm_put(Dst, 1941, Dt21([obj]), DtE(->st), Dt12(->container_spec), Dt21([dst]));
#line 1484 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_findmeth: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        MVMint32 str_idx = ins->operands[2].lit_str_idx;
        MVMuint16 ss_idx = ins->operands[3].lit_i16;
        //| get_spesh_slot TMP1, ss_idx;
        //| mov TMP2, WORK[obj];
        //| mov TMP2, OBJECT:TMP2->st;
        //| cmp TMP1, TMP2;
        //| jne >1;
        //| get_spesh_slot TMP3, ss_idx + 1;
        //| mov WORK[dst], TMP3;
        //| jmp >2;
        //|1:
        dasm_put(Dst, 1977, Dt20(->cur_frame), Dt2(->effective_spesh_slots), Dt15([ss_idx]), Dt21([obj]), DtE(->st), Dt20(->cur_frame), Dt2(->effective_spesh_slots), Dt15([ss_idx + 1]), Dt21([dst]));
#line 1500 "src/jit/x64/emit.dasc"
        /* assign invokish label first */
        //| mov rax, TC->cur_frame;
        //| lea TMP6, [>2];
        //| mov aword FRAME:rax->jit_entry_label, TMP6;
        dasm_put(Dst, 2027, Dt20(->cur_frame), Dt2(->jit_entry_label));
#line 1504 "src/jit/x64/emit.dasc"
        /* call find_method_spesh */
        //| mov ARG1, TC;
        //| mov ARG2, WORK[obj];
        //| get_string ARG3, str_idx;
        dasm_put(Dst, 2041, Dt21([obj]));
         MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, str_idx);
#line 1508 "src/jit/x64/emit.dasc"
        //| mov ARG4, ss_idx;
        //| lea TMP6, WORK[dst];
        //|.if WIN32;
        //| mov qword [rsp+0x20], TMP6;
        //|.else;
        //| mov ARG5, TMP6;
        //|.endif
        //| callp &MVM_6model_find_method_spesh;
        dasm_put(Dst, 2050, Dt22(->body.strings), Dt14([str_idx]), ss_idx, Dt21([dst]));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_6model_find_method_spesh)), (MVMuint32)((uintptr_t)(&MVM_6model_find_method_spesh) >> 32));
#line 1516 "src/jit/x64/emit.dasc"
        //| test RV, RV;
        dasm_put(Dst, 2071);
#line 1517 "src/jit/x64/emit.dasc"
        /* fall out to interpreter */
        //| jnz ->exit;
        //|2:
        dasm_put(Dst, 2080);
#line 1520 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_isconcrete: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[obj];
        //| test TMP1, TMP1;
        //| jz >1;
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov qword WORK[dst], 1;
        //| jmp >2;
        //|1:
        //| mov qword WORK[dst], 0;
        //|2:
        dasm_put(Dst, 2087, Dt21([obj]), DtE(->header.flags), MVM_CF_TYPE_OBJECT, Dt21([dst]), Dt21([dst]));
#line 1535 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_takehandlerresult: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, aword TC->last_handler_result;
        //| mov aword WORK[dst], TMP1;
        //| mov aword TC->last_handler_result, 0;
        dasm_put(Dst, 2133, Dt20(->last_handler_result), Dt21([dst]), Dt20(->last_handler_result));
#line 1542 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_exception: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, TC->active_handlers;
        //| test TMP1, TMP1;
        //| jz >1;
        //| mov TMP1, MVMACTIVEHANDLERS:TMP1->ex_obj;
        //| mov WORK[dst], TMP1;
        //| jmp >2;
        //|1:
        //| get_vmnull TMP1;
        //| mov WORK[dst], TMP1;
        //|2:
        dasm_put(Dst, 2150, Dt20(->active_handlers), DtD(->ex_obj), Dt21([dst]), Dt20(->instance), DtC(->VMNull), Dt21([dst]));
#line 1556 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_scwbdisable: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1d, dword TC->sc_wb_disable_depth;
        //| add TMP1d, 1;
        //| mov dword TC->sc_wb_disable_depth, TMP1d;
        //| mov qword WORK[dst], TMP1;
        dasm_put(Dst, 2190, Dt20(->sc_wb_disable_depth), Dt20(->sc_wb_disable_depth), Dt21([dst]));
#line 1564 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_scwbenable: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1d, dword TC->sc_wb_disable_depth; // should do zero-extension
        //| sub TMP1d, 1;
        //| mov dword TC->sc_wb_disable_depth, TMP1d;
        //| mov qword WORK[dst], TMP1;
        dasm_put(Dst, 2206, Dt20(->sc_wb_disable_depth), Dt20(->sc_wb_disable_depth), Dt21([dst]));
#line 1572 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_assign:
    case MVM_OP_assignunchecked: {
        MVMint16 cont = ins->operands[0].reg.orig;
        MVMint16 obj  = ins->operands[1].reg.orig;
        //| mov ARG2, aword WORK[cont];
        //| mov FUNCTION, OBJECT:ARG2->st;
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| test FUNCTION, FUNCTION;
        //| jnz >1;
        //| throw_adhoc "Cannot assign to an immutable value";
        dasm_put(Dst, 2223, Dt21([cont]), DtE(->st), Dt12(->container_spec), (unsigned int)((uintptr_t)("Cannot assign to an immutable value")), (unsigned int)(((uintptr_t)("Cannot assign to an immutable value"))>>32));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc)), (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc) >> 32));
#line 1584 "src/jit/x64/emit.dasc"
        //|1:
        //| mov ARG1, TC;
        //| mov ARG3, aword WORK[obj];
        dasm_put(Dst, 2252, Dt21([obj]));
#line 1587 "src/jit/x64/emit.dasc"
        if (op == MVM_OP_assign) {
            //| mov FUNCTION, CONTAINERSPEC:FUNCTION->store;
            dasm_put(Dst, 173, Dt16(->store));
#line 1589 "src/jit/x64/emit.dasc"
        } else {
            //| mov FUNCTION, CONTAINERSPEC:FUNCTION->store_unchecked;
            dasm_put(Dst, 173, Dt16(->store_unchecked));
#line 1591 "src/jit/x64/emit.dasc"
        }
        //| call FUNCTION;
        dasm_put(Dst, 2268);
#line 1593 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_getlexstatic_o:
    case MVM_OP_getlexperinvtype_o: {
        MVMint16 dst  = ins->operands[0].reg.orig;
        MVMint16 name = ins->operands[1].reg.orig;
        //| mov ARG1, TC;
        //| mov ARG2, aword WORK[name];
        //| mov ARG3, MVM_reg_obj;
        //| callp &MVM_frame_find_lexical_by_name;
        dasm_put(Dst, 2273, Dt21([name]), MVM_reg_obj);
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_frame_find_lexical_by_name)), (MVMuint32)((uintptr_t)(&MVM_frame_find_lexical_by_name) >> 32));
#line 1603 "src/jit/x64/emit.dasc"
        //| test RV, RV;
        //| jz >1;
        //| mov RV, [RV];
        //|1:
        //| mov WORK[dst], RV;
        dasm_put(Dst, 2287, Dt21([dst]));
#line 1608 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_paramnamesused:
        //| mov ARG2, TC->cur_frame;
        //| lea ARG2, FRAME:ARG2->params;
        //| mov TMP5w, word ARGCTX:ARG2->num_pos;
        //| cmp TMP5w, word ARGCTX:ARG2->arg_count;
        //| je >1;
        //| mov ARG1, TC;
        //| callp &MVM_args_assert_nameds_used;
        dasm_put(Dst, 2309, Dt20(->cur_frame), Dt2(->params), Dt3(->num_pos), Dt3(->arg_count));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_args_assert_nameds_used)), (MVMuint32)((uintptr_t)(&MVM_args_assert_nameds_used) >> 32));
#line 1618 "src/jit/x64/emit.dasc"
        //|1:
        dasm_put(Dst, 1084);
#line 1619 "src/jit/x64/emit.dasc"
        break;
    case MVM_OP_assertparamcheck: {
        MVMint16 ok = ins->operands[0].reg.orig;
        //| mov TMP1, qword WORK[ok];
        //| test TMP1, TMP1;
        //| jnz >1;
        //| mov ARG1, TC;
        //| callp &MVM_args_bind_failed;
        dasm_put(Dst, 2337, Dt21([ok]));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_args_bind_failed)), (MVMuint32)((uintptr_t)(&MVM_args_bind_failed) >> 32));
#line 1627 "src/jit/x64/emit.dasc"
        //|1:
        dasm_put(Dst, 1084);
#line 1628 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_prof_enterspesh:
        //| mov ARG1, TC;
        //| mov ARG2, TC->cur_frame;
        //| mov ARG2, aword FRAME:ARG2->static_info;
        //| mov ARG3, aword MVM_PROFILE_ENTER_JIT;
        //| callp &MVM_profile_log_enter;
        dasm_put(Dst, 2354, Dt20(->cur_frame), Dt2(->static_info), MVM_PROFILE_ENTER_JIT);
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_profile_log_enter)), (MVMuint32)((uintptr_t)(&MVM_profile_log_enter) >> 32));
        dasm_put(Dst, 205);
#line 1636 "src/jit/x64/emit.dasc"
        break;
    case MVM_OP_prof_enterinline: {
        MVMint16 spesh_idx = ins->operands[0].lit_i16;
        //| mov ARG1, TC;
        //| get_spesh_slot ARG2, spesh_idx;
        //| mov ARG3, aword MVM_PROFILE_ENTER_JIT_INLINE;
        //| callp &MVM_profile_log_enter;
        dasm_put(Dst, 2372, Dt20(->cur_frame), Dt2(->effective_spesh_slots), Dt15([spesh_idx]), MVM_PROFILE_ENTER_JIT_INLINE);
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_profile_log_enter)), (MVMuint32)((uintptr_t)(&MVM_profile_log_enter) >> 32));
        dasm_put(Dst, 205);
#line 1643 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_getobjsc: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[obj];
        //| mov TMP2d, dword COLLECTABLE:TMP1->sc_forward_u.sc.sc_idx;
        //| xor TMP3, TMP3;
        //| cmp TMP2d, 0;
        //| jle >1;
        //| mov TMP3, aword TC->instance;
        //| mov TMP3, aword MVMINSTANCE:TMP3->all_scs;
        //| mov TMP3, aword [TMP3 + TMP2d * 8];
        //| mov TMP3, SCREFBODY:TMP3->sc;
        //|1:
        //| mov aword WORK[dst], TMP3;
        dasm_put(Dst, 2394, Dt21([obj]), Dt11(->sc_forward_u.sc.sc_idx), Dt20(->instance), DtC(->all_scs), Dt19(->sc), Dt21([dst]));
#line 1659 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_invokewithcapture: {
        MVMint16 dest    = ins->operands[0].reg.orig;
        MVMint16 code    = ins->operands[1].reg.orig;
        MVMint16 capture = ins->operands[2].reg.orig;
        //| mov TMP1, aword WORK[capture];
        dasm_put(Dst, 1203, Dt21([capture]));
#line 1666 "src/jit/x64/emit.dasc"
        /* if (IS_CONCRETE(capture) && REPR(capture)->ID == MVM_REPR_ID_MVMCallCapture) */
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov TMP2, OBJECT:TMP1->st;
        //| mov TMP2, STABLE:TMP2->REPR;
        //| cmp dword REPR:TMP2->ID, MVM_REPR_ID_MVMCallCapture;
        //| je >2;
        //|1:
        dasm_put(Dst, 2435, DtE(->header.flags), MVM_CF_TYPE_OBJECT, DtE(->st), Dt12(->REPR), Dt13(->ID), MVM_REPR_ID_MVMCallCapture);
#line 1674 "src/jit/x64/emit.dasc"
        /* else throw */
        //| throw_adhoc "invokewithcapture needs a MVMCallCapture";
        dasm_put(Dst, 1074, (unsigned int)((uintptr_t)("invokewithcapture needs a MVMCallCapture")), (unsigned int)(((uintptr_t)("invokewithcapture needs a MVMCallCapture"))>>32));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc)), (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc) >> 32));
#line 1676 "src/jit/x64/emit.dasc"
        //|2:
        dasm_put(Dst, 345);
#line 1677 "src/jit/x64/emit.dasc"
        /* code = MVM_frame_find_invokee(tc, code, NULL) */
        //| mov ARG1, TC;
        //| mov ARG2, WORK[code];
        //| xor ARG3, ARG3;
        //| callp &MVM_frame_find_invokee;
        dasm_put(Dst, 2464, Dt21([code]));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_frame_find_invokee)), (MVMuint32)((uintptr_t)(&MVM_frame_find_invokee) >> 32));
        dasm_put(Dst, 205);
#line 1682 "src/jit/x64/emit.dasc"

        //| mov ARG2, RV; // ARG2 = code
        dasm_put(Dst, 2477);
#line 1684 "src/jit/x64/emit.dasc"
        /* tc->cur_frame->return_value = &GET_REG(cur_op, 2) */
        //| lea ARG1, WORK[dest]; // ARG1 is used as scratch space
        //| mov TMP6, aword TC->cur_frame;
        //| mov aword FRAME:TMP6->return_value, ARG1;
        dasm_put(Dst, 2481, Dt21([dest]), Dt20(->cur_frame), Dt2(->return_value));
#line 1688 "src/jit/x64/emit.dasc"
        /* tc->cur_frame->return_type = MVM_RETURN_OBJ */
        //| mov byte FRAME:TMP6->return_type, MVM_RETURN_OBJ;
        dasm_put(Dst, 2494, Dt2(->return_type), MVM_RETURN_OBJ);
#line 1690 "src/jit/x64/emit.dasc"
        /* tc->cur_frame->return_address = cur_op */
        //| get_cur_op ARG1;
        //| mov aword FRAME:TMP6->return_address, ARG1;
        dasm_put(Dst, 2500, Dt20(->interp_cur_op), Dt2(->return_address));
#line 1693 "src/jit/x64/emit.dasc"
        /* STABLE(code)->invoke(tc, code, capture->body.apc->effective_callsite,
           capture->body.apc->arg) */
        //| mov ARG1, TC;
        //| mov ARG3, WORK[capture];
        //| mov ARG4, aword CAPTURE:ARG3->body.apc;
        //| mov ARG3, aword ARGCTX:ARG4->callsite;
        //| mov ARG4, aword ARGCTX:ARG4->args;
        //| mov FUNCTION, OBJECT:ARG2->st;
        //| mov FUNCTION, STABLE:FUNCTION->invoke;
        //| call FUNCTION;
        dasm_put(Dst, 2512, Dt21([capture]), Dt5(->body.apc), Dt3(->callsite), Dt3(->args), DtE(->st), Dt12(->invoke));
#line 1703 "src/jit/x64/emit.dasc"
        /* jumping out is handled by invokish */
        break;
    }
    case MVM_OP_captureposelems: {
        MVMint16 dest    = ins->operands[0].reg.orig;
        MVMint16 capture = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[capture];
        dasm_put(Dst, 1203, Dt21([capture]));
#line 1710 "src/jit/x64/emit.dasc"
        /* if (IS_CONCRETE(capture) && REPR(capture)->ID == MVM_REPR_ID_MVMCallCapture) */
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov TMP2, OBJECT:TMP1->st;
        //| mov TMP2, STABLE:TMP2->REPR;
        //| cmp dword REPR:TMP2->ID, MVM_REPR_ID_MVMCallCapture;
        //| je >2;
        //|1:
        dasm_put(Dst, 2435, DtE(->header.flags), MVM_CF_TYPE_OBJECT, DtE(->st), Dt12(->REPR), Dt13(->ID), MVM_REPR_ID_MVMCallCapture);
#line 1718 "src/jit/x64/emit.dasc"
        /* else throw */
        //| throw_adhoc "captureposelems needs a concrete MVMCallCapture";
        dasm_put(Dst, 1074, (unsigned int)((uintptr_t)("captureposelems needs a concrete MVMCallCapture")), (unsigned int)(((uintptr_t)("captureposelems needs a concrete MVMCallCapture"))>>32));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc)), (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc) >> 32));
#line 1720 "src/jit/x64/emit.dasc"
        //|2:
        //| mov TMP2, aword CAPTURE:TMP1->body.apc;
        //| movzx TMP2, word ARGPROCCONTEXT:TMP2->num_pos;
        //| mov WORK[dest], TMP2;
        dasm_put(Dst, 2545, Dt5(->body.apc), Dt7(->num_pos), Dt21([dest]));
#line 1724 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_capturehasnameds: {
        MVMint16 dest    = ins->operands[0].reg.orig;
        MVMint16 capture = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[capture];
        dasm_put(Dst, 1203, Dt21([capture]));
#line 1730 "src/jit/x64/emit.dasc"
        /* if (IS_CONCRETE(capture) && REPR(capture)->ID == MVM_REPR_ID_MVMCallCapture) */
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov TMP2, OBJECT:TMP1->st;
        //| mov TMP2, STABLE:TMP2->REPR;
        //| cmp dword REPR:TMP2->ID, MVM_REPR_ID_MVMCallCapture;
        //| je >2;
        //|1:
        dasm_put(Dst, 2435, DtE(->header.flags), MVM_CF_TYPE_OBJECT, DtE(->st), Dt12(->REPR), Dt13(->ID), MVM_REPR_ID_MVMCallCapture);
#line 1738 "src/jit/x64/emit.dasc"
        /* else throw */
        //| throw_adhoc "capturehasnameds needs a concrete MVMCallCapture";
        dasm_put(Dst, 1074, (unsigned int)((uintptr_t)("capturehasnameds needs a concrete MVMCallCapture")), (unsigned int)(((uintptr_t)("capturehasnameds needs a concrete MVMCallCapture"))>>32));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc)), (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc) >> 32));
#line 1740 "src/jit/x64/emit.dasc"
        //|2:
        //| mov TMP2, CAPTURE:TMP1->body.apc;
        //| mov TMP3w, word ARGPROCCONTEXT:TMP2->num_pos;
        //| mov TMP2w, word ARGPROCCONTEXT:TMP2->arg_count;
        //| cmp TMP2w, TMP3w;
        //| setne al;
        //| movzx rax, al;
        //| mov WORK[dest], rax;
        dasm_put(Dst, 2566, Dt5(->body.apc), Dt7(->num_pos), Dt7(->arg_count), Dt21([dest]));
#line 1748 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_getstdin:
    case MVM_OP_getstdout:
    case MVM_OP_getstderr: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP3, aword TC->instance;
        dasm_put(Dst, 2602, Dt20(->instance));
#line 1755 "src/jit/x64/emit.dasc"
        if (ins->info->opcode == MVM_OP_getstdin) {
            //| mov TMP3, aword MVMINSTANCE:TMP3->stdin_handle;
            dasm_put(Dst, 437, DtC(->stdin_handle));
#line 1757 "src/jit/x64/emit.dasc"
        } else if (ins->info->opcode == MVM_OP_getstdout) {
            //| mov TMP3, aword MVMINSTANCE:TMP3->stdout_handle;
            dasm_put(Dst, 437, DtC(->stdout_handle));
#line 1759 "src/jit/x64/emit.dasc"
        } else if (ins->info->opcode == MVM_OP_getstderr) {
            //| mov TMP3, aword MVMINSTANCE:TMP3->stderr_handle;
            dasm_put(Dst, 437, DtC(->stderr_handle));
#line 1761 "src/jit/x64/emit.dasc"
        }
        //| mov aword WORK[dst], TMP3;
        dasm_put(Dst, 567, Dt21([dst]));
#line 1763 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_ordat:
    case MVM_OP_ordfirst: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 str = ins->operands[1].reg.orig;
        //| mov ARG1, TC;
        //| mov ARG2, aword WORK[str];
        dasm_put(Dst, 2041, Dt21([str]));
#line 1771 "src/jit/x64/emit.dasc"
        if (op == MVM_OP_ordat) {
            MVMint16 idx = ins->operands[2].reg.orig;
            //| mov ARG3, qword WORK[idx];
            dasm_put(Dst, 576, Dt21([idx]));
#line 1774 "src/jit/x64/emit.dasc"
        } else {
            //| mov ARG3, 0;
            dasm_put(Dst, 1392);
#line 1776 "src/jit/x64/emit.dasc"
        }
        //| callp &MVM_string_ord_at;
        dasm_put(Dst, 197);
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_string_ord_at)), (MVMuint32)((uintptr_t)(&MVM_string_ord_at) >> 32));
#line 1778 "src/jit/x64/emit.dasc"
        //| mov qword WORK[dst], RV;
        dasm_put(Dst, 2607, Dt21([dst]));
#line 1779 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_setcodeobj: {
        MVMint16 obj  = ins->operands[0].reg.orig;
        MVMint16 code = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[obj];
        dasm_put(Dst, 1203, Dt21([obj]));
#line 1785 "src/jit/x64/emit.dasc"
        /* if (REPR(obj)->ID == MVM_REPR_ID_MVMCode) */
        //| mov TMP2, OBJECT:TMP1->st;
        //| mov TMP2, STABLE:TMP2->REPR;
        //| cmp dword REPR:TMP2->ID, MVM_REPR_ID_MVMCode;
        //| je >1;
        dasm_put(Dst, 2617, DtE(->st), Dt12(->REPR), Dt13(->ID), MVM_REPR_ID_MVMCode);
#line 1790 "src/jit/x64/emit.dasc"
        /* else throw */
        //| throw_adhoc "setcodeobj needs a code ref";
        dasm_put(Dst, 1074, (unsigned int)((uintptr_t)("setcodeobj needs a code ref")), (unsigned int)(((uintptr_t)("setcodeobj needs a code ref"))>>32));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc)), (MVMuint32)((uintptr_t)(&MVM_exception_throw_adhoc) >> 32));
#line 1792 "src/jit/x64/emit.dasc"
        //|1:
        //| mov TMP2, aword WORK[code];
        //| mov aword CODE:TMP1->body.code_object, TMP2;
        dasm_put(Dst, 2634, Dt21([code]), Dt1B(->body.code_object));
#line 1795 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_lastexpayload: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP3, aword TC->last_payload;
        //| mov aword WORK[dst], TMP3;
        dasm_put(Dst, 2650, Dt20(->last_payload), Dt21([dst]));
#line 1801 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_param_sp: {
        MVMuint16 dst = ins->operands[0].reg.orig;
        MVMuint16 off = ins->operands[1].lit_ui16;
        //| mov ARG1, TC
        //| mov ARG2, TC:ARG1->cur_frame
        //| lea ARG2, FRAME:ARG2->params
        //| mov ARG3, off
        //| callp &MVM_args_slurpy_positional;
        dasm_put(Dst, 2659, Dt20(->cur_frame), Dt2(->params), off);
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_args_slurpy_positional)), (MVMuint32)((uintptr_t)(&MVM_args_slurpy_positional) >> 32));
#line 1811 "src/jit/x64/emit.dasc"
        //| mov qword WORK[dst], RV;
        dasm_put(Dst, 2607, Dt21([dst]));
#line 1812 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_param_sn: {
        MVMuint16 dst = ins->operands[0].reg.orig;
        //| mov ARG1, TC
        //| mov ARG2, TC:ARG1->cur_frame
        //| lea ARG2, FRAME:ARG2->params
        //| callp &MVM_args_slurpy_named;
        dasm_put(Dst, 2677, Dt20(->cur_frame), Dt2(->params));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_args_slurpy_named)), (MVMuint32)((uintptr_t)(&MVM_args_slurpy_named) >> 32));
#line 1820 "src/jit/x64/emit.dasc"
        //| mov qword WORK[dst], RV;
        dasm_put(Dst, 2607, Dt21([dst]));
#line 1821 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_cas_o: {
        MVMint16 target = ins->operands[1].reg.orig;
        MVMint16 expected = ins->operands[2].reg.orig;
        MVMint16 value = ins->operands[3].reg.orig;
        MVMint16 result = ins->operands[0].reg.orig;
        //| mov ARG1, TC;
        //| mov ARG2, aword WORK[target];
        //| mov ARG3, aword WORK[expected];
        //| mov ARG4, aword WORK[value];
        //|.if WIN32;
        //| lea TMP6, WORK[result]
        //| mov qword [rsp+0x20], TMP6
        //|.else;
        //| lea ARG5, WORK[result]
        //|.endif
        //| mov FUNCTION, OBJECT:ARG2->st;
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->cas;
        //| call FUNCTION;
        dasm_put(Dst, 2691, Dt21([target]), Dt21([expected]), Dt21([value]), Dt21([result]), DtE(->st), Dt12(->container_spec), Dt16(->cas));
#line 1842 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_atomicload_o: {
        MVMint16 target = ins->operands[1].reg.orig;
        MVMint16 result = ins->operands[0].reg.orig;
        //| mov ARG1, TC;
        //| mov ARG2, aword WORK[target];
        //| mov FUNCTION, OBJECT:ARG2->st;
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->atomic_load;
        //| call FUNCTION;
        //| mov WORK[result], RV
        dasm_put(Dst, 2728, Dt21([target]), DtE(->st), Dt12(->container_spec), Dt16(->atomic_load), Dt21([result]));
#line 1854 "src/jit/x64/emit.dasc"
        break;
    }
    case MVM_OP_sp_atomicstore_o: {
        MVMint16 target = ins->operands[0].reg.orig;
        MVMint16 value = ins->operands[1].reg.orig;
        //| mov ARG1, TC;
        //| mov ARG2, aword WORK[target];
        //| mov ARG3, aword WORK[value];
        //| mov FUNCTION, OBJECT:ARG2->st;
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->atomic_store;
        //| call FUNCTION;
        dasm_put(Dst, 2757, Dt21([target]), Dt21([value]), DtE(->st), Dt12(->container_spec), Dt16(->atomic_store));
#line 1866 "src/jit/x64/emit.dasc"
        break;
    }
    default:
        MVM_panic(1, "Can't JIT opcode <%s>", ins->info->name);
    }
}



/* Call argument decoder */
static void load_call_arg(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                          MVMJitCallArg arg) {
    switch(arg.type) {
    case MVM_JIT_INTERP_VAR:
        switch (arg.v.ivar) {
        case MVM_JIT_INTERP_TC:
            //| mov TMP6, TC;
            dasm_put(Dst, 2786);
#line 1883 "src/jit/x64/emit.dasc"
            break;
        case MVM_JIT_INTERP_CU:
            //| mov TMP6, CU;
            dasm_put(Dst, 2791);
#line 1886 "src/jit/x64/emit.dasc"
            break;
        case MVM_JIT_INTERP_FRAME:
            //| mov TMP6, TC->cur_frame;
            dasm_put(Dst, 158, Dt20(->cur_frame));
#line 1889 "src/jit/x64/emit.dasc"
            break;
        case MVM_JIT_INTERP_PARAMS:
            //| mov TMP6, TC->cur_frame;
            //| lea TMP6, FRAME:TMP6->params;
            dasm_put(Dst, 2796, Dt20(->cur_frame), Dt2(->params));
#line 1893 "src/jit/x64/emit.dasc"
            break;
        case MVM_JIT_INTERP_CALLER:
            //| mov TMP6, TC->cur_frame;
            //| mov TMP6, aword FRAME:TMP6->caller;
            dasm_put(Dst, 2805, Dt20(->cur_frame), Dt2(->caller));
#line 1897 "src/jit/x64/emit.dasc"
            break;
        }
        break;
    case MVM_JIT_REG_VAL:
        //| mov TMP6, qword WORK[arg.v.reg];
        dasm_put(Dst, 2814, Dt21([arg.v.reg]));
#line 1902 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_REG_VAL_F:
        //| mov TMP6, qword WORK[arg.v.reg];
        dasm_put(Dst, 2814, Dt21([arg.v.reg]));
#line 1905 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_REG_ADDR:
        //| lea TMP6, WORK[arg.v.reg];
        dasm_put(Dst, 2819, Dt21([arg.v.reg]));
#line 1908 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_STR_IDX:
        //| get_string TMP6, arg.v.lit_i64;
         MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, arg.v.lit_i64);
        dasm_put(Dst, 2824, Dt22(->body.strings), Dt14([arg.v.lit_i64]));
#line 1911 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_LITERAL:
        //| mov TMP6, arg.v.lit_i64;
        dasm_put(Dst, 2833, arg.v.lit_i64);
#line 1914 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_LITERAL_64:
    case MVM_JIT_LITERAL_PTR:
    case MVM_JIT_LITERAL_F:
        //| mov64 TMP6, arg.v.lit_i64;
        dasm_put(Dst, 2838, (unsigned int)(arg.v.lit_i64), (unsigned int)((arg.v.lit_i64)>>32));
#line 1919 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_REG_STABLE:
        //| mov TMP6, qword WORK[arg.v.reg];
        //| mov TMP6, OBJECT:TMP6->st;
        dasm_put(Dst, 222, Dt21([arg.v.reg]), DtE(->st));
#line 1923 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_REG_OBJBODY:
        //| mov TMP6, qword WORK[arg.v.reg];
        //| lea TMP6, STOOGE:TMP6->data;
        dasm_put(Dst, 2843, Dt21([arg.v.reg]), DtF(->data));
#line 1927 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_REG_DYNIDX:
        //| get_cur_op TMP5;
        //| xor TMP6, TMP6;
        //| mov TMP6w, U16:TMP5[arg.v.reg];
        //| mov TMP6, qword [WORK + TMP6*8];
        dasm_put(Dst, 2852, Dt20(->interp_cur_op), Dt1D([arg.v.reg]));
#line 1933 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_DATA_LABEL:
        //| lea TMP6, [=>(arg.v.lit_i64)];
        dasm_put(Dst, 2872, (arg.v.lit_i64));
#line 1936 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_SAVED_RV:
        //| mov TMP6, qword SAVED_RV;
        dasm_put(Dst, 2877);
#line 1939 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_ARG_I64:
        //| mov TMP6, TC->cur_frame;
        //| mov TMP6, FRAME:TMP6->args;
        //| mov TMP6, qword REGISTER:TMP6[arg.v.lit_i64];
        dasm_put(Dst, 2882, Dt20(->cur_frame), Dt2(->args), Dt1([arg.v.lit_i64]));
#line 1944 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_ARG_PTR:
        //| mov TMP6, TC->cur_frame;
        //| mov TMP6, FRAME:TMP6->args;
        //| mov TMP6, qword REGISTER:TMP6[arg.v.lit_i64];
        //| mov TMP6, aword STOOGE:TMP6->data;
        dasm_put(Dst, 2895, Dt20(->cur_frame), Dt2(->args), Dt1([arg.v.lit_i64]), DtF(->data));
#line 1950 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_ARG_VMARRAY:
        //| mov TMP6, TC->cur_frame;
        //| mov TMP6, FRAME:TMP6->args;
        //| mov TMP6, qword REGISTER:TMP6[arg.v.lit_i64];
        //| mov TMP6, aword VMARRAY:TMP6->body.slots;
        dasm_put(Dst, 2895, Dt20(->cur_frame), Dt2(->args), Dt1([arg.v.lit_i64]), Dt10(->body.slots));
#line 1956 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_PARAM_I64:
        //| mov TMP6, qword WORK[arg.v.lit_i64];
        dasm_put(Dst, 2814, Dt21([arg.v.lit_i64]));
#line 1959 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_PARAM_PTR:
        //| mov TMP6, aword WORK[arg.v.lit_i64];
        //| mov TMP6, aword STOOGE:TMP6->data;
        dasm_put(Dst, 222, Dt21([arg.v.lit_i64]), DtF(->data));
#line 1963 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_PARAM_VMARRAY:
        //| mov TMP6, aword WORK[arg.v.lit_i64];
        //| mov TMP6, aword VMARRAY:TMP6->body.slots;
        dasm_put(Dst, 222, Dt21([arg.v.lit_i64]), Dt10(->body.slots));
#line 1967 "src/jit/x64/emit.dasc"
        break;
    default:
        MVM_oops(tc, "JIT: Unknown JIT argument type %d", arg.type);
    }
}

static void emit_gpr_arg(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                         MVMint32 i) {
    switch (i) {
    case 0:
        //| mov ARG1, TMP6;
        dasm_put(Dst, 2912);
#line 1978 "src/jit/x64/emit.dasc"
        break;
    case 1:
        //| mov ARG2, TMP6;
        dasm_put(Dst, 2916);
#line 1981 "src/jit/x64/emit.dasc"
        break;
    case 2:
        //| mov ARG3, TMP6;
        dasm_put(Dst, 2920);
#line 1984 "src/jit/x64/emit.dasc"
        break;
    case 3:
        //| mov ARG4, TMP6;
        dasm_put(Dst, 2924);
#line 1987 "src/jit/x64/emit.dasc"
        break;
//|.if POSIX
    case 4:
//|        mov ARG5, TMP6;
dasm_put(Dst, 2928);
       break;
#line 1992 "src/jit/x64/emit.dasc"
  case 5:
//|      mov ARG6, TMP6;
dasm_put(Dst, 2932);
     break;
#line 1995 "src/jit/x64/emit.dasc"
//|.endif
    default:
        MVM_oops(tc, "JIT: can't store %d arguments in GPR", i);
    }
}

static void emit_sse_arg(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                         MVMint32 i) {
    switch (i) {
    case 0:
        //| movd ARG1F, TMP6;
        dasm_put(Dst, 2936);
#line 2006 "src/jit/x64/emit.dasc"
        break;
    case 1:
        //| movd ARG2F, TMP6;
        dasm_put(Dst, 2942);
#line 2009 "src/jit/x64/emit.dasc"
        break;
    case 2:
        //| movd ARG3F, TMP6;
        dasm_put(Dst, 2948);
#line 2012 "src/jit/x64/emit.dasc"
        break;
    case 3:
        //| movd ARG4F, TMP6;
        dasm_put(Dst, 2954);
#line 2015 "src/jit/x64/emit.dasc"
        break;
//|.if POSIX
    case 4:
//|        movd ARG5F, TMP6;
dasm_put(Dst, 2960);
       break;
#line 2020 "src/jit/x64/emit.dasc"
    case 5:
//|         movd ARG6F, TMP6;
dasm_put(Dst, 2966);
        break;
#line 2023 "src/jit/x64/emit.dasc"
    case 6:
//|        movd ARG7F, TMP6;
dasm_put(Dst, 2973);
       break;
#line 2026 "src/jit/x64/emit.dasc"
    case 7:
//|        movd ARG8F, TMP6;
dasm_put(Dst, 2980);
       break;
#line 2029 "src/jit/x64/emit.dasc"
//|.endif
    default:
        MVM_oops(tc, "JIT: can't put  %d arguments in SSE", i);
    }
}

static void emit_stack_arg(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                           MVMint32 arg_size, MVMint32 pos) {
    /* basically, stack arguments are passed in right-to-left order
       on both POSIX and W64 backends, it seems. Thus the most logical
       thing to do is to count from the stack top upwards. */
    if (pos + arg_size > 160) {
        MVM_oops(tc, "JIT: trying to pass arguments "
                 " in local space (stack top offset:"
                 " %d, size: %d)", pos, arg_size);
    }
    switch(arg_size) {
    case 1:
        //| mov byte [rsp+pos], TMP6b;
        dasm_put(Dst, 2987, pos);
#line 2048 "src/jit/x64/emit.dasc"
        break;
    case 2:
        //| mov word [rsp+pos], TMP6w;
        dasm_put(Dst, 2994, pos);
#line 2051 "src/jit/x64/emit.dasc"
        break;
    case 4:
        //| mov dword [rsp+pos], TMP6d;
        dasm_put(Dst, 2995, pos);
#line 2054 "src/jit/x64/emit.dasc"
        break;
    case 8:
        //| mov qword [rsp+pos], TMP6;
        dasm_put(Dst, 3002, pos);
#line 2057 "src/jit/x64/emit.dasc"
        break;
    default:
        MVM_oops(tc, "JIT: can't pass arguments size %d bytes",
                 arg_size);
    }
}

static void emit_posix_callargs(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                                MVMJitCallArg args[], MVMint32 num_args) {
    MVMint32 num_gpr = 0, num_fpr = 0, num_stack = 0, i;
    MVMJitCallArg in_gpr[6], in_fpr[8], *on_stack = NULL;
    if (num_args > 6)
        on_stack = MVM_malloc(sizeof(MVMJitCallArg) * (num_args - 6));
    /* divide in gpr, fpr, stack values */
    for (i = 0; i < num_args; i++) {
        switch (args[i].type) {
        case MVM_JIT_INTERP_VAR:
        case MVM_JIT_REG_VAL:
        case MVM_JIT_REG_ADDR:
        case MVM_JIT_REG_OBJBODY:
        case MVM_JIT_REG_STABLE:
        case MVM_JIT_REG_DYNIDX:
        case MVM_JIT_STR_IDX:
        case MVM_JIT_LITERAL:
        case MVM_JIT_LITERAL_64:
        case MVM_JIT_LITERAL_PTR:
        case MVM_JIT_DATA_LABEL:
        case MVM_JIT_SAVED_RV:
        case MVM_JIT_ARG_I64:
        case MVM_JIT_ARG_PTR:
        case MVM_JIT_ARG_VMARRAY:
        case MVM_JIT_PARAM_I64:
        case MVM_JIT_PARAM_PTR:
        case MVM_JIT_PARAM_VMARRAY:
            if (num_gpr < 6) {
                in_gpr[num_gpr++] = args[i];
            } else {
                on_stack[num_stack++] = args[i];
            }
            break;
        case MVM_JIT_REG_VAL_F:
        case MVM_JIT_LITERAL_F:
            if (num_fpr < 8) {
                in_fpr[num_fpr++] = args[i];
            } else {
                on_stack[num_stack++] = args[i];
            }
            break;
        default:
            MVM_oops(tc, "JIT: Unknown JIT argument type %d for emit_posix_callargs", args[i].type);
        }
    }
    for (i = 0; i < num_gpr; i++) {
        load_call_arg(tc, compiler, jg, in_gpr[i]);
        emit_gpr_arg(tc, compiler, jg, i);
    }
    for (i = 0; i < num_fpr; i++) {
        load_call_arg(tc, compiler, jg, in_fpr[i]);
        emit_sse_arg(tc, compiler, jg, i);
    }
    /* push right-to-left */
    for (i = 0; i < num_stack; i++) {
        load_call_arg(tc, compiler, jg, on_stack[i]);
        // I'm not sure this is correct, btw
        emit_stack_arg(tc, compiler, jg, 8, i*8);
    }
    if (on_stack)
        MVM_free(on_stack);
}

static void emit_win64_callargs(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                                MVMJitCallArg args[], MVMint32 num_args) {
    MVMint32 i;
    MVMint32 num_reg_args = (num_args > 4 ? 4 : num_args);
    for (i = 0; i < num_reg_args; i++) {
        load_call_arg(tc, compiler, jg, args[i]);
        if (args[i].type == MVM_JIT_REG_VAL_F ||
            args[i].type == MVM_JIT_LITERAL_F) {
            emit_sse_arg(tc, compiler, jg, i);
        } else {
            emit_gpr_arg(tc, compiler, jg, i);
        }
    }
    for (; i < num_args; i++) {
        load_call_arg(tc, compiler, jg, args[i]);
        emit_stack_arg(tc, compiler, jg, 8, i * 8);
    }
}

void MVM_jit_emit_call_c(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                         MVMJitCallC * call_spec) {

    MVM_jit_log(tc, "emit c call <%d args>\n", call_spec->num_args);
    if (call_spec->has_vargs) {
        MVM_oops(tc, "JIT can't handle varargs yet");
    }
    //|.if WIN32;
#line 2155 "src/jit/x64/emit.dasc"
    //|.else;
     emit_posix_callargs(tc, compiler, jg, call_spec->args, call_spec->num_args);
    //|.endif
    /* Emit the call. I think we should be able to do something smarter than
     * store the constant into the bytecode, like a data segment. But I'm
     * not sure. */
    //| callp call_spec->func_ptr;
    dasm_put(Dst, 197);
    dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(call_spec->func_ptr)), (MVMuint32)((uintptr_t)(call_spec->func_ptr) >> 32));
    dasm_put(Dst, 205);
#line 2162 "src/jit/x64/emit.dasc"
    /* right, now determine what to do with the return value */
    switch(call_spec->rv_mode) {
    case MVM_JIT_RV_VOID:
        break;
    case MVM_JIT_RV_INT:
    case MVM_JIT_RV_PTR:
        //| mov WORK[call_spec->rv_idx], RV;
        dasm_put(Dst, 1006, Dt21([call_spec->rv_idx]));
#line 2169 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_RV_NUM:
        //| movsd qword WORK[call_spec->rv_idx], RVF;
        dasm_put(Dst, 1250, Dt21([call_spec->rv_idx]));
#line 2172 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_RV_DEREF:
        //| mov TMP1, [RV];
        //| mov WORK[call_spec->rv_idx], TMP1;
        dasm_put(Dst, 3009, Dt21([call_spec->rv_idx]));
#line 2176 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_RV_ADDR:
        /* store local at address */
        //| mov TMP1, WORK[call_spec->rv_idx];
        //| mov [RV], TMP1;
        dasm_put(Dst, 3017, Dt21([call_spec->rv_idx]));
#line 2181 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_RV_DYNIDX:
        /* store in register relative to cur_op */
        //| get_cur_op TMP1;
        //| xor TMP2, TMP2;
        //| mov TMP2w, word [TMP1 + call_spec->rv_idx*2];
        //| mov aword [WORK + TMP2*8], RV;
        dasm_put(Dst, 3025, Dt20(->interp_cur_op), call_spec->rv_idx*2);
#line 2188 "src/jit/x64/emit.dasc"
        break;
    }
}

void MVM_jit_emit_block_branch(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                               MVMJitBranch * branch) {
    MVMSpeshIns *ins = branch->ins;
    MVMint32 name = branch->dest;
    /* move gc sync point to the front so as to not have
     * awkward dispatching issues */
    //| gc_sync_point;
    dasm_put(Dst, 3044, Dt20(->gc_status));
    dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_gc_enter_from_interrupt)), (MVMuint32)((uintptr_t)(&MVM_gc_enter_from_interrupt) >> 32));
    dasm_put(Dst, 1084);
#line 2199 "src/jit/x64/emit.dasc"
    if (ins == NULL || ins->info->opcode == MVM_OP_goto) {
        MVM_jit_log(tc, "emit jump to label %d\n", name);
        if (name == MVM_JIT_BRANCH_EXIT) {
            //| jmp ->exit
            dasm_put(Dst, 3059);
#line 2203 "src/jit/x64/emit.dasc"
        } else {
            //| jmp =>(name)
            dasm_put(Dst, 3064, (name));
#line 2205 "src/jit/x64/emit.dasc"
        }
    } else {
        MVMint16 val = ins->operands[0].reg.orig;
        MVM_jit_log(tc, "emit branch <%s> to label %d\n",
                    ins->info->name, name);
        switch(ins->info->opcode) {
        case MVM_OP_if_i:
            //| mov rax, WORK[val];
            //| test rax, rax;
            //| jnz =>(name); // jump to dynamic label
            dasm_put(Dst, 3068, Dt21([val]), (name));
#line 2215 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_unless_i:
            //| mov rax, WORK[val];
            //| test rax, rax;
            //| jz =>(name);
            dasm_put(Dst, 3079, Dt21([val]), (name));
#line 2220 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_if_n:
            //| movd xmm0, qword WORK[val];
            //| xorpd xmm1, xmm1; // make it zero
            //| ucomisd xmm0, xmm1;
            //| jp =>(name);  // is NaN?
            //| jne =>(name); // not equal to zero? we're golden
            dasm_put(Dst, 3090, Dt21([val]), (name), (name));
#line 2227 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_unless_n:
            //| movd xmm0, qword WORK[val];
            //| xorpd xmm1, xmm1; // make it zero
            //| ucomisd xmm0, xmm1;
            //| jp >1; // is NaN
            //| jne >1; // is not zero
            //| jmp =>(name); // it is zero yay!
            //|1:
            dasm_put(Dst, 3111, Dt21([val]), (name));
#line 2236 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_if_s0:
        case MVM_OP_unless_s0:
            //| mov ARG1, TC;
            //| mov ARG2, WORK[val];
            //| callp &MVM_coerce_istrue_s;
            dasm_put(Dst, 3139, Dt21([val]));
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_coerce_istrue_s)), (MVMuint32)((uintptr_t)(&MVM_coerce_istrue_s) >> 32));
#line 2242 "src/jit/x64/emit.dasc"
            //| test RV, RV;
            dasm_put(Dst, 2071);
#line 2243 "src/jit/x64/emit.dasc"
            if (ins->info->opcode == MVM_OP_unless_s0)
                //| jz =>(name);
                dasm_put(Dst, 3086, (name));
#line 2245 "src/jit/x64/emit.dasc"
            else
                //| jnz =>(name);
                dasm_put(Dst, 3075, (name));
#line 2247 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_ifnonnull:
            //| mov TMP1, WORK[val];
            //| test TMP1, TMP1;
            //| jz >1;
            //| get_vmnull TMP2;
            //| cmp TMP1, TMP2;
            //| je >1;
            //| jmp =>(name);
            //|1:
            dasm_put(Dst, 3149, Dt21([val]), Dt20(->instance), DtC(->VMNull), (name));
#line 2257 "src/jit/x64/emit.dasc"
            break;
        case MVM_OP_indexat:
        case MVM_OP_indexnat: {
            MVMint16 offset = ins->operands[1].reg.orig;
            MVMuint32 str_idx = ins->operands[2].lit_str_idx;
            //| mov ARG1, TC;
            //| mov ARG2, WORK[val];
            //| mov ARG3, WORK[offset];
            //| get_string ARG4, str_idx;
            dasm_put(Dst, 3181, Dt21([val]), Dt21([offset]));
             MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, str_idx);
#line 2266 "src/jit/x64/emit.dasc"
            //| callp &MVM_string_char_at_in_string;
            dasm_put(Dst, 3194, Dt22(->body.strings), Dt14([str_idx]));
            dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_string_char_at_in_string)), (MVMuint32)((uintptr_t)(&MVM_string_char_at_in_string) >> 32));
            dasm_put(Dst, 205);
#line 2267 "src/jit/x64/emit.dasc"
            /* This subtlety is due to the value being overloaded to
             * -2 if it is out of bounds. Note that -1 is passed as a
             * 32 bit integer, but this magically works in a 64 bit
             * comparison because 32 bit values are sign-extended */
            //| cmp RV, -1;
            dasm_put(Dst, 3204);
#line 2272 "src/jit/x64/emit.dasc"
            if (ins->info->opcode == MVM_OP_indexat)
                //| jle =>(name);
                dasm_put(Dst, 3211, (name));
#line 2274 "src/jit/x64/emit.dasc"
            else {

                //| jne =>(name);
                dasm_put(Dst, 3075, (name));
#line 2277 "src/jit/x64/emit.dasc"
            }
            break;
        }
        default:
            MVM_panic(1, "JIT: Can't handle conditional <%s>", ins->info->name);
        }
    }
}

void MVM_jit_emit_label(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                        MVMint32 label) {
    MVM_jit_log(tc, "Emitting label %d\n", label);
    //| =>(label):
    dasm_put(Dst, 184, (label));
#line 2290 "src/jit/x64/emit.dasc"
}

void MVM_jit_emit_branch(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMint32 label) {
    //| jmp =>(label);
    dasm_put(Dst, 3064, (label));
#line 2294 "src/jit/x64/emit.dasc"
}

void MVM_jit_emit_conditional_branch(MVMThreadContext *tc, MVMJitCompiler *compiler,
                                     MVMint32 cond, MVMint32 label) {
    switch (cond) {
    case MVM_JIT_LT:
        //| jl =>(label);
        dasm_put(Dst, 3215, (label));
#line 2301 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_LE:
        //| jle =>(label);
        dasm_put(Dst, 3211, (label));
#line 2304 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_EQ:
        //| je =>(label);
        dasm_put(Dst, 3086, (label));
#line 2307 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_NE:
        //| jne =>(label);
        dasm_put(Dst, 3075, (label));
#line 2310 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_GE:
        //| jge =>(label);
        dasm_put(Dst, 3219, (label));
#line 2313 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_GT:
        //| jg =>(label);
        dasm_put(Dst, 3223, (label));
#line 2316 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_NZ:
        //| jnz =>(label);
        dasm_put(Dst, 3075, (label));
#line 2319 "src/jit/x64/emit.dasc"
        break;
    case MVM_JIT_ZR:
        //| jz =>(label);
        dasm_put(Dst, 3086, (label));
#line 2322 "src/jit/x64/emit.dasc"
        break;
    default:
        MVM_oops(tc, "this condition cannot be compiled with conditional_branch");
    }
}

void MVM_jit_emit_guard(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                        MVMJitGuard *guard) {
    MVMint16 op        = guard->ins->info->opcode;
    MVMint16 obj       = guard->ins->operands[0].reg.orig;
    MVMint16 spesh_idx = guard->ins->operands[1].lit_i16;
    MVM_jit_log(tc, "emit guard <%s>\n", guard->ins->info->name);
    /* load object and spesh slot value */
    //| mov TMP1, WORK[obj];
    //| get_spesh_slot TMP2, spesh_idx;
    dasm_put(Dst, 3227, Dt21([obj]), Dt20(->cur_frame), Dt2(->effective_spesh_slots), Dt15([spesh_idx]));
#line 2337 "src/jit/x64/emit.dasc"
    if (op == MVM_OP_sp_guard) {
        /* object in question should just match the type, so it shouldn't
         * be zero, and the STABLE should be equal to the value in the spesh
         * slot */
        /* check for null */
        //| test TMP1, TMP1;
        //| jz >1;
        dasm_put(Dst, 3244);
#line 2344 "src/jit/x64/emit.dasc"
        /* get stable and compare */
        //| cmp TMP2, OBJECT:TMP1->st;
        //| jne >1;
        dasm_put(Dst, 3252, DtE(->st));
#line 2347 "src/jit/x64/emit.dasc"
        /* we're good, no need to deopt */
    } else if (op == MVM_OP_sp_guardtype) {
        /* object in question should be a type object, so it shouldn't
         * be zero, should not be concrete, and the STABLE should be
         * equal to the value in the spesh slot */
        /* check for null */
        //| test TMP1, TMP1;
        //| jz >1;
        dasm_put(Dst, 3244);
#line 2355 "src/jit/x64/emit.dasc"
        /* check if type object (not concrete) */
        //| is_type_object TMP1;
        dasm_put(Dst, 3261, DtE(->header.flags), MVM_CF_TYPE_OBJECT);
#line 2357 "src/jit/x64/emit.dasc"
        /* if zero, this is a concrete object, and we should deopt */
        //| jz >1;
        dasm_put(Dst, 2629);
#line 2359 "src/jit/x64/emit.dasc"
        /* get stable and compare */
        //| cmp TMP2, OBJECT:TMP1->st;
        //| jne >1;
        dasm_put(Dst, 3252, DtE(->st));
#line 2362 "src/jit/x64/emit.dasc"
        /* we're good, no need to deopt */
    } else if (op == MVM_OP_sp_guardconc) {
        /* object should be a non-null concrete (non-type) object */
        //| test TMP1, TMP1;
        //| jz >1;
        dasm_put(Dst, 3244);
#line 2367 "src/jit/x64/emit.dasc"
        /* shouldn't be type object */
        //| is_type_object TMP1;
        //| jnz >1;
        dasm_put(Dst, 3268, DtE(->header.flags), MVM_CF_TYPE_OBJECT);
#line 2370 "src/jit/x64/emit.dasc"
        /* should have our stable */
        //| cmp TMP2, OBJECT:TMP1->st;
        //| jne >1;
        dasm_put(Dst, 3252, DtE(->st));
#line 2373 "src/jit/x64/emit.dasc"
    } else if (op == MVM_OP_sp_guardsf) {
        /* Should be an MVMCode */
        MVMint32 reprid = MVM_REPR_ID_MVMCode;
        //| mov TMP3, OBJECT:TMP1->st;
        //| mov TMP3, STABLE:TMP3->REPR;
        //| cmp qword REPR:TMP3->ID, reprid;
        //| jne >1;
        //| cmp TMP2, CODE:TMP1->body.sf;
        //| jne >1;
        dasm_put(Dst, 3279, DtE(->st), Dt12(->REPR), Dt13(->ID), reprid, Dt1B(->body.sf));
#line 2382 "src/jit/x64/emit.dasc"
    }
    /* if we're here, we didn't jump to deopt, so skip it */
    //| jmp >2;
    //|1:
    dasm_put(Dst, 1927);
#line 2386 "src/jit/x64/emit.dasc"
    /* emit deopt */
    //| mov ARG1, TC;
    //| mov ARG2, guard->deopt_offset;
    //| mov ARG3, guard->deopt_target;
    //| callp &MVM_spesh_deopt_one_direct;
    dasm_put(Dst, 3305, guard->deopt_offset, guard->deopt_target);
    dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_spesh_deopt_one_direct)), (MVMuint32)((uintptr_t)(&MVM_spesh_deopt_one_direct) >> 32));
    dasm_put(Dst, 205);
#line 2391 "src/jit/x64/emit.dasc"
    /* jump out */
    //| jmp ->exit;
    //|2:
    dasm_put(Dst, 3319);
#line 2394 "src/jit/x64/emit.dasc"
}

void MVM_jit_emit_invoke(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg, MVMJitInvoke *invoke) {
    MVMint16 i;
    MVMuint16 callsite_idx = invoke->callsite_idx;
    MVM_jit_log(tc, "Emit invoke (%d args)\n", invoke->arg_count);
    /* Store callsite in tmp6, which we use at the end of invoke */
    //| mov TMP6, CU->body.callsites;
    //| mov TMP6, CALLSITEPTR:TMP6[callsite_idx];
    dasm_put(Dst, 2824, Dt22(->body.callsites), Dt4([callsite_idx]));
#line 2403 "src/jit/x64/emit.dasc"

    /* Store callsite in the frame. I use TMP5 as it never conflicts
     * with argument passing (like TMP6, but unlike other TMP regs) */
    //| mov TMP5, TC->cur_frame;
    //| mov FRAME:TMP5->cur_args_callsite, TMP6;
    dasm_put(Dst, 3326, Dt20(->cur_frame), Dt2(->cur_args_callsite));
#line 2408 "src/jit/x64/emit.dasc"

    /* Setup the frame for returning to our current position */
    if (sizeof(MVMReturnType) == 1) {
        //| mov byte FRAME:TMP5->return_type, invoke->return_type;
        dasm_put(Dst, 3335, Dt2(->return_type), invoke->return_type);
#line 2412 "src/jit/x64/emit.dasc"
    } else {
        MVM_panic(1, "JIT: MVMReturnType has unexpected size");
    }
    /* The register for our return value */
    if (invoke->return_type == MVM_RETURN_VOID) {
        //| mov aword FRAME:TMP5->return_value, NULL;
        dasm_put(Dst, 3341, Dt2(->return_value), NULL);
#line 2418 "src/jit/x64/emit.dasc"
    } else {
        //| lea TMP2, WORK[invoke->return_register];
        //| mov aword FRAME:TMP5->return_value, TMP2;
        dasm_put(Dst, 3347, Dt21([invoke->return_register]), Dt2(->return_value));
#line 2421 "src/jit/x64/emit.dasc"
    }
    /* The return address for the interpreter */
    //| get_cur_op TMP2;
    //| mov aword FRAME:TMP5->return_address, TMP2;
    dasm_put(Dst, 3356, Dt20(->interp_cur_op), Dt2(->return_address));
#line 2425 "src/jit/x64/emit.dasc"

    /* The re-entry label for the JIT, so that we continue in the next BB */
    //| lea TMP2, [=>(invoke->reentry_label)];
    //| mov aword FRAME:TMP5->jit_entry_label, TMP2;
    dasm_put(Dst, 3368, (invoke->reentry_label), Dt2(->jit_entry_label));
#line 2429 "src/jit/x64/emit.dasc"

    /* Install invoke args */
    //| mov TMP5, FRAME:TMP5->args;
    dasm_put(Dst, 173, Dt2(->args));
#line 2432 "src/jit/x64/emit.dasc"
    for (i = 0;  i < invoke->arg_count; i++) {
        MVMSpeshIns *ins = invoke->arg_ins[i];
        switch (ins->info->opcode) {
        case MVM_OP_arg_i:
        case MVM_OP_arg_s:
        case MVM_OP_arg_n:
        case MVM_OP_arg_o: {
            MVMint16 dst = ins->operands[0].lit_i16;
            MVMint16 src = ins->operands[1].reg.orig;
            //| mov TMP4, WORK[src];
            //| mov REGISTER:TMP5[dst], TMP4;
            dasm_put(Dst, 3377, Dt21([src]), Dt1([dst]));
#line 2443 "src/jit/x64/emit.dasc"
            break;
        }
        case MVM_OP_argconst_n:
        case MVM_OP_argconst_i: {
            MVMint16 dst = ins->operands[0].lit_i16;
            MVMint64 val = ins->operands[1].lit_i64;
            //| mov64 TMP4, val;
            //| mov REGISTER:TMP5[dst], TMP4;
            dasm_put(Dst, 3386, (unsigned int)(val), (unsigned int)((val)>>32), Dt1([dst]));
#line 2451 "src/jit/x64/emit.dasc"
            break;
        }
        case MVM_OP_argconst_s: {
            MVMint16 dst = ins->operands[0].lit_i16;
            MVMint32 idx = ins->operands[1].lit_str_idx;
            //| get_string TMP4, idx;
             MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, idx);
#line 2457 "src/jit/x64/emit.dasc"
            //| mov REGISTER:TMP5[dst], TMP4;
            dasm_put(Dst, 3395, Dt22(->body.strings), Dt14([idx]), Dt1([dst]));
#line 2458 "src/jit/x64/emit.dasc"
            break;
        }
        default:
            MVM_panic(1, "JIT invoke: Can't add arg <%s>",
                      ins->info->name);
        }
    }

    /* if we're not fast, then we should get the code from multi resolution */
    if (!invoke->is_fast) {
        /* first, save callsite and args */
        //| mov qword [rbp-0x28], TMP5; // args
        //| mov qword [rbp-0x30], TMP6; // callsite
        dasm_put(Dst, 3408);
#line 2471 "src/jit/x64/emit.dasc"
        /* setup call MVM_frame_multi_ok(tc, code, &cur_callsite, args); */
        //| mov ARG1, TC;
        //| mov ARG2, WORK[invoke->code_register]; // code object
        //| lea ARG3, [rbp-0x30];                  // &cur_callsite
        //| mov ARG4, TMP5;                        // args
        //|.if WIN32;
        //| mov qword [rsp+0x20], 0
        //|.else;
        //| mov ARG5, 0;                           // NULL to &was_multi
        //|.endif
        //| callp &MVM_frame_find_invokee_multi_ok;
        dasm_put(Dst, 3417, Dt21([invoke->code_register]));
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_frame_find_invokee_multi_ok)), (MVMuint32)((uintptr_t)(&MVM_frame_find_invokee_multi_ok) >> 32));
        dasm_put(Dst, 205);
#line 2482 "src/jit/x64/emit.dasc"
        /* restore callsite, args, RV now holds code object */
        //| mov TMP6, [rbp-0x30]; // callsite
        //| mov TMP5, [rbp-0x28]; // args
        dasm_put(Dst, 3441);
#line 2485 "src/jit/x64/emit.dasc"
        /* setup args for call to invoke(tc, code, cur_callsite, args) */
        //| mov ARG1, TC;
        //| mov ARG2, RV;   // code object
        //| mov ARG3, TMP6; // callsite
        //| mov ARG4, TMP5; // args
        dasm_put(Dst, 3450);
#line 2490 "src/jit/x64/emit.dasc"
        /* get the actual function */
        //| mov FUNCTION, OBJECT:RV->st;
        //| mov FUNCTION, STABLE:FUNCTION->invoke;
        //| call FUNCTION;
        dasm_put(Dst, 3464, DtE(->st), Dt12(->invoke));
#line 2494 "src/jit/x64/emit.dasc"
    } else {
        /* call MVM_frame_invoke_code */
        //| mov ARG1, TC;
        //| mov ARG2, WORK[invoke->code_register];
        //| mov ARG3, TMP6; // this is the callsite object
        //| mov ARG4, invoke->spesh_cand;
        //| callp &MVM_frame_invoke_code;
        dasm_put(Dst, 3477, Dt21([invoke->code_register]), invoke->spesh_cand);
        dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(&MVM_frame_invoke_code)), (MVMuint32)((uintptr_t)(&MVM_frame_invoke_code) >> 32));
        dasm_put(Dst, 205);
#line 2501 "src/jit/x64/emit.dasc"
    }
    /* Almost done. jump out into the interprete */
    //| jmp ->exit;
    dasm_put(Dst, 3059);
#line 2504 "src/jit/x64/emit.dasc"
}

void MVM_jit_emit_jumplist(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitGraph *jg,
                           MVMJitJumpList *jumplist) {
    MVMint32 i;
    MVM_jit_log(tc, "Emit jumplist (%"PRId64" labels)\n", jumplist->num_labels);
    //| mov TMP1, WORK[jumplist->reg];
    //| cmp TMP1, 0;
    //| jl >2;
    //| cmp TMP1, jumplist->num_labels;
    //| jge >2;
    //| imul TMP1, 0x8; // 8 bytes per goto
    //| lea TMP2, [>1];
    //| add TMP2, TMP1;
    //| jmp TMP2;
    //|.align 8;
    //|1:
    dasm_put(Dst, 3494, Dt21([jumplist->reg]), jumplist->num_labels);
#line 2521 "src/jit/x64/emit.dasc"
    for (i = 0; i < jumplist->num_labels; i++) {
        //|=>(jumplist->in_labels[i]):
        //| jmp =>(jumplist->out_labels[i]);
        //|.align 8;
        dasm_put(Dst, 3536, (jumplist->in_labels[i]), (jumplist->out_labels[i]));
#line 2525 "src/jit/x64/emit.dasc"
    }
    //|2:
    dasm_put(Dst, 287);
#line 2527 "src/jit/x64/emit.dasc"
}

void MVM_jit_emit_control(MVMThreadContext *tc, MVMJitCompiler *compiler,
                          MVMJitControl *ctrl, MVMJitTile *tile) {
    /* TODO - rename invokish and pre and post invokish into something that makes more sense */
    MVMJitControlType type = (tile != NULL ? tile->args[0] : ctrl->type);
    MVMint8 tmp = (tile != NULL ? tile->values[0] : MVM_JIT_REG(RCX));
    if (type == MVM_JIT_CONTROL_INVOKISH) {
        MVM_jit_log(tc, "Emit invokish control guard\n");
        /* This relies on the fact that MVM_JIT_THROWISH_PRE has already
         * assigned the jit entry label */
        //| mov eax, dword TC->current_frame_nr;
        //| cmp eax, FRAME_NR;
        //| je >9;
        //| jmp ->exit;
        //|9:
        dasm_put(Dst, 3543, Dt20(->current_frame_nr));
#line 2543 "src/jit/x64/emit.dasc"
    }
    else if (type == MVM_JIT_CONTROL_DYNAMIC_LABEL) {
        MVM_jit_log(tc, "Emit dynamic label\n");
        /* This pre-loads a label for the next op, so that throwish
         * operators will know where we're throwing from - NB, I'm not
         * totally sure this is still necessary now */
        //| lea rax, [>1];
        //| mov Rq(tmp), TC->cur_frame;
        //| mov aword FRAME:Rq(tmp)->jit_entry_label, rax;
        //|1:
        dasm_put(Dst, 3561, (tmp), Dt20(->cur_frame), (tmp), Dt2(->jit_entry_label));
#line 2553 "src/jit/x64/emit.dasc"
    }
    else if (type == MVM_JIT_CONTROL_THROWISH_PRE) {
        MVM_jit_log(tc, "Emit throwish pre\n");
        /* Store a reference to the 9 label below. I assume this label isn't
         * used in the intermediate instructions, which is basically valid
         * throughout this codebase. */
        //| lea Rq(tmp), [>9];
        //| mov rax, TC->cur_frame;
        //| mov aword FRAME:rax->jit_entry_label, Rq(tmp);
        dasm_put(Dst, 3585, (tmp), Dt20(->cur_frame), (tmp), Dt2(->jit_entry_label));
#line 2562 "src/jit/x64/emit.dasc"
    }
    else if (type == MVM_JIT_CONTROL_THROWISH_POST) {
        MVM_jit_log(tc, "Emit throwish post\n");
        /* check if our current frame is the same as it was */
        //| mov eax, dword TC->current_frame_nr;
        //| cmp eax, FRAME_NR;
        //| jne >8;
        dasm_put(Dst, 3606, Dt20(->current_frame_nr));
#line 2569 "src/jit/x64/emit.dasc"
        /* if it is, we may still be in a goto-handler, so in that
         * case the throwing machinery should have ensured that
         * jit_entry_label now points to the correct label. If not, it
         * still points to 9:. */
        //| mov rax, TC->cur_frame;
        //| jmp aword FRAME:rax->jit_entry_label;
        dasm_put(Dst, 3618, Dt20(->cur_frame), Dt2(->jit_entry_label));
#line 2575 "src/jit/x64/emit.dasc"
        /* if not the same frame, trampoline to interpreter */
        //|8:
        //| jmp ->exit;
        dasm_put(Dst, 3627);
#line 2578 "src/jit/x64/emit.dasc"
        /* I assume we *never* use a local label 9 in the code between
         * pre-and-post throwish */
        //|9:
        dasm_put(Dst, 3558);
#line 2581 "src/jit/x64/emit.dasc"
    } else if (type == MVM_JIT_CONTROL_BREAKPOINT) {
        /* brrts evil testing purposes breakpoint */
        //| int 3;
        dasm_put(Dst, 3634);
#line 2584 "src/jit/x64/emit.dasc"
    } else {
        MVM_panic(1, "Unknown control code: <%s>", ctrl->ins->info->name);
    }
}


void MVM_jit_emit_load(MVMThreadContext *tc, MVMJitCompiler *compiler,
                       MVMint32 reg_cls, MVMint8 reg_dst,
                       MVMint32 mem_cls, MVMint32 mem_src, MVMint32 size) {
    MVMint8 mem_base;
    if (mem_cls == MVM_JIT_STORAGE_LOCAL) {
        mem_base = MVM_JIT_REG(RBX);
    } else if (mem_cls == MVM_JIT_STORAGE_STACK) {
        mem_base = MVM_JIT_REG(RSP);
    } else {
        MVM_panic(1, "Cannot refer to this memory class: %d", mem_cls);
    }
    if (reg_cls == MVM_JIT_STORAGE_GPR) {
        switch(size) {
        case 1:
            //| mov Rb(reg_dst), byte [Rq(mem_base)+mem_src];
            dasm_put(Dst, 3637, (reg_dst), (mem_base), mem_src);
#line 2605 "src/jit/x64/emit.dasc"
            break;
        case 2:
            //| mov Rw(reg_dst), word [Rq(mem_base)+mem_src];
            dasm_put(Dst, 3648, (reg_dst), (mem_base), mem_src);
#line 2608 "src/jit/x64/emit.dasc"
            break;
        case 4:
            //| mov Rd(reg_dst), dword [Rq(mem_base)+mem_src];
            dasm_put(Dst, 3649, (reg_dst), (mem_base), mem_src);
#line 2611 "src/jit/x64/emit.dasc"
            break;
        case 8:
            //| mov Rq(reg_dst), qword [Rq(mem_base)+mem_src];
            dasm_put(Dst, 3659, (reg_dst), (mem_base), mem_src);
#line 2614 "src/jit/x64/emit.dasc"
            break;
        }
    }
}

void MVM_jit_emit_store(MVMThreadContext *tc, MVMJitCompiler *compiler,
                        MVMint32 mem_cls, MVMint32 mem_dst,
                        MVMint32 reg_cls, MVMint8 reg_src, MVMint32 size) {

    MVMint8 mem_base;
    if (mem_cls == MVM_JIT_STORAGE_LOCAL) {
        mem_base = MVM_JIT_REG(RBX);
    } else if (mem_cls == MVM_JIT_STORAGE_STACK) {
        mem_base = MVM_JIT_REG(RSP);
    } else {
        MVM_panic(1, "Cannot refer to this memory class: %d", mem_cls);
    }
    if (reg_cls == MVM_JIT_STORAGE_GPR) {
        switch (size) {
        case 1:
            //| mov byte [Rq(mem_base)+mem_dst], Rb(reg_src);
            dasm_put(Dst, 3670, (reg_src), (mem_base), mem_dst);
#line 2635 "src/jit/x64/emit.dasc"
            break;
        case 2:
            //| mov word [Rq(mem_base)+mem_dst], Rw(reg_src);
            dasm_put(Dst, 3681, (reg_src), (mem_base), mem_dst);
#line 2638 "src/jit/x64/emit.dasc"
            break;
        case 4:
            //| mov dword [Rq(mem_base)+mem_dst], Rd(reg_src);
            dasm_put(Dst, 3682, (reg_src), (mem_base), mem_dst);
#line 2641 "src/jit/x64/emit.dasc"
            break;
        case 8:
            //| mov qword [Rq(mem_base)+mem_dst], Rq(reg_src);
            dasm_put(Dst, 3692, (reg_src), (mem_base), mem_dst);
#line 2644 "src/jit/x64/emit.dasc"
            break;
        }
    }
}

void MVM_jit_emit_copy(MVMThreadContext *tc, MVMJitCompiler *compiler,
                       MVMint32 dst_cls,  MVMint8 dst_reg, MVMint32 src_cls, MVMint8 src_reg) {
    if (dst_cls == src_cls) {
        if (dst_cls == MVM_JIT_STORAGE_GPR) {
            //| mov Rq(dst_reg), Rq(src_reg);
            dasm_put(Dst, 3703, (src_reg), (dst_reg));
#line 2654 "src/jit/x64/emit.dasc"
        } else {
            MVM_oops(tc, "numeric regs nyi");
        }
    } else {
        MVM_oops(tc, "numeric regs nyi");
    }
}


void MVM_jit_emit_marker(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMint32 num) {
    MVMint32 i;
    for (i = 0; i < num; i++) {
        //| nop;
        dasm_put(Dst, 3712);
#line 2667 "src/jit/x64/emit.dasc"
    }
}


void MVM_jit_emit_data(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitData *data) {
    MVMuint8 *bytes = data->data;
    MVMint32 i;
    //|.data;
    dasm_put(Dst, 197);
#line 2675 "src/jit/x64/emit.dasc"
    //|=>(data->label):
    dasm_put(Dst, 184, (data->label));
#line 2676 "src/jit/x64/emit.dasc"
    for (i = 0; i < data->size; i++) {
        //|.byte bytes[i];
        dasm_put(Dst, 2498, bytes[i]);
#line 2678 "src/jit/x64/emit.dasc"
    }
    //|.code
    dasm_put(Dst, 0);
#line 2680 "src/jit/x64/emit.dasc"
}

void MVM_jit_emit_save_rv(MVMThreadContext *tc, MVMJitCompiler *compiler) {
    //| mov qword SAVED_RV, RV;
    dasm_put(Dst, 3714);
#line 2684 "src/jit/x64/emit.dasc"
}

/* import tiles */
//|.include src/jit/x64/tiles.dasc
#line 1 "src/jit/x64/tiles.dasc"
/* -*-C-*- */
#include "tile_decl.h"

/* NB: The rax/eax/ax/al/ah register is *reserved* for internal use in tiles by
 * the register allocator. Using rax will never overwrite an allocated value */

/* basic memory traffic tiles */
MVM_JIT_TILE_DECL(addr) {
    MVMint8 out  = tile->values[0];
    MVMint8 base = tile->values[1];
    MVMint32 ofs = tile->args[0];
    //| lea Rq(out), [Rq(base)+ofs];
    dasm_put(Dst, 3719, (out), (base), ofs);
#line 13 "src/jit/x64/tiles.dasc"
}


MVM_JIT_TILE_DECL(idx) {
    MVMint8 out  = tile->values[0];
    MVMint8 base = tile->values[1];
    MVMint8 idx  = tile->values[2];
    MVMint8 scl  = tile->args[0];
    if (scl == 8) {
        //| lea Rq(out), [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 3730, (out), (idx), (base), 0);
#line 23 "src/jit/x64/tiles.dasc"
    } else {
        MVM_oops(tc, "Scales other than 8 NYI\n");
    }
}


MVM_JIT_TILE_DECL(const_reg) {
    MVMint8 out = tile->values[0];
    MVMint64 val  = tile->args[0];
    MVMint32 size = tile->args[1];
    if (size == 8 && !fits_in_32_bit(val)) {
        //| mov64 Rq(out), val;
        dasm_put(Dst, 3744, (out), (unsigned int)(val), (unsigned int)((val)>>32));
#line 35 "src/jit/x64/tiles.dasc"
    } else {
        //| mov Rq(out), val;
        dasm_put(Dst, 3752, (out), val);
#line 37 "src/jit/x64/tiles.dasc"
    }
}


MVM_JIT_TILE_DECL(load_reg) {
    MVMint8 out  = tile->values[0];
    MVMint8 base = tile->values[1];
    MVMint32 size = tile->args[0];
    switch (size) {
    case 1:
        //| mov Rb(out), [Rq(base)];
        dasm_put(Dst, 3760, (out), (base), 0);
#line 48 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| mov Rw(out), [Rq(base)];
        dasm_put(Dst, 3648, (out), (base), 0);
#line 51 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| mov Rd(out), [Rq(base)];
        dasm_put(Dst, 3649, (out), (base), 0);
#line 54 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| mov Rq(out), [Rq(base)];
        dasm_put(Dst, 3659, (out), (base), 0);
#line 57 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}

MVM_JIT_TILE_DECL(load_addr) {
    MVMint8 out  = tile->values[0];
    MVMint8 base = tile->values[1];
    MVMint32 ofs  = tile->args[0];
    MVMint32 size = tile->args[1];
    switch (tile->args[1]) {
    case 1:
        //| mov Rb(out), byte [Rq(base)+ofs];
        dasm_put(Dst, 3637, (out), (base), ofs);
#line 71 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| mov Rw(out), word [Rq(base)+ofs];
        dasm_put(Dst, 3648, (out), (base), ofs);
#line 74 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| mov Rd(out), dword [Rq(base)+ofs];
        dasm_put(Dst, 3649, (out), (base), ofs);
#line 77 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| mov Rq(out), qword [Rq(base)+ofs];
        dasm_put(Dst, 3659, (out), (base), ofs);
#line 80 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}

MVM_JIT_TILE_DECL(load_idx) {
    MVMint8 out  = tile->values[0];
    MVMint8 base = tile->values[1];
    MVMint8 idx  = tile->values[2];
    MVMint8 scl  = tile->args[0];
    MVMint32 size = tile->args[1];
    if (scl != 8) {
        MVM_oops(tc, "Unsupported scale size: %d\n", scl);
    }
    switch (size) {
    case 1:
        //| mov Rb(out), byte [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 3770, (out), (idx), (base), 0);
#line 98 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| mov Rw(out), word [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 3784, (out), (idx), (base), 0);
#line 101 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| mov Rd(out), dword [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 3785, (out), (idx), (base), 0);
#line 104 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| mov Rq(out), qword [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 3798, (out), (idx), (base), 0);
#line 107 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}


MVM_JIT_TILE_DECL(store) {
    MVMint8 base  = tile->values[1];
    MVMint8 value = tile->values[2];
    MVMint32 size = tile->args[0];
    switch (size) {
    case 1:
        //| mov byte [Rq(base)], Rb(value);
        dasm_put(Dst, 3670, (value), (base), 0);
#line 121 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| mov word [Rq(base)], Rw(value);
        dasm_put(Dst, 3681, (value), (base), 0);
#line 124 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| mov dword [Rq(base)], Rd(value);
        dasm_put(Dst, 3682, (value), (base), 0);
#line 127 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| mov qword [Rq(base)], Rq(value);
        dasm_put(Dst, 3692, (value), (base), 0);
#line 130 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported store size: %d\n", size);
    }
}

MVM_JIT_TILE_DECL(store_addr) {
    MVMint8 base  = tile->values[1];
    MVMint8 value = tile->values[2];
    MVMint32 ofs  = tile->args[0];
    MVMint32 size = tile->args[1];
    switch (size) {
    case 1:
        //| mov byte [Rq(base)+ofs], Rb(value);
        dasm_put(Dst, 3670, (value), (base), ofs);
#line 144 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| mov word [Rq(base)+ofs], Rw(value);
        dasm_put(Dst, 3681, (value), (base), ofs);
#line 147 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| mov dword [Rq(base)+ofs], Rd(value);
        dasm_put(Dst, 3682, (value), (base), ofs);
#line 150 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| mov qword [Rq(base)+ofs], Rq(value);
        dasm_put(Dst, 3692, (value), (base), ofs);
#line 153 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported store size: %d\n", size);
    }
}

MVM_JIT_TILE_DECL(store_idx) {
    MVMint8 base = tile->values[1];
    MVMint8 idx  = tile->values[2];
    MVMint8 scl  = tile->args[0];
    MVMint32 size = tile->args[1];
    MVMint8 value = tile->values[3];
    if (scl != 8)
        MVM_oops(tc, "Scale %d NYI\n", scl);
    switch (size) {
    case 1:
        //| mov byte [Rq(base)+Rq(idx)*8], Rb(value);
        dasm_put(Dst, 3812, (value), (idx), (base), 0);
#line 170 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| mov word [Rq(base)+Rq(idx)*8], Rw(value);
        dasm_put(Dst, 3826, (value), (idx), (base), 0);
#line 173 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| mov dword [Rq(base)+Rq(idx)*8], Rd(value);
        dasm_put(Dst, 3827, (value), (idx), (base), 0);
#line 176 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| mov qword [Rq(base)+Rq(idx)*8], Rq(value);
        dasm_put(Dst, 3840, (value), (idx), (base), 0);
#line 179 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported store size: %d\n", size);
    }
}


MVM_JIT_TILE_DECL(cast) {
    MVMint32 to_size   = tile->args[0];
    MVMint32 from_size = tile->args[1];
    MVMint32 is_signed = tile->args[2];

    MVMint8  to_reg    = tile->values[0];
    MVMint8  from_reg  = tile->values[1];

    /* possible combinations: 1 -> 2, 4, 8; 2 -> 4, 8; 4 -> 8
     * Hence we can combine from_size | (to_size << 3) to get
     * the following options:
     * 1 + 2<<3 == 17
     * 1 + 4<<3 == 33
     * 1 + 8<<3 == 65
     * 2 + 4<<3 == 34
     * 2 + 8<<3 == 66
     * 4 + 8<<3 == 68
     */
    MVMint32 size_conv = (from_size) | (to_size << 3);
    if (is_signed == MVM_JIT_SIGNED) {
        switch (size_conv) {
        case 17:
            //| movsx Rw(to_reg), Rb(from_reg);
            dasm_put(Dst, 3854, (to_reg), (from_reg));
#line 209 "src/jit/x64/tiles.dasc"
            break;
        case 33:
            //| movsx Rd(to_reg), Rb(from_reg);
            dasm_put(Dst, 3855, (to_reg), (from_reg));
#line 212 "src/jit/x64/tiles.dasc"
            break;
        case 34:
            //| movsx Rd(to_reg), Rw(from_reg);
            dasm_put(Dst, 3865, (to_reg), (from_reg));
#line 215 "src/jit/x64/tiles.dasc"
            break;
        case 65:
            //| movsx Rq(to_reg), Rb(from_reg);
            dasm_put(Dst, 3874, (to_reg), (from_reg));
#line 218 "src/jit/x64/tiles.dasc"
            break;
        case 66:
            //| movsx Rq(to_reg), Rw(from_reg);
            dasm_put(Dst, 3884, (to_reg), (from_reg));
#line 221 "src/jit/x64/tiles.dasc"
            break;
        case 68:
            /* movsx is apparantly not defined for double-to-quadword conversions,
             * which forces us to use the rax register like it's 1978. It might be easier
             * to bithack the sign-extension manually, but I'm not sure how.. */
            //| mov eax, Rd(from_reg);
            //| cdqe;
            //| mov Rq(to_reg), rax;
            dasm_put(Dst, 3894, (from_reg), (to_reg));
#line 229 "src/jit/x64/tiles.dasc"
            break;
        default:
            MVM_oops(tc, "Unsuported signed cast %d -> %d\n", from_size, to_size);
        }
    } else {
        switch (size_conv) {
        case 17:
            //| movzx Rw(to_reg), Rb(from_reg);
            dasm_put(Dst, 3908, (to_reg), (from_reg));
#line 237 "src/jit/x64/tiles.dasc"
            break;
        case 33:
            //| movzx Rd(to_reg), Rb(from_reg);
            dasm_put(Dst, 3909, (to_reg), (from_reg));
#line 240 "src/jit/x64/tiles.dasc"
            break;
        case 34:
            //| movzx Rd(to_reg), Rw(from_reg);
            dasm_put(Dst, 3919, (to_reg), (from_reg));
#line 243 "src/jit/x64/tiles.dasc"
            break;
        case 65:
            //| movzx Rq(to_reg), Rb(from_reg);
            dasm_put(Dst, 3928, (to_reg), (from_reg));
#line 246 "src/jit/x64/tiles.dasc"
            break;
        case 66:
            //| movzx Rq(to_reg), Rw(from_reg);
            dasm_put(Dst, 3938, (to_reg), (from_reg));
#line 249 "src/jit/x64/tiles.dasc"
            break;
        case 68:
            /* In contrast, nothing special is necessary to cast unsigned
             * doublewords to quadwords, because using the lower 4 bytes
             * automatically clears the upper 4 */
            //| mov Rd(to_reg), Rd(from_reg);
            dasm_put(Dst, 3948, (from_reg), (to_reg));
#line 255 "src/jit/x64/tiles.dasc"
            break;
        default:
            MVM_oops(tc, "Unsuported unsigned cast %d -> %d\n", from_size, to_size);
        }
    }
}


MVM_JIT_TILE_DECL(cast_load_addr) {
    MVM_oops(tc, "NYI");
}

/* binary operations have special requirements because x86 is two-operand form, e.g:
 * r0 = r0 <op> r1
 * whereas the JIT uses a three-operand model:
 * r0 = r1 <op> r2 */

static void ensure_two_operand_pre(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitTile *tile, MVMint8 reg[2]) {
    MVMint8 out = tile->values[0], in1 = tile->values[1], in2 = tile->values[2];
    if (out == in1) {
        reg[0] = in1;
        reg[1] = in2;
    } else if (out == in2) {
        if (MVM_jit_expr_op_is_binary_noncommutative(tc, tile->op)) {
            //| mov rax, Rq(in1);
            dasm_put(Dst, 3956, (in1));
#line 280 "src/jit/x64/tiles.dasc"
            reg[0] = MVM_JIT_ARCH_X64_RAX;
            reg[1] = in2;
        } else {
            /* in this case, r2 <op> r1 == r0 <op> r1 */
            reg[0] = out;
            reg[1] = in1;
        }
    } else {
        /* insert a copy */
        //| mov Rq(out), Rq(in1);
        dasm_put(Dst, 3703, (in1), (out));
#line 290 "src/jit/x64/tiles.dasc"
        /* use r0, r2 */
        reg[0] = out;
        reg[1] = in2;
    }
}

static void ensure_two_operand_post(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitTile *tile, MVMint8 reg[2]) {
    MVMint8 out = tile->values[0];
    if (out != reg[0]) {
        /* insert a copy afterwards */
        //| mov Rq(out), Rq(reg[0]);
        dasm_put(Dst, 3703, (reg[0]), (out));
#line 301 "src/jit/x64/tiles.dasc"
    }
}


MVM_JIT_TILE_DECL(add_reg) {
    MVMint8 reg[2];
    ensure_two_operand_pre(tc, compiler, tile, reg);
    //| add Rq(reg[0]), Rq(reg[1]);
    dasm_put(Dst, 3963, (reg[1]), (reg[0]));
#line 309 "src/jit/x64/tiles.dasc"
    ensure_two_operand_post(tc, compiler, tile, reg);
}

MVM_JIT_TILE_DECL(add_const) {
    MVMint8 out = tile->values[0];
    MVMint8 in1  = tile->values[1];
    MVMint64 val = tile->args[0];
    MVMint32 sz  = tile->args[1];
    if (out == in1) {
        if (sz == 8 && !fits_in_32_bit(val)) {
            //| mov64 rax, val;
            //| add Rq(out), rax;
            dasm_put(Dst, 3972, (unsigned int)(val), (unsigned int)((val)>>32), (out));
#line 321 "src/jit/x64/tiles.dasc"
        } else {
            //| add Rq(in1), val;
            dasm_put(Dst, 3983, (in1), val);
#line 323 "src/jit/x64/tiles.dasc"
        }
    } else {
        if (sz == 8 && !fits_in_32_bit(val)) {
            //| mov64 Rq(out), val;
            //| add Rq(out), Rq(in1);
            dasm_put(Dst, 3991, (out), (unsigned int)(val), (unsigned int)((val)>>32), (in1), (out));
#line 328 "src/jit/x64/tiles.dasc"
        } else {
            //| mov Rq(out), Rq(in1);
            //| add Rq(out), val;
            dasm_put(Dst, 4007, (in1), (out), (out), val);
#line 331 "src/jit/x64/tiles.dasc"
        }
    }
}

MVM_JIT_TILE_DECL(add_load_addr) {
    MVMint8 out   = tile->values[0];
    MVMint8 in1   = tile->values[1];
    MVMint8 base  = tile->values[2];
    MVMint32 ofs  = tile->args[0];
    MVMint32 size = tile->args[1];
    if (in1 != out) {
        //| mov Rq(out), Rq(in1);
        dasm_put(Dst, 3703, (in1), (out));
#line 343 "src/jit/x64/tiles.dasc"
    }
    switch (size) {
    case 1:
        //| add Rb(out), byte [Rq(base)+ofs];
        dasm_put(Dst, 4023, (out), (base), ofs);
#line 347 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| add Rw(out), word [Rq(base)+ofs];
        dasm_put(Dst, 4034, (out), (base), ofs);
#line 350 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| add Rd(out), dword [Rq(base)+ofs];
        dasm_put(Dst, 4035, (out), (base), ofs);
#line 353 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| add Rq(out), qword [Rq(base)+ofs];
        dasm_put(Dst, 4045, (out), (base), ofs);
#line 356 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}

MVM_JIT_TILE_DECL(add_load_idx) {
    MVMint8 out  = tile->values[0];
    MVMint8 in1  = tile->values[1];
    MVMint8 base = tile->values[2];
    MVMint8 idx  = tile->values[3];
    MVMint32 scl  = tile->args[0];
    MVMint32 size = tile->args[1];
    if (in1 != out) {
        //| mov Rq(out), Rq(in1);
        dasm_put(Dst, 3703, (in1), (out));
#line 371 "src/jit/x64/tiles.dasc"
    }
    if (scl != 8)
        MVM_oops(tc, "IDX Scale %d NYI\n", scl);
    switch (size) {
    case 1:
        //| add Rb(out), byte [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4056, (out), (idx), (base), 0);
#line 377 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| add Rw(out), word [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4070, (out), (idx), (base), 0);
#line 380 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| add Rd(out), dword [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4071, (out), (idx), (base), 0);
#line 383 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| add Rq(out), qword [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4084, (out), (idx), (base), 0);
#line 386 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}


MVM_JIT_TILE_DECL(and_reg) {
    MVMint8 reg[2];
    ensure_two_operand_pre(tc, compiler, tile, reg);
    //| and Rq(reg[0]), Rq(reg[1]);
    dasm_put(Dst, 4098, (reg[1]), (reg[0]));
#line 397 "src/jit/x64/tiles.dasc"
    ensure_two_operand_post(tc, compiler, tile, reg);
}

MVM_JIT_TILE_DECL(and_const) {
    MVMint8 out = tile->values[0];
    MVMint8 in1  = tile->values[1];
    MVMint64 val = tile->args[0];
    MVMint32 sz  = tile->args[1];
    if (out == in1) {
        if (sz == 8 && !fits_in_32_bit(val)) {
            //| mov64 rax, val;
            //| and Rq(in1), rax;
            dasm_put(Dst, 4107, (unsigned int)(val), (unsigned int)((val)>>32), (in1));
#line 409 "src/jit/x64/tiles.dasc"
        } else {
            //| and Rq(in1), val;
            dasm_put(Dst, 4118, (in1), val);
#line 411 "src/jit/x64/tiles.dasc"
        }
    } else {
        if (sz == 8 && !fits_in_32_bit(val)) {
            //| mov64 Rq(out), val;
            //| and Rq(out), Rq(in1);
            dasm_put(Dst, 4126, (out), (unsigned int)(val), (unsigned int)((val)>>32), (in1), (out));
#line 416 "src/jit/x64/tiles.dasc"
        } else {
            //| mov Rq(out), Rq(in1);
            //| and Rq(out), val;
            dasm_put(Dst, 4142, (in1), (out), (out), val);
#line 419 "src/jit/x64/tiles.dasc"
        }
    }
}

MVM_JIT_TILE_DECL(and_load_addr) {
    MVMint8 out   = tile->values[0];
    MVMint8 in1   = tile->values[1];
    MVMint8 base  = tile->values[2];
    MVMint32 ofs  = tile->args[0];
    MVMint32 size = tile->args[1];
    if (in1 != out) {
        //| mov Rq(out), Rq(in1);
        dasm_put(Dst, 3703, (in1), (out));
#line 431 "src/jit/x64/tiles.dasc"
    }
    switch (size) {
    case 1:
        //| and Rb(out), byte [Rq(base)+ofs];
        dasm_put(Dst, 4158, (out), (base), ofs);
#line 435 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| and Rw(out), word [Rq(base)+ofs];
        dasm_put(Dst, 4169, (out), (base), ofs);
#line 438 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| and Rd(out), dword [Rq(base)+ofs];
        dasm_put(Dst, 4170, (out), (base), ofs);
#line 441 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| and Rq(out), qword [Rq(base)+ofs];
        dasm_put(Dst, 4180, (out), (base), ofs);
#line 444 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}

MVM_JIT_TILE_DECL(and_load_idx) {
    MVMint8 out  = tile->values[0];
    MVMint8 in1  = tile->values[1];
    MVMint8 base = tile->values[2];
    MVMint8 idx  = tile->values[3];
    MVMint32 scl  = tile->args[0];
    MVMint32 size = tile->args[1];

    if (out != in1) {
        //| mov Rq(out), Rq(in1);
        dasm_put(Dst, 3703, (in1), (out));
#line 460 "src/jit/x64/tiles.dasc"
    }
    if (scl != 8)
        MVM_oops(tc, "IDX Scale %d NYI\n", scl);
    switch (size) {
    case 1:
        //| and Rb(out), byte [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4191, (out), (idx), (base), 0);
#line 466 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| and Rw(out), word [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4205, (out), (idx), (base), 0);
#line 469 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| and Rd(out), dword [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4206, (out), (idx), (base), 0);
#line 472 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| and Rq(out), qword [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4219, (out), (idx), (base), 0);
#line 475 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}


MVM_JIT_TILE_DECL(sub_reg) {
    MVMint8 reg[2];
    ensure_two_operand_pre(tc, compiler, tile, reg);
    //| sub Rq(reg[0]), Rq(reg[1]);
    dasm_put(Dst, 4233, (reg[1]), (reg[0]));
#line 486 "src/jit/x64/tiles.dasc"
    ensure_two_operand_post(tc, compiler, tile, reg);
}

MVM_JIT_TILE_DECL(sub_const) {
    MVMint8 out = tile->values[0];
    MVMint8 in1  = tile->values[1];
    MVMint64 val = tile->args[0];
    MVMint32 sz  = tile->args[1];
    if (out == in1) {
        if (sz == 8 && !fits_in_32_bit(val)) {
            //| mov64 rax, val;
            //| sub Rq(in1), rax;
            dasm_put(Dst, 4242, (unsigned int)(val), (unsigned int)((val)>>32), (in1));
#line 498 "src/jit/x64/tiles.dasc"
        } else {
            //| sub Rq(in1), val;
            dasm_put(Dst, 4253, (in1), val);
#line 500 "src/jit/x64/tiles.dasc"
        }
    } else {
        if (sz == 8 && !fits_in_32_bit(val)) {
            //| mov64 rax, val;
            //| mov Rq(out), Rq(in1);
            //| sub Rq(out), rax;
            dasm_put(Dst, 4262, (unsigned int)(val), (unsigned int)((val)>>32), (in1), (out), (out));
#line 506 "src/jit/x64/tiles.dasc"
        } else {
            //| mov Rq(out), Rq(in1);
            //| sub Rq(out), val;
            dasm_put(Dst, 4281, (in1), (out), (out), val);
#line 509 "src/jit/x64/tiles.dasc"
        }
    }
}

MVM_JIT_TILE_DECL(sub_load_addr) {
    MVMint8 out   = tile->values[0];
    MVMint8 in1   = tile->values[1];
    MVMint8 base  = tile->values[2];
    MVMint32 ofs  = tile->args[0];
    MVMint32 size = tile->args[1];
    if (in1 != out) {
        //| mov Rq(out), Rq(in1);
        dasm_put(Dst, 3703, (in1), (out));
#line 521 "src/jit/x64/tiles.dasc"
    }
    switch (size) {
    case 1:
        //| sub Rb(out), byte [Rq(base)+ofs];
        dasm_put(Dst, 4298, (out), (base), ofs);
#line 525 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| sub Rw(out), word [Rq(base)+ofs];
        dasm_put(Dst, 4309, (out), (base), ofs);
#line 528 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| sub Rd(out), dword [Rq(base)+ofs];
        dasm_put(Dst, 4310, (out), (base), ofs);
#line 531 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| sub Rq(out), qword [Rq(base)+ofs];
        dasm_put(Dst, 4320, (out), (base), ofs);
#line 534 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}

MVM_JIT_TILE_DECL(sub_load_idx) {
    MVMint8 out  = tile->values[0];
    MVMint8 in1  = tile->values[1];
    MVMint8 base = tile->values[2];
    MVMint8 idx  = tile->values[3];
    MVMint32 scl  = tile->args[0];
    MVMint32 size = tile->args[1];
    if (out != in1) {
        //| mov Rq(out), Rq(in1);
        dasm_put(Dst, 3703, (in1), (out));
#line 549 "src/jit/x64/tiles.dasc"
    }
    if (scl != 8)
        MVM_oops(tc, "IDX Scale %d NYI\n", scl);
    switch (size) {
    case 1:
        //| sub Rb(out), byte [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4331, (out), (idx), (base), 0);
#line 555 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| sub Rw(out), word [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4345, (out), (idx), (base), 0);
#line 558 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| sub Rd(out), dword [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4346, (out), (idx), (base), 0);
#line 561 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| sub Rq(out), qword [Rq(base)+Rq(idx)*8];
        dasm_put(Dst, 4359, (out), (idx), (base), 0);
#line 564 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported load size: %d\n", size);
    }
}




MVM_JIT_TILE_DECL(test) {
    MVMint8 reg = tile->values[1];
    switch (tile->size) {
    case 1:
        //| test Rb(reg), Rb(reg);
        dasm_put(Dst, 4373, (reg), (reg));
#line 578 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| test Rw(reg), Rw(reg);
        dasm_put(Dst, 4382, (reg), (reg));
#line 581 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| test Rd(reg), Rd(reg);
        dasm_put(Dst, 4383, (reg), (reg));
#line 584 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| test Rq(reg), Rq(reg);
        dasm_put(Dst, 4391, (reg), (reg));
#line 587 "src/jit/x64/tiles.dasc"
        break;
    }
}



MVM_JIT_TILE_DECL(test_addr) {
    MVMint8 base  = tile->values[1];
    MVMint32 ofs  = tile->args[0];
    MVMint32 size = tile->args[1];
    switch (size) {
    case 1:
        //| cmp byte [Rq(base)+ofs], 0;
        dasm_put(Dst, 4400, (base), ofs);
#line 600 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| cmp word [Rq(base)+ofs], 0;
        dasm_put(Dst, 4410, (base), ofs);
#line 603 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| cmp dword [Rq(base)+ofs], 0;
        dasm_put(Dst, 4411, (base), ofs);
#line 606 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| cmp qword [Rq(base)+ofs], 0;
        dasm_put(Dst, 4420, (base), ofs);
#line 609 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported size %d for load\n", size);
    }
}


MVM_JIT_TILE_DECL(test_idx) {
    MVMint8 base = tile->values[1];
    MVMint8 idx  = tile->values[2];
    MVMint32 scl = tile->args[0];
    MVMint32 size = tile->args[1];
    if (scl != 8)
        MVM_oops(tc, "Scale %d NYI\n", scl);
    switch(size) {
    case 1:
        //| cmp byte [Rq(base)+Rq(idx)*8], 0;
        dasm_put(Dst, 4430, (idx), (base), 0);
#line 626 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| cmp word [Rq(base)+Rq(idx)*8], 0;
        dasm_put(Dst, 4443, (idx), (base), 0);
#line 629 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| cmp dword [Rq(base)+Rq(idx)*8], 0;
        dasm_put(Dst, 4444, (idx), (base), 0);
#line 632 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| cmp qword [Rq(base)+Rq(idx)*8], 0;
        dasm_put(Dst, 4456, (idx), (base), 0);
#line 635 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_oops(tc, "Unsupported size %d for load\n", tile->size);
    }
}

MVM_JIT_TILE_DECL(test_and) {
    MVMint8 rega = tile->values[1];
    MVMint8 regb = tile->values[2];
    switch(tile->size) {
    case 1:
        //| test Rb(regb), Rb(rega);
        dasm_put(Dst, 4373, (rega), (regb));
#line 647 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| test Rw(regb), Rw(rega);
        dasm_put(Dst, 4382, (rega), (regb));
#line 650 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| test Rd(regb), Rd(rega);
        dasm_put(Dst, 4383, (rega), (regb));
#line 653 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| test Rq(regb), Rq(rega);
        dasm_put(Dst, 4391, (rega), (regb));
#line 656 "src/jit/x64/tiles.dasc"
        break;
    }
}

MVM_JIT_TILE_DECL(test_const) {
    MVMint8  reg = tile->values[1];
    MVMint64 val = tile->args[0];
    switch(tile->size) {
    case 1:
        //| test Rb(reg), val;
        dasm_put(Dst, 4469, (reg), val);
#line 666 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| test Rw(reg), val;
        dasm_put(Dst, 4478, (reg), val);
#line 669 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| test Rd(reg), val;
        dasm_put(Dst, 4487, (reg), val);
#line 672 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        if (fits_in_32_bit(val)) {
            //| test Rq(reg), val;
            dasm_put(Dst, 4495, (reg), val);
#line 676 "src/jit/x64/tiles.dasc"
        } else {
            //| mov64 rax, val;
            //| test Rq(reg), rax;
            dasm_put(Dst, 4504, (unsigned int)(val), (unsigned int)((val)>>32), (reg));
#line 679 "src/jit/x64/tiles.dasc"
        }
        break;
    }
}

MVM_JIT_TILE_DECL(test_addr_const) {
    MVMint8  reg = tile->values[1];
    /* args: $ofs $lsize $val $csize */
    MVMint32 ofs = tile->args[0];
    MVMint64 val = tile->args[2];
    switch(tile->size) {
    case 1:
        //| test byte [Rq(reg)+ofs], val;
        dasm_put(Dst, 4515, (reg), ofs, val);
#line 692 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| test word [Rq(reg)+ofs], val;
        dasm_put(Dst, 4526, (reg), ofs, val);
#line 695 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| test dword [Rq(reg)+ofs], val;
        dasm_put(Dst, 4537, (reg), ofs, val);
#line 698 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        if (fits_in_32_bit(val)) {
            //| test qword [Rq(reg)+ofs], val;
            dasm_put(Dst, 4547, (reg), ofs, val);
#line 702 "src/jit/x64/tiles.dasc"
        } else {
            //| mov64 rax, val;
            //| test qword [Rq(reg)+ofs], rax;
            dasm_put(Dst, 4558, (unsigned int)(val), (unsigned int)((val)>>32), (reg), ofs);
#line 705 "src/jit/x64/tiles.dasc"
        }
        break;
    }
}


MVM_JIT_TILE_DECL(cmp) {
    MVMint8 regl = tile->values[1];
    MVMint8 regr = tile->values[2];
    switch (tile->size) {
    case 1:
        //| cmp Rb(regl), Rb(regr);
        dasm_put(Dst, 4571, (regr), (regl));
#line 717 "src/jit/x64/tiles.dasc"
        break;
    case 2:
        //| cmp Rw(regl), Rw(regr);
        dasm_put(Dst, 4580, (regr), (regl));
#line 720 "src/jit/x64/tiles.dasc"
        break;
    case 4:
        //| cmp Rd(regl), Rd(regr);
        dasm_put(Dst, 4581, (regr), (regl));
#line 723 "src/jit/x64/tiles.dasc"
        break;
    case 8:
        //| cmp Rq(regl), Rq(regr);
        dasm_put(Dst, 4589, (regr), (regl));
#line 726 "src/jit/x64/tiles.dasc"
        break;
    }
}

MVM_JIT_TILE_DECL(flagval) {
    MVMint8 out = tile->values[0];
    MVMint32 child = tree->nodes[tile->node + 1];
    MVMint32 flag  = tree->nodes[child];
    switch (flag) {
    case MVM_JIT_LT:
        //| setl Rb(out);
        dasm_put(Dst, 4598, (out));
#line 737 "src/jit/x64/tiles.dasc"
        break;
    case MVM_JIT_LE:
        //| setle Rb(out);
        dasm_put(Dst, 4606, (out));
#line 740 "src/jit/x64/tiles.dasc"
        break;
    case MVM_JIT_EQ:
        //| setz Rb(out);
        dasm_put(Dst, 4614, (out));
#line 743 "src/jit/x64/tiles.dasc"
        break;
    case MVM_JIT_NE:
        //| setnz Rb(out);
        dasm_put(Dst, 4622, (out));
#line 746 "src/jit/x64/tiles.dasc"
        break;
    case MVM_JIT_GE:
        //| setge Rb(out);
        dasm_put(Dst, 4630, (out));
#line 749 "src/jit/x64/tiles.dasc"
        break;
    case MVM_JIT_GT:
        //| setg Rb(out);
        dasm_put(Dst, 4638, (out));
#line 752 "src/jit/x64/tiles.dasc"
        break;
    default:
        MVM_panic(1, "No flagval possible");
        break;
    }
    /* XXX THIS IS A HACK

     * The size cast is supposed to be applied by the expression template
     * builder, but that subtly doesn't work (it's not applied for STORE
     * operands, and when it is, it causes even subtler errors with CONST
     * arguments. (const_i64_16 returns a 64 bit signed integer as a 16 bit
     * signed integer argument, and currently CONST doesn't have a sign, and the
     * tile yielding the value (fortunately) doesn't respect the size. */
    //| movzx Rq(out), Rb(out);
    dasm_put(Dst, 3928, (out), (out));
#line 766 "src/jit/x64/tiles.dasc"
}


MVM_JIT_TILE_DECL(mark) {
    MVMint32 label = tile->args[0];
    //|=>(label):
    dasm_put(Dst, 184, (label));
#line 772 "src/jit/x64/tiles.dasc"
}

MVM_JIT_TILE_DECL(label) {
    MVMint8 reg = tile->values[0];
    MVMint32 label = tile->args[0];
    //| lea Rq(reg), [=>label];
    dasm_put(Dst, 4646, (reg), label);
#line 778 "src/jit/x64/tiles.dasc"
}

MVM_JIT_TILE_DECL(branch_label) {
    MVMint32 label = tile->args[0];
    if (label >= 0) {
        //| jmp =>(label);
        dasm_put(Dst, 3064, (label));
#line 784 "src/jit/x64/tiles.dasc"
    } else {
        //| jmp ->exit;
        dasm_put(Dst, 3059);
#line 786 "src/jit/x64/tiles.dasc"
    }
}



static void move_call_value(MVMThreadContext *tc, MVMJitCompiler *compiler, MVMJitTile *tile) {
    if (MVM_JIT_TILE_YIELDS_VALUE(tile)) {
        MVMint8 out = tile->values[0];
        //| mov Rq(out), rax;
        dasm_put(Dst, 3901, (out));
#line 795 "src/jit/x64/tiles.dasc"
    }
}

MVM_JIT_TILE_DECL(call) {
    MVMint8 reg = tile->values[1];
    //| call Rq(reg);
    dasm_put(Dst, 4654, (reg));
#line 801 "src/jit/x64/tiles.dasc"
    move_call_value(tc, compiler, tile);
}

MVM_JIT_TILE_DECL(call_func) {
    MVMint64 ptr = tile->args[0];
    //| callp ptr;
    dasm_put(Dst, 197);
    dasm_put(Dst, 199, (MVMuint32)((uintptr_t)(ptr)), (MVMuint32)((uintptr_t)(ptr) >> 32));
    dasm_put(Dst, 205);
#line 807 "src/jit/x64/tiles.dasc"
    move_call_value(tc, compiler, tile);
}


MVM_JIT_TILE_DECL(call_addr) {
    MVMint8  reg = tile->values[1];
    MVMint32 ofs = tile->args[0];
    //| call qword [Rq(reg)+ofs];
    dasm_put(Dst, 4661, (reg), ofs);
#line 815 "src/jit/x64/tiles.dasc"
    move_call_value(tc, compiler, tile);
}
