/**
 * @file
 * This file contains the declaration of Container, which is
 * a generic container of RbObject objects.
 *
 * @brief Declaration of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#ifndef DagNodeContainer_H
#define DagNodeContainer_H

#include "Container.h"
#include "RbObject.h"
#include "Variable.h"
#include "VariableSlot.h"

#include <ostream>
#include <string>
#include <vector>


/**
 * This class is used for all complex data types seen by the parser
 * as being multidimensional. 
 *
 */
const std::string DagNodeContainer_name = "DAG Node Container";

class DagNodeContainer : public Container {
    
public: 
    DagNodeContainer(void);                                                                             //!< Default constructor 
    DagNodeContainer(size_t length);                                                                    //!< Default constructor with number of elements
    virtual                 ~DagNodeContainer(void);                                                    //!< Virtual destructor
    
    // Basic utility functions
    bool                    allowsVariableInsertion(void) const { return true; }                        //!< Yes we do allow variable to be inserted
    DagNodeContainer*       clone(void) const;                                                          //!< Clone object
    RbObject*               convertTo(const TypeSpec& type) const;                                      //!< Convert to type
    const VectorString&     getClass(void) const;                                                       //!< Get class vector
    const TypeSpec&         getTypeSpec(void) const;                                                    //!< Get language type of the object
    bool                    isConvertibleTo(const TypeSpec& type) const;                                //!< Is convertible to type?
    void                    printValue(std::ostream& o) const;                                          //!< Print value for user
    std::string             richInfo(void) const;                                                       //!< Complete info about object

    // Container functions
    void                    clear(void);                                                                //!< Clear
    RbPtr<RbObject>         getElement(size_t index) const;                                             //!< Get element
    void                    pop_back(void);                                                             //!< Drop element at back
    void                    pop_front(void);                                                            //!< Drop element from front
    void                    push_back(RbPtr<RbObject> x);                                                     //!< Append element to end
    void                    push_front(RbPtr<RbObject> x);                                                    //!< Add element in front
    void                    resize(size_t n);                                                           //!< Resize to new AbstractVector of length n
    void                    setElement(const size_t index, RbPtr<RbObject> elem);                             //!< Set element with type conversion
    void                    setElement(const size_t index, RbPtr<Variable> elem);                             //!< Set an element
    void                    sort(void);                                                                 //!< sort the AbstractVector
    size_t                  size(void) const;                                                           //!< get the number of elements in the AbstractVector
    void                    unique(void);                                                               //!< removes consecutive duplicates

    
private:
    
    std::vector<RbPtr<VariableSlot> >   elements;                                                              //!< The elements
    TypeSpec                typeSpec;                                                                   //!< The type of this instance
};  

#endif

