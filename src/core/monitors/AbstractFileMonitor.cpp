#include "AbstractFileMonitor.h"
#include "DagNode.h"
#include "Mcmc.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "RbVersion.h"

using namespace RevBayesCore;

/* Constructor */
AbstractFileMonitor::AbstractFileMonitor(DagNode *n, unsigned long g, const std::string &fname, bool ap, bool wv) : Monitor(g,n),
    out_stream(),
    filename( fname ),
    working_file_name( fname ),
    append(ap),
    flatten( true ),
    write_version( wv )
{
    
}


AbstractFileMonitor::AbstractFileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, bool ap, bool wv) : Monitor(g,n),
    out_stream(),
    filename( fname ),
    working_file_name( fname ),
    append(ap),
    flatten( true ),
    write_version( wv )
{
    
}


AbstractFileMonitor::AbstractFileMonitor(const AbstractFileMonitor &f) : Monitor( f ),
    out_stream()
{
    
    filename            = f.filename;
    working_file_name   = f.working_file_name;
    append              = f.append;
    flatten             = f.flatten;
    write_version       = f.write_version;
    
    if ( f.out_stream.is_open() == true )
    {
        openStream( true );
    }
    
}


AbstractFileMonitor::~AbstractFileMonitor(void)
{
    // we should always close the stream when the object is deleted
    if (out_stream.is_open())
    {
        closeStream();
    }
    
}



/**
 * Set the file extension.
 */
void AbstractFileMonitor::addFileExtension(const std::string &s, bool dir)
{
    
    // compute the working filename
    if ( dir == false )
    {
        RbFileManager fm = RbFileManager(filename);
        working_file_name = fm.getFilePath() + fm.getPathSeparator() + fm.getFileNameWithoutExtension() + s + "." + fm.getFileExtension();
    }
    else
    {
        RbFileManager fm = RbFileManager(filename);
        working_file_name = fm.getFilePath() + fm.getPathSeparator() + s + fm.getPathSeparator() + fm.getFileName();
    }

}


void AbstractFileMonitor::closeStream()
{
    out_stream.close();
}


bool AbstractFileMonitor::isFileMonitor( void ) const
{
    return true;
}


/** open the file stream for printing */
void AbstractFileMonitor::openStream( bool reopen )
{
    
    RbFileManager f = RbFileManager(working_file_name);
    f.createDirectoryForFile();
            
    // open the stream to the file
    if ( append == true || reopen == true )
    {
        out_stream.open( f.getFullFileName().c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
    }
    else
    {
        out_stream.open( f.getFullFileName().c_str(), std::fstream::out);
        out_stream.close();
        out_stream.open( f.getFullFileName().c_str(), std::fstream::in | std::fstream::out);
    }
        
}


/**
 * Set flag about whether to append to an existing file.
 *
 * \param[in]   tf   Flag if to append.
 */
void AbstractFileMonitor::setAppend(bool tf)
{
    
    append = tf;
    
}

/**
 * Set flag about whether to print the software version.
 *
 * \param[in]   tf   Flag if the version should be printed.
 */
void AbstractFileMonitor::setPrintVersion(bool tf)
{
    
    write_version = tf;
    
}
