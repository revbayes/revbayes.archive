#import "GuiTree.h"
#import "TreePeekView.h"
#import "WindowControllerTreePeek.h"
#import "WindowControllerTreeViewer.h"




@implementation WindowControllerTreePeek

- (BOOL)acceptsFirstResponder {

    return YES;
}

- (void)awakeFromNib {

}

- (GuiTree*)currentTree {

    return [myOwner currentTree];
}

- (BOOL)drawAsMonophyleticIngroup {

    return [myOwner drawMonophyleticWrOutgroup];
}

- (id)initWithController:(WindowControllerTreeViewer*)wc {

	if ( (self = [super initWithWindowNibName:@"TreePeek"]) )
        {
        myOwner = wc;
        }
	return self;
}

- (void)keyDown:(NSEvent*)theEvent {

    NSLog(@"keyDown");
    [NSApp stopModal];
	[self close];
}

- (void)mouseDown:(NSEvent*)theEvent {

    [NSApp stopModal];
	[self close];
}

- (void)mouseUp:(NSEvent*)theEvent {

}

- (void)updateTreePeekView {

    [myView setNeedsDisplay:YES];
}

- (void)windowDidLoad {

    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

@end
