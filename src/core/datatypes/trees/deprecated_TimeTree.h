///**
// * @file
// * This file contains the declaration of a time calibrated tree, a light-weight class 
// * that holds the pointer to the root node of a tree and provides some convinience functions.
// *
// * @brief Declaration of the TimeTree
// *
// * (c) Copyright 2009- under GPL version 3
// * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
// * @author The RevBayes Development Core Team
// * @license GPL version 3
// * @version 1.0
// * @since 2011-04-13, version 1.0
// *
// * $Id: TimeTree.h 1651 2012-07-05 14:47:08Z hoehna $
// */
//
//#ifndef TimeTree_H
//#define TimeTree_H
//
//#include "Tree.h"
//
//#include <vector>
//#include <string>
//
//namespace RevBayesCore {
//    
//    class TimeTree : public Tree {
//    
//    public:
//        TimeTree(void);                                                                                                     //!< Default constructor
//        virtual                                    ~TimeTree(void);                                                                                                        //!< Destructor
//        
//        // Basic utility functions
//        virtual TimeTree*                           clone(void) const;                                                      //!< Clone object
//    
//        // TimeTree functions
//        double                                      getAge(size_t idx) const;
//        double                                      getBranchLength(size_t idx) const;
//        double                                      getTime(size_t idx) const;
//        double                                      getTreeLength(void) const;
//        void                                        setAge(size_t idx, double a);
//    
//    protected:
//        void                                        resizeElementVectors(size_t n);
//        
//    private:
////        void                                        equalizeBranchLengths(TimeNode &node);                                  //!< Equalizing branch lengths between subtrees so that no rounding errors happen
//        
//        std::vector<double>                         ages;
//
//    };
//
//    // Global functions using the class
//    std::ostream&                       operator<<(std::ostream& o, const TimeTree& x);                                         //!< Overloaded output operator
//
//}
//
//#endif
//
