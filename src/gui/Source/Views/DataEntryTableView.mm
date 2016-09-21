#import "DataEntryTableView.h"
#import "WindowControllerDataEntry.h"



@implementation DataEntryTableView

- (void)drawRect:(NSRect)dirtyRect {

    [super drawRect:dirtyRect];
}

- (void)keyDown:(NSEvent*)event {

    unichar key = [[event charactersIgnoringModifiers] characterAtIndex:0];
    if (key == NSDeleteCharacter)
        {
        NSInteger c = [self selectedColumn];
        if (c != -1)
            {
            [dataEntryController deleteColumn:self];
            return;
            }
        
        NSInteger r = [self selectedRow];
        if (r != -1)
            {
            [dataEntryController deleteRow:self];
            return;
            }
        }
}

@end
