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

#include <ostream>
#include <string>
#include <vector>

class DAGNode;
class VariableSlot;
class VectorIndex;
class VectorString;


/**
 * This class is used for all complex data types seen by the parser
 * as being multidimensional. 
 *
 */
class DagNodeContainer : public Container {
    
public: 
    DagNodeContainer(void);                                                                             //!< Default constructor 
    DagNodeContainer(size_t length);                                                                    //!< Default constructor with number of elements
    virtual                 ~DagNodeContainer(void);                                                    //!< Virtual destructor
    
    // Basic utility functions
    bool                    allowsVariableInsertion(void) const { return true; }                        //!< Yes we do allow variable to be inserted
    DagNodeContainer*       clone(void) const;                                                          //!< Clone object
    RbLanguageObject*       convertTo(const std::string& type) const;                                   //!< Convert to type
    const VectorString&     getClass(void) const;                                                       //!< Get class vector
    bool                    isConvertibleTo(const std::string& type, bool once) const;                  //!< Is convertible to type?
    void                    printValue(std::ostream& o) const;                                          //!< Print value for user
    std::string             richInfo(void) const;                                                       //!< Complete info about object

    // Container functions
    void                    clear(void);                                                                //!< Clear
    VariableSlot*           getElement(size_t index);                                                   //!< Get element or subcontainer
    void                    resize(const size_t len);                                                   //!< Resize
    void                    setElement(const size_t index, Variable *elem);                             //!< Set an element
    
private:
    
    std::vector<VariableSlot*>   elements;                                                              //!< The elements
    
};  

#endif

