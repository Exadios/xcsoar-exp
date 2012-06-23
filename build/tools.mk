#include $(topdir)/build/host.mk

ifeq ($(TARGET),UNIX)

name-to-bin = $(patsubst %,$(TARGET_BIN_DIR)/%$(TARGET_EXEEXT),$(1))

TOOL_PROGRAMS = APAt

TOOLS_LDADD = $(MATH_LIBS)

build-tools: $(TOOL_PROGRAMS)

APAt: eqn-expansion.o APAt.o

APAt.o: test/tools/APAt.cpp test/tools/eqn-expansion.hpp \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/string \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/c++config.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/os_defines.h \
 /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/cpu_defines.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stringfwd.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/char_traits.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_algobase.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/functexcept.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/exception_defines.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/cpp_type_traits.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/ext/type_traits.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/ext/numeric_traits.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_pair.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/move.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/concept_check.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_iterator_base_types.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_iterator_base_funcs.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_iterator.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/debug/debug.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/postypes.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/cwchar \
 /usr/include/wchar.h /usr/include/stdio.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/include/stdarg.h \
 /usr/include/bits/wchar.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/include/stddef.h \
 /usr/include/xlocale.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/allocator.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/c++allocator.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/ext/new_allocator.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/new \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/exception \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/localefwd.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/c++locale.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/clocale \
 /usr/include/locale.h /usr/include/bits/locale.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/iosfwd \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/cctype \
 /usr/include/ctype.h /usr/include/bits/types.h \
 /usr/include/bits/typesizes.h /usr/include/endian.h \
 /usr/include/bits/endian.h /usr/include/bits/byteswap.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/ostream_insert.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/cxxabi_forced.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_function.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/backward/binders.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/range_access.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/basic_string.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/ext/atomicity.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/gthr.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/gthr-default.h \
 /usr/include/pthread.h /usr/include/sched.h /usr/include/time.h \
 /usr/include/bits/sched.h /usr/include/bits/time.h /usr/include/signal.h \
 /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h \
 /usr/include/bits/setjmp.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/atomic_word.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/initializer_list \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/basic_string.tcc \
 src/Math/MatrixT.hpp \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/valarray \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/cmath \
 /usr/include/math.h /usr/include/bits/huge_val.h \
 /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
 /usr/include/bits/inf.h /usr/include/bits/nan.h \
 /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/algorithm \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/utility \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_relops.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_algo.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/cstdlib \
 /usr/include/stdlib.h /usr/include/bits/waitflags.h \
 /usr/include/bits/waitstatus.h /usr/include/sys/types.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/sys/sysmacros.h /usr/include/alloca.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/algorithmfwd.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_heap.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_tempbuf.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_construct.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/valarray_array.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/valarray_array.tcc \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/valarray_before.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/slice_array.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/valarray_after.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/gslice.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/gslice_array.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/mask_array.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/indirect_array.h \
 /usr/include/assert.h src/Math/MatrixT.i.cpp src/Math/VectorT.hpp \
 src/Math/./VectorT.i.cpp

eqn-expansion.o: test/tools/eqn-expansion.cpp \
 test/tools/eqn-expansion.hpp \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/string \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/c++config.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/os_defines.h \
 /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/cpu_defines.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stringfwd.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/char_traits.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_algobase.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/functexcept.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/exception_defines.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/cpp_type_traits.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/ext/type_traits.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/ext/numeric_traits.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_pair.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/move.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/concept_check.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_iterator_base_types.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_iterator_base_funcs.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_iterator.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/debug/debug.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/postypes.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/cwchar \
 /usr/include/wchar.h /usr/include/stdio.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/include/stdarg.h \
 /usr/include/bits/wchar.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/include/stddef.h \
 /usr/include/xlocale.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/allocator.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/c++allocator.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/ext/new_allocator.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/new \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/exception \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/localefwd.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/c++locale.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/clocale \
 /usr/include/locale.h /usr/include/bits/locale.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/iosfwd \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/cctype \
 /usr/include/ctype.h /usr/include/bits/types.h \
 /usr/include/bits/typesizes.h /usr/include/endian.h \
 /usr/include/bits/endian.h /usr/include/bits/byteswap.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/ostream_insert.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/cxxabi_forced.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_function.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/backward/binders.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/range_access.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/basic_string.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/ext/atomicity.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/gthr.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/gthr-default.h \
 /usr/include/pthread.h /usr/include/sched.h /usr/include/time.h \
 /usr/include/bits/sched.h /usr/include/bits/time.h /usr/include/signal.h \
 /usr/include/bits/sigset.h /usr/include/bits/pthreadtypes.h \
 /usr/include/bits/setjmp.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
 /usr/include/bits/confname.h /usr/include/getopt.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/i686-pc-linux-gnu/bits/atomic_word.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/initializer_list \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/basic_string.tcc \
 src/Math/MatrixT.hpp \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/valarray \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/cmath \
 /usr/include/math.h /usr/include/bits/huge_val.h \
 /usr/include/bits/huge_valf.h /usr/include/bits/huge_vall.h \
 /usr/include/bits/inf.h /usr/include/bits/nan.h \
 /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/algorithm \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/utility \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_relops.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_algo.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/cstdlib \
 /usr/include/stdlib.h /usr/include/bits/waitflags.h \
 /usr/include/bits/waitstatus.h /usr/include/sys/types.h \
 /usr/include/sys/select.h /usr/include/bits/select.h \
 /usr/include/sys/sysmacros.h /usr/include/alloca.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/algorithmfwd.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_heap.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_tempbuf.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/stl_construct.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/valarray_array.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/valarray_array.tcc \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/valarray_before.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/slice_array.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/valarray_after.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/gslice.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/gslice_array.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/mask_array.h \
 /usr/local/lib/gcc/i686-pc-linux-gnu/4.6.3/../../../../include/c++/4.6.3/bits/indirect_array.h \
 /usr/include/assert.h src/Math/MatrixT.i.cpp src/Math/VectorT.hpp \
 src/Math/./VectorT.i.cpp

endif
