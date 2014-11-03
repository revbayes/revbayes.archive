#include "AminoAcidState.h"
#include "BranchLengthTree.h"
#include "ConstantNode.h"
#include "ContinuousCharacterState.h"
#include "DiscreteCharacterData.h"
#include "DnaState.h"
#include "NewickConverter.h"
#include "NewickTreeReader.h"
#include "NclReader.h"
#include "RbErrorStream.h"
#include "RbFileManager.h"
#include "RnaState.h"
#include "StandardState.h"
#include "StringUtilities.h"
#include "TimeTree.h"
#include "Topology.h"
#include "TopologyNode.h"
#include "Tree.h"
#include "TreeUtilities.h"

using namespace RevBayesCore;

/** Constructs a tree from NCL */
void NclReader::constructBranchLengthTreefromNclRecursively(TopologyNode* tn, std::vector<TopologyNode*> &nodes, std::vector<double> &brlens, const NxsSimpleNode* tnNcl, const NxsTaxaBlock *tb) {
    
    // add the current node to the vector of nodes
    nodes.push_back( tn );
    
    // add the branch length for the node
    double edgeLength = tnNcl->GetEdgeToParent().GetDblEdgeLen();
    edgeLength = (edgeLength < 0 ? 0.0 : edgeLength);
    
    //    tn->setBranchLength( edgeLength );
    // We only store the branch length because we can only set it later once the entire tree is build
    brlens.push_back( edgeLength );
    
    // get the children
    std::vector<NxsSimpleNode*> children = tnNcl->GetChildren();
    
    // iterate over all children
    for (std::vector<NxsSimpleNode*>::iterator it = children.begin(); it!=children.end(); it++) {
        // create a new tree node with given name
        std::string name = (*it)->GetName();
        if ((*it)->IsTip()) {
            name = tb->GetTaxonLabel( (*it)->GetTaxonIndex() ).BlanksToUnderscores();
        }
        TopologyNode* child = new TopologyNode(name);
        
        // add new node as child
        tn->addChild(child);
        
        // add parent to new node
        child->setParent(tn);
        
        // recursive call for the child to parse the tree
        constructBranchLengthTreefromNclRecursively(child, nodes, brlens, *it, tb);
    }
}


/** Reads the blocks stored by NCL and converts them to RevBayes character matrices */
std::vector<AbstractCharacterData* > NclReader::convertFromNcl(const std::string& fileName)
{
    
	std::vector<AbstractCharacterData* > cmv;
    
	size_t numTaxaBlocks = nexusReader.GetNumTaxaBlocks();
	for (unsigned tBlck=0; tBlck<numTaxaBlocks; tBlck++)
    {
		NxsTaxaBlock* taxaBlock = nexusReader.GetTaxaBlock(tBlck);
		std::string taxaBlockTitle          = taxaBlock->GetTitle();
		const unsigned nCharBlocks          = nexusReader.GetNumCharactersBlocks(taxaBlock);
        const unsigned nUnalignedCharBlocks = nexusReader.GetNumUnalignedBlocks(taxaBlock);
        
        // make alignment objects
		for (unsigned cBlck=0; cBlck<nCharBlocks; cBlck++)
        {
			AbstractCharacterData* m = NULL;
			NxsCharactersBlock* charBlock = nexusReader.GetCharactersBlock(taxaBlock, cBlck);
			std::string charBlockTitle = taxaBlock->GetTitle();
			int dt = charBlock->GetDataType();
			if (dt == NxsCharactersBlock::dna || dt == NxsCharactersBlock::nucleotide)
            {
                m = createDnaMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::rna)
            {
                m = createRnaMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::protein)
            {
                m = createAminoAcidMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::standard)
            {
                m = createStandardMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::continuous)
            {
                m = createContinuousMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::mixed)
            {
                addWarning("Mixed data types are not allowed");
            }
            else
            {
                addWarning("Unknown data type");
            }
            
			if (m != NULL)
            {
                m->setFileName( StringUtilities::getLastPathComponent(fileName) );
                m->setFilePath( StringUtilities::getStringWithDeletedLastPathComponent(fileName) );
                
                unsigned int nAssumptions = nexusReader.GetNumAssumptionsBlocks(charBlock);
                if ( nAssumptions > 0 )
                {
                    for (unsigned int i = 0; i < nAssumptions; ++i)
                    {
                        NxsAssumptionsBlock *assumption = nexusReader.GetAssumptionsBlock(charBlock,i);
                        size_t nSets = assumption->GetNumCharSets();
                        NxsStringVector names;
                        assumption->GetCharSetNames(names);
                        for (size_t j = 0; j < nSets; ++j)
                        {
                            const NxsUnsignedSet *set = assumption->GetCharSet(names[j]);
                            AbstractCharacterData *m_tmp = m->clone();
                            m_tmp->excludeAllCharacters();
                            for (std::set<unsigned>::iterator k = set->begin(); k != set->end(); k++)
                            {
                                m_tmp->includeCharacter( *k );
                            }
                            cmv.push_back( m_tmp );
                            
                        }
                        
                    }
                }
                else
                {
                    cmv.push_back( m );
                }
                
            }
        }
        
        // create unaligned data objects
		for (unsigned cBlck=0; cBlck<nUnalignedCharBlocks; cBlck++)
        {
			AbstractCharacterData* m = NULL;
			NxsUnalignedBlock* charBlock = nexusReader.GetUnalignedBlock(taxaBlock, cBlck);
			std::string charBlockTitle = taxaBlock->GetTitle();
			int dt = charBlock->GetDataType();
			if (dt == NxsCharactersBlock::dna || dt == NxsCharactersBlock::nucleotide)
            {
                m = createUnalignedDnaMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::rna)
            {
                m = createUnalignedRnaMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::protein)
            {
                m = createUnalignedAminoAcidMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::standard)
            {
                //m = createStandardMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::continuous)
            {
                //m = createContinuousMatrix(charBlock);
            }
			else if (dt == NxsCharactersBlock::mixed)
            {
                addWarning("Mixed data types are not allowed");
            }
            else
            {
                addWarning("Unknown data type");
            }
            
			if (m != NULL)
            {
                m->setFileName( StringUtilities::getLastPathComponent(fileName) );
                m->setFilePath( StringUtilities::getStringWithDeletedLastPathComponent(fileName) );
				cmv.push_back( m );
            }
        }
        
    }
    
	return cmv;
}


/** Converts trees stored by NCL into RevBayes formatted trees */
std::vector<BranchLengthTree*>* NclReader::convertTreesFromNcl(void) {
	
	const unsigned nTaxaBlocks = nexusReader.GetNumTaxaBlocks();
	std::vector<BranchLengthTree*>* rbTreesFromFile = new std::vector<BranchLengthTree*>();
	for (unsigned t = 0; t < nTaxaBlocks; ++t) {
		const NxsTaxaBlock *tb = nexusReader.GetTaxaBlock(t);
		const unsigned nTreesBlocks = nexusReader.GetNumTreesBlocks(tb);
		if (nTreesBlocks == 0)
			continue;
        
		for (unsigned i = 0; i < nTreesBlocks; ++i) {
			const NxsTreesBlock * trb = nexusReader.GetTreesBlock(tb, i);
			trb->ProcessAllTrees();
			for (unsigned j = 0; j < trb->GetNumTrees(); ++j) {
				const NxsFullTreeDescription & ftd = trb->GetFullTreeDescription(j);
				NxsSimpleTree tree(ftd, -1, -1.0);
                //                tree.WriteAsNewick(std::cout, true, true, true, tb);
				BranchLengthTree* rbTree = translateNclSimpleTreeToBranchLengthTree(tree,tb);
                //                rbTree->fillNodeTimes();
                //                rbTree->equalizeBranchLengths();
				rbTreesFromFile->push_back( rbTree );
            }
        }
    }
    
	return rbTreesFromFile;
}

/** Create an object to hold aligned amino acid data */
DiscreteCharacterData<AminoAcidState>* NclReader::createAminoAcidMatrix(NxsCharactersBlock* charblock) {
    
    if ( charblock == NULL )
    {
        throw RbException("Trying to create an AA matrix from a NULL pointer.");
    }
    
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::protein)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (unsigned int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	size_t numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // instantiate the character matrix
	DiscreteCharacterData<AminoAcidState>* cMat = new DiscreteCharacterData<AminoAcidState>();
    cMat->setHomologyEstablished(true);
    
	// read in the data, including taxon names
	for (unsigned origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++)
    {
        // get the taxon name
        NxsString   tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tName  = NxsString::GetEscaped(tLabel).c_str();
        
        // allocate a vector of Standard states
        DiscreteTaxonData<AminoAcidState> dataVec = DiscreteTaxonData<AminoAcidState>(tName);
        
        for (NxsUnsignedSet::const_iterator cit = charset.begin(); cit != charset.end();cit++)
        {
            AminoAcidState aaState;
            if (charblock->IsGapState(origTaxIndex, *cit) == true)
            {
                aaState.setGapState(true);
                aaState.setState('-');
            }
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true)
            {
                aaState.setState('n');
            }
            else
            {
                size_t nStates = charblock->GetNumStates(origTaxIndex, *cit);
                aaState.setState( charblock->GetState(origTaxIndex, *cit, 0) );
                for(unsigned s=1; s<nStates; s++)
                    aaState.addState( charblock->GetState(origTaxIndex, *cit, s) );
            }
            dataVec.addCharacter( aaState );
        }
        
        // add sequence to character matrix
        cMat->addTaxonData( dataVec );
    }
    
    setExcluded( charblock, cMat );
    
    return cMat;
}

/* Create an object to hold unaligned aminoacid data */
DiscreteCharacterData<AminoAcidState>* NclReader::createUnalignedAminoAcidMatrix(NxsUnalignedBlock* charblock) {
    
    // check that the character block is of the correct type
	if ( charblock->GetDataType() != NxsCharactersBlock::protein )
        return NULL;
    
    // get the set of characters (and the number of taxa)
	unsigned numOrigTaxa = charblock->GetNTax();
    NxsTaxaBlockAPI* taxonBlock = charblock->GetTaxaBlockPtr();
    
    // instantiate the character matrix
	DiscreteCharacterData<AminoAcidState>* cMat = new DiscreteCharacterData<AminoAcidState>();
    cMat->setHomologyEstablished(false);
    
	// read in the data, including taxon names
	for (unsigned origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++)
    {
        // add the taxon name
        NxsString   tLabel = taxonBlock->GetTaxonLabel(origTaxIndex);
        std::string tName  = NxsString::GetEscaped(tLabel).c_str();
        
        // allocate a vector of amino acid states
        DiscreteTaxonData<AminoAcidState> dataVec = DiscreteTaxonData<AminoAcidState>(tName);
        
        // add the sequence information for the sequence associated with the taxon
        std::string rowDataAsString = charblock->GetMatrixRowAsStr(origTaxIndex);
        for (size_t i=0; i<rowDataAsString.size(); i++)
        {
            AminoAcidState aaState;
            aaState.setState(rowDataAsString[i]);
            dataVec.addCharacter( aaState );
        }
        
        // add sequence to character matrix
        cMat->addTaxonData( dataVec );
    }
    
    return cMat;
}

/** Create an object to hold continuous data */
ContinuousCharacterData* NclReader::createContinuousMatrix(NxsCharactersBlock* charblock) {
    
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::continuous)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (unsigned int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	unsigned numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // instantiate the character matrix
	ContinuousCharacterData* cMat = new ContinuousCharacterData();
    cMat->setHomologyEstablished(true);
    
	// read in the data, including taxon names
	for (unsigned origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++)
    {
        // add the taxon name
        NxsString   tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tName  = NxsString::GetEscaped(tLabel).c_str();
        
        // allocate a vector of Standard states
        ContinuousTaxonData dataVec = ContinuousTaxonData(tName);
        
        // add the real-valued observation
        for (NxsUnsignedSet::const_iterator cit = charset.begin(); cit != charset.end();cit++)
        {
            ContinuousCharacterState contObs ;
            const std::vector<double>& x = charblock->GetContinuousValues( origTaxIndex, *cit, std::string("AVERAGE") );
            contObs.setMean(x[0]);
            dataVec.addCharacter( contObs );
        }
        
        // add sequence to character matrix
        cMat->addTaxonData( dataVec );
    }
    
    setExcluded( charblock, cMat );
    
    return cMat;
}


/** Create an object to hold aligned DNA data */
DiscreteCharacterData<DnaState>* NclReader::createDnaMatrix(NxsCharactersBlock* charblock)
{
    
    if ( charblock == NULL )
    {
        throw RbException("Trying to create an DNA matrix from a NULL pointer.");
    }
    
    // check that the character block is of the correct type
	if ( charblock->GetDataType() != NxsCharactersBlock::dna )
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (unsigned int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	unsigned numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // instantiate the character matrix
	DiscreteCharacterData<DnaState>* cMat = new DiscreteCharacterData<DnaState>();
    cMat->setHomologyEstablished(true);
    
	// read in the data, including taxon names
	for (unsigned origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++)
    {
        // add the taxon name
        NxsString   tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tName  = NxsString::GetEscaped(tLabel).c_str();
        
        // allocate a vector of DNA states
        DiscreteTaxonData<DnaState> dataVec = DiscreteTaxonData<DnaState>(tName);
        
        // add the sequence information for the sequence associated with the taxon
        for (NxsUnsignedSet::iterator cit = charset.begin(); cit != charset.end(); cit++)
        {
            // add the character state to the matrix
            DnaState dnaState;
            if ( charblock->IsGapState(origTaxIndex, *cit) == true )
            {
                dnaState.setState('-');
                dnaState.setGapState(true);
            }
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true)
            {
                dnaState.setState('N');
            }
            else
            {
                dnaState.setState( charblock->GetState(origTaxIndex, *cit, 0) );
                for (unsigned int s=1; s<charblock->GetNumStates(origTaxIndex, *cit); s++)
                    dnaState.addState( charblock->GetState(origTaxIndex, *cit, s) );
            }
            dataVec.addCharacter( dnaState );
        }
        
        // add sequence to character matrix
        cMat->addTaxonData( dataVec );
    }
    
    setExcluded( charblock, cMat );
    
    return cMat;
}

/** Create an object to hold unaligned DNA data */
DiscreteCharacterData<DnaState>* NclReader::createUnalignedDnaMatrix(NxsUnalignedBlock* charblock) {
    
    // check that the character block is of the correct type
	if ( charblock->GetDataType() != NxsCharactersBlock::dna )
        return NULL;
    
    // get the set of characters (and the number of taxa)
	unsigned numOrigTaxa = charblock->GetNTax();
    NxsTaxaBlockAPI* taxonBlock = charblock->GetTaxaBlockPtr();
    
    // instantiate the character matrix
	DiscreteCharacterData<DnaState>* cMat = new DiscreteCharacterData<DnaState>();
    cMat->setHomologyEstablished(false);
    
	// read in the data, including taxon names
	for (unsigned origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++)
    {
        // add the taxon name
        NxsString   tLabel = taxonBlock->GetTaxonLabel(origTaxIndex);
        std::string tName  = NxsString::GetEscaped(tLabel).c_str();
        
        // allocate a vector of DNA states
        DiscreteTaxonData<DnaState> dataVec = DiscreteTaxonData<DnaState>(tName);
        
        // add the sequence information for the sequence associated with the taxon
        std::string rowDataAsString = charblock->GetMatrixRowAsStr(origTaxIndex);
        for (size_t i=0; i<rowDataAsString.size(); i++)
        {
            DnaState dnaState;
            dnaState.setState(rowDataAsString[i]);
            dataVec.addCharacter( dnaState );
        }
        
        // add sequence to character matrix
        cMat->addTaxonData( dataVec );
    }
    
    return cMat;
}

/** Create an object to hold aligned RNA data */
DiscreteCharacterData<RnaState>* NclReader::createRnaMatrix(NxsCharactersBlock* charblock) {
    
    // check that the character block is of the correct type
	if ( charblock->GetDataType() != NxsCharactersBlock::rna )
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (unsigned int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	unsigned numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // instantiate the character matrix
	DiscreteCharacterData<RnaState>* cMat = new DiscreteCharacterData<RnaState>();
    cMat->setHomologyEstablished(true);
    
	// read in the data, including taxon names
	for (unsigned origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++)
    {
        // add the taxon name
        NxsString   tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tName  = NxsString::GetEscaped(tLabel).c_str();
        
        // allocate a vector of RNA states
        DiscreteTaxonData<RnaState> dataVec = DiscreteTaxonData<RnaState>(tName);
        
        // add the sequence information for the sequence associated with the taxon
        for (NxsUnsignedSet::iterator cit = charset.begin(); cit != charset.end(); cit++)
        {
            // add the character state to the matrix
            RnaState rnaState;
            if ( charblock->IsGapState(origTaxIndex, *cit) == true )
            {
                rnaState.setGapState(true);
                rnaState.setState('-');
            }
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true)
            {
                rnaState.setState('N');
            }
            else
            {
                rnaState.setState( charblock->GetState(origTaxIndex, *cit, 0) );
                for (unsigned int s=1; s<charblock->GetNumStates(origTaxIndex, *cit); s++)
                    rnaState.addState( charblock->GetState(origTaxIndex, *cit, s) );
            }
            dataVec.addCharacter( rnaState );
        }
        
        // add sequence to character matrix
        cMat->addTaxonData( dataVec );
    }
    
    setExcluded( charblock, cMat );
    
    return cMat;
}

/** Create an object to hold unaligned RNA data */
DiscreteCharacterData<RnaState>* NclReader::createUnalignedRnaMatrix(NxsUnalignedBlock* charblock) {
    
    // check that the character block is of the correct type
	if ( charblock->GetDataType() != NxsCharactersBlock::rna )
        return NULL;
    
    // get the set of characters (and the number of taxa)
	unsigned numOrigTaxa = charblock->GetNTax();
    NxsTaxaBlockAPI* taxonBlock = charblock->GetTaxaBlockPtr();
    
    // instantiate the character matrix
	DiscreteCharacterData<RnaState>* cMat = new DiscreteCharacterData<RnaState>();
    cMat->setHomologyEstablished(false);
    
	// read in the data, including taxon names
	for (unsigned origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++)
    {
        // add the taxon name
        NxsString   tLabel = taxonBlock->GetTaxonLabel(origTaxIndex);
        std::string tName  = NxsString::GetEscaped(tLabel).c_str();
        
        // allocate a vector of DNA states
        DiscreteTaxonData<RnaState> dataVec = DiscreteTaxonData<RnaState>(tName);
        
        // add the sequence information for the sequence associated with the taxon
        std::string rowDataAsString = charblock->GetMatrixRowAsStr(origTaxIndex);
        for (size_t i=0; i<rowDataAsString.size(); i++)
        {
            RnaState rnaState;
            rnaState.setState(rowDataAsString[i]);
            dataVec.addCharacter( rnaState );
        }
        
        // add sequence to character matrix
        cMat->addTaxonData( dataVec );
    }
    
    return cMat;
}

/** Create an object to hold standard data */
DiscreteCharacterData<StandardState>* NclReader::createStandardMatrix(NxsCharactersBlock* charblock) {
    
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::standard)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (unsigned int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	unsigned numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // get the number of states
    const NxsDiscreteDatatypeMapper* mapper = charblock->GetDatatypeMapperForChar(0);
    std::string sym = charblock->GetSymbols();
    size_t nStates = mapper->GetNumStates();
    if (nStates > 10)
        return NULL;
    
    // instantiate the character matrix
	DiscreteCharacterData<StandardState>* cMat = new DiscreteCharacterData<StandardState>();
    cMat->setHomologyEstablished(true);
    
	// read in the data, including taxon names
	for (unsigned origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++)
    {
        // add the taxon name
        NxsString   tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tName  = NxsString::GetEscaped(tLabel).c_str();
        
        // allocate a vector of Standard states
        DiscreteTaxonData<StandardState> dataVec = DiscreteTaxonData<StandardState>(tName);
        
        // add the character information for the data associated with the taxon
        for (NxsUnsignedSet::iterator cit = charset.begin(); cit != charset.end(); cit++)
        {
            // add the character state to the matrix
            StandardState stdState = StandardState(sym);
            if ( charblock->IsGapState(origTaxIndex, *cit) == true )
            {
                stdState.setGapState(true);
                stdState.setState('-');
            }
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true)
            {
                stdState.setState('?');
            }
            else
                for(unsigned int s=0; s<charblock->GetNumStates(origTaxIndex, *cit); s++)
                {
                    stdState.setState( charblock->GetState(origTaxIndex, *cit, 0) );
                    for (unsigned int s=1; s<charblock->GetNumStates(origTaxIndex, *cit); s++)
                        stdState.addState( charblock->GetState(origTaxIndex, *cit, s) );
                }
            dataVec.addCharacter( stdState );
        }
        
        // add sequence to character matrix
        cMat->addTaxonData( dataVec );
    }
    
    setExcluded( charblock, cMat );
    
    return cMat;
}


/** Returns whether a file exists */
bool NclReader::fileExists(const char* fn) const {
    
	bool exists = false;
	FILE *fp = fopen(fn, "r");
	if (fp != NULL)
    {
		fclose(fp);
		exists = true;
    }
	return exists;
}


std::string NclReader::findFileNameFromPath(const std::string& fp) const {
    
    std::string::size_type pos = fp.find_last_of('/');
    if ( pos != std::string::npos )
    {
        std::string fn = fp.substr(pos+1, fp.size()-pos-1);
        return fn;
    }
    return "";
}


/** Attempt to determine the type of data this is being read */
std::string NclReader::intuitDataType(std::string& s) {
    
    // set up some strings containing the valid states for the different data types we are interested
    // in distiguishing
    static std::string dnaStates = "acgtmgrsvwyhkdbn-.?";
    static std::string rnaStates = "acgumgrsvwyhkdbn-.?";
    static std::string aaStates  = "arndcqeghilkmfpstwyv-.?";
    static std::string stdStates = "0123456789n-.?abcdefghijklmnopqrstuvwxyz()";
    static std::string nucStates = "acgtu";
    
    // and intialize a few variables we'll be needing
    size_t nucCount = 0, nMissing = 0;
    bool notDna = false, notRna = false, notAa = false, notStd = false;
    
    // loop over the string (s) that contains the raw data we look at the state and try to determine if the
    // state rules out certain data types
    StringUtilities::toLower( s );
    for (size_t i=0; i<s.size(); i++)
    {
        char c = s[i];
        
        if (c == 'n' || c == '-' || c == '.' || c == '?')
            nMissing++;
        
        if (notDna == false)
        {
            bool foundState = false;
            for (size_t j=0; j<19; j++)
            {
                if ( c == dnaStates[j] )
                {
                    foundState = true;
                    break;
                }
            }
            if (foundState == false)
                notDna = true;
        }
        
        if (notRna == false)
        {
            bool foundState = false;
            for (size_t j=0; j<19; j++)
            {
                if ( c == rnaStates[j] )
                {
                    foundState = true;
                    break;
                }
            }
            if (foundState == false)
                notRna = true;
        }
        
        if (notAa == false)
        {
            bool foundState = false;
            for (size_t j=0; j<23; j++)
            {
                if ( c == aaStates[j] )
                {
                    foundState = true;
                    break;
                }
            }
            if (foundState == false)
                notAa = true;
        }
        
        if (notStd == false)
        {
            bool foundState = false;
            for (size_t j=0; j<14; j++)
            {
                if ( c == stdStates[j] )
                {
                    foundState = true;
                    break;
                }
            }
            if (foundState == false)
                notStd = true;
        }
        
        for (size_t j=0; j<5; j++)
        {
            if ( c == nucStates[j] )
                nucCount++;
        }
    }
    
    // Try to determine which type of data it is. Note that using only the sequence information in the
    // string s that it is difficult or impossible to rule out certain data types. For example, if the
    // data is DNA, then the DNA states with the ambiguity codes do not rule out amino acid data. However,
    // we would expect a lot of A, C, G, and Ts in our data and not much else if the data is DNA, so we
    // use a bit of "fuzzy" logic to determine if the data is highly likely to be nucleotide, or not.
    if (notDna == false && notRna == true && notAa == true && notStd == true)
        return "dna";
    else if (notDna == true && notRna == false && notAa == true && notStd == true)
        return "rna";
    else if (notDna == true && notRna == true && notAa == false && notStd == true)
        return "protein";
    else if (notDna == true && notRna == true && notAa == true && notStd == false)
        return "standard";
    else if (notDna == false && notRna == true && notAa == false && notStd == true)
    {
        if ( (double)nucCount / (s.size()-nMissing) > 0.8 )
            return "dna";
        else
            return "protein";
    }
    else if (notDna == true && notRna == false && notAa == false && notStd == true)
    {
        if ( (double)nucCount / (s.size()-nMissing) > 0.8 )
            return "rna";
        else
            return "protein";
    }
    
    return "";
}


/** Try to determine if the file is likely to be in Fasta format */
bool NclReader::isFastaFile(std::string& fn, std::string& dType) {
    
    // open file
	std::ifstream fStrm;
    fStrm.open(fn.c_str(), ios::in);
    
    // read the file token-by-token looking for Fasta things
    int ch = fStrm.get();
    fStrm.unget();
    std::string word = "";
    std::string seqStr = "";
    int wordNum = 0, lineNum = 0, lastCarotLine = -100;
    int numSequences = 0;
    while (ch != EOF)
    {
        word = "";
        fStrm >> word;
        
        if (wordNum == 0 && word[0] == '>')
        {
            if (lineNum - lastCarotLine > 1)
            {
                lastCarotLine = lineNum;
                numSequences++;
            }
            else
                return false;
        }
        else if (wordNum == 0 && word[0] == ';')
        {
            // comment
        }
        else if (lineNum > 0 && word[0] != '>' && word[0] != ';')
            seqStr += word;
        
        wordNum++;
        ch = fStrm.get();
        if (ch == '\n' || ch == '\r' || ch == EOF)
        {
            lineNum++;
            wordNum = 0;
        }
    }
    
    // close file
    fStrm.close();
    
    if (numSequences < 1)
        return false;
    
    dType = intuitDataType(seqStr);
    
    return true;
}


/**
 * Try to determine if the file is likely to be in Nexus format. We check if the first word in the
 * file is #NEXUS. If not, then we check if the file name ending is ".nex". If neither is true, it
 * is probably not a NEXUS file.
 */
bool NclReader::isNexusFile(const std::string& fn) {
    
    // open file, read first word, close file
	std::ifstream fStrm;
    fStrm.open(fn.c_str(), ios::in);
    std::string word;
    fStrm >> word;
    fStrm.close();
    
    if (word=="#NEXUS")
        return true;
    else {
        size_t found = fn.find_last_of(".");
        if ( found != std::string::npos && fn.substr(found+1) == "nex" )
            return true;
        else
            return false;
    }
}


/** Try to determine if the file is likely to be in Phylip format */
bool NclReader::isPhylipFile(std::string& fn, std::string& dType, bool& isInterleaved) {
    
    // open file
	std::ifstream fStrm;
    fStrm.open(fn.c_str(), ios::in);
    std::string seqStr = "";
    
    // read the file token-by-token looking for NEXUS things
    bool foundNumTaxa = false, foundNumChar = false;
    unsigned int numTaxa = 0;
    std::vector<std::string> taxonNames;
    int ch = fStrm.get();
    fStrm.unget();
    std::string word = "";
    int wordNum = 0, lineNum = 0;
    while (ch != EOF)
    {
        word = "";
        fStrm >> word;
        StringUtilities::toLower( word );
        
        if (lineNum == 0 && wordNum == 0 && StringUtilities::isNumber(word) == true)
        {
            std::istringstream buf(word);
            buf >> numTaxa;
            foundNumTaxa = true;
        }
        else if (lineNum == 0 && wordNum == 1 && StringUtilities::isNumber(word) == true)
            foundNumChar = true;
        else if (lineNum > 0 && wordNum == 0 && word != "" && word.size() < 25)
            taxonNames.push_back( word );
        else if (lineNum > 0 && wordNum > 0)
            seqStr += word;
        
        wordNum++;
        ch = fStrm.get();
        if (ch == '\n' || ch == '\r' || ch == EOF)
        {
            lineNum++;
            wordNum = 0;
        }
    }
    
    // close file
    fStrm.close();
    
    if (foundNumTaxa == true && foundNumChar == true)
    {
        if (taxonNames.size() == 0)
            return false;
        if (taxonNames.size() % numTaxa != 0)
            return false;
        
        if (taxonNames.size() > numTaxa)
            isInterleaved = true;
        dType = intuitDataType(seqStr);
        return true;
    }
    
    return false;
}


std::vector<AbstractCharacterData*> NclReader::readMatrices(const std::string &fn) {
    
    // check that the file/path name has been correctly specified
    RbFileManager myFileManager( fn );
    if ( myFileManager.getFileName() == "" && myFileManager.getFilePath() == "" )
    {
        std::string errorStr = "";
        myFileManager.formatError(errorStr);
        throw RbException("Could not find file or path with name \"" + fn + "\"");
    }
    
    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = false;
    if ( myFileManager.getFilePath() != "" && myFileManager.getFileName() == "")
        readingDirectory = true;
    if (readingDirectory == true)
        RBOUT( "Recursively reading the contents of a directory\n" );
    else
        RBOUT( "Attempting to read the contents of file \"" + myFileManager.getFileName() + "\"\n" );
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if (readingDirectory == true)
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
    else
    {
#       if defined (WIN32)
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "\\" + myFileManager.getFileName() );
#       else
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "/" + myFileManager.getFileName() );
#       endif
    }
    if (readingDirectory == true)
    {
        std::stringstream o1;
        o1 << "Found " << vectorOfFileNames.size() << " files in directory";
        RBOUT( o1.str() );
    }
    
    // clear warnings from warnings buffer
    clearWarnings();
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
    {
        bool isInterleaved = false;
        std::string myFileType = "unknown", dType = "unknown";
        if (isNexusFile(*p) == true)
            myFileType = "nexus";
        else if (isPhylipFile(*p, dType, isInterleaved) == true)
            myFileType = "phylip";
        else if (isFastaFile(*p, dType) == true)
            myFileType = "fasta";
        
        if (myFileType != "unknown")
        {
            std::string suffix = "|" + dType;
            if ( myFileType == "phylip" )
            {
                if (isInterleaved == true)
                    suffix += "|interleaved";
                else
                    suffix += "|noninterleaved";
            }
            else if ( myFileType == "fasta" )
                suffix += "|noninterleaved";
            else
                suffix += "|unknown";
            myFileType += suffix;
            fileMap.insert( std::make_pair(*p,myFileType) );
        }
        else
        {
            addWarning("Unknown file type");
        }
    }
    
    // read the files in the map containing the file names with the output being a vector of pointers to
    // the character matrices that have been read
    std::vector<AbstractCharacterData*> m = readMatrices( fileMap );
    
    // print summary of results of file reading to the user
    if (readingDirectory == true)
    {
        std::stringstream o2;
        if ( m.size() == 0 )
            o2 << "Failed to read any files";
        else if ( m.size() == 1 )
            o2 << "Successfully read one file";
        else
            o2 << "Successfully read " << m.size() << " files";
        RBOUT( o2.str() );
        std::set<std::string> myWarnings = getWarnings();
        if ( vectorOfFileNames.size() - m.size() > 0 && myWarnings.size() > 0 )
        {
            std::stringstream o3;
            if (vectorOfFileNames.size() - m.size() == 1)
                o3 << "Did not read a file for the following ";
            else
                o3 << "Did not read " << vectorOfFileNames.size() - m.size() << " files for the following ";
            if (myWarnings.size() == 1)
                o3 << "reason:";
            else
                o3 << "reasons:";
            RBOUT( o3.str() );
            for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
                RBOUT( "* "+(*it) );
        }
    }
    else
    {
        if (m.size() > 0)
            RBOUT( "Successfully read file" );
        else
        {
            std::set<std::string> myWarnings = getWarnings();
            if ( myWarnings.size() > 0 )
            {
                std::stringstream o3;
                o3 << "Did not read the file for the following ";
                if (myWarnings.size() == 1)
                    o3 << "reason:";
                else
                    o3 << "reasons:";
                RBOUT( o3.str() );
                for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
                    RBOUT( "* "+(*it) );
            }
        }
    }
    
    return m;
}

/** Read a list of file names contained in a map (with file format info too) */
std::vector<AbstractCharacterData *> NclReader::readMatrices(const std::string &fn, const std::string &ft) {
    
    // instantiate a vector of matrices
    std::vector<AbstractCharacterData* > cmv;
    
    // The data types are as follows: Nexus, Phylip+DNA/RNA/AA/Standard+Interleaved/Noninterleaved,
    // Fasta+DNA/RNA/AA.
    
    // Check that the file exists. It is likely that this has been already checked during the formation of
    // the map that is passed into the function, but it never hurts to be safe...
    if ( !fileExists(fn.c_str()) ){
        addWarning("Data file not found");
    }
    else {
        // Extract information on the file format from the value of the key/value pair. Note that we expect the
        // fileFmt string to be in the format file_type|data_type|interleave_type with pipes ('|') separating
        // the format components. It might be better to make an object value in the key/value pair that contains
        // this information.
        std::vector<std::string> fileFmt;
        StringUtilities::stringSplit( ft, "|", fileFmt );
        std::string ff = fileFmt[0];
        std::string dt = fileFmt[1];
        bool il = false;
        if ( fileFmt[2] == "interleaved" )
            il = true;
        
        // read the file
        cmv = readMatrices( fn.c_str(), ff, dt, il );
		nexusReader.ClearContent();
    }
    
    return cmv;
}


/** Read a list of file names contained in a map (with file format info too) */
std::vector<AbstractCharacterData *> NclReader::readMatrices(const std::map<std::string,std::string>& fileMap) {
    
    // instantiate a vector of matrices
    std::vector<AbstractCharacterData* > cmv;
    
    // We loop through the map that contains as the key the path and name of the file to be read and as the
    // value the data type. The data types are as follows: Nexus, Phylip+DNA/RNA/AA/Standard+Interleaved/Noninterleaved,
    // Fasta+DNA/RNA/AA.
    for (std::map<std::string,std::string>::const_iterator p = fileMap.begin(); p != fileMap.end(); p++)
    {
        // Check that the file exists. It is likely that this has been already checked during the formation of
        // the map that is passed into the function, but it never hurts to be safe...
        if ( !fileExists(p->first.c_str()) )
        {
            addWarning("Data file not found");
            continue;
        }
        
        // Extract information on the file format from the value of the key/value pair. Note that we expect the
        // fileFmt string to be in the format file_type|data_type|interleave_type with pipes ('|') separating
        // the format components. It might be better to make an object value in the key/value pair that contains
        // this information.
        std::vector<std::string> fileFmt;
        StringUtilities::stringSplit( p->second, "|", fileFmt );
        std::string ff = fileFmt[0];
        std::string dt = fileFmt[1];
        bool il = false;
        if ( fileFmt[2] == "interleaved" )
            il = true;
        
        // read the file
        std::vector<AbstractCharacterData* > v = readMatrices( p->first.c_str(), ff, dt, il );
		if (v.size() > 0)
        {
			for (std::vector<AbstractCharacterData*>::iterator m = v.begin(); m != v.end(); m++)
				cmv.push_back( (*m) );
        }
		nexusReader.ClearContent();
    }
    
    return cmv;
}


/** Read a list of file names contained in a vector of strings */
std::vector<AbstractCharacterData*> NclReader::readMatrices(const std::vector<std::string> fn, const std::string fileFormat, const std::string dataType, const bool isInterleaved) {
    
	// instantiate a vector of matrices
	std::vector<AbstractCharacterData*> cmv;
    
	// check that the files exist
    for (std::vector<std::string>::const_iterator f = fn.begin(); f != fn.end(); f++)
    {
        if ( !fileExists((*f).c_str()) )
        {
            addWarning("Data file not found");
            return cmv;
        }
    }
    
    // read the data files
    for (std::vector<std::string>::const_iterator f = fn.begin(); f != fn.end(); f++)
    {
        std::vector<AbstractCharacterData*> v = readMatrices( (*f).c_str(), fileFormat, dataType, isInterleaved );
		if (v.size() > 0)
        {
			for (std::vector<AbstractCharacterData*>::iterator m = v.begin(); m != v.end(); m++)
				cmv.push_back( (*m) );
        }
		nexusReader.ClearContent();
    }
    
    return cmv;
}


/** Reads a single file using NCL */
std::vector<AbstractCharacterData*> NclReader::readMatrices(const char* fileName, const std::string fileFormat, const std::string dataType, const bool isInterleaved) {
    
    // check that the file exists
	if ( !fileExists(fileName) )
    {
        addWarning("Data file not found");
        std::vector<AbstractCharacterData*> dummy;
        return dummy;
    }
	try
    {
		if (fileFormat == "nexus")
        {
			// NEXUS file format
			nexusReader.ReadFilepath(fileName, MultiFormatReader::NEXUS_FORMAT);
        }
		else if (fileFormat == "fasta")
        {
			// fasta file format
			if (dataType == "dna")
				nexusReader.ReadFilepath(fileName, MultiFormatReader::FASTA_DNA_FORMAT);
			else if (dataType == "rna")
				nexusReader.ReadFilepath(fileName, MultiFormatReader::FASTA_RNA_FORMAT);
			else if (dataType == "protein")
				nexusReader.ReadFilepath(fileName, MultiFormatReader::FASTA_AA_FORMAT);
        }
		else if (fileFormat == "phylip")
        {
			// phylip file format
			if (isInterleaved == false)
            {
				if (dataType == "dna")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_DNA_FORMAT);
				else if (dataType == "rna")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_RNA_FORMAT);
				else if (dataType == "protein")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_AA_FORMAT);
				else if (dataType == "standard")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_DISC_FORMAT);
            }
			else
            {
				if (dataType == "dna")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::INTERLEAVED_RELAXED_PHYLIP_DNA_FORMAT);
				else if (dataType == "rna")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::INTERLEAVED_RELAXED_PHYLIP_RNA_FORMAT);
				else if (dataType == "protein")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::INTERLEAVED_RELAXED_PHYLIP_AA_FORMAT);
				else if (dataType == "standard")
					nexusReader.ReadFilepath(fileName, MultiFormatReader::INTERLEAVED_RELAXED_PHYLIP_DISC_FORMAT);
            }
        }
    }
	catch(NxsException err)
    {
        std::string fns = fileName;
        
        if ( err.msg.length() == 0 )
            // Basic error message if ncl fails to give something back
            addWarning("Nexus error in file \"" + StringUtilities::getLastPathComponent(fns) + "\"");
        else
            // NxsReader error message
            addWarning(err.msg);
        
        // Position information
        std::stringstream errorMessage;
        errorMessage << "The error occurred while reading line ";
        errorMessage << err.line << " column " << err.col;
        errorMessage << " in file \"" << StringUtilities::getLastPathComponent(fns) << "\"";
        addWarning(errorMessage.str());
        
        // Return empty character matrix vector
        std::vector<AbstractCharacterData*> dummy;
		return dummy;
    }
    
	std::string fn = fileName;
	std::vector<AbstractCharacterData*> cvm = convertFromNcl(fn);
	return cvm;
}


/** Read trees */
std::vector<BranchLengthTree*>* NclReader::readBranchLengthTrees(const std::string &fn)
{
    
	nexusReader.ClearContent();
    
    // check that the file/path name has been correctly specified
    RbFileManager myFileManager( fn );
    if ( (myFileManager.getFileName() == "" && myFileManager.getFilePath() == "") || myFileManager.testFile() == false )
    {
        std::string errorStr = "";
        myFileManager.formatError(errorStr);
        throw RbException("Could not find file or path with name \"" + fn + "\"");
    }
    
    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = false;
    if ( myFileManager.getFilePath() != "" && myFileManager.getFileName() == "")
    {
        readingDirectory = true;
    }
    if (readingDirectory == true)
    {
        RBOUT( "Recursively reading the contents of a directory" );
    }
    else
    {
        RBOUT( "Attempting to read the contents of file \"" + myFileManager.getFileName() + "\"" );
    }
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if (readingDirectory == true)
    {
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
    }
    else
    {
        std::string filepath = myFileManager.getFilePath();
        if ( filepath != "" )
        {
#           if defined (WIN32)
            filepath += "\\";
#           else
            filepath += "/";
#           endif
        }
        filepath += myFileManager.getFileName();
        vectorOfFileNames.push_back( filepath );
    }
    if (readingDirectory == true)
    {
        std::stringstream o1;
        o1 << "Found " << vectorOfFileNames.size() << " files in directory";
        RBOUT( o1.str() );
    }
    
    // clear warnings from its warnings buffer
    clearWarnings();
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::vector<BranchLengthTree*> *trees = NULL;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
    {
        // we should check here the file type first and make sure it is valid
        
        // read the files in the map containing the file names with the output being a vector of pointers to
        // the character matrices that have been read
        trees = readBranchLengthTrees( p->c_str(), "nexus" );
        if ( trees == NULL || trees->size() == 0 )
        {
            delete trees;
            trees = NULL;
            try
            {
                trees = readBranchLengthTrees( p->c_str(), "phylip" );
            }
            catch (RbException e)
            {
                ;
            }
            catch (exception e)
            {
                ;
            }
        }
        
        if ( trees == NULL || trees->size() == 0 )
        {
            delete trees;
            trees = readBranchLengthTrees( p->c_str(), "newick" );
        }
        
    }
    
    // print summary of results of file reading to the user
    if (readingDirectory == true)
    {
        std::stringstream o2;
        std::size_t size = 0;
        if ( trees == NULL || trees->size() == 0 )
        {
            o2 << "Failed to read any tree";
        }
        else if ( trees->size() == 1 )
        {
            size = trees->size();
            o2 << "Successfully read one tree";
        }
        else
        {
            size = trees->size();
            o2 << "Successfully read " << trees->size() << " trees";
        }
        RBOUT( o2.str() );
        std::set<std::string> myWarnings = getWarnings();
        if ( vectorOfFileNames.size() - size > 0 && myWarnings.size() > 0 ) {
            std::stringstream o3;
            if (vectorOfFileNames.size() - size == 1)
            {
                o3 << "Did not read a file for the following ";
            }
            else
            {
                o3 << "Did not read " << vectorOfFileNames.size() - size << " files for the following ";
            }
            if (myWarnings.size() == 1)
            {
                o3 << "reason:";
            }
            else
            {
                o3 << "reasons:";
            }
            RBOUT( o3.str() );
            for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
            {
                RBOUT( "* "+(*it) );
            }
        }
    }
    else
    {
        if ( trees != NULL && trees->size() > 0)
        {
            RBOUT( "Successfully read file" );
        }
        else
        {
            std::set<std::string> myWarnings = getWarnings();
            if ( myWarnings.size() > 0 )
            {
                std::stringstream o3;
                o3 << "Did not read the file for the following ";
                if (myWarnings.size() == 1)
                {
                    o3 << "reason:";
                }
                else
                {
                    o3 << "reasons:";
                }
                RBOUT( o3.str() );
                for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
                {
                    RBOUT( "* " + (*it) );
                }
            }
        }
    }
    
    return trees;
    
}


/** Read trees */
std::vector<BranchLengthTree*>* NclReader::readBranchLengthTrees(const char *fileName, const std::string &fileFormat)
{
	
	// check that the file exists
	if ( !fileExists(fileName) )
    {
        addWarning("Data file not found");
        return NULL;
    }
	
	try
    {
		if (fileFormat == "nexus")
        {
			// NEXUS file format
			nexusReader.ReadFilepath(fileName, MultiFormatReader::NEXUS_FORMAT);
        }
		else if (fileFormat == "phylip")
        {
			// phylip file format with long taxon names
			nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_TREE_FORMAT);
        }
        else if (fileFormat == "newick")
        {
            std::string fn(fileName);
            NewickTreeReader ntr;
            std::vector<BranchLengthTree*>* trees = ntr.readBranchLengthTrees(fn);
            return trees;
        }
    }
	catch(NxsException err)
    {
        std::stringstream o;
		o << "Nexus Error: " << err.msg << " (" << err.pos << ", " << err.line << ", " << err.col << ")";
        throw RbException( o.str() );
        return NULL;
    }
	
	std::vector<std::string> fileNameVector;
	std::string str = fileName;
	fileNameVector.push_back( str );
	
	std::vector<BranchLengthTree*>* cvm = convertTreesFromNcl();
    
	return cvm;
}


std::vector<TimeTree*> NclReader::readTimeTrees( const std::string &treeFilename )
{
    
    std::vector<TimeTree*> trees;
    std::vector<BranchLengthTree*> *m = readBranchLengthTrees( treeFilename );
    
    if (m != NULL)
    {
        for (std::vector<BranchLengthTree*>::iterator it = m->begin(); it != m->end(); it++)
        {
            TimeTree* convertedTree = TreeUtilities::convertTree( *(*it) );
            delete (*it);
            trees.push_back( convertedTree );
        }
    }
    
    return trees;
}

std::vector<AdmixtureTree*> NclReader::readAdmixtureTrees(const std::string &treeFileName)
{
    std::vector<AdmixtureTree*> adm_trees;
    std::vector<BranchLengthTree*>* m = readBranchLengthTrees(treeFileName);
    std::vector<BranchLengthTree*>::iterator it;
    
    std::vector<std::string> names;
    if (m != NULL) {
        for (std::vector<BranchLengthTree*>::iterator it = m->begin(); it != m->end(); it++) {
            AdmixtureTree* convertedTree = TreeUtilities::convertToAdmixtureTree( *(*it), names);
            delete (*it);
            adm_trees.push_back( convertedTree );
        }
    }
    
    return adm_trees;
}


/** Set excluded characters and taxa */
void NclReader::setExcluded( const NxsCharactersBlock* charblock, AbstractCharacterData* cMat ) const {
    
    // Set excluded taxa
    for (unsigned int origTaxIndex=0; origTaxIndex<charblock->GetNTax(); origTaxIndex++ ) {
		if ( !charblock->IsActiveTaxon( origTaxIndex ) )
            cMat->excludeTaxon( origTaxIndex );
    }
    
    // Set excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    for ( NxsUnsignedSet::const_iterator cit = excluded.begin(); cit != excluded.end(); cit++ )
        cMat->excludeCharacter( *cit );
}


/** Translate a single NCL tree into a RevBayes tree */
BranchLengthTree* NclReader::translateNclSimpleTreeToBranchLengthTree(NxsSimpleTree& nTree, const NxsTaxaBlock *tb) {
    
    // get the root from the ncl tree
    const NxsSimpleNode* rn = nTree.GetRootConst();
    
    // create a new tree root node
    const std::string& name = rn->GetName();
    TopologyNode* root = new TopologyNode(name);
    
    // create a map which holds for each node a map of name value pairs.
    //    std::map<const TopologyNode*, std::map<std::string, RevObject*> > nodeParameters;
    
    // the vector of nodes so that we remember the ordering (and matching) which node belongs to which parameter
    std::vector<TopologyNode*> nodes;
    
    
    // the vector of branch lengths that we use later to set the branch length in the tree object
    std::vector<double> brlens;
    
	// construct tree recursively
    constructBranchLengthTreefromNclRecursively(root, nodes, brlens, rn, tb);
    
    // create a new simple tree
    BranchLengthTree* myTreeFromNcl = new BranchLengthTree();
    
    // create the topology object
    Topology *tau = new Topology();
    
    // initialize the topology by setting the root
    tau->setRoot(root);
    
    // connect the tree with the topology
    myTreeFromNcl->setTopology( tau, true );
    
    // finally set the branch lengths
    for ( size_t i = 0; i < nodes.size(); ++i )
    {
        myTreeFromNcl->setBranchLength(nodes[i]->getIndex(), brlens[i] );
    }
    
	return myTreeFromNcl;
    
}
