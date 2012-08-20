#import <Cocoa/Cocoa.h>
@class WindowControllerTreeViewer;



@interface TreeSetView : NSView {

    IBOutlet WindowControllerTreeViewer*    myWindowController;
    IBOutlet NSTextField*                   treeInfo;
}

@end
