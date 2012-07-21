# Note: as of v2.3.10 the upstream revision of this file uses pkg-config to
# provide the link configuration for libSDL-ttf. Apparently SDL-ttf 2.0.11
# has a SDL-ttf.pc avaliable. However, I am using SDL-ttf 2.0.9 which does
# not have a SDL-ttf.pc file (Sigh, why not!) so I have reset this file to
# what it was at 2.3.9.Exp - which did not require a SDL-ttf.pc. I have added
# some comments to help with the conditionals. This file should track the
# upstream ASAP - i.e. when SDL-ttf is updated.
ENABLE_SDL ?= $(call bool_not,$(HAVE_WIN32))

ifeq ($(ENABLE_SDL),y)
# {
ifeq ($(TARGET),UNIX)
# { UNIX
OPENGL ?= y
$(eval $(call pkg-config-library,SDL,sdl SDL_image))
# }
else
# { !UNIX
ifeq ($(TARGET),ANDROID)
# { ANDROID
OPENGL = y
SDL_CPPFLAGS :=
SDL_LDLIBS :=
# }
else
# { !ANDROID
OPENGL ?= n
SDL_CPPFLAGS := -I/usr/local/i586-mingw32msvc/include/SDL
SDL_LDLIBS := -L/usr/local/i586-mingw32msvc/lib -lSDL -lSDL_image
# }
endif # $(TARGET), ANDROID
# }
endif # UNIX

SDL_CPPFLAGS += -DENABLE_SDL
ifeq ($(OPENGL),y)
# { OPENGL
SDL_CPPFLAGS += -DENABLE_OPENGL
ifneq ($(TARGET),ANDROID)
# { !ANDROID
ifeq ($(TARGET_IS_DARWIN),y)
# { TARGET_IS_DARWIN
SDL_LDLIBS += -framework OpenGL
# }
else
# { !TARGET_IS_DARWIN
SDL_LDLIBS += -lGL
# }
endif # $(TARGET_IS_DARWIN),y
# }
endif # $(TARGET),ANDROID
# }
else
# { !OPENGL
SDL_LDLIBS += -lSDL_gfx
# }
endif # !OPENGL
ifneq ($(TARGET),ANDROID)
# { !ANDROID
ifeq ($(TARGET_IS_DARWIN),y)
# { TARGET_IS_DARWIN
# the pkg-config file on MacPorts is broken, we must filter out the
# -lSDL flag manually
SDL_LDLIBS := $(filter-out -l%,$(SDL_LDLIBS))
SDL_LDADD = /opt/local/lib/libSDL_ttf.a /opt/local/lib/libfreetype.a
SDL_LDADD += /opt/local/lib/libbz2.a /opt/local/lib/libz.a
SDL_LDADD += /opt/local/lib/libSDL_image.a
# }
else
# { !TARGET_IS_DARWIN
SDL_LDLIBS += -lSDL_ttf
# }
endif # $(TARGET_IS_DARWIN),y
# }
endif # !ANDROID
# }
endif
