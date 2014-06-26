//
//  RealMatrix.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__RealMatrix__
#define __revbayes__RealMatrix__

#include <iostream>



#include <iostream>
#include <vector>

#include "MatrixReal.h"
#include "RlModelVariableWrapper.h"
#include "TypedDagNode.h"


namespace RevLanguage {
    
    class RealMatrix : public RlModelVariableWrapper<RevBayesCore::MatrixReal>  {
        
    public:
        
        RealMatrix(void);
        RealMatrix(RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal> *mat);                                                            //!< Construct from DAG node
        RealMatrix(const RealMatrix& from);                                                                                    //!< Copy constructor
        
        
        // the value type definition
        virtual RealMatrix*    clone(void) const;                                                      //!< Clone object
        virtual RbLanguageObject*       convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&       getClassName(void);                                                     //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&         getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual bool                    isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        void                            printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
        
    };

}

#endif /* defined(__revbayes__RealMatrix__) */
