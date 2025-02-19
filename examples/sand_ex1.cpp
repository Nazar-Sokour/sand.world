
/*
 *  This example demonstrates how you can work with the sand procedure in C++.
 *  Required libraries: CommonUtils, FuncUtils, SpectrumUtils, m.
 *  Example of compile command by gcc: gcc sand_ex1.cpp -I"../include" -L"../lib" -lCommonUtils -lFuncUtils -lSpectrumUtils -lm

 *  The example consists of five parts:
 *  Part 1 - Put the initial spectrum into the 'NS_spectrum' structure.
 *  Part 2 - Put both the initial and final calibration coefficients into the 'NS_spectrum' structures.
 *  Part 3 - Calculate and put the x-axis energies using the initial calibration coefficients obtained in Part 2 in the 'spectrumOld' structure.
 *           (This part can be skipped if you have already done it in another way, for example, by reading from an external file in Part 1).
 *  Part 4 - Execute the sand procedure.
 *  Part 5 - Write the result to an output text file.
 *
 *  Some errors may occur if the input files have an incorrect format.
 */

/* Functions with the prefix "NS_" are in 'SpectrumUtils', 'CommounUtils' and 'FuncUtils' libraries */

#include "NS/SpectrumUtils.hxx"
#include "NS/CommonUtils.hxx"
#include "NS/FuncUtils.hxx"

#include <stdlib.h>
#include <stdio.h>

#define IFILE "sand_sp1.dat"
#define OFILE "output.dat"
#define CCFILE "sand_spectrum_cc"


int main(int argc, char *argv[])
{
	int sts;
	NS_spectrum spectrumOld; // the structure for a spectrum with the initial calibration that should be converted into the spectrum with the new calibration
	NS_spectrum spectrumNew; // the structure for a new spectrum with the new calibration
	long opt; // an option for reading/writing a spectrum from/to a file
	int ncolumns; // number of columns in the input/output file (see README for more details)

	/* structures must be initialize before using */
	NS_SpectrumInit(&spectrumOld);
	NS_SpectrumInit(&spectrumNew);

	/*
	 * Part 1: Put the initial spectrum into the 'NS_spectrum' structure.
	 *         In this example, we read the spectrum from an external text file (see the IFILE macro) into the 'spectrumOld' structure.
	 */
	ncolumns = 1;
	/* two scan methods are available:
	 * NS_FRA_OVERWRITE - for overwriting spectrum
	 * NS_FRA_UPDATE - for filling current spectrum with one more spectrum from IFILE */
	opt = NS_FRA_SET_SMTD(NS_FRA_OVERWRITE) | ncolumns; // options for reading
	sts = NS_ReadAbstractFile< double >(IFILE, &spectrumOld.data, opt);
	if (0 != sts) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}

	/*
	 * Part 2: Put both the initial and final calibration coefficients into the 'NS_spectrum' structures.
	 *         In this example, we read both the initial and final calibration coefficients from an external file (see the CCFILE macro)
	 *         and put them into 'spectrumOld' and 'spectrumNew', respectively.
	 *         You may put the coefficients in any other way you wish, for example, set them directly in your code.
	 */
	sts = NS_ReadCalibrationCoeffiecients(CCFILE, &spectrumOld.calibcoef, &spectrumNew.calibcoef);
	if (0 != sts) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}

	/*
	 * Part 3: Calculate and put the energies of the x-axis using the initial calibration coefficients obtained in Part 2 into 'spectrumOld' strucutre.
	 *         Be careful: NS_SpectrumSAND function (see Part 4) doesn't fill the x-axis of the initial spectrum.
	 */
	{
		double *cc; // array of calibration coefficients
		unsigned polPow; // power of polynom of calibartion curve

		cc = NS_AbsBufPtr< double >(&spectrumOld.calibcoef, 0);
		polPow = NS_AbsBufCount< double >(&spectrumOld.calibcoef) - 1;
		for (unsigned i = 0; i < NS_AbstractTabCount< double >(&spectrumOld.data); i++)
			*NS_AbstractTabPtrX< double >(&spectrumOld.data, i) = NS_PolF(cc, polPow, i);
	}

	/*
	 * Part 4: Execute the sand procedure.
	 */
	sts = NS_SpectrumSAND< double >(&spectrumOld, &spectrumNew);
	if (sts < 0) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}

	/*
	 * Part 5: Write the result to an output text file.
	 */
	ncolumns = 2;
	sts = NS_WriteAbstractFile< double >(OFILE, &spectrumNew.data, ncolumns);
	if (sts < 0) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}

 L_ret_from_routine:
	/* it's better to finalize structures in the end of using. To free dynamic memory */
	NS_SpectrumFini(&spectrumNew);
	NS_SpectrumFini(&spectrumOld);
	return (sts);
}
