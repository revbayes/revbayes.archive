/**
 * @file
 * This file contains the declaration of Move, which is the abstract
 * base class for moves used in mcmc inference.
 *
 * @brief Declaration of Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-23 18:42:52 +0100 (Fri, 23 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: Move.h 1358 2012-03-23 17:42:52Z hoehna $
 */

#ifndef ParserMove_H
#define ParserMove_H

#include "InferenceMove.h"
#include "MemberObject.h"
#include "Natural.h"
#include "RealPos.h"
#include "SimpleMemberFunction.h"

class RandomNumberGenerator;


class ParserMove : public MemberObject {
    
public:
    ParserMove(InferenceMove* m, const std::string &n, const MemberRules& mr, const std::set<std::string> &attrNames);       //!< Constructor
    ParserMove(const ParserMove& m);                                                                                                            //!< Copy constructor
    virtual                                 ~ParserMove(void);                                                                             //!< Destructor
    
    // Basic utility functions
    ParserMove*                             clone(void) const;                                                                  //!< Clone the object
    static const std::string&               getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                                 //!< Get class type spec
    virtual void                            printValue(std::ostream& o) const;
    const TypeSpec&                         getTypeSpec(void) const;
    
    // Member variable rules
    virtual const MemberRules&              getMemberRules(void) const;                                                             //!< Get member rules
    
    // Member methods
    void                                    setMemberVariable(const std::string& name, const Variable* var);                        //!< set the member variables
    
    // Move functions you have to override
    
    // functions you should not override
    const InferenceMove*                    getLeanMove(void) const;
    const std::vector<RbPtr<const DAGNode> >&   getMoveArgumgents(void) const;

protected:
    
    
private:
    InferenceMove*                          move;
    TypeSpec                                typeSpec;

    std::set<std::string>                   attributeNames;
    std::vector<RbPtr<const DAGNode> >      args;
    MemberRules                             memberRules;
};

#endif
