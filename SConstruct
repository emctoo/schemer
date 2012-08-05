env = Environment(tools = ['mingw'])
print env['PLATFORM']
env.MergeFlags([
	'-g',
	'-std=c99',
	])
env.Program(target = 'word', source = ['lexer.c', 'utils.c'])
# env.SharedLibrary('lexer', source = ['lexer.c'])
