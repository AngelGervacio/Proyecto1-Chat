project(
    'proyecto1',
    'cpp',
    default_options: ['cpp_std=c++20']
)

doxygen = find_program(
    'doxygen',
    required: true
)

gtest = subproject('gtest')
gtest_main_dep = gtest.get_variable('gtest_main_dep')

json_dep = dependency(
    'nlohmann_json',
    fallback: ['nlohmann_json', 'nlohmann_json_dep']
)

executable(
    'proyecto1',
    'src/main.cpp',
    dependencies: json_dep
)

run_target(
    'docs',
    command: [doxygen, meson.project_source_root() / 'Doxyfile']
)

test_usuario = executable(
    'TestUsuario',
    'test/TestUsuario.cpp',
    'src/Usuario.cpp',
    dependencies: [gtest_main_dep, json_dep],
    include_directories: include_directories('src')
)

test_mensaje = executable(
    'TestMensaje',
    'test/TestMensaje.cpp',
    'src/Mensaje.cpp',
    dependencies: [gtest_main_dep, json_dep],
    include_directories: include_directories('src')
)

test_valida_mensaje = executable(
    'TestValidaMensaje',
    'test/TestValidaMensaje.cpp',
    'src/Mensaje.cpp',
    'src/ValidaMensaje.cpp',
    dependencies: [gtest_main_dep, json_dep],
    include_directories: include_directories('src')
)

test_genera_mensaje = executable(
    'TestGeneraMensaje',
    'test/TestGeneraMensaje.cpp',
    'src/Mensaje.cpp',
    'src/GeneraMensaje.cpp',
    dependencies: [gtest_main_dep, json_dep],
    include_directories: include_directories('src')
)

test_genera_json = executable(
    'TestGeneraJSON',
    'test/TestGeneraJSON.cpp',
    'src/Mensaje.cpp',
    'src/GeneraJSON.cpp',
    dependencies: [gtest_main_dep, json_dep],
    include_directories: include_directories('src')
)

test('Pruebas Usuario', test_usuario)
test('Pruebas Mensaje', test_mensaje)
test('Pruebas ValidaMensaje', test_valida_mensaje)
test('Pruebas GeneraMensaje', test_genera_mensaje)
test('Pruebas GeneraJSON', test_genera_json)