CC     = gcc
LD     = gcc
AR     = ar
MAKE   = make 
PERL   = /usr/bin/perl
RM     = rm -f
CAT    = cat
SHELL  = /bin/sh
CP     = $(PERL) -MExtUtils::Command -e cp
MKPATH = $(PERL) -MExtUtils::Command -e mkpath
CHMOD  = $(PERL) -MExtUtils::Command -e chmod
RM_RF  = $(PERL) -MExtUtils::Command -e rm_rf
RM_F   = $(PERL) -MExtUtils::Command -e rm_f

MINGW_UNICODE = 


CONFIG    = --optimize --prefix=/home/travis/brew/moar-nom/install --make-install
ADDCONFIG =

TRACING = 0
CGOTO = 1
RDTSCP = 1
NOISY = 0

MSG = @:
CMD =
NOOUT =
NOERR =

ifneq ($(NOISY), 1)
MSG = @echo
CMD = @
NOOUT = > /dev/null
NOERR = 2> /dev/null
endif


PREFIX    = /home/travis/brew/moar-nom/install
BINDIR    = /home/travis/brew/moar-nom/install/bin
LIBDIR    = /home/travis/brew/moar-nom/install/lib
MASTDIR   = /home/travis/brew/moar-nom/install/share/nqp/lib/MAST

PKGCONFIGDIR = /home/travis/brew/moar-nom/install/share/pkgconfig

CFLAGS    = -Werror=declaration-after-statement -Werror=pointer-arith -O3 -DNDEBUG  -D_REENTRANT -D_FILE_OFFSET_BITS=64 -fPIC -DMVM_TRACING=$(TRACING) -DMVM_CGOTO=$(CGOTO) -DMVM_RDTSCP=$(RDTSCP)
CINCLUDES =  -I3rdparty/libuv/include -I3rdparty/libuv/src -I3rdparty/libatomic_ops/src -I3rdparty/libtommath -I3rdparty/dyncall/dynload -I3rdparty/dyncall/dyncall -I3rdparty/dyncall/dyncallback \
            -I3rdparty/sha1 \
            -I3rdparty/tinymt \
            -I3rdparty/dynasm \
            -I3rdparty \
            -Isrc
LDFLAGS   =  -O3 -DNDEBUG -Wl,-rpath,"//home/travis/brew/moar-nom/install/lib"
LDLIBS    = -lm -lpthread -lrt -ldl
MAIN_LIBS = -L. -lmoar
DLL_LIBS  = 3rdparty/dyncall/dyncall/libdyncall_s.a 3rdparty/dyncall/dyncallback/libdyncallback_s.a 3rdparty/dyncall/dynload/libdynload_s.a 3rdparty/libatomic_ops/src/libatomic_ops.a 3rdparty/tinymt/libtinymt.a 3rdparty/sha1/libsha1.a 3rdparty/libtommath/libtommath.a 3rdparty/libuv/libuv.a $(LDLIBS)
ARFLAGS   = rcs

PLATFORM_WIN32 = src/platform/win32/mmap.o \
                 src/platform/win32/time.o \
                 src/platform/win32/io.o

PLATFORM_POSIX = src/platform/posix/mmap.o \
                 src/platform/posix/time.o \
                 src/platform/posix/io.o

DASM_FLAGS   = -D POSIX=1
JIT_ARCH_X64 = src/jit/x64/emit.o src/jit/x64/arch.o
JIT_STUB     = src/jit/stub.o
JIT_OBJECTS  = src/jit/graph.o \
               src/jit/label.o \
               src/jit/compile.o \
               src/jit/log.o \
               src/jit/expr.o \
               src/jit/tile.o \
               src/jit/linear_scan.o

# JIT intermediate files which clean should remove
JIT_INTERMEDIATES = src/jit/x64/emit.c \
                    src/jit/x64/tile_pattern.h \
                    src/jit/core_templates.h

MAIN_OBJECTS = src/main.o


OBJECTS = src/core/callsite.o \
          src/core/args.o \
          src/core/exceptions.o \
          src/core/interp.o \
          src/core/threadcontext.o \
          src/core/compunit.o \
          src/core/bytecode.o \
          src/core/frame.o \
          src/core/callstack.o \
          src/core/validation.o \
          src/core/bytecodedump.o \
          src/core/threads.o \
          src/core/ops.o \
          src/core/hll.o \
          src/core/loadbytecode.o \
          src/math/num.o \
          src/core/coerce.o \
          src/core/dll.o \
          src/core/ext.o \
          src/core/nativecall.o \
          src/core/nativecall_dyncall.o \
          src/core/continuation.o \
          src/core/intcache.o \
          src/core/fixedsizealloc.o \
          src/core/regionalloc.o \
          src/gen/config.o \
          src/gc/orchestrate.o \
          src/gc/allocation.o \
          src/gc/worklist.o \
          src/gc/roots.o \
          src/gc/collect.o \
          src/gc/gen2.o \
          src/gc/wb.o \
          src/gc/objectid.o \
          src/gc/finalize.o \
          src/gc/debug.o \
          src/io/io.o \
          src/io/eventloop.o \
          src/io/syncfile.o \
          src/io/syncsocket.o \
          src/io/fileops.o \
          src/io/dirops.o \
          src/io/procops.o \
          src/io/timers.o \
          src/io/filewatchers.o \
          src/io/signals.o \
          src/io/asyncsocket.o \
          src/io/asyncsocketudp.o \
          src/6model/reprs.o \
          src/6model/reprconv.o \
          src/6model/containers.o \
          src/6model/parametric.o \
          src/6model/reprs/MVMString.o \
          src/6model/reprs/VMArray.o \
          src/6model/reprs/MVMHash.o \
          src/6model/reprs/MVMCFunction.o \
          src/6model/reprs/KnowHOWREPR.o \
          src/6model/reprs/KnowHOWAttributeREPR.o \
          src/6model/reprs/P6str.o \
          src/6model/reprs/P6opaque.o \
          src/6model/reprs/MVMCode.o \
          src/6model/reprs/MVMOSHandle.o \
          src/6model/reprs/MVMCompUnit.o \
          src/6model/reprs/MVMStaticFrame.o \
          src/6model/reprs/P6int.o \
          src/6model/reprs/P6num.o \
          src/6model/reprs/Uninstantiable.o \
          src/6model/reprs/HashAttrStore.o \
          src/6model/reprs/MVMThread.o \
          src/6model/reprs/MVMIter.o \
          src/6model/reprs/MVMContext.o \
          src/6model/reprs/SCRef.o \
          src/6model/reprs/MVMCallCapture.o \
          src/6model/reprs/P6bigint.o \
          src/6model/reprs/NFA.o \
          src/6model/reprs/MVMException.o \
          src/6model/reprs/MVMDLLSym.o \
          src/6model/reprs/MVMMultiCache.o \
          src/6model/reprs/MVMContinuation.o \
          src/6model/reprs/NativeCall.o \
          src/6model/reprs/CPointer.o \
          src/6model/reprs/CStr.o \
          src/6model/reprs/CArray.o \
          src/6model/reprs/CStruct.o \
          src/6model/reprs/CUnion.o \
          src/6model/reprs/ReentrantMutex.o \
          src/6model/reprs/ConditionVariable.o \
          src/6model/reprs/Semaphore.o \
          src/6model/reprs/ConcBlockingQueue.o \
          src/6model/reprs/MVMAsyncTask.o \
          src/6model/reprs/MVMNull.o \
          src/6model/reprs/CPPStruct.o \
          src/6model/reprs/NativeRef.o \
          src/6model/reprs/MultiDimArray.o \
          src/6model/reprs/Decoder.o \
          src/6model/reprs/MVMSpeshLog.o \
          src/6model/reprs/MVMStaticFrameSpesh.o \
          src/6model/6model.o \
          src/6model/bootstrap.o \
          src/6model/sc.o \
          src/6model/serialization.o \
          src/mast/compiler.o \
          src/mast/driver.o \
          src/spesh/dump.o \
          src/spesh/graph.o \
          src/spesh/codegen.o \
          src/spesh/candidate.o \
          src/spesh/manipulate.o \
          src/spesh/args.o \
          src/spesh/facts.o \
          src/spesh/optimize.o \
          src/spesh/dead_bb_elimination.o \
          src/spesh/deopt.o \
          src/spesh/log.o \
          src/spesh/threshold.o \
          src/spesh/inline.o \
          src/spesh/osr.o \
          src/spesh/lookup.o \
          src/spesh/iterator.o \
          src/spesh/worker.o \
          src/spesh/stats.o \
          src/spesh/plan.o \
          src/spesh/arg_guard.o \
          src/strings/decode_stream.o \
          src/strings/ascii.o \
          src/strings/parse_num.o \
          src/strings/utf8.o \
          src/strings/utf8_c8.o \
          src/strings/nfg.o \
          src/strings/ops.o \
          src/strings/unicode.o \
          src/strings/normalize.o \
          src/strings/latin1.o \
          src/strings/utf16.o \
          src/strings/windows1252.o \
          src/math/bigintops.o \
          src/profiler/instrument.o \
          src/profiler/log.o \
          src/profiler/profile.o \
          src/profiler/heapsnapshot.o \
          src/profiler/telemeh.o \
          src/instrument/crossthreadwrite.o \
          src/instrument/line_coverage.o \
          src/platform/sys.o \
          src/moar.o \
          $(PLATFORM_POSIX) \
          $(JIT_OBJECTS) $(JIT_ARCH_X64)

HEADERS = src/moar.h \
          src/types.h \
          src/bithacks.h \
          src/6model/6model.h \
          src/core/instance.h \
          src/core/threadcontext.h \
          src/core/callsite.h \
          src/core/args.h \
          src/core/exceptions.h \
          src/core/interp.h \
          src/core/alloc.h \
          src/core/vector.h \
          src/core/frame.h \
          src/core/callstack.h \
          src/core/compunit.h \
          src/core/bytecode.h \
          src/core/ops.h \
          src/core/validation.h \
          src/core/bytecodedump.h \
          src/core/threads.h \
          src/core/hll.h \
          src/core/loadbytecode.h \
          src/core/bitmap.h \
          src/math/num.h \
          src/core/coerce.h \
          src/core/dll.h \
          src/core/ext.h \
          src/core/nativecall.h \
          src/core/continuation.h \
          src/core/intcache.h \
          src/core/fixedsizealloc.h \
          src/core/regionalloc.h \
          src/io/io.h \
          src/io/eventloop.h \
          src/io/syncfile.h \
          src/io/syncsocket.h \
          src/io/fileops.h \
          src/io/dirops.h \
          src/io/procops.h \
          src/io/timers.h \
          src/io/filewatchers.h \
          src/io/signals.h \
          src/io/asyncsocket.h \
          src/io/asyncsocketudp.h \
          src/gc/orchestrate.h \
          src/gc/allocation.h \
          src/gc/worklist.h \
          src/gc/collect.h \
          src/gc/roots.h \
          src/gc/gen2.h \
          src/gc/wb.h \
          src/gc/objectid.h \
          src/gc/finalize.h \
          src/gc/debug.h \
          src/6model/reprs.h \
          src/6model/reprconv.h \
          src/6model/bootstrap.h \
          src/6model/serialization.h \
          src/6model/containers.h \
          src/6model/parametric.h \
          src/6model/reprs/MVMString.h \
          src/6model/reprs/VMArray.h \
          src/6model/reprs/MVMHash.h \
          src/6model/reprs/MVMCFunction.h \
          src/6model/reprs/KnowHOWREPR.h \
          src/6model/reprs/KnowHOWAttributeREPR.h \
          src/6model/reprs/P6opaque.h \
          src/6model/reprs/MVMCode.h \
          src/6model/reprs/P6str.h \
          src/6model/reprs/MVMOSHandle.h \
          src/6model/reprs/P6int.h \
          src/6model/reprs/P6num.h \
          src/6model/reprs/Uninstantiable.h \
          src/6model/reprs/HashAttrStore.h \
          src/6model/reprs/MVMThread.h \
          src/6model/reprs/MVMIter.h \
          src/6model/reprs/MVMContext.h \
          src/6model/reprs/SCRef.h \
          src/6model/reprs/MVMCallCapture.h \
          src/6model/reprs/P6bigint.h \
          src/6model/reprs/NFA.h \
          src/6model/reprs/MVMException.h \
          src/6model/reprs/MVMCompUnit.h \
          src/6model/reprs/MVMDLLSym.h \
          src/6model/reprs/MVMMultiCache.h \
          src/6model/reprs/MVMContinuation.h \
          src/6model/reprs/NativeCall.h \
          src/6model/reprs/CPointer.h \
          src/6model/reprs/CStr.h \
          src/6model/reprs/CArray.h \
          src/6model/reprs/CStruct.h \
          src/6model/reprs/CUnion.h \
          src/6model/reprs/ReentrantMutex.h \
          src/6model/reprs/ConditionVariable.h \
          src/6model/reprs/Semaphore.h \
          src/6model/reprs/ConcBlockingQueue.h \
          src/6model/reprs/MVMAsyncTask.h \
          src/6model/reprs/MVMNull.h \
          src/6model/reprs/CPPStruct.h \
          src/6model/reprs/NativeRef.h \
          src/6model/reprs/MultiDimArray.h \
          src/6model/reprs/Decoder.h \
          src/6model/reprs/MVMSpeshLog.h \
          src/6model/reprs/MVMStaticFrameSpesh.h \
          src/6model/sc.h \
          src/mast/compiler.h \
          src/mast/driver.h \
          src/mast/nodes.h \
          src/spesh/dump.h \
          src/spesh/graph.h \
          src/spesh/codegen.h \
          src/spesh/candidate.h \
          src/spesh/manipulate.h \
          src/spesh/args.h \
          src/spesh/facts.h \
          src/spesh/optimize.h \
          src/spesh/dead_bb_elimination.h \
          src/spesh/deopt.h \
          src/spesh/log.h \
          src/spesh/threshold.h \
          src/spesh/inline.h \
          src/spesh/osr.h \
          src/spesh/lookup.h \
          src/spesh/iterator.h \
          src/spesh/worker.h \
          src/spesh/stats.h \
          src/spesh/plan.h \
          src/spesh/arg_guard.h \
          src/strings/unicode_gen.h \
          src/strings/normalize.h \
          src/strings/decode_stream.h \
          src/strings/ascii.h \
          src/strings/parse_num.h \
          src/strings/utf8.h \
          src/strings/utf8_c8.h \
          src/strings/iter.h \
          src/strings/nfg.h \
          src/strings/ops.h \
          src/strings/unicode.h \
          src/strings/latin1.h \
          src/strings/utf16.h \
          src/strings/windows1252.h \
          src/math/bigintops.h \
          src/profiler/instrument.h \
          src/profiler/log.h \
          src/profiler/profile.h \
          src/profiler/heapsnapshot.h \
          src/profiler/telemeh.h \
          src/platform/mmap.h \
          src/platform/time.h \
          src/platform/threads.h \
          src/platform/io.h \
          src/platform/sys.h \
          src/platform/setjmp.h \
          src/platform/memmem.h \
          src/jit/graph.h \
          src/jit/label.h \
          src/jit/expr.h \
          src/jit/expr_ops.h \
          src/jit/compile.h \
          src/jit/tile.h \
          src/jit/register.h \
          src/jit/log.h \
          src/instrument/crossthreadwrite.h \
          src/instrument/line_coverage.h \
          src/gen/config.h \
          3rdparty/uthash.h

UV_UNIX = 3rdparty/libuv/src/fs-poll.o \
          3rdparty/libuv/src/inet.o \
          3rdparty/libuv/src/uv-common.o \
          3rdparty/libuv/src/version.o \
          3rdparty/libuv/src/unix/async.o \
          3rdparty/libuv/src/unix/core.o \
          3rdparty/libuv/src/unix/dl.o \
          3rdparty/libuv/src/unix/fs.o \
          3rdparty/libuv/src/unix/getaddrinfo.o \
          3rdparty/libuv/src/unix/loop-watcher.o \
          3rdparty/libuv/src/unix/loop.o \
          3rdparty/libuv/src/unix/pipe.o \
          3rdparty/libuv/src/unix/poll.o \
          3rdparty/libuv/src/unix/process.o \
          3rdparty/libuv/src/unix/signal.o \
          3rdparty/libuv/src/unix/stream.o \
          3rdparty/libuv/src/unix/tcp.o \
          3rdparty/libuv/src/unix/thread.o \
          3rdparty/libuv/src/threadpool.o \
          3rdparty/libuv/src/unix/timer.o \
          3rdparty/libuv/src/unix/tty.o \
          3rdparty/libuv/src/unix/udp.o

UV_LINUX = 3rdparty/libuv/src/unix/linux-core.o \
           3rdparty/libuv/src/unix/linux-inotify.o \
           3rdparty/libuv/src/unix/linux-syscalls.o \
           3rdparty/libuv/src/unix/proctitle.o \
           $(UV_UNIX)

UV_OPENBSD = 3rdparty/libuv/src/unix/openbsd.o \
             3rdparty/libuv/src/unix/kqueue.o \
             3rdparty/libuv/src/unix/posix-hrtime.o \
             $(UV_UNIX)

UV_NETBSD = 3rdparty/libuv/src/unix/netbsd.o \
            3rdparty/libuv/src/unix/kqueue.o \
            3rdparty/libuv/src/unix/posix-hrtime.o \
            $(UV_UNIX)

UV_FREEBSD = 3rdparty/libuv/src/unix/freebsd.o \
             3rdparty/libuv/src/unix/kqueue.o \
             3rdparty/libuv/src/unix/posix-hrtime.o \
             $(UV_UNIX)

UV_DARWIN = 3rdparty/libuv/src/unix/darwin.o \
            3rdparty/libuv/src/unix/darwin-proctitle.o \
            3rdparty/libuv/src/unix/fsevents.o \
            3rdparty/libuv/src/unix/kqueue.o \
            3rdparty/libuv/src/unix/proctitle.o \
            $(UV_UNIX)

UV_SOLARIS = 3rdparty/libuv/src/unix/sunos.o \
            $(UV_UNIX)

UV_AIX = 3rdparty/libuv/src/unix/aix.o \
         $(UV_UNIX)

UV_OBJECTS = $(UV_LINUX)

TOM_OBJECTS = 3rdparty/libtommath/bn_error.o 3rdparty/libtommath/bn_fast_mp_invmod.o 3rdparty/libtommath/bn_fast_mp_montgomery_reduce.o 3rdparty/libtommath/bn_fast_s_mp_mul_digs.o 3rdparty/libtommath/bn_fast_s_mp_mul_high_digs.o 3rdparty/libtommath/bn_fast_s_mp_sqr.o 3rdparty/libtommath/bn_mp_2expt.o 3rdparty/libtommath/bn_mp_abs.o 3rdparty/libtommath/bn_mp_add.o 3rdparty/libtommath/bn_mp_add_d.o 3rdparty/libtommath/bn_mp_addmod.o 3rdparty/libtommath/bn_mp_and.o 3rdparty/libtommath/bn_mp_clamp.o 3rdparty/libtommath/bn_mp_clear.o 3rdparty/libtommath/bn_mp_clear_multi.o 3rdparty/libtommath/bn_mp_cmp.o 3rdparty/libtommath/bn_mp_cmp_d.o 3rdparty/libtommath/bn_mp_cmp_mag.o 3rdparty/libtommath/bn_mp_cnt_lsb.o 3rdparty/libtommath/bn_mp_copy.o 3rdparty/libtommath/bn_mp_count_bits.o 3rdparty/libtommath/bn_mp_div.o 3rdparty/libtommath/bn_mp_div_2.o 3rdparty/libtommath/bn_mp_div_2d.o 3rdparty/libtommath/bn_mp_div_3.o 3rdparty/libtommath/bn_mp_div_d.o 3rdparty/libtommath/bn_mp_dr_is_modulus.o 3rdparty/libtommath/bn_mp_dr_reduce.o 3rdparty/libtommath/bn_mp_dr_setup.o 3rdparty/libtommath/bn_mp_exch.o 3rdparty/libtommath/bn_mp_export.o 3rdparty/libtommath/bn_mp_expt_d.o 3rdparty/libtommath/bn_mp_expt_d_ex.o 3rdparty/libtommath/bn_mp_exptmod.o 3rdparty/libtommath/bn_mp_exptmod_fast.o 3rdparty/libtommath/bn_mp_exteuclid.o 3rdparty/libtommath/bn_mp_fread.o 3rdparty/libtommath/bn_mp_fwrite.o 3rdparty/libtommath/bn_mp_gcd.o 3rdparty/libtommath/bn_mp_get_int.o 3rdparty/libtommath/bn_mp_get_long.o 3rdparty/libtommath/bn_mp_get_long_long.o 3rdparty/libtommath/bn_mp_grow.o 3rdparty/libtommath/bn_mp_import.o 3rdparty/libtommath/bn_mp_init.o 3rdparty/libtommath/bn_mp_init_copy.o 3rdparty/libtommath/bn_mp_init_multi.o 3rdparty/libtommath/bn_mp_init_set.o 3rdparty/libtommath/bn_mp_init_set_int.o 3rdparty/libtommath/bn_mp_init_size.o 3rdparty/libtommath/bn_mp_invmod.o 3rdparty/libtommath/bn_mp_invmod_slow.o 3rdparty/libtommath/bn_mp_is_square.o 3rdparty/libtommath/bn_mp_jacobi.o 3rdparty/libtommath/bn_mp_karatsuba_mul.o 3rdparty/libtommath/bn_mp_karatsuba_sqr.o 3rdparty/libtommath/bn_mp_lcm.o 3rdparty/libtommath/bn_mp_lshd.o 3rdparty/libtommath/bn_mp_mod.o 3rdparty/libtommath/bn_mp_mod_2d.o 3rdparty/libtommath/bn_mp_mod_d.o 3rdparty/libtommath/bn_mp_montgomery_calc_normalization.o 3rdparty/libtommath/bn_mp_montgomery_reduce.o 3rdparty/libtommath/bn_mp_montgomery_setup.o 3rdparty/libtommath/bn_mp_mul.o 3rdparty/libtommath/bn_mp_mul_2.o 3rdparty/libtommath/bn_mp_mul_2d.o 3rdparty/libtommath/bn_mp_mul_d.o 3rdparty/libtommath/bn_mp_mulmod.o 3rdparty/libtommath/bn_mp_n_root.o 3rdparty/libtommath/bn_mp_n_root_ex.o 3rdparty/libtommath/bn_mp_neg.o 3rdparty/libtommath/bn_mp_or.o 3rdparty/libtommath/bn_mp_prime_fermat.o 3rdparty/libtommath/bn_mp_prime_is_divisible.o 3rdparty/libtommath/bn_mp_prime_is_prime.o 3rdparty/libtommath/bn_mp_prime_miller_rabin.o 3rdparty/libtommath/bn_mp_prime_next_prime.o 3rdparty/libtommath/bn_mp_prime_rabin_miller_trials.o 3rdparty/libtommath/bn_mp_prime_random_ex.o 3rdparty/libtommath/bn_mp_radix_size.o 3rdparty/libtommath/bn_mp_radix_smap.o 3rdparty/libtommath/bn_mp_rand.o 3rdparty/libtommath/bn_mp_read_radix.o 3rdparty/libtommath/bn_mp_read_signed_bin.o 3rdparty/libtommath/bn_mp_read_unsigned_bin.o 3rdparty/libtommath/bn_mp_reduce.o 3rdparty/libtommath/bn_mp_reduce_2k.o 3rdparty/libtommath/bn_mp_reduce_2k_l.o 3rdparty/libtommath/bn_mp_reduce_2k_setup.o 3rdparty/libtommath/bn_mp_reduce_2k_setup_l.o 3rdparty/libtommath/bn_mp_reduce_is_2k.o 3rdparty/libtommath/bn_mp_reduce_is_2k_l.o 3rdparty/libtommath/bn_mp_reduce_setup.o 3rdparty/libtommath/bn_mp_rshd.o 3rdparty/libtommath/bn_mp_set.o 3rdparty/libtommath/bn_mp_set_int.o 3rdparty/libtommath/bn_mp_set_long.o 3rdparty/libtommath/bn_mp_set_long_long.o 3rdparty/libtommath/bn_mp_shrink.o 3rdparty/libtommath/bn_mp_signed_bin_size.o 3rdparty/libtommath/bn_mp_sqr.o 3rdparty/libtommath/bn_mp_sqrmod.o 3rdparty/libtommath/bn_mp_sqrt.o 3rdparty/libtommath/bn_mp_sqrtmod_prime.o 3rdparty/libtommath/bn_mp_sub.o 3rdparty/libtommath/bn_mp_sub_d.o 3rdparty/libtommath/bn_mp_submod.o 3rdparty/libtommath/bn_mp_to_signed_bin.o 3rdparty/libtommath/bn_mp_to_signed_bin_n.o 3rdparty/libtommath/bn_mp_to_unsigned_bin.o 3rdparty/libtommath/bn_mp_to_unsigned_bin_n.o 3rdparty/libtommath/bn_mp_toom_mul.o 3rdparty/libtommath/bn_mp_toom_sqr.o 3rdparty/libtommath/bn_mp_toradix.o 3rdparty/libtommath/bn_mp_toradix_n.o 3rdparty/libtommath/bn_mp_unsigned_bin_size.o 3rdparty/libtommath/bn_mp_xor.o 3rdparty/libtommath/bn_mp_zero.o 3rdparty/libtommath/bn_prime_tab.o 3rdparty/libtommath/bn_reverse.o 3rdparty/libtommath/bn_s_mp_add.o 3rdparty/libtommath/bn_s_mp_exptmod.o 3rdparty/libtommath/bn_s_mp_mul_digs.o 3rdparty/libtommath/bn_s_mp_mul_high_digs.o 3rdparty/libtommath/bn_s_mp_sqr.o 3rdparty/libtommath/bn_s_mp_sub.o 3rdparty/libtommath/bncore.o

THIRDPARTY = 3rdparty/dyncall/dyncall/libdyncall_s.a 3rdparty/dyncall/dyncallback/libdyncallback_s.a 3rdparty/dyncall/dynload/libdynload_s.a 3rdparty/libatomic_ops/src/libatomic_ops.a 3rdparty/tinymt/libtinymt.a 3rdparty/sha1/libsha1.a 3rdparty/libtommath/libtommath.a 3rdparty/libuv/libuv.a

MINILUA = 3rdparty/dynasm/minilua
DYNASM  = $(MINILUA) 3rdparty/dynasm/dynasm.lua
DYNASM_SCRIPTS = 3rdparty/dynasm/dynasm.lua 3rdparty/dynasm/dasm_x86.lua
DYNASM_HEADERS = 3rdparty/dynasm/dasm_proto.h 3rdparty/dynasm/dasm_x86.h

.SUFFIXES: .c .o .i .s .dasc .expr .tile

all: moar pkgconfig/moar.pc

install: all
	$(MKPATH) "$(DESTDIR)$(BINDIR)"
	$(CP) moar "$(DESTDIR)$(BINDIR)"
	$(CHMOD) 755 "$(DESTDIR)$(BINDIR)/moar"
	$(MKPATH) "$(DESTDIR)$(LIBDIR)"
	$(RM_F) "$(DESTDIR)$(LIBDIR)/libmoar.so"
	$(CP) libmoar.so  "$(DESTDIR)$(LIBDIR)"
	$(MKPATH) "$(DESTDIR)$(MASTDIR)"
	$(CP) lib/MAST/Nodes.nqp "$(DESTDIR)$(MASTDIR)"
	$(CP) lib/MAST/Ops.nqp "$(DESTDIR)$(MASTDIR)"
	$(MKPATH) "$(DESTDIR)$(PKGCONFIGDIR)"
	$(CP) pkgconfig/moar.pc "$(DESTDIR)$(PKGCONFIGDIR)"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar"
	$(CP) src/gen/config.h "$(DESTDIR)$(PREFIX)/include/moar"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/6model/reprs"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/core"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/gc"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/gen"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/io"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/mast"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/math"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/platform"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/profiler"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/spesh"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/strings"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/jit"
	$(MKPATH) "$(DESTDIR)$(PREFIX)/include/moar/instrument"
	$(CP) 3rdparty/*.h "$(DESTDIR)$(PREFIX)/include/moar"
	$(CP) src/*.h "$(DESTDIR)$(PREFIX)/include/moar"
	$(CP) src/6model/*.h "$(DESTDIR)$(PREFIX)/include/moar/6model"
	$(CP) src/6model/reprs/*.h "$(DESTDIR)$(PREFIX)/include/moar/6model/reprs"
	$(CP) src/core/*.h "$(DESTDIR)$(PREFIX)/include/moar/core"
	$(CP) src/gc/*.h "$(DESTDIR)$(PREFIX)/include/moar/gc"
	$(CP) src/gen/*.h "$(DESTDIR)$(PREFIX)/include/moar/gen"
	$(CP) src/io/*.h "$(DESTDIR)$(PREFIX)/include/moar/io"
	$(CP) src/mast/*.h "$(DESTDIR)$(PREFIX)/include/moar/mast"
	$(CP) src/math/*.h "$(DESTDIR)$(PREFIX)/include/moar/math"
	$(CP) src/platform/*.h "$(DESTDIR)$(PREFIX)/include/moar/platform"
	$(CP) src/profiler/*.h "$(DESTDIR)$(PREFIX)/include/moar/profiler"
	$(CP) src/spesh/*.h "$(DESTDIR)$(PREFIX)/include/moar/spesh"
	$(CP) src/strings/*.h "$(DESTDIR)$(PREFIX)/include/moar/strings"
	$(CP) src/jit/*.h "$(DESTDIR)$(PREFIX)/include/moar/jit"
	$(CP) src/instrument/*.h "$(DESTDIR)$(PREFIX)/include/moar/instrument"
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libuv
	$(CP) 3rdparty/libuv/include/*.h $(DESTDIR)$(PREFIX)/include/libuv
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/armcc
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/gcc
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/hpc
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/ibmc
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/icc
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/loadstore
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/msftc
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/sunc
	$(CP) 3rdparty/libatomic_ops/src/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/armcc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/armcc
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/gcc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/gcc
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/hpc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/hpc
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/ibmc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/ibmc
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/icc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/icc
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/loadstore/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/loadstore
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/msftc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/msftc
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/sunc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/sunc
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libtommath
	$(CP) 3rdparty/libtommath/*.h $(DESTDIR)$(PREFIX)/include/libtommath
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/dyncall
	$(CP) 3rdparty/dyncall/dynload/*.h $(DESTDIR)$(PREFIX)/include/dyncall
	$(CP) 3rdparty/dyncall/dyncall/*.h $(DESTDIR)$(PREFIX)/include/dyncall
	$(CP) 3rdparty/dyncall/dyncallback/*.h $(DESTDIR)$(PREFIX)/include/dyncall


lib: libmoar.so

help:
	@$(CAT) build/help.txt

test:
	@$(CAT) build/test.txt

reconfig: realclean
	$(MSG) reconfiguring with [ $(CONFIG) $(ADDCONFIG) ]
	$(CMD)$(PERL) Configure.pl $(CONFIG) $(ADDCONFIG)

clangcheck gcccheck:
	@$(MAKE) --no-print-directory -f tools/check.mk $@

moar: $(MAIN_OBJECTS) libmoar.so
	$(MSG) linking $@
	$(CMD)$(LD) -o $@ $(LDFLAGS) $(MINGW_UNICODE) $(MAIN_OBJECTS) $(MAIN_LIBS)

libmoar.a: $(OBJECTS) $(THIRDPARTY)
	$(MSG) linking $@
	$(CMD)$(AR) $(ARFLAGS) $@ $(OBJECTS) $(NOOUT)

libmoar.so: $(OBJECTS) $(THIRDPARTY)
	$(MSG) linking $@
	$(CMD)$(LD) -o $@ $(LDFLAGS) -shared -fPIC  $(OBJECTS) $(DLL_LIBS)

libuv: 3rdparty/libuv/libuv.a

$(MAIN_OBJECTS) $(OBJECTS): $(HEADERS)

tracing:
	$(MSG) enable tracing dispatch
	-$(CMD)$(RM) src/main.o src/core/interp.o
	$(CMD)$(MAKE) TRACING=1 CGOTO=0 NOISY="$(NOISY)"

cgoto:
	$(MSG) enable computed-goto dispatch
	-$(CMD)$(RM) src/main.o src/core/interp.o
	$(CMD)$(MAKE) TRACING=0 CGOTO=1 NOISY="$(NOISY)"

switch no-tracing no-cgoto:
	$(MSG) enable regular dispatch
	-$(CMD)$(RM) src/main.o src/core/interp.o
	$(CMD)$(MAKE) TRACING=0 CGOTO=0 NOISY="$(NOISY)"

# objflags can include -DMVM_BUILD_SHARED which affects pre-processed source
.c.i:
	$(MSG) pre-processing $@
	$(CMD)$(CC) -E $(CFLAGS) -DMVM_BUILD_SHARED -fPIC $(CINCLUDES) > $@ $*.c

.c.s:
	$(MSG) generating assembler for $@
	$(CMD)$(CC) -S $(CFLAGS) -DMVM_BUILD_SHARED -fPIC $(CINCLUDES) -o $@ $*.c

.c.o:
	$(MSG) compiling $@
	$(CMD)$(CC) -c $(CFLAGS) -DMVM_BUILD_SHARED -fPIC $(CINCLUDES) -o $@ $*.c


src/main.o: src/main.c
	$(MSG) compiling $@
	$(CMD)$(CC) -c $(CFLAGS) -DMVM_SHARED $(CINCLUDES) -o $@ $*.c

src/strings/unicode.c: src/strings/unicode_db.c src/strings/unicode_uca.c src/strings/unicode_ops.c
	$(MSG) generating $@
	$(CMD) $(CAT) src/strings/unicode_db.c src/strings/unicode_uca.c src/strings/unicode_ops.c > $@ $(NOERR)

$(MINILUA): 3rdparty/dynasm/minilua.c
	$(CC) $(CFLAGS) $(LDFLAGS) 3rdparty/dynasm/minilua.c -o $@ $(LDLIBS)

.dasc.c:
	$(MSG) preprocessing $<
	$(CMD)$(DYNASM) $(DASM_FLAGS) -o $@ $<

.expr.h:
	$(MSG) Precompiling templates from $<
	$(CMD)$(PERL) -Itools/ tools/expr-template-compiler.pl -o $@ $<

.tile.h:
	$(MSG) Building tile tables from $<
	$(CMD)$(PERL) -Itools/ tools/tiler-table-generator.pl -o $@ $<

src/jit/x64/emit.o: src/jit/x64/emit.c $(DYNASM_HEADERS)

src/jit/x64/emit.c: src/jit/x64/emit.dasc src/jit/x64/tiles.dasc $(MINILUA) $(DYNASM_SCRIPTS)

# Expression list tables
src/jit/core_templates.h: src/jit/core_templates.expr src/jit/macro.expr tools/expr-template-compiler.pl src/core/oplist src/jit/expr_ops.h

# Architecture-specific tiles
src/jit/x64/tile_pattern.h: src/jit/x64/tile_pattern.tile tools/tiler-table-generator.pl src/jit/expr_ops.h

src/jit/expr.o: src/jit/core_templates.h
src/jit/tile.o: src/jit/x64/tile_pattern.h src/jit/x64/tile_decl.h

src/jit/compile.o src/jit/linear_scan.o src/jit/x64/arch.o $(JIT_OBJECTS) $(JIT_ARCH_X64): src/jit/internal.h src/jit/x64/arch.h




3rdparty/libuv/libuv.a: $(UV_OBJECTS)
	$(MSG) linking $@
	$(CMD)$(AR) $(ARFLAGS) $@ $(UV_LINUX) $(NOOUT)

3rdparty/libatomic_ops/src/libatomic_ops.a:
	$(MSG) linking $@
	$(CMD)cd 3rdparty/libatomic_ops && CC='$(CC)' CFLAGS='$(CFLAGS)' ./configure  && cd src && $(MAKE) && cd .. $(NOOUT)

3rdparty/libtommath/libtommath.a: $(TOM_OBJECTS)
	$(MSG) linking $@
	$(CMD)$(AR) $(ARFLAGS) $@ 3rdparty/libtommath/*.o $(NOOUT)

3rdparty/sha1/libsha1.a: 3rdparty/sha1/sha1.o
	$(MSG) linking $@
	$(CMD)$(AR) $(ARFLAGS) $@ 3rdparty/sha1/*.o $(NOOUT)

3rdparty/tinymt/libtinymt.a: 3rdparty/tinymt/tinymt64.o
	$(MSG) linking $@
	$(CMD)$(AR) $(ARFLAGS) $@ 3rdparty/tinymt/*.o $(NOOUT)

3rdparty/dyncall/dyncall/libdyncall_s.a:
	$(MSG) building dyncall...
	$(CMD)cd 3rdparty/dyncall &&  ./configure && CC='$(CC)' CFLAGS='-fPIC' $(MAKE) -f Makefile  $(NOOUT)
	$(MSG) done.

3rdparty/dyncall/dyncallback/libdyncallback_s.a 3rdparty/dyncall/dynload/libdynload_s.a: 3rdparty/dyncall/dyncall/libdyncall_s.a

pkgconfig/moar.pc: build/mk-moar-pc.pl
	$(PERL) build/mk-moar-pc.pl $@

clean:
	$(MSG) remove build files
	-$(CMD)$(RM) $(MAIN_OBJECTS) $(OBJECTS) $(JIT_INTERMEDIATES) $(NOOUT) $(NOERR)

realclean: clean
	$(MSG) remove auxiliary files
	-$(CMD)@: $(NOOUT) $(NOERR)
	$(MSG) remove 3rdparty files
	-$(CMD)$(RM) 3rdparty/libuv/libuv.a $(UV_LINUX) $(NOOUT) $(NOERR)
	-$(CMD)cd 3rdparty/libatomic_ops/src && $(MAKE) distclean $(NOOUT) $(NOERR)
	-$(CMD)$(RM) 3rdparty/libtommath/libtommath.a 3rdparty/libtommath/*.o $(NOOUT) $(NOERR)
	-$(CMD)$(RM) 3rdparty/sha1/libsha1.a 3rdparty/sha1/*.o $(NOOUT) $(NOERR)
	-$(CMD)$(RM) 3rdparty/tinymt/libtinymt.a 3rdparty/tinymt/*.o $(NOOUT) $(NOERR)
	-$(CMD)cd 3rdparty/dyncall && $(MAKE) -f Makefile clean $(NOOUT) $(NOERR)

distclean: realclean
	$(MSG) remove executable and libraries
	-$(CMD)$(RM) moar libmoar.a libmoar.so $(NOOUT) $(NOERR)
	$(MSG) remove configuration and generated files
	-$(CMD)$(RM) Makefile src/gen/config.h src/gen/config.c src/strings/unicode.c \
	    tools/check.mk 3rdparty/libatomic_ops/config.log 3rdparty/libatomic_ops/config.status $(NOOUT) $(NOERR)
	-$(CMD)$(RM_RF) build/mk-moar-pc.pl pkgconfig/ $(NOOUT) $(NOERR)

release:
	[ -n "$(VERSION)" ] || ( echo "\nTry 'make release VERSION=yyyy.mm'\n\n"; exit 1 )
	./tools/release.sh $(VERSION)


sandwich:
	@echo What? Make it yourself.
