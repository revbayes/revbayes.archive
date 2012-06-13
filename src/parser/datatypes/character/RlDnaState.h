/**
 * @file
 * This file contains the declaration of RlDnaState, which is
 * the abstract base class for RlDnaStates, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of RlDnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 17:24:58 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RlDnaState.h 1573 2012-05-24 15:24:58Z hoehna $
 */

#ifndef RlDnaState_H
#define RlDnaState_H

#include "RlDiscreteCharacterState.h"
#include "DnaState.h"

#include <ostream>
#include <string>


class RlDnaState : public RlDiscreteCharacterState {
    
public:
    RlDnaState(void);                                                                   //!< Default constructor
    RlDnaState(const DnaState &s);                                                      //!< Default constructor

    
    bool                            operator==(const RlCharacterState& x) const;        //!< Equality
    
    // the value type definition
    typedef DnaState valueType;

    // Basic utility functions    
    RlDnaState*                     clone(void) const;                                  //!< Clone object
    static const std::string&       getClassName(void);                                 //!< Get class name
    static const TypeSpec&          getClassTypeSpec(void);                             //!< Get class type spec
    const TypeSpec&                 getTypeSpec(void) const;                                   //!< Get language type of the object
    void                            initialize(const std::vector<RbObject*>& attributes);   //!< Initialize this object with the values inside the vector
    void                            printValue(std::ostream& o) const;                  //!< Print value (for user)
    RbValue<void*>                  getLeanValue(void) const;                           //!< Transform the object into a basic element pointer for fast access.
    size_t                          memorySize(void) const;                             //!< Get the size

    const DnaState&                 getValue() const;                                   //!< Get the internal character object.
    
private:
    DnaState                        state;
};

#endif

