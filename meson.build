project(
    'proyecto1',
    'cpp',
    default_options: ['cpp_std=c++20']
)

doxygen = find_program(
    'doxygen',
    required: true
)

run_target(
    'docs',
    command: [doxygen, meson.project_source_root() / 'Doxyfile']
)

executable(
    'proyecto1',
    'src/main.cpp'
)

gtest = subproject('gtest')
gtest_main_dep = gtest.get_variable('gtest_main_dep')

test_usuario = executable(
    'TestUsuario',
    'test/TestUsuario.cpp',
    'src/Usuario.cpp',
    dependencies: gtest_main_dep,
    include_directories: include_directories('src')
)

test_mensaje = executable(
    'TestMensaje',
    'test/TestMensaje.cpp',
    'src/Mensaje.cpp',
    dependencies: gtest_main_dep,
    include_directories: include_directories('src')
)

test_valida_mensaje = executable(
    'TestValidaMensaje',
    'test/TestValidaMensaje.cpp',
    'src/Mensaje.cpp',
    'src/ValidaMensaje.cpp',
    dependencies: gtest_main_dep,
    include_directories: include_directories('src')
)

test('Pruebas Usuario', test_usuario)
test('Pruebas Mensaje', test_mensaje)
test('Pruebas ValidaMensaje', test_valida_mensaje)	