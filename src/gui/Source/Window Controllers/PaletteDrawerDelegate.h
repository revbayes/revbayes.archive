#import <Cocoa/Cocoa.h>
@class PaletteView;
@class WindowControllerModel;



@interface PaletteDrawerDelegate : NSObject <NSDrawerDelegate> {
    
    PaletteView*               myPaletteView;
    WindowControllerModel*     myWindowController;
}

- (id)initWithPaletteView:(PaletteView*)pv andWindowController:(WindowControllerModel*)wc;

@end
