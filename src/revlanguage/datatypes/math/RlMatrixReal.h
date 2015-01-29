#ifndef RlMatrixReal_H
#define RlMatrixReal_H

#include <iostream>



#include <iostream>
#include <vector>

#include "MatrixReal.h"
#include "ModelObject.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class MatrixReal : public ModelObject<RevBayesCore::MatrixReal>  {
        
    public:
                           
        MatrixReal(void);                                                                                       //!< Default constructor
        MatrixReal(const RevBayesCore::MatrixReal& m);
        MatrixReal(RevBayesCore::MatrixReal* m);
        MatrixReal(RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal> *mat);                                  //!< Construct from DAG node
        
        
        // the value type definition
        virtual MatrixReal*             clone(void) const;                                                      //!< Clone object
//        void                            constructInternalObject(void);                                          //!< We construct the a new internal SlidingMove.
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
//        const MemberRules&              getParameterRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    };

}

#endif /* defined(__revbayes__MatrixReal__) */
