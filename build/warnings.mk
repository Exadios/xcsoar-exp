WARNINGS = -Wall -Wextra
WARNINGS += -Wwrite-strings -Wcast-qual -Wpointer-arith -Wsign-compare
WARNINGS += -Wundef
WARNINGS += -Wmissing-declarations
WARNINGS += -Wredundant-decls

ifeq ($(HAVE_CE),y)
WARNINGS += -Wno-cast-function-type
endif

CXXFLAGS += $(WARNINGS)
CXXFLAGS += -Wmissing-noreturn

# disable some warnings, we're not ready for them yet
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += -Wno-missing-field-initializers 
CXXFLAGS += -Wcast-align

# plain C warnings

CFLAGS += $(WARNINGS)
CFLAGS += -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += -Wnested-externs

# make warnings fatal (for perfectionists)

ifneq ($(TARGET),CYGWIN)
WERROR ?= $(DEBUG)
endif

ifeq ($(WERROR),y)
CXXFLAGS += -Werror
CFLAGS += -Werror
# clang has problems with some command line arguments for TARGET=ANDROID7
# Exactly why is not clear. Because, in the debug build all warnings are
# treated as errors the DEBUG=y build fails. Suppress these errors.
ifeq ($(CLANG),y)
CXXFLAGS += -Wno-error=unused-command-line-argument
CFLAGS   += -Wno-error=unused-command-line-argument
endif # $(CLANG)
endif

#CXXFLAGS += -pedantic
#CXXFLAGS += -pedantic-errors

# -Wdisabled-optimization
# -Wunused -Wshadow -Wunreachable-code
# -Wfloat-equal
