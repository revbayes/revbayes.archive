#import "WindowControllerProgressBar.h"



@implementation WindowControllerProgressBar

- (void)awakeFromNib {

    [progressBar setUsesThreadedAnimation:YES];
    [progressBar startAnimation:self];
}

- (id)init {

	if ( (self = [super initWithWindowNibName:@"ProgressBarWindow"]) )
        {
        }
    return self;
}

- (void)setMessage:(NSString*)msg {

    [message setStringValue:msg];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
