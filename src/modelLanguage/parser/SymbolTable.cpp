/*!
 * \file This file contains the implementation of SymbolTable, which is
 * used to hold the symbol table used by the Parser class.
 *
 * \brief Implementation of SymbolTable
 *
 * MrBayes version 4.0 beta
 *
 * (c) Copyright 2005-
 * \version 4.0 Beta
 * \date Last modified: $Date: 2009/02/03 16:26:33 $
 * \author John Huelsenbeck (1)
 * \author Bret Larget (2)
 * \author Paul van der Mark (3)
 * \author Fredrik Ronquist (4)
 * \author Donald Simon (5)
 * \author (authors listed in alphabetical order)
 * (1) Department of Integrative Biology, University of California, Berkeley
 * (2) Departments of Botany and of Statistics, University of Wisconsin - Madison
 * (3) Department of Scientific Computing, Florida State University
 * (4) Department of Entomology, Swedish Museum of Natural History, Stockholm
 * (5) Department of Mathematics/Computer Science, Duquesne University
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License (the file gpl.txt included with this
 * distribution or http://www.gnu.org/licenses/gpl.txt) for more
 * details.
 *
 * $Id: SymbolTable.cpp,v 1.1 2009/02/03 16:26:33 ronquist Exp $
 */

#include "MbAttribute.h"
#include "SymbolTable.h"


//! Parser-handled objects
//  ======================
//
//  Include the header file for all model objects that can be created by the parser
//  here.
#include "DistributionGamma.h"
#include "MbFunctions.h"
#include "Model.h"
#include "MoveSlidingWindow.h"
#include "ParameterPosReal.h"


//! We use a macro here to make it easy for programmers to insert new MbObject creator
//  functions into the predefined symbol table
#define MBOBJECTCREATOR(x)  x::funcName(),new FunctionMbObjectConstructor(new x(x::attributeTemplate))


//! We use another macro to make it easy to insert new move creator functions. These allow
//  the user to see a list of available moves for a given parameter.
//
//  ??Old view?? The map is also needed for the addmove function, which uses the moves
//  in the map as templates for the construction of new moves and also as a key to interpreting the
//  string entered by the user.
#define MOVECREATOR(x)  x::getMoveName(),new x()


using namespace std;


//! Symbol table constructor. Here we set up the initial symbol table, which
//  includes all the predefined objects. Any objects that should be created
//  and managed by the parser need to have a constructor function inserted here.
SymbolTable::SymbolTable() : table() {

    //! Regular functions
    insert("exit", new FunctionExit());
    insert("quit", new FunctionExit());
    insert("c", new FunctionC());

    /*
    symbolTable.insert("analysis", new MbObjectVector(new funcAnalysis));
    symbolTable.insert("ls", new MbObjectVector(new funcLs));
    symbolTable.insert("mcmc", new MbObjectVector(new funcMcmc));
    symbolTable.insert("read.chars", new MbObjectVector(new funcReadChars));
    symbolTable.insert("rep", new MbObjectVector(new funcRep));
    symbolTable.insert("seq", new MbObjectVector(new funcSeq));
    symbolTable.insert("sites.nuc", new MbObjectVector(new funcSitesNuc));
    symbolTable.insert("sites.protein", new MbObjectVector(new funcSitesProtein));
    symbolTable.insert("sites.standard", new MbObjectVector(new funcSitesStandard));
    symbolTable.insert("source", new MbObjectVector(new funcSource));
    */

    /* \todo ideas for more functions */
    // add.move: add a named move to the move vector of a parameter using the static getMoveName() function
    // delete.move: delete a move from the move vector of a parameter
    // list.moves: list moves for a parameter, attribute moves=current|all
    //
    // functions that set starting values and tuning parameters of the members of a static model
    // held in an analysis object. Something like myanalysis[1,1]$<parm>$val <-
    // where <parm> is the identifier of a model object; a list of model objects can be obtained
    // with a command such as list.model(myanalysis)

    //! Functions that create MbObject objects
    insert(MBOBJECTCREATOR( DistributionGamma ));
    insert(MBOBJECTCREATOR( ParameterPosReal ));
    insert(MBOBJECTCREATOR( Model ));
    
    /* \todo Add later
    symbolTable.insert(MBOBJECTCREATOR(DistributionSimplexOrdered));
    symbolTable.insert(MBOBJECTCREATOR(DistributionClockTreeUniform));
    */

    /*  \todo convert to new format
    symbolTable.insert("distribution.simplexorderd",new MbObjectVector(new funcDistributionSimplexOrdered));
    symbolTable.insert("distribution.topologyuniform", new MbObjectVector(new funcDistributionTopologyUniform));
    symbolTable.insert("model.ctmc", new MbObjectVector(new funcModelCtmc));
    symbolTable.insert("model.mixture", new MbObjectVector(new funcModelMixture));
    symbolTable.insert("model.partition", new MbObjectVector(new funcModelPartition));
    symbolTable.insert("model.invariablesitemixture", new MbObjectVector(new funcModelInvariableSiteMixture));
    symbolTable.insert("model.sitemixture", new MbObjectVector(new funcModelSiteMixture));
    symbolTable.insert("model.siteratedistribution", new MbObjectVector(new funcModelSiteRateDistribution));
    symbolTable.insert("model.siteratemixture", new MbObjectVector(new funcModelSiteRateMixture));
    symbolTable.insert("parameter.brlens", new MbObjectVector(new funcParameterBrLens));
    symbolTable.insert("parameter.clocktree", new MbObjectVector(new funcParameterClockTree));
    symbolTable.insert("parameter.fixed",new MbObjectVector(new funcParameterFixed));
    symbolTable.insert("parameter.irrevrate",new MbObjectVector(new funcParameterIrrevRate));
    symbolTable.insert("parameter.ratemult",new MbObjectVector(new funcParameterRateMult));
    symbolTable.insert("parameter.revrate",new MbObjectVector(new funcParameterRevRate));
    symbolTable.insert("parameter.scalar",new MbObjectVector(new funcParameterScalar));
    symbolTable.insert("parameter.simplex",new MbObjectVector(new funcParameterSimplex));
    symbolTable.insert("parameter.simplexorderd",new MbObjectVector(new funcParameterSimplexOrdered));
    symbolTable.insert("parameter.statefreq",new MbObjectVector(new funcParameterStateFreq));
    symbolTable.insert("parameter.steprate",new MbObjectVector(new funcParameterStepRate));
    symbolTable.insert("parameter.topology",new MbObjectVector(new funcParameterTopology));
    symbolTable.insert("substmodel.equalin", new MbObjectVector(new funcSubstModelEqualin));    
    symbolTable.insert("substmodel.gtr", new MbObjectVector(new funcSubstModelGTR));    
    symbolTable.insert("substmodel.jc", new MbObjectVector(new funcSubstModelJC));
    symbolTable.insert("substmodel.equalinstep", new MbObjectVector(new funcSubstModelEqualinStep));    
    symbolTable.insert("substmodel.gtrstep", new MbObjectVector(new funcSubstModelGTRStep));    
    symbolTable.insert("substmodel.jcstep", new MbObjectVector(new funcSubstModelJCStep));
    symbolTable.insert("substmodel.irrev", new MbObjectVector(new funcSubstModelIrrev));
    symbolTable.insert("treemodel", new MbObjectVector(new funcTreeModel));
    symbolTable.insert("treemodel.clock", new MbObjectVector(new funcTreeModelClock));
    */

    //! Functions that create moves
    /*
    moveMap.insert(MOVE(MoveDirichlet));
    moveMap.insert(MOVE(MoveMultiplier));
    moveMap.insert(MOVE(MoveNNI));
    moveMap.insert(MOVE(MoveNormal));
    */
    insert(MOVECREATOR( MoveSlidingWindow ));
}


//! Destructor: remove reference from objects and delete if appropriate
SymbolTable::~SymbolTable() {
    for(map<const string, MbObject *>::iterator i = table.begin(); i!=table.end(); i++) {
        i->second->removeRef();
        if (i->second->getRefCount()==0 && i->second->getAffectedObjects().size()==0)
            delete i->second;
    }
}


//! Erase an entry from the symbol table
void SymbolTable::erase(const string &name) {
    map<const string, MbObject *>::iterator i;
    i=table.find(name);
    if(i!=table.end())
        table.erase(i);
}


//! Add an entry to the symbol table
void SymbolTable::insert(const string &name, MbObject *entry) {
    table.insert(pair<const string, MbObject *>(name,entry));
    entry->addRef(name);
}


//! Check if an entry exists
bool SymbolTable::exists(const string &name) const {
    map<const string, MbObject *>::const_iterator i;
    i=table.find(name);
    if(i==table.end())
        return false;
    else
        return true;
}


//! Get entry in symbol table
MbObject *SymbolTable::getEntry(string &name) {
    if (exists(name))
        return table.find(name)->second;
    else
        return NULL;
}


//! Print symbol table
void SymbolTable::print(ostream &c) const {
    int count=1;
    for(map<const string, MbObject *>::const_iterator i=table.begin();i!=table.end();i++,count++) {
        c << count << " Key=" << (*i).first << " Value=";
        (*i).second->print(c);
        c << "\n";
    }
}
