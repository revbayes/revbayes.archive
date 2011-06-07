#import "ParmConstantVector.h"
#import "ParmConstant.h"
#import "ParmPlateTree.h"
#import "ParmTree.h"
#import "PartitionConstant.h"
#import "TaxonBipartition.h"
#import "ValueVector.h"



@implementation ParmConstantVector

- (void)awakeFromNib {
    
    // display the correct view
    [self chooseView];
}

- (IBAction)changeValueEntry:(id)sender {
    
    [self refreshWindow];
}

- (void)dealloc {
    
    [super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[super encodeWithCoder:aCoder];
}

- (id)init {
    
    if ( (self = [super init]) ) 
        {
        dimensions = 2;
        }
    return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {
    
    if ( (self = [super initWithScaleFactor:sf andTool:t]) ) 
		{
        }
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        }
	return self;
}

- (void)refreshWindow {
    
    // set the title of the control panel
    NSString* panelName = [NSString stringWithFormat:@"Control for \"%@\"", parmName];
    [[self window] setTitle:panelName];
    
    // get a pointer to objects we need
    ParmPlateTree* treePlate = [self assignedTreePlate];
    
    // display the correct view
    [self chooseView];
    
    // set the initial height of the control panel
    float h = 0.0;
    
    // set the dimensions
    NSString* dimStr = [NSString stringWithFormat:@"%d", [self dimensions]];
    [vectorSizeField setStringValue:dimStr];
        
    NSRect visibleBounds = [valueScroll visibleRect];
    if (treePlate == nil)
        {
        // set the view when the constant is not on a tree plate

        int nRows = [self dimensions];
        
        NSRect aRect = NSMakeRect(0.0,0.0,[valueScroll documentVisibleRect].size.width,(16.0*nRows));
        NSMatrix* vec = [[NSMatrix alloc] initWithFrame:aRect mode:NSListModeMatrix cellClass:[NSTextFieldCell class] numberOfRows:nRows numberOfColumns:1]; 
        [vec setScrollable:NO];
        [vec setIntercellSpacing:NSMakeSize(0.0,0.0)];
        [vec setAllowsEmptySelection:YES];
        NSArray* allCells = [vec cells];
        for (int i=0; i<nRows; i++)
            {
            Value* v = [self getValue];
            Value* e = [(ValueVector*)v valueIndexed:i];
            NSString* strRep = [e stringRepresentation];
            NSLog(@"%d -- %@", i, strRep);
            NSTextFieldCell* aCell = [allCells objectAtIndex:i];
            [aCell setTag:1];
            [aCell setEditable:YES];
            [aCell setSelectable:YES];
            [aCell setDrawsBackground:YES];
            [aCell setAlignment:NSRightTextAlignment];
            [aCell setStringValue:strRep];
            [aCell setBackgroundColor:[NSColor whiteColor]];
            [aCell setTextColor:[NSColor blackColor]];
            }
        [valueScroll setDocumentView:vec];
        [vec release];
        NSLog(@"vec cnt = %d", (int)[vec retainCount]);
        [valueScroll display];
        NSLog(@"%@", vec);
        NSLog(@"valueScroll = %@", valueScroll);
        NSLog(@"box coordinates = %@", NSStringFromRect([box frame]));
        NSLog(@"sv coordinates  = %@", NSStringFromRect([valueScroll frame]));
        NSLog(@"win coordinates = %@", NSStringFromRect([[self window] frame]));
        NSLog(@"vb              = %@", NSStringFromRect(visibleBounds));
        NSLog(@"dvr             = %@", NSStringFromRect([valueScroll documentVisibleRect]));
        //NSScrollView

        
        NSSize cs = [vec cellSize];
        [valueScroll setFrame:NSMakeRect(103.0, 0.0, visibleBounds.size.width, (nRows*cs.height-2.0))];

        
        
        
        h = 500.0;
        }
    else
        {
        // set the view when the parameter _is_ on a tree plate
        // set the distribution(s) when the parameter _is_ on a tree plate
        [partitionSelector setHidden:NO];
        [nodeBranchSelector setHidden:NO];
        BOOL partitionConstantsExist = [self hasPartitionConstants];
        if (partitionConstantsExist == NO)
            [self initializeValue];
        
        ParmTree* t = [treePlate getOrderingTree];
        if ( t == nil )
            {
            // we don't have a tree ordering the plate, so we cannot display the bipartitions for model selection
            [partitionSelector removeAllItems];
            [partitionSelector addItemWithTitle:@"No tree orders tree plate"];
            [nodeBranchSelector setEnabled:NO forSegment:0];
            [nodeBranchSelector setEnabled:NO forSegment:1];
            [valueEntry setEnabled:NO];
            [valueEntry setStringValue:@""];
            }
        else
            {
            // we do have a tree and display the model partitions
            
            // set the partition selector
            [partitionSelector removeAllItems];
            for (int i=0; i<[t numBipartitions]; i++)
                [partitionSelector addItemWithTitle:[[t getBipartitionIndexed:i] partitionName]];
            if ( (indexOfSelectedPartitionModel + 1) > [partitionSelector numberOfItems] )
                indexOfSelectedPartitionModel = 0;
            [partitionSelector selectItemWithTitle:[[t getBipartitionIndexed:indexOfSelectedPartitionModel] partitionName]];
            [nodeBranchSelector setEnabled:YES forSegment:0];
            [nodeBranchSelector setEnabled:YES forSegment:1];
            [valueEntry setEnabled:YES];
            
            // set the constant value for the selected partition
            PartitionConstant* pc = [constantValues objectAtIndex:indexOfSelectedPartitionModel];
            Value* pv = [pc value];
            
            [valueEntry removeAllItems];
            // TO DO: Add possibilities depending on the sources connected to the Model Tool
            //NSArray* ma = [NSArray arrayWithObjects:@"name 1",@"name 2",nil];
            //[valueEntry addItemsWithObjectValues:ma];
            if (valueSource >= 0)
                valueSource = -1; // only allow selection of value explicitly for now
            NSString* valStr = [pv stringRepresentation];
            [valueEntry setStringValue:valStr];
            
            }
        
        h = 500.0;
        }
    
    // set the dimensions information, if the parameter is a vector
    if ( [self isVector] == YES )
        {
        h += 46.0;
        }
    else
        {
        }
    
    // resize the control panel
    [self resizeWindowTo:h];
}

@end
