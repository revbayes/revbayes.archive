
@ref:@home
@contents: no
@external: [RevBayes Homepage](http://revbayes.github.io/about.html)

RevBayes (Höhna et al., 2016) is a program for the Bayesian inference of phylogeny. It is the successor to the program MrBayes, written by John Huelsenbeck and Fredrik Ronquist (Huelsenbeck and Ronquist, 2001; Ronquist and Huelsenbeck, 2003). RevBayes was developed to overcome many of the shortcomings of MrBayes and what we perceive to be limitations in other phylogeny packages. The developer team expanded, too. Sebastian Höhna led the development effort. Other major developers include Fredrik Ronquist, John Huelsenbeck, Michael Landis, Tracy Heath, Bastien Boussau, Nicolas Lartillot, and Brian Moore. The program that resulted is much more general than originally envisioned; RevBayes can be used for other statistical problems besides estimating phylogeny. However, the developers optimized the program for inferring phylogeny. This makes sense as all of the developers are interested in the phylogeny problem.

The graphical user interface for the Mac version of RevBayes was developed by John Huelsenbeck, with significant input and help by Sebastian Höhna and Brian Moore. This help book explains how to use the Macintosh graphical user interface (GUI) version of the program.




@ref: Introduction
@title: Introduction to RevBayes
@contents: YES
@keywords: introduction, RevBayes
<a name="Introduction_Anchor"></a>

RevBayes was developed as a command-line_driven program. The user enters text at the user prompt, such as 

x &lt;- readCharacterData("/Users/Sally/Desktop/mydata.nex")

to carry out tasks in the program. \[This command would read in a nexus formatted data file (Maddison et al., 1997) and put it in a variable named 'x' which would find a home in the 'workspace' of the program.\] Indeed, for most computer operating systems, only the command-line version of RevBayes is available. The graphical user interface (GUI) version of the program was developed as a fun experiment: Could an intuitive and engaging interface be developed for a phylogeny program? This is not an easy task. Estimating phylogeny is a daunting statistical problem requiring a model that incorporates stochastic processes and complicated algorithms to calculate probabilities (either exactly or approximately). How can instantiating such a complicated model be made fun or, excepting that goal, at least made intuitive?

The usual way in which a GUI is made for the phylogeny problem is to expose options in the relevant window. For example, different options for the stochastic model that describes how characters change along the branches of the phylogenetic tree might be exposed on a single window. The various model options would then be selected using check boxes or text fields. Several very good GUI programs have been developed for the phylogeny problem that use this idea, such as PAUP\* (Swofford, 2003) and Mesquite (Maddison and Maddison, 2016). The problem with this approach is that as the models become more complicated, and require more 'switches' to active the options, the interface becomes more complicated and cluttered. This approach is challenging to the developer, too, because each new model that is implemented for the user means that new real estate must be found on a window to place the switches that activate the model. To many users, their first experience using a phylogeny program is about as simple as working with a lunar command module panel,

![CommandModule](CommandModule s432x207)

Fortunately, the consequence of throwing a wrong switch for a program such as PAUP\* is not as life threatening as throwing the wrong switch in a lunar command module! The goal in writing the GUI for RevBayes was to avoid an interface that was as cluttered as for a lunar command module, while still allowing for a plethora of models to be selected.

Another limitation of current phylogeny programs is the fact that it is difficult to replicate a phylogenetic analysis at some later date. How many times have you tried to reconstruct what you did in a phylogenetic analysis that was performed as recently as a month ago? Programs such as MrBayes or Beast (Drummond et al, 2012) produce many output files. Even remembering which data input file went with the output files can be daunting. Reconstructing the various settings and assumptions that went into the analysis can be still more challenging. In this program, we attempt to solve the analysis replication problem by maintaining an entire analysis as a logical whole. The entire analysis -- data, modifications of data, model settings, algorithm settings, and output -- are all saved together in one file which can be revived at any point in the future. We even allow you to save notes and images about the analysis in a virtual laboratory notebook.

RevBayes makes use of the metaphores of tools (to carry out specific tasks in a phylogenetic analysis) and of a graphical model (to describe the probability model used in the phylogenetic analysis; Höhna et al., 2014). In this help book, we describe the various tools that we implemented to expose the functionality of RevBayes. The most complicated tool is probably the Model Tool which allows the user to implement the modeling assumptions used in the phylogenetic analysis graphically.




@ref: Workbench
@title: The Analysis Workbench
@contents: yes
<a name="Workbench_Anchor"></a>

When you start up the RevBayes program, the first window you will come across is the Analysis Workbench:

![AnnotatedWorkbench](AnnotatedWorkbench s504x355)

The workbench has several spaces, which are numbered in the figure (above):

1. The main workbench space takes up the majority of the screen. This is where tools are dragged and linked together to carry out the phylogenetic analysis.

2. Below the workbench space is a tool box containing the various tools that can be dragged onto the workbench.

3. To the left of the workbench space, one can control which workbench is active. You can have several different analyses in one document.

4. On the left side of the tool bar (the top-left of the document page) you will find the Analysis Controller which allows you to add, copy, or remove an analysis. You also find a help button, too. Pressing the help button will bring up context-dependent information. In fact, pressing the help button on the Analysis Workbench will bring up the help you are currently reading. Throughout the program, you will find help buttons, each of which will lead you to relevant information for the tool you are using.

5. In the bottom-left corner of the document you will find a laboratory notebook. You can open this notebook to describe the analysis. Importantly, the notebook is unique to each workbench you have active in this Analysis Document. 

6. The 'Execute' button is at the top-right corner of the window. After you have set up the phylogenetic analysis, you push this button. Your phylogenetic analysis will start. Go enjoy a cup of coffee while your RevBayes analysis runs.

Each tool in the tool box performs a specific task (see <a href="TypicalTool.html#TypicalTool_Anchor">A Typical Tool</a>). You drag tools onto the workbench and set the information for each one. We will describe tools in more detail later in this help book. In general, however, each tool (1) performs a limited and specific task, (2) accepts information from other tools from an inlet, and (3) exports information to other tools through an outlet. To specify a phylogenetic analysis, one drags the appropriate tools onto the workbench, links the outlets to inlets, sets the details of each tool through its controller, and presses the 'Execute' button to start the analysis. 

There are two behaviors you should be aware of when hooking up tools: First, you can only hook up outlets to inlets that are of the same color. To hook up two tools, simply drag from the inlet of one tool to the outlet of another. (Alternatively, you can drag from an outlet to an inlet.) Second, you can change the position of an outlet/inlet on a tool by holding the option key down while dragging the outlet/inlet around the perimeter of the tool. 

A fully-spefied phylogenetic analysis might look like this:

![PopulatedWorkbench](PopulatedWorkbench s504x380)

The entire analysis can be saved and later reopened. Every detail of the phylogenetic analysis is remembered.

The laboratory notebook,
![LaboratoryNotebook](LaboratoryNotebook s504x591)
allows you to take notes about the analysis. One of the primary goals of the GUI was to make it easy to remember what was done in a phylogenetic analysis. The fact that the entire analysis can be resurrected from a saved file helps accomplish this goal. The laboratory notebook might be a good place to put notes to yourself or others about the motivations behind your choosing the particular modeling assumptions and settings used in the analysis.


@ref: TypicalTool
@title: A Typical Tool
@keywords: tools, inlets, outlets, contoller
@contents: yes
<a name="TypicalTool_Anchor"></a>

RevBayes uses the metaphor of a tool to carry out specific tasks in a phylogenetic analysis. We attempted to keep the functionality in each tool limited, so as to keep the design uncluttered and intuitive. The Read Data Tool is a typical tool that we will use to describe how tools work in RevBayes:

![TypicalTool](TypicalTool s170x150)

A tool has a large 'i' in the bottom-right corner. Pressing on this button will open the control window for that tool. The control window allows you to access and control the functionality of that tool. For example, the control window for the Read Data Tool allows you to open and read phylogenetic data file(s). (We will describe the Read Data Tool in more detail in another section of this help book: <a href="ReadDataTool.html#ReadDataTool_Anchor">Read Data Tool</a>.)

A tool has inlets and outlets. The Read Data Tool has two different outlets, which are color coded, but no inlets. (One is an outlet for unaligned data and the other is an outlet for data for which the fine scale homology of the characters has been established.) In this particular case, the numbers next to the outlets describe how many data matrices the tool contains of each type, aligned and unaligned. 

After reading in data, the Read Data Tool might look like this

![TypicalToolResolved](TypicalToolResolved s170x150)

Note that there is now a magnifying glass in the bottom-left corner of the tool. Clicking on the magnifying glass will bring up an inspector window. The details of the inspector window vary depending on the tool. For the Read Data Tool, the inspector window allows one to view the data matrices that have been read in by the tool. Also note that the information associated with each outlet has changed; this tool now contains 19 unaligned data matrices and one aligned data matrix.

One can delete a tool by selecting it and pressing the delete key. The tool will disappear with a satisfying poof.




@ref: ReadDataTool
@title: The Read Data Tool
@keywords: data, reading, input
@contents: yes
<a name="ReadDataTool_Anchor"></a>

The Read Data Tool,

![ReadDataTool](ReadDataTool s256x256)

reads in phylogenetic data. After having been read, the data is stored with the tool. RevBayes uses the Nexus Class Library (NCL), developed by Paul Lewis and Mark Holder, to read in data (Lewis, 2003). (The authors of RevBayes are grateful for this resource as writing code to parse files is not fun.)

The controller for the Read Data Tool allows one to select a file or directory:

![ReadDataToolController](ReadDataToolController s309x290)

If you select a file, then RevBayes will attempt to read only the selected file. If, however, you select a directory, RevBayes will recursively read all of the files in that directory. If, for example, you selected your home directory, RevBayes would scan every file you have stored in your account, attempting to read each. We added the ability to read all of the files in a directory to facilitate genomic analyses in which many hundreds or thousands of data files need to be read.

You can specify the data format to be read (Nexus, Phylip, or Fasta), or have RevBayes attempt to discover the data type automatically.

RevBayes also allows a blank matrix to be created. You simply specify the size of the matrix. This blank matrix can then serve as a place holder for simulations in which data is simulated into a container.

Once data is read, one can inspect the data matrix (or matrices) using the matrix inspector,

![DataMatrixClean](DataMatrixClean s504x336)

which shows the data in either aligned or unaligned form. You can get information on the details of each matrix by clicking on the 'Show Information' button,

![DataMatrixExpandedClean](DataMatrixExpandedClean s504x253)

which provides information on the size (number of taxa and characters) and type of data. 

The Read Data Tool stores a copy of the matrices that have been read. If other tools, such as the Alignment Tool or Matrix Filter Tool, later modify the data that have been read in, they do so on a copy of the data stored in this tool, not on the data as it was originally read. This means that you have a record of the data as it was originally read as well as any modifications made to that data.




@ref: AlignmentTool
@title: The Alignment Tool
@keywords: alignment, clustal, muscle, t-coffee, probcons
@contents: yes
<a name="AlignmentTool_Anchor"></a>

The Alignment Tool,
![AlignmentTool](AlignmentTool s256x256)
takes in unaligned data through an inlet and outputs aligned data. The authors of RevBayes did not write an alignment program. Rather, we use other programs to perform the alignments. RevBayes simply acts as an interface controlling these other alignment programs. Currently, RevBayes has packaged the following alignment programs:

1. Clustal (Larkin et al., 2007; [ClustalW](http://www.clustal.org/clustal2/ "ClustalW Website"))

2. Muscle (Edgar, 2004; [Muscle](http://www.drive5.com/muscle/ "Muscle Website"))

3. T-Coffee (Notredame et al., 2000; [T-Coffee](http://www.tcoffee.org "T-Coffee Website"))

4. Probcons (Do et al., 2005; [Probcons](http://probcons.stanford.edu "Probcons Website"))

We intend to include other alignment programs as we have time. 

Alignment can take a while, especially if many unaligned data matrices are aligned at once. RevBayes takes advantage of all of the computational resources available on your computer by running each alignment task on its own tread.




@ref: MatrixFilterTool
@title: The Matrix Filter Tool
@keywords: character, taxa, exclusion
@contents: yes
<a name="MatrixFilterTool_Anchor"></a>

The Matrix Filter Tool,

![MatrixFilterTool](MatrixFilterTool s256x256)

allows you to delete taxa or characters from the data matrix. The controller for this tool has several parts. First, at the top, you can control the number of outlets to other tools. Second, by selecting the 'Taxa' tab, you can delete or restore taxa from the analysis,

![MatrixFilterCtl1](MatrixFilterCtl1 s252x440)

Third, and finally, you can exclude or include characters in the analysis by selecting the 'Character' tax,

![MatrixFilterCtl2](MatrixFilterCtl2 s252x524)

Importantly, this tool contains a copy of the data found in the parent tool, with the modifications that were made through exclusion of taxa and characters.




@ref: ConcatentateTool
@title: The Alignment Concatenation Tool
@keywords: data concatenation
@contents: yes
<a name="ConcatenateTool_Anchor"></a>

The Alignment Concatenation Tool,

![ConcatenationTool](ConcatenationTool s256x256)

merges data matrices into a single data matrix (or a handful of matrices). You can control how the data are concatenated using the controller:

![ConcatenateCtl](ConcatenateCtl s432x290)

For one, you can choose to concatenate data by merging by data type, in which case all of the data of the same type are all merged into a single matrix, while data of others sorts are merged into their own matrices. Alternatively, you can merge data unconditionally, in which case all of the data are merged into a single data matrix of (potentially) mixed data type.

Often the data matrices to be combined do not have consistent taxon coverage. This tool allows you two options for dealing with this situation. You can merge the data by taking the intersection of the species membership in each data. This will result in a matrix that contains only those taxa found in all of the input data matrices. Alternatively, you can merge by taking the union of all of the taxa found in the input matrices. In this case, missing portions of the data are filled in by this tool with missing characters.

(Note: The caterpillar on the tool is a reference to the fact that concatenation of files on UNIX systems is performed using the 'cat' command. You have to remember that the authors of this program are computer geeks.)




@ref: CombinerTool
@title: The Matrix Combiner Tool
@keywords: matrix combiner
@contents: yes
<a name="CombinerTool_Anchor"></a>

The Matrix Combiner Tool,

![CombinerTool](CombinerTool s256x256)

consolidates matrices from two or more tools to a single tool, where the data can then be utilized by tools that require that all of the data be found in one tool. Using the controller for this tool,

![MatrixCombinerController](MatrixCombinerController s287x148)

you can control how many inlets take data from other tools.




@ref: DataEntryTool
@title: The Data Entry Tool
@keywords: data entry, morphology
@contents: yes
<a name="DataEntryTool_Anchor"></a>

The Data Entry Tool,

![DataEntryTool](DataEntryTool s256x256)

allows you to enter morphological characters by hand. You do this through the controller for the data entry tool

![DataEntryCtl](DataEntryCtl s432x288)

You can enter character states directly into the data cells. You can also add or delete taxa or characters using the pop-up controller in the top-right corner of the control window. The Data Entry Tool only allows for ten characters states, labelled 0, 1, ..., 9 as well as the missing character, '?.'




@ref: BootstrapTool
@title: The Bootstrap Tool
@keywords: bootstrap
@contents: yes
<a name="BootstrapTool_Anchor"></a>

The Bootstrap Tool,

![BootstrapTool](BootstrapTool s256x256)

makes one bootstrap replicate each time it is called. Bootstrapping is a method for generating the sampling distribution of a parameter when you do not have knowledge of the underlying probability distribution for the observations (Efron, 1979). The bootstrap was introduced to phylogenetics by Felsenstein (1985). The bootstrap method generates a new data character matrix by randomly sampling, with replacement, the columns of the original data matrix. Columns of the original matrix are randomly selected, with replacement, until a new (bootstrapped) data matrix of the same size as the original matrix is generated.

This tool has no controller. If you want to generate multiple boostrap data matrices, then you need to put this tool on a loop (see Tool Loop).




@ref: ParsimonyTool
@title: The Parsimony Tool
@keywords: parsimony, search, heuristic, branch and bound, exhaustive
@contents: yes
<a name="ParsimonyTool_Anchor"></a>

The Parsimony Tool,

![ParsimonyTool](ParsimonyTool s256x256)

searches for the best tree under the parsimony criterion. This tool uses the program PAUP\* (Swofford, 2003) to find the optimal tree; RevBayes simply exposes the functionality in the PAUP\* program to the user. The tool takes in as input a single aligned data matrix. The output is a set of trees that were found to be the most parsimonious (i.e., required the fewest number of character transformations). 

The controller for the Parsimony Tool allows you to select among three different tree-search options. The exhaustive search,

![ExhaustiveSearchCtl](ExhaustiveSearchCtl s439x165)

enumerates all possible trees, calculating the parsimony score for each. This search method is guaranteed to find the optimal tree. However, exhaustive searches are only feasible for a moderate number of taxa, say 11 or fewer, because the number of trees that must be visited becomes too prohibitive for larger numbers of taxa. Remember, the number of possible trees that could explain thre relationships of *N* taxa grows exponentially. The following table gives the number of unrooted phylogenetic trees for *N* taxa:

| *N*        | Number of Trees           |
| :----------|:-------------------------:|
| 4          | 3                         |
| 5          | 15                        |
| 6          | 105                       |
| 7          | 945                       |
| 8          | 10395                     |
| 9          | 135135                    |
| 10         | 2027025                   |
| 20         | 2.216 X 10<sup>20</sup>   |
| 50         | 2.838 X 10<sup>74</sup>   |
| 100        | 1.700 X 10<sup>182</sup>  |
| 500        | 1.012 X 10<sup>1277</sup> |
| 1000       | 1.927 X 10<sup>2860</sup> |

Because it is not feasible for RevBayes to perform exhaustive searches for a large number of taxa, the program will return an error if the number of taxa is greater than 12.

The branch-and-bound search,

![BandbCtl](BandbCtl s432x192)

implicitly enumerates all of the possible trees. Like the exhaustive search, the branch-and-bound search is guaranteed to find the optimal tree(s) under the parsimony criterion. However, it uses a trick -- that the parsimony score cannot become smaller when a branch is added to a tree -- to mathematically eliminate large numbers of trees as having an optimal score. This search method can work well for moderate numbers of taxa, say for fewer than about 30 taxa.

The heuristic search method, 

![HeuristicCtl](HeuristicCtl s432x452)

can be applied to phylogenetic problems of any size. The tradeoff for using the heuristic search method, however, is that there is no guarantee that the tree found by the method is optimal. 

Detailed descriptions of the search methods are described by Swofford et al. (1997). RevBayes uses the default values for PAUP\* as the defaults for the search options.




@ref: TreeSetTool
@title: The Tree Set Tool
@keywords: trees
@contents: yes
<a name="TreeSetTool_Anchor"></a>

The Tree Set Tool,

![TreeSetTool](TreeSetTool s256x256)

is a container for trees. It can be filled with trees in two ways. First, trees can be deposited in the Tree Set Tool by other tools, such as the Parsimony Tool or the MCMC Tool. In this way, the Tree Set Tool holds the results of a phylogenetic analysis. Second, trees can be read directly into the Tree Set Tool from a file containing trees in Newick format.

The controller for the Tree Set tool,

![TreeSetToolController](TreeSetToolController s349x163)

allows you to select the method by which trees are added to the tool. Once the Tree Set Tool contains trees, the trees can be visualized using the Tree Set Viewer,

![TreeSetViewer](TreeSetViewer s504x388)

which implements a coverflow-like interface for perusing trees. In the viewer, one can select a particular tree by either flipping directly to that tree in the viewer or by selecting a particular tree using the text window to the left of the 'Close' button. If you hold the mouse down when the cursor is over a tree in the viewer, a window will open showing that tree in more detail. This window can be dismissed by clicking anywhere on that window. The rooting and outgroup can be changed through the 'Trees' menu in the menu bar. This menu item appears only when the Tree Viewer is open.




@ref: LoopTool
@title: The Tool Loop
@keywords: repeating analyses
@contents: yes
<a name="LoopTool_Anchor"></a>

The Tool Loop,

![Loop](Loop s256x256)

allows you to repeat portions of the analysis. When you drag a loop onto the analysis workbench, the loop will instantiate itself as a square, which can be resized by grabbing the loop by the top-left or bottom-right corners:

![LoopEg1](LoopEg1 s288x281)

The controller for the Tool Loop,

![LoopCtl](LoopCtl s360x260)

allows you to control the index assigned to the loop and the number of times the loop is repeated. Note that each loop must have a unique index, something the controller enforces by not allowing you to select indices that have been used for other loops.

An example of a loop in use for an analysis could look like the following:

![LoopEg2](LoopEg2 s491x181)

This is a bootstrap analysis for the parsimony method. Tools that are associated with loops indicate this fact with a small loop icon in the top-left corner of the tool. This example analysis will perform 100 bootstrap analyses, depositing the trees found in the analyses in a Tree Set Tool (<a href="TreeSetTool.html#ReadDataTool_Anchor">Tree Set Tool</a>). You shoudl keep the following rules in mind when using the Tool Loop:

1. You can nest loops. However, loops must be completely contained by other loops.

2. Loops must contain contiguous sets of tools. You cannot have one tool residing outside of the loop, with the next tool in ine being back inside the loop.

3. Some tools, such as the Read Data, Concatenate, Matrix Filter, and Alignment Tools, cannot be placed on a loop.




@ref: References
@title: References
@keywords: references
@contents: yes

+ Do, CB, MSP Mahabhashyam, M Brudno, and S Batzoglou. 2005. PROBCONS: Probabilistic consistency-based multiple sequence alignment. *Genome Research* 15:330-340.

+ Drummond, AJ, MA Suchard, D Xie, and A Rambaut. 2012. Bayesian phylogenetics with BEAUti and the BEAST 1.7 *Molecular Biology And Evolution* 29:1969-1973.

+ Edgar, RC. 2004. MUSCLE: Multiple sequence alignment with high accuracy and high throughput *Nucleic Acids Research* 32:1792-1797.

+ Efron, B. 1979. Bootstrap methods: Another look at the jackknife. *The Annals of Statistics* 7:1-26.

+ Felsenstein, J. 1985. Confidence limits on phylogenies: An approach using the bootstrap. *Evolution* 39:783-791.

+ Huelsenbeck, JP, and F Ronquist. 2001. MrBayes: Bayesian inference of phylogeny. *Bioinformatics* 17:754-755.

+ Ronquist, F, and JP Huelsenbeck. 2003. MrBayes version 3.0: Bayesian phylogenetic inference under mixed models. *Bioinformatics* 19:1572-1574.

+ Höhna, S, TA Heath, B Boussau, MJ Landis, F Ronquist, and JP Huelsenbeck. 2014. Probabilistic graphical model representation in phylogenetics. *Systematic Biology* 63:753-771.

+ Höhna, S, MJ Landis, TA Heath, B Boussau, N Lartillot, BR Moore, JP Huelsenbeck, and F Ronquist. 2016. RevBayes: Bayesian phylogenetic inference using graphical models and an interactive model-specification language. *Systematic Biology* 65:726-736.

+ Larkin MA, G Blackshields, NP Brown, R Chenna, PA McGettigan, H McWilliam, F Valentin, IM Wallace, A Wilm, R Lopez, JD Thompson, TJ Gibson, and DG Higgins. 2007. *Bioinformatics* 23:2947-2948.

+ Lewis, PO. 2003. NCL: A C++ class library for interpreting data files in NEXUS format. *Bioinformatics* 19:2330-2331.

+ Maddison, WP and DR Maddison. 2016. Mesquite: a modular system for evolutionary analysis.  Version 3.10  [Mesquite Project](http://mesquiteproject.org "Mesquite Website")

+ Maddison, DR, DL Swofford, and WP Maddison. 1997. NEXUS: An extensible file format for systematic information. *Systematic Biology* 46:590-621.

+ Notredame, C, DG Higgins, and J Heringa. 2000. T-Coffee: A novel method for fast and accurate multiple sequence alignment. *Journal of Molecular Biology* 302:205-217.

+ Swofford, DL. 2003. *PAUP\*. Phylogenetic Analysis Using Parsimony (\*and Other Methods)*. Version 4. Sinauer Associates, Sunderland, Massachusetts.

+ Swofford, DL, GJ Olsen, PJ Waddell, and DM Hillis. 1996. Phylogenetic Inference. In DM Hillis, C Moritz, and BK Mable (Eds.) *Molecular Systematics*. Sinauer ssociates, Sunderland.


 
