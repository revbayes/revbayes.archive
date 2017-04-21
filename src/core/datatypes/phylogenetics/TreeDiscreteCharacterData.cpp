#include "HomologousDiscreteCharacterData.h"
#include "NclReader.h"
#include "NexusWriter.h"
#include "StandardState.h"
#include "TreeDiscreteCharacterData.h"



using namespace RevBayesCore;

TreeDiscreteCharacterData::TreeDiscreteCharacterData()
{
    
}


TreeDiscreteCharacterData* TreeDiscreteCharacterData::clone( void ) const
{
    return new TreeDiscreteCharacterData( *this );
}



AbstractHomologousDiscreteCharacterData& TreeDiscreteCharacterData::getCharacterData( void )
{
    
    return *character_data;
}



const AbstractHomologousDiscreteCharacterData& TreeDiscreteCharacterData::getCharacterData( void ) const
{
    
    return *character_data;
}


/**
 * Initialize this object from a file
 *
 * \param[in]   idx    The site at which we want to know if it is constant?
 */
void TreeDiscreteCharacterData::initFromFile(const std::string &dir, const std::string &fn)
{
    RbFileManager fm = RbFileManager(dir, fn + ".nex");
    
    // get an instance of the NCL reader
    NclReader reader = NclReader();
    
    std::string myFileType = "nexus";
    std::string dType = character_data->getDataType();
    
    std::string suffix = "|" + dType;
    suffix += "|noninterleaved";
    myFileType += suffix;
    
    // read the content of the file now
    std::vector<AbstractCharacterData*> m_i = reader.readMatrices( fm.getFullFileName(), myFileType );
    
    if ( m_i.size() < 1 )
    {
        const std::set<std::string> &warnings = reader.getWarnings();
        for (std::set<std::string>::iterator it = warnings.begin(); it != warnings.end(); ++it)
        {
            std::cerr << "NCL-Warning:\t\t" << *it << std::endl;
        }
        throw RbException("Could not read character data matrix from file \"" + fm.getFullFileName() + "\".");
    }
    
    HomologousDiscreteCharacterData<StandardState> *coreM = static_cast<HomologousDiscreteCharacterData<StandardState> *>( m_i[0] );
    
    character_data = coreM;
    
}


/**
 * Initialize this object from a file
 *
 * \param[in]   s    The value as string
 */
void TreeDiscreteCharacterData::initFromString(const std::string &s)
{
    throw RbException("Cannot initialize a tree with a discrete character data matrix from a string.");
}


void TreeDiscreteCharacterData::setCharacterData( AbstractHomologousDiscreteCharacterData *d )
{
    
    character_data = d;
}


void TreeDiscreteCharacterData::writeToFile(const std::string &dir, const std::string &fn) const
{
    RbFileManager fm = RbFileManager(dir, fn + ".nex");
    fm.createDirectoryForFile();
    
    NexusWriter nw( fm.getFullFileName() );
    nw.openStream(false);
    
    nw.writeNexusBlock( *this );
    nw.writeNexusBlock( *character_data );
    
    nw.closeStream();
    
}


void TreeDiscreteCharacterData::setTree(const Tree &t)
{
    
    nodes.clear();
    delete root;
    root = NULL;
    
    binary      = t.isBinary();
    num_tips    = t.getNumberOfTips();
    num_nodes   = t.getNumberOfNodes();
    rooted      = t.isRooted();
    
    TopologyNode* newRoot = t.getRoot().clone();
    
    // set the root. This will also set the nodes vector
    // do not reorder node indices when copying (WP)
    setRoot(newRoot, false);
}
