## Mandatory Coding Practices

  * Use no tabs; insert four (4) spaces instead. 
  * For curly braces, use either one of the two styles indicated below according to your preferences: 
    
```cpp   
if ( true ) {
    # some code
}
        
if ( true )
{
    # some code
}
```
    

  * To ensure that other developers can safely use a RevBayes class, make sure there is either 
    * no destructor, no copy constructor, and no assignment operator 
    * OR just an empty virtual destructor, no copy constructor and no assignment operator 
    * OR all three (destructor, copy constructor, and assignment operator). 
  * A class with virtual functions must have a virtual destructor. 
  * An assignment operator needs to have return type the same as the class it belongs to and it needs to check for self assignment. 
  * The clone function must always be present and have the same return type as the class it belongs to, so-called co-variant return type. 
  * Static code should go inside functions unless you are absolutely certain that the order of execution is irrelevant. The execution order of static code outside of functions is implementation dependent and cannot be trusted. 
  * Make sure your code compiles without warnings in your IDE before committing it to the repository. 
  * In headers, include project files first, system files then and finally forward declarations. One empty line between each set, and all headers listed in alphabetic order. 
  * An implementation file should have the implementation of the constructors and destructors first, followed by the implementation of all other functions in alphabetic order. For a template class, the same arrangement of code is used in the header file, following the class declaration. 

## Recommended Coding Practices

  * In a derived abstract class, duplicate abstract functions. It is a useful reminder, and there is no overhead. 
  * Avoid duplicate or redundant code to facilitate future changes, debugging and long-term maintenance of the code. For instance: 
    * Do not duplicate code in the copy constructor if you can delegate the call to the assignment operator. 
    * Do not add getters and setters unless needed (avoid redundant code). 
    * Avoid using overloaded operators that are not needed because standard implicit type conversion handles the necessary conversions. 
    * Do not include more header files than necessary; use forward class declarations instead when possible. 
  * Use general variable names that copy well, like "x" or "obj" instead of context-dependent abbreviations. For instance, instead of using an argument called "mc" in the copy constructor for MyClass, use "x", so that the argument is still valid if you want to use the MyClass code as a template for AnotherClass. 
  * Always spell out ` void ` in formal argument specifications. 
  * Never leave code that is commented out without explaining why it is commented out. 

## Documentation practices

  * Each header file and each source file should be equipped with the standard project header. See core/analysis/Model.h and core/analysis/Model.cpp for examples. 
  * In each header file (class declaration), it is mandatory to describe the class and what it does using doxygen comments. All functions and member variables should also have a brief 'post comment' (using the doxygen ` //!` tag) in the class declaration. The comment should fit within a tool-tip popup. See core/analysis/Model.h for examples. 
  * If you feel that more documentation is needed for a function, add proper doxygen comments to the implementation file just before the implementation of the function. See core/analysis/Model.cpp for examples. These comments should not have a ` \brief ` item, which would duplicate the post comment in the header. Except for these cases, do not use doxygen comments in the implementation file. Inside functions, use standard C++ comments where appropriate to explain what the code does. 

## Needing further clarification

  * Always derive from Cloneable but check if already another base class derives from Cloneable (in which case deriving from Cloneable is not necessary). 
