/*!
* \file
* This file contains commonly used statistics functions that are used
* in RevBayes. The probability density (pdf), log of the probability
* density (lnPdf), cumulative probability (cdf), and quantiles of
* common probability distributions.
*
* \brief Namespace containing statistical functions
*
* (c) Copyright 2009- under GPL version 3
* \date Last modified: $Date: 2009-11-24 18:02:14 -0800 (Tue, 24 Nov 2009) $
 * \author Sebastian Hoehna (Department of Mathematics, Stockholm University)
 * \author John Huelsenbeck (Department of Integrative Biology, University of California, Berkeley)
 * \author Fredrik Ronquist (Department of Entomology, Swedish Museum of Natural History)
* \license GPL version 3
*
* $Id: RbStatistics.h 100 2009-11-25 02:02:14Z jhuelsenbeck $
*/

#ifndef RbConstants_H
#define RbConstants_H

#include <cfloat>

namespace RbConstants {

	namespace Double {
	const double    inf       = DBL_MAX;
	const double    neginf 	  = DBL_MIN;
	}

	namespace Object {
	const double    undefined = DBL_MAX;
	const double    neginf    = DBL_MIN;
	}

}

#endif
