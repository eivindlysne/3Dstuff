env = Environment(
    CC='clang',
    CCFLAGS=['-std=c11', '-g', '-Wall'],
    LINKFLAGS=['-lm', '-lGL', '-lGLEW', '-lSDL2']
)

CPPPATH = ['include', 'lib']

LIBS = ['tmx', 'xml2', 'jansson', 'z']
LIBPATH = ['/usr/lib', '/usr/local/lib', './lib/tmx']

src_files = Glob('src/*.c')
src_files.append('lib/bstrlib/bstrlib.c')

env.Program(
    'okapi',
    src_files,
    CPPPATH=CPPPATH,
    LIBS=LIBS,
    LIBPATH=LIBPATH,
)
