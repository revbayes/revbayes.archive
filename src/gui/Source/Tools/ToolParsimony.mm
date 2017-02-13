#import "AnalysisView.h"
#import "Connection.h"
#import "GuiTree.h"
#import "Inlet.h"
#import "Node.h"
#import "Outlet.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbGuiHelper.h"
#import "RbTaxonData.h"
#import "RevBayes.h"
#import "ToolData.h"
#import "ToolLoop.h"
#import "ToolParsimony.h"
#import "ToolTreeSet.h"
#import "GuiTree.h"
#import "WindowControllerParsimony.h"

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <map>
#include <sstream>
#include <vector>



@implementation ToolParsimony

@synthesize searchMethod;
@synthesize hsSwap;
@synthesize hsKeep;
@synthesize hsMulTrees;
@synthesize hsRearrLimit;
@synthesize hsReconLimit;
@synthesize hsNBest;
@synthesize hsRetain;
@synthesize hsAllSwap;
@synthesize hsUseNonMin;
@synthesize hsSteepest;
@synthesize hsNChuck;
@synthesize hsChuckScore;
@synthesize hsAbortRep;
@synthesize hsRandomize;
@synthesize hsAddSeq;
@synthesize hsNReps;
@synthesize hsHold;
@synthesize bbKeep;
@synthesize bbMulTrees;
@synthesize bbUpBound;
@synthesize bbAddSeq;
@synthesize exKeep;

- (BOOL)checkForExecute:(NSMutableDictionary*)errors {

    // find the parent tool
    NSMutableArray* parents = [self getParentTools];
    if ([parents count] == 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Parsimony Tool does not have a parent" forKey:obId];
        return NO;
        }
    else if ([parents count] > 1)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Parsimony Tool has too many parents" forKey:obId];
        return NO;
        }
    if ( [[parents objectAtIndex:0] isKindOfClass:[ToolData class]] == NO )
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Parsimony Tool does not have a data tool as a parent" forKey:obId];
        return NO;
        }
    ToolData* dataTool = (ToolData*)[parents objectAtIndex:0];
    
    // check the data matrices in the parent tool
    if ( [dataTool numAligned] == 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Parsimony Tool does not have any data" forKey:obId];
        return NO;
        }
    if ( [dataTool numUnaligned] > 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Parsimony Tool does has too many data matrices" forKey:obId];
        return NO;
        }
    if ( [dataTool numUnaligned] > 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Parsimony Tool contains unaligned data" forKey:obId];
        return NO;
        }
    if ( [[dataTool dataMatrixIndexed:0] numTaxa] - [[dataTool dataMatrixIndexed:0] numExcludedTaxa] > 11 && searchMethod == EXHAUSTIVE )
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Too many taxa for an exhaustive search for the Parsimony Tool" forKey:obId];
        return NO;
        }
    if ( [[dataTool dataMatrixIndexed:0] numTaxa] - [[dataTool dataMatrixIndexed:0] numExcludedTaxa] < 4 )
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Too few taxa for the Parsimony Tool" forKey:obId];
        return NO;
        }

    return YES;
}

- (BOOL)checkForWarning:(NSMutableDictionary*)warnings {

    return YES;
}

- (void)closeControlPanelWithCancel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)closeControlPanelWithOK {

    [NSApp stopModal];
	[controlWindow close];

    if ( [self hasParents] == NO )
        return;
#   if 0
    [self startProgressIndicator];
    [self setStatusMessage:@"Performing parsimony analysis"];

    [NSThread detachNewThreadSelector:@selector(performToolTask)
                       toTarget:self
                     withObject:nil];
#   endif
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeObject:treeSetContainers forKey:@"treeSetContainers"];
    [aCoder encodeInt:treeLength           forKey:@"treeLength"];
    [aCoder encodeInt:searchMethod         forKey:@"searchMethod"];
    [aCoder encodeObject:hsSwap            forKey:@"hsSwap"];
    [aCoder encodeObject:hsKeep            forKey:@"hsKeep"];
    [aCoder encodeObject:hsMulTrees        forKey:@"hsMulTrees"];
    [aCoder encodeObject:hsRearrLimit      forKey:@"hsRearrLimit"];
    [aCoder encodeObject:hsReconLimit      forKey:@"hsReconLimit"];
    [aCoder encodeObject:hsNBest           forKey:@"hsNBest"];
    [aCoder encodeObject:hsRetain          forKey:@"hsRetain"];
    [aCoder encodeObject:hsAllSwap         forKey:@"hsAllSwap"];
    [aCoder encodeObject:hsUseNonMin       forKey:@"hsUseNonMin"];
    [aCoder encodeObject:hsSteepest        forKey:@"hsSteepest"];
    [aCoder encodeInt:hsNChuck             forKey:@"hsNChuck"];
    [aCoder encodeObject:hsChuckScore      forKey:@"hsChuckScore"];
    [aCoder encodeObject:hsAbortRep        forKey:@"hsAbortRep"];
    [aCoder encodeObject:hsRandomize       forKey:@"hsRandomize"];
    [aCoder encodeObject:hsAddSeq          forKey:@"hsAddSeq"];
    [aCoder encodeInt:hsNReps              forKey:@"hsNReps"];
    [aCoder encodeObject:hsHold            forKey:@"hsHold"];
    [aCoder encodeObject:bbKeep            forKey:@"bbKeep"];
    [aCoder encodeObject:bbMulTrees        forKey:@"bbMulTrees"];
    [aCoder encodeDouble:bbUpBound         forKey:@"bbUpBound"];
    [aCoder encodeObject:bbAddSeq          forKey:@"bbAddSeq"];
    [aCoder encodeObject:exKeep            forKey:@"exKeep"];
    
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

    [self startProgressIndicator];
    [self setStatusMessage:@"Performing parsimony analysis"];
    BOOL errors = [self performToolTask];
    [self setStatusMessage:@""];
    [self stopProgressIndicator];

    return [super execute];
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
        // initialize values
        treeLength = -1;
        [self setSearchMethod:HEURISTIC];
        [self setHsSwap:@"TBR"];
        [self setHsKeep:@"No"];
        [self setHsMulTrees:@"Yes"];
        [self setHsRearrLimit:@"None"];
        [self setHsReconLimit:@"Infinity"];
        [self setHsNBest:@"All"];
        [self setHsRetain:@"No"];
        [self setHsAllSwap:@"No"];
        [self setHsUseNonMin:@"No"];
        [self setHsSteepest:@"No"];
        [self setHsNChuck:0];
        [self setHsChuckScore:@"No"];
        [self setHsAbortRep:@"No"];
        [self setHsRandomize:@"AddSeq"];
        [self setHsAddSeq:@"Simple"];
        [self setHsNReps:10];
        [self setHsHold:@"1"];
        [self setBbKeep:@"No"];
        [self setBbMulTrees:@"No"];
        [self setBbUpBound:0];
        [self setBbAddSeq:@"Furthest"];
        [self setExKeep:@"No"];

		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        treeSetContainers = [[NSMutableArray alloc] init];
		
		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor greenColor]];
		[self addOutletOfColor:[NSColor redColor]];
        [self setInletLocations];
        [self setOutletLocations];
            
		// initialize the control window
		controlWindow = [[WindowControllerParsimony alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        // initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        treeSetContainers = [aDecoder decodeObjectForKey:@"treeSetContainers"];
        treeLength        = [aDecoder decodeIntForKey:@"treeLength"];
        searchMethod      = [aDecoder decodeIntForKey:@"searchMethod"];
        hsSwap            = [aDecoder decodeObjectForKey:@"hsSwap"];
        hsKeep            = [aDecoder decodeObjectForKey:@"hsKeep"];
        hsMulTrees        = [aDecoder decodeObjectForKey:@"hsMulTrees"];
        hsRearrLimit      = [aDecoder decodeObjectForKey:@"hsRearrLimit"];
        hsReconLimit      = [aDecoder decodeObjectForKey:@"hsReconLimit"];
        hsNBest           = [aDecoder decodeObjectForKey:@"hsNBest"];
        hsRetain          = [aDecoder decodeObjectForKey:@"hsRetain"];
        hsAllSwap         = [aDecoder decodeObjectForKey:@"hsAllSwap"];
        hsUseNonMin       = [aDecoder decodeObjectForKey:@"hsUseNonMin"];
        hsSteepest        = [aDecoder decodeObjectForKey:@"hsSteepest"];
        hsNChuck          = [aDecoder decodeIntForKey:@"hsNChuck"];
        hsChuckScore      = [aDecoder decodeObjectForKey:@"hsChuckScore"];
        hsAbortRep        = [aDecoder decodeObjectForKey:@"hsAbortRep"];
        hsRandomize       = [aDecoder decodeObjectForKey:@"hsRandomize"];
        hsAddSeq          = [aDecoder decodeObjectForKey:@"hsAddSeq"];
        hsNChuck          = [aDecoder decodeIntForKey:@"hsNChuck"];
        hsNReps           = [aDecoder decodeIntForKey:@"hsNReps"];
        hsHold            = [aDecoder decodeObjectForKey:@"hsHold"];
        bbKeep            = [aDecoder decodeObjectForKey:@"bbKeep"];
        bbMulTrees        = [aDecoder decodeObjectForKey:@"bbMulTrees"];
        bbUpBound         = [aDecoder decodeDoubleForKey:@"bbUpBound"];
        bbAddSeq          = [aDecoder decodeObjectForKey:@"bbAddSeq"];
        exKeep            = [aDecoder decodeObjectForKey:@"exKeep"];
            
        // initialize the control window
		controlWindow = [[WindowControllerParsimony alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Parsimony25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Parsimony50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Parsimony75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Parsimony100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Parsimony125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Parsimony150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Parsimony200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Parsimony400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)paupFinished:(NSString*)paupDirectory {

    if (numberErrors == 0)
        {
        // read trees
        RbGuiHelper* helper = [[RbGuiHelper alloc] init];
        NSMutableArray* myTrees = [helper readTreesFromFile:paupDirectory];
        float treeWeight = 1.0;
        if ([myTrees count] > 1)
            treeWeight = (float)1.0 / [myTrees count];
        for (unsigned i=0; i<[myTrees count]; i++)
            {
            // TO DO: We should pass a copy of the tree to the Tree set in case there are multiple treesets downstream of this tool
            for (size_t ts=0; ts<[treeSetContainers count]; ts++)
                {
                ToolTreeSet* treeSet = [treeSetContainers objectAtIndex:ts];
                GuiTree* t = [myTrees objectAtIndex:i];
                [t setWeight:treeWeight];
                if (treeLength != -1)
                    {
                    if ( [loopMembership count] > 0)
                        {
                        NSString* infoStr = @"Parsimony length (";
                        for (ToolLoop* loop in loopMembership)
                            {
                            if ([loop isExecuting] == YES)
                                infoStr = [infoStr stringByAppendingFormat:@"%c:%d, ", [loop indexLetter], [loop currentIndex]];
                            }
                        infoStr = [infoStr stringByAppendingFormat:@"%d of %d) = %d", i+1, (int)[myTrees count], treeLength];
                        [t setInfo:infoStr];
                        }
                    else
                        {
                        NSString* infoStr = [NSString stringWithFormat:@"Parsimony length (%d of %d) = %d", i+1, (int)[myTrees count], treeLength];
                        [t setInfo:infoStr];
                        }
                    }
                [self sendTree:t toTreeSet:treeSet];
                }
            if (i == 0)
                {
                for (size_t ts=0; ts<[treeSetContainers count]; ts++)
                    {
                    ToolTreeSet* treeSet = [treeSetContainers objectAtIndex:ts];
                    [treeSet setOutgroupName:[(GuiTree*)[myTrees objectAtIndex:i] outgroupName]];
                    }
                }
            }

        //TEMP!!!
        [self removeFilesFromTemporaryDirectory];
        NSLog(@"paupDirectory = %@", paupDirectory);
        }
    else
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error in Parsimony Analysis"];
        [alert setInformativeText:@"One or more errors occurred during parsimony search."];
        [alert runModal];
        }
}

- (BOOL)paupSearch {

    numberErrors = 0;
    
    // find the parent of this tool, which should be an instance of ToolData
    ToolData* dataTool = nil;
    for (size_t i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (int j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:j];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isKindOfClass:[ToolData class]] == YES )
                dataTool = (ToolData*)t;
            }
        }
    if ( dataTool == nil )
        return NO;

    // calculate how many aligned data matrices exist
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
        
    // get a pointer to the single RbData object
    RbData* d = nil;
    if ( [alignedData count] == 0 )
        {
        return NO;
        }
    else if ( [alignedData count] == 1 )
        {
        d = [alignedData objectAtIndex:0];
        }
    else
        {
        return NO;
        }
        
    // check to see if a tree container is downstream of this tool
    [treeSetContainers removeAllObjects];
    for (size_t i=0; i<[outlets count]; i++)
        {
        Outlet* theOutlet = [outlets objectAtIndex:i];
        for (int j=0; j<[theOutlet numberOfConnections]; j++)
            {
            Connection* c = [theOutlet connectionWithIndex:j];
            Tool* t = [[c inlet] toolOwner];
            if ( [t isKindOfClass:[ToolTreeSet class]] == YES )
                {
                [treeSetContainers addObject:t];
                }
            }
        }
                
    // remove all of the files from the temporary directory
    [self removeFilesFromTemporaryDirectory];
    
    // and make a temporary directory to contain the alignments
    NSString* temporaryDirectory = NSTemporaryDirectory();

    // write the data to the temporary directory
    NSString* modifiedFileName = [NSString stringWithString:[d name]];
    modifiedFileName = [modifiedFileName stringByReplacingOccurrencesOfString:@" " withString:@"_"];
    NSString* dFilePath = [NSString stringWithString:temporaryDirectory];
              dFilePath = [dFilePath stringByAppendingString:modifiedFileName];
              dFilePath = [dFilePath stringByAppendingString:@".nex"];
    [d writeToFile:dFilePath];
    
    // write the file with the nexus commands to the same file
    // write the data to the temporary directory
    NSString* nFilePath = [NSString stringWithString:temporaryDirectory];
              nFilePath = [nFilePath stringByAppendingString:@"nexcmds.nex"];
    NSString* tFilePath = [NSString stringWithString:temporaryDirectory];
              tFilePath = [tFilePath stringByAppendingString:@"ptrees.trees"];
    NSString* cmds = @"#NEXUS\n";
    cmds = [cmds stringByAppendingString:@"begin paup;\n"];
    cmds = [cmds stringByAppendingString:@"set AutoClose=yes WarnReset=no Increase=auto Criterion=parsimony NotifyBeep=no ErrorBeep=no WarnTSave=no;\n"];
    cmds = [cmds stringByAppendingFormat:@"execute %@;", dFilePath];
    
    NSArray* deletedChars = [d getExcludedCharacters];
    NSArray* deletedTaxa  = [d getExcludedTaxa];
    if ( [deletedChars count] > 0 )
        {
        cmds = [cmds stringByAppendingString:@"exclude"];
        for (NSNumber* n in deletedChars)
            cmds = [cmds stringByAppendingFormat:@" %@", n];
        cmds = [cmds stringByAppendingString:@";"];
        }
    if ( [deletedTaxa count] > 0 )
        {
        cmds = [cmds stringByAppendingString:@"delete"];
        for (NSString* s in deletedTaxa)
            cmds = [cmds stringByAppendingFormat:@" %@", s];
        cmds = [cmds stringByAppendingString:@";"];
        }
    
    if (searchMethod == EXHAUSTIVE)
        {
        cmds = [cmds stringByAppendingFormat:@"alltrees keep=%@;\n", exKeep];
        }
    else if (searchMethod == BANDB)
        {
        cmds = [cmds stringByAppendingFormat:@"bandb keep=%@ multrees=%@ addseq=%@ upbound=%1.3lf;\n", bbKeep, bbMulTrees, bbAddSeq, bbUpBound];
        }
    else
        {
        cmds = [cmds stringByAppendingFormat:@"hsearch keep=%@ swap=%@ multrees=%@ RearrLimit=%@ ReconLimit=%@ NBest=%@ Retain=%@ AllSwap=%@ UseNonMin=%@ Steepest=%@ NChuck=%d ChuckScore=%@ AbortRep=%@ Randomize=%@ AddSeq=%@ NReps=%d Hold=%@;\n",
                hsKeep, hsSwap, hsMulTrees, hsRearrLimit, hsReconLimit, hsNBest, hsRetain, hsAllSwap, hsUseNonMin, hsSteepest, hsNChuck, hsChuckScore, hsAbortRep, hsRandomize, hsAddSeq, hsNReps, hsHold];
        }
    cmds = [cmds stringByAppendingFormat:@"savetrees file=%@ format=nexus replace=yes;\n", tFilePath];
    cmds = [cmds stringByAppendingString:@"quit;\n"];
    cmds = [cmds stringByAppendingString:@"end;\n"];
    [cmds writeToFile:nFilePath atomically:YES encoding:NSASCIIStringEncoding error:nil];
    
    // get the path to the paup binary
    NSString* paupPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"paup"];
    
    // set the paup argument
    NSString* paupArg = [NSString stringWithString:nFilePath];
    NSArray* paupArguments = [NSArray arrayWithObjects:paupArg,nil];

    // create the task and launch
    // allocate a task for clustal
    NSTask* paupTask = [[NSTask alloc] init];
    NSPipe* outputPipe = [[NSPipe alloc] init];
    [paupTask setCurrentDirectoryPath:temporaryDirectory];
    [paupTask setLaunchPath:paupPath];
    [paupTask setArguments:paupArguments];
    NSFileHandle* outputFileHandle = [outputPipe fileHandleForReading];
    [paupTask setStandardOutput:outputPipe];

    // launch the task and wait
    [paupTask launch];

    [outputFileHandle waitForDataInBackgroundAndNotify];

    // read the output
    NSData* inData = nil;
    treeLength = -1;
    while ( (inData = [outputFileHandle availableData]) && [inData length] )
        {
        NSString* msg = [[NSString alloc] initWithData:inData encoding:NSASCIIStringEncoding];
        NSRange r = [msg rangeOfString:@"Score of best tree(s) found = "];
        if (r.location == NSNotFound)
            r = [msg rangeOfString:@"Score of best tree found = "];
        std::string s = "";
        if (r.location != NSNotFound)
            {
            for (int i=0; i<10; i++)
                {
                char c = [msg characterAtIndex:(r.location + r.length + i)];
                if (c == ' ')
                    break;
                else
                    s += c;
                }
            int x = 0;
            std::stringstream sStrm;
            sStrm << s;
            sStrm >> x;
            treeLength = x;
            }
        //NSLog(@"msg => \"%@\"", msg);
        }

    [paupTask waitUntilExit];
    int status = [paupTask terminationStatus];
    if (status != 0)
        numberErrors++;

    // read the alignments on the main thread to prevent errors on graphics.
#   if 1
    [self paupFinished:tFilePath];
#   else
    [self performSelectorOnMainThread:@selector(paupFinished:)
                         withObject:tFilePath
                      waitUntilDone:NO];
#   endif

    return YES;
}

- (BOOL)performToolTask {

    return [self paupSearch];
}

- (void)prepareForExecution {

}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Parsimony Tool ";
    if ([self isFullyConnected] == YES)
        myTip = [myTip stringByAppendingString:@"\n Fully Connected "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Missing Connections "];

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];

    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (BOOL)sendTree:(GuiTree*)t toTreeSet:(ToolTreeSet*)treeSet {
    
    if (treeSet != nil)
        {
        [treeSet addTreeToSet:t];
        return YES;
        }
    return NO;
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (NSString*)toolName {

    return @"Parsimony";
}

- (void)updateForChangeInParent {

    
    // find the parent of this tool, which should be an instance of ToolData
    ToolData* dataTool = nil;
    for (int i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (int j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:j];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isKindOfClass:[ToolData class]] == YES )
                dataTool = (ToolData*)t;
            }
        }
    
    if ( [[dataTool className] isEqualToString:@"ToolSimulate"] == YES )
        {
        return;
        }

    // calculate how many aligned data matrices exist
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
}

@end
