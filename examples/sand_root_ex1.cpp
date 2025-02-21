
/*
 *  This example demonstrates how you can work with the sand procedure in the ROOT framework.
 *  Required library: SpectrumUtils.
 *
 *  The example consists of four parts:
 *  Part 1 - Put the initial spectrum into the 'NS_spectrum_root' structure.
 *  Part 2 - Put both the initial and final calibration coefficients into the 'NS_spectrum_root' structures.
 *  Part 3 - Execute the sand procedure.
 *  Part 4 - Write the result to an output ROOT file.
 *
 *  Some errors may occur if the input files have an incorrect format.
 */

/* In this example, all functions with the prefix "NS_" are in 'SpectrumUtils' library */

/* The next includes are required by R__ADD_INCLUDE_PATH, R__ADD_LIBRARY_PATH and R__LOAD_LIBRARY ROOT macros */
#include "root/Rtypes.h"
#include "root/TROOT.h"

/* We need to add the include path to 'SpectrumUtils' header.
 * In this example, we use R__ADD_INCLUDE_PATH root macro.
 * Set the include path as an argument to R__ADD_INCLUDE_PATH according to your setup. */
R__ADD_INCLUDE_PATH(../include)
// R__ADD_INCLUDE_PATH(../../include)
#include "NS/SpectrumUtils.hxx"

#include "root/TH1.h"
#include "root/TFile.h"

#include <stdlib.h>
#include <stdio.h>

/* We need to load 'SpectrumUtils' library.
 * In this example, we use R_ADD_LIBRARY_PATH ROOT macro for adding library path and
 * R__LOAD_LIBRARY ROOT macro for loading library.
 * Set the library path as an argument to R__ADD_LIBRARY_PATH according to your setup. */
R__ADD_LIBRARY_PATH(../lib)
// R__ADD_LIBRARY_PATH(../../../build/gcc/Release/lib)
R__LOAD_LIBRARY(SpectrumUtils)

#define IFILEROOT "sand_sp1.root"
#define HISTNAME "histSpectrumOld"
#define CCFILE "sand_spectrum_cc"
#define OFILEROOT "sand_output.root"


int sand_root_ex1()
{
	int sts;
	NS_spectrum_root spectrumOldRoot, spectrumNewRoot;
	TFile *stmi, *stmo;

	/* structures must be initialize before using */
	NS_SpectrumRootInit(&spectrumOldRoot);
	NS_SpectrumRootInit(&spectrumNewRoot);

	/*
	 * Part 1: Put the initial spectrum into 'NS_spectrum_root' structure.
	 *         In this example, we read the spectrum from an external root file (see IFILEROOT macro) into 'spectrumOldRoot' structure.
	 */
	stmi = new TFile(IFILEROOT, "read");
	spectrumOldRoot.data = (TH1 *)stmi->Get(HISTNAME);

	/*
	 * Part 2: Put both the initial and final calibration coefficients into 'NS_spectrum_root' structures.
	 *         In this example, we read both the initial and final calibration coefficients from an external file (see CCFILE macro)
	 *         and put them into 'spectrumOldRoot' and 'spectrumNewRoot', respectively.
	 */
	sts = NS_ReadCalibrationCoeffiecients(CCFILE, &spectrumOldRoot.calibcoef, &spectrumNewRoot.calibcoef);
	if (0 != sts) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}

	/*
	 * Part 3: Execute the sand procedure.
	 */
	sts = NS_SpectrumSANDRoot(&spectrumOldRoot, &spectrumNewRoot);
	if (sts < 0) { // some error occured
		goto L_ret_from_routine;
		/* NOTREAHCED */
	}
	/* we opened the 'stmi' file earlier; therefore, the final spectrum in 'spectrumNewRoot' structure (the 'data' field, created as a TH1 class)
	 * is associated with this 'stmi' file. If we want to use our final spectrum after closing the file, we should remove
	 * it from a directory using SetDirectory(0) */
	spectrumNewRoot.data->SetDirectory(0);
	/* at this point we don't need the histogram with the initial spectrum and we can close the file */
	stmi->Close();
	/* set TH1 pointer to NULL after closing the file (to ensure protection from accidental failure) */
	spectrumOldRoot.data = NULL;

	/*
	 * Part 4: Write the result to an output ROOT file.
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
