/**
 * @file
 * This file contains the declaration of VariableSlot, which is
 * used to manage variables in frames and processed argument lists.
 *
 * @brief Declaration of VariableSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#ifndef VariableSlot_H 
#define VariableSlot_H

#if 0
#include "RevPtr.h"
#include "TypeSpec.h"

#include <string>

namespace RevLanguage {

class Variable;


class VariableSlot {
    
public:
    VariableSlot(const std::string &label, const TypeSpec& typeSp, const RevPtr<Variable>& var);                    //!< Constructor of filled slot
    VariableSlot(const std::string &label, const TypeSpec& typeSp);                                                 //!< Constructor of empty slot
    VariableSlot(const std::string &label, const RevPtr<Variable>& var);                                            //!< Constructor with filled slot
    virtual                                ~VariableSlot(void) {}
    
    void                                    createVariable(const std::vector<int> &indices);                        //!< Create a variable at the index (and if necessary resize).
    bool                                    doesVariableExist(const std::vector<int> &indices) const;               //!< Does a variable exist for this index?
    size_t                                  getDim(void) const;                                                     //!< Get the number of dimension
    const std::string&                      getLabel(void) const { return label; }                                  //!< Get the label of the slot
    const RevObject&                        getRevObject(void) const;                                               //!< Get the value of the variable
    RevObject&                              getRevObject(void);                                                     //!< Get the value of the variable (non-const to return non-const value)
    RevPtr<const Variable>                  getVariable(const std::vector<int> &indices) const;                     //!< Get the variable
    RevPtr<Variable>                        getVariable(const std::vector<int> &indices);                           //!< Get the variable (non-const to return non-const variable)
    virtual bool                            isValidVariable(const RevObject& newObj ) const;                        //!< Is newObj valid for the slot?
    void                                    printValue(std::ostream& o) const;                                      //!< Print value of slot
    void                                    setLabel(const std::string l) { label = l; }
    void                                    setVariable(const RevPtr<Variable>& var);                               //!< Set a slot with a variable
    
private:
    // helper methods
    RevPtr<Variable>                        getVectorizedVariable(const std::vector<int> &indices) const;
    void                                    resetNames(const std::string &l, size_t level, int &index);
    void                                    resetVariables(std::vector<RevPtr<Variable> > &v, std::vector<int> &l, std::vector<int> &i);

    
    // Member variables
    std::string                             label;                                                                  //!< the label for this slot. The label should correspond to the name under which this slot is stored in variable table. However, the label does not have to correspond to the variable name, e.g. a argument could have the label mean but the name of the variable is mu.
    std::vector<int>                        lengths;
    std::vector<RevPtr<Variable> >          variable;                                                               //!< the argument living in the slot

};

/* Global functions using the class */
std::ostream&                               operator<<(std::ostream& o, const VariableSlot& x);                     //!< Overloaded output operator

}

#endif

#endif
