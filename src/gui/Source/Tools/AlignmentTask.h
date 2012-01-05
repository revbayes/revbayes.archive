#import <Foundation/Foundation.h>
@class ToolAlign;



@interface AlignmentTask : NSObject {

    NSTask*                     alignTask;
    ToolAlign*                  myAlignmentTool;
}

- (void)alignFile:(id)fInfo;
- (void)alignmentTaskDidFinish:(NSNotification*)notification;
- (id)initWithAlignmentTool:(ToolAlign*)t;

@end
