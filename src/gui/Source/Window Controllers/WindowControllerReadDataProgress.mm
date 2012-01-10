#import "WindowControllerReadDataProgress.h"



@implementation WindowControllerReadDataProgress

- (void)awakeFromNib {

    [progressBar setUsesThreadedAnimation:YES];
    [progressBar startAnimation:self];
    NSLog(@"can become key = %d", [[self window] canBecomeKeyWindow]);
}

- (id)init {

	if ( (self = [super initWithWindowNibName:@"ReadDataProgressWindow"]) )
        {
        }
    return self;
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
