
/*
 *  This example demonstrates how you can work with the sand procedure in the ROOT framework.
 *  Required libraries: CommonUtils, FuncUtils, SpectrumUtils
 *
 *  The example consists of six parts, covering the full cycle of work, from reading a spectrum from an external text file
 *  to writing the result into an output text file.
 *  Part 1 - Put the initial spectrum into the 'NS_spectrum' structure.
 *  Part 2 - Put both the initial and final calibration coefficients into the 'NS_spectrum_root' structures.
 *  Part 3 - Calculate and put the x-axis energies using the initial calibration coefficients obtained in Part 2 in 'spectrumOldRoot' structure.
 *           (This part can be skipped if you have already done it in another way, for example, by reading from an external file in Part 1).
 *  Part 4 - Create and fill the initial histogram inside the 'spectrumOldRoot' structure.
 *  Part 5 - Execute the sand procedure.
 *  Part 6 - Write the result to an output text file.
 *
 *  Some errors may occur if the input files have an incorrect format.
 */

/* Functions with the prefix "NS_" are in 'SpectrumUtils', 'CommounUtils' and 'FuncUtils' libraries */

/* The next includes are required by R__ADD_INCLUDE_PATH, R__ADD_LIBRARY_PATH and R__LOAD_LIBRARY ROOT macros */
#include "root/Rtypes.h"
#include "root/TROOT.h"

/* We need to add the include path for 'SpectrumUtils', 'CommonUtils', and 'FuncUtils' headers.
 * In this example, we use R__ADD_INCLUDE_PATH ROOT macro.
 * Set the include path as an argument to R__ADD_INCLUDE_PATH according to your setup. */
R__ADD_INCLUDE_PATH(../include)
#include "NS/SpectrumUtils.hxx"
#include "NS/CommonUtils.hxx"
#include "NS/FuncUtils.hxx"

#include "root/TH1.h"
#include "root/TFile.h"

#include <stdlib.h>
#include <stdio.h>

/* We need to load 'SpectrumUtils', 'CommonUtils', 'FuncUtils' libraries.
 * In this example, we use R_ADD_LIBRARY_PATH ROOT macro for adding library path and
 * R__LOAD_LIBRARY ROOT macro for loading library.
 * Set the library path as an argument to R__ADD_LIBRARY_PATH according to your setup. */
R__ADD_LIBRARY_PATH(../lib)
R__LOAD_LIBRARY(CommonUtils)
R__LOAD_LIBRARY(FuncUtils)
R__LOAD_LIBRARY(SpectrumUtils)

#define IFILE "sand_sp1.dat"
#define OFILE "sand_output.dat"
#define IFILEROOT "sand_sp1.root"
#define OFILEROOT "sand_output.root"


int sand_root_ex2()
{
	int sts;
	NS_spectrum_root spectrumOldRoot, spectrumNewRoot;

	/* structures must be initialize before using */
	NS_SpectrumRootInit(&spectrumOldRoot);
	NS_SpectrumRootInit(&spectrumNewRoot);

	{
		NS_spectrum spectrumOld; // the structure for a spectrum with the initial calibration that should be converted into the spectrum with the new calibration
		long opt; // an option for reading/writing a spectrum from/to a file
		int ncolumns; // number of columns in the input file (see README for more details)

		/*
		 * Part 1: Put the initial spectrum into 'NS_spectrum' structure.
		 *         In this example, we read the spectrum from an external text file (see IFILE macro) into 'spectrumOld' structure.
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
		 * Part 2: Put both the initial and final calibration coefficients into 'NS_spectrum_root' structures.
		 *         In this example, we set them directly in the source code.
		 */
		NS_AbsBufPushBack< double >(&spectrumOldRoot.calibcoef, 1);
		NS_AbsBufPushBack< double >(&spectrumOldRoot.calibcoef, 1);
		NS_AbsBufPushBack< double >(&spectrumNewRoot.calibcoef, 2.61);
		NS_AbsBufPushBack< double >(&spectrumNewRoot.calibcoef, 1.12);
		NS_AbsBufPushBack< double >(&spectrumNewRoot.calibcoef, 0.01);

		/*
		 * Part 3: Calculate and put the x-axis energies using the initial calibration coefficients obtained in Part 2 in 'spectrumOldRoot' structure.
		 */
		double *datax, *datay; // arrays for the x-axis (in energy units) and the y-axis of the spectrum, respectively
		double *cc; // an array of the calibration coefficients
		unsigned polPow; // the power of the polynomial of the calibartion curve

		cc = NS_AbsBufPtr< double >(&spectrumOldRoot.calibcoef, 0);
		polPow = NS_AbsBufCount< double >(&spectrumOldRoot.calibcoef) - 1;

		datax = (double *)spectrumOld.data.xv;
		for (unsigned i = 0; i < NS_AbstractTabCount< double >(&spectrumOld.data); i++)
			datax[i] = NS_PolF(cc, polPow, i);
		datay = (double *)spectrumOld.data.yv;

		/* push one more value into the input spectrum, which represents the upper edge of the last bin (it shouldn't be counted in 'spectrumOld.data') */
		NS_AbstractTabPushBack< double >(&spectrumOld.data, NS_PolF(cc, polPow, NS_AbstractTabCount< double >(&spectrumOld.data)), 0);
		spectrumOld.data.count -= sizeof(double);

		/*
		 *  Part 4: Create and fill the initial histogram inside the 'spectrumOldRoot' structure.
		 */
		spectrumOldRoot.data = new TH1D("histSpectrumOld", "initial spectrum", NS_AbstractTabCount< double >(&spectrumOld.data), datax);
		for (unsigned i = 0; i < spectrumOldRoot.data->GetNbinsX(); i++)
			spectrumOldRoot.data->SetBinContent(i + 1, datay[i]);

		/* at this point, the histogram with the initial spectrum is ready, and we can finalize the unnecessary buffer with the initial spectrum */
		NS_SpectrumFini(&spectrumOld);
	}
	{
		/* save the histogram to a ROOT file */
		TFile *stmr = new TFile(IFILEROOT, "recreate");
		spectrumOldRoot.data->Write();
		stmr->Close();
		// spectrumOldRoot.data->DrawClone("hist");	
	}

	/*
	 * Part 5: Execute the sand procedure.
	 */
	sts = NS_SpectrumSANDRoot(&spectrumOldRoot, &spectrumNewRoot);
	if (sts < 0) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}
	spectrumNewRoot.data->DrawClone("hist");

	{
		NS_spectrum spectrumNew; // a structure for the new spectrum with the new calibration
		int ncolumns; // number of columns in the output file (see README for more details)

		/*
		 * Part 6: Write the result into an output text file.
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
