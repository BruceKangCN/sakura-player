qt = import('qt6')

src = files([
  'main.cpp',
  'sakura-player.cpp',
])

moc_files = qt.compile_moc(
  sources: [
    'sakura-player.hpp',
  ],
  dependencies: [mpv_dep],
)
