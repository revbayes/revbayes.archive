/**
 * @file
 * This file contains the declaration of MoveSchedule, which is a
 * complex type holding an update weight and a set of moves for either
 * a stochastic DAG node or a set of stochastic DAG nodes retrieved
 * through a complex object like a tree.
 *
 * @brief Declaration of MoveSchedule
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef MoveSchedule_H
#define MoveSchedule_H

#include "RbComplex.h"

#include <vector>

class Move;
class RbObject;
class VectorString;
class VariableNode;

class MoveSchedule : public RbComplex {

    public:
                                MoveSchedule(VariableNode* node, double weight);    //!< Constructor
                                MoveSchedule(const MoveSchedule& x);                //!< Copy constructor
        virtual                 ~MoveSchedule();                                    //!< Delete moves

        // Assignment operator
        MoveSchedule&           operator=(const MoveSchedule& x);                   //!< Assignment operator

        // Basic RbObject functions
        RbObject*               clone() const;                                      //!< Clone object
        bool                    equals(const RbObject* obj) const;                  //!< Equals comparison
 		const VectorString&     getClass(void) const;                               //!< Get class
        void                    printValue(std::ostream& o) const;                  //!< Print value (for user)
        std::string             toString(void) const;                               //!< General info on object

        // Functions for managing the move schedule
        void                    addMove(Move* move);                                //!< Add move
        void                    eraseMove(const Move* move);                        //!< Erase  move
        std::vector<Move*>&     getMoves(void) { return schedule; }                 //!< Get the moves
        double					getUpdateWeight(void) { return nodeUpdateWeight; }  //!< Get update weight
        void                    setUpdateWeight(double weight);                     //!< Set node update weight

	protected:
        RbObject*                   convertTo(const std::string& type) const;        //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;  //!< Is convertible to type and dim?
        VariableNode*           theNode;                        //!< The node to move
        std::vector<Move*>      schedule;                       //!< The move schedule
        double                  nodeUpdateWeight;               //!< Node update weight
};

#endif


