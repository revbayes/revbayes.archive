#import "AlignmentTask.h"
#import "AnalysisView.h"
#import "RbData.h"
#import "Connection.h"
#import "RbData.h"
#import "Inlet.h"
#import "InOutlet.h"
#import "Outlet.h"
#import "RevBayes.h"
#import "ToolAlign.h"
#import "ToolReadData.h"
#import "WindowControllerAlign.h"
#import "WindowControllerCharacterMatrix.h"
#include <string>

#include "CharacterState.h"
#include "AbstractCharacterData.h"
#include "NclReader.h"
#include "Parser.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "Workspace.h"



@implementation ToolAlign

@synthesize alignmentMethod;
@synthesize clustalAlign;
@synthesize clustalWordLength;
@synthesize clustalWindow;
@synthesize clustalScoreType;
@synthesize clustalNumberDiagonals;
@synthesize clustalPairGapPenalty;
@synthesize clustalMatrix;
@synthesize clustalGapOpenPenalty;
@synthesize clustalEndGaps;
@synthesize clustalGapExtensionCost;
@synthesize clustalGapSeparationPenalty;
@synthesize clustalIteration;
@synthesize clustalNumberOfIterations;

- (void)alignSequences {

    if (alignmentMethod == ALN_CLUSTAL)
        [self helperRunClustal:self];
}

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)decrementTaskCount {

    // @John: I need to comment this out to get it working on my old OS X ... (Sebastian)
    // @Sebastian: Update your fucking OSX version.
    OSAtomicDecrement32(&taskCount);
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
    [aCoder encodeInt:alignmentMethod             forKey:@"alignmentMethod"];
	[aCoder encodeObject:clustalAlign             forKey:@"clustalAlign"];
	[aCoder encodeInt:clustalWordLength           forKey:@"clustalWordLength"];
 	[aCoder encodeInt:clustalWindow               forKey:@"clustalWindow"];
	[aCoder encodeObject:clustalScoreType         forKey:@"clustalScoreType"];
	[aCoder encodeInt:clustalNumberDiagonals      forKey:@"clustalNumberDiagonals"];
	[aCoder encodeInt:clustalPairGapPenalty       forKey:@"clustalPairGapPenalty"];
	[aCoder encodeObject:clustalMatrix            forKey:@"clustalMatrix"];
	[aCoder encodeFloat:clustalGapOpenPenalty     forKey:@"clustalGapOpenPenalty"];
	[aCoder encodeObject:clustalEndGaps           forKey:@"clustalEndGaps"];
	[aCoder encodeFloat:clustalGapExtensionCost   forKey:@"clustalGapExtensionCost"];
	[aCoder encodeInt:clustalGapSeparationPenalty forKey:@"clustalGapSeparationPenalty"];
	[aCoder encodeObject:clustalIteration         forKey:@"clustalIteration"];
	[aCoder encodeInt:clustalNumberOfIterations   forKey:@"clustalNumberOfIterations"];
       
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

    if ( [self numDataMatrices] == 0 )
        {
        BOOL isSuccessful = [self helperRunClustal:self];
        if (isSuccessful == NO)
            return NO;
        }

    return [super execute]; // instantiate data in the core
}

- (BOOL)helperRunClustal:(id)sender {

    [self setIsResolved:NO];
    
    // find the parent of this tool, which should be an instance of ToolReadData
    ToolReadData* dataTool = nil;
    for (size_t i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (int j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:j];
            Tool* t = [[c outlet] toolOwner];
            NSString* className = NSStringFromClass([t class]); 
            if ( [className isEqualToString:@"ToolReadData"] == YES )
                dataTool = (ToolReadData*)t;
            }
        }
    if ( dataTool == nil )
        return NO;
    
    NSLog(@"dataTool=%@", dataTool);

    // calculate how many unaligned data matrices exist
    NSMutableArray* unalignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == NO )
            [unalignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    if ( [unalignedData count] == 0 )
        return NO;
        
    // remove all of the files from the temporary directory
    [self removeFilesFromTemporaryDirectory];

    NSLog(@"unalignedData=%@", unalignedData);
    
    // and make a temporary directory to contain the alignments
    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[NSFileManager alloc] init];
    NSString* alnDirectory = [NSString stringWithString:temporaryDirectory];
              alnDirectory = [alnDirectory stringByAppendingString:@"myAlignments/"];
    NSDictionary* dirAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"dirAttributes"];
    [fm createDirectoryAtPath:alnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];

    NSLog(@"temporaryDirectory=%@", temporaryDirectory);
    NSLog(@"alnDirectory=%@", alnDirectory);

    // write the alignment files to the temporary directory
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // have the data object save a fasta file to the temporary directory
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:alnDirectory];
                  dFilePath = [dFilePath stringByAppendingString:[d name]];
                  dFilePath = [dFilePath stringByAppendingString:@".fas"];
    NSLog(@"dFilePath=%@", dFilePath);

        [d writeToFile:dFilePath];
        }
    
    // set the indeterminate progress bar to on
    [self startProgressIndicator];
    
    // align each file on a separate thread
    taskCount = 0;
    NSMutableArray* taskArray = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // increment task count
        taskCount++;
        
        // allocate the task object
        AlignmentTask* theTask = [[AlignmentTask alloc] initWithAlignmentTool:self];
        [taskArray addObject:theTask];

        // initialize the thread variables
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* fName = [NSString stringWithString:[d name]];
                  fName = [fName stringByAppendingString:@".fas"];
        NSString* tempDir = [NSString stringWithFormat:@"temp_%lu", i+1];
        NSNumber* nt = [NSNumber numberWithInt:[d numTaxa]];
        
        NSMutableArray* theTaskInfo = [[NSMutableArray alloc] initWithCapacity:2];
        [theTaskInfo addObject:alnDirectory];
        [theTaskInfo addObject:fName];
        [theTaskInfo addObject:tempDir];
        [theTaskInfo addObject:nt];
        NSLog(@"theTaskInfo=%@", theTaskInfo);
        
        // detach a thread with this task ... each thread decrements the task count when completed
        [NSThread detachNewThreadSelector:@selector(alignFile:) toTarget:theTask withObject:theTaskInfo];
        }
        
    // wait for all of the alignment tasks to finish
    while (taskCount > 0)
        {
        }
    
    // free the tasks
    
    // read the alignments ********************************
    
    // check the workspace and make certain that we use an unused name for the
    // data variable
    std::string variableName = RevLanguage::Workspace::userWorkspace().generateUniqueVariableName();
    NSString* nsVariableName = [NSString stringWithCString:variableName.c_str() encoding:NSUTF8StringEncoding];
		    
    // format a string command to read the data file(s) and send the
    // formatted string to the parser
    const char* cmdAsCStr = [alnDirectory UTF8String];
    std::string cmdAsStlStr = cmdAsCStr;
    std::string line = variableName + " <- read(\"" + cmdAsStlStr + "\")";
    int coreResult = RevLanguage::Parser::getParser().processCommand(line, &RevLanguage::Workspace::userWorkspace());
    if (coreResult != 0)
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return NO;
        }

    // instantiate data matrices for the gui, by reading the matrices that were 
    // read in by the core

    // retrieve the value (character data matrix or matrices) from the workspace
    const RevLanguage::RevObject& dv = RevLanguage::Workspace::userWorkspace().getRevObject(variableName);
    if ( dv == RevLanguage::RevNullObject::getInstance() )
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return NO;
        }

    // TODO: New implementation (Sebastian)
    std::cerr << "Missing implementation in ToolAlign.\n";
//    RlVector<RlCharacterData>* dnc = dynamic_cast<RlVector<RlCharacterData> *>( dv );
//    RlCharacterData* cd = dynamic_cast<RlCharacterData*>( dv );
//    if ( dnc != NULL )
//        {
//        [self removeAllDataMatrices];
//        for (int i=0; i<dnc->size(); i++)
//            {
//            const RevPtr<RbObject>& theDagNode = dnc->getElement( i );
//            const RlCharacterData& cd = static_cast<const RlCharacterData&>( *theDagNode );
//            RbData* newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:cd.getValue()];
//            [newMatrix setAlignmentMethod:@"Unknown"];
//            [self addMatrix:newMatrix];
//            }
//        }
//    else if ( cd != NULL )
//        {
//        [self removeAllDataMatrices];
//        RbData* newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:cd->getValue()];
//        [newMatrix setAlignmentMethod:@"Unknown"];
//        [self addMatrix:newMatrix];
//        }
//    else
//        {
//        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
//        [self stopProgressIndicator];
//        goto errorExit;
//        }

    // erase the data in the core
    if ( RevLanguage::Workspace::userWorkspace().existsVariable(variableName) )
        RevLanguage::Workspace::userWorkspace().eraseVariable(variableName);
        
    // set the name of the variable in the tool
    [self setDataWorkspaceName:@""];

    // set the alignment method for every data matrix
    for (size_t i=0; i<[dataMatrices count]; i++)
        {
        RbData* d = [dataMatrices objectAtIndex:i];
        [d setIsHomologyEstablished:YES];
        [d setAlignmentMethod:@"ClustalW2"];
        
        // and also the unaligned data matrix that this aligned matrix derives from...
        NSString* alignedName    = [d name];
        NSArray* brokenNameArray = [alignedName componentsSeparatedByString:@".fas"];
        NSString* brokenName     = [brokenNameArray objectAtIndex:0];
        [d setName:brokenName];
        alignedName = [d name];
        for (size_t j=0; j<[unalignedData count]; j++)
            {
            NSString* unalignedName  = [[unalignedData objectAtIndex:j] name];
            if ( [alignedName isEqualToString:unalignedName] == YES )
                {
                RbData* ud = [unalignedData objectAtIndex:j];
                [d setCopiedFrom:ud];
                }
            }
        }
    
    [self makeDataInspector];
    [self setIsResolved:YES];

    errorExit:
    
    [self removeFilesFromTemporaryDirectory];

    // turn the indeterminate progress bar off
    [self stopProgressIndicator];

    [myAnalysisView updateToolsDownstreamFromTool:self];
    return YES;
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
		
		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor cyanColor]];
		[self addOutletOfColor:[NSColor greenColor]];
        [self setInletLocations];
        [self setOutletLocations];
        
        // initialize variables
        alignmentMethod = ALN_CLUSTAL;
            
        // initialize clustal variables here
        // Default values taken from http://www.ebi.ac.uk/Tools/msa/clustalw2/help/
        clustalTask = nil;
        [self setClustalAlign: @"Full"];
        [self setClustalWordLength: 1];
        [self setClustalWindow: 5];
        [self setClustalScoreType: @"Percent"];
        [self setClustalNumberDiagonals: 5];
        [self setClustalPairGapPenalty: 3];
        [self setClustalMatrix: @"Gonnet"];
        [self setClustalGapOpenPenalty: 10.0];
        [self setClustalEndGaps: @"No"];
        [self setClustalGapExtensionCost: 0.20];
        [self setClustalGapSeparationPenalty: 5];
        [self setClustalIteration: @"none"];
        [self setClustalNumberOfIterations: 1];
            		
		// initialize the control window
		controlWindow = [[WindowControllerAlign alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
            // initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // decode variables
        alignmentMethod = [aDecoder decodeIntForKey:@"alignmentMethod"];
            
        // resuscitate Clustal variables here before recreating new windowcontroller
        clustalAlign                = [aDecoder decodeObjectForKey:@"clustalAlign"];
        clustalWordLength           = [aDecoder decodeIntForKey:@"clustalWordLength"];
        clustalWindow               = [aDecoder decodeIntForKey:@"clustalWindow"];
        clustalScoreType            = [aDecoder decodeObjectForKey:@"clustalScoreType"];
        clustalNumberDiagonals      = [aDecoder decodeIntForKey:@"clustalNumberDiagonals"];
        clustalPairGapPenalty       = [aDecoder decodeIntForKey:@"clustalPairGapPenalty"];
        clustalMatrix               = [aDecoder decodeObjectForKey:@"clustalMatrix"];
        clustalGapOpenPenalty       = [aDecoder decodeFloatForKey:@"clustalGapOpenPenalty"];
        clustalEndGaps              = [aDecoder decodeObjectForKey:@"clustalEndGaps"];
        clustalGapExtensionCost     = [aDecoder decodeFloatForKey:@"clustalGapExtensionCost"];
        clustalGapSeparationPenalty = [aDecoder decodeIntForKey:@"clustalGapSeparationPenalty"];
        clustalIteration            = [aDecoder decodeObjectForKey:@"clustalIteration"];
        clustalNumberOfIterations   = [aDecoder decodeIntForKey:@"clustalNumberOfIterations"];
            
        // initialize the control window
		controlWindow = [[WindowControllerAlign alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Align25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Align50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Align75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Align100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Align125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Align150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Align200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Align400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Sequence Alignment Tool ";
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingString:@"\n Status: Resolved "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Status: Unresolved "];
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

/* Receive Clustal data */ 
- (void)receiveData:(NSNotification*)aNotification {

    NSData *incomingData;
    NSString *incomingText;
     
    incomingData = [[aNotification userInfo] objectForKey: NSFileHandleNotificationDataItem];
     
    incomingText = [[NSString alloc] initWithData: incomingData
    encoding: NSASCIIStringEncoding];
    
    //NSLog(@"%@", incomingText);
     
    [clustalFromClustal readInBackgroundAndNotify];
    
    NSString *completionText;
    completionText = @"FASTA file created!";
    
    if ([incomingText rangeOfString: completionText].length > 0)
        {
        [self taskCompleted];
        }
}

- (BOOL)resolveStateOnWindowOK {

    [self alignSequences];
    
    return YES;
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)taskCompleted {

}

- (NSString*)toolName {

    return @"Alignment";
}

- (void)updateForChangeInUpstreamState {
    
    // find the parent of this tool, which should be an instance of ToolReadData
    ToolReadData* dataTool = nil;
    for (size_t i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (int j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:j];
            Tool* t = [[c outlet] toolOwner];
            NSString* className = NSStringFromClass([t class]); 
            if ( [className isEqualToString:@"ToolReadData"] == YES )
                dataTool = (ToolReadData*)t;
            }
        }
    if ( dataTool == nil )
        {
        [self removeAllDataMatrices];
        return;
        }

    // calculate how many unaligned data matrices exist
    NSMutableArray* unalignedData = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == NO )
            [unalignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    if ( [unalignedData count] == 0 || [unalignedData count] != [self numDataMatrices] )
        {
        [self removeAllDataMatrices];
        return;
        }
        
    // check that the aligned data in this tool can all be traced to unaligned data in the ToolReadData tool
    int numNotTraced = 0;
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        RbData* ud = [unalignedData objectAtIndex:i];
        BOOL tracedData = NO;
        for (size_t j=0; j<[dataMatrices count]; j++)
            {
            RbData* d = [dataMatrices objectAtIndex:j];
            if ( [d copiedFrom] == ud )
                tracedData = YES;
            }
        if (tracedData == NO)
            numNotTraced++;
        }
    if (numNotTraced > 0)
        {
        [self removeAllDataMatrices];
        return;
        }
}

@end
