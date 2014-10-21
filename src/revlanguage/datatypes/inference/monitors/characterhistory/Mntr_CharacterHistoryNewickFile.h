/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of an extended newick file monitor.
 *
 * @brief Declaration of Mntr_CharacterHistoryNewickFile
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

#ifndef Mntr_CharacterHistoryNewickFile_H
#define Mntr_CharacterHistoryNewickFile_H

#include "TreeCharacterHistoryNodeMonitor.h"
#include "RlMonitor.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Mntr_CharacterHistoryNewickFile : public Monitor {
        
    public:
        
        Mntr_CharacterHistoryNewickFile(void);                                                                              //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Mntr_CharacterHistoryNewickFile*    clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Mntr_CharacterHistoryNewickFile.
        static const std::string&                   getClassType(void);                                                     //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                     //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstParameter(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        std::set<RevPtr<const Variable> >           vars;
        RevPtr<const Variable>                      filename;
        RevPtr<const Variable>                      tree;
        RevPtr<const Variable>                      ctmc;
        RevPtr<const Variable>                      printgen;
        RevPtr<const Variable>                      separator;
        RevPtr<const Variable>                      prior;
        RevPtr<const Variable>                      posterior;
        RevPtr<const Variable>                      likelihood;
        RevPtr<const Variable>                      type;
//        RevPtr<const Variable>                      counts;
//        RevPtr<const Variable>                      events;
        RevPtr<const Variable>                      style;
        RevPtr<const Variable>                      append;
        
    };
    
}

#endif
