/**
 * @file
 * This file contains the declaration of ArgumentRules, which is
 * is the container for all argument rules. We need a special object
 * for proper memory allocation and copying of the objects.
 *
 * @brief Declaration of ArgumentRules
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-02-13, version 1.0
 *
 * $Id$
 */

#ifndef ArgumentRules_H
#define ArgumentRules_H

#include "RbInternal.h"

#include <vector>

class ArgumentRule;
class VectorString;

const std::string ArgumentRules_name = "argument rules";

class ArgumentRules : public RbInternal {
    
public:
                                                ArgumentRules();
                                                ArgumentRules(const ArgumentRules& a);
    virtual                                    ~ArgumentRules();
    
    ArgumentRules&                              operator=(const ArgumentRules& a);
    ArgumentRule&                               operator[](size_t index);
    const ArgumentRule&                         operator[](size_t index) const;
    
    // Basic utility functions
    virtual ArgumentRules*                      clone(void) const { return new ArgumentRules(*this); }                                               //!< Clone object
    virtual const VectorString&                 getClass(void) const;                                                                               //!< Get class vector
    virtual const TypeSpec&                     getTypeSpec(void) const;                                                                            //!< Get language type of the object
    void                                        printValue(std::ostream& o) const;                                                                  //!< Print value for user

    std::vector<ArgumentRule*>::iterator        begin(void);
    std::vector<ArgumentRule*>::const_iterator  begin(void) const;
    void                                        clear(void);
    std::vector<ArgumentRule*>::iterator        end(void);
    std::vector<ArgumentRule*>::const_iterator  end(void) const;
    void                                        insert(std::vector<ArgumentRule*>::iterator it, ArgumentRule* rule);
    void                                        insert(std::vector<ArgumentRule*>::iterator it, std::vector<ArgumentRule*>::const_iterator first, std::vector<ArgumentRule*>::const_iterator last);
    void                                        push_back(ArgumentRule* rule);
    size_t                                      size(void) const;
private:
    
    std::vector<ArgumentRule*>                  rules;
    
    static const TypeSpec                       typeSpec;
};

#endif
