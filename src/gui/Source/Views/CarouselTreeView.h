#import <Cocoa/Cocoa.h>
@class GuiTree;



@interface CarouselTreeView : NSView {

    GuiTree*                myTree;

}

- (id)initWithFrame:(NSRect)frame andTree:(GuiTree*)t;

@end
