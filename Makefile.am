bin_PROGRAMS = crypt_underworld

crypt_underworld_CXXFLAGS = -I$(top_srcdir)/include $(PTHREAD_CFLAGS) $(SDL_CFLAGS)
crypt_underworld_LDADD = $(PTHREAD_LIBS) $(SDL_LIBS)
crypt_underworld_CXX = $(PTHREAD_CXX)
crypt_underworld_SOURCES = \
	src/main.cpp \
	src/sdl.cpp \
	src/vulkan.cpp \
	src/instance.cpp \
	src/phys_devices.cpp \
	src/phys_device.cpp \
	src/surface.cpp \
	src/queue_family.cpp \
	src/device.cpp \
	src/log.cpp \
	src/swapchain.cpp \
	src/cli.cpp \
	src/debug_msgr.cpp \
	src/image.cpp \
	src/image_view.cpp \
	src/image_format.cpp \
	src/bin_file.cpp \
	src/shader_module.cpp \
	src/descriptor_set_layout_binding.cpp \
	src/descriptor_set_layout.cpp \
	src/pipeline_layout.cpp \
	src/compute_pipeline.cpp \
	src/descriptor_pool.cpp \
	src/command_pool.cpp \
	src/command_buffer.cpp \
	src/semaphore.cpp \
	src/timeline_semaphore.cpp \
	src/binary_semaphore.cpp \
	src/engine.cpp
