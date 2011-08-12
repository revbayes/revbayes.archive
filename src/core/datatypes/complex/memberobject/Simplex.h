/**
 * @file
 * This file contains the declaration of Simplex, a complex type
 * used to hold a simplex.
 *
 * @brief Declaration of Simplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Simplex_H
#define Simplex_H

#include "VectorRealPos.h"

#include <ostream>
#include <string>
#include <vector>


/**
 * @brief Simplex class
 *
 * This class is used to hold a simplex. Note that a simplex cannot
 * be a container because then it would be a loose collection of
 * RealPos numbers. Such a collection could not be associated with
 * a single distribution. Nor would type checking work properly then,
 * because the language type would then be +Real[] and not Simplex.
 *
 * For these reasons, we derive simplex from MemberObject. We
 * implement subscripting so that you can access the elements through
 * subscripting (their values, no modify access). The subscript operator
 * is also implemented to give the RevBayes source code access to element
 * values, but not references.
 */
class Simplex : public VectorRealPos {

    public:
                                    Simplex(const size_t n = 2);                                //!< Simplex of length (size) n
                                    Simplex(const std::vector<double>& x);                      //!< Simplex from double vector
                                    Simplex(const VectorRealPos& x);                            //!< Simplex from positive real vector
    
        double                      operator[](size_t i);                                       //!< Index op
        const double&               operator[](size_t i) const;                                 //!< Const index op
        bool                        operator==(const VectorReal& x) const;                      //!< Equals comparison
        bool                        operator!=(const VectorReal& x) const;                      //!< Not equals comparison
    
        // Basic utility functions
        Simplex*                    clone(void) const;                                          //!< Clone object
        const VectorString&         getClass(void) const;                                       //!< Get class
        void                        printValue(std::ostream& o) const;                          //!< Print value (for user)
        std::string                 richInfo(void) const;                                       //!< Complete info about object
    
        // Vector functions, including STL-like functions
        void                        push_back(double x);                                        //!< Append element to end
        void                        push_front(double x);                                       //!< Add element in front
        void                        setValue(const std::vector<double>& x);                         //!< Set the value using STL vector of int
        void                        setValue(const VectorInteger& x);                               //!< Set the value using VectorInteger
        void                        setValue(const VectorNatural& x);                               //!< Set the value using VectorNatural
        void                        setValue(const VectorReal& x);                                  //!< Set the value using VectorReal
        void                        setValue(const VectorRealPos& x);                               //!< Set the value using VectorRealPos

    private:
        void                    rescale(void);                                                      //!< Rescale the simplex

};

#endif
