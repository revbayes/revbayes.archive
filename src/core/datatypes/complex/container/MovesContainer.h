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

#ifndef MovesContainer_H
#define MovesContainer_H

#include "Container.h"
#include "Move.h"

#include <ostream>
#include <string>
#include <vector>

class VectorIndex;
class VectorString;


/**
 * This class is used for all complex data types seen by the parser
 * as being multidimensional. 
 *
 */
const std::string MovesContainer_name = "Move Container";

class MovesContainer : public Container {
    
public: 
    MovesContainer(void);                                                                               //!< Default constructor 
    MovesContainer(size_t length);                                                                      //!< Default constructor with number of elements
    virtual                 ~MovesContainer(void);                                                      //!< Virtual destructor
    
    // Basic utility functions
    bool                    allowsVariableInsertion(void) const { return false; }                       //!< Yes we do allow variable to be inserted
    MovesContainer*         clone(void) const;                                                          //!< Clone object
    const VectorString&     getClass(void) const;                                                       //!< Get class
    void                    printValue(std::ostream& o) const;                                          //!< Print value for user
    std::string             richInfo(void) const;                                                       //!< Complete info about object
    
    // Container functions
    void                    clear(void);                                                                //!< Clear
    Move*                   getElement(size_t index);                                                   //!< Get element or subcontainer
    void                    push_back(Move* m);                                                         //!< Add a move object
    void                    resize(const size_t len);                                                   //!< Resize
    void                    setElement(const size_t index, Move *elem);                                 //!< Set an element
    
private:
    
    std::vector<Move*>   elements;                                                                      //!< The elements
    
};  

#endif

