#import "iCarouselMacAppDelegate.h"



@implementation iCarouselMacAppDelegate

@synthesize window;
@synthesize windowController;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(__unused NSApplication*)sender {

    return YES;
}

@end
