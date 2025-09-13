project(
  'sakura-player',
  'cpp',
  version : '0.1.0',
  meson_version : '>= 1.3.0',
  default_options : [
    'warning_level=3',
    'cpp_std=c++23',
    'b_ndebug=if-release',
  ],
)

inc = include_directories('src')

qt6_dep = dependency('Qt6', modules: ['Core', 'Quick', 'QuickControls2'])
mpvqt_dep = dependency('MpvQt', fallback: ['MpvQt', 'mpvqt'])

subdir('src')

executable(
  'sakura-player',
  src,
  moc_files,
  include_directories: inc,
  dependencies : [qt6_dep, mpvqt_dep],
)
