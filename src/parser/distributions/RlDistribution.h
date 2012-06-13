/**
 * @file
 * This file contains the declaration of RlDistribution, which specifies the
 * interface for RlDistributions in RevBayes. A RlDistribution is taken in
 * the statistical sense.
 *
 * @brief Declaration of RlDistribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface RlDistribution
 *
 * $Id: RlDistribution.h 1353 2012-03-19 08:25:05Z hoehna $
 */

#ifndef RlDistribution_H
#define RlDistribution_H

#include "Distribution.h"
#include "MemberObject.h"
#include "RbValue.h"
#include "Real.h"
#include "RealPos.h"
#include "SimpleMemberFunction.h"

#include <set>
#include <string>

class RandomNumberGenerator;
class Distribution;


class RlDistribution : public MemberObject  {
    
public:
    RlDistribution( Distribution *d, const std::string &n, const MemberRules& memberRules, const RbPtr<RbLanguageObject> &rv);          //!< Simple constructor
    RlDistribution( const RlDistribution &p);                                                                                           //!< Copy constructor
    virtual                                ~RlDistribution(void) {}                                                                     //!< Destructor

    // overloaded operators
    RlDistribution&                         operator=(const RlDistribution& p);
    
    // Basic utility functions
    virtual RlDistribution*                 clone(void) const;                                                                          //!< Clone object
    static const std::string&               getClassName(void);                                                                         //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                                     //!< Get class type spec
    virtual const TypeSpec&                 getTypeSpec(void) const;    
    
    // Member object function you have to override
    virtual const MemberRules&              getMemberRules(void) const;                                                                 //!< Get member rules
    virtual RbPtr<RbLanguageObject>         executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args);     //!< Override to map member methods to internal functions

    // Member object functions you may want to override
    virtual void                            clear(void);                                                                                //!< Clear the arguments
    virtual const MethodTable&              getMethods(void) const;                                                                     //!< Get member methods
    const std::vector<RbPtr< Argument> >&   getParameters() const;
    virtual const TypeSpec&                 getVariableType(void) const;                                                                //!< Get random variable type
    virtual const RbLanguageObject&         getTemplateRandomVariable(void) const;                                                      //!< Get the template ranom variable
    
    // functions you have to override
    virtual Distribution*                   getLeanDistribution(void) const;                                                            //!< Get the lean distribution
    virtual double                          jointLnPdf( const RlValue<RbLanguageObject>& value) const;                                  //!< Ln probability density
    virtual double                          lnPdf( const RbLanguageObject& value) const;                                                //!< Ln probability density
    virtual double                          pdf( const RbLanguageObject& value) const;                                                  //!< Probability density function
    virtual void                            rv(void);                                                                                   //!< Generate a random draw
    virtual void                            setParameters(const std::vector<RbValue<void*> > &p);                                       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    virtual void                            setValue(const RbValue<void*> &v);                                                          //!< Set the pointers to the value of the distribution.
    
protected:  
    virtual void                            setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var);          //!< Set member variable
    virtual void                            setSimpleMemberValue(const std::string& name, const RbPtr<const RbLanguageObject> &var);    //!< Set member variable

    Distribution*                           distribution;
    std::string                             name;
    MemberRules                             memberRules;
    std::vector<RbPtr<Argument> >           params;
    RbPtr<RbLanguageObject>                 randomValue;
    TypeSpec                                typeSpec;

};

#endif

