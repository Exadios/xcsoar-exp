project('tutorial', [ 'cpp', 'java' ])

add_global_arguments('-I/usr/include/antlr4-runtime', language : 'cpp')

p = custom_target('antlr4cpp',
                  input : 'Make.g4',
                  output : [ 'MakeBaseListener.cpp',
                             'MakeBaseListener.h',
                             'MakeLexer.cpp',
                             'MakeLexer.h',
                             'MakeListener.cpp',
                             'MakeListener.h',
                             'MakeParser.cpp',
                             'MakeParser.h'],
                  command : [ 'antlr4', '-Dlanguage=Cpp', '@INPUT@' ])
j = custom_target('antlr4java',
                  input : 'Make.g4',
                  output : [ 'MakeBaseListener.java',
                             'MakeLexer.java',
                             'MakeListener.java',
                             'MakeParser.java'],
                  command : [ 'antlr4', '@INPUT@' ])
executable('Maker', ['MakeMain.cpp', 'MakeHooks.cpp', p],
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

