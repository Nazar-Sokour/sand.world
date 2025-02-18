
/*
 *  This example demonstrates how you can work with the sand procedure in the ROOT framework.
 *  Required libraries: CommonUtils, FuncUtils, SpectrumUtils
 *  Required root libraries: Core, Hist
 *  The example consists of six parts, which include the full cycle of work, from reading a spectrum from an external file
 *  to writing the result into an output file.
 *  Part 1 - Put the initial spectrum into the 'NS_spectrum_root' structure.
 *  Part 2 - Read both the initial and final calibration coefficients into the 'NS_spectrum_root' structure.
 *  Part 3 - Execute the sand procedure.
 *  Part 4 - Write the result into an output text file.
 *
 *  This is just an example and some errors may occure in a case of input files with wrong format.
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

#include "root/TH1.h"
#include "root/TFile.h"

#include <stdlib.h>
#include <stdio.h>

/* You need to add library path to 'SpectrumUtils', 'CommonUtils', 'FuncUtils' libraries.
 * In this example I used R__ADD_LIBRARY_PATH root macro.
 * Put the library path into argument of R__ADD_LIBRARY_PATH depending on your case. */
R__ADD_LIBRARY_PATH(/usr/local/lib/NSutils)
/* Load libraries required by 'NS_SpectrumSANDRoot' function */
// R__LOAD_LIBRARY(CommonUtils)
// R__LOAD_LIBRARY(FuncUtils)
R__LOAD_LIBRARY(SpectrumUtils)

#define CCFILE "sand_spectrum_cc"
#define IFILEROOT "sand_sp1.root"
#define HISTNAME "histSpectrumOld"
#define OFILEROOT "sand_output.root"


int main()
{
	int sts;
	NS_spectrum_root spectrumOldRoot, spectrumNewRoot;
	TFile *stmi, *stmo;

	/* structures must be initialize before using */
	NS_SpectrumRootInit(&spectrumOldRoot);
	NS_SpectrumRootInit(&spectrumNewRoot);

	/*
	 * Part 1: put access to the initial spectrum into the 'NS_spectrum_root' structure.
	 *         In this example, I read the spectrum from an external root file (see the IFILEROOT macro) into the 'spectrumOldRoot' structure.
	 */
	stmi = new TFile(IFILEROOT, "read");
	spectrumOldRoot.data = (TH1 *)stmi->Get(HISTNAME);

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
	 * Part 3: sand procedure.
	 */
	sts = NS_SpectrumSANDRoot(&spectrumOldRoot, &spectrumNewRoot);
	if (sts < 0) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}
	/* we opened the 'stmi' file earlier; therefore, the final spectrum in the 'spectrumNewRoot' structure (the 'data' field, created as a TH1 class)
	 * is associated with this 'stmi' file. If we want to use our final spectrum after closing the file, we should remove
	 * it from a directory using SetDirectory(0) */
	spectrumNewRoot.data->SetDirectory(0);
	/* at this point we don't need the histogram with the initial spectrum and we can close the file */
	stmi->Close();
	/* set TH1 pointer to NULL after closing the file (to ensure protection from accidental failure) */
	spectrumOldRoot.data = NULL;

	/*
	 * Part 4: write the result into output root file.
	 */
	stmo = new TFile(OFILEROOT, "recreate");
	/* associate the histogram in 'spectrumNewRoot' with the newly created file */
	spectrumNewRoot.data->SetDirectory(gDirectory);
	spectrumNewRoot.data->Write();
	stmo->Close();

 L_ret_from_routine:
	/* it's better to finalize structures in the end of using. To free dynamic memory */
	NS_SpectrumRootFini(&spectrumNewRoot);
	NS_SpectrumRootFini(&spectrumOldRoot);
	return (sts);
}
