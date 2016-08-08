#import <Foundation/Foundation.h>
@class ToolAlign;



@interface AlignmentMuscleTask : NSObject {

    NSTask*                     alignTask;
    NSPipe*                     outputPipe;
    NSFileHandle*               outputFileHandle;
    ToolAlign*                  myAlignmentTool;
}

- (void)alignFile:(id)fInfo;
- (void)alignmentTaskDidFinish:(NSNotification*)notification;
- (id)initWithAlignmentTool:(ToolAlign*)t;
- (void)receiveData:(NSNotification*)aNotification;

@end
