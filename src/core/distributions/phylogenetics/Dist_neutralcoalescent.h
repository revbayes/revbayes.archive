/**
 * @file
 * This file contains the declaration of Dist_neutralcoalescent, which holds
 * the parameters and functions related to Kingman's neutral
 * n-coalescent distribution.
 *
 * @brief Declaration of Dist_neutralcoalescent
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-07-26, version 1.0
 *
 * $Id$
 */

#ifndef Dist_neutralcoalescent_H
#define Dist_neutralcoalescent_H

#include "Distribution.h"
#include "TreePlate.h"
#include "TopologyNode.h"

#include <ostream>
#include <string>

class VectorString;

class Dist_neutralcoalescent: public Distribution {

public:
    Dist_neutralcoalescent(void);                                                                   //!< Default constructor

    // convenience
    typedef std::map<TopologyNode*, double> TopologyNodeToRealMapType;                        //!< Save our fingers
    typedef std::map<unsigned long, double> IntegerToRealMapType;

    // Basic utility functions
    Dist_neutralcoalescent*     clone(void) const;                                                  //!< Clone object
    static const std::string&   getClassName(void);                                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                             //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object

    // Member variable setup
    const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules

    // Discrete distribution functions
    const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (Simplex)
    double                      lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
    double                      pdf(const RbLanguageObject& value) const;                           //!< Probability density
    const RbLanguageObject&     rv(void);                                                           //!< Generate tree from the coalescent

private:
    double                      drawWaitingTime(unsigned long numNodes, unsigned long haploidPopSize);

    // Private variables
    TopologyNodeToRealMapType   nodeEdgeLengths;
    IntegerToRealMapType        numNodesToWaitingTimeMap;


};

#endif

