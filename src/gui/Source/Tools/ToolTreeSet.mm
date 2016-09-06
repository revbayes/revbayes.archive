#import "AnalysisView.h"
#import "GuiTree.h"
#import "InOutlet.h"
#import "Node.h"
#import "RbGuiHelper.h"
#import "RevBayes.h"
#import "ToolTreeSet.h"
#import "WindowControllerTreeSet.h"
#import "WindowControllerTreeViewer.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>




@implementation ToolTreeSet

@synthesize myTrees;
@synthesize outgroupName;

- (void)addTreeToSet:(GuiTree*)t {

    [myTrees addObject:t];
    if (hasInspectorInfo == NO)
        {
        hasInspectorInfo = YES;
        [analysisView setNeedsDisplay:YES];
        }
    [self updateChildrenTools];
}

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)closeInspectorPanel {

	[treeInspector close];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeObject:myTrees  forKey:@"myTrees"];
    [aCoder encodeObject:outgroupName forKey:@"outgroupName"];
    
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

    // instantiate trees in core
    
    return [super execute];
}

- (void)exportTrees {

    // check that we have trees to export
    if ([myTrees count] == 0)
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error Exporting Trees"];
        [alert setInformativeText:@"The tool does not contain any trees to export."];
        [alert runModal];
        return;
        }

    // create a file to write to
    NSSavePanel* savePanel = [NSSavePanel savePanel];
    [savePanel setAllowsOtherFileTypes:YES];
    int savePanelReturn = (int)[savePanel runModal];
    if (savePanelReturn == NSModalResponseOK)
        {
        }
    else if (savePanelReturn == NSModalResponseCancel)
        {
        return;
        }
    else
        {
        return;
        }
    NSString* myFile = [[savePanel URL] path];
    myFile = [myFile stringByAppendingString:@".nex"];
    
    // open the file to write to
	std::ofstream myOutStrm;
    myOutStrm.open( [myFile cStringUsingEncoding:NSASCIIStringEncoding], std::ios::out );
    
    // write
    myOutStrm << "#NEXUS" << std::endl << std::endl;
    myOutStrm << "begin trees;" << std::endl;
    myOutStrm << "   translate" << std::endl;
    for (int i=0; i<[[myTrees objectAtIndex:0] numberOfTaxa]; i++)
        {
        myOutStrm << "      " << i+1 << " " << [[[[myTrees objectAtIndex:0] nodeWithIndex:i] name] cStringUsingEncoding:NSASCIIStringEncoding];
        if (i + 1 == [[myTrees objectAtIndex:0] numberOfTaxa])
            myOutStrm << std::endl;
        else
            myOutStrm << "," << std::endl;
        }
    myOutStrm << "      ;" << std::endl;
    int i = 0;
    for (GuiTree* t in myTrees)
        {
        NSString* tStr = [t newickString];
        myOutStrm << "   tree user_tree_" << i+1 << " = ";
        myOutStrm << [tStr cStringUsingEncoding:NSASCIIStringEncoding];
        myOutStrm << ";" << std::endl;
        }
    myOutStrm << "end;" << std::endl;
    
    // close the file
    myOutStrm.close();
}

- (NSString*)getOutgroupName {
    
    NSString* og = nil;
    if ([myTrees count] > 0)
        {
        GuiTree* t = [myTrees objectAtIndex:0];
        for (int i=0; i<[t numberOfNodes]; i++)
            {
            Node* p = [t downPassNodeIndexed:i];
            if ( [[p name] isEqualToString:outgroupName] == YES && [p isLeaf] == YES )
                {
                return [NSString stringWithString:[p name]];
                }
            }
        }
    return og;
}

- (GuiTree*)getTreeIndexed:(int)idx {

    if ([myTrees count] == 0 || idx >= [myTrees count])
        return nil;
    return [myTrees objectAtIndex:idx];
}

- (int)indexOfTaxon:(NSString*)name {

    if ([myTrees count] > 0)
        {
        GuiTree* t = [myTrees objectAtIndex:0];
        for (int i=0; i<[t numberOfNodes]; i++)
            {
            Node* p = [t downPassNodeIndexed:i];
            if ([p isLeaf] == YES)
                {
                if ( [[p name] isEqualToString:name] == YES )
                    return [p index];
                }
            }
        }
    return -1;
}

- (void)importTrees {

    // make an array containing the valid file types that can be chosen
	NSArray* fileTypes = [NSArray arrayWithObjects: @"nex", @"t", @"tree", @"trees", NSFileTypeForHFSTypeCode( 'TEXT' ), nil];

    // get the open panel
    NSOpenPanel* oPanel = [NSOpenPanel openPanel];
    [oPanel setAllowsMultipleSelection:NO];
    [oPanel setCanChooseDirectories:NO];

    // open the panel
    NSString* fileToOpen = @"";
    [oPanel setAllowedFileTypes:fileTypes];
    int result = (int)[oPanel runModal];
    if ( result == NSFileHandlingPanelOKButton )
        {
        NSArray* filesToOpen = [oPanel URLs];
        int count = (int)[filesToOpen count];
        for (int i=0; i<count; i++) 
            {
            fileToOpen = [[filesToOpen objectAtIndex:i] path];
            }
        }
    else
        {
        return;
        }
    
    // read the files on another thread
    [self startProgressIndicator];
    [self setStatusMessage:@"Importing trees from file"];
    [self lockView];
    [NSThread detachNewThreadSelector:@selector(importTaskWithFile:)
                       toTarget:self
                     withObject:fileToOpen];
    
}

- (void)importTreesFinished {

    // set the outgroup
    GuiTree* t = [self getTreeIndexed:0];
    if (t != nil)
        [self setOutgroupName:[t outgroupName]];
    
    // set the inspector window
    if (hasInspectorInfo == NO)
        {
        hasInspectorInfo = YES;
        [analysisView setNeedsDisplay:YES];
        }
    
    [self unlockView];
    [self stopProgressIndicator];
    [self setStatusMessage:@""];
    [self updateChildrenTools];
}

- (void)importTaskWithFile:(NSString*)fileToOpen {

    
    [self removeAllTreesFromSet];
    [self readTreesInFile:fileToOpen];

    // read the alignments on the main thread to prevent errors on graphics.
    [self performSelectorOnMainThread:@selector(importTreesFinished)
                         withObject:nil
                      waitUntilDone:NO];
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
        numberOfInlets = 1;
		[self addInletOfColor:[NSColor redColor]];
		[self addOutletOfColor:[NSColor brownColor]];
        [self setInletLocations];
        [self setOutletLocations];
        [self setOutgroupName:@""];
        
        // allocate an array to hold the trees
        myTrees = [[NSMutableArray alloc] init];
        
        controlWindow = [[WindowControllerTreeSet alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // get the set of trees
        outgroupName = [aDecoder decodeObjectForKey:@"outgroupName"];
        myTrees = [aDecoder decodeObjectForKey:@"myTrees"];
        if ([myTrees count] > 0)
            hasInspectorInfo = YES;

        controlWindow = [[WindowControllerTreeSet alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_TreeSummarize25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_TreeSummarize50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_TreeSummarize75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_TreeSummarize100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_TreeSummarize125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_TreeSummarize150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_TreeSummarize200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_TreeSummarize400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (int)numberOfTreesInSet {

    return (int)[myTrees count];
}

- (BOOL)readTreesFile {
	
	return NO;
}


- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Tree Set Tool ";
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingString:@"\n Status: Resolved "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Status: Unresolved "];
    myTip = [myTip stringByAppendingFormat:@"\n # Trees in Set: %d ", (int)[myTrees count]];
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

- (IBAction)okButtonAction:(id)sender {
    
}

- (void)removeAllTreesFromSet {

    [myTrees removeAllObjects];
    hasInspectorInfo = NO;
}

- (int)numberOfInlets {

    return numberOfInlets;
}

- (BOOL)readTreesInFile:(NSString*)fd {

    RbGuiHelper* helper = [[RbGuiHelper alloc] init];
    myTrees = [helper readTreesFromFile:fd];
    return YES;
}

- (void)rerootOnTaxonNamed:(NSString*)newOutgroupName {

    [self setOutgroupName:newOutgroupName];

    if ([myTrees count] > 0)
        {
        for (GuiTree* t in myTrees)
            {
            [t setOutgroupName:newOutgroupName];
            }
        [self updateChildrenTools];
        
        }
}

- (void)setNumberOfInlets:(int)x {

    if (numberOfInlets != x)
        {
        numberOfInlets = x;
        [self removeAllInlets];
        for (int i=0; i<[self numberOfInlets]; i++)
            {
            [self addInletOfColor:[NSColor redColor]];
            [self setInletLocations];
            }
        }
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)showInspectorPanel {

    treeInspector = nil;
    treeInspector = [[WindowControllerTreeViewer alloc] initWithTool:self];

    NSPoint p = [self originForControlWindow:[treeInspector window]];
    [[treeInspector window] setFrameOrigin:p];
	[treeInspector showWindow:self];    
	[[treeInspector window] makeKeyAndOrderFront:nil];
}

- (NSString*)toolName {

    return @"Tree Set";
}

- (void)updateForChangeInParent {

    isResolved = NO;
    Tool* parentTool = [self getParentToolOfInletIndexed:0];
    if (parentTool != nil)
        {
        if ([parentTool isResolved] == YES)
            isResolved = YES;
        [self removeAllTreesFromSet];
        }
    else
        [self removeAllTreesFromSet];
}

- (BOOL)writeTreesFile {
    
	return NO;
}

@end
