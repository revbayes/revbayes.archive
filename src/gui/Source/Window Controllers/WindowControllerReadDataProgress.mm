#import "WindowControllerReadDataProgress.h"



@implementation WindowControllerReadDataProgress

- (void)awakeFromNib {

    [progressBar setUsesThreadedAnimation:YES];
    [progressBar startAnimation:self];
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
