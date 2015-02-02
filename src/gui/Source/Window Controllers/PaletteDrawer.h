#import <Cocoa/Cocoa.h>
@class PaletteDrawerDelegate;
@class PaletteView;
@class WindowControllerModel;



@interface PaletteDrawer : NSDrawer {
    
    IBOutlet NSPopUpButton*           paletteSelector;
    IBOutlet NSScrollView*            paletteScrollView;
    IBOutlet PaletteView*             paletteView;
    IBOutlet WindowControllerModel*   modelWindowController;
    PaletteDrawerDelegate*            myDelegate;
}

- (IBAction)changePaletteSet:(id)sender;
- (NSString*)titleOfSelectedPaletteSet;

@end
