#include "AbstractFileMonitor.h"
#include "DagNode.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "RbFileManager.h"
#include "RbVersion.h"

using namespace RevBayesCore;

/* Constructor */
AbstractFileMonitor::AbstractFileMonitor(DagNode *n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,n),
    out_stream(),
    filename( fname ),
    working_file_name( fname ),
    separator( del ),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    append(ap),
    flatten( true )
{
    
}


AbstractFileMonitor::AbstractFileMonitor(const std::vector<DagNode *> &n, unsigned long g, const std::string &fname, const std::string &del, bool pp, bool l, bool pr, bool ap) : Monitor(g,n),
    out_stream(),
    filename( fname ),
    working_file_name( fname ),
    separator( del ),
    posterior( pp ),
    prior( pr ),
    likelihood( l ),
    append(ap),
    flatten( true )
{
    
}


AbstractFileMonitor::AbstractFileMonitor(const AbstractFileMonitor &f) : Monitor( f ),
    out_stream()
{
    
    filename            = f.filename;
    working_file_name   = f.working_file_name;
    separator           = f.separator;
    prior               = f.prior;
    posterior           = f.posterior;
    likelihood          = f.likelihood;
    append              = f.append;
    flatten             = f.flatten;
    
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


/**
 * Combine output for the monitor.
 * Overwrite this method for specialized behavior.
 */
void AbstractFileMonitor::combineReplicates( size_t n_reps )
{
    
    if ( enabled == true )
    {
        
        std::fstream combined_output_stream;
        
        int sample_number = 0;
        
        // open the stream to the file
        combined_output_stream.open( filename.c_str(), std::fstream::out);
        combined_output_stream.close();
        combined_output_stream.open( filename.c_str(), std::fstream::in | std::fstream::out);
        
        for (size_t i=0; i<n_reps; ++i)
        {
            std::stringstream ss;
            ss << "_run_" << (i+1);
            std::string s = ss.str();
            RbFileManager fm = RbFileManager(filename);
            std::string current_file_name = fm.getFilePath() + fm.getPathSeparator() + fm.getFileNameWithoutExtension() + s + "." + fm.getFileExtension();
            
            RbFileManager current_fm = RbFileManager(current_file_name);
            std::ifstream current_input_stream;
            
            if ( current_fm.openFile(current_input_stream) == false )
            {
                throw RbException( "Could not open file '" + current_file_name + "'." );
            }
            
            std::string read_line = "";
            size_t lines_skipped = 0;
            size_t lines_to_skip = 1;
            while (std::getline(current_input_stream,read_line))
            {
                ++lines_skipped;
                if ( lines_skipped <= lines_to_skip)
                {
                    if ( i == 0 )
                    {
                        // write output
                        combined_output_stream << read_line;

                        // add a new line
                        combined_output_stream << std::endl;
                    }
                    continue;
                }
                
                std::vector<std::string> fields;
                StringUtilities::stringSplit(read_line, separator, fields);
                
                // add the current sample number
                combined_output_stream << sample_number;
                ++sample_number;
                for (size_t j=1; j<fields.size(); ++j)
                {
                    // add a separator before every new element
                    combined_output_stream << separator;

                    // write output
                    combined_output_stream << fields[j];
                }
                // add a new line
                combined_output_stream << std::endl;
                
            };
            
            fm.closeFile( current_input_stream );

        }
        
        combined_output_stream.close();

    }

}


bool AbstractFileMonitor::isFileMonitor( void ) const
{
    return true;
}


/** 
 * Monitor value at generation gen 
 */
void AbstractFileMonitor::monitorVariables(unsigned long gen)
{
        
    for (std::vector<DagNode*>::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
        // add a separator before every new element
        out_stream << separator;
            
        // get the node
        DagNode *node = *i;
            
        // print the value
        node->printValue(out_stream, separator, -1, false, false, flatten);
    }
    
}


/**
 * Monitor 
 */
void AbstractFileMonitor::monitor(unsigned long gen)
{
    
    // get the printing frequency
    unsigned long samplingFrequency = printgen;
    
    if ( enabled == true && gen % samplingFrequency == 0 )
    {
//        out_stream.open( working_file_name.c_str(), std::fstream::out | std::fstream::app);
        out_stream.seekg(0, std::ios::end);
        
        // print the iteration number first
        out_stream << gen;
        
        if ( posterior == true )
        {
            // add a separator before every new element
            out_stream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                pp += (*it)->getLnProbability();
            }
            out_stream << pp;
        }
        
        if ( likelihood == true )
        {
            // add a separator before every new element
            out_stream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                if ( (*it)->isClamped() )
                {
                    pp += (*it)->getLnProbability();
                }
            }
            out_stream << pp;
        }
        
        if ( prior == true )
        {
            // add a separator before every new element
            out_stream << separator;
            
            const std::vector<DagNode*> &n = model->getDagNodes();
            double pp = 0.0;
            for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it)
            {
                if ( !(*it)->isClamped() )
                {
                    pp += (*it)->getLnProbability();
                }
            }
            out_stream << pp;
        }
        
        monitorVariables( gen );
        
        out_stream << std::endl;
        
        out_stream.flush();
        
//        out_stream.close();
        
    }
    
    
}


/** open the file stream for printing */
void AbstractFileMonitor::openStream( bool reopen )
{
    
    RbFileManager f = RbFileManager(working_file_name);
    f.createDirectoryForFile();
        
    // open the stream to the file
    if ( append == true || reopen == true )
    {
        out_stream.open( working_file_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
    }
    else
    {
        out_stream.open( working_file_name.c_str(), std::fstream::out);
        out_stream.close();
        out_stream.open( working_file_name.c_str(), std::fstream::in | std::fstream::out);
    }
    
//    out_stream.close();
    
}

/** 
 * Print header for monitored values 
 */
void AbstractFileMonitor::printHeader( void )
{
    
    if ( enabled == true )
    {
    
//    out_stream.open( working_file_name.c_str(), std::fstream::out | std::fstream::app);
        out_stream.seekg(0, std::ios::end);
        
        RbVersion version;
        out_stream << "#RevBayes version " + version.getVersion() + ")\n";
        out_stream << "#Build from " + version.getGitBranch() + " (" + version.getGitCommit() + ") on " + version.getDate() + "\n";
    
        // print one column for the iteration number
        out_stream << "Iteration";
    
        if ( posterior == true )
        {
            // add a separator before every new element
            out_stream << separator;
            out_stream << "Posterior";
        }
        
        if ( likelihood == true )
        {
            // add a separator before every new element
            out_stream << separator;
            out_stream << "Likelihood";
        }
    
        if ( prior == true )
        {
            // add a separator before every new element
            out_stream << separator;
            out_stream << "Prior";
        }
    
        // print the headers for the variables
        printFileHeader();
    
        out_stream << std::endl;
        
        out_stream.flush();
    
        //    out_stream.close();
    }
    
}


/**
 * Print additional header for monitored values
 */
void AbstractFileMonitor::printFileHeader( void )
{
    
    for (std::vector<DagNode *>::const_iterator it=nodes.begin(); it!=nodes.end(); ++it)
    {
        // add a separator before every new element
        out_stream << separator;
        
        const DagNode* the_node = *it;
        
        // print the header
        if (the_node->getName() != "")
        {
            the_node->printName(out_stream,separator, -1, true, flatten);
        }
        else
        {
            out_stream << "Unnamed";
        }
        
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
 * Set flag about whether to print the likelihood.
 *
 * \param[in]   tf   Flag if the likelihood should be printed.
 */
void AbstractFileMonitor::setPrintLikelihood(bool tf)
{
    
    likelihood = tf;
    
}


/**
 * Set flag about whether to print the posterior probability.
 *
 * \param[in]   tf   Flag if the posterior probability should be printed.
 */
void AbstractFileMonitor::setPrintPosterior(bool tf)
{
    
    posterior = tf;
    
}


/**
 * Set flag about whether to print the prior probability.
 *
 * \param[in]   tf   Flag if the prior probability should be printed.
 */
void AbstractFileMonitor::setPrintPrior(bool tf)
{
    
    prior = tf;
    
}


