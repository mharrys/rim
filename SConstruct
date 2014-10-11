env = Environment(
    CC='g++',
    CCFLAGS='-std=c++11 -pedantic -Wall -Wextra -Wno-switch -O3',
    CPPPATH='#/',
    LIBS=['GL', 'GLEW', 'SDL2', 'assimp']
)

source = Glob('src/*.cpp')

program = env.Program(target='bin/rim', source=source);
