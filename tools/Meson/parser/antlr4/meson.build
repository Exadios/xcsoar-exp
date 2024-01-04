project('Antlr4 Meson Build', [ 'cpp', 'java' ])

add_global_arguments(['-I/usr/include/antlr4-runtime', '-I./out'],
                     language : 'cpp')

antlr4 = find_program('antlr4')
p = custom_target('antlr4cpp',
                  input : 'Make.g4',
                  output : [ 'MakeBaseListener.cpp',
                             'MakeBaseListener.h',
                             'MakeLexer.cpp',
                             'MakeLexer.h',
                             'MakeListener.cpp',
                             'MakeListener.h',
                             'MakeParser.cpp',
                             'MakeParser.h',
                             'MakeBaseVisitor.cpp',
                             'MakeBaseVisitor.h',
                             'MakeVisitor.cpp',
                             'MakeVisitor.h',],
                  install_dir : '@BUILD_ROOT@',
                  command : [ antlr4, 
                                '-Dlanguage=Cpp',
                                '-listener',
                                '-visitor',
                                '-o', '@OUTDIR@', 
                                '@INPUT@',
                  # This lunacy is required because, for reason best known to
                  # itself, antlr4 ignores the '-o @OUTDIR@' argument and 
                  # write its output to the same directory as the g4 file is
                  # in.
                                '&&', 'mv', '../MakeBaseListener.cpp', '.',
                                '&&', 'mv', '../MakeBaseListener.h', '.',
                                '&&', 'mv', '../MakeListener.cpp', '.',
                                '&&', 'mv', '../MakeListener.h', '.',
                                '&&', 'mv', '../MakeLexer.cpp', '.',
                                '&&', 'mv', '../MakeLexer.h', '.',
                                '&&', 'mv', '../MakeParser.cpp', '.',
                                '&&', 'mv', '../MakeParser.h', '.',
                                '&&', 'mv', '../MakeBaseVisitor.cpp', '.',
                                '&&', 'mv', '../MakeBaseVisitor.h', '.',
                                '&&', 'mv', '../MakeVisitor.cpp', '.',
                                '&&', 'mv', '../MakeVisitor.h', '.',])

j = custom_target('antlr4java',
                  input : 'Make.g4',
                  output : [ 'MakeBaseListener.java',
                             'MakeLexer.java',
                             'MakeListener.java',
                             'MakeParser.java'],
                  command : [ antlr4, '-o', '@OUTDIR@', '@INPUT@' ])

executable('Maker', [p, 'MakeMain.cpp', 'MakeHooks.cpp'],
           link_args : '-lantlr4-runtime')
#executable('MakerJava', j,
#           compile_args : [ '-cp' ,
#                            '.:/usr/share/java/antlr4.jar:/usr/share/java/antlr4-runtime.jar' 
#                          ])

javac = find_program('javac')
generator(javac,
          arguments : [ '-cp',
                        '.:/usr/share/java/antlr4.jar:/usr/share/java/antlr4-runtime.jar',
                        '@INPUT@'
                      ],
          output : '@BASENAME@.class'
                      )

