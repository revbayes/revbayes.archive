#import "AlignmentTaskClustal.h"
#import "AlignmentTaskMuscle.h"
#import "AlignmentTaskProbcons.h"
#import "AlignmentTaskTcoffee.h"
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
#include "WorkspaceVector.h"
#include "RlAbstractCharacterData.h"



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
@synthesize muscleAnchorSpacing;
@synthesize muscleCenter;
@synthesize muscleCluster1;
@synthesize muscleCluster2;
@synthesize muscleDiagLength;
@synthesize muscleDiagMargin;
@synthesize muscleDistance1;
@synthesize muscleDistance2;
@synthesize muscleGapOpen;
@synthesize muscleHydro;
@synthesize muscleHydroFactor;
@synthesize muscleMaxDiagBreak;
@synthesize muscleMaxIters;
@synthesize muscleMaxTrees;
@synthesize muscleMinBestColScore;
@synthesize muscleMinSmoothScore;
@synthesize muscleObjScore;
@synthesize muscleRoot1;
@synthesize muscleRoot2;
@synthesize muscleSmoothScoreCeil;
@synthesize muscleSmoothWindow;
@synthesize muscleSUEFF;
@synthesize muscleWeight1;
@synthesize muscleWeight2;
@synthesize probconsConsistency;
@synthesize probconsIterativeRefinement;
@synthesize tcoffeeGapOpenPenalty;
@synthesize tcoffeeGapExtensionCost;

- (void)alignmentFinished:(NSString*)alnDirectory {

    if (numberErrors == 0)
        {
        if ([self readAlignmentsInTemporaryFolder:alnDirectory] == NO)
            {
            [self removeFilesFromTemporaryDirectory];
            }
        [self stopProgressIndicator];
        [self setStatusMessage:@""];
        [self unlockView];
        [self updateChildrenTools];
        }
    else
        {
        [self stopProgressIndicator];
        [self setStatusMessage:@""];
        [self unlockView];
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error Aligning Sequences"];
        [alert setInformativeText:@"One or more errors occurred while aligning sequences."];
        [alert runModal];
        }
}

- (void)alignSequences {

    if (alignmentMethod == ALN_CLUSTAL)
        [self helperRunClustal:self];
    else if (alignmentMethod == ALN_MUSCLE)
        [self helperRunMuscle:self];
    else if (alignmentMethod == ALN_PROBCONS)
        [self helperRunProbcons:self];
    else if (alignmentMethod == ALN_TCOFFEE)
        [self helperRunTcoffee:self];
}

- (void)awakeFromNib {

}

- (void)closeControlPanelWithCancel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)closeControlPanelWithOK {

    [NSApp stopModal];
	[controlWindow close];

    if ( [self findDataParent] == nil )
        return;

    [self lockView];
    [self startProgressIndicator];

    if (alignmentMethod == ALN_CLUSTAL)
        [self setStatusMessage:@"Aligning sequences using Clustal"];
    else if (alignmentMethod == ALN_MUSCLE)
        [self setStatusMessage:@"Aligning sequences using MUSCLE"];
    else if (alignmentMethod == ALN_PROBCONS)
        [self setStatusMessage:@"Aligning sequences using Probcons"];
    else if (alignmentMethod == ALN_TCOFFEE)
        [self setStatusMessage:@"Aligning sequences using T-Coffee"];

    [NSThread detachNewThreadSelector:@selector(alignSequences)
                       toTarget:self
                     withObject:nil];
}

- (void)decrementTaskCount {

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
    [aCoder encodeInt:muscleAnchorSpacing         forKey:@"muscleAnchorSpacing"];
    [aCoder encodeFloat:muscleCenter              forKey:@"muscleCenter"];
    [aCoder encodeObject:muscleCluster1           forKey:@"muscleCluster1"];
    [aCoder encodeObject:muscleCluster2           forKey:@"muscleCluster2"];
    [aCoder encodeInt:muscleDiagLength            forKey:@"muscleDiagLength"];
    [aCoder encodeInt:muscleDiagMargin            forKey:@"muscleDiagMargin"];
    [aCoder encodeObject:muscleDistance1          forKey:@"muscleDistance1"];
    [aCoder encodeObject:muscleDistance2          forKey:@"muscleDistance2"];
    [aCoder encodeFloat:muscleGapOpen             forKey:@"muscleGapOpen"];
    [aCoder encodeInt:muscleHydro                 forKey:@"muscleHydro"];
    [aCoder encodeFloat:muscleHydroFactor         forKey:@"muscleHydroFactor"];
    [aCoder encodeInt:muscleMaxDiagBreak          forKey:@"muscleMaxDiagBreak"];
    [aCoder encodeInt:muscleMaxIters              forKey:@"muscleMaxIters"];
    [aCoder encodeInt:muscleMaxTrees              forKey:@"muscleMaxTrees"];
    [aCoder encodeFloat:muscleMinBestColScore     forKey:@"muscleMinBestColScore"];
    [aCoder encodeFloat:muscleMinSmoothScore      forKey:@"muscleMinSmoothScore"];
    [aCoder encodeObject:muscleObjScore           forKey:@"muscleObjScore"];
    [aCoder encodeObject:muscleRoot1              forKey:@"muscleRoot1"];
    [aCoder encodeObject:muscleRoot2              forKey:@"muscleRoot2"];
    [aCoder encodeFloat:muscleSmoothScoreCeil     forKey:@"muscleSmoothScoreCeil"];
    [aCoder encodeInt:muscleSmoothWindow          forKey:@"muscleSmoothWindow"];
    [aCoder encodeFloat:muscleSUEFF               forKey:@"muscleSUEFF"];
    [aCoder encodeObject:muscleWeight1            forKey:@"muscleWeight1"];
    [aCoder encodeObject:muscleWeight2            forKey:@"muscleWeight2"];
    [aCoder encodeInt:probconsConsistency         forKey:@"probconsConsistency"];
    [aCoder encodeInt:probconsIterativeRefinement forKey:@"probconsIterativeRefinement"];
    [aCoder encodeFloat:tcoffeeGapOpenPenalty     forKey:@"tcoffeeGapOpenPenalty"];
    [aCoder encodeFloat:tcoffeeGapExtensionCost   forKey:@"tcoffeeGapExtensionCost"];

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

- (ToolReadData*)findDataParent {

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
    return dataTool;
}

- (BOOL)helperRunClustal:(id)sender {

    [self setIsResolved:NO];
    
    // find the parent of this tool, which should be an instance of ToolReadData
    ToolReadData* dataTool = [self findDataParent];
    if ( dataTool == nil )
        return NO;
    
    // add the unaligned data matrices on the parent tool to an array
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
    
    // and make a temporary directory to contain the alignments
    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[NSFileManager alloc] init];
    NSString* unalnDirectory = [NSString stringWithString:temporaryDirectory];
              unalnDirectory = [unalnDirectory stringByAppendingString:@"unaligned/"];
    NSDictionary* dirAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"dirAttributes"];
    [fm createDirectoryAtPath:unalnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];
    NSString* alnDirectory = [NSString stringWithString:temporaryDirectory];
              alnDirectory = [alnDirectory stringByAppendingString:@"aligned/"];
    [fm createDirectoryAtPath:alnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];

    // write the unaligned files to the temporary directory
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // have the data object save a fasta file to the temporary directory
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:unalnDirectory];
                  dFilePath = [dFilePath stringByAppendingString:[d name]];
                  dFilePath = [dFilePath stringByAppendingString:@".fas"];
        [d writeToFile:dFilePath];
        }
    
    // align each file on a separate thread
    taskCount = 0;
    NSMutableArray* taskArray = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // increment task count
        taskCount++;
        
        // allocate the task object
        AlignmentTaskClustal* theTask = [[AlignmentTaskClustal alloc] initWithAlignmentTool:self];
        [taskArray addObject:theTask];

        // initialize the thread variables
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* fName = [NSString stringWithString:[d name]];
                  fName = [fName stringByAppendingString:@".fas"];
        NSString* tempDir = [NSString stringWithFormat:@"temp_%lu", i+1];
        NSNumber* nt = [NSNumber numberWithInt:[d numTaxa]];
        
        NSMutableArray* theTaskInfo = [[NSMutableArray alloc] initWithCapacity:2];
        [theTaskInfo addObject:unalnDirectory];
        [theTaskInfo addObject:alnDirectory];
        [theTaskInfo addObject:fName];
        [theTaskInfo addObject:tempDir];
        [theTaskInfo addObject:nt];
        
        // detach a thread with this task ... each thread decrements the task count when completed
        [NSThread detachNewThreadSelector:@selector(alignFile:) toTarget:theTask withObject:theTaskInfo];
        }
        
    // wait for all of the alignment tasks to finish
    while (taskCount > 0)
        {
        }

    // read the alignments on the main thread to prevent errors on graphics.
    [self performSelectorOnMainThread:@selector(alignmentFinished:)
                         withObject:alnDirectory
                      waitUntilDone:NO];
    
    return YES;
}

- (BOOL)helperRunMuscle:(id)sender {

    [self setIsResolved:NO];
    
    // find the parent of this tool, which should be an instance of ToolReadData
    ToolReadData* dataTool = [self findDataParent];
    if ( dataTool == nil )
        return NO;
    
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
    
    // and make a temporary directory to contain the alignments
    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[NSFileManager alloc] init];
    NSString* unalnDirectory = [NSString stringWithString:temporaryDirectory];
              unalnDirectory = [unalnDirectory stringByAppendingString:@"unaligned/"];
    NSDictionary* dirAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"dirAttributes"];
    [fm createDirectoryAtPath:unalnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];
    NSString* alnDirectory = [NSString stringWithString:temporaryDirectory];
              alnDirectory = [alnDirectory stringByAppendingString:@"aligned/"];
    [fm createDirectoryAtPath:alnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];

    // write the alignment files to the temporary directory
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // have the data object save a fasta file to the temporary directory
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:unalnDirectory];
                  dFilePath = [dFilePath stringByAppendingString:[d name]];
                  dFilePath = [dFilePath stringByAppendingString:@".fas"];
        [d writeToFile:dFilePath];
        }
    
    // align each file on a separate thread
    taskCount = 0;
    numberErrors = 0;
    NSMutableArray* taskArray = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // increment task count
        taskCount++;
        
        // allocate the task object
        AlignmentTaskMuscle* theTask = [[AlignmentTaskMuscle alloc] initWithAlignmentTool:self];
        [taskArray addObject:theTask];

        // initialize the thread variables
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* fName = [NSString stringWithString:[d name]];
                  fName = [fName stringByAppendingString:@".fas"];
        NSString* tempDir = [NSString stringWithFormat:@"temp_%lu", i+1];
        NSNumber* nt = [NSNumber numberWithInt:[d numTaxa]];
        
        NSMutableArray* theTaskInfo = [[NSMutableArray alloc] initWithCapacity:2];
        [theTaskInfo addObject:unalnDirectory];
        [theTaskInfo addObject:alnDirectory];
        [theTaskInfo addObject:fName];
        [theTaskInfo addObject:tempDir];
        [theTaskInfo addObject:nt];
        
        // detach a thread with this task ... each thread decrements the task count when completed
        [NSThread detachNewThreadSelector:@selector(alignFile:) toTarget:theTask withObject:theTaskInfo];
        }
        
    // wait for all of the alignment tasks to finish
    while (taskCount > 0)
        {
        }
    
    // read the alignments on the main thread to prevent errors on graphics.
    [self performSelectorOnMainThread:@selector(alignmentFinished:)
                         withObject:alnDirectory
                      waitUntilDone:NO];

    return YES;
}

- (BOOL)helperRunProbcons:(id)sender {

    [self setIsResolved:NO];
    
    // find the parent of this tool, which should be an instance of ToolReadData
    ToolReadData* dataTool = [self findDataParent];
    if ( dataTool == nil )
        return NO;
    
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
    
    // and make a temporary directory to contain the alignments
    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[NSFileManager alloc] init];
    NSString* unalnDirectory = [NSString stringWithString:temporaryDirectory];
              unalnDirectory = [unalnDirectory stringByAppendingString:@"unaligned/"];
    NSDictionary* dirAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"dirAttributes"];
    [fm createDirectoryAtPath:unalnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];
    NSString* alnDirectory = [NSString stringWithString:temporaryDirectory];
              alnDirectory = [alnDirectory stringByAppendingString:@"aligned/"];
    [fm createDirectoryAtPath:alnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];

    // write the alignment files to the temporary directory
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // have the data object save a fasta file to the temporary directory
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:unalnDirectory];
                  dFilePath = [dFilePath stringByAppendingString:[d name]];
                  dFilePath = [dFilePath stringByAppendingString:@".fas"];
        [d writeToFile:dFilePath];
        }
    
    // align each file on a separate thread
    taskCount = 0;
    numberErrors = 0;
    NSMutableArray* taskArray = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // increment task count
        taskCount++;
        
        // allocate the task object
        AlignmentTaskProbcons* theTask = [[AlignmentTaskProbcons alloc] initWithAlignmentTool:self];
        [taskArray addObject:theTask];

        // initialize the thread variables
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* fName = [NSString stringWithString:[d name]];
                  fName = [fName stringByAppendingString:@".fas"];
        NSString* tempDir = [NSString stringWithFormat:@"temp_%lu", i+1];
        NSNumber* nt = [NSNumber numberWithInt:[d numTaxa]];
        
        NSMutableArray* theTaskInfo = [[NSMutableArray alloc] initWithCapacity:2];
        [theTaskInfo addObject:unalnDirectory];
        [theTaskInfo addObject:alnDirectory];
        [theTaskInfo addObject:fName];
        [theTaskInfo addObject:tempDir];
        [theTaskInfo addObject:nt];
        
        // detach a thread with this task ... each thread decrements the task count when completed
        [NSThread detachNewThreadSelector:@selector(alignFile:) toTarget:theTask withObject:theTaskInfo];
        }
        
    // wait for all of the alignment tasks to finish
    while (taskCount > 0)
        {
        }
    
    // read the alignments on the main thread to prevent errors on graphics.
    [self performSelectorOnMainThread:@selector(alignmentFinished:)
                         withObject:alnDirectory
                      waitUntilDone:NO];

    return YES;
}

- (BOOL)helperRunTcoffee:(id)sender {

    [self setIsResolved:NO];
    
    // find the parent of this tool, which should be an instance of ToolReadData
    ToolReadData* dataTool = [self findDataParent];
    if ( dataTool == nil )
        return NO;
    
    // add the unaligned data matrices on the parent tool to an array
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
    
    // and make a temporary directory to contain the alignments
    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[NSFileManager alloc] init];
    NSString* unalnDirectory = [NSString stringWithString:temporaryDirectory];
              unalnDirectory = [unalnDirectory stringByAppendingString:@"unaligned/"];
    NSDictionary* dirAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"dirAttributes"];
    [fm createDirectoryAtPath:unalnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];
    NSString* alnDirectory = [NSString stringWithString:temporaryDirectory];
              alnDirectory = [alnDirectory stringByAppendingString:@"aligned/"];
    [fm createDirectoryAtPath:alnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];

    // write the unaligned files to the temporary directory
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // have the data object save a fasta file to the temporary directory
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:unalnDirectory];
                  dFilePath = [dFilePath stringByAppendingString:[d name]];
                  dFilePath = [dFilePath stringByAppendingString:@".fas"];
        [d writeToFile:dFilePath];
        }
    
    // align each file on a separate thread
    taskCount = 0;
    NSMutableArray* taskArray = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[unalignedData count]; i++)
        {
        // increment task count
        taskCount++;
        
        // allocate the task object
        AlignmentTaskTcoffee* theTask = [[AlignmentTaskTcoffee alloc] initWithAlignmentTool:self];
        [taskArray addObject:theTask];

        // initialize the thread variables
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* fName = [NSString stringWithString:[d name]];
                  fName = [fName stringByAppendingString:@".fas"];
        NSString* tempDir = [NSString stringWithFormat:@"temp_%lu", i+1];
        NSNumber* nt = [NSNumber numberWithInt:[d numTaxa]];
        
        NSMutableArray* theTaskInfo = [[NSMutableArray alloc] initWithCapacity:2];
        [theTaskInfo addObject:unalnDirectory];
        [theTaskInfo addObject:alnDirectory];
        [theTaskInfo addObject:fName];
        [theTaskInfo addObject:tempDir];
        [theTaskInfo addObject:nt];
        
        // detach a thread with this task ... each thread decrements the task count when completed
        [NSThread detachNewThreadSelector:@selector(alignFile:) toTarget:theTask withObject:theTaskInfo];
        }
        
    // wait for all of the alignment tasks to finish
    while (taskCount > 0)
        {
        }

    // read the alignments on the main thread to prevent errors on graphics.
    [self performSelectorOnMainThread:@selector(alignmentFinished:)
                         withObject:alnDirectory
                      waitUntilDone:NO];
    
    return YES;
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
        // listen for thread completion
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

        // initialize muscle variables here
        // default values taken from http://www.drive5.com/muscle/muscle.html
        [self setMuscleAnchorSpacing:32];
        [self setMuscleCenter:-1.0];
        [self setMuscleCluster1:@"upgmb"];
        [self setMuscleCluster2:@"upgmb"];
        [self setMuscleDiagLength:24];
        [self setMuscleDiagMargin:5];
        [self setMuscleDistance1:@"kmer4_6"];
        [self setMuscleDistance2:@"pctid_kimura"];
        [self setMuscleGapOpen:-1.0];
        [self setMuscleHydro:5];
        [self setMuscleHydroFactor:1.2];
        [self setMuscleMaxDiagBreak:1];
        [self setMuscleMaxIters:16];
        [self setMuscleMaxTrees:1];
        [self setMuscleMinBestColScore:-1.0];
        [self setMuscleMinSmoothScore:-1.0];
        [self setMuscleObjScore:@"spm"];
        [self setMuscleRoot1:@"pseudo"];
        [self setMuscleRoot2:@"pseudo"];
        [self setMuscleSmoothScoreCeil:-1.0];
        [self setMuscleSmoothWindow:7];
        [self setMuscleSUEFF:0.1];
        [self setMuscleWeight1:@"clustalw"];
        [self setMuscleWeight2:@"clustalw"];

        // initialize probcons variables here
        [self setProbconsConsistency:2];
        [self setProbconsIterativeRefinement:100];
        
        // initialize t-coffee variables here
        [self setTcoffeeGapOpenPenalty:-50.0];
        [self setTcoffeeGapExtensionCost:0.0];

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

        // resuscitate Muscle variables here before recreating new windowcontroller
        muscleAnchorSpacing         = [aDecoder decodeIntForKey:@"muscleAnchorSpacing"];
        muscleCenter                = [aDecoder decodeFloatForKey:@"muscleCenter"];
        muscleCluster1              = [aDecoder decodeObjectForKey:@"muscleCluster1"];
        muscleCluster2              = [aDecoder decodeObjectForKey:@"muscleCluster2"];
        muscleDiagLength            = [aDecoder decodeIntForKey:@"muscleDiagLength"];
        muscleDiagMargin            = [aDecoder decodeIntForKey:@"muscleDiagMargin"];
        muscleDistance1             = [aDecoder decodeObjectForKey:@"muscleDistance1"];
        muscleDistance2             = [aDecoder decodeObjectForKey:@"muscleDistance2"];
        muscleGapOpen               = [aDecoder decodeFloatForKey:@"muscleGapOpen"];
        muscleHydro                 = [aDecoder decodeIntForKey:@"muscleHydro"];
        muscleHydroFactor           = [aDecoder decodeFloatForKey:@"muscleHydroFactor"];
        muscleMaxDiagBreak          = [aDecoder decodeIntForKey:@"muscleMaxDiagBreak"];
        muscleMaxIters              = [aDecoder decodeIntForKey:@"muscleMaxIters"];
        muscleMaxTrees              = [aDecoder decodeIntForKey:@"muscleMaxTrees"];
        muscleMinBestColScore       = [aDecoder decodeFloatForKey:@"muscleMinBestColScore"];
        muscleMinSmoothScore        = [aDecoder decodeFloatForKey:@"muscleMinSmoothScore"];
        muscleObjScore              = [aDecoder decodeObjectForKey:@"muscleObjScore"];
        muscleRoot1                 = [aDecoder decodeObjectForKey:@"muscleRoot1"];
        muscleRoot2                 = [aDecoder decodeObjectForKey:@"muscleRoot2"];
        muscleSmoothScoreCeil       = [aDecoder decodeFloatForKey:@"muscleSmoothScoreCeil"];
        muscleSmoothWindow          = [aDecoder decodeIntForKey:@"muscleSmoothWindow"];
        muscleSUEFF                 = [aDecoder decodeFloatForKey:@"muscleSUEFF"];
        muscleWeight1               = [aDecoder decodeObjectForKey:@"muscleWeight1"];
        muscleWeight2               = [aDecoder decodeObjectForKey:@"muscleWeight2"];

        // resuscitate Probcons variables here before recreating new windowcontroller
        probconsConsistency         = [aDecoder decodeIntForKey:@"probconsConsistency"];
        probconsIterativeRefinement = [aDecoder decodeIntForKey:@"probconsIterativeRefinement"];

        // resuscitate T-coffee variables here before recreating new windowcontroller
        tcoffeeGapOpenPenalty       = [aDecoder decodeFloatForKey:@"tcoffeeGapOpenPenalty"];
        tcoffeeGapExtensionCost     = [aDecoder decodeFloatForKey:@"tcoffeeGapExtensionCost"];
            
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

- (void)incrementErrorCount {

    OSAtomicIncrement32(&numberErrors);
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

- (BOOL)readAlignmentsInTemporaryFolder:(NSString*)alnDirectory {
    
    // check the workspace and make certain that we use an unused name for the
    // data variable
    std::string variableName = RevLanguage::Workspace::userWorkspace().generateUniqueVariableName();
    NSString* nsVariableName = [NSString stringWithCString:variableName.c_str() encoding:NSUTF8StringEncoding];
		    
    // format a string command to read the data file(s) and send the formatted string to the parser
    const char* cmdAsCStr = [alnDirectory UTF8String];
    std::string cmdAsStlStr = cmdAsCStr;
    std::string line = variableName + " = readCharacterData(\"" + cmdAsStlStr + "\",alwaysReturnAsVector=TRUE)";
    int coreResult = RevLanguage::Parser::getParser().processCommand(line, &RevLanguage::Workspace::userWorkspace());
    if (coreResult != 0)
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        return NO;
        }
    
    // retrieve the value (character data matrix or matrices) from the workspace
    const RevLanguage::RevObject& dv = RevLanguage::Workspace::userWorkspace().getRevObject(variableName);
    if ( dv == RevLanguage::RevNullObject::getInstance() )
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        return NO;
        }
    
    // instantiate data matrices for the gui, by reading the matrices that were
    // read in by the core and stored in the WorkspaceVector
    const WorkspaceVector<RevLanguage::AbstractCharacterData> *dnc = dynamic_cast<const WorkspaceVector<RevLanguage::AbstractCharacterData> *>( &dv );
    if (dnc != NULL)
        {
        if (dnc->size() == 0)
            {
            [self readDataError:@"No data matrices read" forVariableNamed:nsVariableName];
            return NO;
            }
        [self removeAllDataMatrices];
        for (int i=0; i<dnc->size(); i++)
            {
            RbData* newMatrix = NULL;
            const RevBayesCore::AbstractCharacterData* cd = &((*dnc)[i].getValue());
            
            if (cd->isHomologyEstablished() == true)
                {
                // homology (alignment) has been established
                if (cd->getDataType() == "RNA")
                    {
                    std::string type = "RNA";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "DNA")
                    {
                    std::string type = "DNA";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Protein")
                    {
                    std::string type = "Protein";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Standard")
                    {
                    std::string type = "Standard";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else if (cd->getDataType() == "Continuous")
                    {
                    std::string type = "Continuous";
                    newMatrix = [self makeNewGuiDataMatrixFromCoreMatrixWithAddress:(*cd) andDataType:type];
                    }
                else
                    {
                    [self readDataError:@"Unrecognized data type" forVariableNamed:nsVariableName];
                    [self stopProgressIndicator];
                    return NO;
                    }

                if (newMatrix == NULL)
                    {
                    [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
                    return NO;
                    }
                    
                NSString* oldFileName = [newMatrix name];
                NSRange rng = NSMakeRange(0, [oldFileName length]-4);
                NSString* newFileName = [oldFileName substringWithRange:rng];
                [newMatrix setName:newFileName];
                
                if (alignmentMethod == ALN_CLUSTAL)
                    [newMatrix setAlignmentMethod:@"Clustal"];
                else if (alignmentMethod == ALN_MAFFT)
                    [newMatrix setAlignmentMethod:@"MAFFT"];
                else if (alignmentMethod == ALN_DIALIGN)
                    [newMatrix setAlignmentMethod:@"DIALIGN"];
                else if (alignmentMethod == ALN_MUSCLE)
                    [newMatrix setAlignmentMethod:@"MUSCLE"];
                else if (alignmentMethod == ALN_TCOFFEE)
                    [newMatrix setAlignmentMethod:@"T-COFFEE"];
                else if (alignmentMethod == ALN_DCA)
                    [newMatrix setAlignmentMethod:@"DCA"];
                else if (alignmentMethod == ALN_PROBCONS)
                    [newMatrix setAlignmentMethod:@"PROBCONS"];
                    
                [self addMatrix:newMatrix];
                }
            else
                {
                // we should never be here
                [self readDataError:@"Unaligned data found" forVariableNamed:nsVariableName];
                return NO;
                }
            }
        }
    else
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        return NO;
        }
    
    // erase the data in the core
    if ( RevLanguage::Workspace::userWorkspace().existsVariable(variableName) )
        RevLanguage::Workspace::userWorkspace().eraseVariable(variableName);
        
    // set the name of the variable in the tool
    [self setDataWorkspaceName:@""];
    
    // set up the data inspector
    [self makeDataInspector];
    [self setIsResolved:YES];

    // clean up
    [self removeFilesFromTemporaryDirectory];

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

- (void)updateForChangeInParent {
    
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
