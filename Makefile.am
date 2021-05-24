bin_PROGRAMS = crypt_underworld

crypt_underworld_CPPFLAGS = -I$(top_srcdir)/include -Wall -Werror
crypt_underworld_LDADD = -lvulkan -lpthread $(crypt_underworld_LIBS)
crypt_underworld_SOURCES = \
	src/main.cpp \
	src/sdl.cpp \
	src/vulkan.cpp \
	src/instance.cpp \
	src/phys_devices.cpp \
	src/surface.cpp \
	src/queue_family.cpp \
	src/logi_device.cpp \
	src/log.cpp \
	src/engine.cpp
