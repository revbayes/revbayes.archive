#include "AbstractTaxonData.h"
#include "CharacterState.h"
#include "NexusWriter.h"
#include "RbFileManager.h"

using namespace RevBayesCore;


/**
 * Default constructor.
 * The default constructor does nothing except allocating the object.
 *
 * \param[in]   fileName    The name of the file into which the objects is to be written.
 */
NexusWriter::NexusWriter( const std::string &fn ) :
    fileName( fn )
{
    
}


/**
 * Close the file stream.
 */
void NexusWriter::closeStream( void ) 
{
    
    // close the stream
    outStream.close();
}


/**
 * Open the file stream to a file with the name used in the constructor.
 */
void NexusWriter::openStream( void ) 
{
    RbFileManager f = RbFileManager(fileName);
    f.createDirectoryForFile();
    
    // open the stream to the file
    outStream.open( fileName.c_str(), std::fstream::out );
    
    // write header line
    outStream << "#NEXUS" << std::endl;
    
}


/**
 * This method simply writes a discrete character data object into a file in Nexus format.
 *
 * \param[in]   data        The discrete character data object which is written out.
 */
void NexusWriter::writeNexusBlock(const AbstractHomologousDiscreteCharacterData &data) 
{
    // write initial lines of the character block
    outStream << std::endl;
    outStream << "Begin data;" << std::endl;
    outStream << "Dimensions ntax=" << data.getNumberOfIncludedTaxa() << " nchar=" << data.getNumberOfIncludedCharacters() << ";" << std::endl;
    outStream << "Format datatype=" << data.getDataType() << " ";
    if ( data.getDataType() == "Standard" )
    {
        outStream << "symbols=\"" << data.getTaxonData(0).getCharacter(0).getStateLabels() << "\" ";
    }
    outStream << "missing=? gap=-;" << std::endl;
    outStream << "Matrix" << std::endl;

    
    const std::vector<Taxon> &taxa = data.getTaxa();
    for (std::vector<Taxon>::const_iterator it = taxa.begin();  it != taxa.end(); ++it)
    {
        
        if ( !data.isTaxonExcluded( it->getName() ) )
        {
            outStream << it->getName() << "   ";
            const AbstractDiscreteTaxonData &taxon = data.getTaxonData( it->getName() );
            size_t nChars = taxon.getNumberOfCharacters();
            
            size_t count = 0;
            
            for (size_t i = 0; i < nChars; ++i)
            {
                if ( !data.isCharacterExcluded( i ) )
                {
                    const CharacterState &c = taxon.getCharacter( i );
                    outStream << c.getStringValue();
                    ++count;
                }
            }
        
            outStream << std::endl;
        }
        
    }
    
    outStream << ";" << std::endl;
    outStream << "End;" << std::endl;
    outStream << std::endl;
    
}


/**
 * This method simply writes a continuous character data object into a file in Nexus format.
 *
 * \param[in]   data        The continuous character data object which is written out.
 */
void NexusWriter::writeNexusBlock(const ContinuousCharacterData &data)
{
    
    // write initial lines of the character block
    outStream << std::endl;
    outStream << "Begin data;" << std::endl;
    outStream << "Dimensions ntax=" << data.getNumberOfIncludedTaxa() << " nchar=" << data.getNumberOfIncludedCharacters() << ";" << std::endl;
    outStream << "Format datatype=" << data.getDataType() << " ";
    outStream << "missing=? gap=-;" << std::endl;
    outStream << "Matrix" << std::endl;
    
    
    const std::vector<Taxon> &taxa = data.getTaxa();
    for (std::vector<Taxon>::const_iterator it = taxa.begin();  it != taxa.end(); ++it)
    {
        if ( !data.isTaxonExcluded( it->getName() ) )
        {
            outStream << it->getName() << "   ";
            const ContinuousTaxonData &taxon = data.getTaxonData( it->getName() );
            size_t nChars = taxon.getNumberOfCharacters();
            for (size_t i = 0; i < nChars; ++i)
            {
                if ( !data.isCharacterExcluded( i ) )
                {
                    const double &c = taxon.getCharacter( i );
                    outStream << c << " ";
                }
            }
        
            outStream << std::endl;
        }
    }

    outStream << ";" << std::endl;
    outStream << "End;" << std::endl;
    outStream << std::endl;
    
}


/**
 * This method simply writes a taxon-block into a file in Nexus format.
 *
 * \param[in]   clade        The clade object from which the taxa are taken.
 */
void NexusWriter::writeNexusBlock(const Clade &c) 
{
    
    const std::vector<Taxon>& labels = c.getTaxa();
    
    outStream << std::endl;
    outStream << "\tBegin taxa;" << std::endl;
    outStream << "\tDimensions ntax=" << c.size() << ";" << std::endl;;
    outStream << "\tTaxlabels" << std::endl;
    for (std::vector<Taxon>::const_iterator it = labels.begin(); it != labels.end(); ++it)
    {
        outStream << "\t\t" << it->getName() << std::endl;
    }
    outStream << "\t\t;" << std::endl;
    outStream << "End;" << std::endl;
    
}


/**
 * This method simply writes a tree into a file in Nexus format.
 *
 * \param[in]   tree        The tree object which is written out.
 */
void NexusWriter::writeNexusBlock(const Tree &tree) 
{
    
    outStream << std::endl; 
    outStream << "Begin trees;" << std::endl;
    outStream << "tree TREE1 = [&R]" << tree << std::endl;
    outStream << "End;" << std::endl;
    
    
}


/**
 * This method simply writes a tree or multiple trees into a file in Nexus format.
 *
 * \param[in]   trees        The trees which are written out.
 */
void NexusWriter::writeNexusBlock(const std::vector<Tree> &trees) 
{
    
    outStream << std::endl; 
    outStream << "Begin trees;" << std::endl;
    for (size_t i = 0; i < trees.size(); ++i)
    {
        outStream << "tree TREE" << (i+1) << " = [&R]" << trees[i] << ";" << std::endl;
    }
    outStream << "End;" << std::endl;

    
}
