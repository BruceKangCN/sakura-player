# Dear ImGui @ v1.92.3-docking

imgui_inc = include_directories('imgui', 'imgui' / 'backends')
imgui_src = files([
  'imgui' / 'imgui.cpp',
  'imgui' / 'imgui_draw.cpp',
  'imgui' / 'imgui_tables.cpp',
  'imgui' / 'imgui_widgets.cpp',
  'imgui' / 'backends' / 'imgui_impl_opengl3.cpp',
  'imgui' / 'backends' / 'imgui_impl_glfw.cpp',
])

# ImGuiFileDialog @ v0.6.7

igfd_inc = include_directories('ImGuiFileDialog')
igfd_src = files([
  'ImGuiFileDialog' / 'ImGuiFileDialog.cpp',
])

# Glad @ 2.0.8, generated using:
# - Language: C/C++
# - API:
#   - gl: Version 4.6, Core

glad_inc = include_directories('glad' / 'include')
glad_src = files([
  'glad' / 'src' / 'gl.c',
])
