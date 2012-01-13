#import <Cocoa/Cocoa.h>



@interface WindowControllerProgressBar : NSWindowController {

    IBOutlet NSTextField*             message;
    IBOutlet NSProgressIndicator*     progressBar;
}

- (id)init;
- (void)setMessage:(NSString*)msg;

@end
