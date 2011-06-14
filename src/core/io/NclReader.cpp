#include "CharacterMatrixContinuous.h"
#include "CharacterMatrixDiscrete.h"
#include "NclReader.h"
#include "Tree.h"
#include "TreeNode.h"
#include "TreeSimple.h"



NclReader::NclReader(void) {
    
}

NclReader::~NclReader(void) {
    
}


void NclReader::constructTreefromNclRecursively(TreeNode *tn, const NxsSimpleNode *tnNcl) {
    // get the children
    std::vector<NxsSimpleNode*> children = tnNcl->GetChildren();
    
    // iterate over all children
    for (std::vector<NxsSimpleNode*>::iterator it = children.begin(); it!=children.end(); it++) {
        // create a new tree node with given name
        const std::string& name = (*it)->GetName(); 
        TreeNode* child = new TreeNode(name);
        
        // add new node as child
        tn->addChild(child);
        
        // add parent to new node
        child->setParent(tn);
        
        // recursive call for the child to parse the tree
        constructTreefromNclRecursively(child, *it);
    }
}

std::vector<CharacterMatrix*>* NclReader::convertFromNcl(std::vector<std::string>& fnv) {
    
	std::vector<CharacterMatrix*>* cmv = new std::vector<CharacterMatrix*>();
    
	int numTaxaBlocks = nexusReader.GetNumTaxaBlocks();
	int k = 0;
	for (int tBlck=0; tBlck<numTaxaBlocks; tBlck++)
    {
		NxsTaxaBlock* taxaBlock = nexusReader.GetTaxaBlock(tBlck);
		std::string taxaBlockTitle = taxaBlock->GetTitle();
		const unsigned nCharBlocks = nexusReader.GetNumCharactersBlocks(taxaBlock);
		for (unsigned cBlck=0; cBlck<nCharBlocks; cBlck++)
        {
			CharacterMatrix* m = NULL;
			NxsCharactersBlock* charBlock = nexusReader.GetCharactersBlock(taxaBlock, cBlck);
			std::string charBlockTitle = taxaBlock->GetTitle();
			int dt = charBlock->GetDataType();
			if (dt == NxsCharactersBlock::dna || dt == NxsCharactersBlock::rna || dt == NxsCharactersBlock::nucleotide)
            {
                m = createNucleotideMatrix(charBlock);
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
				std::cerr << "Error: Mixed data types are no longer allowed" << std::endl;
            }
            
			m->setName( fnv[k++] );
            
			if (m != NULL)
				cmv->push_back( m );
        }
    }
    
	return cmv;
}

CharacterMatrix* NclReader::createAminoAcidMatrix(NxsCharactersBlock* charblock) {
    
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::protein)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigChar = (int)charset.size();
	int numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // allocate a matrix of the correct size
	CharacterMatrixDiscrete* cm = new CharacterMatrixDiscrete( numOrigTaxa, numOrigChar, "amino acid" );
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
    {
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cm->excludeTaxon(origTaxIndex);
        
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cm->addTaxonName(tn);
        
        for (NxsUnsignedSet::const_iterator cit = charset.begin(); cit != charset.end();cit++)
        {	
            // check if the site is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cm->excludeCharacter(*cit);
            
            if (charblock->IsGapState(origTaxIndex, *cit) == true) 
                cm->setAminoAcid(origTaxIndex, (*cit), '?');
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true) 
                cm->setAminoAcid(origTaxIndex, (*cit), '?');
            else
            {
                int nStates = charblock->GetNumStates(origTaxIndex, *cit);
                cm->setAminoAcid( origTaxIndex, (*cit), charblock->GetState(origTaxIndex, *cit, 0) );
                for(int s=1; s<nStates; s++)
                    cm->addAminoAcid( origTaxIndex, (*cit), charblock->GetState(origTaxIndex, *cit, s) );
            }
        }
    }
    
    if (cm->checkMatrix() == false)
        return NULL;
    
    return cm;
}

CharacterMatrix* NclReader::createContinuousMatrix(NxsCharactersBlock* charblock) {
    
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::continuous)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigChar = (int)charset.size();
	int numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // allocate a matrix of the correct size
	CharacterMatrixContinuous* cm = new CharacterMatrixContinuous( numOrigTaxa, numOrigChar );
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
    {
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cm->excludeTaxon(origTaxIndex);
        
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cm->addTaxonName(tn);
        
        // add the real-valued observation
        int j = 0;
        for (NxsUnsignedSet::const_iterator cit = charset.begin(); cit != charset.end();cit++)
        {	
            // check if the character is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cm->excludeCharacter(*cit);
            
            const std::vector<double>& x = charblock->GetContinuousValues( origTaxIndex, *cit, std::string("AVERAGE") );
            cm->setState(origTaxIndex, (*cit), x[0]);
            j++;
        }
    }
    
    return cm;
}

CharacterMatrix* NclReader::createNucleotideMatrix(NxsCharactersBlock* charblock) {
    
    // check that the character block is of the correct type
	if ( charblock->GetDataType() != NxsCharactersBlock::dna && charblock->GetDataType() != NxsCharactersBlock::rna && charblock->GetDataType() != NxsCharactersBlock::nucleotide )
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigChar = (int)charset.size();
	int numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // allocate a matrix of the correct size
	CharacterMatrixDiscrete* cm = new CharacterMatrixDiscrete( numOrigTaxa, numOrigChar, "dna" );
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
    {
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cm->excludeTaxon(origTaxIndex);
        
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cm->addTaxonName(tn);
        
        // add the sequence information for the sequence associated with the taxon
        for (NxsUnsignedSet::iterator cit = charset.begin(); cit != charset.end(); cit++)
        {	
            // check if the site is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cm->excludeCharacter(*cit);
            
            // add the character state to the matrix
            if ( charblock->IsGapState(origTaxIndex, *cit) == true ) 
                cm->setNucleotide( origTaxIndex, (*cit), 'N' );
            else if (charblock->IsMissingState(origTaxIndex, *cit) == true) 
                cm->setNucleotide( origTaxIndex, (*cit), 'N' );
            else
            {
                int nStates = charblock->GetNumStates(origTaxIndex, *cit);
                cm->setNucleotide( origTaxIndex, (*cit), charblock->GetState(origTaxIndex, *cit, 0) );
                for(int s=1; s<nStates; s++)
                    cm->addNucleotide( origTaxIndex, (*cit), charblock->GetState(origTaxIndex, *cit, s) );
            }
        }
    }
    
    if (cm->checkMatrix() == false)
        return NULL;
    
	return cm;
}

CharacterMatrix* NclReader::createStandardMatrix(NxsCharactersBlock* charblock) {
    
    // check that the character block is of the correct type
	if (charblock->GetDataType() != NxsCharactersBlock::standard)
        return NULL;
    
    // get the set of characters (and the number of taxa)
    NxsUnsignedSet charset;
    for (int i=0; i<charblock->GetNumChar(); i++)
        charset.insert(i);
	int numOrigChar = (int)charset.size();
	int numOrigTaxa = charblock->GetNTax();
    
	// get the set of excluded characters
	NxsUnsignedSet excluded = charblock->GetExcludedIndexSet();
    
    // get the number of states
    const NxsDiscreteDatatypeMapper* mapper = charblock->GetDatatypeMapperForChar(0);
    std::string sym = charblock->GetSymbols();
    int nStates = mapper->GetNumStates();
    
    // allocate a matrix of the correct size
	CharacterMatrixDiscrete* cm = new CharacterMatrixDiscrete( numOrigTaxa, numOrigChar, nStates, "standard" );
    
	// read in the data, including taxon names
	for (int origTaxIndex=0; origTaxIndex<numOrigTaxa; origTaxIndex++) 
    {
        // check if the taxon is excluded
		if ( !charblock->IsActiveTaxon(origTaxIndex) )
            cm->excludeTaxon(origTaxIndex);
        
        // add the taxon name
        NxsString tLabel = charblock->GetTaxonLabel(origTaxIndex);
        std::string tn = NxsString::GetEscaped(tLabel).c_str();
        cm->addTaxonName(tn);
        
        // add the character information for the data associated with the taxon
        for (NxsUnsignedSet::iterator cit = charset.begin(); cit != charset.end(); cit++)
        {	
            // check if the site is excluded
            NxsUnsignedSet::iterator it = excluded.find(*cit);
            if (it != excluded.end())
                cm->excludeCharacter(*cit);
            
            // add the character state to the matrix
            int ns = charblock->GetNumStates(origTaxIndex, *cit);
            for(int s=0; s<ns; s++)
            {
                char c = charblock->GetState(origTaxIndex, *cit, s);
                for (int i=0; i<nStates; i++)
                {
                    if ( sym[i] == c )
                        cm->setState(origTaxIndex, *cit, i);
                }
            }
        }
    }
    
    if (cm->checkMatrix() == false)
        return NULL;
    
    return cm;
}

bool NclReader::fileExists(const char *fn) const {
    
	bool exists = false;
	FILE *fp = fopen(fn, "r");
	if (fp != NULL)
    {
		fclose(fp);
		exists = true;
    }
	return exists;
}

NclReader& NclReader::getInstance(void) {
    
	static NclReader rb;
	return rb;
}

std::vector<CharacterMatrix*>* NclReader::readMatrices(const std::vector<std::string> fn, const std::string fileFormat, const std::string dataType, const bool isInterleaved) {
    
	// check that the files exist
    for (std::vector<std::string>::const_iterator f = fn.begin(); f != fn.end(); f++)
    {
        if ( !fileExists((*f).c_str()) )	
        {
            std::cerr << "Error: Data file \"" << (*f) << "\"not found" << std::endl;
            return NULL;
        }
    }
    
	// allocate a vector of matrices
	std::vector<CharacterMatrix*>* cmv = new std::vector<CharacterMatrix*>();
    
    // read the data files
    for (std::vector<std::string>::const_iterator f = fn.begin(); f != fn.end(); f++)
    {
        std::vector<CharacterMatrix*>* v = readMatrices( (*f).c_str(), fileFormat, dataType, isInterleaved );
		if (v != NULL)
        {
			for (std::vector<CharacterMatrix*>::iterator m = v->begin(); m != v->end(); m++)
				cmv->push_back( (*m) );
			delete v;
        }
		else 
			return NULL;
		nexusReader.ClearContent();
    }
    
    return cmv;
}

std::vector<CharacterMatrix*>* NclReader::readMatrices(const char* fileName, const std::string fileFormat, const std::string dataType, const bool isInterleaved) {
    
	// check that the file exists
	if ( !fileExists(fileName) )	
    {
		std::cerr << "Error: Data file \"" << fileName << "\"not found" << std::endl;
        return NULL;
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
		std::cout << "Nexus Error: " << err.msg << " (" << err.pos << ", " << err.line << ", " << err.col << ")" << std::endl;
		return NULL;
    }
    
	std::vector<std::string> fileNameVector;
	std::string str = fileName;
	fileNameVector.push_back( str );
    
	std::vector<CharacterMatrix*>* cvm = convertFromNcl(fileNameVector);
	if (cvm->size() == 0)
    {
		std::cout << "Deleting cvm" << std::endl;
		delete cvm;
		return NULL;
    }
	return cvm;
}

std::vector<Tree*>* NclReader::readTrees(const std::string fn, const std::string fileFormat) {
	
	// check that the file exist
    if ( !fileExists(fn.c_str()) ) {
        std::cerr << "Error: Data file \"" << fn << "\"not found" << std::endl;
        return NULL;
	}
    
	
	// allocate a vector of trees
	std::vector<Tree*>* trees = new std::vector<Tree*>();
	
    // TODO @Tracy: Why do we read a vector of trees, then copy every single tree into a new vector; instead of returning the vector straight away?!? (Sebastian)
    
    // read the data files
 	std::vector<Tree*>* f = readTrees( fn.c_str(), fileFormat);
	if (f != NULL) {
		for (std::vector<Tree*>::iterator m = f->begin(); m != f->end(); m++)
			trees->push_back( (*m) );
		delete f;
	}
	else 
		return NULL;
    
	nexusReader.ClearContent();
	
	
    return trees;
}

std::vector<Tree*>* NclReader::readTrees(const char* fileName, const std::string fileFormat) {
	
	// check that the file exists
	if ( !fileExists(fileName) ) {
		std::cerr << "Error: Data file \"" << fileName << "\"not found" << std::endl;
        return NULL;
	}
	
	try {
		if (fileFormat == "nexus") {
			// NEXUS file format
			nexusReader.ReadFilepath(fileName, MultiFormatReader::NEXUS_FORMAT);
		}
		else if (fileFormat == "phylip") {
			// phylip file format with long taxon names
			nexusReader.ReadFilepath(fileName, MultiFormatReader::RELAXED_PHYLIP_TREE_FORMAT);
		}
	}
	catch(NxsException err) {
		std::cout << "Nexus Error: " << err.msg << " (" << err.pos << ", " << err.line << ", " << err.col << ")" << std::endl;
		return NULL;
	}
	
	std::vector<std::string> fileNameVector;
	std::string str = fileName;
	fileNameVector.push_back( str );
	
	std::vector<Tree*>* cvm = convertTreesFromNcl();
	return cvm;
}

std::vector<Tree*>* NclReader::convertTreesFromNcl() {
	
	const unsigned nTaxaBlocks = nexusReader.GetNumTaxaBlocks();
	std::vector<Tree*>* rbTreesFromFile;
	for (unsigned t = 0; t < nTaxaBlocks; ++t) {
        
		const NxsTaxaBlock * tb = nexusReader.GetTaxaBlock(t);
		const unsigned nTreesBlocks = nexusReader.GetNumTreesBlocks(tb);
		if (nTreesBlocks == 0)
			continue;
        
		for (unsigned i = 0; i < nTreesBlocks; ++i) {
			const NxsTreesBlock * trb = nexusReader.GetTreesBlock(tb, i);
			trb->ProcessAllTrees();
			for (unsigned j = 0; j < trb->GetNumTrees(); ++j) {
				const NxsFullTreeDescription & ftd = trb->GetFullTreeDescription(j);
				NxsSimpleTree tree(ftd, -1, -1.0);
				Tree *rbTree = translateNclSimpleTreeToTree(tree);
				rbTreesFromFile->push_back( rbTree );
				
			}
		}
	}
    
	return rbTreesFromFile;
    
}


Tree* NclReader::translateNclSimpleTreeToTree(NxsSimpleTree &nTree){
    
    // get the root from the ncl tree
    const NxsSimpleNode* rn = nTree.GetRootConst();
    
    // create a new tree root node
    const std::string& name = rn->GetName();
    TreeNode *root = new TreeNode(name);
    
	// construct tree recursively
    constructTreefromNclRecursively(root, rn);
    
    // create a new simple tree
    Tree *myTreeFromNcl = new TreeSimple(root);
    
	return myTreeFromNcl;
}
