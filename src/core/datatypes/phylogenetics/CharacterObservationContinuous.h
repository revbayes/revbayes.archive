/**
 * @file
 * This file contains the declaration of CharacterObservationContinuous, which is
 * the class for the continuous data types in RevBayes.
 *
 * @brief Declaration of CharacterObservationContinuous
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CharacterObservationContinuous_H
#define CharacterObservationContinuous_H

#include "CharacterObservation.h"
#include <ostream>
#include <vector>



class CharacterObservationContinuous : public CharacterObservation {

    public:
                                        CharacterObservationContinuous(void);                                    //!< Default constructor
                                        CharacterObservationContinuous(const CharacterObservationContinuous& s); //!< Copy constructor
                                        CharacterObservationContinuous(const double x);                          //!< Constructor with mean value
                                        CharacterObservationContinuous(const double x, const double v);          //!< Constructor with mean and variance values
        bool                            operator==(const CharacterObservation& x) const;                         //!< Equality
        bool                            operator!=(const CharacterObservation& x) const;                         //!< Inequality

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                                       //!< Print value (for user)

        // Basic utility functions you have to override
        CharacterObservationContinuous* clone(void) const;                                                       //!< Clone object
        const VectorString&             getClass(void) const;                                                    //!< Get class vector
        std::string                     richInfo(void) const;                                                    //!< Complete info about object

        // Discrete character observation functions
        const double                    getValue(void) const { return mean; }                                    //!< Get the continuous observation
        void                            setValue(const double x) { mean = x; }                                   //!< Set the continuous observation
        const double                    getVariance(void) const { return variance; }                             //!< Get the continuous observation
        bool                            isMissAmbig(void) const { return false; }                                //!< Is the character missing or ambiguous
        void                            setVariance(const double x) { variance = x; }                            //!< Set the continuous observation

    private:
        double                          mean;                                                                    //!< Mean value (or just the value)
        double                          variance;                                                                //!< Variance (most likely 0)
};

#endif

