project('RevBayes', ['cpp', 'c'],
	version: '1.0.9',
	default_options: [
	  'cpp_std=c++14'
	],
	meson_version: '>= 0.47',
	license: 'GPLv2')

cpp = meson.get_compiler('cpp')
cc = meson.get_compiler('c')

# Consider:
# -Wno-sign-compare
# -D_GLIBCXX_DEBUG

# Default compiler flags
#if (WIN32)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -static -msse -msse2 -msse3")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg -static")
#else ()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -msse -msse2 -msse3 -stdlib=libstdc++")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg")
#endif ()


add_project_arguments(['-O3','-msse','-msse2','-msse3'], language : 'cpp')
add_project_arguments(['-O3'], language : 'c')

# So many warnings!
add_project_arguments(['-Wall','-Wno-sign-compare','-Woverloaded-virtual','-Wstrict-aliasing','-Wno-unknown-pragmas'], language : 'cpp')

boost_modules = [ 'regex',
		  'program_options',
		  'thread','signals',
		  'system',
		  'filesystem',
		  'date_time',
		  'serialization']

# This doesn't build an internal boost from the internal copy, yet.
boost = dependency('boost', modules : boost_modules)


root_inc = include_directories([
'src/core/analysis',
'src/core/analysis/mcmc',
'src/core/analysis/mcmc/convergence',
'src/core/analysis/mcmc/output',
'src/core/analysis/stoppingRule',
'src/core/dag',
'src/core/datatypes',
'src/core/datatypes/math',
'src/core/datatypes/phylogenetics',
'src/core/datatypes/phylogenetics/character',
'src/core/datatypes/phylogenetics/characterdata',
'src/core/datatypes/phylogenetics/characterhistory',
'src/core/datatypes/phylogenetics/cladogeneticmatrix',
'src/core/datatypes/phylogenetics/geography',
'src/core/datatypes/phylogenetics/ratemap',
'src/core/datatypes/phylogenetics/ratematrix',
'src/core/datatypes/trees',
'src/core/distributions',
'src/core/distributions/math',
'src/core/distributions/phylogenetics',
'src/core/distributions/phylogenetics/characterhistory',
'src/core/distributions/phylogenetics/continuousCharacter',
'src/core/distributions/phylogenetics/substitution',
'src/core/distributions/phylogenetics/tree',
'src/core/distributions/phylogenetics/tree/birthdeath',
'src/core/distributions/phylogenetics/tree/coalescent',
'src/core/functions',
'src/core/functions/builtin',
'src/core/functions/distribution',
'src/core/functions/math',
'src/core/functions/phylogenetics',
'src/core/functions/phylogenetics/cladogenetic',
'src/core/functions/phylogenetics/ratemap',
'src/core/functions/phylogenetics/ratematrix',
'src/core/functions/phylogenetics/tree',
'src/core/functions/popgen',
'src/core/functions/vector',
'src/core/help',
'src/core/io',
'src/core/math',
'src/core/math/distributions',
'src/core/monitors',
'src/core/monitors/characterhistory',
'src/core/moves',
'src/core/moves/characterhistory',
'src/core/moves/compound',
'src/core/moves/mixture',
'src/core/moves/proposal',
'src/core/moves/proposal/matrix',
'src/core/moves/proposal/mixture',
'src/core/moves/proposal/scalar',
'src/core/moves/proposal/simplex',
'src/core/moves/proposal/tree',
'src/core/moves/proposal/vector',
'src/core/statistics/mixture',
'src/core/statistics/tree',
'src/core/utils',
'src/libs/lineedit',
'src/libs/linenoise',
'src/libs/ncl',
'src/revlanguage',
'src/revlanguage/analysis',
'src/revlanguage/analysis/mcmc',
'src/revlanguage/analysis/mcmc/output',
'src/revlanguage/dag',
'src/revlanguage/datatypes',
'src/revlanguage/datatypes/basic',
'src/revlanguage/datatypes/container',
'src/revlanguage/datatypes/math',
'src/revlanguage/datatypes/phylogenetics',
'src/revlanguage/datatypes/phylogenetics/character',
'src/revlanguage/datatypes/phylogenetics/characterdata',
'src/revlanguage/datatypes/phylogenetics/characterhistory',
'src/revlanguage/datatypes/phylogenetics/geography',
'src/revlanguage/datatypes/phylogenetics/trees',
'src/revlanguage/distributions',
'src/revlanguage/distributions/math',
'src/revlanguage/distributions/mixture',
'src/revlanguage/distributions/phylogenetics',
'src/revlanguage/distributions/phylogenetics/branchrate',
'src/revlanguage/distributions/phylogenetics/character',
'src/revlanguage/distributions/phylogenetics/tree',
'src/revlanguage/functions',
'src/revlanguage/functions/argument',
'src/revlanguage/functions/argumentrules',
'src/revlanguage/functions/basic',
'src/revlanguage/functions/internal',
'src/revlanguage/functions/io',
'src/revlanguage/functions/math',
'src/revlanguage/functions/math_helper',
'src/revlanguage/functions/phylogenetics',
'src/revlanguage/functions/phylogenetics/ratemap',
'src/revlanguage/functions/phylogenetics/ratematrix',
'src/revlanguage/functions/phylogenetics/tree',
'src/revlanguage/functions/popgen',
'src/revlanguage/monitors',
'src/revlanguage/monitors/characterhistory',
'src/revlanguage/moves',
'src/revlanguage/moves/integer',
'src/revlanguage/moves/matrix',
'src/revlanguage/moves/mixture',
'src/revlanguage/moves/scalar',
'src/revlanguage/moves/simplex',
'src/revlanguage/moves/tree',
'src/revlanguage/moves/vector',
'src/revlanguage/parser',
'src/revlanguage/ui',
'src/revlanguage/utils',
'src/revlanguage/workspace'])
			     
####
subdir('src')

############# executables #################
# basic rev-bayes binary

rb = executable('rb',
		rb_sources,
		include_directories: [root_inc],
		dependencies: [boost],
		install: true)


#test('run_integration_tests',
#     run_integration_tests_sh,
#     timeout: 3000,
#     workdir: join_paths(meson.source_root(), 'tests'),
#     args: [])

