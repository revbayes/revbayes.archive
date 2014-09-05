#import "ParmConstant.h"
#import "ParmPlateTree.h"
#import "ParmTree.h"
#import "PartitionConstant.h"
#import "RevBayes.h"
#import "TaxonBipartition.h"
#import "ToolModel.h"
#import "Value.h"
#import "ValueVector.h"
#import "ViewControllerConstant.h"



@implementation ParmConstant

@synthesize appliesToNode;

- (Value*)allocateValue {

    return nil;
}

- (IBAction)cancelAction:(id)sender {
    
    [self restoreWindowState];
	[self closeControlPanel];
}

- (IBAction)changeDimensionsActionField:(id)sender {
    
    int n = [[vectorSizeField stringValue] intValue];
    if (n >= 2 && n <= 100000)
        {
        [self setDimensions:n];
        Value* v = [self getValue];
        [(ValueVector*)v setDimensions:[self dimensions]];
        }
    [vectorSizeField setStringValue:[NSString stringWithFormat:@"%d", dimensions]];
    [self refreshWindow];
}

- (IBAction)changeDimensionsActionStepper:(id)sender {
    
    Value* v = [self getValue];
    [(ValueVector*)v setDimensions:[self dimensions]];
    [self refreshWindow];
}

- (IBAction)changeNodeOrBranchSelection:(id)sender {
    
    ParmPlateTree* treePlate = [self assignedTreePlate];
    if ( treePlate == nil )
        {
        }
    else
        {

        }
    [self refreshWindow];
}

- (IBAction)changePartitionSelectionAction:(id)sender {
    
    int idx = (int)[partitionSelector indexOfSelectedItem];
    indexOfSelectedPartitionModel = idx;
    [self refreshWindow];
}

- (IBAction)changeValueEntry:(id)sender {
            
    if ( [valueEntry objectValueOfSelectedItem] == nil )
        {
        // the user is entering a value directly into the text box of the combobox
        if ( [self isValidEntry:[valueEntry stringValue]] == YES )
            {
            if ( [self assignedTreePlate] == nil )
                {
                Value* v = [constantValues objectAtIndex:0];
                [self setValueFromString:[valueEntry stringValue] forValue:v];
                }
            else
                {
                if ( [[self assignedTreePlate] getOrderingTree] != nil )
                    {
                    PartitionConstant* pc = [constantValues objectAtIndex:indexOfSelectedPartitionModel];
                    Value* v = [pc value];
                    [self setValueFromString:[valueEntry stringValue] forValue:v];
                    }
                }

            }
        else
            {
            Value* v = nil;
            if ( [self assignedTreePlate] == nil )
                v = [constantValues objectAtIndex:0];
            else
                {
                PartitionConstant* pc = [constantValues objectAtIndex:indexOfSelectedPartitionModel];
                v = [pc value];
                }
            [valueEntry setStringValue:[v stringRepresentation]];
            }
        valueSource = -1;
        }
    else
        {
        // the user is selecting an external source for the value for this constant
        valueSource = (int)[valueEntry indexOfSelectedItem];
        }
    [self refreshWindow];
}

- (void)chooseView {
    
    if ( ([self assignedTreePlate] == nil && [self isVector] == NO  && whichView == 0) || 
         ([self assignedTreePlate] != nil && [self isVector] == NO  && whichView == 1) || 
         ([self assignedTreePlate] == nil && [self isVector] == YES && whichView == 2) ||
         ([self assignedTreePlate] != nil && [self isVector] == YES && whichView == 3) )
        return;
        
    NSWindow* w = [box window];
    BOOL ended = [w makeFirstResponder:w];
    if (!ended)
        return;
    
    if ( [self assignedTreePlate] == nil && [self isVector] == NO )
        {
        NSView* v = [parameterView view];
        [box setContentView:v];
        nameField             = [parameterView nameField];
        nameLabel             = [parameterView nameLabel];
        valueEntry            = [parameterView valueEntry];
        whichView             = 0;
        }
    else if ( [self assignedTreePlate] != nil && [self isVector] == NO )
        {
        NSView* v = [treePlateView view];
        [box setContentView:v];
        nameField             = [treePlateView nameField];
        nameLabel             = [treePlateView nameLabel];
        valueEntry            = [treePlateView valueEntry];
        partitionSelector     = [treePlateView partitionSelector];
        nodeBranchSelector    = [treePlateView nodeBranchSelector];
        whichView             = 1;
        }
    else if ( [self assignedTreePlate] == nil && [self isVector] == YES )
        {
        NSView* v = [parameterVectorView view];
        [box setContentView:v];
        nameField             = [parameterVectorView nameField];
        nameLabel             = [parameterVectorView nameLabel];
        vectorSizeStepper     = [parameterVectorView vectorSizeStepper];
        vectorSizeField       = [parameterVectorView vectorSizeField];
        vectorSizeLabel       = [parameterVectorView vectorSizeLabel];
        valueScroll           = [parameterVectorView valueScroll];
        whichView             = 2;
        [valueScroll setHasHorizontalScroller:NO];
        [[valueScroll horizontalScroller] setControlSize:NSSmallControlSize];
        [valueScroll setHasVerticalScroller:YES];
        [[valueScroll verticalScroller] setControlSize:NSSmallControlSize];
        }
    else if ( [self assignedTreePlate] != nil && [self isVector] == YES )
        {
        NSView* v = [treePlateVectorView view];
        [box setContentView:v];
        nameField             = [treePlateVectorView nameField];
        nameLabel             = [treePlateVectorView nameLabel];
        partitionSelector     = [treePlateVectorView partitionSelector];
        nodeBranchSelector    = [treePlateVectorView nodeBranchSelector];
        vectorSizeStepper     = [treePlateVectorView vectorSizeStepper];
        vectorSizeField       = [treePlateVectorView vectorSizeField];
        vectorSizeLabel       = [treePlateVectorView vectorSizeLabel];
        valueScroll           = [treePlateVectorView valueScroll];
        whichView             = 3;
        //[valueScroll setHasHorizontalScroller:NO];
        //[[valueScroll horizontalScroller] setControlSize:NSSmallControlSize];
        //[valueScroll setHasVerticalScroller:YES];
        //[[valueScroll verticalScroller] setControlSize:NSSmallControlSize];
        }
}

- (void)encodeWithCoder:(NSCoder *)aCoder {
    
    [super encodeWithCoder:aCoder];
    [aCoder encodeObject:constantValues forKey:@"constantValues"];
    [aCoder encodeInt:indexOfSelectedPartitionModel forKey:@"indexOfSelectedPartitionModel"];
    [aCoder encodeBool:appliesToNode forKey:@"appliesToNode"];
    [aCoder encodeInt:valueSource forKey:@"valueSource"];
}

- (NSArray*)getPotentialClampValues {
    
    return nil;
}

- (Value*)getValue {
    
    Value* v = nil;
    if ( [self assignedTreePlate] == nil )
        {
        v = [constantValues objectAtIndex:0];
        }
    else
        {
        if ( [[self assignedTreePlate] getOrderingTree] != nil )
            {
            PartitionConstant* pc = [constantValues objectAtIndex:indexOfSelectedPartitionModel];
            v = [pc value];
            }
        }
    return v;
}

- (BOOL)hasPartitionConstants {
    
    if ( [constantValues count] == 0 )
        return NO;
    for (size_t i=0; i<[constantValues count]; i++)
        {
        if ( [[[constantValues objectAtIndex:i] className] isEqualToString:@"PartitionConstant"] == NO )
            return NO;
        }
    return YES;
}

- (id)init {
    
    self = [self initWithScaleFactor:1.0 andTool:nil];
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {
    
    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        constantValues                = [aDecoder decodeObjectForKey:@"constantValues"];
        indexOfSelectedPartitionModel = [aDecoder decodeIntForKey:@"indexOfSelectedPartitionModel"];
        appliesToNode                 = [aDecoder decodeBoolForKey:@"appliesToNode"];
        valueSource                   = [aDecoder decodeIntForKey:@"valueSource"];
        whichView = -1;
        savedConstantValues = [[NSMutableArray alloc] init];
		}
	return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {
    
    if ( (self = [super initWithScaleFactor:sf andTool:t]) ) 
		{
        constantValues = [[NSMutableArray alloc] init];
        savedConstantValues = [[NSMutableArray alloc] init];
        indexOfSelectedPartitionModel = 0;
        isConstant                    = YES;
        valueSource                   = -1;
        whichView                     = -1;
        [self setAppliesToNode:YES];
		}
    return self;
}

- (void)initializeImage {
    
	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)initializeValue {
    
    ParmPlateTree* tp = [self assignedTreePlate];
    if ( tp == nil )
        {
        Value* myValue = [self allocateValue];
        [constantValues removeAllObjects];
        [constantValues addObject:myValue];
        }
    else
        {
        ParmTree* t = [tp getOrderingTree];
        [constantValues removeAllObjects];
        if ( t != nil )
            {
            indexOfSelectedPartitionModel = 0;
            for (int i=0; i<[t numBipartitions]; i++)
                {
                TaxonBipartition* p = [t getBipartitionIndexed:i];
                PartitionConstant* pm = [[PartitionConstant alloc] initWithParm:self];
                Value* myValue = [self allocateValue];
                [pm setValue:myValue];
                [pm setPartition:p];
                [constantValues addObject:pm];
                }
            }
        }
}

- (BOOL)isValidEntry:(NSString*)str {
    
    int pd = [self domain];
    NSScanner* myStrScanner = [NSScanner scannerWithString:str];
    
    int xi;
    float xf;
    BOOL isGood = YES;
    if ( pd == REAL_NUMBER )
        {
        isGood = [myStrScanner scanFloat:&xf]; 
        }
    else if ( pd == POSITIVE_REAL_NUMBER )
        {
        isGood = [myStrScanner scanFloat:&xf]; 
        if (xf < 0.0)
            isGood = NO;
        }
    else if ( pd == INTEGER )
        {
        isGood = [myStrScanner scanInt:&xi]; 
        }
    else if ( pd == POSITIVE_INTEGER )
        {
        isGood = [myStrScanner scanInt:&xi]; 
        if (xi < 0)
            isGood = NO;
        }
    
    return isGood;
}

- (IBAction)okAction:(id)sender {
    
    [savedConstantValues removeAllObjects];
	[self closeControlPanel];
}

- (void)refreshWindow {
    
    // refresh for scalar constants...this function is different in ParmConstantVector
        
    // set the title of the control panel
    NSString* panelName = [NSString stringWithFormat:@"Control for \"%@\"", parmName];
    [[self window] setTitle:panelName];
    
    // get a pointer to objects we need
    ParmPlateTree* treePlate = [self assignedTreePlate];
    
    // display the correct view
    [self chooseView];
    
    // set the initial height of the control panel
    float h = 0.0;
        
    if (treePlate == nil)
        {
        // set the view when the constant is not on a tree plate
        [valueEntry removeAllItems];
        // TO DO: Add possibilities depending on the sources connected to the Model Tool
        //NSArray* ma = [NSArray arrayWithObjects:@"name 1",@"name 2",nil];
        //[valueEntry addItemsWithObjectValues:ma];
        if (valueSource >= 0)
            valueSource = -1; // only allow selection of value explicitly for now

        Value* currentValue = nil;
        if ( [constantValues count] != 1 )
            {
            [constantValues removeAllObjects];
            currentValue = nil;
            }
        else 
            currentValue = [constantValues objectAtIndex:0];
        if ( currentValue == nil )
            [self initializeValue];

        NSString* valStr = [currentValue stringRepresentation];
        [valueEntry setStringValue:valStr];

        h = 190.0;
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

        h = 260.0;
        }
        
    // resize the control panel
    [self resizeWindowTo:h];
}

- (void)restoreWindowState {
        
    // restore instance variables
    [self setAppliesToNode:savedAppliesToNode];
    indexOfSelectedPartitionModel = savedIndexOfSelectedPartitionModel;
    savedValueSource              = valueSource;
    
    // restore the values for the constant
    [constantValues removeAllObjects];
    NSEnumerator* enumerator = [savedConstantValues objectEnumerator];
    id element;
    while ( (element = [enumerator nextObject]) )
        [constantValues addObject:element];
    [savedConstantValues removeAllObjects];
}

- (void)saveWindowState {
    
    // remember some of the instance variables
    savedAppliesToNode                 = appliesToNode;
    savedIndexOfSelectedPartitionModel = indexOfSelectedPartitionModel;
    savedValueSource                   = valueSource;
    
    // remember the values for the constant
    [savedConstantValues removeAllObjects];
    NSEnumerator* enumerator = [constantValues objectEnumerator];
    id element;
    while ( (element = [enumerator nextObject]) )
        [savedConstantValues addObject:[element clone]];
}

- (void)setValueFromString:(NSString*)str forValue:(Value*)v {
    
}

- (void)showControlPanel {
    
    NSPoint p = [self originForControlWindow:[self window]];
    [[self window] setFrameOrigin:p];
    [self refreshWindow];
    [self saveWindowState];
	[self showWindow:self];
	[[self window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[self window]];
}

@end
