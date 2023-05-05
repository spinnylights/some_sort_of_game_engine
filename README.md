- [Documentation](#documentation)
- [Building](#building)
  - [Build dependencies](#build-dependencies)
  - [Runtime dependencies](#runtime-dependencies)
  - [If you're working directly from the source repo](#if-youre-working-directly-from-the-source-repo)
  - [Build steps](#build-steps)
  - [Installation](#installation)
    - [Linux](#linux)
    - [Windows and macOS](#windows-and-macos)
- [Contributing](#contributing)
  - [Bug reports and feature requests](#bug-reports-and-feature-requests)
  - [Patches](#patches)


---

## Documentation

API documentation is available
[here](https://milky.flowers/programs/some_sort_of_game_engine/doc/). If
you'd like to generate it yourself, you'll need
[Doxygen](https://www.doxygen.nl/index.html) and
[Graphviz](https://graphviz.org/).

## Building

For binary distribution purposes, the goal is to build the Linux,
Windows, and macOS builds all on Linux, cross-compiling as
needed. With that in mind, these instructions have been tested in
a Linux environment only for the time being. They _should_ work
under Windows using e.g. [MSYS2](https://www.msys2.org/) and
MinGW-w64 (MSVC support is not guaranteed), and under macOS
provided that you have a C++ compiler installed. The Linux
instructions ought to work under most other \*nix environments as
well, like the BSDs, at least I hope (feel free to file a bug
report if not).

### Build dependencies

* [SDL2 development libs + headers](https://www.libsdl.org/download-2.0.php)
* [Vulkan headers](https://www.lunarg.com/vulkan-sdk/)

### Runtime dependencies

* SDL2
* Vulkan-capable graphics drivers
* On macOS,
  [MoltenVK](https://formulae.brew.sh/formula/molten-vk)

### If you're working directly from the source repo

This applies if you've obtained a local copy of the source via
`git clone` or the like, as opposed to working with a release
tarball. It will be easier to build from such a tarball when
there are any, so if you just want to build the package for your
own use I'd recommend waiting for that. Go this route if you want
to help out with development.

If you're on a Mac, make sure you're set up to build GNU-style
packages (e.g. by installing [Homebrew](https://brew.sh/) and
running `brew install autoconf automake libtool`—this package
doesn't need libtool at this point but you might as well have it
if you're getting the other two). If you're on Windows and using
MSYS2 I think you'll already have everything you need in that
regard.

You should also have [git](https://git-scm.com/) installed,
although you probably do already.

Anyway, all you should need to do before proceeding is run
`autoreconf --install` from the project root. If this finishes
without erroring out, continue to the next section. If you get an
error, please file a bug report.

### Build steps

This is for doing an out-of-source build, which keeps things nice
'n' tidy compared to not doing it that way. But you can ignore
the `build` directory and just run `configure` from the project
root if you don't care about that. Anyway.

1. Starting from the root directory of the source repo, `cd
   build`.
1. `../configure --srcdir=..` should be all you need if you're
   doing a native build. If you'd like to build with debug flags,
   you can add `CPPFLAGS="-Og -ggdb"` or the like to that; if
   you're working on a patch and would like to enable warnings,
   you can add `-Wall -Werror` or the like to `CPPFLAGS` as well.
   If you're cross-compiling, pass the `--host` and `--build`
   flags as well (e.g. to cross-compile for Windows from Linux,
   pass `--host=x86_64-w64-mingw32 --build=x86_64-pc-linux-gnu`,
   provided that you have `mingw` installed). You can also set
   the installation root directory from here (`--prefix`), as
   well as many other options. See `../configure --help` and the
   [Autoconf
   manual](https://www.gnu.org/software/autoconf/manual/) for
   more information.
1. `make`. This will run the build. If you'd like to parallelize
   the build process to speed it up, you can pass `-j#` to
   `make`, where `#` is the number of jobs you would like to
   run simultaneously.

### Installation

#### Linux

`make install` after running `make` will work (you will probably
need to do this as root). By default, this will use `/usr/local`
as the installation root directory; you can specify a different
directory by passing `--prefix` to `configure`. Note that if the
game has been released you're almost certainly better off using
your distro's package manager rather than using this command
directly, and if there isn't a package available for your distro
under those circumstances, you might consider making your own.

To uninstall, you can run `make uninstall` from the same place
you ran `make`. Again though, you're probably better off using
your distro's package manager if possible.

#### Windows and macOS

When the game is a bit further along I'll have the build system
generate archives/installers for these platforms that fit their
conventions better. For the time being, you can at least try
running the game from the build directory.

## Contributing

The most obviously helpful thing you can do at the moment is test
the game and file a bug report if you run into any problems.
"Problems" in this case means crashes, build errors, performance
issues, and other obviously unintended behavior—stuff like design
and interface suggestions should probably wait for now. You'll
need to build the game from source for the time being, but I'll
start releasing binaries as soon as the game is actually drawing
a whole scene.

Before communicating with anyone involved on the project, you
might want to look at the document `SOCIAL_EXPECTATIONS.md` in
the project's root directory. This outlines how I expect everyone
to behave here in social terms, naturally. My goal in writing
this is to create a more pleasant, relaxing development
atmosphere—hopefully you will feel that way in practice.

### Bug reports and feature requests

To report a bug, either create a new issue (see [GitHub's docs on
this](https://docs.github.com/en/github/managing-your-work-on-github/creating-an-issue)
for the specifics) or [contact me](https://milky.flowers) if you
specifically want to avoid using GitHub. In any case, give your
report a name that tersely sums up how the program defies your
expectations. In the body of the report, give a more detailed
description of your expectations and how the program defies them,
a step-by-step description of how to trigger this behavior, a
mention of how often it occurs when these steps are followed, and
any helpful details about your environment (OS, hardware, build
tooling, etc.).

To submit a feature request, you should basically follow the same
steps as the bug report. However, you can be more lax about the
technical details in many cases; most features should work on all
the hardware supported by the game. In some cases it may still be
helpful to be very explicit about the steps you are following and
how they deviate from what you expect, like if changing a certain
setting in the preferences doesn't account for a small thing you
think it ought to.

The line between a bug and a missing feature can be hard to
discern as a user, as the distinction is made from the
developer's standpoint (i.e. "Does the existing code not work as
I intend?" vs. "Is there no existing code to do this at all?").
Often it's obvious, but other times not so much. If you have any
doubts, err on the side of the bug report style (i.e being
explicit and information-heavy). Be prepared to learn that what
you thought was a bug is actually a missing feature, though, and
that it may take a lot more time and effort to account for than
you might have guessed.

### Patches

Patches to fix bugs and performance issues are also welcome. A
[pull
request](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/about-pull-requests)
is a fine way to do this (again, let me know if you want to avoid
GitHub—there are other possibilities too). If you need to make
any significant changes to the codebase, I recommend filing a bug
report first and describing your proposed fix so we can make sure
we're on the same page first.

Note that if you're submitting a patch to correct for a
performance issue, it would be really nice (almost mandatory) to
have timing results/memory usage data/whatever's relevant that
demonstrate how much of a difference your patch makes and under
what circumstances. I can take these measurements myself, but of
course my environment is not the same as yours, so I may not see
the same things.
