
/*
 *  This example demonstrate how you can work with the sand procedure in C++.
 *  Required libraries: CommonUtils, FuncUtils, SpectrumUtils.
 *  The example consists of five parts, which include the full cycle of work, from reading a spectrum from an extarnal file
 *  to writing the result into an output file.
 *  Part 1 - put initial spectrum into 'NS_spectrum' structure.
 *  Part 2 - put both the initial and final calibration coefficients into the 'NS_spectrum' structures.
 *  Part 3 - calculate and put the energies of the x-axis using the initial calibration coefficients obtained in Part 2.
 *           (This part can be skipped If you have already did it in some other way, for example, by reading from an external file in Part 1).
 *  Part 4 - sand procedure.
 *  Part 5 - write the new spectrum to a file.
 *
 *  Some parts can be modified depending on your case and preference.
 */

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
	NS_spectrum spectrumOld; // a structure for a spectrum with initial calibration that should be converted into a spectrum with new calibration
	NS_spectrum spectrumNew; // a structure for a new spectrum with new calibration
	long opt; // option how to read/write spectrum from/to a file
	int ncolumns; // number of columns in input/output file (see README for more explanation)

	/* structures must be initialize before using */
	NS_SpectrumInit(&spectrumOld);
	NS_SpectrumInit(&spectrumNew);

	/*
	 * Part 1: put initial spectrum into 'NS_spectrum' structure.
	 *         In this example, I read the spectrum from an external text file (see the IFILE macro) into the 'spectrumOld' structure.
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
	 * Part 2: put both the initial and final calibration coefficients into the 'NS_spectrum' structures.
	 *         In this example, I read both the initial and final calibration coefficients from an external file (see the CCFILE macro)
	 *         and put them into 'spectrumOld' and 'spectrumNew', respectively.
	 */
	sts = NS_ReadCalibrationCoeffiecients(CCFILE, &spectrumOld.calibcoef, &spectrumNew.calibcoef);
	if (0 != sts) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}

	/*
	 * Part 3: calculate the energies of the x-axis using the initial calibration coefficients obtained in Part 2.
	 *         Be careful: NS_SpectrumSAND function (see Part 4) doesn't fill x-axis of initial spectrum,
	 *         so you have to do it yourself if you didn't read it from input file early
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
	 * Part 4: sand procedure.
	 */
	sts = NS_SpectrumSAND< double >(&spectrumOld, &spectrumNew);
	if (sts < 0) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}

	/*
	 * Part 5: write new spectrum to a file.
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
