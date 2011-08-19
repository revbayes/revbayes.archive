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

#include <ostream>
#include <string>

class VectorString;

const std::string Dist_neutralcoalescent_name = "Dist_neutralcoalescent";

class Dist_neutralcoalescent: public Distribution{

public:
    Dist_neutralcoalescent(void);                                                                   //!< Default constructor

    // Basic utility functions
    Dist_neutralcoalescent*     clone(void) const;                                                  //!< Clone object
    const VectorString&         getClass(void) const;                                               //!< Get class vector

    // Member variable setup
    const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules

    // Discrete distribution functions
    const TypeSpec              getVariableType(void) const;                                        //!< Get random variable type (Simplex)
    double                      lnPdf(const RbLanguageObject* value);                               //!< Ln probability density
    double                      pdf(const RbLanguageObject* value);                                 //!< Probability density
    TreePlate*                  rv(void);                                                           //!< Generate tree from the coalescent

private:
    void                        buildRandomBinaryTree(std::vector<TopologyNode *> &interior, std::vector<TopologyNode *> &tips, size_t numTaxa);


};

#endif

