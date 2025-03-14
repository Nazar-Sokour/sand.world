
The SAND project consists of the executable file and shared libraries.
See doc/man for a detailed review of the executable.
See examples/tutorial for a detailed review of library usage with a C/C++ compiler and a ROOT session.

	[For linux users]
	If you want to use the SAND procedure in your own source code, you first need to create the additional library by
sourcing link_spectrumutlis.sh (". bin/link_spectrumutlis.sh"). If you are a ROOT user and want to use
the SAND procedure in a ROOT session, you need to add the option "-rootlibdir=PATH", replacing PATH with your
ROOT library path.

	[For windows users]
	The SAND executable program doesn't know about the path of libraries required by SAND.
There is batch-script 'sand.bat' in bin directory that temporary add required librires path to %PATH%.
So, instead of manually modify %PATH% and execute 'sand.exe', it's easier to execute 'sand.bat'.
All options that you add to 'sand.bat' will pass to 'sand.exe' (see doc/man for detailed review of options).
