/**
 * @file
 * This file contains the declaration of NclReader, which is the wrapper class for the NCL
 * for reading character matrices, sequences, and trees.
 
 * @brief Declaration of NclReader
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#ifndef NclReader_H
#define NclReader_H

#include "AminoAcidState.h"
#include "AbstractCharacterData.h"
#include "ContinuousCharacterData.h"
#include "ContinuousCharacterState.h"
#include "DiscreteCharacterData.h"
#include "DnaState.h"
#include "ncl.h"
#include "nxsmultiformat.h"
#include "RbFileManager.h"
#include "RnaState.h"
#include "StandardState.h"
#include "TopologyNode.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class Tree;
    class BranchLengthTree;
    class TimeTree;
    class AdmixtureTree;
    
    class NclReader{
        
        friend class ::NxsBlock;
        friend class ::NxsUnalignedBlock;
        
    public:
        
        void                                        addWarning(std::string s) { warningsSummary.insert(s); }                        //!< Add a warning to the warnings vector
        void                                        clearWarnings(void) { warningsSummary.clear(); }                                //!< Clear all of the warnings from the warnings vector
        size_t                                      getNumWarnings(void) { return warningsSummary.size(); }                         //!< Return the number of warnings
        std::set<std::string>&                      getWarnings(void) { return warningsSummary; }                                   //!< Get a reference to the warnings vector
        static NclReader&                           getInstance(void);                                                              //!< Get a reference to this singleton class
        void                                        clearContent(void) { nexusReader.ClearContent(); }                              //!< Clear the content of the NCL object
        
        // file type methods
        bool                                        isFastaFile(std::string& fn, std::string& dType);                               //!< Checks if the file is in Fasta format
        bool                                        isNexusFile(const std::string& fn);                                             //!< Checks if the file is in NEXUS format
        bool                                        isPhylipFile(std::string& fn, std::string& dType, bool& isInterleaved);         //!< Checks if the file is in Phylip format
        
        // alignment stuff
        std::vector<AbstractCharacterData* >        readMatrices(const std::string &fn);
        std::vector<AbstractCharacterData* >        readMatrices(const std::string &fn, const std::string &ft);                     //!< Read character data from a single file (with file format info too)
        std::vector<AbstractCharacterData* >        readMatrices(const std::map<std::string,std::string>& fileMap);                 //!< Read a list of file names contained in a map (with file format info too)
        std::vector<AbstractCharacterData* >        readMatrices(const std::vector<std::string> fn, const std::string fileFormat, const std::string dataType, const bool isInterleaved);              //!< Read a list of file names contained in a vector of strings
        
        // stuff for reading trees
        std::vector<BranchLengthTree* >*            readBranchLengthTrees(const std::string &fn);                                   //!< Read trees
        std::vector<TimeTree*>                      readTimeTrees(const std::string &treeFilename);
        std::vector<AdmixtureTree* >                readAdmixtureTrees(const std::string &treeFileName);
        
    private:
        NclReader(void) { }                                                             //!< Default constructor
        NclReader(const NclReader& r) { }                                               //!< Copy constructor
        virtual                                    ~NclReader(void) { }                                                             //!< Destructor
        
        DiscreteCharacterData<AminoAcidState>*      createAminoAcidMatrix(NxsCharactersBlock* charblock);                           //!< Create an object to hold amino acid data
        ContinuousCharacterData*                    createContinuousMatrix(NxsCharactersBlock* charblock);                          //!< Create an object to hold continuous data
        DiscreteCharacterData<DnaState>*            createDnaMatrix(NxsCharactersBlock* charblock);                                 //!< Create an object to hold DNA data
        DiscreteCharacterData<RnaState>*            createRnaMatrix(NxsCharactersBlock* charblock);                                 //!< Create an object to hold RNA data
        DiscreteCharacterData<StandardState>*       createStandardMatrix(NxsCharactersBlock* charblock);                            //!< Create an object to hold standard data
        DiscreteCharacterData<AminoAcidState>*      createUnalignedAminoAcidMatrix(NxsUnalignedBlock* charblock);                   //!< Create an object to hold amino acid data
        DiscreteCharacterData<DnaState>*            createUnalignedDnaMatrix(NxsUnalignedBlock* charblock);                         //!< Create an object to hold DNA data
        DiscreteCharacterData<RnaState>*            createUnalignedRnaMatrix(NxsUnalignedBlock* charblock);                         //!< Create an object to hold RNA data
        bool                                        fileExists(const char *fn) const;                                               //!< Returns whether a file exists
        std::string                                 findFileNameFromPath(const std::string& fp) const;                              //!< Returns the file name from a file path
        std::string                                 intuitDataType(std::string& s);                                                 //!< Attempt to determine the type of data
        
        // methods for reading sequence alignments
        std::vector<AbstractCharacterData* >        convertFromNcl(const std::string& fileName);                                    //!< Reads the blocks stored by NCL and converts them to RevBayes character matrices
        std::vector<AbstractCharacterData* >        readMatrices(const char* fileName, const std::string fileFormat, const std::string dataType, const bool isInterleaved);                          //!< Reads a single file using NCL
        void                                        setExcluded(const NxsCharactersBlock* charblock, AbstractCharacterData* cMat ) const;       //!< Set excluded taxa and excluded characters
        
        // methods for reading trees
        void                                        constructBranchLengthTreefromNclRecursively(TopologyNode* tn, std::vector<TopologyNode*> &nodes, std::vector<double> &brlens, const NxsSimpleNode* tnNcl, const NxsTaxaBlock *tb);  //!< Constructs a tree from NCL
                                                                                                                                                                                                                                        //void                                      constructTreefromNclRecursively(TopologyNode* tn, const NxsSimpleNode* tnNcl, const NxsTaxaBlock *tb);  //!< Constructs a tree from NCL
        std::vector<BranchLengthTree* >*            readBranchLengthTrees(const char *fn, const std::string &fileFormat);           //!< Read trees
                                                                                                                                    //        void            readBranchLengthTrees(const char* fileName, const std::string fileFormat);      //!< Reads trees contained in a file
        std::vector<BranchLengthTree* >*            convertTreesFromNcl(void);                                                      //!< Converts trees stored by NCL into RevBayes formatted trees
        BranchLengthTree*                           translateNclSimpleTreeToBranchLengthTree(NxsSimpleTree &nTree,const NxsTaxaBlock *tb);  //!< Translate a single NCL tree into a RevBayes tree
        
        MultiFormatReader                           nexusReader;                                                                    //!< The NCL object that reads the files
        std::set<std::string>                       warningsSummary;                                                                //!< A vector that contains the warnings that acumulate
    };
    
}

#endif
