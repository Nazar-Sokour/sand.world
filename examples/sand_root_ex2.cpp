
/*
 *  This example demonstrates how you can work with the sand procedure in the ROOT framework.
 *  Required libraries: CommonUtils, FuncUtils, SpectrumUtils
 *  Required root libraries: Core, Hist
 *  The example consists of six parts, which include the full cycle of work, from reading a spectrum from an external file
 *  to writing the result into an output file.
 *  Part 1 - Put the initial spectrum into the 'NS_spectrum' structure.
 *           This part can be skipped If you use another way to obtain a histogram for the sand procedure.
 *  Part 2 - Read the calibration coefficients.
 *           You may read the coefficients in any other way you wish or set them directly in your code.
 *  Part 3 - Calculate the x-axis energies using the initial calibration coefficients obtained in Part 2.
 *           This part can also be skipped if you use another way to obtain histogram for the sand procedure.
 *  Part 4 - Fill the first 'NS_spectrum_root' structure ('spectrumOldRoot' variable in this example)
 *           with the initial histogram (field 'data' in 'NS_spectrum_root') and calibration coefficients (field 'calibcoef' in 'NS_spectrum_root').
 *           Fill the second 'NS_spectrum_root' structure ('spectrumNewRoot' variable) with the final calibration coefficients.
 *  Part 5 - Execute the sand procedure.
 *  Part 6 - Write the result into an output text file.
 *
 *  This is just an example, and some errors may occur if the input files have an incorrect format.
 */

/* These includes are required by R__ADD_INCLUDE_PATH, R__ADD_LIBRARY_PATH and R__LOAD_LIBRARY macros */
#include "root/Rtypes.h"
#include "root/TROOT.h"

/* You need to add include path to 'SpectrumUtils', 'CommonUtils', 'FuncUtils' headers.
 * In this example I used R__ADD_INCLUDE_PATH root macro.
 * Put the include path into argument of R__ADD_INCLUDE_PATH depending on your case. */
R__ADD_INCLUDE_PATH(../../include)

/* These includes are required by functions with prefix "NS_" */
#include "NS/SpectrumUtils.hxx"
#include "NS/CommonUtils.hxx"
#include "NS/FuncUtils.hxx"

#include "root/TH1.h"
#include "root/TFile.h"

#include <stdlib.h>
#include <stdio.h>

/* You need to add library path to 'SpectrumUtils', 'CommonUtils', 'FuncUtils' libraries.
 * In this example I used R__ADD_LIBRARY_PATH root macro.
 * Put the library path into argument of R__ADD_LIBRARY_PATH depending on your case. */
R__ADD_LIBRARY_PATH(../../../build/gcc/Release/lib)
/* Load libraries required by 'NS_SpectrumSANDRoot' function */
R__LOAD_LIBRARY(CommonUtils)
R__LOAD_LIBRARY(FuncUtils)
R__LOAD_LIBRARY(SpectrumUtils)

#define IFILE "sand_sp1.dat"
#define OFILE "sand_output.dat"
#define CCFILE "sand_spectrum_cc"
#define IFILEROOT "sand_sp1.root"
#define OFILEROOT "sand_output.root"


int main()
{
	int sts;
	NS_spectrum_root spectrumOldRoot, spectrumNewRoot;

	NS_SpectrumRootInit(&spectrumOldRoot);
	NS_SpectrumRootInit(&spectrumNewRoot);

	{
		NS_spectrum spectrumOld; // a structure for a spectrum with initial calibration that should be converted into a spectrum with new calibration
		long opt; // option how to read/write spectrum from/to a file
		int ncolumns; // number of columns in input file (see README for more explanation)

		/*
		 * Part 1: put initial spectrum into 'NS_spectrum' structure.
		 *         In this example, I read the spectrum from an external text file (see the IFILE macro) into the 'spectrumOld' structure.
		 */
		/* structure must be initialize before using */
		NS_SpectrumInit(&spectrumOld);

		ncolumns = 1;
		/* two scan methods are available:
		 * NS_FRA_OVERWRITE - for overwriting spectrum
		 * NS_FRA_UPDATE - for filling current spectrum with one more spectrum from IFILE */
		opt = NS_FRA_SET_SMTD(NS_FRA_OVERWRITE) | ncolumns;
		sts = NS_ReadAbstractFile< double >(IFILE, &spectrumOld.data, opt);
		if (0 != sts) { // some error occured
			NS_SpectrumFini(&spectrumOld);
			goto L_ret_from_routine;
			/* NOTREAHCED */
		}

		/*
		 * Part 2: put both the initial and final calibration coefficients into the 'NS_spectrum_root' structures.
		 *         In this example, I read both the initial and final calibration coefficients from an external file (see the CCFILE macro)
		 *         and put them into 'spectrumOldRoot' and 'spectrumNewRoot', respectively.
		 */
		sts = NS_ReadCalibrationCoeffiecients(CCFILE, &spectrumOldRoot.calibcoef, &spectrumNewRoot.calibcoef);
		if (0 != sts) { // some error occured
			goto L_ret_from_routine;
			/* NOTREAHCED */
		}

		/*
		 * Part 3: calculate the energies of the x-axis using the initial calibration coefficients obtained in Part 2.
		 */
		double *datax, *datay; // array for the x-saxis (in energy units) and the y-axis of the spectrum, respectively
		double *cc; // array of the calibration coefficients
		unsigned polPow; // power of polynom of calibartion curve

		cc = NS_AbsBufPtr< double >(&spectrumOldRoot.calibcoef, 0);
		polPow = NS_AbsBufCount< double >(&spectrumOldRoot.calibcoef) - 1;

		datax = (double *)spectrumOld.data.xv;
		for (unsigned i = 0; i < NS_AbstractTabCount< double >(&spectrumOld.data); i++)
			datax[i] = NS_PolF(cc, polPow, i);
		datay = (double *)spectrumOld.data.yv;

		/* push one more value into input spectrum which is the upper edge of the last bin (shouldn't be counted in 'spectrumOld.data') */
		NS_AbstractTabPushBack< double >(&spectrumOld.data, NS_PolF(cc, polPow, NS_AbstractTabCount< double >(&spectrumOld.data)), 0);
		spectrumOld.data.count -= sizeof(double);

		/*
		 * Part 4: create and fill TH1 histogram in field 'data' of 'spectrumOldRoot' structure.
		 */
		spectrumOldRoot.data = new TH1D("histSpectrumOld", "initial spectrum", NS_AbstractTabCount< double >(&spectrumOld.data), datax);
		for (unsigned i = 0; i < spectrumOldRoot.data->GetNbinsX(); i++)
			spectrumOldRoot.data->SetBinContent(i + 1, datay[i]);

		/* at this point histogram with initial spectrum is ready and I can finalize unnecessary buffer with initial spectrum */
		NS_SpectrumFini(&spectrumOld);
	}
	{
		/* save histogram to root file */
		TFile *stmr = new TFile(IFILEROOT, "recreate");
		spectrumOldRoot.data->Write();
		stmr->Close();
		// spectrumOldRoot.data->DrawClone("hist");	
	}

	/*
	 * Part 5: sand procedure.
	 */
	sts = NS_SpectrumSANDRoot(&spectrumOldRoot, &spectrumNewRoot);
	if (sts < 0) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}
	spectrumNewRoot.data->DrawClone("hist");

	{
		NS_spectrum spectrumNew; // structure for new spectrum with new calibration
		int ncolumns; // number of columns in output file (see README for more explanation)

		/*
		 * Part 6: write the result into output text file.
		 */
		NS_SpectrumInit(&spectrumNew);
		ncolumns = 2;
		for (unsigned i = 0; i < spectrumNewRoot.data->GetNbinsX(); i++) {
			double xEnergy = spectrumNewRoot.data->GetBinLowEdge(i + 1);
			double yCounts = spectrumNewRoot.data->GetBinContent(i + 1);
			NS_AbstractTabPushBack< double >(&spectrumNew.data, xEnergy, yCounts);
		}
		sts = NS_WriteAbstractFile< double >(OFILE, &spectrumNew.data, ncolumns);
		NS_SpectrumFini(&spectrumNew);
	}

 L_ret_from_routine:
	/* it's better to finalize structures in the end of using. To free dynamic memory */
	NS_SpectrumRootFini(&spectrumNewRoot);
	NS_SpectrumRootFini(&spectrumOldRoot);
	return (sts);
}
