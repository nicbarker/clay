AT_COMPILE_TIME :: true;

RAYLIB_PATH :: "raylib";

DECLARATIONS_TO_OMIT :: string.[
    // These have custom declarations in module.jai
    "Vector2",
    "Vector3",
    "Vector4",
    "Quaternion",
    "Matrix",
    "PI",
];

DUPLICATE_DECLARATIONS :: string.[
    "rlTraceLogLevel",
    "rlPixelFormat",
    "rlTextureFilter",
    "rlBlendMode",
    "rlShaderLocationIndex",
    "rlShaderAttributeDataType",
    "rlShaderUniformDataType",
];

#if AT_COMPILE_TIME {
    #run,stallable {
        Compiler.set_build_options_dc(.{do_output=false});
        root_options := Compiler.get_build_options();
        args := root_options.compile_time_command_line;
        if !generate_bindings(args) {
            Compiler.compiler_set_workspace_status(.FAILED);
        }
    }
} else {
    #import "System";

    main :: () {
        set_working_directory(path_strip_filename(get_path_of_running_executable()));
        args := get_command_line_arguments();
        if !generate_bindings(args) {
            exit(1);
        }
    }
}

generate_bindings :: (args: [] string) -> bool
{
    compile := array_find(args, "-compile");
    compile_debug := array_find(args, "-debug");

    if compile {
        raylib_source_files: [..] string;
        array_add(*raylib_source_files,  tprint("%/src/rcore.c", RAYLIB_PATH));
        array_add(*raylib_source_files,  tprint("%/src/utils.c", RAYLIB_PATH));
        array_add(*raylib_source_files,  tprint("%/src/rshapes.c", RAYLIB_PATH));
        array_add(*raylib_source_files,  tprint("%/src/rtextures.c", RAYLIB_PATH));
        array_add(*raylib_source_files,  tprint("%/src/rtext.c", RAYLIB_PATH));
        array_add(*raylib_source_files,  tprint("%/src/rmodels.c", RAYLIB_PATH));
        array_add(*raylib_source_files,  tprint("%/src/raudio.c", RAYLIB_PATH));

        success := true;

        #if OS == .WINDOWS {
            File.make_directory_if_it_does_not_exist("windows");

            success &&= BuildCpp.build_cpp_static_lib(
                "windows/raylib", 
                ..raylib_source_files, 
                extra = .[
                    "/w",
                    "/DSUPPORT_MODULE_RSHAPES", 
                    "/DSUPPORT_MODULE_RTEXTURES",
                    "/DSUPPORT_MODULE_RTEXT",
                    "/DSUPPORT_MODULE_RMODELS",
                    "/DSUPPORT_MODULE_RAUDIO",
                    "/D_CRT_SECURE_NO_WARNINGS",
                    "/DPLATFORM_DESKTOP_RGFW",
                    "/DGRAPHICS_API_OPENGL_43",
                ]
            );
        } else {
            assert(false);
        }
    }

    output_filename: string;
    opts: Generator.Generate_Bindings_Options;
    {
        using opts;

        #if OS == .WINDOWS {
            array_add(*libpaths, "windows");
            output_filename = "windows.jai";
        } else {
            assert(false);
        }

        array_add(*libnames, "raylib");
        array_add(*include_paths, RAYLIB_PATH);
        array_add(*source_files, tprint("%/src/raylib.h", RAYLIB_PATH));
        // Can't be compiled on windows because of missing math.h ? Leaving it out for now since Jai has the functions we need
        // array_add(*source_files, tprint("%/src/raymath.h", RAYLIB_PATH));
        array_add(*source_files, tprint("%/src/rlgl.h", RAYLIB_PATH));
        array_add(
            *extra_clang_arguments, 
            "-c",
            "-DSUPPORT_MODULE_RSHAPES",
            "-DSUPPORT_MODULE_RTEXTURES",
            "-DSUPPORT_MODULE_RTEXT",
            "-DSUPPORT_MODULE_RMODELS",
            "-DSUPPORT_MODULE_RAUDIO",
            "-DPLATFORM_DESKTOP_RGFW",
            "-DGRAPHICS_API_OPENGL_43",
        );
        array_add(*strip_prefixes, "rl");

        // auto_detect_enum_prefixes = true;
        // log_stripped_declarations = false;
        // generate_compile_time_struct_checks = true;

        visitor   = raylib_visitor;
    }

    return Generator.generate_bindings(opts, output_filename);
}

raylib_visitor :: (decl: *Generator.Declaration, parent_decl: *Generator.Declaration) -> Generator.Declaration_Visit_Result
{
    if !parent_decl && array_find(DECLARATIONS_TO_OMIT, decl.name)
    {
        decl.decl_flags |= .OMIT_FROM_OUTPUT;
        return .STOP;
    }

    if decl.kind == .ENUM {
        en := cast(*Generator.Enum)decl;
        if String.contains(decl.name, "Flags") || decl.name == "Gesture" {
            en.flags |= .IS_ENUM_FLAGS;
            en.flags |= .VALUES_IN_HEX;
        }

        if array_find(DUPLICATE_DECLARATIONS, decl.name) {
            decl.decl_flags |= .OMIT_FROM_OUTPUT;
            return .STOP;
        }

        if en.type {
            if en.type.size == {
                case 1;
                    en.type = context.generator.type_def_u8;
                case 2;
                    en.type = context.generator.type_def_u16;
                case 4;
                    en.type = context.generator.type_def_u32;
                case 8;
                    en.type = context.generator.type_def_u64;
            }
        }
    }

    return .RECURSE;
}

using Basic :: #import "Basic";
Generator :: #import "Bindings_Generator";
Compiler :: #import "Compiler";
String :: #import "String";
BuildCpp :: #import "BuildCpp";
File :: #import "File";
WindowsResources :: #import "Windows_Resources";