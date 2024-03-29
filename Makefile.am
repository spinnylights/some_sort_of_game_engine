bin_PROGRAMS = crypt_underworld cu_tests vulkan_integ

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
	src/bin_data.cpp \
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
	src/fence.cpp \
	src/heap.cpp \
	src/engine.cpp

cu_tests_CXXFLAGS = $(crypt_underworld_CXXFLAGS) -I/usr/include/doctest/ -I/usr/local/include/doctest
cu_tests_LDADD = $(crypt_underworld_LDADD)
cu_tests_CXX = $(cu_tests_CXX)
cu_tests_SOURCES = \
	src/bin_data.cpp \
	test/bin_data.cpp

vulkan_integ_CXXFLAGS = $(crypt_underworld_CXXFLAGS) -I/usr/include/doctest/ -I/usr/local/include/doctest
vulkan_integ_LDADD = $(PTHREAD_LIBS) $(SDL_LIBS)
vulkan_integ_CXX = $(PTHREAD_CXX)
# TODO: Instead of doing this, compile a proper library
vulkan_integ_SOURCES = \
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
	src/bin_data.cpp \
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
	src/fence.cpp \
	src/heap.cpp \
	src/engine.cpp \
	test/vulkan_integ.cpp

examples_dir = $(top_srcdir)/examples
circ_dir = $(examples_dir)/circ
shaders_out_dir = shaders

BUILT_SOURCES := shaders/comp.spv

$(shaders_out_dir)/comp.spv : $(circ_dir)/main.comp
	mkdir -p $(shaders_out_dir)
	glslangValidator --target-env vulkan1.3 $^ -o $@
