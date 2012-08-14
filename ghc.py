from os.path import abspath, join
from SCons.Builder import Builder
from SCons.Scanner import Scanner
from SCons.Script  import *
import re

impdecl_regexp = re.compile(r"[\s]*import[\s]*(qualified)?[\s]*([\w\.]*).*")
foreign_decl_regexp = re.compile(r"[\s]*foreign[\s]*(export|import)[\s]*ccall(.*\"[\s]*wrapper[\s]*\")?.*::.*")

def find_foreign_declarations_that_require_stubs(node):
        foreigns = foreign_decl_regexp.findall(node.get_contents())
        for foreign in foreigns:
                if foreign[0] == "export":
                        return True
                if foreign[1] != "":
                        return True
        return False

def ghc_emitter(target, source, env):
        src_name = str(source[0])
        hi_name = src_name.replace(source[0].suffix, ".hi")
        env.SideEffect(hi_name, target)
        env.Clean(target, hi_name)
        stubs = []
        if find_foreign_declarations_that_require_stubs(source[0]):
                stub_name = str(source[0]).replace(source[0].suffix, "_stub")
                stubs.append(stub_name + ".o")
                env.Clean(target, stub_name + ".c")
                env.SideEffect(stub_name + ".h", target)
                env.Clean(target, stub_name + ".h")
        return (target + stubs, source)

def ghc_path_function(env, dir, targets, sources):
        if dir.path != ".":
                env.AppendUnique(HSSEARCHPATH = [dir.path])
        return tuple([dir.path] + env["HSSEARCHPATH"])

def ghc_scanner_function(node, env, path):
        from os.path import dirname, exists
        imports = impdecl_regexp.findall(node.get_contents())
        modules = map(lambda (qualified, module) : module, imports)

        interfaces = []
        for module in modules:
                module = module.replace(".", "/")
                interface = module + ".hi"
                hs_file   = module + ".hs"
                lhs_file  = module + ".lhs"
                for dir in path:
                        if exists(join(dir, hs_file)) or exists(join(dir, lhs_file)):
                                interfaces.append(interface)
                                break
        return interfaces

def exists(env):
        return WhereIs("ghc")

def generate(env):
        def _ghc_searchpath_opts(paths):
                return "".join(" -i" + path for path in paths)
        env["_ghc_searchpath_opts"] = _ghc_searchpath_opts
        def _ghc_package_opts(packages):
                return "".join(" -package " + package for package in packages)
        env["_ghc_package_opts"] = _ghc_package_opts

        if env['PLATFORM'] == 'win32':
            path = 'C:\\Program Files\\Haskell Platform\\2012.2.0.0\\bin\\'
        elif env['PLATFORM'] == 'posix':
            path = '' 
        env["HSC"] = File( path + 'ghc')
        env["HSCFLAGS"] = []
        env["HSLINKFLAGS"] = []
        env["HSSEARCHPATH"] = []
        env["HSPACKAGES"] = []
        env["_HSPACKAGE_OPTS"] = "${_ghc_package_opts(HSPACKAGES)}"
        env["_HSSEARCHPATH_OPTS"] = "${_ghc_searchpath_opts(HSSEARCHPATH)}"

        ghc_scanner = Scanner(
                function = ghc_scanner_function,
                skeys = [".hs", ".lhs"],
                path_function = ghc_path_function
                )

        ghc_c_compiler = Builder(
                action = "$HSC $HSCFLAGS -c -o $TARGET $SOURCE",
                src_suffix = [ ".c" ],
                suffix = ".o",
                single_source = True
                )

        ghc_compiler = Builder(
                action = "$HSC $HSCFLAGS $_HSSEARCHPATH_OPTS -c -o $TARGET $SOURCE",
                src_suffix = [ ".hs", ".lhs" ],
                suffix = ".o",
                single_source = True,
                emitter = ghc_emitter,
                source_scanner = ghc_scanner
                )

        ghc_linker = Builder(
                action = "$HSC $HSLINKFLAGS $_HSPACKAGE_OPTS -o $TARGET $SOURCES",
                src_suffix = ".o",
                suffix = "$PROGSUFFIX",
                src_builder = [ ghc_compiler, ghc_c_compiler ]
                )

        env.Append( BUILDERS = { "HaskellProgram" : ghc_linker, "HaskellObject" : ghc_compiler } )