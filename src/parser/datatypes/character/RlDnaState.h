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

#include "RlCharacterState.h"
#include "DnaState.h"

#include <ostream>
#include <string>


class RlDnaState : public RlCharacterState {
    
public:
    RlDnaState(void);                                                                   //!< Default constructor
    RlDnaState(const DnaState &s);                                                      //!< Default constructor

    
    virtual bool                    operator==(const RlDnaState& x) const;              //!< Equality
    virtual bool                    operator!=(const RlDnaState& x) const;              //!< Inequality
    
    // Basic utility functions
    static const std::string&       getClassName(void);                                 //!< Get class name
    static const TypeSpec&          getClassTypeSpec(void);                             //!< Get class type spec
    
    // Basic utility functions you have to override
    virtual RlDnaState*             clone(void) const = 0;                              //!< Clone object
    virtual void                    printValue(std::ostream& o) const;                  //!< Print value (for user)
    
    virtual const DnaState&         getValue() const;                                   //!< Get the internal character object.
    
private:
    DnaState                        state;
};

#endif

