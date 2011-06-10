#import "DistributionGui.h"
#import "DistributionList.h"
#import "Parameter.h"
#import "ParmRandomVariable.h"
#import "ParmPlateTree.h"
#import "ParmTree.h"
#import "PartitionModel.h"
#import "RevBayes.h"
#import "TaxonBipartition.h"
#import "ToolModel.h"
#import "ViewControllerRandomVariable.h"



@implementation ParmRandomVariable

@synthesize appliesToNode;
@synthesize isClamped;
@synthesize monitorValue;

- (IBAction)cancelAction:(id)sender {

    [self restoreWindowState];
	[self closeControlPanel];
}

- (IBAction)changeDistributionAction:(id)sender {

    NSString* userSelectionName = [distributionSelector titleOfSelectedItem];
    ParmPlateTree* treePlate    = [self assignedTreePlate];
    DistributionList* distList  = [myModel distributionList];
    
    if ( treePlate == nil )
        {
        // change the distribution for a parameter
        [probabilityModel removeAllObjects];
        DistributionGui* d = [distList newDistributionWithName:userSelectionName andParm:self];
        if (d != nil)
            [probabilityModel addObject:d];
        }
    else
        {
        // change the distribution for a parameter on a tree plate
        PartitionModel* pm = [probabilityModel objectAtIndex:indexOfSelectedPartitionModel];
        if ( pm == nil )
            return;
        DistributionGui* d = [distList newDistributionWithName:userSelectionName andParm:self];
        if (d != nil)
            [pm setDistributionGui:d];
        else
            [pm setDistribution:nil];
        }

    [self refreshWindow];
}

- (IBAction)changeIsClampedAction:(id)sender {

    if (isClamped == YES)
        isClamped = NO;
    else
        isClamped = YES;
    [self refreshWindow];
}

- (IBAction)changeClampledValueSelectionAction:(id)sender {

    [self refreshWindow];
}

- (IBAction)changeDimensionsAction:(id)sender {
    
}

- (IBAction)changeMonitorValueAction:(id)sender {
    
    if (monitorValue == YES)
        monitorValue = NO;
    else
        monitorValue = YES;
    [self refreshWindow];
}

- (IBAction)changeNodeOrBranchSelection:(id)sender {

    ParmPlateTree* treePlate = [self assignedTreePlate];
    if ( treePlate == nil )
        {
        }
    else
        {
        // change the distribution for a parameter on a tree plate
        PartitionModel* pm = [probabilityModel objectAtIndex:indexOfSelectedPartitionModel];
        if ( pm == nil )
            return;
        [pm setDistribution:nil];
        }

    [self refreshWindow];
}

- (IBAction)changeParameter1:(id)sender {
    
    ParmPlateTree* treePlate = [self assignedTreePlate];
    NSString* userSelectionName = [parameter1Selector titleOfSelectedItem];
    
    if (treePlate == nil)
        {
        if ( [probabilityModel count] == 1)
            {
            Parameter* currentParm = [[probabilityModel objectAtIndex:0] getParameterIndexed:0];
            Parm* p = [self getParentWithName:userSelectionName];
            [currentParm setRefParm:p];
            }
        }
    else
        {
        
        }
    [self refreshWindow];
}

- (IBAction)changeParameter2:(id)sender {
    
    ParmPlateTree* treePlate = [self assignedTreePlate];
    NSString* userSelectionName = [parameter2Selector titleOfSelectedItem];
    
    if (treePlate == nil)
        {
        if ( [probabilityModel count] == 1)
            {
            Parameter* currentParm = [[probabilityModel objectAtIndex:0] getParameterIndexed:1];
            Parm* p = [self getParentWithName:userSelectionName];
            [currentParm setRefParm:p];
            }
        }
    else
        {
        
        }
    [self refreshWindow];
}

- (IBAction)changeParameter3:(id)sender {
    
    ParmPlateTree* treePlate = [self assignedTreePlate];
    NSString* userSelectionName = [parameter3Selector titleOfSelectedItem];
    
    if (treePlate == nil)
        {
        if ( [probabilityModel count] == 1)
            {
            Parameter* currentParm = [[probabilityModel objectAtIndex:0] getParameterIndexed:2];
            Parm* p = [self getParentWithName:userSelectionName];
            [currentParm setRefParm:p];
            }
        }
    else
        {
        
        }
    [self refreshWindow];
}

- (IBAction)changeParameter4:(id)sender {
    
    ParmPlateTree* treePlate = [self assignedTreePlate];
    NSString* userSelectionName = [parameter4Selector titleOfSelectedItem];
    
    if (treePlate == nil)
        {
        if ( [probabilityModel count] == 1)
            {
            Parameter* currentParm = [[probabilityModel objectAtIndex:0] getParameterIndexed:3];
            Parm* p = [self getParentWithName:userSelectionName];
            [currentParm setRefParm:p];
            }
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

- (void)chooseView {

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
        distributionLabel     = [parameterView distributionLabel];
        distributionSelector  = [parameterView distributionSelector];
        parameter1Label       = [parameterView parameter1Label];
        parameter1Selector    = [parameterView parameter1Selector];
        parameter2Label       = [parameterView parameter2Label];
        parameter2Selector    = [parameterView parameter2Selector];
        parameter3Label       = [parameterView parameter3Label];
        parameter3Selector    = [parameterView parameter3Selector];
        parameter4Label       = [parameterView parameter4Label];
        parameter4Selector    = [parameterView parameter4Selector];
        clampedValueButton    = [parameterView clampedValueButton];
        clampedValueSelector  = [parameterView clampedValueSelector];
        }
    else if ( [self assignedTreePlate] != nil && [self isVector] == NO )
        {
        NSView* v = [treePlateView view];
        [box setContentView:v];
        nameField             = [treePlateView nameField];
        nameLabel             = [treePlateView nameLabel];
        partitionSelector     = [treePlateView partitionSelector];
        nodeBranchSelector    = [treePlateView nodeBranchSelector];
        distributionLabel     = [treePlateView distributionLabel];
        distributionSelector  = [treePlateView distributionSelector];
        parameter1Label       = [treePlateView parameter1Label];
        parameter1Selector    = [treePlateView parameter1Selector];
        parameter2Label       = [treePlateView parameter2Label];
        parameter2Selector    = [treePlateView parameter2Selector];
        parameter3Label       = [treePlateView parameter3Label];
        parameter3Selector    = [treePlateView parameter3Selector];
        parameter4Label       = [treePlateView parameter4Label];
        parameter4Selector    = [treePlateView parameter4Selector];
        clampedValueButton    = [treePlateView clampedValueButton];
        clampedValueSelector  = [treePlateView clampedValueSelector];
        }
    else if ( [self assignedTreePlate] == nil && [self isVector] == YES )
        {
        NSView* v = [parameterVectorView view];
        [box setContentView:v];
        nameField             = [parameterVectorView nameField];
        nameLabel             = [parameterVectorView nameLabel];
        distributionLabel     = [parameterVectorView distributionLabel];
        distributionSelector  = [parameterVectorView distributionSelector];
        parameter1Label       = [parameterVectorView parameter1Label];
        parameter1Selector    = [parameterVectorView parameter1Selector];
        parameter2Label       = [parameterVectorView parameter2Label];
        parameter2Selector    = [parameterVectorView parameter2Selector];
        parameter3Label       = [parameterVectorView parameter3Label];
        parameter3Selector    = [parameterVectorView parameter3Selector];
        parameter4Label       = [parameterVectorView parameter4Label];
        parameter4Selector    = [parameterVectorView parameter4Selector];
        clampedValueButton    = [parameterVectorView clampedValueButton];
        clampedValueSelector  = [parameterVectorView clampedValueSelector];
        monitorVariableButton = [parameterVectorView monitorVariableButton];
        vectorSizeStepper     = [parameterVectorView vectorSizeStepper];
        vectorSizeField       = [parameterVectorView vectorSizeField];
        vectorSizeLabel       = [parameterVectorView vectorSizeLabel];
        }
    else if ( [self assignedTreePlate] != nil && [self isVector] == YES )
        {
        NSView* v = [treePlateVectorView view];
        [box setContentView:v];
        nameField             = [treePlateVectorView nameField];
        nameLabel             = [treePlateVectorView nameLabel];
        partitionSelector     = [treePlateVectorView partitionSelector];
        nodeBranchSelector    = [treePlateVectorView nodeBranchSelector];
        distributionLabel     = [treePlateVectorView distributionLabel];
        distributionSelector  = [treePlateVectorView distributionSelector];
        parameter1Label       = [treePlateVectorView parameter1Label];
        parameter1Selector    = [treePlateVectorView parameter1Selector];
        parameter2Label       = [treePlateVectorView parameter2Label];
        parameter2Selector    = [treePlateVectorView parameter2Selector];
        parameter3Label       = [treePlateVectorView parameter3Label];
        parameter3Selector    = [treePlateVectorView parameter3Selector];
        parameter4Label       = [treePlateVectorView parameter4Label];
        parameter4Selector    = [treePlateVectorView parameter4Selector];
        clampedValueButton    = [treePlateVectorView clampedValueButton];
        clampedValueSelector  = [treePlateVectorView clampedValueSelector];
        monitorVariableButton = [treePlateVectorView monitorVariableButton];
        vectorSizeStepper     = [treePlateVectorView vectorSizeStepper];
        vectorSizeField       = [treePlateVectorView vectorSizeField];
        vectorSizeLabel       = [treePlateVectorView vectorSizeLabel];
        }
}

- (void)dealloc {

	[probabilityModel release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [super encodeWithCoder:aCoder];
	[aCoder encodeObject:probabilityModel           forKey:@"probabilityModel"];
    [aCoder encodeBool:isClamped                    forKey:@"isClamped"];
    [aCoder encodeBool:monitorValue                 forKey:@"monitorValue"];
    [aCoder encodeInt:indexOfSelectedPartitionModel forKey:@"indexOfSelectedPartitionModel"];
}

- (NSArray*)getPotentialClampValues {

    return nil;
}

- (BOOL)hasPartitionModels {

    if ( [probabilityModel count] == 0 )
        return NO;
    for (int i=0; i<[probabilityModel count]; i++)
        {
        if ( [[[probabilityModel objectAtIndex:i] className] isEqualToString:@"PartitionModel"] == NO )
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
		probabilityModel              = [aDecoder decodeObjectForKey:@"probabilityModel"];
        isClamped                     = [aDecoder decodeBoolForKey:@"isClamped"];
        monitorValue                  = [aDecoder decodeBoolForKey:@"monitorValue"];
        indexOfSelectedPartitionModel = [aDecoder decodeIntForKey:@"indexOfSelectedPartitionModel"];
		[probabilityModel retain];
		}
	return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {

    if ( (self = [super initWithScaleFactor:sf andTool:t]) ) 
		{
        probabilityModel = [[NSMutableArray alloc] init];
        indexOfSelectedPartitionModel = 0;
        [self setIsClamped:NO];
        [self setMonitorValue:YES];
        //[self initializeViewControllers];
		}
    return self;
}

- (void)initializeImage {
    
	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)initializePartitionModels {

    ParmPlateTree* tp = [self assignedTreePlate];
    if (tp == nil)
        return;
    ParmTree* t = [tp getOrderingTree];
    
    [probabilityModel removeAllObjects];
    
    if ( t != nil )
        {
        indexOfSelectedPartitionModel = 0;
        for (int i=0; i<[t numBipartitions]; i++)
            {
            TaxonBipartition* p = [t getBipartitionIndexed:i];
            PartitionModel* pm = [[PartitionModel alloc] initWithParm:self];
            [pm setPartition:p];
            [probabilityModel addObject:pm];
            }
        }
}

- (IBAction)okAction:(id)sender {

	[self closeControlPanel];
}

- (void)refreshWindow {
    
    // set the title of the control panel
    NSString* panelName = [NSString stringWithFormat:@"Control for \"%@\"", parmName];
    [[self window] setTitle:panelName];
    
    // get a pointer to objects we need
    ParmPlateTree* treePlate = [self assignedTreePlate];
    DistributionList* distList = [myModel distributionList];
    
    // display the view
    [self chooseView];
    
    // set the initial height of the control panel
    float h = 0.0;

    // set the distribution display
    if (treePlate == nil)
        {
        // set the distribution(s) when the parameter is not on a tree plate
        [partitionSelector setHidden:YES];
        [nodeBranchSelector setHidden:YES];
        
        [distributionSelector removeAllItems];
        [distributionSelector addItemWithTitle:@""];
        for (int i=0; i<[distList numDistributions]; i++)
            {
            DistributionGui* d = [distList getDistributionWithIndex:i];
            BOOL displayDist = NO;
            if ( ((unsigned)[self domain] & (unsigned)[d distributionDomain]) != 0 )
                displayDist = YES;
            if ( [d appliesExclusivelyToNodesOfUnrootedTrees]    == YES || 
                 [d appliesExclusivelyToBranchesOfUnrootedTrees] == YES ||
                 [d appliesExclusivelyToNodesOfRootedTrees]      == YES ||
                 [d appliesExclusivelyToBranchsOfRootedTrees]    == YES )
                displayDist = NO;

            if ( displayDist == YES )
                [distributionSelector addItemWithTitle:[d distributionName]];
            }
        
        Distribution* currentDist = nil;
        if ( [probabilityModel count] != 1 )
            currentDist = nil;
        else 
            currentDist = [probabilityModel objectAtIndex:0];
        if ( currentDist == nil )
            {
            // no distribution selected
            [distributionSelector selectItemWithTitle:@""];
            [parameter1Label setHidden:YES];
            [parameter2Label setHidden:YES];
            [parameter3Label setHidden:YES];
            [parameter4Label setHidden:YES];
            [parameter1Selector setHidden:YES];
            [parameter2Selector setHidden:YES];
            [parameter3Selector setHidden:YES];
            [parameter4Selector setHidden:YES];
            }
        else
            {
            // a distribution has been selected and we should show the parameter information 
            [distributionSelector selectItemWithTitle:[currentDist distributionName]];
            
            // show the information for the parameters
            [self showInformationForParameterIndexed:0 withDistribution:currentDist];
            [self showInformationForParameterIndexed:1 withDistribution:currentDist];
            [self showInformationForParameterIndexed:2 withDistribution:currentDist];
            [self showInformationForParameterIndexed:3 withDistribution:currentDist];
            }
            
        h = 450.0; //420.0;
        h -= (4 - [currentDist numParameters]) * 45.0;
        }
    else
        {
        // set the distribution(s) when the parameter _is_ on a tree plate
        [partitionSelector setHidden:NO];
        [nodeBranchSelector setHidden:NO];
        BOOL partitionModelsExist = [self hasPartitionModels];
        if (partitionModelsExist == NO)
            [self initializePartitionModels];
            
        int nParms = 0;
            
        ParmTree* t = [treePlate getOrderingTree];
        if ( t == nil )
            {
            // we don't have a tree ordering the plate, so we cannot display the bipartitions for model selection
            [partitionSelector addItemWithTitle:@"No tree orders tree plate"];
            [nodeBranchSelector setEnabled:NO forSegment:0];
            [nodeBranchSelector setEnabled:NO forSegment:1];
            [distributionSelector removeAllItems];
            [distributionSelector addItemWithTitle:@""];
            [parameter1Label setHidden:YES];
            [parameter2Label setHidden:YES];
            [parameter3Label setHidden:YES];
            [parameter4Label setHidden:YES];
            [parameter1Selector setHidden:YES];
            [parameter2Selector setHidden:YES];
            [parameter3Selector setHidden:YES];
            [parameter4Selector setHidden:YES];
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

            // set the distribution for the selected partition
            PartitionModel* pm = [probabilityModel objectAtIndex:indexOfSelectedPartitionModel];
            Distribution* pd = [pm distribution];
            
            [distributionSelector removeAllItems];
            [distributionSelector addItemWithTitle:@""];
            for (int i=0; i<[distList numDistributions]; i++)
                {
                DistributionGui* d = [distList getDistributionWithIndex:i];
                BOOL displayDist = NO;
                if ( ((unsigned)[self domain] & (unsigned)[d distributionDomain]) != 0 )
                    displayDist = YES;
                    
                if ( [d appliesExclusivelyToNodesOfUnrootedTrees]    == YES || 
                     [d appliesExclusivelyToBranchesOfUnrootedTrees] == YES ||
                     [d appliesExclusivelyToNodesOfRootedTrees]      == YES ||
                     [d appliesExclusivelyToBranchsOfRootedTrees]    == YES )
                    {
                    int nodeOrBranch = (int)[nodeBranchSelector selectedSegment];
                    if ( [d appliesExclusivelyToNodesOfUnrootedTrees] == YES )
                        if (nodeOrBranch == BRANCH || [t rootednessType] == ROOTED_TREE )
                            displayDist = NO;
                    if ( [d appliesExclusivelyToBranchesOfUnrootedTrees] == YES )
                        if (nodeOrBranch == NODE || [t rootednessType] == ROOTED_TREE )
                            displayDist = NO;
                    if ( [d appliesExclusivelyToNodesOfRootedTrees] == YES )
                        if (nodeOrBranch == BRANCH || [t rootednessType] == UNROOTED_TREE )
                            displayDist = NO;
                    if ( [d appliesExclusivelyToBranchsOfRootedTrees] == YES )
                        if (nodeOrBranch == NODE || [t rootednessType] == UNROOTED_TREE )
                            displayDist = NO;
                    }

                if ( displayDist == YES )
                    [distributionSelector addItemWithTitle:[d distributionName]];
                }
            if (pd == nil)
                {
                [distributionSelector selectItemWithTitle:@""];
                [parameter1Label setHidden:YES];
                [parameter2Label setHidden:YES];
                [parameter3Label setHidden:YES];
                [parameter4Label setHidden:YES];
                [parameter1Selector setHidden:YES];
                [parameter2Selector setHidden:YES];
                [parameter3Selector setHidden:YES];
                [parameter4Selector setHidden:YES];
                }
            else
                {
                [distributionSelector selectItemWithTitle:[pd distributionName]];
                [self showInformationForParameterIndexed:0 withDistribution:pd];
                [self showInformationForParameterIndexed:1 withDistribution:pd];
                [self showInformationForParameterIndexed:2 withDistribution:pd];
                [self showInformationForParameterIndexed:3 withDistribution:pd];
                nParms = [pd numParameters];
                }            
            }
            
        h = 530.0;
        h -= (4 - nParms) * 45.0;
        }
        
    // set the dimensions information, if the parameter is a vector
    if ( [self isVector] == YES )
        {
        h += 46.0;
        }
    
    // set the clamping 
    if ( [self isClamped] == YES )
        {
        [clampedValueSelector setHidden:NO];
        [clampedValueSelector removeAllItems];
        // TO DO: add items to display as potential clampees here
        [clampedValueButton setTitle:@"Clamp to:"];
        }
    else
        {
        [clampedValueSelector setHidden:YES];
        [clampedValueSelector removeAllItems];
        [clampedValueButton setTitle:@"Clamp random variable?"];
        }
        
    // resize the control panel
    [self resizeWindowTo:h];
}

- (void)restoreWindowState {

}

- (void)saveWindowState {

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

- (void)showInformationForParameterIndexed:(int)idx withDistribution:(Distribution*)cd {

    if (cd == nil)
        return;
        
    NSTextField* label = nil;
    NSPopUpButton* button = nil;
    if (idx == 0)
        {
        label = parameter1Label;
        button = parameter1Selector;
        }
    else if (idx == 1)
        {
        label = parameter2Label;
        button = parameter2Selector;
        }
    else if (idx == 2)
        {
        label = parameter3Label;
        button = parameter3Selector;
        }
    else if (idx == 3)
        {
        label = parameter4Label;
        button = parameter4Selector;
        }
    else
        {
        return;
        }
    
    // set the distribution display
    ParmPlateTree* treePlate = [self assignedTreePlate];
    if (treePlate == nil)
        {
        // the parameter is not on a tree plate
        if ( [cd numParameters] > idx )
            {
            Parameter* p = [cd getParameterIndexed:idx];
            [label setHidden:NO];
            [button setHidden:NO];
            [label setStringValue:[p parameterName]];
            [button removeAllItems];
            [button addItemWithTitle:@""];
            for (int i=0; i<[self numParents]; i++)
                {
                if ( [[self getParentIndexed:i] domain] == [p type] )
                    [button addItemWithTitle:[[self getParentIndexed:i] parmName]];
                }
            if ([p refParm] != nil)
                [button selectItemWithTitle:[[p refParm] parmName]];
            else
                [button selectItemWithTitle:@""];
            }
        else
            {
            [label setHidden:YES];
            [button setHidden:YES];
            }
        }
    else
        {
        // the parameter is on a tree plate
        ParmTree* t = [treePlate getOrderingTree];
        if ( t == nil )
            {
            [label setHidden:YES];
            [button setHidden:YES];
            return;
            }
        else
            {
            if ( [cd numParameters] > idx )
                {
                Parameter* p = [cd getParameterIndexed:idx];
                [label setHidden:NO];
                [button setHidden:NO];
                [nodeBranchSelector setEnabled:YES forSegment:0];
                [nodeBranchSelector setEnabled:YES forSegment:1];
                [label setStringValue:[p parameterName]];
                [button removeAllItems];
                [button addItemWithTitle:@""];
                for (int i=0; i<[self numParents]; i++)
                    {
                    if ( [[self getParentIndexed:i] domain] == [p type] )
                        [button addItemWithTitle:[[self getParentIndexed:i] parmName]];
                    }
                if ([p refParm] != nil)
                    [button selectItemWithTitle:[[p refParm] parmName]];
                else
                    [button selectItemWithTitle:@""];
                }
            else
                {
                [label setHidden:YES];
                [button setHidden:YES];
                }
            }
        }
}

@end
