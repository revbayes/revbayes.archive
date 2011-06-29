/**
 * @file
 * This file contains the declaration of VectorContinuousObservations, a complex type
 * used to hold continuous character observations.
 *
 * @brief Declaration of VectorContinuousObservations
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id:$
 */

#ifndef VectorContinuousObservations_H
#define VectorContinuousObservations_H

#include "VectorCharacterObservations.h"
#include <iostream>
#include <vector>

class CharacterObservationContinuous;



class VectorContinuousObservations : public VectorCharacterObservations {

    public:
	                                                 VectorContinuousObservations(void);                                          //!< Default constructor (empty vector)
                                                     VectorContinuousObservations(const VectorContinuousObservations& x);         //!< Construct vector with a vector of continuous characters

        // Overloaded operators
	    CharacterObservationContinuous&              operator[](size_t i);                                                        //!< Index op allowing change
	    const CharacterObservationContinuous&        operator[](size_t i) const;                                                  //!< Const index op
        bool                                         operator==(const VectorContinuousObservations& x) const;                     //!< Equality
        bool                                         operator!=(const VectorContinuousObservations& x) const;                     //!< Inequality
        VectorContinuousObservations                 operator+(const VectorContinuousObservations& x) const;                      //!< Concatenate
        VectorContinuousObservations                 operator+(const CharacterObservationContinuous& x) const;                    //!< Concatenate

        // Basic utility functions
        VectorContinuousObservations*                clone(void) const;                                                           //!< Clone object
        const VectorString&                          getClass(void) const;                                                        //!< Get class
        void                                         printValue(std::ostream& o) const;                                           //!< Print value for user
        std::string                                  richInfo(void) const;                                                        //!< Complete info about object
        
        // Vector functions
	    void                                         push_back(CharacterObservationContinuous x);                                 //!< Add element
        std::vector<CharacterObservationContinuous*> getStdVector(void) const;                                                    //!< Get the value vector as a std::vector

    protected:
        RbObject*                                    getDefaultElement(void) const { return new VectorContinuousObservations(); } //!< Get default element for empty slots
};

#endif

