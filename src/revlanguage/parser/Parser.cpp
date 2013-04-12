/**
 * @file
 * This file contains the implementation of Parser, which is
 * used to execute syntax trees and get help info.
 *
 * @brief Implementation of Parser
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "Help.h"
#include "Parser.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RlString.h"
#include "SyntaxElement.h"
#include "SyntaxFormal.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxVariable.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <cstdlib>
#include <cstring>
#include <list>
#include <sstream>

#include "grammar.tab.h"

// Global flags indicating flex state
bool foundNewline;
bool foundErrorBeforeEnd;
bool foundEOF;


// Global command buffer RlString stream
std::stringstream rrcommand;


RevLanguage::Environment *executionEnvironment;


/**
 * This function breaks a buffer into Rev lines, taking into account the fact that ';' also
 * breaks lines in addition to newliines. The function also deals with different types of line
 * endings and translate all to Unix '\n' line endings.
 */
void RevLanguage::Parser::breakIntoLines( const std::string& cmd, std::list<std::string>& lines ) const {

    bool inComment, inQuote;

    // Initialize
    lines.clear();  // just in case
    inComment = inQuote = false;
    std::stringstream buf( cmd );   // a stream is convenient for reading

    // Process command buffer
    while ( buf.good() ) {

        std::stringstream temp;

        while ( buf.good() ) {
    
            char c = buf.get();

            if ( c == EOF && inQuote == true ) {
                throw RbException( "End of line while in quote" );
            }
            else if ( c == '"' ) {
                /* switch quote on or off if not in comment */
                if ( inQuote == true )
                    inQuote = false;
                else if ( inComment == false )
                    inQuote = true;
            }
            else if ( c == '#' && inQuote == false ) {
                /* we are now in comment */
                inComment = true;
            }
            else if ( c == ';' && inQuote == false && inComment == false ) {
                /* break line here */
                break;
            }
            else if ( c == EOF && inQuote == false ) {
                /* break line here */
                break;
            }
            else if ( c == '\n' && inQuote == false ) {
                /* break line here */
                break;
            }
            else if ( c == '\377' && inQuote == false ) {
                /* break line here */
//                c = '\n';
                continue;
            }
            else if ( c == '\r' && inQuote == false ) {
                /* break line here but first swallow any extra newline in DOS line ending */
                char d = buf.peek();
                if ( d == '\n' )
                     c = buf.get();
                break;
            }

            temp.put( c );
        }


        if ( temp.str().size() > 0 ) {
            lines.push_back( temp.str() + "\n" );
        }
    }
}


/** This function causes recursive execution of a syntax tree by calling the root to get its value */
int RevLanguage::Parser::execute(SyntaxElement* root, Environment &env) const {

#	ifdef DEBUG_PARSER
    // Print syntax tree
    std::cerr << std::endl;
    std::cerr << "Syntax tree root before execution:\n";
    root->printValue(std::cerr);
    std::cerr << std::endl;
#	endif

    // Declare a variable for the result
    RbPtr<Variable> result = NULL;
    
    //! Execute syntax tree
    try {
#ifdef DEBUG_PARSER
        printf("Parser getting the semantic value of the syntax tree...\n");
#endif
        result = root->evaluateContent(env);
    }
    catch(RbException& rbException) {

        std::ostringstream msg;

        // Catch a quit request
        if (rbException.getExceptionType() == RbException::QUIT) {
            exit(0);
        }

        // Catch a missing variable exception that might be interpreted as a request for
        // usage help on a function
        SyntaxVariable* rootPtr = dynamic_cast<SyntaxVariable*>( (SyntaxElement*)root);
        SyntaxVariable* theVariable = rootPtr;
        if ( rbException.getExceptionType() == RbException::MISSING_VARIABLE && theVariable != NULL && !theVariable->isMemberVariable() ) {

            const std::string& fxnName = theVariable->getIdentifier();
            const std::vector<Function*>& functions = Workspace::userWorkspace().getFunctionTable().findFunctions( fxnName );
            if ( functions.size() != 0 ) {
                RBOUT( "Usage:" );
                for ( std::vector<Function*>::const_iterator i=functions.begin(); i!=functions.end(); i++ ) {
                    RBOUT( (*i)->debugInfo() );
                }
                return 0;
            }
        }

        // All other exceptions
#ifdef DEBUG_PARSER
        printf("Caught an exception\n");
#endif
        rbException.print(msg);
        RBOUT( msg.str() );

        // Return signal indicating problem
        return 2;
    }

    // Print result if the root is not an assign expression
    if ( !root->isAssignment() && result != NULL && result->getValue() != RbNullObject::getInstance()) {
        std::ostringstream msg;
        result->getValue().printValue(msg);
        RBOUT( msg.str() );
    }

    // Return success
    return 0;
}


/** Give flex a line to parse */
void RevLanguage::Parser::getline(char* buf, size_t maxsize) {

    if (!rrcommand.good()) {
        foundEOF = true;
//        buf[0] = EOF;
//        buf[1] = '\0';
        buf[0] = '\0';
    }
    else {
        foundNewline = false;
        rrcommand.getline(buf, maxsize-3);
        // Deal with line endings in case getline uses non-Unix endings
        size_t i = strlen(buf);
        if ( i >= 1 && buf[i-1] == '\r' )
            buf[i-1] = '\n';
        else if ( i >= 2 && buf[i-1] == '\n' && buf[i-2] == '\r' ) {
            buf[i-2] = '\n';
            i--;
        }
        else if ( i == 0 || (i >= 1 && buf[i-1] != '\n') ) {
            buf[i++] = '\n';
        }
        buf[i] = '\0';
    }

//#	if defined (DEBUG_PARSER)
//    if (foundEOF == true)
//        printf("Parser gave flex EOF\n");
//    else
//        printf("Parser gave flex line(s):\n %s", buf);
//#	endif
}


/** This function gets help info about a symbol */
int RevLanguage::Parser::help(const std::string& symbol) const {

    std::ostringstream msg;

#	if defined DEBUG_PARSER
    // Print syntax tree 
    std::cerr << std::endl;
    std::cerr << "Parser trying to get help for symbol '" << symbol << "'";
    std::cerr << std::endl;
#	endif

    // Get some help
    Help& userHelp = Help::getHelp();
    if ( userHelp.isUserHelpAvailable() == true && userHelp.isHelpAvailableForQuery( symbol ) == true )
        {
        std::string hStr = userHelp.formatHelpString( symbol, 100);
        UserInterface::userInterface().output(hStr, false);
        }
    else {
        if (userHelp.isUserHelpAvailable() == false)
            RBOUT("User help is unavailable");
        else if ( userHelp.isHelpAvailableForQuery( symbol ) == false )
            RBOUT("Help unavailable for \"" + symbol + "\"");

        std::vector<Function *> functions = Workspace::userWorkspace().getFunctionTable().findFunctions( symbol );
        if ( functions.size() != 0 ) {
            RBOUT( "Usage:" );
            for ( std::vector<Function *>::const_iterator i=functions.begin(); i!=functions.end(); i++ ) {
                RBOUT( (*i)->debugInfo() );
                delete *i;
            }
        }
    }

    // Return success
    return 0;
}


/** This function prints help info about a function if it sees a function call */
int RevLanguage::Parser::help(const SyntaxFunctionCall& root) const {

    std::ostringstream msg;

#	if defined DEBUG_PARSER
    // Print syntax tree
    std::cerr << std::endl;
    std::cerr << "Syntax tree root before help:\n";
    root.printValue(std::cerr);
    std::cerr << std::endl;
#	endif

    RlString symbol = root.getFunctionName();

    return help( symbol.getValue() );
}


extern int yyparse(void);   // Defined in grammar.tab.cpp (from gammar.y)

/**
 * Process command buffer with the help of the bison-generated code.
 *
 * @param   command     command RlString; modified to contain any unprocessed incomplete statement residue, "" if none
 * @return  integer flag indicating status:
 *          1 - (last) statement in command buffer is incomplete
 *          2 - syntax error encountered
 *          0 - all statements in command buffer syntactically correct, complete and executed
 *
 * @note If an exception occurs on execution of a syntactically correct
 *       statement, the exception msg is printed here and the return
 *       signal is set to 2. Any remaining part of the command buffer
 *       is discarded.
 */
int RevLanguage::Parser::processCommand(std::string& command, Environment *env) {

//    extern Environment* executionEnvironment;
    
    executionEnvironment = env;
    
#ifdef DEBUG_PARSER
    std::cerr << "Processing command ... " << std::endl;
#endif
    
    // Break command into Rev lines
    std::list<std::string> lines;
    try {
        breakIntoLines( command, lines );
    }
    catch( RbException& rbException ) {
        
#ifdef DEBUG_PARSER
        printf("Caught an exception while breaking command buffer into lines\n");
#endif

        // Print message
        std::ostringstream msg;
        rbException.print(msg);
        msg << std::endl;
        RBOUT(msg.str());

        // We printed a message so we dealt with the problem
        // We exit immediately, discarding any command buffer content
        // We return 2 to signal a problem, which the caller may choose to ignore or act upon
        command = "";
        return 2;
    }

    // Initialize flex column count
    yycolumn = 1;

    // Call Bison code, which calls Flex code, which calls rrinput
    for ( std::list<std::string>::iterator i=lines.begin(); i!=lines.end(); i++ ) {

        /* prepare globals for call to parser */
        rrcommand.str( (*i) );
        rrcommand.clear();
        foundNewline = false;
        foundEOF = false;
        foundErrorBeforeEnd = false;
#ifdef DEBUG_PARSER
        printf("\nCalling bison with rrcommand:\n'%s'\n", rrcommand.str().c_str());
#endif

        int result;
        try {
            result = yyparse();
        }
        catch(RbException& rbException) {
#ifdef DEBUG_PARSER
            printf("Caught an exception\n");
#endif

            // Catch a quit request in case it was not caught before
            if (rbException.getExceptionType() == RbException::QUIT)
                exit(0);

            // All other uncaught exceptions
#ifdef DEBUG_PARSER
            printf("Abnormal exception during parsing or execution of statement; discarding any remaining command buffer\n");
#endif

            std::ostringstream msg;
            rbException.print(msg);
            msg << std::endl;
            RBOUT(msg.str());

            // We exit immediately, discarding any remaining buffer content
            // We return 2 to signal a problem, which the caller may choose to ignore or act upon
            command = "";
            return 2;
        }

        if ( result == 0 ) {
            
#ifdef DEBUG_PARSER
            printf("Parser successfully executed statement\n\n" );
#endif
        }
        else if ( result == 2 ) {

            // The execute function printed the error message resulting from the exception
            // We exit immediately, discarding any remaining buffer content
            // We return 2 to signal a problem, which the caller may choose to ignore or act upon
            command = "";
            return 2;
        }
        else if ( foundErrorBeforeEnd == true ) {
            
#ifdef DEBUG_PARSER
            printf("Syntax error detected by parser\n");
            printf("Parser discarding any remaining parts of command buffer\n\n");
#endif
            
            std::ostringstream msg;
            if ( yylloc.first_column == yylloc.last_column )
                msg << "Syntax error while reading character '" << command[yylloc.first_column-1] << "' at position " << yylloc.first_column << " in command:" << std::endl;
            else {
                msg << "Syntax error while reading characters \"";
                for (int i = yylloc.first_column; i <= yylloc.last_column; ++i) {
                    msg << command[i-1];
                }
                msg << "\" at position " << yylloc.first_column << " to " << yylloc.last_column << " in command:" << std::endl; 
            }
            msg << command;

            RBOUT( msg.str() );
            command = "";
            return 2;
        }
        else if ( foundNewline == true && foundEOF == false ) {
            
#ifdef DEBUG_PARSER     
            printf("Incomplete statement ending with inappropriate newline; replaced with space\n");
#endif
            
            /* Replace newline with space */
            std::string temp = (*i);
            temp[temp.size()-1] = ' ';

            std::list<std::string>::iterator j = i;
            j++;
            if ( j == lines.end() ) {

                /* If no more input lines, we need to ask for more */
#ifdef DEBUG_PARSER
                printf("Reached end; asking for more content to append to (last) statement.\n\n");
#endif
                command = temp;
                return 1;
            }
            else {

                /* If more input lines, put temp in front before proceeding */
#ifdef DEBUG_PARSER
                printf("Continuing with next Rev line in command buffer.\n\n");
#endif
                (*j) = temp + (*j);
            }
        }
        else if ( foundNewline == true && foundEOF == true ) {
            
#ifdef DEBUG_PARSER
            printf("Incomplete statement ending with appropriate newline.\n");
#endif
            
            std::list<std::string>::iterator j = i;
            j++;
            if ( j == lines.end() ) {
                
                /* If no more input lines, we need to ask for more */
#ifdef DEBUG_PARSER
                printf("Reached end; asking for more content to append to (last) statement.\n\n");
#endif
                command = (*i);
                return 1;
            }
            else {

                /* If more input lines, put current line in front before proceeding */
#ifdef DEBUG_PARSER
                printf("Continuing with next Rev line in command buffer.\n\n");
#endif
                (*j) = (*i) + (*j);
            }
        }
        else {
            
#ifdef DEBUG_PARSER
            printf("Unknown parse error\n");
#endif

            RBOUT( "Unknown parse error" );
            command = "";
            return 2;
        }
    }

    /* Successfully processed all statements in command buffer */
    command = "";
    return 0;
}


/** Global call-back function for the flex-generated code in lex.yy.cpp */
void rrinput(char* buf, size_t& result, size_t maxsize) {

    buf[0] = '\0';
    RevLanguage::Parser::getParser().getline(buf, maxsize);
    result = strlen(buf);
}
