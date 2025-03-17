
The SAND project consists of an executable file and shared libraries.
See doc/man for a detailed review of the executable.
See examples/tutorial for a detailed review of library usage with a C/C++ compiler and a ROOT session.

		[For linux users]
	If you want to use the SAND procedure in your own source code, you first need to create the additional library by
sourcing link_spectrumutlis.sh (". bin/link_spectrumutlis.sh"). If you are a ROOT user and want to use
the SAND procedure in a ROOT session, you need to add the option "-rootlibdir=PATH", replacing "PATH" with your
ROOT library path.

		[For windows users]
	The SAND executable program requires a path to:
		1. Its own libraries (located in the "lib" directory)
		2. ROOT directories (if you are a ROOT user)

	Solution:
		1. Use the "sand.bat" batch script from the "bin" directory instead of manually modifying %PATH% and executing "sand.exe".
		   This script temporarily adds the required library path to %PATH%.
		   Any options you pass to "sand.bat" will be forwarded to "sand.exe" (see doc/man for a detailed review of options).

		   The example command in command line (from the SAND root directory):

		   sand.bat examples/sand_sp1.dat -o examples/sand_output.dat -c examples/sand_spectrum_cc.dat

		2. If your ROOT libraries are stored in non-standard directories, run "thisroot.bat" first.

		   The example command in command line (from the SAND root directory):

		   1) "YOUR\PATH\TO\ROOT\FRAMEWORK"\bin\thisroot.bat (run this once per new command line session)
		   2) sand.bat examples/sand_sp1.dat -o examples/sand_output.dat -c examples/sand_spectrum_cc.dat
