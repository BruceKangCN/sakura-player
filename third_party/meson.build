# Dear ImGui @ v1.92.3-docking

imgui_inc = include_directories('imgui', 'imgui/backends')

imgui_src = files([
  'imgui/imgui.cpp',
  'imgui/imgui_draw.cpp',
  'imgui/imgui_tables.cpp',
  'imgui/imgui_widgets.cpp',
  'imgui/backends/imgui_impl_opengl3.cpp',
  'imgui/backends/imgui_impl_glfw.cpp',
])

# Glad @ 0.1.36, generated using:
# - Language: C/C++
# - Specification: OpenGL
# - API:
#   - gl: Version 4.6
#   - gles1: Version 1.0
#   - gles2: Version 3.2
#   - glsc2: Version 2.0
# - Profile: Compatibility
# - Extensions: []
# - [x] Generate a loader
# - [ ] Omit KHR (due to recent changes to the specification, this may not work anymore)
# - [x] Local Files

glad_inc = include_directories('glad')
glad_src = files([
  'glad/glad.c',
])
