#import "MyPanel.h"



@implementation MyPanel

- (id)init {

    if ( (self = [super init]) ) 
        {
        }
    return self;
}

- (BOOL)canBecomeKeyWindow {

    return YES;
}

@end
