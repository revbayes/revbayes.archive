/**
 * @file
 * This file contains the declaration of NclReader, which is the wrapper class for the ncl library
 * for reading character matrices, sequences and trees.
 
 * @brief Declaration of NclReader
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id:$
 */



#ifndef NclReader_H
#define NclReader_H

#include <string>
#include <vector>
#include "ncl.h"
#include "nxsmultiformat.h"

class CharacterMatrix;
class Tree;

class NclReader{
    
	friend class NxsBlock;
	
public:
    NclReader(void);
    virtual                            ~NclReader(void);
    
    static NclReader&                   getInstance(void);
    std::vector<CharacterMatrix*>*      convertFromNcl(std::vector<std::string>& fnv);
    bool                                fileExists(const char *fn) const;
    std::vector<CharacterMatrix*>*      readData(const std::vector<std::string> fn, const std::string fileFormat, const std::string dataType, const bool isInterleaved);
    std::vector<CharacterMatrix*>*      readData(const char* fileName, const std::string fileFormat, const std::string dataType, const bool isInterleaved);
    
    // TAH: stuff for reading trees
    std::vector<Tree*>*                 readTree(const std::string fn, const std::string fileFormat);
    std::vector<Tree*>*                 readTree(const char* fileName, const std::string fileFormat);
    std::vector<Tree*>*                 convertTreeFromNcl();
    Tree*                               translateNclSimpleTreeToTree(NxsSimpleTree &nTree);
    
    void                                clearContent() { nexusReader.ClearContent(); }
    
private:
    MultiFormatReader                   nexusReader;
    CharacterMatrix*                    createAminoAcidMatrix(NxsCharactersBlock* charblock);
    CharacterMatrix*                    createContinuousMatrix(NxsCharactersBlock* charblock);
    CharacterMatrix*                    createNucleotideMatrix(NxsCharactersBlock* charblock);
    CharacterMatrix*                    createStandardMatrix(NxsCharactersBlock* charblock);
};

#endif