#import <Cocoa/Cocoa.h>
@class GuiTree;
@class WindowControllerTreeViewer;



@interface CarouselTreeView : NSView {

    GuiTree*                    myTree;
    WindowControllerTreeViewer* myWindowController;
    BOOL                        drawAsMonophyleticIngroup;
}

@property (nonatomic) BOOL drawAsMonophyleticIngroup;

- (id)initWithFrame:(NSRect)frame andTree:(GuiTree*)t andController:(WindowControllerTreeViewer*)wc;

@end
