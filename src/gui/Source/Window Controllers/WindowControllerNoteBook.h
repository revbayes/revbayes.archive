#import <Cocoa/Cocoa.h>
@class AnalysisTools;
@class EditorBackgroundView;
@class NotebookView;



@interface WindowControllerNoteBook : NSWindowController {

    IBOutlet NotebookView*       textView;
    NSMutableAttributedString*   mString;
	AnalysisTools*               myAnalysis;
}

@property (nonatomic,strong,getter=string,setter=setString:) NSMutableAttributedString* mString;

- (id)initWithAnalysis:(AnalysisTools*)a;

@end
