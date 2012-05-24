/**
 * @file
 * This file contains the declaration of CharacterContinuous, which is
 * the class for the continuous data types in RevBayes.
 *
 * @brief Declaration of CharacterContinuous
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CharacterContinuous_H
#define CharacterContinuous_H

#include "Character.h"
#include <ostream>
#include <vector>



class CharacterContinuous : public Character {

    public:
                                        CharacterContinuous(void);                           //!< Default constructor
                                        CharacterContinuous(const CharacterContinuous& s);   //!< Copy constructor
                                        CharacterContinuous(const double x);                 //!< Constructor with mean value
                                        CharacterContinuous(const double x, const double v); //!< Constructor with mean and variance values
        bool                            operator==(const Character& x) const;                //!< Equality
        bool                            operator!=(const Character& x) const;                //!< Inequality

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                   //!< Print value (for user)

        // Basic utility functions you have to override
        CharacterContinuous*            clone(void) const;                                   //!< Clone object
        static const std::string&       getClassName(void);                                  //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                              //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                             //!< Get language type of the object

        // Discrete character observation functions
        bool                            getIsGapState(void) const { return false; }          //!< Get whether this is a gapped character state
        const double                    getMean(void) const { return mean; }                 //!< Get the continuous observation
        virtual double                  getRealValue(void) const { return mean; }            //!< Return the double representation of the character
        const double                    getValue(void) const { return mean; }                //!< Get the continuous observation
        void                            setValue(double x) { mean = x; }                     //!< Set the continuous observation
        const double                    getVariance(void) const { return variance; }         //!< Get the continuous observation
        bool                            isMissingOrAmbiguous(void) const { return false; }   //!< Is the character missing or ambiguous
        void                            setIsGapState(bool tf) { }                           //!< Set whether this is a gapped character
        void                            setVariance(const double x) { variance = x; }        //!< Set the continuous observation

    private:
        double                          mean;                                                //!< Mean value (or just the value)
        double                          variance;                                            //!< Variance (most likely 0)
    
};

#endif

