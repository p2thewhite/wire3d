You can build Wire3D for the following target platforms:

### Wii ###
  * Install the [Homebrew Channel](http://wiibrew.org/wiki/Homebrew_setup) on your Wii
  * Install [DevkitPPC](http://wiibrew.org/wiki/DevkitPPC) on your [Windows](http://wiibrew.org/wiki/Devkitppc_setup_%28Windows%29), [Mac OSX](http://wiibrew.org/wiki/Devkitppc_setup_%28Mac_OS_X%29) or [Linux](http://wiibrew.org/wiki/Devkitppc_setup_%28Linux%29) machine
  * Get the latest [release](https://code.google.com/p/wire3d/wiki/Downloads) or current [version](http://code.google.com/p/wire3d/source/checkout) via SVN.
  * Provided Wii project files for Windows:
    * Visual Studio 2005 (2008), 2010, 2012, 2013: `Wire3D_Wii.sln` in its respective `builds` directory
  * Compilation of Wire3D and its Samples:
    * Windows: use Visual Studio or `make`.
    * Mac OSX: use `make`.
    * Linux: use `make`.

**Note**: Use `make WIRE_DEBUG="y"` for debug builds.

<a href='Hidden comment: 
*Note*: If you run a program from Visual Studio, it will use [http://wiibrew.org/wiki/Wiiload Wiiload] (that comes with your DevkitPPC installation) to transfer the program to the Wii via network. Wiiload requires the environment variable WIILOAD to be set to tcp:yourIP, where _yourIP_ is the IP address of your Wii console.
'></a>

### Windows ###
  * Install [DirectX SDK](http://msdn.microsoft.com/en-us/directx/default.aspx)
    * In Visual Studio 2005/2008 add DirectX SDK include and library directories to Visual Studio global directories (Tools -> Options -> Projects and Solutions -> VC++ Directories)
  * Get the latest [release](https://code.google.com/p/wire3d/wiki/Downloads) or current [version](http://code.google.com/p/wire3d/source/checkout) via SVN.
  * Open `Wire3D.sln` from `builds` directory that corresponds with your version of Visual Studio. Supported versions: 2005 (2008), 2010, 2012 or 2013
  * Compile and run any of the samples in the solution

**Note**: If you are using Visual Studio 2008, open the VS2005 solution and let Visual Studio convert the solution and project files for you. After that, it compiles without any warnings.

**Note**: If you are using Visual Studio Express versions before 2012, you will get a warning that solution folders are not supported. While the folders appear to be unloaded, they have just been moved to the root solution space, and compilation succeeds without further warnings.