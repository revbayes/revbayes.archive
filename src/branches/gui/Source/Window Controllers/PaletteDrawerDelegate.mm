#import "PaletteDrawerDelegate.h"
#import "PaletteView.h"
#import "WindowControllerModel.h"



@implementation PaletteDrawerDelegate

- (void)drawerDidClose:(NSNotification*)notification {
    
    [myWindowController drawerClosed];
}

- (void)drawerDidOpen:(NSNotification*)notification {
    
    [myPaletteView setViewBounds];
    [myWindowController drawerOpened];
}

- (id)initWithPaletteView:(PaletteView*)pv andWindowController:(WindowControllerModel*)wc {
    
    if ( (self = [super init]) ) 
        {
        myPaletteView = pv;
        myWindowController = wc;
        }
    return self;
}

@end
