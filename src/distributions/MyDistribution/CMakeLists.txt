# Create a library called "MyDistribution" which includes the source file "MyDistribution.cxx".
# The extension is already found. Any number of sources could be listed here.
add_definitions(-DPLUGIN_EXPORTS)
add_library (MyDistribution MyDistribution.cpp) 