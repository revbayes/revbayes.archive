//
//  WindowControllerNumberMatrix.m
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 3/25/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "WindowControllerNumberMatrix.h"
#import "Trace.h"


@implementation WindowControllerNumberMatrix

//@synthesize burninLabel;
//@synthesize convergenceLabel;
//@synthesize essLabel;
//@synthesize meanLabel;
//@synthesize medianLabel;
//@synthesize parameterNameLabel;
//@synthesize semLabel;

- (IBAction)closeAction:(id)sender {
    
    [self close];
}

- (void)dealloc
{
    [super dealloc];
}

- (void)drawerDidClose:(NSNotification*)notification {
    
    [showInfoButton setTitle:@"Show Information"];
}

- (void)drawerDidOpen:(NSNotification*)notification {
    
    [showInfoButton setTitle:@"Hide Information"];
    
}

- (IBAction)helpButtonAction:(id)sender {
    
}

- (id)initWithTool:(ToolNumericalMcmcOutput*)t 
{
    if ( (self = [super initWithWindowNibName:@"NumberMatrix"]) )
    {
        // allocate objects
    
        // initialize the address of the tool associated with this control window
        myTool = t;
    
        // initialize the data
        [self setTableColumns];
        
    }
    return self;
}

-(void)tableView:(NSTableView *)tableView mouseDownInHeaderOfTableColumn:(NSTableColumn *)tableColumn {
    NSInteger index     = [table columnWithIdentifier:[tableColumn identifier]];;
    if (index >= 0 && index < (int)[myTool data]->size()) {
        RevBayesCore::Trace* t            = [myTool data]->at((int) index);
        
        [burninLabel setStringValue:[NSString stringWithFormat:@"%lu", (unsigned long)(t->getBurnin())]];
        [convergenceLabel setStringValue:[NSString stringWithFormat:@"%lu", 0]];
        [essLabel setStringValue:[NSString stringWithFormat:@"%f", t->getEss()]];
        [meanLabel setStringValue:[NSString stringWithFormat:@"%f", t->getMean()]];
        [medianLabel setStringValue:[NSString stringWithFormat:@"%f", t->getMedian()]];
        //        [parameterNameLabel setStringValue:[NSString stringWithFormat:@"TestTestTest"]];
        [parameterNameLabel setStringValue:[NSString stringWithCString:t->getParameterName().c_str() encoding:NSUTF8StringEncoding]];
        [semLabel setStringValue:[NSString stringWithFormat:@"%f", t->getSem()]];
    }
}

- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
    // HACK: check if the number of columns is correct; if not, reset columns
    std::vector<RevBayesCore::Trace* >* data  = [myTool data];
    int dataSize                = (int) data->size();
    
    if ([[table tableColumns] count] != dataSize) 
    {
        [self setTableColumns];
    }
    
    RevBayesCore::Trace* t = data->at(0);
    
    return t->size();
}

- (void)setControlWindowSize {
    
	NSRect oldFrame = [[self window] frame];
    NSRect newFrame = oldFrame;
    
    // setting the window size to a default value which fits nicely all elements into the window. The window itself remains not-resizeable.
//    newFrame.size.height = 178.0;
    newFrame.origin.y += (oldFrame.size.height - newFrame.size.height);
    [[self window] setFrame:newFrame display:YES animate:YES];
}

- (void)setTableColumns {
    // first, removing all old columns
    NSArray* tc = [table tableColumns];
    
    while ([tc count] > 0)
    {
        NSTableColumn* col = [tc objectAtIndex:0]; 
        [table removeTableColumn:col];
    }
    
    // adding columns
    float width = 100;
    float minWidth = 100;
    
    // adding a column for each trace
    std::vector<RevBayesCore::Trace* >* data = [myTool data];
    
    for (int i=0; i<data->size(); i++) {
        // get the trace i
        RevBayesCore::Trace* t = data->at(i);
        // get the name for the trace
        NSString* identifier = [[NSString alloc] initWithCString:t->getParameterName().c_str() encoding:NSUTF8StringEncoding];
        [identifier autorelease];
        // create a table column
        NSTableColumn* col = [[NSTableColumn alloc] initWithIdentifier:identifier];
        [col setWidth:width];
        [col setMinWidth:minWidth];
        [col autorelease];
        // set the value for the table header
        NSTableHeaderCell* header = [col headerCell];
        [header setObjectValue:identifier];
        // add the table column to the table view
        [table addTableColumn:col];
    }
    
    tc = [table tableColumns];
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row
{
    // HACK: check if the number of columns is correct; if not, reset columns
    if ([[table tableColumns] count] != [myTool data]->size()) 
    {
        [self setTableColumns];
    }
    
    std::vector<RevBayesCore::Trace* >* d = [myTool data];
    
    for (int i=0; i<d->size(); i++) 
    {
        RevBayesCore::Trace* t = d->at(i);
        NSString* parmName = [[NSString alloc] initWithCString:t->getParameterName().c_str() encoding:NSUTF8StringEncoding];
        [parmName autorelease];
        if ([parmName isEqualToString:[[tableColumn headerCell] objectValue]] == YES)
        {
            double val = t->objectAt(row);
            NSNumber* n = [[NSNumber alloc] initWithDouble:val];
            [n autorelease];
            return n;
        }
    }

    return nil;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

@end
