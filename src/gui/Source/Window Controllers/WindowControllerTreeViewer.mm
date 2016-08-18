#import "CarouselTreeView.h"
#import "GuiTree.h"
#import "Node.h"
#import "ToolTreeSet.h"
#import "TreeSetView.h"
#import "WindowControllerTreeViewer.h"




@implementation WindowControllerTreeViewer

@synthesize carousel;
@synthesize items;
@synthesize wrap;

@synthesize selectedTree;
@synthesize fontSize;
@synthesize drawMonophyleticWrOutgroup;

- (GuiTree*)activeTree {

    return [myTool getTreeIndexed:selectedTree];
}

- (void)awakeFromNib {

    [self initializeTreeInformation];

    if ([myTool numberOfTreesInSet] <= 1)
        {
        [treeSelector setHidden:YES];
        [treeSelectorLabel setHidden:YES];
        }
    else
        {
        [treeSelector setHidden:NO];
        [treeSelectorLabel setHidden:NO];
        [treeSelector removeAllItems];
        for (int i=1; i<=[myTool numberOfTreesInSet]; i++)
            {
            [treeSelector addItemWithObjectValue:[NSNumber numberWithInteger:i]];
            }
        }

    //configure carousel
    [self.window makeFirstResponder:self.carousel];
}

- (IBAction)changeFontSize:(id)sender {

    [treeView setNeedsDisplay:YES];
}

- (IBAction)changedDrawMonophyleticTreeCheck:(id)sender {

    if ([drawMonophyleticTreeCheck state] == NSOnState)
        [drawMonophyleticTreeCheck setTitle:@"Yes"];
    else
        [drawMonophyleticTreeCheck setTitle:@"No"];
    [treeView setNeedsDisplay:YES];
}

- (IBAction)changeOutgroup:(id)sender {

    int n = [myTool numberOfTreesInSet];
    if (n > 0)
        {
        // get the name of the desired outgroup name
        NSString* outgroupName = [[outgroupList selectedItem] title];
        
        for (int i=0; i<n; i++)
            {
            // get the tree
            GuiTree* t = [myTool getTreeIndexed:i];
            [t initializeDownPassSequence];
            
            // find the node in the tree with the outgroup name
            Node* outgroupNode = [t nodeWithName:outgroupName];
            if (outgroupNode == nil)
                {
                NSAlert* alert = [[NSAlert alloc] init];
                [alert setMessageText:@"Problem Rerooting tree"];
                [alert setInformativeText:@"Cannot find outgroup node."];
                [alert runModal];
                }
            else
                {
                // reroot the tree
                [t rootTreeOnNode:outgroupNode];
                [t setCoordinates:[self drawMonophyleticWrOutgroup]];
                }
            }
        }

    [treeView setNeedsDisplay:YES];
}

- (IBAction)closeButtonAction:(id)sender {

    [myTool closeInspectorPanel];
}

- (void)dealloc {

	//it's a good idea to set these to nil here to avoid
	//sending messages to a deallocated window or view controller
	carousel.delegate = nil;
	carousel.dataSource = nil;
}

- (IBAction)helpButtonAction:(id)sender {

}

- (IBAction)incrementFontSize:(id)sender {

    int x = [fontStepper intValue];
    [self setFontSize:(float)x];
    [treeView setNeedsDisplay:YES];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolTreeSet*)t {

	if ( (self = [super initWithWindowNibName:@"TreeViewer"]) )
        {
        myTool       = t;
        selectedTree = 0;
        fontSize     = 14.0;

        }
	return self;
}

- (void)initializeTreeInformation {

#   if 0
    selectedTree = 0;
    int n = [myTool numberOfTreesInSet];
    if (n == 0)
        {
        [[self window] setTitle:@"Tree Set (Contains No Trees)"];
        [treeCounter setStringValue:@"N/A"];
        [treeCounter setHidden:YES];
        [fontLabel setHidden:YES];
        [fontEntry setHidden:YES];
        [self populateOutgroupList];
        [leftTree setEnabled:NO];
        [rightTree setEnabled:NO];
        }
    else
        {
        if (n == 1)
            [[self window] setTitle:@"Tree Set (Contains One Tree)"];
        else
            [[self window] setTitle:[NSString stringWithFormat:@"Tree Set (Contains %d Trees)", n]];
        [treeCounter setStringValue:[NSString stringWithFormat:@"%d", 1]];
        [fontLabel setHidden:NO];
        [fontEntry setHidden:NO];
        if (n == 1)
            {
            [treeCounter setHidden:YES];
            [leftTree setEnabled:NO];
            [rightTree setEnabled:NO];
            [leftTree setHidden:YES];
            [rightTree setHidden:YES];
            [treeStepLabel setHidden:YES];
            }
        else 
            {
            [treeCounter setHidden:NO];
            [leftTree setEnabled:NO];
            [rightTree setEnabled:YES];
            [leftTree setHidden:NO];
            [rightTree setHidden:NO];
            [treeStepLabel setHidden:NO];
            }
        [self populateOutgroupList];
        }
    [self changeOutgroup:self];
#   endif
}

- (IBAction)leftTreeAction:(id)sender {

    [self setSelectedTree:(selectedTree-1)];
    [treeCounter setStringValue:[NSString stringWithFormat:@"%d", selectedTree+1]];
    [treeView setNeedsDisplay:YES];
    if (selectedTree == 0)
        [leftTree setEnabled:NO];
    else
        [leftTree setEnabled:YES];
    if (selectedTree+1 >= [myTool numberOfTreesInSet])
        [rightTree setEnabled:NO];
    else
        [rightTree setEnabled:YES];
}

- (void)populateOutgroupList {

    [outgroupList removeAllItems];
    int n = [myTool numberOfTreesInSet];
    if (n > 0)
        {
        GuiTree* t = [myTool getTreeIndexed:0];
        [t initializeDownPassSequence];
        if (t != nil)
            {
            for (int i=0; i<[t numberOfNodes]; i++)
                {
                Node* p = [t downPassNodeIndexed:i];
                if ([p numberOfDescendants] == 0)
                    [outgroupList addItemWithTitle:[p name]];
                }
            }
        }
}

- (IBAction)showWindow:(id)sender {

    [super showWindow:sender];
}

- (IBAction)rightTreeAction:(id)sender {

    [self setSelectedTree:(selectedTree+1)];
    [treeCounter setStringValue:[NSString stringWithFormat:@"%d", selectedTree+1]];
    [treeView setNeedsDisplay:YES];
    if (selectedTree == 0)
        [leftTree setEnabled:NO];
    else
        [leftTree setEnabled:YES];
    if (selectedTree+1 >= [myTool numberOfTreesInSet])
        [rightTree setEnabled:NO];
    else
        [rightTree setEnabled:YES];
}

- (IBAction)selectTree:(id)sender {

    int whichTree = (int)[treeSelector integerValue];
    if (whichTree >= 1 && whichTree <= [myTool numberOfTreesInSet])
        {
        [carousel scrollToItemAtIndex:(NSInteger)(whichTree-1) duration:0.5];
        //[carousel scrollToItemAtIndex:(NSInteger)(whichTree-1) animated:YES];
        }
}

- (void)windowDidLoad {

    [super windowDidLoad];
}








#pragma mark -
#pragma mark iCarousel methods

- (NSInteger)numberOfItemsInCarousel:(__unused iCarousel *)carousel {

    //return (NSInteger)[self.items count];
    return (NSInteger)[myTool numberOfTreesInSet];
}

- (NSView*)carousel:(__unused iCarousel*)carousel viewForItemAtIndex:(NSInteger)index reusingView:(NSView*)view {

    NSTextField* label = nil;
    
    //create new view if no view is available for recycling
	if (view == nil)
        {
        // draw the tree
		//NSImage* image = [NSImage imageNamed:@"page240x500"];
		NSImage* image = [NSImage imageNamed:@"page300x625"];
        GuiTree* t = [myTool getTreeIndexed:(unsigned)index];
       	view = [[CarouselTreeView alloc] initWithFrame:NSMakeRect(0,0,image.size.width,image.size.height) andTree:t];
        
        // add the number
        label = [[NSTextField alloc] init];
        [label setBackgroundColor:[NSColor clearColor]];
        [label setBordered:NO];
        [label setSelectable:NO];
        [label setAlignment:NSCenterTextAlignment];
        [label setFont:[NSFont fontWithName:[(NSFont * __nonnull)[label font] fontName] size:18]];
        label.tag = 1;
        [view addSubview:label];
        }
	else
        {
		//get a reference to the label in the recycled view
		label = (NSTextField*)[view viewWithTag:1];
        }
    
	//set item label
    //remember to always set any properties of your carousel item
    //views outside of the `if (view == nil) {...}` check otherwise
    //you'll get weird issues with carousel item content appearing
    //in the wrong place in the carousel
	[label setStringValue:[NSString stringWithFormat:@"%lu", index+1]];
    [label sizeToFit];
    [label setFrameOrigin:NSMakePoint((view.bounds.size.width - label.frame.size.width)/2.0,
                                      (view.bounds.size.height - label.frame.size.height)/4.0)];


	return view;
}

- (NSInteger)numberOfPlaceholdersInCarousel:(__unused iCarousel*)carousel {

	//note: placeholder views are only displayed if wrapping is disabled
	return 2;
}

- (NSView*)carousel:(__unused iCarousel*)carousel placeholderViewAtIndex:(NSInteger)index reusingView:(NSView*)view {

	NSTextField* label = nil;
    
    //create new view if no view is available for recycling
	if (view == nil)
        {
        // draw the tree
		//NSImage* image = [NSImage imageNamed:@"page240x500"];
		NSImage* image = [NSImage imageNamed:@"page300x625"];
        GuiTree* t = [myTool getTreeIndexed:(unsigned)index];
       	view = [[CarouselTreeView alloc] initWithFrame:NSMakeRect(0,0,image.size.width,image.size.height) andTree:t];
        
        // add the number
        label = [[NSTextField alloc] init];
        [label setBackgroundColor:[NSColor clearColor]];
        [label setBordered:NO];
        [label setSelectable:NO];
        [label setAlignment:NSCenterTextAlignment];
        [label setFont:[NSFont fontWithName:[(NSFont * __nonnull)[label font] fontName] size:18]];
        label.tag = 1;
        [view addSubview:label];
        }
	else
        {
        //get a reference to the label in the recycled view
		label = (NSTextField*)[view viewWithTag:1];
        }
    
	//set item label
    //remember to always set any properties of your carousel item
    //views outside of the `if (view == nil) {...}` check otherwise
    //you'll get weird issues with carousel item content appearing
    //in the wrong place in the carousel
	[label setStringValue:(index == 0) ? @"[" : @"]"];
    [label sizeToFit];
    [label setFrameOrigin:NSMakePoint((view.bounds.size.width - label.frame.size.width)/2.0,
                                      (view.bounds.size.height - label.frame.size.height)/4.0)];

    return view;
}

- (CGFloat)carouselItemWidth:(__unused iCarousel *)carousel {

    //set correct view size
    //because the background image on the views makes them too large
    //return (CGFloat)200.0;
    return (CGFloat)300.0;
}

- (CATransform3D)carousel:(__unused iCarousel *)_carousel itemTransformForOffset:(CGFloat)offset baseTransform:(CATransform3D)transform {

    //implement 'flip3D' style carousel
    transform = CATransform3DRotate(transform, M_PI / (CGFloat)8.0, (CGFloat)0.0, (CGFloat)1.0, (CGFloat)0.0);
    return CATransform3DTranslate(transform, (CGFloat)0.0, (CGFloat)0.0, offset * self.carousel.itemWidth);
}

- (CGFloat)carousel:(__unused iCarousel *)_carousel valueForOption:(iCarouselOption)option withDefault:(CGFloat)value {

    //customize carousel display
    switch (option)
        {
        case iCarouselOptionWrap:
            {
            //normally you would hard-code this to YES or NO
            return self.wrap;
            }
        case iCarouselOptionSpacing:
            {
            //reduce item spacing to compensate
            //for drop shadow and reflection around views
            return value * (CGFloat)1.05;
            }
        case iCarouselOptionFadeMax:
            {
            return value;
            }
        case iCarouselOptionShowBackfaces:
        case iCarouselOptionRadius:
        case iCarouselOptionAngle:
        case iCarouselOptionArc:
        case iCarouselOptionTilt:
        case iCarouselOptionCount:
        case iCarouselOptionFadeMin:
        case iCarouselOptionFadeMinAlpha:
        case iCarouselOptionFadeRange:
        case iCarouselOptionOffsetMultiplier:
        case iCarouselOptionVisibleItems:
            {
            return value;
            }
        }
}

@end
