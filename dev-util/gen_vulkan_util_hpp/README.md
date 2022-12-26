# `gen_vulkan_util_hpp`

This script can generate the header found at
`include/vulkan_util.hpp`, which provides types that wrap
Vulkan's enums and bitmasks with names I prefer, as well as
providing string conversion functions for them that can be used
with the logger.

It assumes that you're in a \*nix environment. If you're in
Windows and need to use it, I suggest running it from within an
environment like [MSYS2](https://www.msys2.org/) or
[Cygwin](https://www.cygwin.com/). It requires a working local
[Ruby](https://www.ruby-lang.org/) installation with the
[Nokogiri](https://nokogiri.org/) gem installed.

`gen_vulkan_util_hpp` uses the [Vulkan API
Registry](https://github.com/KhronosGroup/Vulkan-Docs/tree/main/xml),
an XML file that Vulkan also uses to generate its headers. It
assumes this file is available locally, and has a default path to
it, although you can specify another (pass `-h` for an overview
of the CLI).

With no arguments, it generates the header and pipes it into your
preferred pager. You can also have it write the generated header
to a file (`vulkan_util.hpp` by default), or send it to stdout.

If you need to modify the script, you can have it test the header
it generates against an existing header (`vulkan_util.hpp` by
default) and show a diff if the test fails. This makes it easy to
"refactor" the script without worrying about messing up the
header accidentally, and also allows you to easily see the
changes you're planning to make to the header if you do intend to
modify it.
