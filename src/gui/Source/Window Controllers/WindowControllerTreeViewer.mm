#import "CarouselTreeView.h"
#import "GuiTree.h"
#import "Node.h"
#import "ToolTreeSet.h"
#import "WindowControllerTreePeek.h"
#import "WindowControllerTreeViewer.h"




@implementation WindowControllerTreeViewer

@synthesize carousel;
@synthesize items;
@synthesize wrap;

@synthesize selectedTree;
@synthesize drawMonophyleticWrOutgroup;

- (GuiTree*)activeTree {

    return [myTool getTreeIndexed:selectedTree];
}

- (void)awakeFromNib {
        
    // hide or show the tree selector, depending on how many tree are in the tree set
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
    [carousel reloadData];
<<<<<<< HEAD
=======

    if ([self activeTree] != nil)
        [carousel scrollToItemAtIndex:(NSInteger)(0) duration:0.5];
>>>>>>> development
    
    [infoLabel setHidden:YES];
}

- (IBAction)changedDrawMonophyleticTree:(id)sender {

    if ([myTool numberOfTreesInSet] == 1)
        {
        if (drawMonophyleticWrOutgroup == YES)
            {
            drawMonophyleticWrOutgroup = NO;
            [(NSMenuItem*)sender setTitle:@"Root Tree"];
            }
        else
            {
            drawMonophyleticWrOutgroup = YES;
            [(NSMenuItem*)sender setTitle:@"Deroot Tree"];
            }
        }
    else
        {
        if (drawMonophyleticWrOutgroup == YES)
            {
            drawMonophyleticWrOutgroup = NO;
            [(NSMenuItem*)sender setTitle:@"Root Trees"];
            }
        else
            {
            drawMonophyleticWrOutgroup = YES;
            [(NSMenuItem*)sender setTitle:@"Deroot Trees"];
            }
        }
    [carousel reloadData];
    [treePeeker updateTreePeekView];
}

- (IBAction)changeOutgroup:(id)sender {

    NSString* newOutgroupName = [(NSMenuItem*)sender title];
    int newOutgroupIdx = [myTool indexOfTaxon:newOutgroupName];
    if (newOutgroupIdx == -1)
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Problem Rerooting Tree"];
        [alert setInformativeText:@"Cannot find outgroup node."];
        [alert runModal];
        return;
        }
    
    for (NSMenuItem* m in outgroupMenuItems)
        {
        [m setState:NSOffState];
        if ([[m title] isEqualToString:newOutgroupName] == YES)
            {
            [m setState:NSOnState];
            if ( [newOutgroupName isEqualToString:[myTool outgroupName]] == NO )
                {
                [myTool rerootOnTaxonNamed:newOutgroupName];
                }
            }
        }
    
    [carousel reloadData];
    [treePeeker updateTreePeekView];
}

- (IBAction)closeButtonAction:(id)sender {

    [myTool closeInspectorPanel];
}

- (GuiTree*)currentTree {

    GuiTree* t = [myTool getTreeIndexed:(unsigned)[carousel currentItemIndex]];
    return t;
}
<<<<<<< HEAD

- (void)dealloc {

=======

- (void)dealloc {

>>>>>>> development
	//it's a good idea to set these to nil here to avoid
	//sending messages to a deallocated window or view controller
	carousel.delegate = nil;
	carousel.dataSource = nil;

    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSWindowDidBecomeKeyNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSWindowDidResignKeyNotification object:nil];
}

- (IBAction)helpButtonAction:(id)sender {

}

- (id)init {

    return [self initWithTool:nil];
}

- (void)initializeTreeMenu {

    treeMenu = [[NSMenuItem alloc] initWithTitle:@"Tree" action:NULL keyEquivalent:@""];
    NSMenu* treeMenuMenu = [[NSMenu alloc] initWithTitle:@"Tree" ];
    NSMenuItem* outgroupMenu = [[NSMenuItem alloc] initWithTitle:@"Outgroup" action:NULL keyEquivalent:@""];
    [treeMenu setSubmenu:treeMenuMenu];
    if (drawMonophyleticWrOutgroup == YES)
        [treeMenuMenu addItemWithTitle:@"Deroot Tree" action:@selector(changedDrawMonophyleticTree:) keyEquivalent:@""];
    else
        [treeMenuMenu addItemWithTitle:@"Root Tree" action:@selector(changedDrawMonophyleticTree:) keyEquivalent:@""];
    [treeMenuMenu addItem:outgroupMenu];
    
    [treeMenu setEnabled:YES];
    outgroupMenuItems = [[NSMutableArray alloc] init];
    if ([myTool numberOfTreesInSet] > 0)
        {
        NSString* outgroupName = [myTool getOutgroupName];
        NSMenu* outgroupMenuMenu = [[NSMenu alloc] initWithTitle:@"Outgroup"];
        GuiTree* t = [myTool getTreeIndexed:0];
        NSMutableArray* names = [t taxaNames];
        for (NSString* taxonName in names)
            {
            NSMenuItem* ogItem = [[NSMenuItem alloc] initWithTitle:taxonName action:@selector(changeOutgroup:) keyEquivalent:@""];
            if ( [taxonName isEqualToString:outgroupName] == YES )
                [ogItem setState:NSOnState];
            [outgroupMenuMenu addItem:ogItem];
            [outgroupMenuItems addObject:ogItem];
            }
        [outgroupMenu setSubmenu:outgroupMenuMenu];
        }
}

- (id)initWithTool:(ToolTreeSet*)t {

	if ( (self = [super initWithWindowNibName:@"TreeViewer"]) )
        {
        myTool       = t;
        selectedTree = 0;
        
        [self initializeTreeMenu];

        NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
        [defaultCenter addObserver:self
                          selector:@selector(windowDidExpose:)
                              name:NSWindowDidBecomeKeyNotification
                            object:[self window]];
        [defaultCenter addObserver:self
                          selector:@selector(windowDidResign:)
                              name:NSWindowDidResignKeyNotification
                            object:[self window]];
        }
	return self;
}

- (IBAction)showWindow:(id)sender {

    [super showWindow:sender];
}

- (IBAction)selectTree:(id)sender {

    int whichTree = (int)[treeSelector integerValue];
    if (whichTree >= 1 && whichTree <= [myTool numberOfTreesInSet])
        {
        [carousel scrollToItemAtIndex:(NSInteger)(whichTree-1) duration:0.5];
        }
}

- (void)showTreePeeker {

    treePeeker = nil;
    treePeeker = [[WindowControllerTreePeek alloc] initWithController:self];
    
    // make a rectangle of the appropriate width/heigth centered on the screen
    float verticalCoverage   = 0.95;
    float widthToHeightRatio = 0.75;  // 0.75 to make it the same as the carousel view window
    NSScreen* mainScreen = [NSScreen mainScreen];
    NSRect screenRect = [mainScreen visibleFrame];
    float h = screenRect.size.height * verticalCoverage;
    float w = h * widthToHeightRatio;
    NSPoint windowOrigin = CGPointMake(NSMidX(screenRect), NSMidY(screenRect));
    NSRect windowRect = NSMakeRect(windowOrigin.x - w * 0.5, windowOrigin.y - h * 0.5, w, h);
    
    // show the window
    [[treePeeker window] setFrame:windowRect display:YES];
	[treePeeker showWindow:self];
	[[treePeeker window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[treePeeker window]];
    [treePeeker updateTreePeekView];
}

- (void)updateInfoLabelForTreeIndexed:(int)idx {

    // get the tree
    GuiTree* t = [myTool getTreeIndexed:idx];
    
    // get the label information
    NSString* msg = [t info];
    
    // change the status field
    [infoLabel setEditable:NO];
    if ( [msg isEqualToString:@""] == YES || t == nil )
        {
        [infoLabel setHidden:YES];
        }
    else
        {
        [infoLabel setHidden:NO];
        [infoLabel setTextColor:[NSColor blueColor]];
        [infoLabel setStringValue:msg];
        }
}

- (void)windowDidExpose:(NSNotification*)notification {

    NSMenu* mainMenu = [NSApp mainMenu];
    [mainMenu insertItem:treeMenu atIndex:5];
}

- (void)windowDidResign:(NSNotification*)notification {

    NSMenu* mainMenu = [NSApp mainMenu];
    if (treeMenu != nil)
        [mainMenu removeItem:treeMenu];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

- (BOOL)validateMenuItem:(NSMenuItem*)item {
	
	SEL act = [item action];
	if ( act == @selector(changedDrawMonophyleticTree:) )
        {
        return YES;
        }
    else if ( act == @selector(changeOutgroup:) )
        {
        return YES;
        }

    return YES;
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
       	view = [[CarouselTreeView alloc] initWithFrame:NSMakeRect(0,0,image.size.width,image.size.height) andTree:t andController:self];
        
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
       	view = [[CarouselTreeView alloc] initWithFrame:NSMakeRect(0,0,image.size.width,image.size.height) andTree:t andController:self];
        
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
