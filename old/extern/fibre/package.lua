
fibre_root = tup.getcwd()

-- Returns a table that contains the Fibre code files and the flags required to
-- compile and link those files.
--
-- args: A dictionary containing the fibre options. Refer to the Compile Options
--       in README.md for a list of available options. For example the option
--       `FIBRE_ENABLE_SERVER` maps to the argument `args.enable_server`.
-- In addition:
-- args.pkgconf: Controls the use of the pkgconf or pkg-config utility that
--     shall be used to locate build dependencies. Can be one of the following:
--        - A string: Use the binary provided by the string. Fail if it doesn't
--          exist.
--        - true: Use "pkgconf" and fall back to "pkg-config" if "pkgconf"
--          doesn't exist. Fail if both don't exist.
--        - false: Don't use pkg-config. The user is responsible of determining
--          the required compile and link flags.
--        - nil: Try both "pkgconf" and "pkg-config". If both don't exist fall
--          back to a hardcoded list of well-known settings.
--
-- Returns: A dictionary with the following items:
--  code_files: A list of strings that name the C++ code files to be compiled.
--              The names are relative to package.lua.
--  include_dirs: A list of directories that must be added to the include path
--                when compiling the code files. The paths are relative to
--                package.lua.
--  cflags: A list of flags that should be passed to the compiler/linker when
--          compiling and linking the code files.
--  ldflags: A list of linker flags that should be passed to the linker when
--           linking the object files.
function get_fibre_package(args)
    pkg = {
        root = fibre_root,
        code_files = {
            'fibre.cpp',
            'channel_discoverer.cpp',
        },
        include_dirs = {'include'},
        cflags = {},
        ldflags = {},
    }

    -- Select a pkgconf function
    if args.pkgconf == true or args.pkgconf == nil then
        -- Autodetect pkgconf
        if test_pkgconf('pkgconf') then
            print("using pkgconf")
            pkgconf_file = 'pkgconf'
            pkgconf = real_pkgconf
        elseif test_pkgconf('pkg-config') then
            print("using pkg-config")
            pkgconf_file = 'pkg-config'
            pkgconf = real_pkgconf
        elseif args.pkgconf == nil then
            print("using hardcoded pkgconf")
            pkgconf = hardcoded_pkgconf
        else
            error("couldn't find pkgconf nor pkg-config")
        end

    elseif args.pkgconf == false then
        print("not using pkgconf")
        pkgconf_file = nil
        pkgconf = null_pkgconf
    else
        print("using pkgconf: "..args.pkgconf)
        pkgconf_file = args.pkgconf
        pkgconf = real_pkgconf
    end

    if args.enable_libusb_backend then
        pkg.code_files += 'platform_support/libusb_backend.cpp'
        pkgconf(pkg, "libusb-1.0")

        -- TODO: only add pthread on linux and windows
        pkg.ldflags += '-lpthread'
    end
    if args.enable_client then
        --pkg.code_files += 'legacy_object_client.cpp'
        pkg.code_files += 'legacy_object_client.cpp'
    end
    if args.enable_server then
        pkg.code_files += 'legacy_object_server.cpp'
    end

    pkg.code_files += 'legacy_protocol.cpp'
    pkg.code_files += 'connection.cpp'
    pkg.code_files += 'endpoint_connection.cpp'
    pkg.code_files += 'multiplexer.cpp'
    pkg.code_files += 'func_utils.cpp'
    pkg.code_files += 'platform_support/epoll_event_loop.cpp'
    pkg.code_files += 'platform_support/socket_can.cpp'
    pkg.code_files += 'platform_support/can_adapter.cpp'
    pkg.code_files += 'platform_support/posix_tcp_backend.cpp'
    pkg.code_files += 'platform_support/posix_socket.cpp'
    pkg.code_files += 'platform_support/usb_host_adapter.cpp'
    pkg.code_files += 'platform_support/webusb_backend.cpp'

    if args.enable_tcp_client_backend or args.enable_tcp_server_backend then
        -- TODO: chose between windows and posix backend
        pkg.ldflags += '-lanl'
    end

    return pkg
end

function fibre_autogen(yaml_file)
    python_command = 'python3 -B'
    interface_generator = '../tools/interface_generator.py'
    tup.frule{inputs={'../cpp/static_exports_template.j2'}, command=python_command..' '..interface_generator..' --definitions '..yaml_file..' --template %f --output %o', outputs='autogen/static_exports.cpp'}
    tup.frule{inputs={'../cpp/interfaces_template.j2'}, command=python_command..' '..interface_generator..' --definitions '..yaml_file..' --template %f --output %o', outputs='autogen/interfaces.hpp'}
    tup.frule{inputs={'../cpp/legacy_endpoints_template.j2'}, command=python_command..' '..interface_generator..' --definitions '..yaml_file..' --template %f --output %o', outputs='autogen/endpoints.cpp'}

    return {
        code_files={'autogen/static_exports.cpp', 'autogen/endpoints.cpp'},
        autogen_headers={'autogen/interfaces.hpp'}
    }
end

-- Runs the specified shell command immediately (not as part of the dependency
-- graph).
-- Returns the values (return_code, stdout) where stdout has the trailing new
-- line removed.
function fibre_run_now(command)
    local handle
    handle = io.popen(command)
    local output = handle:read("*a")
    local rc = {handle:close()}
    return string.sub(output, 0, -2), rc[1]
end

function test_pkgconf(name)
    local str, rc = fibre_run_now(name.." --version 2>&1 >/dev/null")
    return rc
end

function real_pkgconf(pkg, lib)
    pkg.cflags += fibre_run_now(pkgconf_file..' '..lib..' --cflags')
    pkg.ldflags += fibre_run_now(pkgconf_file..' '..lib..' --libs')
end

function null_pkgconf(pkg, lib)
    -- don't do anything
end

function hardcoded_pkgconf(pkg, lib)
    libs = {
        ['libusb-1.0'] = {cflags = {}, ldflags = {}},
    }
    tup.append_table(pkg.cflags, libs[lib].cflags)
    tup.append_table(pkg.ldflags, libs[lib].ldflags)
end