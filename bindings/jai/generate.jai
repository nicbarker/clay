AT_COMPILE_TIME :: true;

SOURCE_PATH :: "source";

DECLARATIONS_TO_OMIT :: string.[
    "Clay_Vector2",
    "Clay_Color",
];

#if AT_COMPILE_TIME {
    #run,stallable {
        Compiler.set_build_options_dc(.{do_output=false});
        options := Compiler.get_build_options();
        args := options.compile_time_command_line;
        if !generate_bindings(args, options.minimum_os_version) {
            Compiler.compiler_set_workspace_status(.FAILED);
        }
    }
} else {
    #import "System";

    main :: () {
        set_working_directory(path_strip_filename(get_path_of_running_executable()));
        if !generate_bindings(get_command_line_arguments(), #run get_build_options().minimum_os_version) {
            exit(1);
        }
    }
}

generate_bindings :: (args: [] string, minimum_os_version: type_of(Compiler.Build_Options.minimum_os_version)) -> bool {
    compile := array_find(args, "-compile");
    compile_debug := array_find(args, "-debug");

    could_copy := FileUtils.copy_file("../../clay.h", "source/clay.h");
    if !could_copy then return false;

    if compile {
        source_file := tprint("%/clay.c", SOURCE_PATH);

        success := true;
        #if OS == .WINDOWS {
            File.make_directory_if_it_does_not_exist("clay-jai/windows", true);

            command := ifx compile_debug {
                Process.break_command_into_strings("clang g -gcodeview -c source\\clay.c");
            } else {
                Process.break_command_into_strings("clang -O3 -c source\\clay.c");
            }
            result := Process.run_command(..command, capture_and_return_output=true, print_captured_output=true);
            if result.exit_code != 0 then return false;
            defer File.file_delete("clay.o");

            command = Process.break_command_into_strings("llvm-ar -rc clay-jai/windows/clay.lib clay.o");
            result = Process.run_command(..command, capture_and_return_output=true, print_captured_output=true);
            if result.exit_code != 0 then return false;
        } else {
            // TODO MacOS
            // TODO Linux
            assert(false);
        }

        if !success then return false;
    }

    output_filename: string;
    options: Generator.Generate_Bindings_Options;
    {
        using options;

        #if OS == .WINDOWS {
            array_add(*libpaths, "clay-jai/windows");
            output_filename = "windows.jai";
        } else {
            assert(false);
        }

        array_add(*libnames, "clay");
        array_add(*include_paths, SOURCE_PATH);
        array_add(*source_files, tprint("%/clay.h", SOURCE_PATH));
        array_add(*strip_prefixes, "Clay_");

        auto_detect_enum_prefixes = true;
        log_stripped_declarations = true;
        generate_compile_time_struct_checks = true;

        visitor = clay_visitor;
    }

    could_generate := Generator.generate_bindings(options, output_filename);

    File.file_delete("source/clay.h");

    return could_generate;
}

clay_visitor :: (decl: *Generator.Declaration, parent_decl: *Generator.Declaration) -> Generator.Declaration_Visit_Result {
    if !parent_decl
    {
        if array_find(DECLARATIONS_TO_OMIT, decl.name)
        {
            decl.decl_flags |= .OMIT_FROM_OUTPUT;
            return .STOP;
        }
    }

    return .RECURSE;
}

#scope_file

using Basic :: #import "Basic";
Generator :: #import "Bindings_Generator";
Compiler :: #import "Compiler";
File :: #import "File";
FileUtils :: #import "File_Utilities";
BuildCpp :: #import "BuildCpp";
Process :: #import "Process";
String :: #import "String";
WindowsResources :: #import "Windows_Resources";

#if OS == .WINDOWS {
    Windows :: #import "Windows";
    getenv :: Windows.getenv;
} else {
    Posix :: #import "POSIX";
    getenv :: Posix.getenv;
}
