/**
 * @file
 * This file contains the declaration of MemberEnvironment, which is
 * used to hold member variables in MemberObject data types.
 *
 * @brief Declaration of MemberEnvironment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#ifndef MemberEnvironment_H 
#define MemberEnvironment_H

#include "Environment.h"


#include <string>
#include <vector>


const std::string MemberEnvironment_name = "Member Environment";

class MemberEnvironment : public Environment {
    
public:
    MemberEnvironment(void);                                                                         //!< Constructor of variable Environment with NULL parent
    MemberEnvironment(RbPtr<Environment> parentFr);                                                        //!< Constructor of variable Environment with parent
    MemberEnvironment(const MemberEnvironment &x);                                                   //!< Copy constructor
    virtual ~MemberEnvironment();                                                                    //!< Destructor
    
    // Basic utility functions
    MemberEnvironment*          clone(void) const { return new MemberEnvironment(*this); }           //!< Clone variable Environment
    const VectorString&         getClass() const;                                                    //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                                             //!< Get language type of the object
    void                        printValue(std::ostream& o) const;                                   //!< Print table for user
    std::string                 richInfo(void) const;                                                //!< Complete info to string
    
private:
    static const TypeSpec       typeSpec;
};

#endif
