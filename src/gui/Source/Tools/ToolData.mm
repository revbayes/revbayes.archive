#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "ToolData.h"
#import "WindowControllerCharacterMatrix.h"
#include <string>
#include <cmath>
#include "AminoAcidState.h"
#include "CharacterState.h"
#include "AbstractDiscreteTaxonData.h"
#include "ContinuousCharacterData.h"
#include "DnaState.h"
#include "Parser.h"
#include "AbstractCharacterData.h"
#include "HomologousCharacterData.h"
#include "NonHomologousCharacterData.h"
#include "RbMathLogic.h"
#include "RnaState.h"
#include "StandardState.h"
#include "AbstractTaxonData.h"
#include "Workspace.h"



@implementation ToolData

@synthesize dataWorkspaceName;
@synthesize numAligned;
@synthesize numUnaligned;

- (void)addMatrix:(RbData*)m {

    if ( [m isHomologyEstablished] == YES )
        numAligned++;
    else
        numUnaligned++;
	[dataMatrices addObject:m];
    hasInspectorInfo = YES;
}

- (void)awakeFromNib {

}

- (NSMutableArray*)dataMatrices {

    return dataMatrices;
}

- (RbData*)dataMatrixIndexed:(size_t)i {

	if (i > [dataMatrices count])
		return nil;
	return [dataMatrices objectAtIndex:i];
}

- (NSMutableArray*)getAlignedData {

    NSMutableArray* arr = [NSMutableArray arrayWithCapacity:0];
    for (size_t i=0; i<[dataMatrices count]; i++)
        {
        RbData* d = [dataMatrices objectAtIndex:i];
        if ( [d isHomologyEstablished] == YES )
            [arr addObject:d];
        }
    return arr;
}

- (NSMutableArray*)getUnalignedData {

    NSMutableArray* arr = [NSMutableArray arrayWithCapacity:0];
    for (size_t i=0; i<[dataMatrices count]; i++)
        {
        RbData* d = [dataMatrices objectAtIndex:i];
        if ( [d isHomologyEstablished] == NO )
            [arr addObject:d];
        }
    return arr;
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:dataMatrices      forKey:@"dataMatrices"];
    [aCoder encodeObject:dataWorkspaceName forKey:@"dataWorkspaceName"];
       
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

    if (isResolved == NO)
        return NO;
    
    //[self startProgressIndicator];
    [self instantiateDataInCore];
    //[self stopProgressIndicator];
    
    return YES;
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
		dataMatrices      = [[NSMutableArray alloc] init];
        dataWorkspaceName = @"";
        dataInspector     = nil;
        
        // check to see if there are any data matrices stored in the tool
        if ([dataMatrices count] > 0)
            hasInspectorInfo = YES;
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        dataMatrices      = [aDecoder decodeObjectForKey:@"dataMatrices"];
        dataWorkspaceName = [aDecoder decodeObjectForKey:@"dataWorkspaceName"];
        dataInspector     = nil;

        // check to see if there are any data matrices stored in the tool
        if ([dataMatrices count] > 0)
            hasInspectorInfo = YES;

        // make certain that the data matrices are instantiated in the core
        if ([dataMatrices count] > 0)
            [self makeDataInspector];
        
        numAligned   = [self numAlignedMatrices];
        numUnaligned = [self numUnalignedMatrices];
		}
	return self;
}

- (void)initializeImage {

}

- (void)instantiateDataInCore {

    // check that we have data to instantiate in the core
    if ( [self numDataMatrices] == 0 )
        return;
    
    // check to see if we already have data in the core, in which case we erase the data
    if ( [[self dataWorkspaceName] isEqualToString:@""] == NO )
        {
        const char* tempSeeStr = [[self dataWorkspaceName] UTF8String];
        std::string tempStr = tempSeeStr;
        if ( RevLanguage::Workspace::userWorkspace().existsVariable(tempStr) == true )
            RevLanguage::Workspace::userWorkspace().eraseVariable(tempStr);
        [self setDataWorkspaceName:@""];
        }
        
    // remove all of the files from the temporary directory
    [self removeFilesFromTemporaryDirectory];
        
    // and make a temporary directory to contain the alignments
    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[NSFileManager alloc] init];
    NSString* alnDirectory = [NSString stringWithString:temporaryDirectory];
              alnDirectory = [alnDirectory stringByAppendingString:@"myAlignments/"];
    NSDictionary* dirAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"dirAttributes"];
    [fm createDirectoryAtPath:alnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];

    // write the data matrix/matrices in this tool to the temporary directory
    for (size_t i=0; i<[dataMatrices count]; i++)
        {
        // have the data object save a file to the temporary directory
        RbData* d = [dataMatrices objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:alnDirectory];
                  dFilePath = [dFilePath stringByAppendingString:[d name]];
        if ( [d isHomologyEstablished] == YES )
            dFilePath = [dFilePath stringByAppendingString:@".nex"];
        else
            dFilePath = [dFilePath stringByAppendingString:@".fas"];
        [d writeToFile:dFilePath];
        }

    // check the workspace and make certain that we use an unused name for the data variable
    std::string variableName = RevLanguage::Workspace::userWorkspace().generateUniqueVariableName();
    NSString* nsVariableName = [NSString stringWithCString:variableName.c_str() encoding:NSUTF8StringEncoding];
		  
    // format a string command to read the data file(s) and send the
    // formatted string to the parser
    const char* cmdAsCStr = [alnDirectory UTF8String];
    std::string cmdAsStlStr = cmdAsCStr;
    std::string line = variableName + " <- readDiscreteCharacterData(\"" + cmdAsStlStr + "\")";
    int coreResult = RevLanguage::Parser::getParser().processCommand(line, &RevLanguage::Workspace::userWorkspace());
    if (coreResult != 0)
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return;
        }

    // set the variable name for this tool
    [self setDataWorkspaceName:nsVariableName];

    // remove all of the files from the temporary directory
    [self removeFilesFromTemporaryDirectory];
}

- (void)makeDataInspector {

    [self removeDataInspector];
    dataInspector = [[WindowControllerCharacterMatrix alloc] initWithTool:self];
    [dataInspector window];
}

- (RbData*)makeNewGuiDataMatrixFromCoreMatrixWithAddress:(const RevBayesCore::AbstractCharacterData&)cd andDataType:(const std::string&)dt {

    std::string fn = cd.getFileName();
    
    NSString* nsfn = [NSString stringWithCString:(fn.c_str()) encoding:NSUTF8StringEncoding];
    RbData* m = [[RbData alloc] init];
    [m setNumTaxa:(int)(cd.getNumberOfTaxa())];
    if ( cd.isHomologyEstablished() == true )
        {
        [m setIsHomologyEstablished:YES];
        const RevBayesCore::HomologousCharacterData* hd = dynamic_cast<const RevBayesCore::HomologousCharacterData*>(&cd);
        if (!hd)
            {
            
            }
        [m setNumCharacters:(int)(hd->getNumberOfCharacters())];
        }
    else
        {
        [m setIsHomologyEstablished:NO];
        const RevBayesCore::NonHomologousCharacterData* nhd = dynamic_cast<const RevBayesCore::NonHomologousCharacterData*>(&cd);
        if (!nhd)
            {
            
            }
        std::vector<size_t> sequenceLengths = nhd->getNumberOfCharacters();
        size_t maxLen = 0;
        for (int i=0; i<sequenceLengths.size(); i++)
            {
            if (sequenceLengths[i] > maxLen)
                maxLen = sequenceLengths[i];
            }
        [m setNumCharacters:(int)maxLen];
        }
    
    // get the state labels
    std::string stateLabels = cd.getStateLabels();
    NSString* sl = [NSString stringWithCString:(stateLabels.c_str()) encoding:NSUTF8StringEncoding];
    [m setStateLabels:sl];
    
    [m setName:nsfn];
    if ( dt == "DNA" )
        [m setDataType:DNA];
    else if ( dt == "RNA" )
        [m setDataType:RNA];
    else if ( dt == "Protein" )
        [m setDataType:AA];
    else if ( dt == "Standard" )
        [m setDataType:STANDARD];
    else if ( dt == "Continuous" )
        [m setDataType:CONTINUOUS];

    for (size_t i=0; i<cd.getNumberOfTaxa(); i++)
        {
        const RevBayesCore::AbstractTaxonData& td = cd.getTaxonData(i);
        NSString* taxonName = [NSString stringWithCString:td.getTaxonName().c_str() encoding:NSUTF8StringEncoding];
        [m cleanName:taxonName];
        [m addTaxonName:taxonName];
        RbTaxonData* rbTaxonData = [[RbTaxonData alloc] init];
        [rbTaxonData setTaxonName:taxonName];
        for (size_t j=0; j<td.getNumberOfCharacters(); j++)
            {
            RbDataCell* cell = [[RbDataCell alloc] init];
            [cell setDataType:[m dataType]];
            if ( [m dataType] != CONTINUOUS )
                {
                const RevBayesCore::DiscreteCharacterState& theChar = static_cast<const RevBayesCore::AbstractDiscreteTaxonData &>(td).getCharacter(j);
                unsigned int x = (unsigned int)static_cast<const RevBayesCore::DiscreteCharacterState &>(theChar).getState();
                NSNumber* n = [NSNumber numberWithUnsignedInt:x];
                [cell setVal:n];
                [cell setIsDiscrete:YES];
                [cell setNumStates:((int)theChar.getNumberOfStates())];
                if ( theChar.isAmbiguous() == true )
                    [cell setIsAmbig:YES];
                if (theChar.isGapState() == true)
                    [cell setIsGapState:YES];
                else
                    [cell setIsGapState:NO];
                }
            else
                {
                const double x = static_cast<const RevBayesCore::ContinuousCharacterData &>(cd).getCharacter(i, j);
                if ( RevBayesCore::RbMath::isNan(x) )
                    {
                    [cell setIsAmbig:YES];
                    }
                else
                    {
                    NSNumber* n = [NSNumber numberWithDouble:x];
                    [cell setVal:n];
                    [cell setIsDiscrete:NO];
                    [cell setNumStates:0];
                    }
                }
            [cell setRow:i];
            [cell setColumn:j];
            [rbTaxonData addObservation:cell];
            }
        [m addTaxonData:rbTaxonData];
        }
    
    return m;
}

- (NSString*)nameOfMatrixIndexed:(int)idx {

    return [[dataMatrices objectAtIndex:idx] name];
}

- (size_t)numDataMatrices {

	return [dataMatrices count];
}

- (size_t)numAlignedMatrices {

    size_t n = 0;
    NSEnumerator* matrixEnumerator = [dataMatrices objectEnumerator];
    RbData* d = nil;
    while ( (d = [matrixEnumerator nextObject]) )
        {
        if ( [d isHomologyEstablished] == YES )
            n++;
        }
    return n;
}

- (size_t)numUnalignedMatrices {

    size_t n = 0;
    NSEnumerator* matrixEnumerator = [dataMatrices objectEnumerator];
    RbData* d = nil;
    while ( (d = [matrixEnumerator nextObject]) )
        {
        if ( [d isHomologyEstablished] == NO )
            n++;
        }
    return n;
}

- (void)readDataError:(NSString*)eName forVariableNamed:(NSString*)vName {

    NSAlert* alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Problem Reading Data"];
    [alert setInformativeText:eName];
    [alert runModal];
    //NSRunAlertPanel(@"Problem Reading Data", eName, @"OK", nil, nil);

    std::string tempName = [vName UTF8String];
    if ( RevLanguage::Workspace::userWorkspace().existsVariable(tempName) )
        RevLanguage::Workspace::userWorkspace().eraseVariable(tempName);
    [self removeAllDataMatrices];
}

- (void)removeAllDataMatrices {

	[dataMatrices removeAllObjects];
    numAligned = 0;
    numUnaligned = 0;
    hasInspectorInfo = NO;
    isResolved = NO;
    [self removeDataInspector];
}

- (void)removeDataInspector {

    dataInspector = nil;
}

- (void)showInspectorPanel {

    if (dataInspector == nil)
        NSLog(@"No idea why we don't have a data inspector");
    [[dataInspector window] center];
    [dataInspector showWindow:self];
}

- (void)updateForChangeInUpstreamState {

}

@end
