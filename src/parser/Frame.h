/**
 * @file
 * This file contains the declaration of Frame, which is
 * used to hold information about an evaluation or execution
 * frame. Each frame has a pointer to the enclosing (parent)
 * frame. A frame and its parents constitute an evaluation
 * environment. The base environment is the global workspace.
 * It is a special type of frame, which is described in the
 * class Workspace, derived from Frame through VariableFrame.
 *
 * A frame essentially consists of a table (map) or vector of
 * variables, which may or may not be labeled. The workspace
 * also contains a function table, and a class (type) table.
 *
 * @brief Declaration of Frame
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#ifndef Frame_H 
#define Frame_H

#include <map>
#include <string>

class VariableSlot;


/**
 * @brief Execution or evaluation frame
 *
 * Frame is used to hold information about an evaluation or
 * execution frame. Each frame has a pointer to the enclosing
 * (parent) frame. A frame and its parents together constitute
 * an evaluation environment. The base environment is the global
 * workspace. It is a special type of frame, which is described
 * in the class Workspace, derived from Frame. The global work-
 * space contains the user workspace, which is a workspace hold-
 * ing user-defined types and functions. The user workspace, in
 * turn, contains the nested regular frames, which are created
 * during execution or evaluation of code.
 *
 * A frame essentially consists of a variable table. The workspace
 * also contains a function table, and a class (type) table.
 *
 */
class Frame {

    public:
                                        Frame(void);                                                                            //!< Constructor of frame with NULL parent
                                        Frame(Frame* parentFr);                                                                 //!< Constructor of frame with parent
                                        Frame(const Frame& x);                                                                  //!< Copy constructor: set frame of variable slots

        // Operators
        virtual VariableSlot&           operator[](const std::string& name);                                                    //!< Get named variable slot reference
        virtual const VariableSlot&     operator[](const std::string& name) const;                                              //!< Get named variable slot const reference
        bool                            operator==(const Frame& x) const;                                                       //!< Equals comparison
        bool                            operator!=(const Frame& x) const { return !operator==(x); }                             //!< Not equals comparison

        // Basic utility functions
        virtual Frame*                  clone(void) const { return new Frame(*this); }                                          //!< Clone frame
        virtual Frame*                  cloneEnvironment(void) const;                                                           //!< Clone environment
        virtual void                    printValue(std::ostream& o) const;                                                      //!< Print table for user
        virtual std::string             richInfo(void) const;                                                                   //!< Complete info to string

        // Frame functions
        Frame*                          getParentFrame(void) { return parentFrame; }                                            //!< Get parent frame ptr

    protected:
        Frame*                          parentFrame;                                                                            //!< Pointer to enclosing frame
};

#endif
