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
#include "DagNodeContainer.h"
#include "Simplex.h"


class Mixture: public MutableMemberObject {
    
public:
    Mixture(void);                                                                                          //!< Default constructor
    Mixture(DagNodeContainer* allocationVector, DagNodeContainer* parameters);                      //!< constructor
    Mixture(DagNodeContainer* allocationVector, DagNodeContainer* parameters, Simplex* classProbabilities );        //!< constructor
    Mixture(const size_t numObservations, DagNodeContainer* parameters);                                //!< constructor


    Mixture(const Mixture& m);                                                                              //!< Copy constructor
    ~Mixture(void);                                                                                         //!< Destructor
    
    // Basic utility functions
    Mixture*                        clone(void) const;                                                      //!< Clone object
    static const std::string&       getClassName(void);                                                     //!< Get class name
    static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
    const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
    void                            printValue(std::ostream& o) const;                                      //!< Print value for user
    
    // Member variable functions
    const MemberRules&              getMemberRules(void) const;                                             //!< Get member rules
    void                            setMemberVariable(const std::string& name, Variable* var);                //!< Catch setting of the mixture
    
    // Member method inits
    const RbLanguageObject&         executeOperation(const std::string& name, Environment& args);           //!< Execute method
    const RbLanguageObject&         executeOperationSimple(const std::string& name, const std::vector<Argument>& args); //!< Execute method
    const MethodTable&              getMethods(void) const;                                                 //!< Get methods
    
    // Mixture functions
    size_t                          getNumberOfClasses();                                                   //!< Get the number of classes in the mixture
    size_t                          getNumberOfElements();                                                   //!< Get the number of elements in the mixture
    void                            addClass(DagNodeContainer* parameter);                           //!< Add a new class to the mixture
    void                            removeClass(unsigned int classId);                                      //!< Remove a class from the mixture
    DagNodeContainer&               getParameters();                                                        //!< Get the vector of parameter values associated to the classes of the mixture
    void                            setParameters(DagNodeContainer* parameters);              //!< Set the vector of parameter values associated to the classes of the mixture
    void                            setParameter(unsigned int classId, DagNodeContainer* parameter);                                                    //!< Set the value of a parameter associated to a particular class
    DagNodeContainer&               getAllocationVector();                                                  //!< Get the allocation vector associating class indices to elements
    void                            setAllocationVector(DagNodeContainer*  allocationVector);               //!< Set the allocation vector associating class indices to elements
    void                            allocateElement (int elementId, int classId);                           //!< Change the class of a particular element
    void                            allocateElementToNewClass (int elementId);                              //!< Change the class of a particular element
    Integer                         getElementAllocation (int elementId);                                   //!< Get the class of a particular element
    void                            setClassProbabilities(Simplex* proba);                                  //!< Set the vector containing class probabilities
    const Simplex&                  getClassProbabilities();                                                //!< Get the vector containing class probabilities
    void                            estimateClassProbabilities();                                           //!< Set the vector containing class probabilities from the numbers of elements in each class
    void                            computeNumberOfElementsInClasses();                                     //!< Compute the number of elements in each class by going through the allocation vector
    void                            indexAllocationVector();                                                //!< Re-number the classes in the allocation vector so that they start from 0 and end at number_of_classes - 1
    RbObject&                       getParameter(unsigned int classId);                                     //!< Get the vector of parameter values associated to the class classId
    RbObject&                       getParameterForElem(unsigned int index);                                //!< Get the vector of parameter values associated to the element index

    
private:
    DagNodeContainer*               allocationVector_;                                                      //!< Vector allocating elements to cluster indices
    DagNodeContainer*               parameters_;                                                            //!< Vector of size the number of classes and containing parameters associated to the classes
    VectorNatural                   numberOfElementsInClasses_;                                             //!< Vector giving the number of elements in each class
    Simplex*                        classProbabilities_;                                                    //!< Vector giving class probabilities

    // memberfunction return values
    Natural                         numClasses;

};

#endif
