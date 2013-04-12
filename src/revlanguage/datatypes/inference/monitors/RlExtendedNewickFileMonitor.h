/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of an extended newick file monitor.
 *
 * @brief Declaration of RlExtendedNewickFileMonitor
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef RlExtendedNewickFileMonitor_H
#define RlExtendedNewickFileMonitor_H

#include "ExtendedNewickTreeMonitor.h"
#include "RlMonitor.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class ExtendedNewickFileMonitor : public Monitor {
        
    public:
        
        ExtendedNewickFileMonitor(void);                                                                                      //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual ExtendedNewickFileMonitor*          clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal ExtendedNewickFileMonitor.
        static const std::string&                   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);              //!< Set member variable
        
        std::set<RbPtr<const Variable> >            vars;
        RbPtr<const Variable>                       filename;
        RbPtr<const Variable>                       tree;
        RbPtr<const Variable>                       printgen;
        RbPtr<const Variable>                       separator;
        RbPtr<const Variable>                       prior;
        RbPtr<const Variable>                       posterior;
        RbPtr<const Variable>                       likelihood;
        
    };
    
}

#endif
