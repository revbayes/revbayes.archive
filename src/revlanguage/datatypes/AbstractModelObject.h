/**
 * @file
 * This file contains the declaration of the Rev language ModelObject, which is
 * the Rev abstract base class for all language objects wrapping core datatypes
 * that can be stored inside DAG nodes and hence used in model graphs.
 *
 * @brief Declaration of ModelObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-08-06
 *
 * $Id: RevObject.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef AbstractModelObject_H
#define AbstractModelObject_H

#include "RevObject.h"

namespace RevLanguage {
    
    class AbstractModelObject : public RevObject {
    
    public:
        
        virtual                                ~AbstractModelObject(void) {}                                                          //!< Virtual destructor
        
        std::string                             getGuiVariableName(void) { return guiVariableName; }
        std::string                             getGuiLatexSymbol(void) { return guiLatexSymbol; }
        bool                                    isModelObject(void) const;
        void                                    setGuiVariableName(std::string s) { guiVariableName = s; }
        void                                    setGuiLatexSymbol(std::string s) { guiLatexSymbol = s; }

     private:
        
        std::string                             guiVariableName;
        std::string                             guiLatexSymbol;

    };
    
}

#endif
