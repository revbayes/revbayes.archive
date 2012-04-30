/**
 * @file
 * This file contains the declaration of Move_mslide, which moves
 * a real value uniformly within a sliding window.
 *
 * @brief Declaration of Move_mslide
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef Move_mslide_H
#define Move_mslide_H

#include <ostream>
#include <set>
#include <string>

#include "InferenceMoveSimple.h"


class Move_slide : public InferenceMoveSimple {

    public:
                                    Move_slide(void);                                                       //!< Parser constructor

        // Basic utility functions
        Move_slide*                 clone(void) const;                                                      //!< Clone object

	protected:
        double                      performSimpleMove(void);                                                          //!< Perform move
        void                        rejectSimpleMove(void);
    
    private:

        // parameters
        double                      delta;
};

#endif

