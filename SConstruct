import os
env = Environment(
	# ENV = {'PATH' : 'C:/Program\ Files/Haskell\ Platform/2012.2.0.0/bin' },
	# tools = ['mingw', 'ghc'], 
	# toolpath = ['.', "C:/Program\ Files/Haskell\ Platform/2012.2.0.0/bin"]
	tools = ['mingw']
	)
# env.Detect(['ghc'])
print env['PLATFORM']
env.Append(CPPPATH = '.')
env.MergeFlags([
	'-g',
	'-std=c99',
	])
env.Program(target = 'word', source = ['lexer.c', 'utils.c'])
env.Program(
		target = 'cool', 
		source = ['main.c', 'lexer_utils.c', 'code_file.c', 'lexer_identifier.c']
		)
# env.SharedLibrary('lexer', source = ['lexer.c'])

'''
env.HaskellProgram(
	target = 'schemer',
	source = ['schemer.hs'],
	HSPACKAGE = ["process", "mtl", "parsec"],
	)

ghc = File('C:\\Program Files\\Haskell Platform\\2012.2.0.0\\bin\\ghc')
env.Command(
	'schemer', 
	'schemer.hs', 
	"C:\\Program Files\\Haskell Platform\\2012.2.0.0\\bin\\ghc $SOURCE -o $TARGET"
	)

haskell_bin = Dir('C:\\Program Files\\Haskell Platform\\2012.2.0.0\\bin\\')
added_path = ['C:\\Program Files\\Haskell Platform\\2012.2.0.0\\bin\\', ]
env = Environment(ENV = {'PATH' : added_path})
print env['ENV']
path = env['ENV']['PATH']
for item in path:
	print item
'''