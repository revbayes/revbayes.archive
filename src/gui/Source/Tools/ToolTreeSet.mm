#include <string>
#include <vector>
#import "InOutlet.h"
#import "RevBayes.h"
#import "ToolTreeSet.h"
#include "NclReader.h" // TAH: for ncl trees
#include "Topology.h"



@implementation ToolTreeSet

- (void)awakeFromNib {

}

- (void)dealloc {
	
	[trees release];
	[source release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

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
		[self addInletOfColor:[NSColor redColor]];
        [self setInletLocations];
        [self setOutletLocations];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
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

- (BOOL)readTreesFile {
	
#   if 0
	// start the file dialog and get the file name
    
    // make an array containing the valid file types that can be chosen
	NSArray* fileTypes = [NSArray arrayWithObjects: @"tre", @"t", @"txt", @"tree", @"trees", @"phy", NSFileTypeForHFSTypeCode( 'TEXT' ), nil];
    
    // get the open panel
    NSOpenPanel* oPanel = [NSOpenPanel openPanel];
    [oPanel setAllowsMultipleSelection:NO];
    [oPanel setCanChooseDirectories:NO];
    
	std::vector<Topology*>* myTrees;
	
    // open the panel
    NSString* fileToOpen;
    int result = (int)[oPanel runModalForDirectory:nil file:nil types:fileTypes];
    if ( result == NSOKButton ) 
    {
        // get the file path/name to the file    
        fileToOpen = [oPanel filename];
		std::string fileFormat = "nexus"; // this is just a default for the moment, needs to be set by user
		std::string thisFileToOpen = [fileToOpen cStringUsingEncoding:[NSString defaultCStringEncoding]];
		       
        NSLog(@"Reading file with name %@.", fileToOpen);

        // instantiate an NCL reader object
		NclReader& myReader = NclReader::getInstance();
		
		try {
			myTrees = myReader.readTrees(thisFileToOpen, fileFormat);
			if (myTrees == NULL)
				throw NxsException("help");
		}
		catch (NxsException& x){
			NxsString m = x.msg;
			NSString* errFile = [NSString stringWithString:fileToOpen];
			NSAlert* alert = [NSAlert alertWithMessageText:@"Error Reading Data File" 
			                                 defaultButton:@"OK" 
										   alternateButton:nil 
										       otherButton:nil 
								 informativeTextWithFormat:@"Could not read file \"%@\". Check the settings in the control window to make certain that the file and data types are set correctly. Also, check that the data is formatted correctly in the file.", errFile];
			[alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:nil contextInfo:NULL];
			return NO;
		}
	}
#   endif
	return NO;
	
}



- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Tree Set Tool "];
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

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {
    
    // getting the new selected source (either File or MCMC)
    source = [NSString stringWithString:[[sourceTypeTab selectedTabViewItem] label]];
    if ( [source isEqualToString:@"MCMC"] == YES )
        {
        [okButton setTitle:(@"OK")];
        NSString* msg = @"MCMC output:\nNo further information available yet. Might contain later info about mcmc and model.";
        [mcmcTextField setStringValue:msg];
        }
    else if ( [source isEqualToString:@"Import"] == YES )
        {
        [okButton setTitle:(@"Import")];
        
        }
    else if ( [source isEqualToString:@"Export"] == YES )
        {
        [okButton setTitle:(@"Export")];
        
        }
    
    // updating the outlets and inlets
    [self setInletsAndOutlets];
    
    // resetting the window size
    [self setControlWindowSize];
}

- (IBAction)okButtonAction:(id)sender {
    
    // set the tool state to unresolved
    [self setIsResolved:NO];
    
	// remember the state of the control panel
    
	// perform the action
    
    source = [NSString stringWithString:[[sourceTypeTab selectedTabViewItem] label]];
    if ( [source isEqualToString:@"MCMC"] == YES )
        {
        [self closeControlPanel];
        [self updateForConnectionChange];
        [self setIsResolved:YES];
        }
    else if ( [source isEqualToString:@"Import"] == YES )
        {
        // user selected "OK" for a number set to be written to the memory from the mcmc output)
        //		[myTool removeAllDataMatrices];
		BOOL isSuccessful = [self readTreesFile];
		
		if (isSuccessful == YES)
        {
            [self setHasInspectorInfo:YES];
			[self closeControlPanel];
			[self updateForConnectionChange];
            [self setIsResolved:YES];
        }
		else 
        {
            NSLog(@"D'oh! Couldn't read tree file.");
            
        }
        
    }
    else if ( [source isEqualToString:@"Export"] == YES )
    {
        BOOL isSuccessful = [self writeTreesFile];
        
        if (isSuccessful == YES)
        {
            [self closeControlPanel];
            [self updateForConnectionChange];
            [self setIsResolved:YES];
        }
        else 
        {
            NSLog(@"D'oh! Couldn't write tree file.");
            
        }
    } 
}

- (BOOL)writeTreesFile {
    
    NSSavePanel *sp = [NSSavePanel savePanel];
    [sp setTitle:@"Save output file as"];
    NSArray *fileTypes = [NSArray arrayWithObjects: @"txt", @"text", NSFileTypeForHFSTypeCode( 'TEXT' ), nil];
    [sp setAllowedFileTypes:fileTypes];
    
    int spInt = (int)[sp runModalForDirectory:nil file:nil];
    if (spInt == NSOKButton)
    {
        NSLog(@"doSaveAs we have an OK button");	
        
        return YES;
    }
    else if (spInt == NSCancelButton) 
    {
        NSLog(@"doSaveAs we have a Cancel button");
        return NO;
    }
    else 
    {
        NSLog(@"doSaveAs tvarInt not equal 1 or zero = %3d", spInt);
        return NO;
    }     
    
	
    
	return NO;
}



@end
