/*
** This file has been pre-processed with DynASM.
** http://luajit.org/dynasm.html
** DynASM version 1.3.0, DynASM x64 version 1.3.0
** DO NOT EDIT! The original file is in "src/jit/emit_x64.dasc".
*/

#line 1 "src/jit/emit_x64.dasc"
#include "moar.h"
#include <dasm_proto.h>
#include <dasm_x86.h>
#include "emit.h"

#ifdef _MSC_VER
#pragma warning( disable : 4129 )
#endif

//|.arch x64
#if DASM_VERSION != 10300
#error "Version mismatch between DynASM and included encoding engine"
#endif
#line 11 "src/jit/emit_x64.dasc"
//|.actionlist actions
static const unsigned char actions[4015] = {
  85,72,137,229,255,72,129,252,236,0,1,0,0,255,76,137,117,252,248,76,137,109,
  252,240,76,137,101,232,72,137,93,224,255,73,137,252,254,73,137,252,245,77,
  139,166,233,73,139,156,253,36,233,255,252,255,226,255,248,10,72,199,192,0,
  0,0,0,248,11,255,76,139,117,252,248,76,139,109,252,240,76,139,101,232,72,
  139,93,224,255,72,137,252,236,93,195,255,72,185,237,237,72,137,139,233,255,
  72,199,131,233,237,255,73,139,141,233,72,139,137,233,72,137,139,233,255,73,
  139,142,233,72,139,137,233,72,137,139,233,255,72,139,139,233,72,139,137,233,
  255,72,139,137,233,73,139,150,233,72,139,146,233,72,133,201,72,15,68,202,
  255,77,137,227,255,77,139,155,233,255,77,139,147,233,255,77,139,146,233,255,
  77,133,210,15,133,244,247,255,76,137,252,247,76,137,222,72,199,194,237,73,
  186,237,237,65,252,255,210,255,73,137,194,248,1,255,76,137,147,233,255,76,
  137,252,247,255,73,139,181,233,72,139,182,233,72,199,194,237,73,186,237,237,
  65,252,255,210,72,133,192,15,132,244,247,72,139,0,248,1,72,137,131,233,255,
  76,137,225,255,72,139,137,233,72,139,147,233,72,137,145,233,255,73,139,140,
  253,36,233,72,139,137,233,72,137,139,233,255,72,139,139,233,72,141,145,233,
  76,139,137,233,77,141,145,233,77,133,201,73,15,69,210,255,76,139,2,77,133,
  192,255,15,133,244,249,255,77,139,134,233,77,139,128,233,248,3,255,76,139,
  2,255,77,133,192,15,133,244,250,255,77,139,132,253,36,233,77,139,128,233,
  255,102,252,247,129,233,236,15,149,208,73,131,252,248,0,15,149,212,132,196,
  15,149,208,102,65,252,247,128,233,236,15,148,212,132,196,15,132,244,249,72,
  137,85,216,76,137,69,208,76,137,252,247,72,139,179,233,73,186,237,237,65,
  252,255,210,76,139,69,208,72,139,85,216,248,3,255,76,137,2,248,4,255,76,139,
  2,77,133,192,15,133,244,250,255,76,137,252,247,73,139,180,253,36,233,72,139,
  182,233,73,186,237,237,65,252,255,210,73,137,192,255,102,252,247,129,233,
  236,15,149,208,73,131,252,248,0,15,149,212,132,196,15,149,208,102,65,252,
  247,128,233,236,15,148,212,132,196,15,132,244,249,72,137,85,216,76,137,69,
  208,76,137,252,247,72,139,179,233,73,186,237,237,65,252,255,210,76,139,69,
  208,72,139,85,216,248,3,76,137,2,255,76,137,131,233,255,72,139,139,233,72,
  139,147,233,255,102,252,247,129,233,236,15,149,208,72,131,252,250,0,15,149,
  212,132,196,15,149,208,102,252,247,130,233,236,15,148,212,132,196,255,15,
  132,244,248,76,137,252,247,72,139,179,233,73,186,237,237,65,252,255,210,72,
  139,139,233,72,139,147,233,248,2,255,72,139,139,233,72,139,145,233,72,137,
  147,233,255,72,139,139,233,72,139,147,233,72,139,137,233,72,137,17,255,72,
  139,139,233,76,139,129,233,73,139,16,72,137,147,233,255,72,139,139,233,72,
  139,147,233,76,141,129,233,73,131,184,233,0,15,132,244,247,77,139,128,233,
  248,1,255,102,252,247,129,233,236,15,149,208,72,131,252,250,0,15,149,212,
  132,196,15,149,208,102,252,247,130,233,236,15,148,212,132,196,15,132,244,
  248,72,137,85,216,76,137,69,208,76,137,252,247,72,139,179,233,73,186,237,
  237,65,252,255,210,76,139,69,208,72,139,85,216,248,2,255,73,137,144,233,255,
  72,139,139,233,72,137,139,233,255,72,139,139,233,73,137,142,233,255,73,139,
  142,233,72,137,139,233,73,199,134,233,237,255,73,139,140,253,36,233,72,137,
  139,233,255,73,139,140,253,36,233,72,133,201,15,132,244,247,72,139,137,233,
  248,1,72,137,139,233,255,73,139,141,233,255,72,199,131,233,0,0,0,0,255,72,
  139,139,233,72,133,201,15,148,210,72,15,182,210,72,137,147,233,255,72,129,
  131,233,239,255,72,129,171,233,239,255,72,129,139,233,239,255,72,129,163,
  233,239,255,72,129,179,233,239,255,72,139,131,233,255,72,1,131,233,255,72,
  41,131,233,255,72,9,131,233,255,72,33,131,233,255,72,49,131,233,255,72,129,
  192,239,255,72,129,232,239,255,72,129,200,239,255,72,129,224,239,255,72,129,
  252,240,239,255,72,3,131,233,255,72,43,131,233,255,72,11,131,233,255,72,35,
  131,233,255,72,51,131,233,255,72,15,175,131,233,255,138,139,233,72,211,224,
  255,138,139,233,72,211,232,255,72,139,139,233,72,133,201,15,133,244,247,76,
  137,252,247,72,190,237,237,73,186,237,237,65,252,255,210,248,1,255,72,139,
  131,233,72,153,72,252,247,252,249,255,73,137,192,73,131,232,1,72,131,252,
  248,0,15,156,213,72,133,210,15,149,209,132,205,73,15,69,192,72,137,131,233,
  255,72,131,131,233,1,255,72,131,171,233,1,255,72,139,139,233,72,252,247,209,
  72,137,139,233,255,72,139,139,233,72,252,247,217,72,137,139,233,255,252,242,
  15,16,131,233,255,252,242,15,88,131,233,255,252,242,15,92,131,233,255,252,
  242,15,89,131,233,255,252,242,15,94,131,233,255,252,242,15,17,131,233,255,
  252,242,72,15,42,131,233,252,242,15,17,131,233,255,252,242,72,15,44,131,233,
  72,137,131,233,255,72,199,193,1,0,0,0,72,193,225,63,72,139,147,233,72,49,
  202,72,137,147,233,255,72,59,131,233,255,15,148,208,255,15,149,208,255,15,
  156,208,255,15,158,208,255,15,159,208,255,15,157,208,255,72,15,182,192,72,
  137,131,233,255,72,139,139,233,255,72,59,139,233,255,15,159,210,72,15,182,
  210,65,15,156,208,77,15,182,192,76,41,194,72,137,147,233,255,72,199,194,1,
  0,0,0,255,72,199,194,252,255,252,255,252,255,252,255,255,72,199,194,0,0,0,
  0,255,72,59,147,233,255,72,199,193,0,0,0,0,255,72,199,193,1,0,0,0,255,252,
  242,15,16,131,233,102,15,46,131,233,255,15,147,209,255,15,155,210,72,15,68,
  202,255,15,154,210,72,15,68,202,255,15,151,209,255,72,15,182,201,72,137,139,
  233,255,252,242,15,16,131,233,252,242,15,16,139,233,102,15,46,193,15,151,
  209,72,15,182,193,102,15,46,200,15,151,209,72,15,182,201,72,41,200,72,137,
  131,233,255,72,199,199,237,72,139,179,233,72,139,147,233,73,186,237,237,65,
  252,255,210,255,72,129,252,248,239,15,148,208,255,72,129,252,248,239,15,149,
  208,255,72,139,139,233,72,133,201,15,132,244,247,72,139,137,233,72,139,137,
  233,72,129,185,233,239,15,133,244,247,72,199,131,233,1,0,0,0,252,233,244,
  248,248,1,72,199,131,233,0,0,0,0,248,2,255,72,139,139,233,72,139,145,233,
  72,131,194,1,76,139,129,233,255,76,57,194,15,156,209,72,15,182,201,72,137,
  139,233,255,72,139,139,233,72,139,145,233,72,133,210,15,149,210,72,15,182,
  210,72,137,147,233,255,76,139,155,233,77,133,219,15,132,244,247,73,139,179,
  233,76,139,150,233,77,139,146,233,76,137,252,247,65,252,255,210,76,15,183,
  152,233,248,1,76,137,155,233,255,76,139,155,233,77,133,219,15,132,244,247,
  73,139,179,233,76,139,150,233,77,139,146,233,76,137,252,247,65,252,255,210,
  76,15,183,152,233,77,133,219,15,132,244,247,76,15,183,152,233,248,1,76,137,
  155,233,255,72,139,139,233,72,133,201,15,149,210,77,139,134,233,77,139,128,
  233,76,57,193,65,15,149,208,68,32,194,72,15,182,210,72,137,147,233,255,72,
  139,139,233,72,133,201,15,148,210,77,139,134,233,77,139,128,233,76,57,193,
  65,15,148,208,68,8,194,72,15,182,210,72,137,147,233,255,76,137,252,247,72,
  199,198,237,73,186,237,237,65,252,255,210,73,139,140,253,36,233,72,139,137,
  233,72,137,136,233,102,199,128,233,236,65,139,142,233,137,136,233,72,137,
  131,233,255,76,139,147,233,77,133,210,255,15,132,244,247,102,65,252,247,130,
  233,236,255,15,133,244,247,77,139,154,233,77,139,155,233,77,133,219,255,15,
  132,244,247,76,137,252,247,76,137,214,72,141,147,233,77,139,147,233,65,252,
  255,210,252,233,244,248,248,1,255,76,137,147,233,248,2,255,72,139,139,233,
  72,133,201,15,132,244,247,72,139,137,233,72,139,137,233,72,133,201,248,1,
  15,149,209,72,15,182,201,72,137,139,233,255,73,139,140,253,36,233,198,129,
  233,1,255,73,139,140,253,36,233,72,139,137,233,72,139,147,233,72,139,146,
  233,72,57,209,15,133,244,247,77,139,132,253,36,233,77,139,128,233,76,137,
  131,233,252,233,244,248,248,1,76,137,252,247,72,139,179,233,255,73,139,149,
  233,72,139,146,233,72,199,193,237,76,141,155,233,77,137,216,73,186,237,237,
  65,252,255,210,72,133,192,15,132,244,248,255,72,199,192,1,0,0,0,72,141,13,
  244,248,73,137,140,253,36,233,252,233,244,11,248,2,255,72,139,139,233,72,
  133,201,15,132,244,247,102,252,247,129,233,236,15,133,244,247,72,199,131,
  233,1,0,0,0,252,233,244,248,248,1,72,199,131,233,0,0,0,0,248,2,255,73,139,
  142,233,72,137,139,233,73,199,134,233,0,0,0,0,255,72,139,139,233,72,133,201,
  15,132,244,247,102,252,247,129,233,236,15,133,244,247,72,139,145,233,72,139,
  146,233,72,129,186,233,239,15,133,244,247,72,139,145,233,72,137,147,233,252,
  233,244,248,248,1,255,76,137,252,247,72,190,237,237,73,186,237,237,65,252,
  255,210,255,73,139,142,233,72,133,201,15,132,244,247,72,139,9,72,139,137,
  233,72,137,139,233,252,233,244,248,248,1,73,139,142,233,72,139,137,233,72,
  137,139,233,248,2,255,65,139,142,233,131,193,1,65,137,142,233,72,137,139,
  233,255,65,139,142,233,131,252,233,1,65,137,142,233,72,137,139,233,255,72,
  139,179,233,76,139,150,233,77,139,146,233,77,133,210,15,133,244,247,76,137,
  252,247,72,190,237,237,73,186,237,237,65,252,255,210,248,1,76,137,252,247,
  72,139,147,233,255,76,137,252,247,72,139,179,233,72,199,194,237,73,186,237,
  237,65,252,255,210,72,133,192,15,132,244,247,72,139,0,248,1,72,137,131,233,
  255,73,141,180,253,36,233,102,68,139,150,233,102,68,59,150,233,15,132,244,
  247,76,137,252,247,73,186,237,237,65,252,255,210,248,1,255,72,139,139,233,
  72,133,201,15,133,244,247,76,137,252,247,73,186,237,237,65,252,255,210,248,
  1,255,76,137,252,247,73,139,180,253,36,233,72,199,194,237,73,186,237,237,
  65,252,255,210,255,76,137,252,247,73,139,180,253,36,233,72,139,182,233,72,
  199,194,237,73,186,237,237,65,252,255,210,255,72,139,139,233,139,145,233,
  77,49,192,131,252,250,0,15,142,244,247,77,139,134,233,77,139,128,233,77,139,
  4,208,77,139,128,233,248,1,76,137,131,233,255,102,252,247,129,233,236,15,
  133,244,247,72,139,145,233,72,139,146,233,129,186,233,239,15,132,244,248,
  248,1,255,76,137,252,247,72,190,237,237,73,186,237,237,65,252,255,210,248,
  2,255,76,137,252,247,72,139,179,233,72,49,210,73,186,237,237,65,252,255,210,
  255,72,137,198,255,72,141,187,233,73,137,188,253,36,233,255,65,199,132,253,
  36,233,237,255,73,139,190,233,72,139,63,73,137,188,253,36,233,255,76,137,
  252,247,72,139,147,233,72,139,138,233,72,139,137,233,72,139,146,233,76,139,
  150,233,77,139,146,233,65,252,255,210,255,76,137,252,247,72,190,237,237,73,
  186,237,237,65,252,255,210,248,2,72,139,145,233,72,15,183,146,233,72,137,
  147,233,255,76,137,252,247,72,190,237,237,73,186,237,237,65,252,255,210,248,
  2,72,139,145,233,102,68,139,130,233,102,139,146,233,102,68,57,194,15,149,
  208,72,15,182,192,72,137,131,233,255,77,139,134,233,255,73,186,237,237,65,
  252,255,210,72,131,252,248,0,15,141,244,247,255,76,137,252,247,72,137,198,
  73,186,237,237,65,252,255,210,255,139,128,233,248,1,72,137,131,233,255,77,
  137,252,243,255,77,137,252,235,255,77,141,156,253,36,233,255,77,139,156,253,
  36,233,255,76,139,155,233,255,76,141,155,233,255,77,139,157,233,77,139,155,
  233,255,73,199,195,237,255,73,187,237,237,255,76,139,155,233,77,139,155,233,
  255,76,139,155,233,77,141,155,233,255,76,137,223,255,76,137,222,255,76,137,
  218,255,76,137,217,255,77,137,216,255,77,137,217,255,102,73,15,110,195,255,
  102,73,15,110,203,255,102,73,15,110,211,255,102,73,15,110,219,255,102,73,
  15,110,227,255,102,73,15,110,252,235,255,102,73,15,110,252,243,255,102,73,
  15,110,252,251,255,68,136,156,253,36,233,255,102,68,137,156,253,36,233,255,
  76,137,156,253,36,233,255,72,139,8,72,137,139,233,255,72,139,139,233,72,137,
  8,255,73,131,190,233,0,15,132,244,247,76,137,252,247,73,186,237,237,65,252,
  255,210,248,1,255,252,233,244,10,255,252,233,245,255,72,139,131,233,72,133,
  192,15,133,245,255,72,139,131,233,72,133,192,15,132,245,255,102,72,15,110,
  131,233,102,15,87,201,102,15,46,193,15,138,245,15,133,245,255,102,72,15,110,
  131,233,102,15,87,201,102,15,46,193,15,138,244,247,15,133,244,247,252,233,
  245,248,1,255,76,137,252,247,72,139,179,233,73,186,237,237,65,252,255,210,
  72,133,192,255,72,139,139,233,72,133,201,15,132,244,247,73,139,150,233,72,
  139,146,233,72,57,209,15,132,244,247,252,233,245,248,1,255,76,137,252,247,
  72,139,179,233,72,139,147,233,255,73,139,141,233,72,139,137,233,73,186,237,
  237,65,252,255,210,255,72,131,252,248,252,255,255,15,142,245,255,72,139,139,
  233,73,139,148,253,36,233,72,139,146,233,255,72,133,201,15,132,244,247,255,
  102,252,247,129,233,236,255,72,59,145,233,15,133,244,247,255,102,252,247,
  129,233,236,15,133,244,247,255,72,133,201,15,132,244,247,102,252,247,129,
  233,236,15,133,244,247,76,139,145,233,76,57,210,15,133,244,247,77,139,146,
  233,77,139,146,233,72,137,206,76,137,252,247,72,141,85,216,65,252,255,210,
  76,139,69,216,77,133,192,15,132,244,247,102,65,252,247,128,233,236,15,133,
  244,247,73,139,144,233,73,139,140,253,36,233,72,139,137,233,255,72,57,209,
  15,133,244,247,255,72,133,201,15,132,244,247,102,252,247,129,233,236,15,133,
  244,247,76,139,145,233,76,57,210,15,133,244,247,77,139,146,233,77,139,146,
  233,72,137,206,76,137,252,247,72,141,85,216,65,252,255,210,76,139,69,216,
  77,133,192,15,132,244,247,102,65,252,247,128,233,236,255,15,132,244,247,73,
  139,144,233,73,139,140,253,36,233,72,139,137,233,72,57,209,15,133,244,247,
  255,72,133,201,15,132,244,247,102,252,247,129,233,236,15,133,244,247,255,
  76,139,145,233,76,57,210,15,133,244,247,77,139,146,233,77,139,146,233,72,
  137,206,76,137,252,247,65,252,255,210,255,72,133,192,15,132,244,247,255,72,
  139,139,233,76,139,145,233,77,139,146,233,77,139,146,233,255,72,137,206,76,
  137,252,247,72,141,85,216,65,252,255,210,255,76,139,69,216,77,133,192,15,
  132,244,247,102,65,252,247,128,233,236,15,133,244,247,73,139,144,233,73,139,
  140,253,36,233,72,139,137,233,72,57,209,15,133,244,247,255,72,133,201,15,
  132,244,247,102,252,247,129,233,236,15,133,244,247,76,139,145,233,76,57,210,
  15,133,244,247,255,77,139,146,233,77,139,146,233,72,137,206,76,137,252,247,
  65,252,255,210,72,133,192,15,132,244,247,72,139,139,233,76,139,145,233,77,
  139,146,233,77,139,146,233,72,137,206,76,137,252,247,72,141,85,216,255,65,
  252,255,210,76,139,69,216,255,77,133,192,15,132,244,247,102,65,252,247,128,
  233,236,15,132,244,247,73,139,144,233,73,139,140,253,36,233,72,139,137,233,
  72,57,209,15,133,244,247,255,76,137,252,247,72,199,198,237,72,199,194,237,
  73,186,237,237,65,252,255,210,255,72,199,192,237,252,233,244,11,248,2,255,
  76,137,252,247,76,137,252,238,72,199,194,237,73,186,237,237,65,252,255,210,
  73,137,195,255,77,139,148,253,36,233,255,76,139,139,233,77,137,138,233,255,
  73,185,237,237,77,137,138,233,255,77,139,141,233,77,139,137,233,77,137,138,
  233,255,73,199,132,253,36,233,237,255,72,141,147,233,73,137,148,253,36,233,
  255,73,139,150,233,72,139,18,73,137,148,253,36,233,255,72,141,21,245,73,137,
  148,253,36,233,255,76,137,85,216,76,137,93,208,255,76,137,252,247,72,139,
  179,233,72,141,85,208,76,137,209,73,186,237,237,65,252,255,210,255,76,139,
  93,208,76,139,85,216,255,76,137,252,247,72,137,198,76,137,218,76,137,209,
  255,76,139,144,233,77,139,146,233,65,252,255,210,255,76,137,252,247,72,139,
  179,233,76,137,218,72,199,193,237,73,186,237,237,65,252,255,210,255,72,199,
  192,1,0,0,0,252,233,244,11,255,72,139,139,233,72,131,252,249,0,15,140,244,
  248,72,129,252,249,239,15,141,244,248,72,107,201,8,72,141,21,244,247,72,1,
  202,252,255,226,250,7,248,1,255,249,252,233,245,250,7,255,77,59,166,233,15,
  132,244,247,72,141,13,244,247,73,137,140,253,36,233,72,199,192,1,0,0,0,252,
  233,244,11,248,1,255,72,141,13,244,247,73,137,140,253,36,233,248,1,255,72,
  141,13,244,255,73,137,140,253,36,233,255,77,59,166,233,15,133,244,254,255,
  65,252,255,164,253,36,233,255,248,8,72,199,192,1,0,0,0,252,233,244,11,255,
  248,9,255,205,3,255
};

#line 12 "src/jit/emit_x64.dasc"
//|.section code
#define DASM_SECTION_CODE	0
#define DASM_MAXSECTION		1
#line 13 "src/jit/emit_x64.dasc"
//|.globals MVM_JIT_LABEL_
enum {
  MVM_JIT_LABEL_exit,
  MVM_JIT_LABEL_out,
  MVM_JIT_LABEL__MAX
};
#line 14 "src/jit/emit_x64.dasc"

/* type declarations */
//|.type REGISTER, MVMRegister
#define Dt1(_V) (int)(ptrdiff_t)&(((MVMRegister *)0)_V)
#line 17 "src/jit/emit_x64.dasc"
//|.type ARGCTX, MVMArgProcContext
#define Dt2(_V) (int)(ptrdiff_t)&(((MVMArgProcContext *)0)_V)
#line 18 "src/jit/emit_x64.dasc"
//|.type CAPTURE, MVMCallCapture
#define Dt3(_V) (int)(ptrdiff_t)&(((MVMCallCapture *)0)_V)
#line 19 "src/jit/emit_x64.dasc"
//|.type CAPTUREBODY, MVMCallCaptureBody
#define Dt4(_V) (int)(ptrdiff_t)&(((MVMCallCaptureBody *)0)_V)
#line 20 "src/jit/emit_x64.dasc"
//|.type ARGPROCCONTEXT, MVMArgProcContext
#define Dt5(_V) (int)(ptrdiff_t)&(((MVMArgProcContext *)0)_V)
#line 21 "src/jit/emit_x64.dasc"
//|.type STATICFRAME, MVMStaticFrame
#define Dt6(_V) (int)(ptrdiff_t)&(((MVMStaticFrame *)0)_V)
#line 22 "src/jit/emit_x64.dasc"
//|.type P6OPAQUE, MVMP6opaque
#define Dt7(_V) (int)(ptrdiff_t)&(((MVMP6opaque *)0)_V)
#line 23 "src/jit/emit_x64.dasc"
//|.type P6OBODY, MVMP6opaqueBody
#define Dt8(_V) (int)(ptrdiff_t)&(((MVMP6opaqueBody *)0)_V)
#line 24 "src/jit/emit_x64.dasc"
//|.type MVMITER, MVMIter
#define Dt9(_V) (int)(ptrdiff_t)&(((MVMIter *)0)_V)
#line 25 "src/jit/emit_x64.dasc"
//|.type MVMINSTANCE, MVMInstance
#define DtA(_V) (int)(ptrdiff_t)&(((MVMInstance *)0)_V)
#line 26 "src/jit/emit_x64.dasc"
//|.type MVMACTIVEHANDLERS, MVMActiveHandler
#define DtB(_V) (int)(ptrdiff_t)&(((MVMActiveHandler *)0)_V)
#line 27 "src/jit/emit_x64.dasc"
//|.type OBJECT, MVMObject
#define DtC(_V) (int)(ptrdiff_t)&(((MVMObject *)0)_V)
#line 28 "src/jit/emit_x64.dasc"
//|.type STOOGE, MVMObjectStooge
#define DtD(_V) (int)(ptrdiff_t)&(((MVMObjectStooge *)0)_V)
#line 29 "src/jit/emit_x64.dasc"
//|.type COLLECTABLE, MVMCollectable
#define DtE(_V) (int)(ptrdiff_t)&(((MVMCollectable *)0)_V)
#line 30 "src/jit/emit_x64.dasc"
//|.type STABLE, MVMSTable
#define DtF(_V) (int)(ptrdiff_t)&(((MVMSTable *)0)_V)
#line 31 "src/jit/emit_x64.dasc"
//|.type REPR, MVMREPROps
#define Dt10(_V) (int)(ptrdiff_t)&(((MVMREPROps *)0)_V)
#line 32 "src/jit/emit_x64.dasc"
//|.type LEXOTIC, MVMLexotic
#define Dt11(_V) (int)(ptrdiff_t)&(((MVMLexotic *)0)_V)
#line 33 "src/jit/emit_x64.dasc"
//|.type STRING, MVMString*
#define Dt12(_V) (int)(ptrdiff_t)&(((MVMString* *)0)_V)
#line 34 "src/jit/emit_x64.dasc"
//|.type OBJECTPTR, MVMObject*
#define Dt13(_V) (int)(ptrdiff_t)&(((MVMObject* *)0)_V)
#line 35 "src/jit/emit_x64.dasc"
//|.type CONTAINERSPEC, MVMContainerSpec
#define Dt14(_V) (int)(ptrdiff_t)&(((MVMContainerSpec *)0)_V)
#line 36 "src/jit/emit_x64.dasc"
//|.type STORAGESPEC, MVMStorageSpec
#define Dt15(_V) (int)(ptrdiff_t)&(((MVMStorageSpec *)0)_V)
#line 37 "src/jit/emit_x64.dasc"
//|.type HLLCONFIG, MVMHLLConfig;
#define Dt16(_V) (int)(ptrdiff_t)&(((MVMHLLConfig *)0)_V)
#line 38 "src/jit/emit_x64.dasc"
//|.type SCREFBODY, MVMSerializationContextBody
#define Dt17(_V) (int)(ptrdiff_t)&(((MVMSerializationContextBody *)0)_V)
#line 39 "src/jit/emit_x64.dasc"
//|.type NFGSYNTH, MVMNFGSynthetic
#define Dt18(_V) (int)(ptrdiff_t)&(((MVMNFGSynthetic *)0)_V)
#line 40 "src/jit/emit_x64.dasc"
//|.type U8, MVMuint8
#define Dt19(_V) (int)(ptrdiff_t)&(((MVMuint8 *)0)_V)
#line 41 "src/jit/emit_x64.dasc"
//|.type U16, MVMuint16
#define Dt1A(_V) (int)(ptrdiff_t)&(((MVMuint16 *)0)_V)
#line 42 "src/jit/emit_x64.dasc"
//|.type U32, MVMuint32
#define Dt1B(_V) (int)(ptrdiff_t)&(((MVMuint32 *)0)_V)
#line 43 "src/jit/emit_x64.dasc"
//|.type U64, MVMuint64
#define Dt1C(_V) (int)(ptrdiff_t)&(((MVMuint64 *)0)_V)
#line 44 "src/jit/emit_x64.dasc"



/* Static allocation of relevant types to registers. I pick
 * callee-save registers for efficiency. It is likely we'll be calling
 * quite a C functions, and this saves us the trouble of storing
 * them. Moreover, C compilers preferentially do not use callee-saved
 * registers, and so in most cases, these won't be touched at all. */
//|.type TC, MVMThreadContext, r14
#define Dt1D(_V) (int)(ptrdiff_t)&(((MVMThreadContext *)0)_V)
#line 53 "src/jit/emit_x64.dasc"
/* Alternative base pointer. I'll be using this often, so picking rbx
 * here rather than the extended registers will lead to smaller
 * bytecode */
//|.type WORK, MVMRegister, rbx
#define Dt1E(_V) (int)(ptrdiff_t)&(((MVMRegister *)0)_V)
#line 57 "src/jit/emit_x64.dasc"
//|.type FRAME, MVMFrame, r12
#define Dt1F(_V) (int)(ptrdiff_t)&(((MVMFrame *)0)_V)
#line 58 "src/jit/emit_x64.dasc"
//|.type CU, MVMCompUnit, r13
#define Dt20(_V) (int)(ptrdiff_t)&(((MVMCompUnit *)0)_V)
#line 59 "src/jit/emit_x64.dasc"




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


//|.macro callp, funcptr
//| mov64 FUNCTION, (uintptr_t)funcptr
//| call FUNCTION
//|.endmacro


//|.macro check_wb, root, ref;
//| test word COLLECTABLE:root->flags, MVM_CF_SECOND_GEN;
//| setnz al;
//| cmp ref, 0x0;
//| setne ah;
//| test ah, al;
//| setnz al;
//| test word COLLECTABLE:ref->flags, MVM_CF_SECOND_GEN;
//| setz ah;
//| test ah, al;
//|.endmacro;

//|.macro hit_wb, obj
//| mov ARG1, TC;
//| mov ARG2, obj;
//| callp &MVM_gc_write_barrier_hit;
//|.endmacro

//|.macro get_spesh_slot, reg, idx;
//| mov reg, FRAME->effective_spesh_slots;
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

/* A function prologue is always the same in x86 / x64, becuase
 * we do not provide variable arguments, instead arguments are provided
 * via a frame. All JIT entry points receive a prologue. */
void MVM_jit_emit_prologue(MVMThreadContext *tc, MVMJitGraph *jg,
                           dasm_State **Dst) {
    /* Setup stack */
    //| push rbp; // nb, this aligns the stack to 16 bytes again
    //| mov rbp, rsp;
    dasm_put(Dst, 0);
#line 228 "src/jit/emit_x64.dasc"
    /* allocate stack space: 0x100 bytes = 256 bytes
     *
     * layout: [ a: 0x20 | b: 0x40 | c: 0xa0 | d: 0x20 ]
     * a: space for 4 callee-save registers
     * b: small scratch space
     * c: space for stack arguments to c calls
     * d: reserve space for GPR registers to c calls (win64) or more space for
     * stack arguments (posix) */
    //| sub rsp, 0x100;
    dasm_put(Dst, 5);
#line 237 "src/jit/emit_x64.dasc"
    /* save callee-save registers */
    //| mov [rbp-0x8],  TC;
    //| mov [rbp-0x10], CU;
    //| mov [rbp-0x18], FRAME;
    //| mov [rbp-0x20], WORK;
    dasm_put(Dst, 14);
#line 242 "src/jit/emit_x64.dasc"
    /* setup special frame variables */
    //| mov TC,   ARG1;
    //| mov CU,   ARG2;
    //| mov FRAME, TC->cur_frame;
    //| mov WORK, FRAME->work;
    dasm_put(Dst, 33, Dt1D(->cur_frame), Dt1F(->work));
#line 247 "src/jit/emit_x64.dasc"
    /* ARG3 contains our 'entry label' */
    //| jmp ARG3
    dasm_put(Dst, 52);
#line 249 "src/jit/emit_x64.dasc"
}

/* And a function epilogue is also always the same */
void MVM_jit_emit_epilogue(MVMThreadContext *tc, MVMJitGraph *jg,
                           dasm_State **Dst) {
    //| ->exit:
    //| mov RV, 0;
    //| ->out:
    dasm_put(Dst, 56);
#line 257 "src/jit/emit_x64.dasc"
    /* restore callee-save registers */
    //| mov TC, [rbp-0x8];
    //| mov CU, [rbp-0x10];
    //| mov FRAME, [rbp-0x18];
    //| mov WORK, [rbp-0x20];
    dasm_put(Dst, 68);
#line 262 "src/jit/emit_x64.dasc"
    /* Restore stack */
    //| mov rsp, rbp;
    //| pop rbp;
    //| ret;
    dasm_put(Dst, 87);
#line 266 "src/jit/emit_x64.dasc"
}

static MVMuint64 try_emit_gen2_ref(MVMThreadContext *tc, MVMJitGraph *jg,
                                   MVMObject *obj, MVMint16 reg,
                                   dasm_State **Dst) {
    if (!(obj->header.flags & MVM_CF_SECOND_GEN))
        return 0;
    //| mov64 TMP1, (uintptr_t)obj;
    //| mov WORK[reg], TMP1;
    dasm_put(Dst, 94, (unsigned int)((uintptr_t)obj), (unsigned int)(((uintptr_t)obj)>>32), Dt1E([reg]));
#line 275 "src/jit/emit_x64.dasc"
    return 1;
}

static MVMint64 fits_in_32_bit(MVMint64 number) {
    /* Used to determine if a 64 bit integer can be safely used as a
     * 32 bit constant for immediate mode access */
    return (number >= INT32_MIN) && (number <= INT32_MAX);
}

/* compile per instruction, can't really do any better yet */
void MVM_jit_emit_primitive(MVMThreadContext *tc, MVMJitGraph *jg,
                            MVMJitPrimitive * prim, dasm_State **Dst) {
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
        dasm_put(Dst, 103, Dt1E([reg]), val);
#line 302 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_const_i64: {
        MVMint32 reg = ins->operands[0].reg.orig;
        MVMint64 val = ins->operands[1].lit_i64;
        //| mov64 TMP1, val;
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 94, (unsigned int)(val), (unsigned int)((val)>>32), Dt1E([reg]));
#line 309 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_const_n64: {
        MVMint16 reg = ins->operands[0].reg.orig;
        MVMint64 valbytes = ins->operands[1].lit_i64;
        MVM_jit_log(tc, "store const %f\n", ins->operands[1].lit_n64);
        //| mov64 TMP1, valbytes;
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 94, (unsigned int)(valbytes), (unsigned int)((valbytes)>>32), Dt1E([reg]));
#line 317 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 94, (unsigned int)(tmp.i64), (unsigned int)((tmp.i64)>>32), Dt1E([reg]));
#line 332 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_const_s: {
         MVMint16 reg = ins->operands[0].reg.orig;
         MVMuint32 idx = ins->operands[1].lit_str_idx;
         MVMStaticFrame *sf = jg->sg->sf;
         MVMString * s = MVM_cu_string(tc, sf->body.cu, idx);
         if (!try_emit_gen2_ref(tc, jg, (MVMObject*)s, reg, Dst)) {
             //| get_string TMP1, idx;
              MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, idx);
#line 341 "src/jit/emit_x64.dasc"
             //| mov WORK[reg], TMP1;
             dasm_put(Dst, 109, Dt20(->body.strings), Dt12([idx]), Dt1E([reg]));
#line 342 "src/jit/emit_x64.dasc"
         }
         break;
    }
    case MVM_OP_null: {
        MVMint16 reg = ins->operands[0].reg.orig;
        //| get_vmnull TMP1;
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 122, Dt1D(->instance), DtA(->VMNull), Dt1E([reg]));
#line 349 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_getwhat:
    case MVM_OP_getwho: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[obj];
        //| mov TMP1, OBJECT:TMP1->st;
        dasm_put(Dst, 135, Dt1E([obj]), DtC(->st));
#line 357 "src/jit/emit_x64.dasc"
        if (op == MVM_OP_getwho) {
            //| mov TMP1, STABLE:TMP1->WHO;
            //| get_vmnull TMP2;
            //| test TMP1, TMP1;
            //| cmovz TMP1, TMP2;
            dasm_put(Dst, 144, DtF(->WHO), Dt1D(->instance), DtA(->VMNull));
#line 362 "src/jit/emit_x64.dasc"
        } else {
            //| mov TMP1, STABLE:TMP1->WHAT;
            dasm_put(Dst, 139, DtF(->WHAT));
#line 364 "src/jit/emit_x64.dasc"
        }
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 98, Dt1E([dst]));
#line 366 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_getlex: {
        MVMuint16 *lexical_types;
        MVMStaticFrame * sf = jg->sg->sf;
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 idx = ins->operands[1].lex.idx;
        MVMint16 out = ins->operands[1].lex.outers;
        MVMint16 i;
        //| mov TMP6, FRAME;
        dasm_put(Dst, 164);
#line 376 "src/jit/emit_x64.dasc"
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
            dasm_put(Dst, 168, Dt1F(->outer));
#line 386 "src/jit/emit_x64.dasc"
            sf = sf->body.outer;
        }
        /* get array of lexicals */
        //| mov TMP5, FRAME:TMP6->env;
        dasm_put(Dst, 173, Dt1F(->env));
#line 390 "src/jit/emit_x64.dasc"
        /* read value */
        //| mov TMP5, REGISTER:TMP5[idx];
        dasm_put(Dst, 178, Dt1([idx]));
#line 392 "src/jit/emit_x64.dasc"
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
            dasm_put(Dst, 183);
#line 404 "src/jit/emit_x64.dasc"
            /* setup args */
            //| mov ARG1, TC;
            //| mov ARG2, TMP6;
            //| mov ARG3, idx;
            //| callp &MVM_frame_vivify_lexical;
            dasm_put(Dst, 191, idx, (unsigned int)((uintptr_t)&MVM_frame_vivify_lexical), (unsigned int)(((uintptr_t)&MVM_frame_vivify_lexical)>>32));
#line 409 "src/jit/emit_x64.dasc"
            /* use return value for the result */
            //| mov TMP5, RV;
            //|1:
            dasm_put(Dst, 211);
#line 412 "src/jit/emit_x64.dasc"
        }
        /* store the value */
        //| mov WORK[dst], TMP5;
        dasm_put(Dst, 217, Dt1E([dst]));
#line 415 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_getlex_no: {
        MVMint16  dst = ins->operands[0].reg.orig;
        MVMuint32 idx = ins->operands[1].lit_str_idx;
        //| mov ARG1, TC;
        //| get_string ARG2, idx;
        dasm_put(Dst, 222);
         MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, idx);
#line 422 "src/jit/emit_x64.dasc"
        //| mov ARG3, MVM_reg_obj;
        //| callp &MVM_frame_find_lexical_by_name;
        //| test RV, RV;
        //| jz >1;
        //| mov RV, [RV];
        //|1:
        //| mov WORK[dst], RV;
        dasm_put(Dst, 227, Dt20(->body.strings), Dt12([idx]), MVM_reg_obj, (unsigned int)((uintptr_t)&MVM_frame_find_lexical_by_name), (unsigned int)(((uintptr_t)&MVM_frame_find_lexical_by_name)>>32), Dt1E([dst]));
#line 429 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_bindlex: {
        MVMint16 idx = ins->operands[0].lex.idx;
        MVMint16 out = ins->operands[0].lex.outers;
        MVMint16 src = ins->operands[1].reg.orig;
        MVMint16 i;
        //| mov TMP1, FRAME;
        dasm_put(Dst, 264);
#line 437 "src/jit/emit_x64.dasc"
        for (i = 0; i < out; i++) {
            //| mov TMP1, FRAME:TMP1->outer;
            dasm_put(Dst, 139, Dt1F(->outer));
#line 439 "src/jit/emit_x64.dasc"
        }
        //| mov TMP1, FRAME:TMP1->env;
        //| mov TMP2, WORK[src];
        //| mov REGISTER:TMP1[idx], TMP2;
        dasm_put(Dst, 268, Dt1F(->env), Dt1E([src]), Dt1([idx]));
#line 443 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_sp_getarg_o:
    case MVM_OP_sp_getarg_n:
    case MVM_OP_sp_getarg_s:
    case MVM_OP_sp_getarg_i: {
        MVMint32 reg = ins->operands[0].reg.orig;
        MVMuint16 idx = ins->operands[1].callsite_idx;
        //| mov TMP1, FRAME->params.args;
        //| mov TMP1, REGISTER:TMP1[idx];
        //| mov WORK[reg], TMP1;
        dasm_put(Dst, 281, Dt1F(->params.args), Dt1([idx]), Dt1E([reg]));
#line 454 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 296, Dt1E([obj]), (offset + body), Dt7(->body.replaced), offset);
#line 473 "src/jit/emit_x64.dasc"
        /* TMP2 now contains address of item */
        if (op == MVM_OP_sp_p6oget_o) {
            //| mov TMP3, [TMP2];
            //| test TMP3, TMP3;
            dasm_put(Dst, 320);
#line 477 "src/jit/emit_x64.dasc"
            /* Check if object doesn't point to NULL */
            //| jnz >3;
            dasm_put(Dst, 327);
#line 479 "src/jit/emit_x64.dasc"
            /* Otherwise load VMNull */
            //| get_vmnull TMP3;
            //|3:
            dasm_put(Dst, 332, Dt1D(->instance), DtA(->VMNull));
#line 482 "src/jit/emit_x64.dasc"
        } else if (op == MVM_OP_sp_p6ogetvt_o) {
            /* vivify as type object */
            MVMint16 spesh_idx = ins->operands[3].lit_i16;
            //| mov TMP3, [TMP2];
            dasm_put(Dst, 343);
#line 486 "src/jit/emit_x64.dasc"
            /* check for null */
            //| test TMP3, TMP3;
            //| jnz >4;
            dasm_put(Dst, 347);
#line 489 "src/jit/emit_x64.dasc"
            /* if null, vivify as type object from spesh slot */
            //| get_spesh_slot TMP3, spesh_idx;
            dasm_put(Dst, 355, Dt1F(->effective_spesh_slots), Dt13([spesh_idx]));
#line 491 "src/jit/emit_x64.dasc"
            /* need to hit write barrier? */
            //| check_wb TMP1, TMP3;
            //| jz >3;
            //| mov qword [rbp-0x28], TMP2; // address
            //| mov qword [rbp-0x30], TMP3; // value
            //| hit_wb WORK[obj]; // write barrier for header
            //| mov TMP3, qword [rbp-0x30];
            //| mov TMP2, qword [rbp-0x28];
            //|3:
            dasm_put(Dst, 366, DtE(->flags), MVM_CF_SECOND_GEN, DtE(->flags), MVM_CF_SECOND_GEN, Dt1E([obj]), (unsigned int)((uintptr_t)&MVM_gc_write_barrier_hit), (unsigned int)(((uintptr_t)&MVM_gc_write_barrier_hit)>>32));
#line 500 "src/jit/emit_x64.dasc"
            /* store vivified type value in memory location */
            //| mov qword [TMP2], TMP3;
            //|4:
            dasm_put(Dst, 439);
#line 503 "src/jit/emit_x64.dasc"
        } else if (op == MVM_OP_sp_p6ogetvc_o) {
            MVMint16 spesh_idx = ins->operands[3].lit_i16;
            //| mov TMP3, [TMP2];
            //| test TMP3, TMP3;
            //| jnz >4;
            dasm_put(Dst, 445);
#line 508 "src/jit/emit_x64.dasc"
            /* vivify as clone */
            //| mov ARG1, TC;
            //| get_spesh_slot ARG2, spesh_idx;
            //| callp &MVM_repr_clone;
            //| mov TMP3, RV;
            dasm_put(Dst, 456, Dt1F(->effective_spesh_slots), Dt13([spesh_idx]), (unsigned int)((uintptr_t)&MVM_repr_clone), (unsigned int)(((uintptr_t)&MVM_repr_clone)>>32));
#line 513 "src/jit/emit_x64.dasc"
            /* reload object and address */
            //| mov TMP1, WORK[obj];
            //| lea TMP2, [TMP1 + (offset + body)];
            //| mov TMP4, P6OPAQUE:TMP1->body.replaced;
            //| lea TMP5, [TMP4 + offset];
            //| test TMP4, TMP4;
            //| cmovnz TMP2, TMP5;
            dasm_put(Dst, 296, Dt1E([obj]), (offset + body), Dt7(->body.replaced), offset);
#line 520 "src/jit/emit_x64.dasc"
            /* assign with write barrier */
            //| check_wb TMP1, TMP3;
            //| jz >3;
            //| mov qword [rbp-0x28], TMP2; // address
            //| mov qword [rbp-0x30], TMP3; // value
            //| hit_wb WORK[obj]; // write barrier for header
            //| mov TMP3, qword [rbp-0x30];
            //| mov TMP2, qword [rbp-0x28];
            //|3:
            //| mov qword [TMP2], TMP3;
            dasm_put(Dst, 482, DtE(->flags), MVM_CF_SECOND_GEN, DtE(->flags), MVM_CF_SECOND_GEN, Dt1E([obj]), (unsigned int)((uintptr_t)&MVM_gc_write_barrier_hit), (unsigned int)(((uintptr_t)&MVM_gc_write_barrier_hit)>>32));
#line 530 "src/jit/emit_x64.dasc"
            /* done */
            //|4:
            dasm_put(Dst, 442);
#line 532 "src/jit/emit_x64.dasc"
        } else {
            /* the regular case */
            //| mov TMP3, [TMP2];
            dasm_put(Dst, 343);
#line 535 "src/jit/emit_x64.dasc"
        }
        /* store in local register */
        //| mov WORK[dst], TMP3;
        dasm_put(Dst, 558, Dt1E([dst]));
#line 538 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 563, Dt1E([obj]), Dt1E([val]));
#line 549 "src/jit/emit_x64.dasc"
        if (op == MVM_OP_sp_bind_o || op == MVM_OP_sp_bind_s) {
            /* check if we should hit write barrier */
            //| check_wb TMP1, TMP2;
            dasm_put(Dst, 572, DtE(->flags), MVM_CF_SECOND_GEN, DtE(->flags), MVM_CF_SECOND_GEN);
#line 552 "src/jit/emit_x64.dasc"
            /* note: it is uneccesary to store pointers, because they
               can just be loaded from memory */
            //| jz >2;
            //| hit_wb WORK[obj];
            //| mov TMP1, aword WORK[obj]; // reload object
            //| mov TMP2, aword WORK[val]; // reload value
            //|2: // done
            dasm_put(Dst, 606, Dt1E([obj]), (unsigned int)((uintptr_t)&MVM_gc_write_barrier_hit), (unsigned int)(((uintptr_t)&MVM_gc_write_barrier_hit)>>32), Dt1E([obj]), Dt1E([val]));
#line 559 "src/jit/emit_x64.dasc"
        }
        //| mov qword [TMP1+offset], TMP2; // store value into body
        dasm_put(Dst, 276, offset);
#line 561 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 637, Dt1E([obj]), offset, Dt1E([dst]));
#line 573 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 650, Dt1E([obj]), Dt1E([val]), offset);
#line 584 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 666, Dt1E([obj]), offset, Dt1E([dst]));
#line 595 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 682, Dt1E([obj]), Dt1E([val]), Dt7(->body), Dt8(->replaced), Dt8(->replaced));
#line 611 "src/jit/emit_x64.dasc"
        if (op == MVM_OP_sp_p6obind_o || op == MVM_OP_sp_p6obind_s) {
            /* check if we should hit write barrier */
            //| check_wb TMP1, TMP2;
            //| jz >2;
            //| mov qword [rbp-0x28], TMP2; // store value
            //| mov qword [rbp-0x30], TMP3; // store body pointer
            //| hit_wb WORK[obj];
            //| mov TMP3, qword [rbp-0x30]; // restore body pointer
            //| mov TMP2, qword [rbp-0x28]; // restore value
            //|2: // done
            dasm_put(Dst, 710, DtE(->flags), MVM_CF_SECOND_GEN, DtE(->flags), MVM_CF_SECOND_GEN, Dt1E([obj]), (unsigned int)((uintptr_t)&MVM_gc_write_barrier_hit), (unsigned int)(((uintptr_t)&MVM_gc_write_barrier_hit)>>32));
#line 621 "src/jit/emit_x64.dasc"
        }
        //| mov [TMP3+offset], TMP2; // store value into body
        dasm_put(Dst, 782, offset);
#line 623 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_getwhere:
    case MVM_OP_set: {
         MVMint32 reg1 = ins->operands[0].reg.orig;
         MVMint32 reg2 = ins->operands[1].reg.orig;
         //| mov TMP1, WORK[reg2];
         //| mov WORK[reg1], TMP1;
         dasm_put(Dst, 787, Dt1E([reg2]), Dt1E([reg1]));
#line 631 "src/jit/emit_x64.dasc"
         break;
    }
    case MVM_OP_sp_getspeshslot: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 spesh_idx = ins->operands[1].lit_i16;
        //| get_spesh_slot TMP1, spesh_idx;
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 281, Dt1F(->effective_spesh_slots), Dt13([spesh_idx]), Dt1E([dst]));
#line 638 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_setdispatcher: {
        MVMint16 src = ins->operands[0].reg.orig;
        //| mov TMP1, aword WORK[src];
        //| mov aword TC->cur_dispatcher, TMP1;
        dasm_put(Dst, 796, Dt1E([src]), Dt1D(->cur_dispatcher));
#line 644 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_takedispatcher: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, aword TC->cur_dispatcher;
        //| mov aword WORK[dst], TMP1;
        //| mov aword TC->cur_dispatcher, NULL;
        dasm_put(Dst, 805, Dt1D(->cur_dispatcher), Dt1E([dst]), Dt1D(->cur_dispatcher), NULL);
#line 651 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_curcode: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, aword FRAME->code_ref;
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 819, Dt1F(->code_ref), Dt1E([dst]));
#line 657 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_getcode: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMuint16 idx = ins->operands[1].coderef_idx;
        //| mov TMP1, aword CU->body.coderefs;
        //| mov TMP1, aword OBJECTPTR:TMP1[idx];
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 109, Dt20(->body.coderefs), Dt13([idx]), Dt1E([dst]));
#line 665 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_callercode: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, aword FRAME->caller;
        //| test TMP1, TMP1;
        //| jz >1;
        //| mov TMP1, aword FRAME:TMP1->code_ref;
        //|1:
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 830, Dt1F(->caller), Dt1F(->code_ref), Dt1E([dst]));
#line 675 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_hllboxtype_n:
    case MVM_OP_hllboxtype_s:
    case MVM_OP_hllboxtype_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, CU->body.hll_config;
        dasm_put(Dst, 854, Dt20(->body.hll_config));
#line 682 "src/jit/emit_x64.dasc"
        if (op == MVM_OP_hllboxtype_n) {
            //| mov TMP1, aword HLLCONFIG:TMP1->num_box_type;
            dasm_put(Dst, 139, Dt16(->num_box_type));
#line 684 "src/jit/emit_x64.dasc"
        } else if (op == MVM_OP_hllboxtype_s) {
            //| mov TMP1, aword HLLCONFIG:TMP1->str_box_type;
            dasm_put(Dst, 139, Dt16(->str_box_type));
#line 686 "src/jit/emit_x64.dasc"
        } else {
            //| mov TMP1, aword HLLCONFIG:TMP1->int_box_type;
            dasm_put(Dst, 139, Dt16(->int_box_type));
#line 688 "src/jit/emit_x64.dasc"
        }
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 98, Dt1E([dst]));
#line 690 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_null_s: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov qword WORK[dst], 0;
        dasm_put(Dst, 859, Dt1E([dst]));
#line 695 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 868, Dt1E([src]), Dt1E([dst]));
#line 705 "src/jit/emit_x64.dasc"
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
                    dasm_put(Dst, 887, Dt1E([reg_a]), value);
#line 728 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_sub_i:
                    //| sub qword WORK[reg_a], qword value;
                    dasm_put(Dst, 893, Dt1E([reg_a]), value);
#line 731 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_bor_i:
                    //| or qword WORK[reg_a], qword value;
                    dasm_put(Dst, 899, Dt1E([reg_a]), value);
#line 734 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_band_i:
                    //| and qword WORK[reg_a], qword value;
                    dasm_put(Dst, 905, Dt1E([reg_a]), value);
#line 737 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_bxor_i:
                    //| xor qword WORK[reg_a], qword value;
                    dasm_put(Dst, 911, Dt1E([reg_a]), value);
#line 740 "src/jit/emit_x64.dasc"
                    break;
                }
            } else {
                MVM_jit_log(tc, "accumulator for %s stayed in memory\n", ins->info->name);
                //| mov rax, WORK[reg_c];
                dasm_put(Dst, 917, Dt1E([reg_c]));
#line 745 "src/jit/emit_x64.dasc"
                switch(ins->info->opcode) {
                case MVM_OP_add_i:
                    //| add WORK[reg_a], rax;
                    dasm_put(Dst, 922, Dt1E([reg_a]));
#line 748 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_sub_i:
                    //| sub WORK[reg_a], rax;
                    dasm_put(Dst, 927, Dt1E([reg_a]));
#line 751 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_bor_i:
                    //| or WORK[reg_a], rax;
                    dasm_put(Dst, 932, Dt1E([reg_a]));
#line 754 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_band_i:
                    //| and WORK[reg_a], rax;
                    dasm_put(Dst, 937, Dt1E([reg_a]));
#line 757 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_bxor_i:
                    //| xor WORK[reg_a], rax;
                    dasm_put(Dst, 942, Dt1E([reg_a]));
#line 760 "src/jit/emit_x64.dasc"
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
                dasm_put(Dst, 917, Dt1E([reg_b]));
#line 770 "src/jit/emit_x64.dasc"
                switch(ins->info->opcode) {
                case MVM_OP_add_i:
                    //| add rax, qword value;
                    dasm_put(Dst, 947, value);
#line 773 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_sub_i:
                    //| sub rax, qword value;
                    dasm_put(Dst, 952, value);
#line 776 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_bor_i:
                    //| or rax, qword value;
                    dasm_put(Dst, 957, value);
#line 779 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_band_i:
                    //| and rax, qword value;
                    dasm_put(Dst, 962, value);
#line 782 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_bxor_i:
                    //| xor rax, qword value;
                    dasm_put(Dst, 967, value);
#line 785 "src/jit/emit_x64.dasc"
                    break;
                }
                //| mov WORK[reg_a], rax;
                dasm_put(Dst, 259, Dt1E([reg_a]));
#line 788 "src/jit/emit_x64.dasc"
            } else {
                //| mov rax, WORK[reg_b];
                dasm_put(Dst, 917, Dt1E([reg_b]));
#line 790 "src/jit/emit_x64.dasc"
                switch(ins->info->opcode) {
                case MVM_OP_add_i:
                    //| add rax, WORK[reg_c];
                    dasm_put(Dst, 973, Dt1E([reg_c]));
#line 793 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_sub_i:
                    //| sub rax, WORK[reg_c];
                    dasm_put(Dst, 978, Dt1E([reg_c]));
#line 796 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_bor_i:
                    //| or rax, WORK[reg_c];
                    dasm_put(Dst, 983, Dt1E([reg_c]));
#line 799 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_band_i:
                    //| and rax, WORK[reg_c];
                    dasm_put(Dst, 988, Dt1E([reg_c]));
#line 802 "src/jit/emit_x64.dasc"
                    break;
                case MVM_OP_bxor_i:
                    //| xor rax, WORK[reg_c];
                    dasm_put(Dst, 993, Dt1E([reg_c]));
#line 805 "src/jit/emit_x64.dasc"
                    break;
                }
                //| mov WORK[reg_a], rax;
                dasm_put(Dst, 259, Dt1E([reg_a]));
#line 808 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 917, Dt1E([reg_b]));
#line 819 "src/jit/emit_x64.dasc"
        switch(ins->info->opcode) {
        case MVM_OP_mul_i:
            //| imul rax, WORK[reg_c];
            dasm_put(Dst, 998, Dt1E([reg_c]));
#line 822 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_blshift_i:
            //| mov cl, byte WORK[reg_c];
            //| shl rax, cl;
            dasm_put(Dst, 1004, Dt1E([reg_c]));
#line 826 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_brshift_i:
            //| mov cl, byte WORK[reg_c];
            //| shr rax, cl;
            dasm_put(Dst, 1011, Dt1E([reg_c]));
#line 830 "src/jit/emit_x64.dasc"
            break;
        }
        //| mov WORK[reg_a], rax;
        dasm_put(Dst, 259, Dt1E([reg_a]));
#line 833 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_div_i:
    case MVM_OP_mod_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 a   = ins->operands[1].reg.orig;
        MVMint16 b   = ins->operands[2].reg.orig;
        //| mov rcx, WORK[b];
        //| test rcx, rcx;
        //| jnz >1;
        //| throw_adhoc "Division by zero";
        //|1:
        dasm_put(Dst, 1018, Dt1E([b]), (unsigned int)((uintptr_t)("Division by zero")), (unsigned int)(((uintptr_t)("Division by zero"))>>32), (unsigned int)((uintptr_t)&MVM_exception_throw_adhoc), (unsigned int)(((uintptr_t)&MVM_exception_throw_adhoc)>>32));
#line 845 "src/jit/emit_x64.dasc"
        // division is safe, proceed
        //| mov rax, WORK[a];
        //| cqo;  // convert quadword to octoword. look it up :-)
        //| idiv rcx; // rcx was the denominator
        dasm_put(Dst, 1048, Dt1E([a]));
#line 849 "src/jit/emit_x64.dasc"
        if (op == MVM_OP_div_i) {
            //| mov TMP3, rax;
            //| sub TMP3, 1;
            //| cmp rax, 0; // quotient < 0
            //| setl ch;
            //| test rdx, rdx; // remainder != 0
            //| setnz cl;
            //| test ch, cl;
            //| cmovnz rax, TMP3;
            //| mov WORK[dst], rax;
            dasm_put(Dst, 1060, Dt1E([dst]));
#line 859 "src/jit/emit_x64.dasc"
        } else {
            //| mov WORK[dst], rdx;
            dasm_put(Dst, 645, Dt1E([dst]));
#line 861 "src/jit/emit_x64.dasc"
        }
        break;
    }

    case MVM_OP_inc_i: {
         MVMint32 reg = ins->operands[0].reg.orig;
         //| add qword WORK[reg], 1;
         dasm_put(Dst, 1092, Dt1E([reg]));
#line 868 "src/jit/emit_x64.dasc"
         break;
    }
    case MVM_OP_dec_i: {
        MVMint32 reg = ins->operands[0].reg.orig;
        //| sub qword WORK[reg], 1;
        dasm_put(Dst, 1098, Dt1E([reg]));
#line 873 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_bnot_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[src];
        //| not TMP1;
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 1104, Dt1E([src]), Dt1E([dst]));
#line 881 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_neg_i: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP1, WORK[src];
        //| neg TMP1;
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 1117, Dt1E([src]), Dt1E([dst]));
#line 889 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1130, Dt1E([reg_b]));
#line 901 "src/jit/emit_x64.dasc"
        switch(ins->info->opcode) {
        case MVM_OP_add_n:
            //| addsd xmm0, qword WORK[reg_c];
            dasm_put(Dst, 1137, Dt1E([reg_c]));
#line 904 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_sub_n:
            //| subsd xmm0, qword WORK[reg_c];
            dasm_put(Dst, 1144, Dt1E([reg_c]));
#line 907 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_mul_n:
            //| mulsd xmm0, qword WORK[reg_c];
            dasm_put(Dst, 1151, Dt1E([reg_c]));
#line 910 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_div_n:
            //| divsd xmm0, qword WORK[reg_c];
            dasm_put(Dst, 1158, Dt1E([reg_c]));
#line 913 "src/jit/emit_x64.dasc"
            break;
        }
        //| movsd qword WORK[reg_a], xmm0;
        dasm_put(Dst, 1165, Dt1E([reg_a]));
#line 916 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_coerce_in: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        /* convert simple integer to double precision */
        //| cvtsi2sd xmm0, qword WORK[src];
        //| movsd qword WORK[dst], xmm0;
        dasm_put(Dst, 1172, Dt1E([src]), Dt1E([dst]));
#line 924 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_coerce_ni: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        /* convert double precision to simple intege */
        //| cvttsd2si rax, qword WORK[src];
        //| mov WORK[dst], rax;
        dasm_put(Dst, 1186, Dt1E([src]), Dt1E([dst]));
#line 932 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1198, Dt1E([src]), Dt1E([dst]));
#line 943 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 917, Dt1E([reg_b]));
#line 956 "src/jit/emit_x64.dasc"
        /* comparison result in the setting bits in the rflags register */
        //| cmp rax, WORK[reg_c];
        dasm_put(Dst, 1221, Dt1E([reg_c]));
#line 958 "src/jit/emit_x64.dasc"
        /* copy the right comparison bit to the lower byte of the rax
           register */
        switch(ins->info->opcode) {
        case MVM_OP_eqaddr:
        case MVM_OP_eq_i:
            //| sete al;
            dasm_put(Dst, 1226);
#line 964 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_ne_i:
            //| setne al;
            dasm_put(Dst, 1230);
#line 967 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_lt_i:
            //| setl al;
            dasm_put(Dst, 1234);
#line 970 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_le_i:
            //| setle al;
            dasm_put(Dst, 1238);
#line 973 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_gt_i:
            //| setg al;
            dasm_put(Dst, 1242);
#line 976 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_ge_i:
            //| setge al;
            dasm_put(Dst, 1246);
#line 979 "src/jit/emit_x64.dasc"
            break;
        }
        /* zero extend al (lower byte) to rax (whole register) */
        //| movzx rax, al;
        //| mov WORK[reg_a], rax;
        dasm_put(Dst, 1250, Dt1E([reg_a]));
#line 984 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_cmp_i : {
        MVMint32 reg_a = ins->operands[0].reg.orig;
        MVMint32 reg_b = ins->operands[1].reg.orig;
        MVMint32 reg_c = ins->operands[2].reg.orig;
        //| mov TMP1, WORK[reg_b];
        dasm_put(Dst, 1259, Dt1E([reg_b]));
#line 991 "src/jit/emit_x64.dasc"
        /* comparison result in the setting bits in the rflags register */
        //| cmp TMP1, WORK[reg_c];
        dasm_put(Dst, 1264, Dt1E([reg_c]));
#line 993 "src/jit/emit_x64.dasc"
        /* copy the right comparison bit to the lower byte of the rax
           register */
        //| setg TMP2b;
        //| movzx TMP2, TMP2b;
        //| setl TMP3b;
        //| movzx TMP3, TMP3b;
        //| sub TMP2, TMP3;
        //| mov WORK[reg_a], TMP2;
        dasm_put(Dst, 1269, Dt1E([reg_a]));
#line 1001 "src/jit/emit_x64.dasc"
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
                dasm_put(Dst, 1292);
#line 1014 "src/jit/emit_x64.dasc"
                break;
            case MVM_OP_lt_s:
                //| mov TMP2, -1;
                dasm_put(Dst, 1300);
#line 1017 "src/jit/emit_x64.dasc"
                break;
            case MVM_OP_ge_s: case MVM_OP_le_s:
                //| mov TMP2, 0;
                dasm_put(Dst, 1312);
#line 1020 "src/jit/emit_x64.dasc"
                break;
        }
        //| cmp TMP2, WORK[reg];
        dasm_put(Dst, 1320, Dt1E([reg]));
#line 1023 "src/jit/emit_x64.dasc"
        /* Mind the reversedness of the constant and the value as compared to
         * interp.c */
        switch(ins->info->opcode) {
            case MVM_OP_gt_s:
                //| sete al;
                dasm_put(Dst, 1226);
#line 1028 "src/jit/emit_x64.dasc"
                break;
            case MVM_OP_ge_s:
                //| setle al;
                dasm_put(Dst, 1238);
#line 1031 "src/jit/emit_x64.dasc"
                break;
            case MVM_OP_lt_s:
                //| sete al;
                dasm_put(Dst, 1226);
#line 1034 "src/jit/emit_x64.dasc"
                break;
            case MVM_OP_le_s:
                //| setge al;
                dasm_put(Dst, 1246);
#line 1037 "src/jit/emit_x64.dasc"
                break;
        }
        //| movzx rax, al;
        //| mov WORK[reg], rax;
        dasm_put(Dst, 1250, Dt1E([reg]));
#line 1041 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 868, Dt1E([src]), Dt1E([dst]));
#line 1051 "src/jit/emit_x64.dasc"
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
            dasm_put(Dst, 1325);
#line 1064 "src/jit/emit_x64.dasc"
        } else if (op == MVM_OP_ne_n) {
            //| mov TMP1, 1;
            dasm_put(Dst, 1333);
#line 1066 "src/jit/emit_x64.dasc"
        }
        if (op == MVM_OP_lt_n || op == MVM_OP_le_n) {
            //| movsd xmm0, qword WORK[b];
            //| ucomisd xmm0, qword WORK[a];
            dasm_put(Dst, 1341, Dt1E([b]), Dt1E([a]));
#line 1070 "src/jit/emit_x64.dasc"
        } else {
            //| movsd xmm0, qword WORK[a];
            //| ucomisd xmm0, qword WORK[b];
            dasm_put(Dst, 1341, Dt1E([a]), Dt1E([b]));
#line 1073 "src/jit/emit_x64.dasc"
        }

        if (op == MVM_OP_le_n || op == MVM_OP_ge_n) {
            //| setae TMP1b;
            dasm_put(Dst, 1353);
#line 1077 "src/jit/emit_x64.dasc"
        } else if (op == MVM_OP_eq_n) {
            //| setnp TMP2b; // zero if either is NaN, 1 otherwise
            //| cmove TMP1, TMP2; // if equal, overwrite 0 with 1
            dasm_put(Dst, 1357);
#line 1080 "src/jit/emit_x64.dasc"
        } else if (op == MVM_OP_ne_n) {
            //| setp TMP2b; // 1 if either is NaN (in which case they can't be equal)
            //| cmove TMP1, TMP2; // if equal, overwrite 1 with IsNan(a) | IsNaN(b)
            dasm_put(Dst, 1365);
#line 1083 "src/jit/emit_x64.dasc"
        } else {
            //| seta TMP1b;
            dasm_put(Dst, 1373);
#line 1085 "src/jit/emit_x64.dasc"
        }
        //| movzx TMP1, TMP1b;
        //| mov WORK[dst], TMP1;
        dasm_put(Dst, 1377, Dt1E([dst]));
#line 1088 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1386, Dt1E([a]), Dt1E([b]), Dt1E([dst]));
#line 1104 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1428, tc, Dt1E([reg_b]), Dt1E([reg_c]), (unsigned int)((uintptr_t)&MVM_bigint_cmp), (unsigned int)(((uintptr_t)&MVM_bigint_cmp)>>32));
#line 1120 "src/jit/emit_x64.dasc"
        /* Handle result by opcode. */
        switch(ins->info->opcode) {
        case MVM_OP_eq_I:
            //| cmp RV, MP_EQ
            //| sete al;
            dasm_put(Dst, 1449, MP_EQ);
#line 1125 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_ne_I:
            //| cmp RV, MP_EQ
            //| setne al;
            dasm_put(Dst, 1458, MP_EQ);
#line 1129 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_lt_I:
            //| cmp RV, MP_LT
            //| sete al;
            dasm_put(Dst, 1449, MP_LT);
#line 1133 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_le_I:
            //| cmp RV, MP_GT
            //| setne al;
            dasm_put(Dst, 1458, MP_GT);
#line 1137 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_gt_I:
            //| cmp RV, MP_GT
            //| sete al;
            dasm_put(Dst, 1449, MP_GT);
#line 1141 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_ge_I:
            //| cmp RV, MP_LT
            //| setne al;
            dasm_put(Dst, 1458, MP_LT);
#line 1145 "src/jit/emit_x64.dasc"
            break;
        }
        /* zero extend al (lower byte) to rax (whole register) */
        //| movzx rax, al;
        //| mov WORK[reg_a], rax;
        dasm_put(Dst, 1250, Dt1E([reg_a]));
#line 1150 "src/jit/emit_x64.dasc"
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
                          op == MVM_OP_islist ? MVM_REPR_ID_MVMArray :
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
        dasm_put(Dst, 1467, Dt1E([obj]), DtC(->st), DtF(->REPR), Dt10(->ID), reprid, Dt1E([dst]), Dt1E([dst]));
#line 1176 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_sp_boolify_iter_arr: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[obj];
        //| mov TMP2, MVMITER:TMP1->body.array_state.index;
        //| add TMP2, 1;
        //| mov TMP3, MVMITER:TMP1->body.array_state.limit;
        dasm_put(Dst, 1520, Dt1E([obj]), Dt9(->body.array_state.index), Dt9(->body.array_state.limit));
#line 1185 "src/jit/emit_x64.dasc"
        /* index - limit will give a carry flag when index < limit */
        //| cmp TMP2, TMP3;
        //| setl TMP1b;
        //| movzx TMP1, TMP1b;
        //| mov aword WORK[dst], TMP1;
        dasm_put(Dst, 1537, Dt1E([dst]));
#line 1190 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1552, Dt1E([obj]), Dt9(->body.hash_state.next), Dt1E([dst]));
#line 1201 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1575, Dt1E([type]), DtC(->st), DtF(->REPR), Dt10(->get_storage_spec), Dt15(->boxed_primitive), Dt1E([dst]));
#line 1217 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1618, Dt1E([type]), DtC(->st), DtF(->REPR), Dt10(->get_storage_spec), Dt15(->boxed_primitive), Dt15(->bits), Dt1E([dst]));
#line 1236 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1673, Dt1E([obj]), Dt1D(->instance), DtA(->VMNull), Dt1E([dst]));
#line 1250 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1710, Dt1E([obj]), Dt1D(->instance), DtA(->VMNull), Dt1E([dst]));
#line 1264 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_sp_fastcreate: {
        MVMint16 dst       = ins->operands[0].reg.orig;
        MVMuint16 size     = ins->operands[1].lit_i16;
        MVMint16 spesh_idx = ins->operands[2].lit_i16;
        //| mov ARG1, TC;
        //| mov ARG2, size;
        //| callp &MVM_gc_allocate_zeroed;
        //| get_spesh_slot TMP1, spesh_idx;
        //| mov aword OBJECT:RV->st, TMP1;  // st is 64 bit (pointer)
        //| mov word OBJECT:RV->header.size, size; // object size is 16 bit
        //| mov TMP1d, dword TC->thread_id;  // thread id is 32 bit
        //| mov dword OBJECT:RV->header.owner, TMP1d; // does this even work?
        //| mov aword WORK[dst], RV; // store in local register
        dasm_put(Dst, 1747, size, (unsigned int)((uintptr_t)&MVM_gc_allocate_zeroed), (unsigned int)(((uintptr_t)&MVM_gc_allocate_zeroed)>>32), Dt1F(->effective_spesh_slots), Dt13([spesh_idx]), DtC(->st), DtC(->header.size), size, Dt1D(->thread_id), DtC(->header.owner), Dt1E([dst]));
#line 1279 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_decont: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 src = ins->operands[1].reg.orig;
        //| mov TMP5, WORK[src];
        //| test TMP5, TMP5;
        dasm_put(Dst, 1794, Dt1E([src]));
#line 1286 "src/jit/emit_x64.dasc"
        // obj is null
        //| jz >1;
        //| is_type_object TMP5;
        dasm_put(Dst, 1802, DtC(->header.flags), MVM_CF_TYPE_OBJECT);
#line 1289 "src/jit/emit_x64.dasc"
        // object is type object (not concrete)
        //| jnz >1;
        //| mov TMP6, OBJECT:TMP5->st;
        //| mov TMP6, STABLE:TMP6->container_spec;
        //| test TMP6, TMP6;
        dasm_put(Dst, 1814, DtC(->st), DtF(->container_spec));
#line 1294 "src/jit/emit_x64.dasc"
        // container spec is zero
        //| jz >1;
        //| mov ARG1, TC;
        //| mov ARG2, TMP5;      // object
        //| lea ARG3, WORK[dst]; // destination register
        //| mov FUNCTION, CONTAINERSPEC:TMP6->fetch; // get function pointer
        //| call FUNCTION;
        //| jmp >2;
        //|1:
        dasm_put(Dst, 1830, Dt1E([dst]), Dt14(->fetch));
#line 1303 "src/jit/emit_x64.dasc"
        // otherwise just move the object into the register
        //| mov WORK[dst], TMP5;
        //|2:
        dasm_put(Dst, 1860, Dt1E([dst]));
#line 1306 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 1867, Dt1E([obj]), DtC(->st), DtF(->container_spec), Dt1E([dst]));
#line 1321 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_sp_namedarg_used: {
        MVMuint16 param = ins->operands[0].lit_i16;
        //| mov TMP1, FRAME->params.named_used;
        //| mov byte U8:TMP1[param], 1;
        dasm_put(Dst, 1903, Dt1F(->params.named_used), Dt19([param]));
#line 1327 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_sp_findmeth: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 obj = ins->operands[1].reg.orig;
        MVMint32 str_idx = ins->operands[2].lit_str_idx;
        MVMuint16 ss_idx = ins->operands[3].lit_i16;
        MVMStaticFrame *sf = jg->sg->sf;
        //| get_spesh_slot TMP1, ss_idx;
        //| mov TMP2, WORK[obj];
        //| mov TMP2, OBJECT:TMP2->st;
        //| cmp TMP1, TMP2;
        //| jne >1;
        //| get_spesh_slot TMP3, ss_idx + 1;
        //| mov WORK[dst], TMP3;
        //| jmp >2;
        //|1:
        //| mov ARG1, TC;
        //| mov ARG2, WORK[obj];
        //| get_string ARG3, str_idx;
        dasm_put(Dst, 1914, Dt1F(->effective_spesh_slots), Dt13([ss_idx]), Dt1E([obj]), DtC(->st), Dt1F(->effective_spesh_slots), Dt13([ss_idx + 1]), Dt1E([dst]), Dt1E([obj]));
         MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, str_idx);
#line 1347 "src/jit/emit_x64.dasc"
        //| mov ARG4, ss_idx;
        //| lea TMP6, WORK[dst];
        //|.if WIN32;
        //| mov qword [rsp+0x20], TMP6;
        //|.else;
        //| mov ARG5, TMP6;
        //|.endif
        //| callp &MVM_6model_find_method_spesh;
        //| test RV, RV;
        //| jz >2;
        dasm_put(Dst, 1968, Dt20(->body.strings), Dt12([str_idx]), ss_idx, Dt1E([dst]), (unsigned int)((uintptr_t)&MVM_6model_find_method_spesh), (unsigned int)(((uintptr_t)&MVM_6model_find_method_spesh)>>32));
#line 1357 "src/jit/emit_x64.dasc"
        /* invokish, fall out to the interpreter */
        //| mov RV, 1;
        //| lea TMP1, [>2];
        //| mov aword FRAME->jit_entry_label, TMP1;
        //| jmp ->out;
        //|2:
        dasm_put(Dst, 2003, Dt1F(->jit_entry_label));
#line 1363 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 2028, Dt1E([obj]), DtC(->header.flags), MVM_CF_TYPE_OBJECT, Dt1E([dst]), Dt1E([dst]));
#line 1378 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_takehandlerresult: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, aword TC->last_handler_result;
        //| mov aword WORK[dst], TMP1;
        //| mov aword TC->last_handler_result, 0;
        dasm_put(Dst, 2074, Dt1D(->last_handler_result), Dt1E([dst]), Dt1D(->last_handler_result));
#line 1385 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_lexoticresult: {
        MVMint16 src = ins->operands[1].reg.orig;
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, aword WORK[src];
        //| test TMP1, TMP1;
        //| jz >1;
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov TMP2, aword OBJECT:TMP1->st;
        //| mov TMP2, aword STABLE:TMP2->REPR;
        //| cmp aword REPR:TMP2->ID, MVM_REPR_ID_Lexotic;
        //| jnz >1;
        //| mov TMP2, aword LEXOTIC:TMP1->body.result;
        //| mov WORK[dst], TMP2;
        //| jmp >2;
        //|1:
        dasm_put(Dst, 2091, Dt1E([src]), DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), DtF(->REPR), Dt10(->ID), MVM_REPR_ID_Lexotic, Dt11(->body.result), Dt1E([dst]));
#line 1403 "src/jit/emit_x64.dasc"
        /* throw an exception */
        //| throw_adhoc "lexoticresult needs a Lexotic";
        dasm_put(Dst, 2144, (unsigned int)((uintptr_t)("lexoticresult needs a Lexotic")), (unsigned int)(((uintptr_t)("lexoticresult needs a Lexotic"))>>32), (unsigned int)((uintptr_t)&MVM_exception_throw_adhoc), (unsigned int)(((uintptr_t)&MVM_exception_throw_adhoc)>>32));
#line 1405 "src/jit/emit_x64.dasc"
        /* we never return from an adhoc exception, so no need to deal with that */
        //|2:
        dasm_put(Dst, 634);
#line 1407 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_exception: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1, TC->active_handlers;
        //| test TMP1, TMP1;
        //| jz >1;
        //| mov TMP1, [TMP1];
        //| mov TMP1, MVMACTIVEHANDLERS:TMP1->ex_obj;
        //| mov WORK[dst], TMP1;
        //| jmp >2;
        //|1:
        //| get_vmnull TMP1;
        //| mov WORK[dst], TMP1;
        //|2:
        dasm_put(Dst, 2161, Dt1D(->active_handlers), DtB(->ex_obj), Dt1E([dst]), Dt1D(->instance), DtA(->VMNull), Dt1E([dst]));
#line 1422 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_scwbdisable: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1d, dword TC->sc_wb_disable_depth;
        //| add TMP1d, 1;
        //| mov dword TC->sc_wb_disable_depth, TMP1d;
        //| mov qword WORK[dst], TMP1;
        dasm_put(Dst, 2204, Dt1D(->sc_wb_disable_depth), Dt1D(->sc_wb_disable_depth), Dt1E([dst]));
#line 1430 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_scwbenable: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP1d, dword TC->sc_wb_disable_depth; // should do zero-extension
        //| sub TMP1d, 1;
        //| mov dword TC->sc_wb_disable_depth, TMP1d;
        //| mov qword WORK[dst], TMP1; 
        dasm_put(Dst, 2220, Dt1D(->sc_wb_disable_depth), Dt1D(->sc_wb_disable_depth), Dt1E([dst]));
#line 1438 "src/jit/emit_x64.dasc"
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
        //|1:
        //| mov ARG1, TC;
        //| mov ARG3, aword WORK[obj];
        dasm_put(Dst, 2237, Dt1E([cont]), DtC(->st), DtF(->container_spec), (unsigned int)((uintptr_t)("Cannot assign to an immutable value")), (unsigned int)(((uintptr_t)("Cannot assign to an immutable value"))>>32), (unsigned int)((uintptr_t)&MVM_exception_throw_adhoc), (unsigned int)(((uintptr_t)&MVM_exception_throw_adhoc)>>32), Dt1E([obj]));
#line 1453 "src/jit/emit_x64.dasc"
        if (op == MVM_OP_assign) {
            //| mov FUNCTION, CONTAINERSPEC:FUNCTION->store;
            dasm_put(Dst, 178, Dt14(->store));
#line 1455 "src/jit/emit_x64.dasc"
        } else {
            //| mov FUNCTION, CONTAINERSPEC:FUNCTION->store_unchecked;
            dasm_put(Dst, 178, Dt14(->store_unchecked));
#line 1457 "src/jit/emit_x64.dasc"
        }
        //| call FUNCTION;
        dasm_put(Dst, 206);
#line 1459 "src/jit/emit_x64.dasc"
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
        //| test RV, RV;
        //| jz >1;
        //| mov RV, [RV];
        //|1:
        //| mov WORK[dst], RV;
        dasm_put(Dst, 2283, Dt1E([name]), MVM_reg_obj, (unsigned int)((uintptr_t)&MVM_frame_find_lexical_by_name), (unsigned int)(((uintptr_t)&MVM_frame_find_lexical_by_name)>>32), Dt1E([dst]));
#line 1474 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_paramnamesused:
        //| lea ARG2, FRAME->params;
        //| mov TMP5w, word ARGCTX:ARG2->num_pos;
        //| cmp TMP5w, word ARGCTX:ARG2->arg_count;
        //| je >1;
        //| mov ARG1, TC;
        //| callp &MVM_args_assert_nameds_used;
        //|1:
        dasm_put(Dst, 2320, Dt1F(->params), Dt2(->num_pos), Dt2(->arg_count), (unsigned int)((uintptr_t)&MVM_args_assert_nameds_used), (unsigned int)(((uintptr_t)&MVM_args_assert_nameds_used)>>32));
#line 1484 "src/jit/emit_x64.dasc"
        break;
    case MVM_OP_assertparamcheck: {
        MVMint16 ok = ins->operands[0].reg.orig;
        //| mov TMP1, qword WORK[ok];
        //| test TMP1, TMP1;
        //| jnz >1;
        //| mov ARG1, TC;
        //| callp &MVM_args_bind_failed;
        //|1:
        dasm_put(Dst, 2355, Dt1E([ok]), (unsigned int)((uintptr_t)&MVM_args_bind_failed), (unsigned int)(((uintptr_t)&MVM_args_bind_failed)>>32));
#line 1493 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_prof_enterspesh:
        //| mov ARG1, TC;
        //| mov ARG2, aword FRAME->static_info;
        //| mov ARG3, aword MVM_PROFILE_ENTER_JIT;
        //| callp &MVM_profile_log_enter;
        dasm_put(Dst, 2381, Dt1F(->static_info), MVM_PROFILE_ENTER_JIT, (unsigned int)((uintptr_t)&MVM_profile_log_enter), (unsigned int)(((uintptr_t)&MVM_profile_log_enter)>>32));
#line 1500 "src/jit/emit_x64.dasc"
        break;
    case MVM_OP_prof_enterinline: {
        MVMint16 spesh_idx = ins->operands[0].lit_i16;
        //| mov ARG1, TC;
        //| get_spesh_slot ARG2, spesh_idx;
        //| mov ARG3, aword MVM_PROFILE_ENTER_JIT_INLINE;
        //| callp &MVM_profile_log_enter;
        dasm_put(Dst, 2404, Dt1F(->effective_spesh_slots), Dt13([spesh_idx]), MVM_PROFILE_ENTER_JIT_INLINE, (unsigned int)((uintptr_t)&MVM_profile_log_enter), (unsigned int)(((uintptr_t)&MVM_profile_log_enter)>>32));
#line 1507 "src/jit/emit_x64.dasc"
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
        dasm_put(Dst, 2431, Dt1E([obj]), DtE(->sc_forward_u.sc.sc_idx), Dt1D(->instance), DtA(->all_scs), Dt17(->sc), Dt1E([dst]));
#line 1523 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_invokewithcapture: {
        MVMint16 dest    = ins->operands[0].reg.orig;
        MVMint16 code    = ins->operands[1].reg.orig;
        MVMint16 capture = ins->operands[2].reg.orig;
        //| mov TMP1, aword WORK[capture];
        dasm_put(Dst, 1259, Dt1E([capture]));
#line 1530 "src/jit/emit_x64.dasc"
        /* if (IS_CONCRETE(capture) && REPR(capture)->ID == MVM_REPR_ID_MVMCallCapture) */
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov TMP2, OBJECT:TMP1->st;
        //| mov TMP2, STABLE:TMP2->REPR;
        //| cmp dword REPR:TMP2->ID, MVM_REPR_ID_MVMCallCapture;
        //| je >2;
        //|1:
        dasm_put(Dst, 2472, DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), DtF(->REPR), Dt10(->ID), MVM_REPR_ID_MVMCallCapture);
#line 1538 "src/jit/emit_x64.dasc"
        /* else throw */
        //| throw_adhoc "invokewithcapture needs a MVMCallCapture";
        //|2:
        dasm_put(Dst, 2501, (unsigned int)((uintptr_t)("invokewithcapture needs a MVMCallCapture")), (unsigned int)(((uintptr_t)("invokewithcapture needs a MVMCallCapture"))>>32), (unsigned int)((uintptr_t)&MVM_exception_throw_adhoc), (unsigned int)(((uintptr_t)&MVM_exception_throw_adhoc)>>32));
#line 1541 "src/jit/emit_x64.dasc"
        /* code = MVM_frame_find_invokee(tc, code, NULL) */
        //| mov ARG1, TC;
        //| mov ARG2, WORK[code];
        //| xor ARG3, ARG3;
        //| callp &MVM_frame_find_invokee;
        dasm_put(Dst, 2520, Dt1E([code]), (unsigned int)((uintptr_t)&MVM_frame_find_invokee), (unsigned int)(((uintptr_t)&MVM_frame_find_invokee)>>32));
#line 1546 "src/jit/emit_x64.dasc"

        //| mov ARG2, RV; // ARG2 = code
        dasm_put(Dst, 2540);
#line 1548 "src/jit/emit_x64.dasc"
        /* tc->cur_frame->return_value = &GET_REG(cur_op, 2) */
        //| lea ARG1, WORK[dest]; // ARG1 is used as scratch space
        //| mov aword FRAME->return_value, ARG1;
        dasm_put(Dst, 2544, Dt1E([dest]), Dt1F(->return_value));
#line 1551 "src/jit/emit_x64.dasc"
        /* tc->cur_frame->return_type = MVM_RETURN_OBJ */
        //| mov dword FRAME->return_type, MVM_RETURN_OBJ;
        dasm_put(Dst, 2555, Dt1F(->return_type), MVM_RETURN_OBJ);
#line 1553 "src/jit/emit_x64.dasc"
        /* tc->cur_frame->return_address = cur_op */
        //| get_cur_op ARG1;
        //| mov aword FRAME->return_address, ARG1;
        dasm_put(Dst, 2563, Dt1D(->interp_cur_op), Dt1F(->return_address));
#line 1556 "src/jit/emit_x64.dasc"
        /* STABLE(code)->invoke(tc, code, capture->body.effective_callsite,
           capture->body.apc->arg) */
        //| mov ARG1, TC;
        //| mov ARG3, WORK[capture];
        //| mov ARG4, aword CAPTURE:ARG3->body.apc;
        //| mov ARG4, aword ARGCTX:ARG4->args;
        //| mov ARG3, aword CAPTURE:ARG3->body.effective_callsite;
        //| mov FUNCTION, OBJECT:ARG2->st;
        //| mov FUNCTION, STABLE:FUNCTION->invoke;
        //| call FUNCTION;
        dasm_put(Dst, 2577, Dt1E([capture]), Dt3(->body.apc), Dt2(->args), Dt3(->body.effective_callsite), DtC(->st), DtF(->invoke));
#line 1566 "src/jit/emit_x64.dasc"
        /* jumping out is handled by invokish */
        break;
    }
    case MVM_OP_captureposelems: {
        MVMint16 dest    = ins->operands[0].reg.orig;
        MVMint16 capture = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[capture];
        dasm_put(Dst, 1259, Dt1E([capture]));
#line 1573 "src/jit/emit_x64.dasc"
        /* if (IS_CONCRETE(capture) && REPR(capture)->ID == MVM_REPR_ID_MVMCallCapture) */
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov TMP2, OBJECT:TMP1->st;
        //| mov TMP2, STABLE:TMP2->REPR;
        //| cmp dword REPR:TMP2->ID, MVM_REPR_ID_MVMCallCapture;
        //| je >2;
        //|1:
        dasm_put(Dst, 2472, DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), DtF(->REPR), Dt10(->ID), MVM_REPR_ID_MVMCallCapture);
#line 1581 "src/jit/emit_x64.dasc"
        /* else throw */
        //| throw_adhoc "captureposelems needs a concrete MVMCallCapture";
        //|2:
        //| mov TMP2, aword CAPTURE:TMP1->body.apc;
        //| movzx TMP2, word ARGPROCCONTEXT:TMP2->num_pos;
        //| mov WORK[dest], TMP2;
        dasm_put(Dst, 2610, (unsigned int)((uintptr_t)("captureposelems needs a concrete MVMCallCapture")), (unsigned int)(((uintptr_t)("captureposelems needs a concrete MVMCallCapture"))>>32), (unsigned int)((uintptr_t)&MVM_exception_throw_adhoc), (unsigned int)(((uintptr_t)&MVM_exception_throw_adhoc)>>32), Dt3(->body.apc), Dt5(->num_pos), Dt1E([dest]));
#line 1587 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_capturehasnameds: {
        MVMint16 dest    = ins->operands[0].reg.orig;
        MVMint16 capture = ins->operands[1].reg.orig;
        //| mov TMP1, aword WORK[capture];
        dasm_put(Dst, 1259, Dt1E([capture]));
#line 1593 "src/jit/emit_x64.dasc"
        /* if (IS_CONCRETE(capture) && REPR(capture)->ID == MVM_REPR_ID_MVMCallCapture) */
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov TMP2, OBJECT:TMP1->st;
        //| mov TMP2, STABLE:TMP2->REPR;
        //| cmp dword REPR:TMP2->ID, MVM_REPR_ID_MVMCallCapture;
        //| je >2;
        //|1:
        dasm_put(Dst, 2472, DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), DtF(->REPR), Dt10(->ID), MVM_REPR_ID_MVMCallCapture);
#line 1601 "src/jit/emit_x64.dasc"
        /* else throw */
        //| throw_adhoc "capturehasnameds needs a concrete MVMCallCapture";
        //|2:
        //| mov TMP2, CAPTURE:TMP1->body.apc;
        //| mov TMP3w, word ARGPROCCONTEXT:TMP2->num_pos;
        //| mov TMP2w, word ARGPROCCONTEXT:TMP2->arg_count;
        //| cmp TMP2w, TMP3w;
        //| setne al;
        //| movzx rax, al;
        //| mov WORK[dest], rax;
        dasm_put(Dst, 2642, (unsigned int)((uintptr_t)("capturehasnameds needs a concrete MVMCallCapture")), (unsigned int)(((uintptr_t)("capturehasnameds needs a concrete MVMCallCapture"))>>32), (unsigned int)((uintptr_t)&MVM_exception_throw_adhoc), (unsigned int)(((uintptr_t)&MVM_exception_throw_adhoc)>>32), Dt3(->body.apc), Dt5(->num_pos), Dt5(->arg_count), Dt1E([dest]));
#line 1611 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_getstdin:
    case MVM_OP_getstdout:
    case MVM_OP_getstderr: {
        MVMint16 dst = ins->operands[0].reg.orig;
        //| mov TMP3, aword TC->instance;
        dasm_put(Dst, 2689, Dt1D(->instance));
#line 1618 "src/jit/emit_x64.dasc"
        if (ins->info->opcode == MVM_OP_getstdin) {
            //| mov TMP3, aword MVMINSTANCE:TMP3->stdin_handle;
            dasm_put(Dst, 361, DtA(->stdin_handle));
#line 1620 "src/jit/emit_x64.dasc"
        } else if (ins->info->opcode == MVM_OP_getstdout) {
            //| mov TMP3, aword MVMINSTANCE:TMP3->stdout_handle;
            dasm_put(Dst, 361, DtA(->stdout_handle));
#line 1622 "src/jit/emit_x64.dasc"
        } else if (ins->info->opcode == MVM_OP_getstderr) {
            //| mov TMP3, aword MVMINSTANCE:TMP3->stderr_handle;
            dasm_put(Dst, 361, DtA(->stderr_handle));
#line 1624 "src/jit/emit_x64.dasc"
        }
        //| mov aword WORK[dst], TMP3;
        dasm_put(Dst, 558, Dt1E([dst]));
#line 1626 "src/jit/emit_x64.dasc"
        break;
    }
    case MVM_OP_ordat:
    case MVM_OP_ordfirst: {
        MVMint16 dst = ins->operands[0].reg.orig;
        MVMint16 str = ins->operands[1].reg.orig;
        //| mov ARG1, TC;
        //| mov ARG2, aword WORK[str];
        dasm_put(Dst, 1959, Dt1E([str]));
#line 1634 "src/jit/emit_x64.dasc"
        if (op == MVM_OP_ordat) {
            MVMint16 idx = ins->operands[2].reg.orig;
            //| mov ARG3, qword WORK[idx];
            dasm_put(Dst, 567, Dt1E([idx]));
#line 1637 "src/jit/emit_x64.dasc"
        } else {
            //| mov ARG3, 0;
            dasm_put(Dst, 1312);
#line 1639 "src/jit/emit_x64.dasc"
        }
        //| callp &MVM_string_get_grapheme_at;
        //| cmp RV, 0;
        //| jge >1;
        dasm_put(Dst, 2694, (unsigned int)((uintptr_t)&MVM_string_get_grapheme_at), (unsigned int)(((uintptr_t)&MVM_string_get_grapheme_at)>>32));
#line 1643 "src/jit/emit_x64.dasc"
        /* NB - I see no reason why this should be incorrect, but I
           haven't been able to trigger it yet. */
        //| mov ARG1, TC;
        //| mov ARG2, RV;
        //| callp &MVM_nfg_get_synthetic_info;
        dasm_put(Dst, 2712, (unsigned int)((uintptr_t)&MVM_nfg_get_synthetic_info), (unsigned int)(((uintptr_t)&MVM_nfg_get_synthetic_info)>>32));
#line 1648 "src/jit/emit_x64.dasc"
	/* base is only 32 bit wide */
        //| mov RVd, dword NFGSYNTH:RV->base;
        //|1:
        //| mov qword WORK[dst], RV;
        dasm_put(Dst, 2728, Dt18(->base), Dt1E([dst]));
#line 1652 "src/jit/emit_x64.dasc"
        break;
    }
    default:
        MVM_panic(1, "Can't JIT opcode <%s>", ins->info->name);
    }
}



/* Call argument decoder */
static void load_call_arg(MVMThreadContext *tc, MVMJitGraph *jg,
                          MVMJitCallArg arg, dasm_State **Dst) {
    switch(arg.type) {
    case MVM_JIT_INTERP_VAR:
        switch (arg.v.ivar) {
        case MVM_JIT_INTERP_TC:
            //| mov TMP6, TC;
            dasm_put(Dst, 2738);
#line 1669 "src/jit/emit_x64.dasc"
            break;
        case MVM_JIT_INTERP_CU:
            //| mov TMP6, CU;
            dasm_put(Dst, 2743);
#line 1672 "src/jit/emit_x64.dasc"
            break;
        case MVM_JIT_INTERP_FRAME:
            //| mov TMP6, FRAME;
            dasm_put(Dst, 164);
#line 1675 "src/jit/emit_x64.dasc"
            break;
        case MVM_JIT_INTERP_PARAMS:
            //| lea TMP6, FRAME->params;
            dasm_put(Dst, 2748, Dt1F(->params));
#line 1678 "src/jit/emit_x64.dasc"
            break;
        case MVM_JIT_INTERP_CALLER:
            //| mov TMP6, aword FRAME->caller;
            dasm_put(Dst, 2755, Dt1F(->caller));
#line 1681 "src/jit/emit_x64.dasc"
            break;
        }
        break;
    case MVM_JIT_REG_VAL:
        //| mov TMP6, qword WORK[arg.v.reg];
        dasm_put(Dst, 2762, Dt1E([arg.v.reg]));
#line 1686 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_REG_VAL_F:
        //| mov TMP6, qword WORK[arg.v.reg];
        dasm_put(Dst, 2762, Dt1E([arg.v.reg]));
#line 1689 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_REG_ADDR:
        //| lea TMP6, WORK[arg.v.reg];
        dasm_put(Dst, 2767, Dt1E([arg.v.reg]));
#line 1692 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_STR_IDX:
        //| get_string TMP6, arg.v.lit_i64;
         MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, arg.v.lit_i64);
        dasm_put(Dst, 2772, Dt20(->body.strings), Dt12([arg.v.lit_i64]));
#line 1695 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_LITERAL:
        //| mov TMP6, arg.v.lit_i64;
        dasm_put(Dst, 2781, arg.v.lit_i64);
#line 1698 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_LITERAL_64:
    case MVM_JIT_LITERAL_PTR:
    case MVM_JIT_LITERAL_F:
        //| mov64 TMP6, arg.v.lit_i64;
        dasm_put(Dst, 2786, (unsigned int)(arg.v.lit_i64), (unsigned int)((arg.v.lit_i64)>>32));
#line 1703 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_REG_STABLE:
        //| mov TMP6, qword WORK[arg.v.reg];
        //| mov TMP6, OBJECT:TMP6->st;
        dasm_put(Dst, 2791, Dt1E([arg.v.reg]), DtC(->st));
#line 1707 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_REG_OBJBODY:
        //| mov TMP6, qword WORK[arg.v.reg];
        //| lea TMP6, STOOGE:TMP6->data;
        dasm_put(Dst, 2800, Dt1E([arg.v.reg]), DtD(->data));
#line 1711 "src/jit/emit_x64.dasc"
        break;
    }
}

static void emit_gpr_arg(MVMThreadContext *tc, MVMJitGraph *jg,
                         MVMint32 i, dasm_State **Dst) {
    switch (i) {
    case 0:
        //| mov ARG1, TMP6;
        dasm_put(Dst, 2809);
#line 1720 "src/jit/emit_x64.dasc"
        break;
    case 1:
        //| mov ARG2, TMP6;
        dasm_put(Dst, 2813);
#line 1723 "src/jit/emit_x64.dasc"
        break;
    case 2:
        //| mov ARG3, TMP6;
        dasm_put(Dst, 2817);
#line 1726 "src/jit/emit_x64.dasc"
        break;
    case 3:
        //| mov ARG4, TMP6;
        dasm_put(Dst, 2821);
#line 1729 "src/jit/emit_x64.dasc"
        break;
//|.if POSIX
    case 4:
//|        mov ARG5, TMP6;
dasm_put(Dst, 2825);
       break;
#line 1734 "src/jit/emit_x64.dasc"
  case 5:
//|      mov ARG6, TMP6;
dasm_put(Dst, 2829);
     break;
#line 1737 "src/jit/emit_x64.dasc"
//|.endif
    default:
        MVM_oops(tc, "JIT: can't store %d arguments in GPR", i);
    }
}

static void emit_sse_arg(MVMThreadContext *tc, MVMJitGraph *jg,
                         MVMint32 i, dasm_State **Dst) {
    switch (i) {
    case 0:
        //| movd ARG1F, TMP6;
        dasm_put(Dst, 2833);
#line 1748 "src/jit/emit_x64.dasc"
        break;
    case 1:
        //| movd ARG2F, TMP6;
        dasm_put(Dst, 2839);
#line 1751 "src/jit/emit_x64.dasc"
        break;
    case 2:
        //| movd ARG3F, TMP6;
        dasm_put(Dst, 2845);
#line 1754 "src/jit/emit_x64.dasc"
        break;
    case 3:
        //| movd ARG4F, TMP6;
        dasm_put(Dst, 2851);
#line 1757 "src/jit/emit_x64.dasc"
        break;
//|.if POSIX
    case 4:
//|        movd ARG5F, TMP6;
dasm_put(Dst, 2857);
       break;
#line 1762 "src/jit/emit_x64.dasc"
    case 5:
//|         movd ARG6F, TMP6;
dasm_put(Dst, 2863);
        break;
#line 1765 "src/jit/emit_x64.dasc"
    case 6:
//|        movd ARG7F, TMP6;
dasm_put(Dst, 2870);
       break;
#line 1768 "src/jit/emit_x64.dasc"
    case 7:
//|        movd ARG8F, TMP6;
dasm_put(Dst, 2877);
       break;
#line 1771 "src/jit/emit_x64.dasc"
//|.endif
    default:
        MVM_oops(tc, "JIT: can't put  %d arguments in SSE", i);
    }
}

static void emit_stack_arg(MVMThreadContext *tc, MVMJitGraph *jg,
                           MVMint32 arg_size, MVMint32 pos,
                           dasm_State **Dst) {
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
        dasm_put(Dst, 2884, pos);
#line 1791 "src/jit/emit_x64.dasc"
        break;
    case 2:
        //| mov word [rsp+pos], TMP6w;
        dasm_put(Dst, 2891, pos);
#line 1794 "src/jit/emit_x64.dasc"
        break;
    case 4:
        //| mov dword [rsp+pos], TMP6d;
        dasm_put(Dst, 2892, pos);
#line 1797 "src/jit/emit_x64.dasc"
        break;
    case 8:
        //| mov qword [rsp+pos], TMP6;
        dasm_put(Dst, 2899, pos);
#line 1800 "src/jit/emit_x64.dasc"
        break;
    default:
        MVM_oops(tc, "JIT: can't pass arguments size %d bytes",
                 arg_size);
    }
}

static void emit_posix_callargs(MVMThreadContext *tc, MVMJitGraph *jg,
                                MVMJitCallArg args[], MVMint32 num_args,
                                dasm_State **Dst) {
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
        case MVM_JIT_STR_IDX:
        case MVM_JIT_LITERAL:
        case MVM_JIT_LITERAL_64:
        case MVM_JIT_LITERAL_PTR:
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
        }
    }
    for (i = 0; i < num_gpr; i++) {
        load_call_arg(tc, jg, in_gpr[i], Dst);
        emit_gpr_arg(tc, jg, i, Dst);
    }
    for (i = 0; i < num_fpr; i++) {
        load_call_arg(tc, jg, in_fpr[i], Dst);
        emit_sse_arg(tc, jg, i, Dst);
    }
    /* push right-to-left */
    for (i = 0; i < num_stack; i++) {
        load_call_arg(tc, jg, on_stack[i], Dst);
        // I'm not sure this is correct, btw
        emit_stack_arg(tc, jg, 8, i*8, Dst);
    }
    if (on_stack)
        MVM_free(on_stack);
}

static void emit_win64_callargs(MVMThreadContext *tc, MVMJitGraph *jg,
                                MVMJitCallArg args[], MVMint32 num_args,
                                dasm_State **Dst) {
    MVMint32 i;
    MVMint32 num_reg_args = (num_args > 4 ? 4 : num_args);
    for (i = 0; i < num_reg_args; i++) {
        load_call_arg(tc, jg, args[i], Dst);
        if (args[i].type == MVM_JIT_REG_VAL_F ||
            args[i].type == MVM_JIT_LITERAL_F) {
            emit_sse_arg(tc, jg, i, Dst);
        } else {
            emit_gpr_arg(tc, jg, i, Dst);
        }
    }
    for (; i < num_args; i++) {
        load_call_arg(tc, jg, args[i], Dst);
        emit_stack_arg(tc, jg, 8, i * 8, Dst);
    }
}

void MVM_jit_emit_call_c(MVMThreadContext *tc, MVMJitGraph *jg,
                         MVMJitCallC * call_spec, dasm_State **Dst) {

    MVM_jit_log(tc, "emit c call <%d args>\n", call_spec->num_args);
    if (call_spec->has_vargs) {
        MVM_oops(tc, "JIT can't handle varargs yet");
    }
    //|.if WIN32;
#line 1889 "src/jit/emit_x64.dasc"
    //|.else;
     emit_posix_callargs(tc, jg, call_spec->args, call_spec->num_args, Dst);
    //|.endif
    /* Emit the call. I think we should be able to do something smarter than
     * store the constant into the bytecode, like a data segment. But I'm
     * not sure. */
    //| callp call_spec->func_ptr;
    dasm_put(Dst, 202, (unsigned int)((uintptr_t)call_spec->func_ptr), (unsigned int)(((uintptr_t)call_spec->func_ptr)>>32));
#line 1896 "src/jit/emit_x64.dasc"
    /* right, now determine what to do with the return value */
    switch(call_spec->rv_mode) {
    case MVM_JIT_RV_VOID:
        break;
    case MVM_JIT_RV_INT:
    case MVM_JIT_RV_PTR:
        //| mov WORK[call_spec->rv_idx], RV;
        dasm_put(Dst, 259, Dt1E([call_spec->rv_idx]));
#line 1903 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_RV_NUM:
        //| movsd qword WORK[call_spec->rv_idx], RVF;
        dasm_put(Dst, 1165, Dt1E([call_spec->rv_idx]));
#line 1906 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_RV_DEREF:
        //| mov TMP1, [RV];
        //| mov WORK[call_spec->rv_idx], TMP1;
        dasm_put(Dst, 2906, Dt1E([call_spec->rv_idx]));
#line 1910 "src/jit/emit_x64.dasc"
        break;
    case MVM_JIT_RV_ADDR:
        /* store local at address */
        //| mov TMP1, WORK[call_spec->rv_idx];
        //| mov [RV], TMP1;
        dasm_put(Dst, 2914, Dt1E([call_spec->rv_idx]));
#line 1915 "src/jit/emit_x64.dasc"
        break;
    }
}

void MVM_jit_emit_branch(MVMThreadContext *tc, MVMJitGraph *jg,
                         MVMJitBranch * branch, dasm_State **Dst) {
    MVMSpeshIns *ins = branch->ins;
    MVMint32 name = branch->dest;
    /* move gc sync point to the front so as to not have
     * awkward dispatching issues */
    //| gc_sync_point;
    dasm_put(Dst, 2922, Dt1D(->gc_status), (unsigned int)((uintptr_t)&MVM_gc_enter_from_interrupt), (unsigned int)(((uintptr_t)&MVM_gc_enter_from_interrupt)>>32));
#line 1926 "src/jit/emit_x64.dasc"
    if (ins == NULL || ins->info->opcode == MVM_OP_goto) {
        MVM_jit_log(tc, "emit jump to label %d\n", name);
        if (name == MVM_JIT_BRANCH_EXIT) {
            //| jmp ->exit
            dasm_put(Dst, 2946);
#line 1930 "src/jit/emit_x64.dasc"
        } else {
            //| jmp =>(name)
            dasm_put(Dst, 2951, (name));
#line 1932 "src/jit/emit_x64.dasc"
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
            dasm_put(Dst, 2955, Dt1E([val]), (name));
#line 1942 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_unless_i:
            //| mov rax, WORK[val];
            //| test rax, rax;
            //| jz =>(name);
            dasm_put(Dst, 2966, Dt1E([val]), (name));
#line 1947 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_if_n:
            //| movd xmm0, qword WORK[val];
            //| xorpd xmm1, xmm1; // make it zero
            //| ucomisd xmm0, xmm1;
            //| jp =>(name);  // is NaN?
            //| jne =>(name); // not equal to zero? we're golden
            dasm_put(Dst, 2977, Dt1E([val]), (name), (name));
#line 1954 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_unless_n:
            //| movd xmm0, qword WORK[val];
            //| xorpd xmm1, xmm1; // make it zero
            //| ucomisd xmm0, xmm1;
            //| jp >1; // is NaN
            //| jne >1; // is not zero
            //| jmp =>(name); // it is zero yay!
            //|1:
            dasm_put(Dst, 2998, Dt1E([val]), (name));
#line 1963 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_if_s0:
        case MVM_OP_unless_s0:
            //| mov ARG1, TC;
            //| mov ARG2, WORK[val];
            //| callp &MVM_coerce_istrue_s;
            //| test RV, RV;
            dasm_put(Dst, 3026, Dt1E([val]), (unsigned int)((uintptr_t)&MVM_coerce_istrue_s), (unsigned int)(((uintptr_t)&MVM_coerce_istrue_s)>>32));
#line 1970 "src/jit/emit_x64.dasc"
            if (ins->info->opcode == MVM_OP_unless_s0)
                //| jz =>(name);
                dasm_put(Dst, 2973, (name));
#line 1972 "src/jit/emit_x64.dasc"
            else
                //| jnz =>(name);
                dasm_put(Dst, 2962, (name));
#line 1974 "src/jit/emit_x64.dasc"
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
            dasm_put(Dst, 3046, Dt1E([val]), Dt1D(->instance), DtA(->VMNull), (name));
#line 1984 "src/jit/emit_x64.dasc"
            break;
        case MVM_OP_indexat:
        case MVM_OP_indexnat: {
            MVMint16 offset = ins->operands[1].reg.orig;
            MVMuint32 str_idx = ins->operands[2].lit_str_idx;
            //| mov ARG1, TC;
            //| mov ARG2, WORK[val];
            //| mov ARG3, WORK[offset];
            //| get_string ARG4, str_idx;
            dasm_put(Dst, 3078, Dt1E([val]), Dt1E([offset]));
             MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, str_idx);
#line 1993 "src/jit/emit_x64.dasc"
            //| callp &MVM_string_char_at_in_string;
            dasm_put(Dst, 3091, Dt20(->body.strings), Dt12([str_idx]), (unsigned int)((uintptr_t)&MVM_string_char_at_in_string), (unsigned int)(((uintptr_t)&MVM_string_char_at_in_string)>>32));
#line 1994 "src/jit/emit_x64.dasc"
            /* This subtlety is due to the value being overloaded to
             * -2 if it is out of bounds. Note that -1 is passed as a
             * 32 bit integer, but this magically works in a 64 bit
             * comparison because 32 bit values are sign-extended */
            //| cmp RV, -1;
            dasm_put(Dst, 3108);
#line 1999 "src/jit/emit_x64.dasc"
            if (ins->info->opcode == MVM_OP_indexat)
                //| jle =>(name);
                dasm_put(Dst, 3115, (name));
#line 2001 "src/jit/emit_x64.dasc"
            else {

                //| jne =>(name);
                dasm_put(Dst, 2962, (name));
#line 2004 "src/jit/emit_x64.dasc"
            }
            break;
        }
        default:
            MVM_panic(1, "JIT: Can't handle conditional <%s>", ins->info->name);
        }
    }
}

void MVM_jit_emit_label(MVMThreadContext *tc, MVMJitGraph *jg,
                        MVMJitLabel *label, dasm_State **Dst) {
    //| =>(label->name):
    dasm_put(Dst, 330, (label->name));
#line 2016 "src/jit/emit_x64.dasc"
}

void MVM_jit_emit_guard(MVMThreadContext *tc, MVMJitGraph *jg,
                        MVMJitGuard *guard, dasm_State **Dst) {
    MVMint16 op        = guard->ins->info->opcode;
    MVMint16 obj       = guard->ins->operands[0].reg.orig;
    MVMint16 spesh_idx = guard->ins->operands[1].lit_i16;
    MVM_jit_log(tc, "emit guard <%s>\n", guard->ins->info->name);
    /* load object and spesh slot value */
    //| mov TMP1, WORK[obj];
    //| get_spesh_slot TMP2, spesh_idx;
    dasm_put(Dst, 3119, Dt1E([obj]), Dt1F(->effective_spesh_slots), Dt13([spesh_idx]));
#line 2027 "src/jit/emit_x64.dasc"
    if (op == MVM_OP_sp_guardtype) {
        /* object in queston should be a type object, so it shouldn't
         * be zero, should not be concrete, and the STABLE should be
         * equal to the value in the spesh slot */
        /* check for null */
        //| test TMP1, TMP1;
        //| jz >1;
        dasm_put(Dst, 3134);
#line 2034 "src/jit/emit_x64.dasc"
        /* check if type object (not concrete) */
        //| is_type_object TMP1;
        dasm_put(Dst, 3142, DtC(->header.flags), MVM_CF_TYPE_OBJECT);
#line 2036 "src/jit/emit_x64.dasc"
        /* if zero, this is a concrete object, and we should deopt */
        //| jz >1;
        dasm_put(Dst, 3137);
#line 2038 "src/jit/emit_x64.dasc"
        /* get stable and compare */
        //| cmp TMP2, OBJECT:TMP1->st;
        //| jne >1;
        dasm_put(Dst, 3149, DtC(->st));
#line 2041 "src/jit/emit_x64.dasc"
        /* we're good, no need to deopt */
    } else if (op == MVM_OP_sp_guardconc) {
        /* object should be a non-null concrete (non-type) object */
        //| test TMP1, TMP1;
        //| jz >1;
        dasm_put(Dst, 3134);
#line 2046 "src/jit/emit_x64.dasc"
        /* shouldn't be type object */
        //| is_type_object TMP1;
        //| jnz >1;
        dasm_put(Dst, 3158, DtC(->header.flags), MVM_CF_TYPE_OBJECT);
#line 2049 "src/jit/emit_x64.dasc"
        /* should have our stable */
        //| cmp TMP2, OBJECT:TMP1->st;
        //| jne >1;
        dasm_put(Dst, 3149, DtC(->st));
#line 2052 "src/jit/emit_x64.dasc"
    } else if (op == MVM_OP_sp_guardcontconc) {
        MVMint16 val_spesh_idx = guard->ins->operands[2].lit_i16;
        //| test TMP1, TMP1;
        //| jz >1;
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov FUNCTION, OBJECT:TMP1->st;
        //| cmp TMP2, FUNCTION;
        //| jne >1;
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->fetch;
        //| mov ARG2, TMP1;
        //| mov ARG1, TC;
        //| lea ARG3, [rbp-0x28]; // hurray for scratch space
        //| call FUNCTION;
        //| mov TMP3, aword [rbp-0x28];
        //| test TMP3, TMP3;
        //| jz >1;
        //| is_type_object TMP3;
        //| jnz >1;
        //| mov TMP2, OBJECT:TMP3->st;
        //| get_spesh_slot TMP1, val_spesh_idx;
        //| cmp TMP1, TMP2;
        dasm_put(Dst, 3169, DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), DtF(->container_spec), Dt14(->fetch), DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), Dt1F(->effective_spesh_slots), Dt13([val_spesh_idx]));
#line 2075 "src/jit/emit_x64.dasc"
        //| jne >1;
        dasm_put(Dst, 3257);
#line 2076 "src/jit/emit_x64.dasc"
    } else if (op == MVM_OP_sp_guardconttype) {
        MVMint16 val_spesh_idx = guard->ins->operands[2].lit_i16;
        //| test TMP1, TMP1;
        //| jz >1;
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov FUNCTION, OBJECT:TMP1->st;
        //| cmp TMP2, FUNCTION;
        //| jne >1;
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->fetch;
        //| mov ARG2, TMP1;
        //| mov ARG1, TC;
        //| lea ARG3, [rbp-0x28]; // hurray for scratch space
        //| call FUNCTION;
        //| mov TMP3, aword [rbp-0x28];
        //| test TMP3, TMP3;
        //| jz >1;
        //| is_type_object TMP3;
        dasm_put(Dst, 3265, DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), DtF(->container_spec), Dt14(->fetch), DtC(->header.flags), MVM_CF_TYPE_OBJECT);
#line 2095 "src/jit/emit_x64.dasc"
        // all of this is the same as for guardcontconc, but this time
        // we want to have a type object instead of a concrete object.
        //| jz >1;
        //| mov TMP2, OBJECT:TMP3->st;
        //| get_spesh_slot TMP1, val_spesh_idx;
        //| cmp TMP1, TMP2;
        //| jne >1;
        dasm_put(Dst, 3335, DtC(->st), Dt1F(->effective_spesh_slots), Dt13([val_spesh_idx]));
#line 2102 "src/jit/emit_x64.dasc"
    } else if (op == MVM_OP_sp_guardrwconc) {
        MVMint16 val_spesh_idx  = guard->ins->operands[2].lit_i16;
        /* if (check && IS_CONCRETE(check) */
        //| test TMP1, TMP1;
        //| jz >1;
        //| is_type_object TMP1;
        //| jnz >1;
        dasm_put(Dst, 3361, DtC(->header.flags), MVM_CF_TYPE_OBJECT);
#line 2109 "src/jit/emit_x64.dasc"
        /* && STABLE(check) == (want_c = cur_frame->effective_spesh_slots[GET_UI16(cur_op,2)] */
        //| mov FUNCTION, OBJECT:TMP1->st;
        //| cmp TMP2, FUNCTION;
        //| jne >1;
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->can_store;
        //| mov ARG2, TMP1;
        //| mov ARG1, TC;
        //| call FUNCTION;
        dasm_put(Dst, 3379, DtC(->st), DtF(->container_spec), Dt14(->can_store));
#line 2118 "src/jit/emit_x64.dasc"
        /* if (constspec->can_store(tc, check) */
        //| test RV, RV;
        //| jz >1;
        dasm_put(Dst, 3410);
#line 2121 "src/jit/emit_x64.dasc"
        /* restore TMP1 */
        //| mov TMP1, WORK[obj];
        //| mov FUNCTION, OBJECT:TMP1->st; // restore FUNCTION to the st as before
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->fetch;
        dasm_put(Dst, 3418, Dt1E([obj]), DtC(->st), DtF(->container_spec), Dt14(->fetch));
#line 2126 "src/jit/emit_x64.dasc"
        /* constpec->fetch(tc, check, &r) */
        //| mov ARG2, TMP1;
        //| mov ARG1, TC;
        //| lea ARG3, [rbp-0x28]; // hurray for scratch space
        //| call FUNCTION;
        dasm_put(Dst, 3435);
#line 2131 "src/jit/emit_x64.dasc"
        /* if (r.o && IS_CONCRETE(r.o) && STABLE(r.o) == want_v) */
        //| mov TMP3, aword [rbp-0x28];
        //| test TMP3, TMP3;
        //| jz >1;
        //| is_type_object TMP3;
        //| jnz >1;
        //| mov TMP2, OBJECT:TMP3->st;
        //| get_spesh_slot TMP1, val_spesh_idx;
        //| cmp TMP1, TMP2;
        //| jne >1;
        dasm_put(Dst, 3451, DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), Dt1F(->effective_spesh_slots), Dt13([val_spesh_idx]));
#line 2141 "src/jit/emit_x64.dasc"
    } else if (op == MVM_OP_sp_guardrwtype) {
        MVMint16 val_spesh_idx  = guard->ins->operands[2].lit_i16;
        /* if (check && IS_CONCRETE(check) && STABLE(check) == want_c) */
        //| test TMP1, TMP1;
        //| jz >1;
        //| is_type_object TMP1;
        //| jnz >1;
        //| mov FUNCTION, OBJECT:TMP1->st;
        //| cmp TMP2, FUNCTION;
        //| jne >1;
        dasm_put(Dst, 3495, DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st));
#line 2151 "src/jit/emit_x64.dasc"
        /* if (constpect->can_store(tc, check)) */
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->can_store;
        //| mov ARG2, TMP1;
        //| mov ARG1, TC;
        //| call FUNCTION;
        //| test RV, RV;
        //| jz >1;
        //| mov TMP1, WORK[obj];
        //| mov FUNCTION, OBJECT:TMP1->st; // restore FUNCTION to the st as before
        //| mov FUNCTION, STABLE:FUNCTION->container_spec;
        //| mov FUNCTION, CONTAINERSPEC:FUNCTION->fetch;
        //| mov ARG2, TMP1;
        //| mov ARG1, TC;
        //| lea ARG3, [rbp-0x28];
        dasm_put(Dst, 3524, DtF(->container_spec), Dt14(->can_store), Dt1E([obj]), DtC(->st), DtF(->container_spec), Dt14(->fetch));
#line 2166 "src/jit/emit_x64.dasc"
        /* contspec->fetch(tc, check, &r); */
        //| call FUNCTION;
        //| mov TMP3, aword [rbp-0x28];
        dasm_put(Dst, 3578);
#line 2169 "src/jit/emit_x64.dasc"
        /* if (r.o && !IS_CONCRETE(r.o) && STABLE(r.o) == want_v) */
        //| test TMP3, TMP3;
        //| jz >1;
        //| is_type_object TMP3;
        //| jz >1;
        //| mov TMP2, OBJECT:TMP3->st;
        //| get_spesh_slot TMP1, val_spesh_idx;
        //| cmp TMP1, TMP2;
        //| jne >1;
        dasm_put(Dst, 3587, DtC(->header.flags), MVM_CF_TYPE_OBJECT, DtC(->st), Dt1F(->effective_spesh_slots), Dt13([val_spesh_idx]));
#line 2178 "src/jit/emit_x64.dasc"
    }
    /* if we're here, we didn't jump to deopt, so skip it */
    //| jmp >2;
    //|1:
    dasm_put(Dst, 1853);
#line 2182 "src/jit/emit_x64.dasc"
    /* emit deopt */
    //| mov ARG1, TC;
    //| mov ARG2, guard->deopt_offset;
    //| mov ARG3, guard->deopt_target;
    //| callp &MVM_spesh_deopt_one_direct;
    dasm_put(Dst, 3627, guard->deopt_offset, guard->deopt_target, (unsigned int)((uintptr_t)&MVM_spesh_deopt_one_direct), (unsigned int)(((uintptr_t)&MVM_spesh_deopt_one_direct)>>32));
#line 2187 "src/jit/emit_x64.dasc"
    /* tell jit driver we're deopting */
    //| mov RV, MVM_JIT_CTRL_DEOPT
    //| jmp ->out;
    //|2:
    dasm_put(Dst, 3648, MVM_JIT_CTRL_DEOPT);
#line 2191 "src/jit/emit_x64.dasc"
}

void MVM_jit_emit_invoke(MVMThreadContext *tc, MVMJitGraph *jg, MVMJitInvoke *invoke,
                         dasm_State **Dst) {
    MVMint16 i;
    MVM_jit_log(tc, "Emit invoke (%d args)\n", invoke->arg_count);
    /* setup the callsite */
    //| mov ARG1, TC;
    //| mov ARG2, CU;
    //| mov ARG3, invoke->callsite_idx;
    //| callp &MVM_args_prepare;
    //| mov TMP6, RV; // store callsite in tmp6, which we don't use until the end
    dasm_put(Dst, 3659, invoke->callsite_idx, (unsigned int)((uintptr_t)&MVM_args_prepare), (unsigned int)(((uintptr_t)&MVM_args_prepare)>>32));
#line 2203 "src/jit/emit_x64.dasc"
    /* Store arguments in the buffer. I use TMP5 as it never conflicts
     * with argument passing (like TMP6, but unlike other TMP regs) */
    //| mov TMP5, FRAME->args;
    dasm_put(Dst, 3683, Dt1F(->args));
#line 2206 "src/jit/emit_x64.dasc"
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
            dasm_put(Dst, 3690, Dt1E([src]), Dt1([dst]));
#line 2217 "src/jit/emit_x64.dasc"
            break;
        }
        case MVM_OP_argconst_n:
        case MVM_OP_argconst_i: {
            MVMint16 dst = ins->operands[0].lit_i16;
            MVMint64 val = ins->operands[1].lit_i64;
            //| mov64 TMP4, val;
            //| mov REGISTER:TMP5[dst], TMP4;
            dasm_put(Dst, 3699, (unsigned int)(val), (unsigned int)((val)>>32), Dt1([dst]));
#line 2225 "src/jit/emit_x64.dasc"
            break;
        }
        case MVM_OP_argconst_s: {
            MVMint16 dst = ins->operands[0].lit_i16;
            MVMint32 idx = ins->operands[1].lit_str_idx;
            //| get_string TMP4, idx;
             MVM_cu_ensure_string_decoded(tc, jg->sg->sf->body.cu, idx);
#line 2231 "src/jit/emit_x64.dasc"
            //| mov REGISTER:TMP5[dst], TMP4;
            dasm_put(Dst, 3708, Dt20(->body.strings), Dt12([idx]), Dt1([dst]));
#line 2232 "src/jit/emit_x64.dasc"
            break;
        }
        default:
            MVM_panic(1, "JIT invoke: Can't add arg <%s>",
                      ins->info->name);
        }
    }

    /* Setup the frame for returning to our current position */
    if (sizeof(MVMReturnType) == 4) {
        //| mov dword FRAME->return_type, invoke->return_type;
        dasm_put(Dst, 2555, Dt1F(->return_type), invoke->return_type);
#line 2243 "src/jit/emit_x64.dasc"
    } else {
        MVM_panic(1, "JIT: MVMReturnType has unexpected size");
    }
    /* The register for our return value */
    if (invoke->return_type == MVM_RETURN_VOID) {
        //| mov aword FRAME->return_value, NULL;
        dasm_put(Dst, 3721, Dt1F(->return_value), NULL);
#line 2249 "src/jit/emit_x64.dasc"
    } else {
        //| lea TMP2, WORK[invoke->return_register];
        //| mov aword FRAME->return_value, TMP2;
        dasm_put(Dst, 3729, Dt1E([invoke->return_register]), Dt1F(->return_value));
#line 2252 "src/jit/emit_x64.dasc"
    }
    /* The return address for the interpreter */
    //| get_cur_op TMP2;
    //| mov aword FRAME->return_address, TMP2;
    dasm_put(Dst, 3740, Dt1D(->interp_cur_op), Dt1F(->return_address));
#line 2256 "src/jit/emit_x64.dasc"

    /* The re-entry label for the JIT, so that we continue in the next BB */
    //| lea TMP2, [=>(invoke->reentry_label)];
    //| mov aword FRAME->jit_entry_label, TMP2;
    dasm_put(Dst, 3754, (invoke->reentry_label), Dt1F(->jit_entry_label));
#line 2260 "src/jit/emit_x64.dasc"

    /* if we're not fast, then we should get the code from multi resolution */
    if (!invoke->is_fast) {
        /* first, save callsite and args */
        //| mov qword [rbp-0x28], TMP5; // args
        //| mov qword [rbp-0x30], TMP6; // callsite
        dasm_put(Dst, 3765);
#line 2266 "src/jit/emit_x64.dasc"
        /* setup call MVM_frame_multi_ok(tc, code, &cur_callsite, args); */
        //| mov ARG1, TC;
        //| mov ARG2, WORK[invoke->code_register]; // code object
        //| lea ARG3, [rbp-0x30];                  // &cur_callsite
        //| mov ARG4, TMP5;                        // args
        //| callp &MVM_frame_find_invokee_multi_ok;
        dasm_put(Dst, 3774, Dt1E([invoke->code_register]), (unsigned int)((uintptr_t)&MVM_frame_find_invokee_multi_ok), (unsigned int)(((uintptr_t)&MVM_frame_find_invokee_multi_ok)>>32));
#line 2272 "src/jit/emit_x64.dasc"
        /* restore callsite, args, RV now holds code object */
        //| mov TMP6, [rbp-0x30]; // callsite
        //| mov TMP5, [rbp-0x28]; // args
        dasm_put(Dst, 3798);
#line 2275 "src/jit/emit_x64.dasc"
        /* setup args for call to invoke(tc, code, cur_callsite, args) */
        //| mov ARG1, TC;
        //| mov ARG2, RV;   // code object
        //| mov ARG3, TMP6; // callsite
        //| mov ARG4, TMP5; // args
        dasm_put(Dst, 3807);
#line 2280 "src/jit/emit_x64.dasc"
        /* get the actual function */
        //| mov FUNCTION, OBJECT:RV->st;
        //| mov FUNCTION, STABLE:FUNCTION->invoke;
        //| call FUNCTION;
        dasm_put(Dst, 3821, DtC(->st), DtF(->invoke));
#line 2284 "src/jit/emit_x64.dasc"
    } else {
        /* call MVM_frame_invoke_code */
        //| mov ARG1, TC;
        //| mov ARG2, WORK[invoke->code_register];
        //| mov ARG3, TMP6; // this is the callsite object
        //| mov ARG4, invoke->spesh_cand;
        //| callp &MVM_frame_invoke_code;
        dasm_put(Dst, 3834, Dt1E([invoke->code_register]), invoke->spesh_cand, (unsigned int)((uintptr_t)&MVM_frame_invoke_code), (unsigned int)(((uintptr_t)&MVM_frame_invoke_code)>>32));
#line 2291 "src/jit/emit_x64.dasc"
    }
    /* Almost done. jump out into the interprete */
    //| mov RV, 1;
    //| jmp ->out;
    dasm_put(Dst, 3858);
#line 2295 "src/jit/emit_x64.dasc"
}

void MVM_jit_emit_jumplist(MVMThreadContext *tc, MVMJitGraph *jg,
                           MVMJitJumpList *jumplist, dasm_State **Dst) {
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
    dasm_put(Dst, 3870, Dt1E([jumplist->reg]), jumplist->num_labels);
#line 2312 "src/jit/emit_x64.dasc"
    for (i = 0; i < jumplist->num_labels; i++) {
        //|=>(jumplist->in_labels[i]):
        //| jmp =>(jumplist->out_labels[i]);
        //|.align 8;
        dasm_put(Dst, 3912, (jumplist->in_labels[i]), (jumplist->out_labels[i]));
#line 2316 "src/jit/emit_x64.dasc"
    }
    //|2:
    dasm_put(Dst, 634);
#line 2318 "src/jit/emit_x64.dasc"
}

void MVM_jit_emit_control(MVMThreadContext *tc, MVMJitGraph *jg,
                          MVMJitControl *ctrl, dasm_State **Dst) {
    if (ctrl->type == MVM_JIT_CONTROL_INVOKISH) {
        MVM_jit_log(tc, "Emit invokish control guard\n");
        //| cmp FRAME, aword TC->cur_frame;
        //| je >1;
        //| lea TMP1, [>1];
        //| mov aword FRAME->jit_entry_label, TMP1;
        //| mov RV, 1;
        //| jmp ->out;
        //|1:
        dasm_put(Dst, 3919, Dt1D(->cur_frame), Dt1F(->jit_entry_label));
#line 2331 "src/jit/emit_x64.dasc"
    }
    else if (ctrl->type == MVM_JIT_CONTROL_DYNAMIC_LABEL) {
        MVM_jit_log(tc, "Emit throwish control guard\n");
        /* This pre-loads a label for the next op, so that throwish
         * operators will know where we're throwing from - NB, I'm not
         * totally sure this is still necessary now */
        //| lea TMP1, [>1];
        //| mov aword FRAME->jit_entry_label, TMP1;
        //|1:
        dasm_put(Dst, 3952, Dt1F(->jit_entry_label));
#line 2340 "src/jit/emit_x64.dasc"
    }
    else if (ctrl->type == MVM_JIT_CONTROL_THROWISH_PRE) {
        /* Store a reference to the 9 label below. I assume this label
         * isn't used in the intermediate instructions, which is
         * basically valid throughout this codebase. */
        //| lea TMP1, [>9];
        //| mov aword FRAME->jit_entry_label, TMP1;
        dasm_put(Dst, 3966, Dt1F(->jit_entry_label));
#line 2347 "src/jit/emit_x64.dasc"
    }
    else if (ctrl->type == MVM_JIT_CONTROL_THROWISH_POST) {
        /* check if our current frame is the same as it was */
        //| cmp FRAME, aword TC->cur_frame;
        //| jne >8;
        dasm_put(Dst, 3978, Dt1D(->cur_frame));
#line 2352 "src/jit/emit_x64.dasc"
        /* if it is, we may still be in a goto-handler, so in that
         * case the throwing machinery should have ensured that
         * jit_entry_label now points to the correct label. If not, it
         * still points to 9:. */
        //| jmp aword FRAME->jit_entry_label;
        dasm_put(Dst, 3987, Dt1F(->jit_entry_label));
#line 2357 "src/jit/emit_x64.dasc"
        /* if not the same frame, trampoline to interpreter */
        //|8:
        //| mov RV, 1;
        //| jmp ->out;
        dasm_put(Dst, 3995);
#line 2361 "src/jit/emit_x64.dasc"
        /* I assume we *never* use a local label 9 in the code between
         * pre-and-post throwish */
        //|9:
        dasm_put(Dst, 4009);
#line 2364 "src/jit/emit_x64.dasc"
    } else if (ctrl->type == MVM_JIT_CONTROL_BREAKPOINT) {
        /* brrts evil testing purposes breakpoint */
        //| int 3;
        dasm_put(Dst, 4012);
#line 2367 "src/jit/emit_x64.dasc"
    } else {
        MVM_panic(1, "Unknown conrtol code: <%s>", ctrl->ins->info->name);
    }
}
