nxdk_nv2a_register_finder
====

Attempts to diff PGRAPH registers after sending pushbuffer commands.

## Usage

Tests will be executed automatically and results written to the Xbox harddrive
on completion.

## Build prerequisites

This test suite requires some modifications to the pbkit used by the nxdk in order to operate.

To facilitate this, the nxdk is included as a submodule of this project, referencing the
`nxdk_pgraph_tester` branch from https://github.com/abaire/nxdk.

This project should be cloned with the `--recursive` flag to pull the submodules and their submodules,
after the fact this can be achieved via `git submodule update --init --recursive`.

As of July 2023, the nxdk's CMake implementation requires bootstrapping before it may be used. To facilitate this, run
the `prewarm-nxdk.sh` script from this project's root directory. It will navigate into the `nxdk` subdir and build all
of the sample projects, triggering the creation of the `nxdk` libraries needed for the toolchain and for this project.

### Building with CLion

The CMake target can be configured to use the toolchain from the nxdk:

* CMake options

  `-DCMAKE_TOOLCHAIN_FILE=<absolute_path_to_this_project>/third_party/nxdk/share/toolchain-nxdk.cmake`

* Environment

  `NXDK_DIR=<absolute_path_to_this_project>/third_party/nxdk`

On macOS you may also have to modify `PATH` in the `Environment` section such that a homebrew version of LLVM
is preferred over Xcode's (to supply `dlltool`).

#### Example settings

Assuming that this project has been checked out at `/development/nxdk_nv2a_register_finder`:

* CMake options: `-DCMAKE_TOOLCHAIN_FILE=/development/nxdk_nv2a_register_finder/third_party/nxdk/share/toolchain-nxdk.cmake`
* Environment: `NXDK_DIR=/development/nxdk_nv2a_register_finder/third_party/nxdk`

## Running with CLion

## Deploying with [xbdm_gdb_bridge](https://github.com/abaire/xbdm_gdb_bridge)

To create a launch configuration that deploys the devhost to an XBDM-enabled XBOX (devkit) with debugging enabled:

1. Upload the full set of resource files to the XBOX

   This can be done with [xbdm_gdb_bridge binary](https://github.com/abaire/xbdm_gdb_bridge) and a `CMake Application`
   target.
    1. Create a new `CMake Application` target
    2. Set `Target` to `nxdk_nv2a_register_finder_xiso`
    3. Set `Executable` to the full path to the [xbdm_gdb_bridge binary](https://github.com/abaire/xbdm_gdb_bridge)
       binary
    4. Set `Program arguments`
       to
       `<YOUR_XBOX_IP> -v3 -- mkdir e:\$CMakeCurrentTargetName$ && %syncdir $CMakeCurrentBuildDir$/xbe/xbe_file e:\$CMakeCurrentTargetName$ -f`
    5. Run the target. You will need to do this any time the resources are changed. The XBE can be uploaded as part of
       the debug step.

1. Create a new `Embedded GDB Server` run config.
    1. Set the "Target" to `nxdk_nv2a_register_finder`
    1. Set the "Executable binary" to `nxdk_nv2a_register_finder`
    1. Set "Download executable" to `Never`
    1. Set "'target remote' args" to `localhost:1999`
    1. Set "GDB Server" to the full path to the [xbdm_gdb_bridge binary](https://github.com/abaire/xbdm_gdb_bridge)
       binary
    1. Set "GDB Server args"
       to `<YOUR_XBOX_IP> -s -- gdb :1999 e:\$CMakeCurrentTargetName$`
    1. Under "Advanced GDB Server Options"
        1. Set "Reset command" to `Never`
    1. Add a "Before launch" > "Run External tool" to copy the default.xbe to the XBOX:
        1. Set "Program" to the full path to the [xbdm_gdb_bridge binary](https://github.com/abaire/xbdm_gdb_bridge)
           binary
        2. Set "Arguments"
           to
           `<YOUR_XBOX_IP> -- mkdir e:\$CMakeCurrentTargetName$ && putfile $CMakeCurrentBuildDir$/xbe/xbe_file/default.xbe e:\$CMakeCurrentTargetName$ -f`
        3. Uncheck "Synchronize files after execution"
