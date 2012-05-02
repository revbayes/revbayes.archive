/**
 * @file
 * This file contains the declaration of Move_mscale, which scales
 * a real value with a factor drawn from an appropriate distribution.
 *
 * @brief Declaration of Move_mscale
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

#ifndef Move_mscale_H
#define Move_mscale_H

#include <ostream>
#include <set>
#include <string>

#include "InferenceMoveSimple.h"
#include "RbValue.h"


class Move_scale : public InferenceMoveSimple {

    public:
                                        Move_scale(void);                                                       //!< Parser constructor

        // Basic utility functions
        Move_scale*                     clone(void) const;                                                      //!< Clone object
        void                            setAttribute(const std::string &name, const RbValue<void*> &a);
    
	protected:
        double                          performSimpleMove(void);                                                //!< Perform move
        void                            rejectSimpleMove(void);
        void                            setInternalArguments(const std::vector<StochasticInferenceNode*> & args);
    
        // parameters
        double                          lambda;
    
        RbValue<double*>                value;
        RbValue<double*>                storedValue;
};

#endif

