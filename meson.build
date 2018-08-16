project('RevBayes', ['cpp', 'c'],
	version: '1.0.9',
	default_options: [
	  'cpp_std=c++14'
	],
	meson_version: '>= 0.47',
	license: 'GPLv2')

cpp = meson.get_compiler('cpp')
cc = meson.get_compiler('c')


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

####
subdir('src')
subdir('tests')

############# executables #################

rb = executable('rb',
		rb_sources,
		include_directories: [root_inc],
		dependencies: [boost],
		install: true)


