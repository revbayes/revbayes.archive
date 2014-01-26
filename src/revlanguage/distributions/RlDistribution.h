/**
 * @file
 * This file contains the declaration of Distribution, which is
 * the interface and abstract base class for RevLanguage distribution objects.
 *
 * @brief Declaration of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface Distribution
 * @package distributions
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Function.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef RlDistribution_H
#define RlDistribution_H

#include "Distribution.h"
#include "RbLanguageObject.h"

namespace RevLanguage {

class Distribution : public RbLanguageObject {
    
public:
    virtual                                         ~Distribution(void);                                                                //!< Destructor
    
    // Basic utility functions you have to override
    virtual Distribution*                           clone(void) const = 0;                                                              //!< Clone object
    static const std::string&                       getClassName(void);                                                                 //!< Get class name
    static const TypeSpec&                          getClassTypeSpec(void);                                                             //!< Get class type spec
       
    // Basit utility functions
    void                                            printValue(std::ostream& o) const;                                                  //!< Print value for user
    
    // Distribution functions you have to override
    virtual RbLanguageObject*                       createRandomVariable(void) const = 0;                                               //!< Create a random variable from this distribution
    virtual RevBayesCore::Distribution*             createDistribution(void) const = 0;                                                 //!< Create a random variable from this distribution
    
    
protected:
    Distribution(void);                                                                                                                 //!< Basic constructor hidden to disallow initialization
    
};
    
}

#endif

