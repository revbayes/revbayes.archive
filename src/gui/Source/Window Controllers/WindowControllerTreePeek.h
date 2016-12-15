#import <Cocoa/Cocoa.h>
@class GuiTree;
@class TreePeekView;
@class WindowControllerTreeViewer;



@interface WindowControllerTreePeek : NSWindowController {

    WindowControllerTreeViewer*         myOwner;
    IBOutlet TreePeekView*              myView;
}

- (GuiTree*)currentTree;
- (BOOL)drawAsMonophyleticIngroup;
- (id)initWithController:(WindowControllerTreeViewer*)wc;
- (void)updateTreePeekView;

@end
