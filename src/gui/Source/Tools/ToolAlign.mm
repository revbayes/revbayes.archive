#import "AlignmentTask.h"
#import "Connection.h"
#import "RbData.h"
#import "Inlet.h"
#import "InOutlet.h"
#import "RevBayes.h"
#import "ToolAlign.h"
#import "ToolReadData.h"
#import "WindowControllerAlign.h"


@implementation ToolAlign

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


- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
    [self setInletsAndOutlets];
}

- (void)dealloc {

    if (clustalTask != nil)
        [clustalTask release];
        
    [clustalAlign release];
    [clustalScoreType release];
    [clustalMatrix release];
    [clustalEndGaps release];
    [clustalIteration release];

	[controlWindow release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
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
		[self addInletOfColor:[NSColor greenColor]];
		[self addOutletOfColor:[NSColor greenColor]];
        [self setInletLocations];
        [self setOutletLocations];
            
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
            
        // resuscitate Clustal variables here before recreating new windowcontroller
        clustalAlign = [aDecoder decodeObjectForKey:@"clustalAlign"];
            [clustalAlign retain];
        clustalWordLength = [aDecoder decodeIntForKey:@"clustalWordLength"];
        clustalWindow = [aDecoder decodeIntForKey:@"clustalWindow"];
        clustalScoreType = [aDecoder decodeObjectForKey:@"clustalScoreType"];
            [clustalScoreType retain];
        clustalNumberDiagonals = [aDecoder decodeIntForKey:@"clustalNumberDiagonals"];
        clustalPairGapPenalty = [aDecoder decodeIntForKey:@"clustalPairGapPenalty"];
        clustalMatrix = [aDecoder decodeObjectForKey:@"clustalMatrix"];
            [clustalMatrix retain];
        clustalGapOpenPenalty = [aDecoder decodeFloatForKey:@"clustalGapOpenPenalty"];
        clustalEndGaps = [aDecoder decodeObjectForKey:@"clustalEndGaps"];
            [clustalEndGaps retain];
        clustalGapExtensionCost = [aDecoder decodeFloatForKey:@"clustalGapExtensionCost"];
        clustalGapSeparationPenalty = [aDecoder decodeIntForKey:@"clustalGapSeparationPenalty"];
        clustalIteration = [aDecoder decodeObjectForKey:@"clustalIteration"];
            [clustalIteration retain];
        clustalNumberOfIterations = [aDecoder decodeIntForKey:@"clustalNumberOfIterations"];
        
            
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

    NSString* myTip = [NSString stringWithString:@" Sequence Alignment Tool "];
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

- (void)setInletsAndOutlets {

}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)helperRunClustal:(id)sender {

    // find the parent of this tool, which should be an instance of ToolReadData
    ToolReadData* dataTool = nil;
    for (int i=0; i<[inlets count]; i++)
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
        return;
        
    // calculate how many unaligned data matrices exist
    NSMutableArray* unalignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == NO )
            [unalignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    if ( [unalignedData count] == 0 )
        return;
        
    // write the alignment files to the temporary directory
    NSString* temporaryDirectory = NSTemporaryDirectory();
    for (int i=0; i<[unalignedData count]; i++)
        {
        // have the data object save a fasta file to the temporary directory
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:temporaryDirectory];
        dFilePath = [dFilePath stringByAppendingString:[d name]];
        dFilePath = [dFilePath stringByAppendingString:@".fas"];
        [d writeToFile:dFilePath];
        }
        
    // set the indeterminate progress bar to on
    [self startProgressIndicator];
    
    // align each file on a separate thread
    taskCount = 0;
    NSMutableArray* taskArray = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[unalignedData count]; i++)
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
        NSString* tempDir = [NSString stringWithFormat:@"temp_%d", i+1];
        NSNumber* nt = [NSNumber numberWithInt:[d numTaxa]];
        
        NSMutableArray* theTaskInfo = [[NSMutableArray alloc] initWithCapacity:2];
        [theTaskInfo addObject:temporaryDirectory];
        [theTaskInfo addObject:fName];
        [theTaskInfo addObject:tempDir];
        [theTaskInfo addObject:nt];
        
        // detach a thread with this task
        [NSThread detachNewThreadSelector:@selector(alignFile:) toTarget:theTask withObject:theTaskInfo];
        }
        
    // wait for all of the alignment tasks to finish
    while (taskCount > 0)
        {
        }
    
    // free the tasks
	NSEnumerator* taskEnumerator = [taskArray objectEnumerator];
	id element;
	while ( (element = [taskEnumerator nextObject]) )
		{
        [(AlignmentTask*)element release];
        }
        
    // turn the indeterminate progress bar off
    [self stopProgressIndicator];
        
        
        
        
        
        
        
        
        



}


- (void)decrementTaskCount {

    OSAtomicDecrement32(&taskCount);
}

- (void)performAlignment {

    NSLog(@"taskCount = %d", taskCount);
    [NSThread sleepForTimeInterval:5.0f];

    OSAtomicDecrement32(&taskCount);
}

/* Receive Clustal data */ 
- (void)receiveData:(NSNotification*)aNotification {

    NSData *incomingData;
    NSString *incomingText;
     
    incomingData = [[aNotification userInfo] objectForKey: NSFileHandleNotificationDataItem];
     
    incomingText = [[NSString alloc] initWithData: incomingData
    encoding: NSASCIIStringEncoding];
    
    NSLog(@"%@", incomingText);
     
    [clustalFromClustal readInBackgroundAndNotify];
    
    NSString *completionText;
    completionText = @"FASTA file created!";
    
    if ([incomingText rangeOfString: completionText].length > 0)
        {
        [self taskCompleted];
        }
     
    [incomingText release];
}
    

- (void)taskCompleted {

    NSLog (@"Clustal task completed");
}


@end
