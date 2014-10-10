/**
 * @file
 * This file contains the declaration of the Rev language WorkspaceObject, which is
 * the Rev abstract base class for all language objects wrapping core datatypes
 * that can be stored inside DAG nodes and hence used in Workspace graphs.
 *
 * @brief Declaration of WorkspaceObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-08-06
 *
 * $Id: RevObject.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef AbstractWorkspaceObject_H
#define AbstractWorkspaceObject_H

#include "RevObject.h"

namespace RevLanguage {
    
    class AbstractWorkspaceObject : public RevObject {
        
    public:
        virtual                                ~AbstractWorkspaceObject(void) {}                                                          //!< Virtual destructor
        
        virtual bool                            isWorkspaceObject(void) const;                                                            //!< Is this a Workspace object (compared with a workspace object)
        
        
    };
    
}

#endif
