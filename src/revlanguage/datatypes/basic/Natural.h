#ifndef Natural_H
#define Natural_H

#include "Integer.h"

#include <ostream>
#include <string>



namespace RevLanguage {

    class RealPos;

    /**
     * Primitive type for Natural numbers (including 0).
     *
     * Note that we derive this from Integer. To make
     * sure inheritance is safe, we restrict the range
     * of natural numbers from 0 to to INT_MAX
     */
    class Natural : public Integer {

        public:
        Natural(void);                                                                                      //!< Default constructor (value is 0)
        Natural(RevBayesCore::TypedDagNode<int> *v);                                                        //!< Constructor with DAG node
        Natural(int x);                                                                                     //!< Constructor from int
        Natural(unsigned int x);                                                                            //!< Constructor from int
        Natural(unsigned long x);                                                                           //!< Constructor from size_t

        // Basic operator functions
        RbLanguageObject*           add(const RbLanguageObject &rhs) const;                                 //!< Addition operator used for example in '+=' statements
        Natural*                    add(const Natural &rhs) const;                                          //!< Addition operator used for example in '+=' statements
        RealPos*                    add(const RealPos &rhs) const;                                          //!< Addition operator used for example in '+=' statements
        RbLanguageObject*           divide(const RbLanguageObject &rhs) const;                              //!< Division operator used for example in '/=' statements
        RealPos*                    divide(const Natural &rhs) const;                                       //!< Division operator used for example in '/=' statements
        RealPos*                    divide(const RealPos &rhs) const;                                       //!< Division operator used for example in '/=' statements
        RbLanguageObject*           multiply(const RbLanguageObject &rhs) const;                            //!< Multiplication operator used for example in '*=' statements
        Natural*                    multiply(const Natural &rhs) const;                                     //!< Multiplication operator used for example in '*=' statements
        RealPos*                    multiply(const RealPos &rhs) const;                                     //!< Multiplication operator used for example in '*=' statements

        // Basic utility functions
        Natural*                    clone(void) const;                                                      //!< Clone object
        RbLanguageObject*           convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        bool                        isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?

};
    
}

#endif

