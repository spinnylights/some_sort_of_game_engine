bin_PROGRAMS = crypt_underworld

crypt_underworld_CXXFLAGS = -I$(top_srcdir)/include $(PTHREAD_CFLAGS)
crypt_underworld_LDADD = $(PTHREAD_LIBS)
crypt_underworld_CXX = $(PTHREAD_CXX)
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
