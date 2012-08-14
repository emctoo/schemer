import os
env = Environment(
	# ENV = {'PATH' : 'C:/Program\ Files/Haskell\ Platform/2012.2.0.0/bin' },
	tools = ['mingw', 'ghc'], 
	toolpath = ['.', "C:/Program\ Files/Haskell\ Platform/2012.2.0.0/bin"]
	)
# env.Detect(['ghc'])
print env['PLATFORM']
env.MergeFlags([
	'-g',
	'-std=c99',
	])
env.Program(target = 'word', source = ['lexer.c', 'utils.c'])
# env.SharedLibrary('lexer', source = ['lexer.c'])
env.HaskellProgram(
	target = 'schemer',
	source = ['schemer.hs'],
	HSPACKAGE = ["mtl", "parsec"],
	)