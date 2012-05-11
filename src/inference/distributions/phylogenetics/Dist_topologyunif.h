///**
// * @file
// * This file contains the declaration of Dist_topologyunif, which holds
// * the parameters and functions related to a uniform distribution on
// * topologies (unrooted or rooted, binary or polytomous).
// *
// * @brief Declaration of Dist_topologyunif
// *
// * (c) Copyright 2009- under GPL version 3
// * @date Last modified: $Date$
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// * @since 2009-08-27, version 1.0
// *
// * $Id$
// */
//
//#ifndef Dist_topologyunif_H
//#define Dist_topologyunif_H
//
//#include "DistributionDiscrete.h"
//#include "Topology.h"
//#include "Variable.h"
//
//#include <ostream>
//#include <string>
//
//class TopologyNode;
//
//
//class Dist_topologyunif: public DistributionDiscrete {
//
//    public:
//                                                Dist_topologyunif(void);                                            //!< Parser constructor
//
//        // Basic utility functions
//        Dist_topologyunif*                      clone(void) const;                                                  //!< Clone object
//        static const std::string&               getClassName(void);                                                 //!< Get class name
//        static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
//        const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object
//    
//        // Member variable setup
//        const MemberRules&                      getMemberRules(void) const;                                         //!< Get member variable rules
//        void                                    setMemberVariable(const std::string& name, const Variable* var);    //!< Set member variable (ensure topologyProb is updated)
//
//        // Discrete distribution functions
//        size_t                                  getNumberOfStates(void) const;                                      //!< Get number of states
//        virtual const Simplex&                  getProbabilityMassVector(void);                                     //!< Get probability mass vector
//        const std::vector<RbLanguageObject*>&   getStateVector(void) const;
//        const TypeSpec&                         getVariableType(void) const;                                        //!< Get random variable type (Simplex)
//        double                                  lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
//        double                                  pdf(const RbLanguageObject& value) const;                           //!< Probability density
//        const RbLanguageObject&                 rv(void);                                                           //!< Generate random variable
//
//    private:
//        void                            buildRandomBinaryTree(std::vector<TopologyNode* >& tips, size_t numTaxa);    //!< Build a random binary tree recursively until we have numTaxa tips
//        void                            calculateTopologyProb(void);                                        //!< Calculate and set probability member variables
//        void                            calculateNumberOfStates(void);                                      //!< Calculate and set number of states member variables
//
//        double                          topologyProb;                                                       //!< The probability of a topology
//        double                          lnTopologyProb;                                                     //!< The log probability of a topology
//        size_t                          numberOfStates;                                                     //!< The number of states for this topology distribution
//    
//        // parameters
//        RbConstVariablePtr              numTaxa;
//        RbConstVariablePtr              isRooted;
//        RbConstVariablePtr              isBinary;
//        RbConstVariablePtr              tipNames;
//    
//        // memberfunction return values
//        Topology                        randomVariable;
//};
//
//#endif
//
