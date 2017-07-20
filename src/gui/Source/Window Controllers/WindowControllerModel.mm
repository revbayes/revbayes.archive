#import "ModelView.h"
#import "ToolModel.h"
#import "WindowControllerModel.h"
#import "WindowControllerModelBrowser.h"
#import "WindowControllerModelSubmission.h"



@implementation WindowControllerModel

@synthesize showParmNames;

- (void)awakeFromNib {
    
    [self updateInletSelector];
}

- (IBAction)changeInlets:(id)sender {
    
    NSString* buttonTitle = [sender title];
    NSLog(@"buttonTitle = %@", buttonTitle);
    if ( [buttonTitle isEqualToString:@"Alignment"] == YES )
        {
        if ( [myTool isInletActiveWithIndex:0] == YES )
            [myTool setPossibleInletTo:NO forPossibility:0];
        else
            [myTool setPossibleInletTo:YES forPossibility:0];
        [myTool updateInlets];
        }
    else if ( [buttonTitle isEqualToString:@"Unaligned"] == YES )
        {
        if ( [myTool isInletActiveWithIndex:1] == YES )
            [myTool setPossibleInletTo:NO forPossibility:1];
        else
            [myTool setPossibleInletTo:YES forPossibility:1];
        [myTool updateInlets];
        }
    else if ( [buttonTitle isEqualToString:@"Trees"] == YES )
        {
        if ( [myTool isInletActiveWithIndex:2] == YES )
            [myTool setPossibleInletTo:NO forPossibility:2];
        else
            [myTool setPossibleInletTo:YES forPossibility:2];
        [myTool updateInlets];
        }
    else if ( [buttonTitle isEqualToString:@"Numbers"] == YES )
        {
        if ( [myTool isInletActiveWithIndex:3] == YES )
            [myTool setPossibleInletTo:NO forPossibility:3];
        else
            [myTool setPossibleInletTo:YES forPossibility:3];
        [myTool updateInlets];
        }
    [self updateInletSelector];        
}

- (void)drawerClosed {
    
    [toggleParameterPaletteButton setTitle:@"Show Parameter Palette"];
}

- (void)drawerOpened {
    
    [toggleParameterPaletteButton setTitle:@"Hide Parameter Palette"];
}

- (IBAction)exportButtonAction:(id)sender {

	[myTool exportModel];
	[modelViewPtr setNeedsDisplay:YES];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (IBAction)importButtonAction:(id)sender {

	[myTool importModel];
	[modelViewPtr setNeedsDisplay:YES];
}

- (id)init {

    return [self initWithTool:nil andParms:nil];
}

- (id)initWithTool:(ToolModel*)t andParms:(NSMutableArray*)p {

	if ( (self = [super initWithWindowNibName:@"ControlWindowModel"]) )
        {
        myTool            = t;
		showParmNames     = NO;
        selectedInlets[0] = [myTool isInletActiveWithIndex:0];
        selectedInlets[1] = [myTool isInletActiveWithIndex:1];
        selectedInlets[2] = [myTool isInletActiveWithIndex:2];
        selectedInlets[3] = [myTool isInletActiveWithIndex:3];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (void)setParmsPtr:(NSMutableArray*)p {

	[modelViewPtr setItems:p];
}

- (void)setSourceInformation {

    [connectedSourcesSelector removeAllItems];
	NSMutableArray* src = [myTool dataMatrices];
	if ( src == nil )
		{
		[connectedSourcesSelector addItemWithTitle:@"No data sources"];
		}
	else 
		{
        if ( [src count] == 0 )
            [connectedSourcesSelector addItemWithTitle:@"No data sources"];
        else if ( [src count] == 1 )
            {
            [connectedSourcesSelector addItemWithTitle:@"One data source"];
            NSString* str = [[[src objectAtIndex:0] name] lastPathComponent];
            [connectedSourcesSelector addItemWithTitle:str];
            }
        else 
            {
            NSString* s = [NSString stringWithFormat:@"%d data sources", (int)[src count]];
            [connectedSourcesSelector addItemWithTitle:s];
            for (int i=0; i<[src count]; i++)
                {
                NSString* str = [NSString stringWithFormat:@"M[%d]: ", i+1];
                str = [str stringByAppendingString:[[[src objectAtIndex:i] name] lastPathComponent]];
                [connectedSourcesSelector addItemWithTitle:str];
                }
            }
		}
}

- (IBAction)toggleShowParmNames:(id)sender {

	[modelViewPtr setShowParmNames:showParmNames];
	[modelViewPtr setNeedsDisplay:YES];
}

- (void)updateInletSelector {
    
    NSArray* str = [NSArray arrayWithObjects:@"Alignment",@"Unaligned",@"Trees",@"Numbers", nil];
    NSArray* btn = [NSArray arrayWithObjects:possibleInletsButton0,possibleInletsButton1,possibleInletsButton2,possibleInletsButton3,nil];
    
    for (int i=0; i<4; i++)
        {
        NSMutableDictionary* attrs = [NSMutableDictionary dictionaryWithCapacity:1];

        NSMutableParagraphStyle* style = [[NSMutableParagraphStyle alloc] init];
        [style setAlignment:NSCenterTextAlignment];

        [attrs setObject:[NSFont fontWithName:@"Lucida Grande" size:13.0] forKey:NSFontAttributeName];
        [attrs setObject:style forKey:NSParagraphStyleAttributeName];
        if ( [myTool isInletActiveWithIndex:i] == NO )
            [attrs setObject:[NSColor blackColor] forKey:NSForegroundColorAttributeName];
        else 
            [attrs setObject:[NSColor blueColor] forKey:NSForegroundColorAttributeName];
        NSMutableAttributedString* attrStr = [[NSMutableAttributedString alloc] initWithString:[str objectAtIndex:i] attributes:attrs];
        [(NSButton*)[btn objectAtIndex:i] setAttributedTitle:attrStr];
        }

}

- (void)windowDidLoad {

	[modelViewPtr setItems:[myTool parms]];
	[modelViewPtr setShowParmNames:showParmNames];
	[modelViewPtr setMyTool:myTool];
	[modelViewPtr setNeedsDisplay:YES];
}

- (void)windowDidResize {
    
    NSLog(@"model window resized");
}

- (BOOL)validateMenuItem:(NSMenuItem*)item {

    NSLog(@"in validateMenuItem for model window controller");
    return YES;
}

- (NSMutableArray*)variables {

    return [myTool variables];
}

@end
