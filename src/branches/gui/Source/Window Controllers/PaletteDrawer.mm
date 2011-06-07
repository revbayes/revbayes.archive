#import "PaletteDrawer.h"
#import "PaletteDrawerDelegate.h"
#import "PaletteView.h"



@implementation PaletteDrawer

- (void)awakeFromNib {
    
	[paletteScrollView setHasVerticalScroller:YES];
	[[paletteScrollView verticalScroller] setControlSize:NSSmallControlSize];
    [paletteView updateViewFrame];

	myDelegate = [[PaletteDrawerDelegate alloc] initWithPaletteView:paletteView andWindowController:modelWindowController];
    [self setDelegate:myDelegate];
    [paletteView updateViewFrame];
    [paletteView setNeedsDisplay:YES];
}

- (IBAction)changePaletteSet:(id)sender {
    
    [paletteView updateViewFrame];
    [paletteView setNeedsDisplay:YES];
}

- (void)dealloc {
    
    [myDelegate release];
    [super dealloc];
}

- (id)init {
    
    if ( (self = [super init]) ) 
        {
        }
    return self;
}

- (NSString*)titleOfSelectedPaletteSet {
    
    return [paletteSelector titleOfSelectedItem];
}

- (BOOL)worksWhenModal {

    return YES;
}

@end
