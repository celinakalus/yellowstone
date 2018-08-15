# Yellowstone

Vulkan-based game engine

## Targets

To produce a binary with additional debug information (default target):

```Bash
$ make debug
```

For release builds with optimizations enabled and no debug information:

```Bash
$ make release
``

For verbose output during compiling and linking (maybe useful for debugging):

```Bash
$ make verbose
```

To clean up the build directory:

```Bash
$ make clean
```

Building is usually done in `./obj`, with the output binary ending up in `./`. 
If you want object files to be generated in a different directory, you can pass 
the variable `OBJDIR`:

```Bash
$ make OBJDIR=$HOME/build
```

Additionally, you can pass the path to the output executable with the variable 
`PROG`:

```Bash
$ make PROG=$HOME/.local/bin/yellowstone
```
