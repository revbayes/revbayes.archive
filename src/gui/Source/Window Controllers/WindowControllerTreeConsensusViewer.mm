#import "ConTreeView.h"
#import "GuiTree.h"
#import "ToolTreeConsensus.h"
#import "WindowControllerTreeConsensusViewer.h"



@interface WindowControllerTreeConsensusViewer ()

@end

@implementation WindowControllerTreeConsensusViewer

- (void)awakeFromNib {

    [[self window] makeFirstResponder:nil];
}

- (IBAction)changeOutgroup:(id)sender {

    NSString* newOutgroupName = [(NSMenuItem*)sender title];
    /*int newOutgroupIdx = [myTool indexOfTaxon:newOutgroupName];
    if (newOutgroupIdx == -1)
        {
        NSAlert* alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Problem Rerooting Tree"];
        [alert setInformativeText:@"Cannot find outgroup node."];
        [alert runModal];
        return;
        }*/
    
    GuiTree* conTree = [myTool consensusTree];
    for (NSMenuItem* m in outgroupMenuItems)
        {
        [m setState:NSOffState];
        if ([[m title] isEqualToString:newOutgroupName] == YES)
            {
            [m setState:NSOnState];
            if ( [newOutgroupName isEqualToString:[myTool outgroupName]] == NO )
                {
                [conTree rootTreeOnNodeNamed:newOutgroupName];
                [myTool setOutgroupName:newOutgroupName];
                }
            }
        }
    
    [self update];
}

- (IBAction)closeAction:(id)sender {

    [myTool closeInspectorPanel];
}

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSWindowDidBecomeKeyNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSWindowDidResignKeyNotification object:nil];
}

- (GuiTree*)getConsensusTree {

    return [myTool consensusTree];
}

- (id)init {

    return [self initWithTool:nil];
}

- (void)initializeTreeMenu {

    GuiTree* conTree = [myTool consensusTree];
    
    if (conTree != nil && treeMenu == nil)
        {
        treeMenu = [[NSMenuItem alloc] initWithTitle:@"Tree" action:NULL keyEquivalent:@""];
        NSMenu* treeMenuMenu = [[NSMenu alloc] initWithTitle:@"Tree" ];

        NSMenuItem* outgroupMenu = [[NSMenuItem alloc] initWithTitle:@"Outgroup" action:NULL keyEquivalent:@""];
        [treeMenu setSubmenu:treeMenuMenu];
        /*if (drawMonophyleticWrOutgroup == YES)
            [treeMenuMenu addItemWithTitle:@"Deroot Tree" action:@selector(changedDrawMonophyleticTree:) keyEquivalent:@""];
        else
            [treeMenuMenu addItemWithTitle:@"Root Tree" action:@selector(changedDrawMonophyleticTree:) keyEquivalent:@""];*/
        [treeMenuMenu addItem:outgroupMenu];
        
        [treeMenu setEnabled:YES];
        outgroupMenuItems = [[NSMutableArray alloc] init];
        if ([myTool consensusTree] != nil)
            {
            NSString* outgroupName = [myTool outgroupName];
            NSMenu* outgroupMenuMenu = [[NSMenu alloc] initWithTitle:@"Outgroup"];
            GuiTree* t = [myTool consensusTree];
            NSMutableArray* names = [t taxaNames];
            for (NSString* taxonName in names)
                {
                NSMenuItem* ogItem = [[NSMenuItem alloc] initWithTitle:taxonName action:@selector(changeOutgroup:) keyEquivalent:@""];
                if ( [taxonName isEqualToString:outgroupName] == YES )
                    [ogItem setState:NSOnState];
                [outgroupMenuMenu addItem:ogItem];
                [outgroupMenuItems addObject:ogItem];
                }
            [outgroupMenu setSubmenu:outgroupMenuMenu];
            }
        }
}

- (id)initWithTool:(ToolTreeConsensus*)t {

	if ( (self = [super initWithWindowNibName:@"ConTreeViewer"]) )
        {
        myTool = t;
        treeMenu = nil;
        
        [self initializeTreeMenu];

        NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
        [defaultCenter addObserver:self
                          selector:@selector(windowDidExpose:)
                              name:NSWindowDidBecomeKeyNotification
                            object:[self window]];
        [defaultCenter addObserver:self
                          selector:@selector(windowDidResign:)
                              name:NSWindowDidResignKeyNotification
                            object:[self window]];
        }
	return self;
}

- (void)update {

    [myView setNeedsDisplay:YES];
}

- (void)windowDidLoad {

    [super windowDidLoad];
    
    // initialize here
}

- (void)windowDidExpose:(NSNotification*)notification {

    [self initializeTreeMenu];
    if (treeMenu != nil)
        {
        NSMenu* mainMenu = [NSApp mainMenu];
        [mainMenu insertItem:treeMenu atIndex:5];
        }
}

- (void)windowDidResign:(NSNotification*)notification {

    if (treeMenu != nil)
        {
        NSMenu* mainMenu = [NSApp mainMenu];
        [mainMenu removeItem:treeMenu];
        }
}

- (BOOL)validateMenuItem:(NSMenuItem*)item {
	
	SEL act = [item action];
	if ( act == @selector(changeOutgroup:) )
        {
        return YES;
        }

    return YES;
}

@end
