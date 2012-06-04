/**
 * @file
 * This file contains the declaration of a Topology a light-weight class that holds the topology of a tree as a set of nodes.
 *
 * @brief Declaration of the Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-15 18:59:11 +0200 (Tue, 15 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: RlTopology.h 1544 2012-05-15 16:59:11Z hoehna $
 */

#ifndef RlTopology_H
#define RlTopology_H

#include "MemberObject.h"
#include "Natural.h"
#include "Topology.h"
#include <set>
#include <string>

class MethodTable;
class TopologyNode;


class RlTopology: public MemberObject {
    
public:
    RlTopology(void);                                                                                                   //!< Default constructor
    RlTopology(const Topology& t);                                                                                      //!< Constructor with internal topology
    ~RlTopology(void);                                                                                                  //!< Destructor
        
    // Basic utility functions
    RlTopology*                                 clone(void) const;                                                      //!< Clone object
    static const std::string&                   getClassName(void);                                                         //!< Get class name
    static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
    const TypeSpec&                             getTypeSpec(void) const;                                                //!< Get language type of the object  
    void                                        printValue(std::ostream& o) const;                                      //!< Print value for user
    
    // Member variable rules
    const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules
    
    // Member method inits
    virtual RbPtr<RbLanguageObject>             executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args);         //!< Override to map member methods to internal functions
    const MethodTable&                          getMethods(void) const;                                                 //!< Get methods
    
    const Topology&                             getValue(void) const;

private:
    Topology                                    topology;
};

#endif

