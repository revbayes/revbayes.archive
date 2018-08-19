project('RevBayes', ['cpp', 'c'],
        version: '1.0.9',
        default_options: [ 'cpp_std=c++14' ],
        meson_version: '>= 0.47',
        license: 'GPLv2')

cpp = meson.get_compiler('cpp')
cc = meson.get_compiler('c')


add_project_arguments(['-O3','-msse','-msse2','-msse3'], language : 'cpp')
add_project_arguments(['-O3'], language : 'c')

# So many warnings!
add_project_arguments(['-Wall','-Wno-sign-compare','-Woverloaded-virtual',
                       '-Wstrict-aliasing','-Wno-unknown-pragmas'],
                      language : 'cpp')

# Handle file too large error when cross-compiling to windows
if host_machine.system() == 'windows'
  add_project_arguments(['-Wa,-mbig-obj'], language : 'cpp')
endif

boost_modules = [ 'regex',
                  'program_options',
                  'thread','signals',
                  'system',
                  'filesystem',
                  'date_time',
                  'serialization']

# This doesn't build an internal boost from the internal copy, yet.
boost = dependency('boost', modules : boost_modules)

rb_name = 'rb'
if get_option('mpi')
  rb_name += '-mpi'
  mpi = dependency('mpi', language='cpp')
else
  mpi = dependency('', required: false)
endif

if get_option('jupyter')
  rb_name += '-jupyter'
  add_project_arguments(['-DRB_XCODE'], language : 'cpp')
endif

####
subdir('src')
subdir('tests')

############# executables #################
core = static_library('rb-core',
                      core_sources,
                      include_directories: [src_inc])

revlanguage = static_library('rb-revlanguage',
                             revlanguage_sources,
                             include_directories: [src_inc])

libs = static_library('rb-lib',
                      libs_sources,
                      include_directories: [src_inc])

############# executables #################

rb = executable(rb_name,
                ['src/revlanguage/main.cpp'],
                link_with: [core, revlanguage, libs],
                include_directories: [src_inc],
                dependencies: [boost, mpi],
                install: true)

if get_option('studio')
  gtk2 = dependency('gtk+-2.0')
  cmd = static_library('rb-cmd',
                       cmd_sources,
                       include_directories: [src_inc],
                       dependencies: [gtk2])

  #  Add cmd/main.cpp cmd/RbGTKGui.cpp
  #  includes: RbGTRGui.ui
  revstudio = executable('RevStudio',
                         ['src/cmd/main.cpp'],
                         link_with: [core, revlanguage, libs, cmd],
                         include_directories: [src_inc],
                         dependencies: [boost, mpi, gtk2],
                         install: true)

endif


