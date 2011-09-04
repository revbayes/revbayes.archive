/**
 * @file
 * This file contains the declaration of Func_readAlignment, which uses the NCL to read in an alignment in any of the supported formats:
 * nexus
 *
 * @brief Declaration of Func_readAlignment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_readAlignment_H
#define Func_readAlignment_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class RbFileManager;
class VectorString;

/**
 * This is the interface for a class that reads in phylogenetic character matrix data, such as
 * nucleotide or amino acid data. It uses the Nexus Class Library (NCL) to do the heavy lifting
 * for reading files in Fasta, Phylip, or Nexus formats. The function will take in a string (RbString)
 * as an argument that indicates the file location or directory location. If the path is to a file, then
 * that file is read (or an attempt to read it is made). If the path is to a directory, then the program
 * attempts to read _all_ of the files in that directory, and does so recursively, burrowing into other
 * directories in that path and reading the files there too. The function attempts to figure out the file
 * format type (Nexus, Fasta, or Phylip), whether the data is interleaved or not (if the file is in Phylip
 * format), and the data type (if the file is in Fasta or Phylip format). Currently, the program can read the
 * following:
 *
 *            A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q 
 *    Nexus   Y   Y   Y   Y   Y   Y   Y   Y   Y   Y   N   N   N   N   N   N   N
 *    Phylip  Y   Y   Y   Y   N   Y   Y   Y   Y   N   N   N   N   N   N   N   N
 *    Fasta   Y   Y   Y   N   N   N   N   N   N   N   N   N   N   N   N   N   N
 *
 *    A: Aligned DNA (not interleaved); B: Aligned RNA (not interleaved); C: Aligned Amino Acid (not interleaved); 
 *    D: Standard (discrete) data (not interleaved); E: Continuous data (not interleaved);
 *    F: Aligned DNA (interleaved); G: Aligned RNA (interleaved); H: Aligned Amino Acid (interleaved); 
 *    I: Standard (discrete) data (interleaved); J: Continuous data (interleaved);
 *    K: Unaligned DNA (not interleaved); L: Unaligned RNA (not interleaved); M: Unaligned Amino Acid (not interleaved);
 *    O: Unaligned DNA (interleaved); P: Unaligned RNA (interleaved); Q: Unaligned Amino Acid (interleaved).
 * 
 * Note that some data formats do not support interleaving of data (such as Fasta) or reading of certain data types, so
 * the table is unlikely to complete with Yes's (Y) in all of the cells. However, Nexus and Fasta support
 * unaligned data for nucleotide and amino acid sequences. Eventually the program will support those data too.
 *
 * TODO: Implement the reading of unaligned data.
 *
 */
const std::string Func_readAlignment_name = "Read alignment function";

class Func_readAlignment :  public RbFunction {
    
    public:
        // Basic utility functions
        Func_readAlignment*         clone(void) const;                                                      //!< Clone the object
        const VectorString&         getClass(void) const;                                                   //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        RbLanguageObject*           execute(void);                                                          //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                                           //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                              //!< Get type of return value
    
    private:
        void                        formatError(RbFileManager& fm, std::string& errorStr);                  //!< Format the error string when (mis)reading files

        static const TypeSpec       typeSpec;
        static const TypeSpec       returnTypeSpec;
};

#endif

