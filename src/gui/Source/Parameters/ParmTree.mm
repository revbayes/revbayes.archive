#import "ParmPlate.h"
#import "ParmTree.h"
#import "RevBayes.h"
#import "TaxonBipartition.h"
#import "ToolModel.h"
#import "WindowControllerPartitionSelector.h"



@implementation ParmTree

@synthesize rootednessType;
@synthesize treeSizeSource;

@synthesize originalRootednessType;

- (IBAction)addBipartitionAction:(id)sender {
    
    // decide whether we let the user specify new bipartitions
    int numMatrices = [self numAttachedMatrices];
    int minNum = 100000000;
    int maxNum = 0;
    for (int i=0; i<numMatrices; i++)
        {
        int n = [self numTaxaForMatrix:i];
        if (n > maxNum)
            maxNum = n;
        if (n < minNum)
            minNum = n;
        }
    
    if ( numMatrices == 0 || (numMatrices == 1 && treeSizeSource == NO_SOURCE) || (maxNum-minNum) != 0 )
        {
        
        }
    else 
        {
        WindowControllerPartitionSelector* psel = [[WindowControllerPartitionSelector alloc] initWithParm:self andTool:myModel];
        NSPoint p = [self originForControlWindow:[psel window]];
        [[psel window] setFrameOrigin:p];
        [psel showControlPanel];
        
        }
}

- (void)addUserBipartition:(TaxonBipartition*)bp {
    
    [bipartitions addObject:bp];
    [self refreshWindow];
}

- (IBAction)changeDistributionAction:(id)sender {

    if ( rootednessType == UNROOTED_TREE )
        {
        if ( [[distributionSelector titleOfSelectedItem] isEqualToString:@"Uniform on Topologies"] == YES )
            unrootedDistType = DISTRIBUTION_UNIFORM_UNROOTED_TOPOLOGY;
        else if ( [[distributionSelector titleOfSelectedItem] isEqualToString:@"Constant"] == YES )
            unrootedDistType = DISTRIBUTION_CONSTANT;
        }
    else
        {
        if ( [[distributionSelector titleOfSelectedItem] isEqualToString:@"Uniform on Topologies"] == YES )
            rootedDistType = DISTRIBUTION_UNIFORM_ROOTED_TOPOLOGY;
        else if ( [[distributionSelector titleOfSelectedItem] isEqualToString:@"Uniform on Labeled Histories"] == YES )
            rootedDistType = DISTRIBUTION_UNIFORM_HISTORIES;
        else if ( [[distributionSelector titleOfSelectedItem] isEqualToString:@"Constant"] == YES )
            rootedDistType = DISTRIBUTION_CONSTANT;
        }
    [self refreshWindow];
}

- (IBAction)changeRootednessTypeAction:(id)sender {

    if ( [[rootednessTypeSelector titleOfSelectedItem] isEqualToString:@"Unrooted"] == YES )
        [self setRootednessType:UNROOTED_TREE];
    else 
        [self setRootednessType:ROOTED_TREE];
    //[self changeDistributionAction:self];
    [self refreshWindow];
}

- (IBAction)changeTreeSizeSourceAction:(id)sender {
    
    if ( [[sourceSelector titleOfSelectedItem] isEqualToString:@""] == YES )
        treeSizeSource = NO_SOURCE;
    else if ( [[sourceSelector titleOfSelectedItem] isEqualToString:@"Number of Taxa in First/Only Attached Matrix"] == YES )
        treeSizeSource = FIRST_MATRIX;
    else 
        treeSizeSource = ITH_MATRIX;
    NSLog(@"changed treeSizeSource = %d", treeSizeSource);
    [self refreshWindow];
}

- (BOOL)doesTreeOrderPlate {

    for (int i=0; i<[self numChildren]; i++)
        {
        Parm* c = [self getChildIndexed:i];
        if ( [c parmType] == PARM_TREE_PLATE )
            return YES;
        }
    return NO;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[super encodeWithCoder:aCoder];

    [aCoder encodeInt:rootednessType forKey:@"rootednessType"];
    [aCoder encodeInt:treeSizeSource forKey:@"treeSizeSource"];
    [aCoder encodeObject:bipartitions forKey:@"bipartitions"];
    [aCoder encodeInt:unrootedDistType forKey:@"unrootedDistType"];
    [aCoder encodeInt:rootedDistType forKey:@"rootedDistType"];
	
}

- (NSString*)getNextDefaultPartitionName {
    
    NSString* ds;
    int n = 1;
    do 
        {
        ds = [NSString stringWithFormat:@"User Bipartition %d", n++];
        } while( [self isBipartitionNameTaken:ds] == YES );
    return ds;
}

- (TaxonBipartition*)getBipartitionIndexed:(int)idx {

    return [bipartitions objectAtIndex:idx];
}

- (id)init {

    self = [self initWithScaleFactor:1.0 andTool:nil];
    return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {

    if ( (self = [super initWithScaleFactor:sf andTool:t]) ) 
		{
		// initialize the parameter image
		[self initializeImage];
        [self setImageWithSize:itemSize];

        // initialize some variables
        parmType         = PARM_TREE;
        treeSizeSource   = NO_SOURCE;
        
        // initialize variables
        [self setRootednessType:UNROOTED_TREE];
        unrootedDistType = DISTRIBUTION_UNIFORM_UNROOTED_TOPOLOGY;
        rootedDistType   = DISTRIBUTION_UNIFORM_HISTORIES;

        // initialize the taxon bipartitions
        bipartitions = [[NSMutableArray alloc] init];
        TaxonBipartition* p1 = [[TaxonBipartition alloc] init];
        TaxonBipartition* p2 = [[TaxonBipartition alloc] init];
        TaxonBipartition* p3 = [[TaxonBipartition alloc] init];
        TaxonBipartition* p4 = [[TaxonBipartition alloc] init];
        [p1 setPartitionName:@"Any Bipartition"];
        [p2 setPartitionName:@"Tip Bipartitions"];
        [p3 setPartitionName:@"Interior Bipartitions"];
        [p4 setPartitionName:@"Root Bipartition"];
        [p1 setPartitionType:ANY_PART];
        [p2 setPartitionType:TIP_PARTS];
        [p3 setPartitionType:INT_PARTS];
        [p4 setPartitionType:ROOT_PART];
        [bipartitions addObject:p1];
        [bipartitions addObject:p2];
        [bipartitions addObject:p3];
        [bipartitions addObject:p4];
        }
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the parameter image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // initialize variables
        rootednessType   = [aDecoder decodeIntForKey:@"rootednessType"];
        treeSizeSource   = [aDecoder decodeIntForKey:@"treeSizeSource"];
        bipartitions     = [aDecoder decodeObjectForKey:@"bipartitions"];
        unrootedDistType = [aDecoder decodeIntForKey:@"unrootedDistType"];
        rootedDistType   = [aDecoder decodeIntForKey:@"rootedDistType"];
		}
	return self;
}

- (void)initializeImage {

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (int)numTaxa {
    
    int numMatrices = [self numAttachedMatrices];
    int minNum = 100000000;
    int maxNum = 0;
    for (int i=0; i<numMatrices; i++)
        {
        int n = [self numTaxaForMatrix:i];
        if (n > maxNum)
            maxNum = n;
        if (n < minNum)
            minNum = n;
        }
    if (numMatrices == 0)
        return -1;
    else if (numMatrices == 1)
        {
        if (treeSizeSource == FIRST_MATRIX || treeSizeSource == ITH_MATRIX)
            return maxNum;
        else
            return -1;
        }
    else 
        {
        if (treeSizeSource == NO_SOURCE)
            return -1;
        else if (treeSizeSource == FIRST_MATRIX)
            return [self numTaxaForMatrix:0];
        else 
            {
            if (maxNum - minNum == 0)
                return maxNum;
            else 
                return -1;
            }
        }
    return -1;
}

- (BOOL)isBipartitionNameTaken:(NSString*)nme {
    
	NSEnumerator* partEnumerator = [bipartitions objectEnumerator];
	id element;
	while ( (element = [partEnumerator nextObject]) )
        {
        if ( [[element partitionName] isEqualToString:nme] == YES )
            return YES;
        }
    return NO;
}

- (int)numBipartitions {

    return (int)[bipartitions count];
}

- (void)refreshWindow {
	
    // set the rootedness of the tree
    [rootednessTypeSelector removeAllItems];
    [rootednessTypeSelector addItemWithTitle:@"Unrooted"];
    [rootednessTypeSelector addItemWithTitle:@"Rooted"];
    if ( [self rootednessType] == UNROOTED_TREE )
        [rootednessTypeSelector selectItemWithTitle:@"Unrooted"];
    else 
        [rootednessTypeSelector selectItemWithTitle:@"Rooted"];
        
    // set the tree-size source, if necessary
    [sourceSelector removeAllItems];
    [sourceSelector addItemWithTitle:@""];
    [sourceSelector addItemWithTitle:@"Number of Taxa in First/Only Attached Matrix"];
    int numPlatesIteratingOverMatrices = 0;
    for (int i=0; i<[self numAssignedPlates]; i++)
        {
        ParmPlate* plt = [self getPlateIndexed:i];
        if ( [plt indexSource] == SRC_NUMBER_MATRICES )
            numPlatesIteratingOverMatrices++;
        }
    if (numPlatesIteratingOverMatrices == 1)
        {
        for (int i=0; i<[self numAssignedPlates]; i++)
            {
            ParmPlate* plt = [self getPlateIndexed:i];
            if ( [plt indexSource] == SRC_NUMBER_MATRICES )
                {
                char ic = [plt indexLetter];
                NSString* msg = [NSString stringWithFormat:@"Number of Taxa in Matrix indexed %c", ic];
                [sourceSelector addItemWithTitle:msg];
                }
            }
        }
    else 
        {
        if (treeSizeSource == ITH_MATRIX)
            treeSizeSource = NO_SOURCE;
        }
    
    int src = [self treeSizeSource];
    if (src == NO_SOURCE)
        [sourceSelector selectItemAtIndex:NO_SOURCE]; // 0
    else if (src == FIRST_MATRIX)
        [sourceSelector selectItemAtIndex:FIRST_MATRIX]; // 1
    else 
        {
		if (numPlatesIteratingOverMatrices == 1)
			[sourceSelector selectItemAtIndex:ITH_MATRIX];
		else 
			[sourceSelector selectItemAtIndex:NO_SOURCE];
        }
    
    // set the size-source information field
    int numMatrices = [self numAttachedMatrices];
    int minNum = 100000000;
    int maxNum = 0;
    for (int i=0; i<numMatrices; i++)
        {
        int n = [self numTaxaForMatrix:i];
        if (n > maxNum)
            maxNum = n;
        if (n < minNum)
            minNum = n;
        }
    if (numMatrices == 0)
        [treeSizeField setStringValue:@"?"];
    else if (numMatrices == 1)
        {
        if (treeSizeSource == FIRST_MATRIX || treeSizeSource == ITH_MATRIX)
            {
            NSString* msg = [NSString stringWithFormat:@"%d", maxNum];
            [treeSizeField setStringValue:msg];
            }
        else
            [treeSizeField setStringValue:@"?"];
        }
    else 
        {
        if (treeSizeSource == NO_SOURCE)
            [treeSizeField setStringValue:@"?"];
        else if (treeSizeSource == FIRST_MATRIX)
            {
            NSString* msg = [NSString stringWithFormat:@"%d", [self numTaxaForMatrix:0]];
            [treeSizeField setStringValue:msg];
            }
        else 
            {
            NSString* msg;
            if (maxNum - minNum == 0)
                msg = [NSString stringWithFormat:@"%d", maxNum];
            else 
                msg = [NSString stringWithFormat:@"%d - %d", minNum, maxNum];
            [treeSizeField setStringValue:msg];
            }
        }
       
    // fill in the exposed taxon bipartitions
    [partitionsList removeAllItems];
    [partitionsList addItemWithTitle:@"Exposed Bipartitions"];
	NSEnumerator* partEnumerator = [bipartitions objectEnumerator];
	id element;
	while ( (element = [partEnumerator nextObject]) )
        {
        if ( [element partitionType] == USER_PART && !(numMatrices == 0 || (numMatrices == 1 && treeSizeSource == NO_SOURCE) || (maxNum-minNum) != 0) )
            [partitionsList addItemWithTitle:[element partitionName]];
        else if ( [element partitionType] != USER_PART )
            [partitionsList addItemWithTitle:[element partitionName]];
        }
    
    // show or hide the "+" button for adding bipartitions
    if ( numMatrices == 0 || (numMatrices == 1 && treeSizeSource == NO_SOURCE) || (maxNum-minNum) != 0 )
        [partitionAddButton setHidden:YES];
    else 
        [partitionAddButton setHidden:NO];
    
    // set the possible distributions
	[distributionSelector removeAllItems];
    if ( [[rootednessTypeSelector titleOfSelectedItem] isEqualToString:@"Unrooted"] == YES )
        {
        [distributionSelector addItemWithTitle:@"Uniform on Topologies"];
        }
    else 
        {
        [distributionSelector addItemWithTitle:@"Uniform on Topologies"];
        [distributionSelector addItemWithTitle:@"Uniform on Labeled Histories"];
        }
    [distributionSelector addItemWithTitle:@"Constant"];
    if ( rootednessType == UNROOTED_TREE )
        {
        if (unrootedDistType == DISTRIBUTION_UNIFORM_UNROOTED_TOPOLOGY)
            [distributionSelector selectItemWithTitle:@"Uniform on Topologies"];
        else if (unrootedDistType == DISTRIBUTION_CONSTANT)
            [distributionSelector selectItemWithTitle:@"Constant"];
        }
    else
        {
        if (rootedDistType == DISTRIBUTION_UNIFORM_ROOTED_TOPOLOGY)
            [distributionSelector selectItemWithTitle:@"Uniform on Topologies"];
        if (rootedDistType == DISTRIBUTION_UNIFORM_HISTORIES)
            [distributionSelector selectItemWithTitle:@"Uniform on Labeled Histories"];
        else if (rootedDistType == DISTRIBUTION_CONSTANT)
            [distributionSelector selectItemWithTitle:@"Constant"];
        }

    // show/hide the connector for the constant tree topology
    if ( ([[rootednessTypeSelector titleOfSelectedItem] isEqualToString:@"Unrooted"] == YES && unrootedDistType == DISTRIBUTION_CONSTANT) ||
         ([[rootednessTypeSelector titleOfSelectedItem] isEqualToString:@"Rooted"] == YES   && rootedDistType   == DISTRIBUTION_CONSTANT))
        {
        [fixedTopologySelector setHidden:NO];
        [fixedValueField setHidden:NO];
        [fixedTopologySelector removeAllItems];
        [fixedTopologySelector addItemWithTitle:@""];
        // try to find topologies that we can connect to this outlet
        }
    else
        {
        [fixedTopologySelector setHidden:YES];
        [fixedValueField setHidden:YES];
        }
  
    // set the clamp
    if ( ([[rootednessTypeSelector titleOfSelectedItem] isEqualToString:@"Unrooted"] == YES && unrootedDistType == DISTRIBUTION_CONSTANT) ||
        ([[rootednessTypeSelector titleOfSelectedItem] isEqualToString:@"Rooted"] == YES   && rootedDistType   == DISTRIBUTION_CONSTANT))
        {
        [clampedValueButton setHidden:YES];
        [clampedValueSelector setHidden:YES];
        [clampDivider setHidden:YES];
        }
    else
        {
        [clampedValueButton setHidden:NO];
        [clampDivider setHidden:NO];
        if ( [self isClamped] == YES )
            {
            [clampedValueButton setTitle:@"Clamp to:"];
            [clampedValueSelector setHidden:NO];
            [clampedValueSelector removeAllItems];
            [clampedValueSelector addItemWithTitle:@""];
            }
        else 
            {
            [clampedValueButton setTitle:@"Clamp value?"];
            [clampedValueSelector setHidden:YES];
            }
        NSArray* clampees = [self getPotentialClampValues];
        // iterate over potential clamp values and display them
        
        }
    
    // change the height of the window
    float h = 435.0;
    [self resizeWindowTo:h];

}

- (void)rememberSettings {

    [self setOriginalRootednessType:[self rootednessType]];
}

- (void)setPossibleDistributions {

}

- (NSString*)xibName {

	NSString* xn = @"ControlWindowTree";
	return xn;
}

@end
