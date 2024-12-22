AT_COMPILE_TIME :: true;

SOURCE_PATH :: "source";

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


Build_Type :: enum {
    STATIC_LIBRARY;
    DYNAMIC_LIBRARY;
    EXECUTABLE;
    OBJ_FILE;
}

build_cpp_static_lib  :: #bake_arguments build_cpp(type = .STATIC_LIBRARY);
build_cpp_dynamic_lib :: #bake_arguments build_cpp(type = .DYNAMIC_LIBRARY);
build_cpp_executable  :: #bake_arguments build_cpp(type = .EXECUTABLE);

// This is a modified version of the procedure from BuildCpp. It will assume a clang-like compiler if you add something to compiler_executable_path.
build_cpp :: (
    output_basename: string, 
    files: ..string, 
    type: Build_Type, 
    debug := false, 
    extra: [] string = .[], 
    library_files: [] string = .[], 
    target := OS, 
    compiler_executable_path := "", 
    ar_executable_path := "", 
    working_directory := "", 
    loc := #caller_location
) -> bool {
    Basic.auto_release_temp();
    Basic.push_allocator(Basic.temp);

    arguments: [..] string;

    output_filename: string;
    if target == .WINDOWS && compiler_executable_path == "" {
        if #complete type == {
            case .STATIC_LIBRARY;
                output_filename = Basic.tprint("%.lib", output_basename);
            case .DYNAMIC_LIBRARY;
                output_filename = Basic.tprint("%.dll", output_basename);
            case .EXECUTABLE;
                output_filename = Basic.tprint("%.exe", output_basename);
            case .OBJ_FILE;
                output_filename = Basic.tprint("%.obj", output_basename);
        }

        #if OS == .WINDOWS {
            String.path_overwrite_separators(output_filename, #char "\\");

            vc_path, linker_path := WindowsResources.find_visual_studio_in_a_ridiculous_garbage_way();

            kit_root := WindowsResources.find_windows_kit_root();
            if !kit_root {
                Compiler.compiler_report("Unable to find Windows Kit root; can't compile.\n", loc);
                return false;
            }
        } else {
            Compiler.compiler_report("Unable to find Visual Studio; can't compile.\n", loc);
            vc_path, linker_path: string;
            kit_root: string;
            return false; // Visual studio is not available in non-windows OS.
        }

        linker := String.join(linker_path, "\\", "cl.exe");
        Basic.array_add(*arguments, linker);

        Basic.array_add(*arguments, "/nologo");

        // Include directories:
        vc_include_path := String.join(vc_path, "\\..\\..\\include");
        kit_root_include := String.replace(kit_root, "Lib", "Include");
        Basic.array_add(*arguments,
            Basic.tprint("/I%", vc_include_path),
            Basic.tprint("/I%\\um", kit_root_include),
            Basic.tprint("/I%\\ucrt", kit_root_include),
            Basic.tprint("/I%\\shared", kit_root_include),
        );

        // Definitions:
        Basic.array_add(*arguments, "/DWIN32");
        if debug {
            Basic.array_add(*arguments, "/DDEBUG");
        }

        // Compiler options:
        if debug {
            Basic.array_add(*arguments, "/Od");   // Disable optimizations.
        } else {
            Basic.array_add(*arguments,
                "/O2",                      // Maximize speed.
                "/Oi",                      // Enable intrinsics.
            );
        }
        Basic.array_add(*arguments, "/W3");

        if debug {
            Basic.array_add(*arguments,
                "/DEBUG",                   // Generate debug info.
                "/Zi",                      // Generate pdb file.
                Basic.tprint("/Fd%.pdb", output_basename), // Sets name of pdb file.
            );
        }

        Basic.array_add(*arguments,
            "-diagnostics:caret",
            "-diagnostics:column"
        );

        Basic.array_add(*arguments, .. extra);

        // Add files:
        objs: [..] string;
        Basic.array_reserve(*objs, files.count);
        for files {
            src := String.copy_temporary_string(it);
            String.path_overwrite_separators(src);
            Basic.array_add(*arguments, src);

            Basic.array_add(*objs, Basic.tprint("%.obj", String.path_strip_extension(String.path_filename(it))));
        }

        // Make sure to cleanup the resulting obj files.
        defer {
            if type != .OBJ_FILE {
                for objs File.file_delete(it);
            }

            if type == .DYNAMIC_LIBRARY then File.file_delete(Basic.tprint("%.exp", output_basename));
        }

        if type == .STATIC_LIBRARY || type == .OBJ_FILE {
            Basic.array_add(*arguments, "/c");        // Compile without linking.
        } else {
            Basic.array_add(*arguments, "/link");

            // Linker options:
            if type == .DYNAMIC_LIBRARY {
                Basic.array_add(*arguments, "/DLL");
            }
            Basic.array_add(*arguments,
                "/MACHINE:AMD64",
                Basic.tprint("/OUT:%", output_filename),
                Basic.tprint("/libpath:%", vc_path),
                Basic.tprint("/libpath:%\\um\\x64", kit_root),
                Basic.tprint("/libpath:%\\ucrt\\x64", kit_root),
            );
        }

        Basic.array_add(*arguments, .. library_files);

        Basic.log("%", Process.get_quoted_command_string(arguments));
        result, output_string, error_string := Process.run_command(..arguments, capture_and_return_output = true, print_captured_output = true, working_directory = working_directory);

        if result.exit_code {
            Compiler.compiler_report(Basic.tprint("Compiler failed with exit code '%'.\n", result.exit_code), loc);
            return false;
        }

        if type == .STATIC_LIBRARY {
            // Create library:
            Basic.array_reset_keeping_memory(*arguments);

            librarian := String.join(linker_path, "\\lib.exe");

            Basic.array_add(*arguments, librarian, "/nologo");
            Basic.array_add(*arguments, ..objs);
            Basic.array_add(*arguments, Basic.tprint("/OUT:%", output_filename));

            Basic.log("%", Process.get_quoted_command_string(arguments));
            result, output_string, error_string := Process.run_command(..arguments, capture_and_return_output = true, print_captured_output = true, working_directory = working_directory);

            if result.exit_code {
                Compiler.compiler_report(Basic.tprint("Librarian failed with exit code '%'.\n", result.exit_code), loc);
                return false;
            }
        }
    } else {
        if #complete type == {
            case .STATIC_LIBRARY;
                #if OS == .WINDOWS {
                    output_filename = Basic.tprint("%.lib", output_basename);
                } else {
                    output_filename = Basic.tprint("%.a", output_basename);
                }
                
            case .OBJ_FILE;
                output_filename = Basic.tprint("%.o", output_basename);
            case .DYNAMIC_LIBRARY;
                if target == {
                    case .WINDOWS; output_filename = Basic.tprint("%.dll", output_basename);
                    case .MACOS;   output_filename = Basic.tprint("%.dylib", output_basename);
                    case .LINUX;   #through;
                    case .ANDROID; output_filename = Basic.tprint("%.so", output_basename);
                    case .NONE; #if OS == .WINDOWS then output_filename = Basic.tprint("%.dll", output_basename); else assert(false);
                    case; assert(false);
                }
            case .EXECUTABLE;
                output_filename = Basic.copy_temporary_string(output_basename);
        }

        #if OS == .WINDOWS {
            String.path_overwrite_separators(output_filename, #char "\\");
        }

        compiler := compiler_executable_path;
        ar := ar_executable_path;
        if !compiler || (type == .STATIC_LIBRARY && !ar) {
            is_cpp_project := false;
            for files {
                if String.ends_with(it, ".cpp") {
                    is_cpp_project = true; // Use c++ compiler, so we link the required c++ runtime libraries by default.
                    break;
                }
            }

            if !compiler {
                if is_cpp_project {
                    // @Cleanup: Could be simplified to the following, but that currently triggers a compiler bug. -rluba, 2024-01-23
                    // compiler = ifx to_string(getenv("CXX")) else "clang++";
                    compiler = to_string(getenv("CXX"));
                    if !compiler compiler ="clang++";
                } else {
                    compiler = to_string(getenv("CC"));
                    if !compiler compiler ="clang";
                }
            }

            if !ar ar = "ar";
            
        }

        Basic.array_add(*arguments, compiler);

        if debug {
            Basic.array_add(*arguments, "-g", "-Og");
        } else {
            Basic.array_add(*arguments, "-O3");
        }

        Basic.array_add(*arguments, ..extra);

        if type == .STATIC_LIBRARY || type == .OBJ_FILE {
            array_add(*arguments, "-c");
        } else {
            if type == .DYNAMIC_LIBRARY {
                array_add(*arguments, "-shared", "-fpic");
            }

            if target == .MACOS {
                if type == {
                    case .DYNAMIC_LIBRARY;
                        array_add(*arguments, "-install_name", tprint("@rpath/%", output_filename));
                    case .EXECUTABLE;
                        array_add(*arguments, "-rpath", "@loader_path");
                }
            }

            array_add(*arguments, "-o", output_filename);
        }

        // Add files:
        objs: [..] string;
        array_reserve(*objs, files.count);
        for files {
            src := copy_temporary_string(it);
            String.path_overwrite_separators(src);
            array_add(*arguments, src);

            array_add(*objs, tprint("%.o", String.path_basename(it)));
        }

        // Make sure to cleanup the resulting obj files.
        defer {
            if type != .OBJ_FILE {
                for objs File.file_delete(it);
            }
        }

        array_add(*arguments, .. library_files);

        log("%", Process.get_quoted_command_string(arguments));
        result, output_string, error_string := Process.run_command(..arguments, capture_and_return_output = true, print_captured_output = true, working_directory = working_directory);

        if result.exit_code {
            Compiler.compiler_report(tprint("Compiler failed with exit code '%'.\n", result.exit_code), loc);
            return false;
        }

        if type == .STATIC_LIBRARY {
            File.file_delete(output_filename); // ar only adds/updates the archive, but does not delete files from it.

            // Create library:
            array_reset_keeping_memory(*arguments);
            array_add(*arguments,
                ar,
                "-rc",   // replace or insert files into the archive, do not warn if archive needs to be created.
                output_filename,
            );

            array_add(*arguments, ..objs);

            // Run archiver command:
            log("%", Process.get_quoted_command_string(arguments));
            result, output_string, error_string := Process.run_command(..arguments, capture_and_return_output = true, print_captured_output = true, working_directory = working_directory);

            if result.exit_code {
                Compiler.compiler_report(tprint("Archive command failed with exit code '%'.\n", result.exit_code), loc);
                return false;
            }
        }
    }

    return true;
}

enum_cpp_files :: (path: string, recursive:=false) -> [..] string {
    files: [..] string;

    visitor :: (info: *FileUtils.File_Visit_Info, files: *[..] string) {
        extension := String.path_extension(info.full_name);
        if extension == "cpp" || extension == "c" {
            Basic.array_add(files, String.copy_string(info.full_name));
        }
    }

    FileUtils.visit_files(path, recursive=recursive, *files, visitor);

    return files;
}

free_cpp_files :: (files: [] string) {
    for files Basic.free(it);
    Basic.array_free(files);
}


generate_bindings :: (args: [] string, minimum_os_version: type_of(Compiler.Build_Options.minimum_os_version)) -> bool {
    compile := Basic.array_find(args, "-compile");
    compile_debug := Basic.array_find(args, "-debug");

    if compile {
        could_copy := FileUtils.copy_file("../../clay.h", "source/clay.h");
        if !could_copy then return false;

        source_file := Basic.tprint("%/clay.c", SOURCE_PATH);

        success := true;
        #if OS == .WINDOWS {
            File.make_directory_if_it_does_not_exist("clay-jai/windows", true);

            // Can't use this because clay doesn't support MSVC
            success &&= build_cpp_static_lib(
                "clay-jai/windows/clay", 
                source_file, 
                debug = compile_debug, 
                target=.NONE, 
                compiler_executable_path="clang",
                ar_executable_path="llvm-ar",
            );

            // {
            //     command := Process.break_command_into_strings("clang -c -o clay-jai/windows/clay.lib -static source/clay.c");
            //     result, out, error := Process.run_command(..command, capture_and_return_output = true);

            //     write_string(out);
            //     if result.exit_code != 0
            //     {
            //         write_string("Failed to build clay. Do you have clang installed ?\n");
            //         write_string(error);
            //         success = false;
            //     }
            // }
            
            // {
            //     command := Process.break_command_into_strings("clang -c -o clay-jai/windows/clay.dll -dynamic source/clay.c");
            //     result, out, error := Process.run_command(..command, capture_and_return_output = true);

            //     write_string(out);
            //     if result.exit_code != 0
            //     {
            //         write_string("Failed to build clay. Do you have clang installed ?\n");
            //         write_string(error);
            //         success = false;
            //     }
            // }
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
            Basic.array_add(*libpaths, "clay-jai/windows");
            output_filename = "windows.jai";
        } else {
            assert(false);
        }

        Basic.array_add(*libnames, "clay");
        Basic.array_add(*include_paths, SOURCE_PATH);
        Basic.array_add(*source_files, Basic.tprint("%/clay.h", SOURCE_PATH));
        Basic.array_add(*strip_prefixes, "Clay_");

        auto_detect_enum_prefixes = true;
        log_stripped_declarations = true;
        generate_compile_time_struct_checks = false;
    }

    could_generate := Generator.generate_bindings(options, output_filename);

    File.file_delete("source/clay.h");

    return could_generate;
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
