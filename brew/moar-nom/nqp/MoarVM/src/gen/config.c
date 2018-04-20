/* !!!
 * If you're looking at config.c and thinking of editing - this is a
 * generated file. See build/config.c.in, unless you're just twiddling
 * to get things to work on your platform before updating Configure,
 * in which case go right ahead. :-)
 * !!!
 */

#include "moar.h"

#define add_entry(tc, hash, name, value) do { \
    MVMString * const key = MVM_string_ascii_decode_nt(tc, tc->instance->VMString, (name)); \
    MVMROOT(tc, key, { \
        MVMString * const value_str = MVM_string_ascii_decode_nt(tc, tc->instance->VMString, (value)); \
        MVMObject * const boxed_value = MVM_repr_box_str(tc, MVM_hll_current(tc)->str_box_type, value_str); \
        MVM_repr_bind_key_o(tc, hash, key, boxed_value); \
    }); \
} while (0)

MVMObject *MVM_backend_config(MVMThreadContext *tc) {
    MVMObject *config = tc->instance->cached_backend_config;
    if (config)
        return config;

    config = MVM_repr_alloc_init(tc, tc->instance->boot_types.BOOTHash);
    MVMROOT(tc, config, {
        add_entry(tc, config, "ar", "ar");
        add_entry(tc, config, "arflags", "rcs");
        add_entry(tc, config, "arout", "");
        add_entry(tc, config, "asm", ".s");
        add_entry(tc, config, "asmout", "-o ");
        add_entry(tc, config, "asmswitch", "-S");
        add_entry(tc, config, "auxclean", "@:");
        add_entry(tc, config, "be", "0");
        add_entry(tc, config, "bindir", "/home/travis/brew/moar-nom/install/bin");
        add_entry(tc, config, "booltype", "_Bool");
        add_entry(tc, config, "can_specific_werror", "1");
        add_entry(tc, config, "can_unaligned_int32", "1");
        add_entry(tc, config, "can_unaligned_int64", "1");
        add_entry(tc, config, "can_unaligned_num64", "1");
        add_entry(tc, config, "cancgoto", "1");
        add_entry(tc, config, "canrdtscp", "1");
        add_entry(tc, config, "cat", "cat");
        add_entry(tc, config, "cc", "gcc");
        add_entry(tc, config, "ccdebugflags", "-g3");
        add_entry(tc, config, "ccdef", "-D");
        add_entry(tc, config, "ccdefflags", "-D_REENTRANT -D_FILE_OFFSET_BITS=64");
        add_entry(tc, config, "ccinc", "-I");
        add_entry(tc, config, "ccinstflags", "-pg");
        add_entry(tc, config, "ccmiscflags", "-Werror=declaration-after-statement -Werror=pointer-arith");
        add_entry(tc, config, "ccoptiflags", "-O3 -DNDEBUG");
        add_entry(tc, config, "ccout", "-o ");
        add_entry(tc, config, "ccshared", "-fPIC");
        add_entry(tc, config, "ccswitch", "-c");
        add_entry(tc, config, "ccwarnflags", "");
        add_entry(tc, config, "cflags", "-Werror=declaration-after-statement -Werror=pointer-arith -O3 -DNDEBUG  -D_REENTRANT -D_FILE_OFFSET_BITS=64 -fPIC");
        add_entry(tc, config, "cincludes", " -I3rdparty/libuv/include -I3rdparty/libuv/src -I3rdparty/libatomic_ops/src -I3rdparty/libtommath -I3rdparty/dyncall/dynload -I3rdparty/dyncall/dyncall -I3rdparty/dyncall/dyncallback");
        add_entry(tc, config, "config", "--optimize --prefix=/home/travis/brew/moar-nom/install --make-install");
        add_entry(tc, config, "cppout", "> ");
        add_entry(tc, config, "cppswitch", "-E");
        add_entry(tc, config, "crossconf", "");
        add_entry(tc, config, "dasm_flags", "-D POSIX=1");
        add_entry(tc, config, "dcbclean", "$(RM) 3rdparty/dyncall/dyncallback/libdyncallback_s.a");
        add_entry(tc, config, "dcblib", "3rdparty/dyncall/dyncallback/libdyncallback_s.a");
        add_entry(tc, config, "dcbobjects", "");
        add_entry(tc, config, "dcbrule", "@:");
        add_entry(tc, config, "dcclean", "cd 3rdparty/dyncall && $(MAKE) -f Makefile clean");
        add_entry(tc, config, "dclib", "3rdparty/dyncall/dyncall/libdyncall_s.a");
        add_entry(tc, config, "dcobjects", "");
        add_entry(tc, config, "dcrule", "cd 3rdparty/dyncall &&  ./configure && CC='$(CC)' CFLAGS='-fPIC' $(MAKE) -f Makefile ");
        add_entry(tc, config, "defs[0]", "_REENTRANT");
        add_entry(tc, config, "defs[1]", "_FILE_OFFSET_BITS=64");
        add_entry(tc, config, "dlclean", "$(RM) 3rdparty/dyncall/dynload/libdynload_s.a");
        add_entry(tc, config, "dll", "lib%s.so");
        add_entry(tc, config, "dllexport", "__attribute__ ((visibility (\"default\")))");
        add_entry(tc, config, "dllib", "3rdparty/dyncall/dynload/libdynload_s.a");
        add_entry(tc, config, "dllimport", "__attribute__ ((visibility (\"default\")))");
        add_entry(tc, config, "dlllocal", "__attribute__ ((visibility (\"hidden\")))");
        add_entry(tc, config, "dlobjects", "");
        add_entry(tc, config, "dlrule", "@:");
        add_entry(tc, config, "exe", "");
        add_entry(tc, config, "formatattribute", "__attribute__((format(X, Y, Z)))");
        add_entry(tc, config, "has_pthread_yield", "0");
        add_entry(tc, config, "havebooltype", "1");
        add_entry(tc, config, "impinst", "");
        add_entry(tc, config, "install", "	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libuv\
	$(CP) 3rdparty/libuv/include/*.h $(DESTDIR)$(PREFIX)/include/libuv\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/armcc\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/gcc\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/hpc\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/ibmc\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/icc\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/loadstore\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/msftc\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/sunc\
	$(CP) 3rdparty/libatomic_ops/src/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/armcc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/armcc\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/gcc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/gcc\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/hpc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/hpc\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/ibmc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/ibmc\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/icc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/icc\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/loadstore/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/loadstore\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/msftc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/msftc\
	$(CP) 3rdparty/libatomic_ops/src/atomic_ops/sysdeps/sunc/*.h $(DESTDIR)$(PREFIX)/include/libatomic_ops/atomic_ops/sysdeps/sunc\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/libtommath\
	$(CP) 3rdparty/libtommath/*.h $(DESTDIR)$(PREFIX)/include/libtommath\
	$(MKPATH) $(DESTDIR)$(PREFIX)/include/dyncall\
	$(CP) 3rdparty/dyncall/dynload/*.h $(DESTDIR)$(PREFIX)/include/dyncall\
	$(CP) 3rdparty/dyncall/dyncall/*.h $(DESTDIR)$(PREFIX)/include/dyncall\
	$(CP) 3rdparty/dyncall/dyncallback/*.h $(DESTDIR)$(PREFIX)/include/dyncall\
");
        add_entry(tc, config, "jit_arch", "MVM_JIT_ARCH_X64");
        add_entry(tc, config, "jit_obj", "$(JIT_OBJECTS) $(JIT_ARCH_X64)");
        add_entry(tc, config, "jit_platform", "MVM_JIT_PLATFORM_POSIX");
        add_entry(tc, config, "laoclean", "cd 3rdparty/libatomic_ops/src && $(MAKE) distclean");
        add_entry(tc, config, "laolib", "3rdparty/libatomic_ops/src/libatomic_ops.a");
        add_entry(tc, config, "laoobjects", "");
        add_entry(tc, config, "laorule", "cd 3rdparty/libatomic_ops && CC='$(CC)' CFLAGS='$(CFLAGS)' ./configure  && cd src && $(MAKE) && cd ..");
        add_entry(tc, config, "ld", "gcc");
        add_entry(tc, config, "lddebugflags", "-g3");
        add_entry(tc, config, "lddir", "-L");
        add_entry(tc, config, "ldflags", " -O3 -DNDEBUG -Wl,-rpath,\"//home/travis/brew/moar-nom/install/lib\"");
        add_entry(tc, config, "ldimp", "");
        add_entry(tc, config, "ldinstflags", "-pg");
        add_entry(tc, config, "ldlibs", "-lm -lpthread -lrt -ldl");
        add_entry(tc, config, "ldmiscflags", "");
        add_entry(tc, config, "ldoptiflags", "-O3 -DNDEBUG");
        add_entry(tc, config, "ldout", "-o ");
        add_entry(tc, config, "ldrpath", "-Wl,-rpath,\"//home/travis/brew/moar-nom/install/lib\"");
        add_entry(tc, config, "ldshared", "-shared -fPIC");
        add_entry(tc, config, "ldsys", "-l%s");
        add_entry(tc, config, "ldusr", "-l%s");
        add_entry(tc, config, "lib", "lib%s.a");
        add_entry(tc, config, "libdir", "/home/travis/brew/moar-nom/install/lib");
        add_entry(tc, config, "mainflags", "-DMVM_SHARED");
        add_entry(tc, config, "mainlibs", "-L. -lmoar");
        add_entry(tc, config, "make", "make");
        add_entry(tc, config, "mastdir", "/home/travis/brew/moar-nom/install/share/nqp/lib/MAST");
        add_entry(tc, config, "mkflags", "");
        add_entry(tc, config, "mknoisy", "ifneq ($(NOISY), 1)\
MSG = @echo\
CMD = @\
NOOUT = > /dev/null\
NOERR = 2> /dev/null\
endif\
");
        add_entry(tc, config, "moar", "libmoar.so");
        add_entry(tc, config, "moardll", "libmoar.so");
        add_entry(tc, config, "moarlib", "libmoar.a");
        add_entry(tc, config, "moarshared", "");
        add_entry(tc, config, "mtclean", "$(RM) 3rdparty/tinymt/libtinymt.a 3rdparty/tinymt/*.o");
        add_entry(tc, config, "mtlib", "3rdparty/tinymt/libtinymt.a");
        add_entry(tc, config, "mtobjects", "3rdparty/tinymt/tinymt64.o");
        add_entry(tc, config, "mtrule", "$(AR) $(ARFLAGS) $@ 3rdparty/tinymt/*.o");
        add_entry(tc, config, "name", "moar");
        add_entry(tc, config, "nativecall_backend", "dyncall");
        add_entry(tc, config, "noreturnattribute", "__attribute__((noreturn))");
        add_entry(tc, config, "noreturnspecifier", "");
        add_entry(tc, config, "nul", "/dev/null");
        add_entry(tc, config, "obj", ".o");
        add_entry(tc, config, "objflags", "-DMVM_BUILD_SHARED -fPIC");
        add_entry(tc, config, "os", "linux");
        add_entry(tc, config, "osname", "linux");
        add_entry(tc, config, "osvers", "3.16.0-4-amd64");
        add_entry(tc, config, "perl", "/usr/bin/perl");
        add_entry(tc, config, "pkgconfig", "/usr/bin/pkg-config");
        add_entry(tc, config, "pkgconfig_works", "1");
        add_entry(tc, config, "platform", "$(PLATFORM_POSIX)");
        add_entry(tc, config, "prefix", "/home/travis/brew/moar-nom/install");
        add_entry(tc, config, "ptr_size", "8");
        add_entry(tc, config, "rm", "rm -f");
        add_entry(tc, config, "sh", "/bin/sh");
        add_entry(tc, config, "shaclean", "$(RM) 3rdparty/sha1/libsha1.a 3rdparty/sha1/*.o");
        add_entry(tc, config, "shaincludedir", "3rdparty/sha1");
        add_entry(tc, config, "shalib", "3rdparty/sha1/libsha1.a");
        add_entry(tc, config, "shaobjects", "3rdparty/sha1/sha1.o");
        add_entry(tc, config, "sharedlib", "");
        add_entry(tc, config, "sharule", "$(AR) $(ARFLAGS) $@ 3rdparty/sha1/*.o");
        add_entry(tc, config, "static_inline", "static __inline__");
        add_entry(tc, config, "staticlib", "");
        add_entry(tc, config, "syslibs[0]", "m");
        add_entry(tc, config, "syslibs[1]", "pthread");
        add_entry(tc, config, "syslibs[2]", "rt");
        add_entry(tc, config, "syslibs[3]", "dl");
        add_entry(tc, config, "thirdpartylibs", "3rdparty/dyncall/dyncall/libdyncall_s.a 3rdparty/dyncall/dyncallback/libdyncallback_s.a 3rdparty/dyncall/dynload/libdynload_s.a 3rdparty/libatomic_ops/src/libatomic_ops.a 3rdparty/tinymt/libtinymt.a 3rdparty/sha1/libsha1.a 3rdparty/libtommath/libtommath.a 3rdparty/libuv/libuv.a");
        add_entry(tc, config, "tomclean", "$(RM) 3rdparty/libtommath/libtommath.a 3rdparty/libtommath/*.o");
        add_entry(tc, config, "tomlib", "3rdparty/libtommath/libtommath.a");
        add_entry(tc, config, "tomobjects", "3rdparty/libtommath/bn_error.o 3rdparty/libtommath/bn_fast_mp_invmod.o 3rdparty/libtommath/bn_fast_mp_montgomery_reduce.o 3rdparty/libtommath/bn_fast_s_mp_mul_digs.o 3rdparty/libtommath/bn_fast_s_mp_mul_high_digs.o 3rdparty/libtommath/bn_fast_s_mp_sqr.o 3rdparty/libtommath/bn_mp_2expt.o 3rdparty/libtommath/bn_mp_abs.o 3rdparty/libtommath/bn_mp_add.o 3rdparty/libtommath/bn_mp_add_d.o 3rdparty/libtommath/bn_mp_addmod.o 3rdparty/libtommath/bn_mp_and.o 3rdparty/libtommath/bn_mp_clamp.o 3rdparty/libtommath/bn_mp_clear.o 3rdparty/libtommath/bn_mp_clear_multi.o 3rdparty/libtommath/bn_mp_cmp.o 3rdparty/libtommath/bn_mp_cmp_d.o 3rdparty/libtommath/bn_mp_cmp_mag.o 3rdparty/libtommath/bn_mp_cnt_lsb.o 3rdparty/libtommath/bn_mp_copy.o 3rdparty/libtommath/bn_mp_count_bits.o 3rdparty/libtommath/bn_mp_div.o 3rdparty/libtommath/bn_mp_div_2.o 3rdparty/libtommath/bn_mp_div_2d.o 3rdparty/libtommath/bn_mp_div_3.o 3rdparty/libtommath/bn_mp_div_d.o 3rdparty/libtommath/bn_mp_dr_is_modulus.o 3rdparty/libtommath/bn_mp_dr_reduce.o 3rdparty/libtommath/bn_mp_dr_setup.o 3rdparty/libtommath/bn_mp_exch.o 3rdparty/libtommath/bn_mp_export.o 3rdparty/libtommath/bn_mp_expt_d.o 3rdparty/libtommath/bn_mp_expt_d_ex.o 3rdparty/libtommath/bn_mp_exptmod.o 3rdparty/libtommath/bn_mp_exptmod_fast.o 3rdparty/libtommath/bn_mp_exteuclid.o 3rdparty/libtommath/bn_mp_fread.o 3rdparty/libtommath/bn_mp_fwrite.o 3rdparty/libtommath/bn_mp_gcd.o 3rdparty/libtommath/bn_mp_get_int.o 3rdparty/libtommath/bn_mp_get_long.o 3rdparty/libtommath/bn_mp_get_long_long.o 3rdparty/libtommath/bn_mp_grow.o 3rdparty/libtommath/bn_mp_import.o 3rdparty/libtommath/bn_mp_init.o 3rdparty/libtommath/bn_mp_init_copy.o 3rdparty/libtommath/bn_mp_init_multi.o 3rdparty/libtommath/bn_mp_init_set.o 3rdparty/libtommath/bn_mp_init_set_int.o 3rdparty/libtommath/bn_mp_init_size.o 3rdparty/libtommath/bn_mp_invmod.o 3rdparty/libtommath/bn_mp_invmod_slow.o 3rdparty/libtommath/bn_mp_is_square.o 3rdparty/libtommath/bn_mp_jacobi.o 3rdparty/libtommath/bn_mp_karatsuba_mul.o 3rdparty/libtommath/bn_mp_karatsuba_sqr.o 3rdparty/libtommath/bn_mp_lcm.o 3rdparty/libtommath/bn_mp_lshd.o 3rdparty/libtommath/bn_mp_mod.o 3rdparty/libtommath/bn_mp_mod_2d.o 3rdparty/libtommath/bn_mp_mod_d.o 3rdparty/libtommath/bn_mp_montgomery_calc_normalization.o 3rdparty/libtommath/bn_mp_montgomery_reduce.o 3rdparty/libtommath/bn_mp_montgomery_setup.o 3rdparty/libtommath/bn_mp_mul.o 3rdparty/libtommath/bn_mp_mul_2.o 3rdparty/libtommath/bn_mp_mul_2d.o 3rdparty/libtommath/bn_mp_mul_d.o 3rdparty/libtommath/bn_mp_mulmod.o 3rdparty/libtommath/bn_mp_n_root.o 3rdparty/libtommath/bn_mp_n_root_ex.o 3rdparty/libtommath/bn_mp_neg.o 3rdparty/libtommath/bn_mp_or.o 3rdparty/libtommath/bn_mp_prime_fermat.o 3rdparty/libtommath/bn_mp_prime_is_divisible.o 3rdparty/libtommath/bn_mp_prime_is_prime.o 3rdparty/libtommath/bn_mp_prime_miller_rabin.o 3rdparty/libtommath/bn_mp_prime_next_prime.o 3rdparty/libtommath/bn_mp_prime_rabin_miller_trials.o 3rdparty/libtommath/bn_mp_prime_random_ex.o 3rdparty/libtommath/bn_mp_radix_size.o 3rdparty/libtommath/bn_mp_radix_smap.o 3rdparty/libtommath/bn_mp_rand.o 3rdparty/libtommath/bn_mp_read_radix.o 3rdparty/libtommath/bn_mp_read_signed_bin.o 3rdparty/libtommath/bn_mp_read_unsigned_bin.o 3rdparty/libtommath/bn_mp_reduce.o 3rdparty/libtommath/bn_mp_reduce_2k.o 3rdparty/libtommath/bn_mp_reduce_2k_l.o 3rdparty/libtommath/bn_mp_reduce_2k_setup.o 3rdparty/libtommath/bn_mp_reduce_2k_setup_l.o 3rdparty/libtommath/bn_mp_reduce_is_2k.o 3rdparty/libtommath/bn_mp_reduce_is_2k_l.o 3rdparty/libtommath/bn_mp_reduce_setup.o 3rdparty/libtommath/bn_mp_rshd.o 3rdparty/libtommath/bn_mp_set.o 3rdparty/libtommath/bn_mp_set_int.o 3rdparty/libtommath/bn_mp_set_long.o 3rdparty/libtommath/bn_mp_set_long_long.o 3rdparty/libtommath/bn_mp_shrink.o 3rdparty/libtommath/bn_mp_signed_bin_size.o 3rdparty/libtommath/bn_mp_sqr.o 3rdparty/libtommath/bn_mp_sqrmod.o 3rdparty/libtommath/bn_mp_sqrt.o 3rdparty/libtommath/bn_mp_sqrtmod_prime.o 3rdparty/libtommath/bn_mp_sub.o 3rdparty/libtommath/bn_mp_sub_d.o 3rdparty/libtommath/bn_mp_submod.o 3rdparty/libtommath/bn_mp_to_signed_bin.o 3rdparty/libtommath/bn_mp_to_signed_bin_n.o 3rdparty/libtommath/bn_mp_to_unsigned_bin.o 3rdparty/libtommath/bn_mp_to_unsigned_bin_n.o 3rdparty/libtommath/bn_mp_toom_mul.o 3rdparty/libtommath/bn_mp_toom_sqr.o 3rdparty/libtommath/bn_mp_toradix.o 3rdparty/libtommath/bn_mp_toradix_n.o 3rdparty/libtommath/bn_mp_unsigned_bin_size.o 3rdparty/libtommath/bn_mp_xor.o 3rdparty/libtommath/bn_mp_zero.o 3rdparty/libtommath/bn_prime_tab.o 3rdparty/libtommath/bn_reverse.o 3rdparty/libtommath/bn_s_mp_add.o 3rdparty/libtommath/bn_s_mp_exptmod.o 3rdparty/libtommath/bn_s_mp_mul_digs.o 3rdparty/libtommath/bn_s_mp_mul_high_digs.o 3rdparty/libtommath/bn_s_mp_sqr.o 3rdparty/libtommath/bn_s_mp_sub.o 3rdparty/libtommath/bncore.o");
        add_entry(tc, config, "tomrule", "$(AR) $(ARFLAGS) $@ 3rdparty/libtommath/*.o");
        add_entry(tc, config, "translate_newline_output", "0");
        add_entry(tc, config, "uvclean", "$(RM) 3rdparty/libuv/libuv.a $(UV_LINUX)");
        add_entry(tc, config, "uvlib", "3rdparty/libuv/libuv.a");
        add_entry(tc, config, "uvobjects", "$(UV_LINUX)");
        add_entry(tc, config, "uvrule", "$(AR) $(ARFLAGS) $@ $(UV_LINUX)");
        add_entry(tc, config, "version", "2017.10");
        add_entry(tc, config, "versionmajor", "2017");
        add_entry(tc, config, "versionminor", "10");
        add_entry(tc, config, "versionpatch", "0");

    });
    tc->instance->cached_backend_config = config;

    return config;
}
