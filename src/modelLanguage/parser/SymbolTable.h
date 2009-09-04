/*!
 * \file This file contains the declaration of SymbolTable, which is
 * used to hold the symbol table used by the Parser class.
 *
 * \brief Declaration of SymbolTable
 *
 * MrBayes version 4.0 beta
 *
 * (c) Copyright 2005-
 * \version 4.0 Beta
 * \date Last modified: $Date: 2009/02/03 16:26:33 $
 * \author John Huelsenbeck (1)
 * \author Bret Larget (2)
 * \author Paul van der Mark (3)
 * \author Fredrik Ronquist (4)
 * \author Donald Simon (5)
 * \author (authors listed in alphabetical order)
 * (1) Department of Integrative Biology, University of California, Berkeley
 * (2) Departments of Botany and of Statistics, University of Wisconsin - Madison
 * (3) Department of Scientific Computing, Florida State University
 * (4) Department of Entomology, Swedish Museum of Natural History, Stockholm
 * (5) Department of Mathematics/Computer Science, Duquesne University
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License (the file gpl.txt included with this
 * distribution or http://www.gnu.org/licenses/gpl.txt) for more
 * details.
 *
 * $Id: SymbolTable.h,v 1.1 2009/02/03 16:26:33 ronquist Exp $
 */

#ifndef SymbolTable_H
#define SymbolTable_H

#include <map>
#include <string>

#include "MbObject.h"

using namespace std;


//! Class SymbolTable is used to maintain a table of all the symbols that are defined.
//  Each entry in the table is a pair consisting of a string holding the name of the entry (the key)
//  and a pointer to the object itself. Note that the same object can be pointed to by several
//  different keys in the symbol table. These multiple keys are synonymous names for the
//  object. On the other hand, all symbols in the table are unique, that is, no two objects can
//  have the same name. This is unlike R, for instance, where functions and variables can have
//  the same name.
//
//  Functions known to the program are also entries in the symbol table. The object in this case is
//  derived from MbFunction, which is itself derived from MbObject.
//
//  Initialization of the symbol table is taken care of in the default constructor. It is there
//  that programmers need to add entries if they wish to extend the functionality of MrBayes 4.
//
//  If we wish to support scoping in the future (for loops and user-defined functions), this is
//  probably the right place to keep track of the scope.
class SymbolTable {

    public:
                  SymbolTable();     //!< default constructor, built-in objects are added here
                  ~SymbolTable();    //!< destructor, objects are deleted here if appropriate
            
        void      erase(const string &name);                    //!< erase entry
        bool      exists(const string &name) const;             //!< check if entry exists
        void      insert(const string &name, MbObject *entry);  //!< add entry
        MbObject *getEntry(string &name);                       //!< get entry
        void      print(ostream &c) const;                      //!< print table

    private:
        map<const string, MbObject *> table;      //!< table holding entries in the symbol table
};

#endif

