
NAME
		sand - a program that converts a spectrum with its own calibration into another spectrum with a different calibration.

SYNOPSYS
	UNIX-style usage
		sand [FILE] -o [FILE] -c [FILE] ...

	GNU-style usage
		sand [FILE] --opath [FILE] --ccpath [FILE] ...

DESCRIPTION
		sand - a program that converts a spectrum with its own (initial) calibration into another spectrum with a different (final) calibration.
		The convertion is performed by proportionally splitting the initial bin contents according to the energy bin edges in the final calibration.
		The program requires:
			1. a spectrum. The spectrum should be a text file (input file) which may have any extention (.dat, .txt, ...) or no extention at all.
			2. initial and final calibration coefficients, which can be read from a text file (additional file),
			   passed as program arguments or set interactively during program's execution in "interactive" mode.

		Program execution modes:
			1. standard mode. The program uses predefined values for required variables if they are not set via program arguments or environment variables. 
			2. interactive method (use the "-u" option). In this mode, the user can manually enter values
			   if they are not set via program arguments or environment variables. The user can either input value or choose the default one.
		
		The program retrieves required variable values in the following order:
			1. program arguments
			2. environment variables
			3. default values

		The user can specify the path of any file (input, output, or additional) directly (absolute or relative path) or specify the file name and directory separately.
		If both path and name are provided, the program prioritizes the path.
		If only the file name is provided, the program will search for the file in:
			1. the current working directory
			2. the default directory

	Environment variables:
		SAND_INPUT_PATH:
			input file path

		SAND_INPUT_NAME:
			input file name

		SAND_OUTPUT_PATH:
			output file path

		SAND_OUTPUT_NAME:
			output file name

		SAND_CC_PATH:
			additional file path

		SAND_CC_NAME:
			additional file name

		SAND_DIR:
			directory for input/output/additional files

	Default values:
		directory:
			“${HOME}/.sand” (non-Windows) або “%USERPROFILE%/.sand” (Windows)

		input file name:
			“spectrum_input”

		output file name:
			“spectrum_output”

		additional file name:
			“spectrum_cc”

		number of columns in an input file:
			1

		number of columns in an output file:
			1

	Option styles
		In UNIX or short-option style, each option letter is prefixed with a single dash. If an option takes argument, the argument follows it
		as a separate command line word. Any number of options not taking arguments can be clustered together after a single dash, e.g. -uv.

		The example command in short-option style:

		sand -usv input.txt -h cc.txt
		or
		sand -u -s -v input.txt -h cc.txt

		In GNU or long-option style, each option begins with two dashes and has a meaningful name, consisting of lowercase letters.
		Arguments to long options are supplied as a separate command line word.

		The example command in long-option style:

		sand --ifile input.txt --opath ~/output.txt --ccfile cc.txt --dir .

		Short and long options can be mixed. Reusing an option that requires an argument will cause the program to exit with an "Option reuse" error.

OPTIONS
	Without arguments:
		-u:
			interactive (user) mode.
		-s:
			prevents the program from overwriting the output file if a file with the same name already exists (not implemented yet).
		-v:
			verbose mode.

	With arguments
		-i, --ifile [FILE]:
			sets the input file name.

		-o, --opath [FILE]:
			sets the output file path.

		-f, --ofile [FILE]:
			sets the output file name.

		-c, --ccpath [FILE]:
			sets the additional file path.

		-h, --ccfile [FILE]:
			sets the additional file name.

		-d, --dir [DIRECTORY]:
			sets the directory for input/output/additional files.

		-n, --icols [NUMBER]:
			sets the number of columns in an input file.

		-k, --ocols [NUMBER]:
			sets the number of columns in an output file.

		-a, --cci [NUMBER]:
			sets the initial calibration coefficients.

		-b, --cco [NUMBER]:
			sets the final calibration coefficients.

RETURN VALUES
		sand exit code indicates whetever it was able to successfully perform the required operation, and if not, what kind of error occured.

		-1   An error occured in the standard C library with an 'errno' error code
		-2   Invalid argument
		-3   Reuse of an option with argument
		-4   Argument not provided
		-5   Invalid input
		-6   Input/output error

BUG REPORTS
		Report bugs to <nazar19681980@gmail.com>.
