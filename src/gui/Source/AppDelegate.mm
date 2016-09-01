#import "AppDelegate.h"



@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)notification {

    [[NSHelpManager sharedHelpManager] registerBooksInBundle:[NSBundle mainBundle]];
}

- (void)applicationWillTerminate:(NSNotification*)notification {

}

@end
