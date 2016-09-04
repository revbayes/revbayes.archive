#import "ToolLoop.h"
#import "WindowControllerLoop.h"



@implementation WindowControllerLoop

@synthesize index;
@synthesize upperLimit;

- (void)awakeFromNib {

	[possibleIndicesButton setAutoenablesItems:NO];
    [self setIndices];
}

- (IBAction)cancelButtonAction:(id)sender {

    [myTool closeControlPanel];
}

- (id)init {

    return [self initWithTool:nil];
}

- (id)initWithTool:(ToolLoop*)t {

	if ( (self = [super initWithWindowNibName:@"ControlWindowLoop"]) )
        {
        myTool = t;
        [self setIndex:[NSString stringWithFormat:@"%c", [myTool indexLetter]]];
        [self setUpperLimit:1];
        }
	return self;
}

- (IBAction)okButtonAction:(id)sender {
    
    char c = [index characterAtIndex:0];
    [myTool setIndexLetter:c];
    [myTool setIndexUpperLimit:upperLimit];
    
    [myTool closeControlPanel];
}

- (void)setIndices {

    if (possibleIndicesButton == nil)
        return;

    NSString* selectedIdxStr = [NSString stringWithFormat:@"%c", [myTool indexLetter]];
    [self setIndex:selectedIdxStr];
    
    for (int i=0; i<[possibleIndicesButton numberOfItems]; i++)
        {
        NSMenuItem* mi = [possibleIndicesButton itemAtIndex:i];
        [mi setEnabled:YES];
        }
    
    NSMutableArray* unavailableIndices = [myTool unavailableIndices];
    for (int i=0; i<[unavailableIndices count]; i++)
        {
        NSString* c = [unavailableIndices objectAtIndex:i];
        NSMenuItem* mi = [possibleIndicesButton itemWithTitle:c];
        [mi setEnabled:NO];
        }
    
    [[possibleIndicesButton itemWithTitle:selectedIdxStr] setEnabled:YES];
}

- (void)windowDidLoad {

    [super windowDidLoad];
}

@end
