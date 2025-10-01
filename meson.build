project(
  'sakura-player',
  'cpp',
  version: '0.1.0',
  meson_version: '>= 1.3.0',
  default_options: [
    'warning_level=3',
    'cpp_std=c++23',
    'b_ndebug=if-release',
  ],
)

inc = include_directories('src')

# TODO: Glad, RapidJson and a native file dialog library may be also needed
mpv_dep = dependency('mpv')
glfw_dep = dependency('glfw3')

subdir('src')
subdir('third_party')

win_subsystem = get_option('debug') ? 'console' : 'windows'

executable(
  'sakura-player',
  src,
  imgui_src,
  include_directories: [inc, imgui_inc],
  dependencies: [mpv_dep, glfw_dep],
  win_subsystem: win_subsystem,
)
