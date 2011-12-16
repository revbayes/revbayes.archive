/**
 * @file
 * This file contains the declaration of Mixture, a type
 * used to hold a mixture model, where elements are assigned different parameter values.
 *
 * @brief Declaration of Mixture
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#ifndef Mixture_h
#define Mixture_h

#include "MutableMemberObject.h"
#include <string>
#include "Vector.h"
#include "VectorNatural.h"
#include "VectorRealPos.h"

const std::string Mixture_name = "Mixture";

class Mixture: public MutableMemberObject {
    
public:
    Mixture(void);                                                                                          //!< Default constructor
    Mixture(VectorNatural& allocationVector, std::vector<RbPtr<Vector> >& parameters);                      //!< constructor
    Mixture(VectorNatural& allocationVector, std::vector<RbPtr<Vector> >& parameters, VectorRealPos& classProbabilities);        //!< constructor

    Mixture(const Mixture& m);                                                                              //!< Copy constructor
    ~Mixture(void);                                                                                         //!< Destructor
    
    // Basic utility functions
    Mixture*                        clone(void) const;                                                      //!< Clone object
    const VectorString&             getClass(void) const;                                                   //!< Get class vector
    const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
    void                            printValue(std::ostream& o) const;                                      //!< Print value for user
    std::string                     richInfo(void) const;                                                   //!< Complete info
    
    // Member variable functions
    RbPtr<const MemberRules>        getMemberRules(void) const;                                             //!< Get member rules
    void                            setMemberVariable(const std::string& name, RbPtr<Variable> var);        //!< Catch setting of the topology
    
    // Member method inits
    RbPtr<RbLanguageObject>         executeOperation(const std::string& name, Environment& args);           //!< Execute method
    RbPtr<const MethodTable>        getMethods(void) const;                                                 //!< Get methods
    
    // Mixture functions
    size_t                          getNumberOfClasses();                                                   //!< Get the number of classes in the mixture
    void                            setNumberOfClasses(size_t numClasses);                                  //!< Set the number of classes in the mixture
    void                            addClass();                                                             //!< Add a new class to the mixture
    void                            removeClass();                                                          //!< Remove a class from the mixture
    const std::vector<RbPtr<Vector> >&     getVectorOfParameters();                                         //!< Get the vector of parameter values associated to the classes of the mixture
    void                            setVectorOfParameters(std::vector<RbPtr<Vector> >& parameters);         //!< Set the vector of parameter values associated to the classes of the mixture
    void                            setParameterValue();                                                    //!< Set the value of a parameter associated to a particular class
    VectorNatural                   getAllocationVector();                                                  //!< Get the allocation vector associating class indices to elements
    void                            setAllocationVector(Vector allocationVector);                           //!< Set the allocation vector associating class indices to elements
    void                            allocateElement (int elementId, int classId);                           //!< Change the class of a particular element
    VectorRealPos                   getClassProbabilities();                                                //!< Get the vector containing class probabilities
    void                            setClassProbabilities();                                                //!< Set the vector containing class probabilities
    void                            estimateClassProbabilities();                                           //!< Set the vector containing class probabilities from the numbers of elements in each class
    void                            computeNumberOfElementsInClasses();                                     //!<Compute the number of elements in each class by going through the allocation vector
    void                            indexAllocationVector();                                                //!< Re-number the classes in the allocation vector so that they start from 0 and end at number_of_classes - 1
    const RbPtr<Vector>&             getParameters(int classId);                                             //!< Get the vector of parameter values associated to the classe classId

    
private:
    static const TypeSpec           typeSpec_;                                                              
    VectorNatural                   allocationVector_;                                                      //!< Vector allocating elements to cluster indices
    std::vector<RbPtr<Vector> >     parameters_;                                                            //!< Vector of size the number of classes and containing parameters associated to the classes
    VectorNatural                   numberOfElementsInClasses_;                                             //!< Vector giving the number of elements in each class
    VectorRealPos                   classProbabilities_;                                                    //!< Vector giving class probabilities
};

#endif
