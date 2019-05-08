/*
 * MntrNexusFile.h
 *
 *  Created on: May 7, 2019
 *      Author: joellebs
 */

#ifndef SRC_REVLANGUAGE_MONITORS_MNTRNEXUSFILE_H_
#define SRC_REVLANGUAGE_MONITORS_MNTRNEXUSFILE_H_

#include "RlMonitor.h"

namespace RevLanguage {

class Mntr_NexusFile: public Monitor {

public:

    Mntr_NexusFile();
    virtual Mntr_NexusFile*                     clone(void) const;
    void                                        constructInternalObject(void);  //!< Build a new internal NexusMonitor.

    static const std::string&                   getClassType(void);  //!< Get Rev type
    static const TypeSpec&                      getClassTypeSpec(void);  //!< Get class type spec
    virtual const TypeSpec&                     getTypeSpec(void) const; //!< Get language type of the object

    std::string                                 getMonitorName(void) const;  //!< Get the name used for the constructor function in Rev.
    const MemberRules&                          getParameterRules(void) const;  //!< Get member rules (const)

    virtual void                                printValue(std::ostream& o) const;  //!< Print value (for user)

protected:

    void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);   //!< Set member variable

    std::vector<RevPtr<const RevVariable> >     vars; //!< data associated with the tree
    RevPtr<const RevVariable>                   filename; //!< output file
    RevPtr<const RevVariable>                   tree; //!< monitored tree
    RevPtr<const RevVariable>                   isNodeParameter; //!< whether data is on the nodes or branches
    RevPtr<const RevVariable>                   writeTaxa; //!< whether to write a taxa block
    RevPtr<const RevVariable>                   printgen; //!< print frequency
};

} /* namespace RevLanguage */

#endif /* SRC_REVLANGUAGE_MONITORS_MNTRNEXUSFILE_H_ */
