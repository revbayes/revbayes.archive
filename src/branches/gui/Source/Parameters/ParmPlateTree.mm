#import "ParmPlateTree.h"
#import "ParmTree.h"
#import "RevBayes.h"
#import "ToolModel.h"



@implementation ParmPlateTree

- (void)encodeWithCoder:(NSCoder *)aCoder {
    
	[super encodeWithCoder:aCoder];
    // encode additional variables after super
}

- (NSString*)getEndingRangeForPlate {
    
    NSString* pStr;
    if ( [self isConnectedToTree] == NO )
        {
        pStr = [NSString stringWithString:@"P[?]"];
        }
    else
        {
        ParmTree* ot = [self getOrderingTree];
        
        // decide whether the ordering tree and tree plate are in a senible configuration
        int errNum;
        BOOL isConfigOk = [self isPlateConfigurationSensible:&errNum];
        if (isConfigOk == NO)
            {
            if (errNum == 1)
                pStr = [NSString stringWithString:@"P[?]"];
            else
                pStr = [NSString stringWithString:@"P[?]"];
            }
        else
            {
            int nt = [ot numTaxa];
            
            if ( [ot treeSizeSource] == NO_SOURCE )
                {
                pStr = [NSString stringWithString:@"P[?]"];
                }
            else if ( [ot treeSizeSource] == FIRST_MATRIX )
                {
                if (nt == -1)
                    pStr = [NSString stringWithString:@"P[?]"];
                else
                    pStr = [NSString stringWithFormat:@"P[%d]", nt];
                }
            else if ( [ot treeSizeSource] == ITH_MATRIX )
                {
                int numMatrices = [self numAttachedMatrices];
                BOOL numPlatesIteratingOverMatrices = 0;
                char idxChar;
                for (int i=0; i<[ot numAssignedPlates]; i++)
                    {
                    if ( [[ot getPlateIndexed:i] indexSource] == SRC_NUMBER_MATRICES )
                        {
                        idxChar = [[ot getPlateIndexed:i] indexLetter];
                        numPlatesIteratingOverMatrices++;
                        }
                    }
                if (numPlatesIteratingOverMatrices == 0)
                    pStr = [NSString stringWithString:@"P[?]"];
                else if (numPlatesIteratingOverMatrices == 1)
                    {
                    if (numMatrices == 0)
                        pStr = [NSString stringWithFormat:@"P[T[%c]]", idxChar];
                    else
                        pStr = [NSString stringWithFormat:@"P[T[%c]]", idxChar];
                    italicsRange = NSMakeRange(4, 1);
                    }
                else
                    {
                    pStr = [NSString stringWithString:@"P[?]"];
                    }
                }
            }
        }
	
    return pStr;
}

- (ParmTree*)getOrderingTree {
    
    if ( [self numParents] > 1 )
        return nil;
    else if ( [self numParents] == 1 )
        {
        Parm* p = [self getParentIndexed:0];
        if ( [p parmType] == PARM_TREE )
            return (ParmTree*)[self getParentIndexed:0];
        return nil;
        }
    return nil;
}

- (BOOL)isConnectedToTree {
    
    for (int i=0; i<[self numParents]; i++)
        {
        Parm* p = [self getParentIndexed:i];
        if ( [p parmType] == PARM_TREE )
            return YES;
        }
    return NO;
}

- (id)init {
    
    self = [self initWithScaleFactor:1.0 andTool:nil];
    return self;
}

- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t {
    
    if ( (self = [super initWithScaleFactor:sf andTool:t]) ) 
		{
        // initialize some variables
        parmType = PARM_TREE_PLATE;
        [self setImageWithSize:plateRect.size];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
    
    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        parmType = PARM_TREE_PLATE;
		[self initializeImage];
        [self setImageWithSize:itemSize];
        [self setImageWithSize:plateRect.size];
		}
	return self;
}

- (BOOL)isPlateConfigurationSensible:(int*)errNum {
    
    (*errNum) = 0;
    if ( [self isConnectedToTree] == NO )
        {
        // until the plate is ordered by a tree we consider the configuration sensible
        return YES;
        }
    else
        {
        ParmTree* ot = [self getOrderingTree];
        if ( [ot numAssignedPlates] == 0 )
            {
            // the ordering tree is not on a plate, which means that at worse we have a one-to-one relationship
            // between the tree and the tree plate
            return YES;
            }
        else
            {
            for (int i=0; i<[ot numAssignedPlates]; i++)
                {
                // the tree ordering the plate is sitting on a tree plate, which is quite silly
                ParmPlate* plt = [ot getPlateIndexed:i];
                if ( [plt parmType] == PARM_TREE_PLATE )
                    {
                    (*errNum) = 1;
                    return NO;
                    }
                // the tree ordering the tree plate is sitting on a different plate than the tree plate (not sensible) 
                if ( [self isAssignedToPlate:plt] == NO )
                    {
                    (*errNum) = 2;
                    return NO;
                    }
                }
            }
        }
    return YES;
}

- (void)refreshWindow {
        
	// set the index value
	char possIndices[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
	ToolModel* tm = [self myModel];
	char currentIndex = [self indexLetter];
	[indexSelector removeAllItems];
	for (int i=0; i<26; i++)
		{
		NSString* idxStr = [NSString stringWithFormat:@"%c", possIndices[i]];
		[indexSelector addItemWithTitle:idxStr];
		if ( [tm isPlateIndexAvailableForChar:possIndices[i]] == NO && currentIndex != possIndices[i] )
			[[indexSelector itemWithTitle:idxStr] setEnabled:NO];
		if ( currentIndex == possIndices[i] )
			[indexSelector selectItemWithTitle:idxStr];
		}
    
	// set the message
    if ( [self isConnectedToTree] == NO )
        {
        // the plate is not connected to a tree
        [msgLabelField setStringValue:@"This tree plate is not ordered by any tree. You should connect a tree parameter to this plate so the plate \"knows\" how many bipartitions to range over."];
        }
    else
        {
        ParmTree* ot = [self getOrderingTree];
        
        // decide whether the ordering tree and tree plate are in a senible configuration
        int errNum;
        BOOL isConfigOk = [self isPlateConfigurationSensible:&errNum];
        if (isConfigOk == NO)
            {
            if (errNum == 1)
                [msgLabelField setStringValue:@"The tree ordering the tree plate cannot be sitting on a tree plate."];
            else
                [msgLabelField setStringValue:@"The tree and the tree plate ordered by that tree cannot be sitting on different plates."];
            }
        else
            {
            int nt = [ot numTaxa];
            
            if ( [ot treeSizeSource] == NO_SOURCE )
                {
                NSString* msg = [NSString stringWithFormat:@"This tree plate is ordered by a tree named \"%@\". However, that tree does not have a source so its size is not determined.", [ot parmName]];
                [msgLabelField setStringValue:msg];
                }
            else if ( [ot treeSizeSource] == FIRST_MATRIX )
                {
                NSString* msg;
                if (nt == -1)
                    msg = [NSString stringWithFormat:@"This tree plate is ordered by a tree named \"%@\". This tree takes as its size the number of taxa in the first (or only) matrix attached to the model tool. Currently, there are no matrices attached to the tool.", [ot parmName]];
                else
                    msg = [NSString stringWithFormat:@"This tree plate is ordered by a tree named \"%@\". This tree takes as its size the number of taxa in the first (or only) matrix attached to the model tool. The current size of the ordering tree is %d taxa.", [ot parmName], nt];
                [msgLabelField setStringValue:msg];
                }
            else if ( [ot treeSizeSource] == ITH_MATRIX )
                {
                int numMatrices = [self numAttachedMatrices];
                BOOL numPlatesIteratingOverMatrices = 0;
                char idxChar;
                for (int i=0; i<[ot numAssignedPlates]; i++)
                    {
                    if ( [[ot getPlateIndexed:i] indexSource] == SRC_NUMBER_MATRICES )
                        {
                        numPlatesIteratingOverMatrices++;
                        idxChar = [[ot getPlateIndexed:i] indexLetter];
                        }
                    }
                NSString* msg;
                if (numPlatesIteratingOverMatrices == 0)
                    msg = [NSString stringWithFormat:@"This tree plate is ordered by a tree named \"%@\". This tree takes as its size the number of taxa in i-th matrix attached to the model tool. However, the tree is not sitting on a plate that iterates over matrices.", [ot parmName]];
                else if (numPlatesIteratingOverMatrices == 1)
                    {
                    if (numMatrices == 0)
                        msg = [NSString stringWithFormat:@"This tree plate is ordered by a tree named \"%@\". This tree takes as its size the number of taxa in %c-th matrix attached to the model tool. Currently, there are no matrices attached to the model tool.", [ot parmName], idxChar];
                    else
                        msg = [NSString stringWithFormat:@"This tree plate is ordered by a tree named \"%@\". This tree takes as its size the number of taxa in %c-th matrix attached to the model tool. Currently, there are %d matrices attached to the model tool.", [ot parmName], idxChar, numMatrices];
                    }
                else
                    {
                    msg = [NSString stringWithFormat:@"This tree plate is ordered by a tree named \"%@\". This tree takes as its size the number of taxa in i-th matrix attached to the model tool. However, the tree is sitting on more than one plate that iterates over matrices.", [ot parmName]];
                    }
                [msgLabelField setStringValue:msg];
                }
            }
        }
	
	// resize panel
	//[self resizeWindowTo:panelHeight];
}

- (NSString*)xibName {
    
	NSString* xn = [NSString stringWithString:@"ControlWindowTreePlate"];
	return xn;
}

@end
