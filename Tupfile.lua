
tup.include('package.lua')

CFLAGS = {'-fPIC -std=c++11 -DFIBRE_COMPILE'}
LDFLAGS = {'-static-libstdc++'}

-- Runs the specified shell command immediately (not as part of the dependency
-- graph).
-- Returns the values (return_code, stdout) where stdout has the trailing new
-- line removed.
function run_now(command)
    local handle
    handle = io.popen(command)
    local output = handle:read("*a")
    local rc = {handle:close()}
    if not rc[1] then
        error("failed to invoke "..command)
    end
    return string.sub(output, 0, -2)
end

if tup.getconfig("CC") == "" then
    CXX = 'clang++'
    LINKER = 'clang++'
else
    CXX = tup.getconfig("CC")
    LINKER = tup.getconfig("CC")
end

function get_bool_config(name, default)
    if tup.getconfig(name) == "" then
        return default
    elseif tup.getconfig(name) == "true" then
        return true
    elseif tup.getconfig(name) == "false" then
        return false
    else
        error(name.." ("..tup.getconfig(name)..") must be 'true' or 'false'.")
    end
end

CFLAGS += tup.getconfig("CFLAGS")
LDFLAGS += tup.getconfig("LDFLAGS")
DEBUG = get_bool_config("DEBUG", true)

machine = run_now(CXX..' -dumpmachine') -- works with both clang and GCC

BUILD_TYPE='-shared'

if string.find(machine, "x86_64.*%-linux%-.*") then
    outname = 'libfibre-linux-amd64.so'
    LDFLAGS += '-lpthread -Wl,--version-script=libfibre.version -Wl,--gc-sections'
    STRIP = not DEBUG
elseif string.find(machine, "arm.*%-linux%-.*") then
	outname = 'libfibre-linux-armhf.so'
    LDFLAGS += '-lpthread -Wl,--version-script=libfibre.version -Wl,--gc-sections'
    STRIP = false
elseif string.find(machine, "x86_64.*-mingw.*") then
    outname = 'libfibre-windows-amd64.dll'
    LDFLAGS += '-lpthread -Wl,--version-script=libfibre.version'
    STRIP = not DEBUG
elseif string.find(machine, "x86_64.*-apple-.*") then
    outname = 'libfibre-macos-x86.dylib'
    STRIP = false
elseif string.find(machine, "wasm.*") then
    outname = 'libfibre-wasm.js'
    STRIP = false
    BUILD_TYPE = ''
else
    error('unknown machine identifier '..machine)
end

LDFLAGS += BUILD_TYPE

if DEBUG then
    CFLAGS += '-O1 -g'
else
    CFLAGS += '-O3' -- TODO: add back -lfto
end

function compile(src_file)
    obj_file = 'build/'..tup.file(src_file)..'.o'
    tup.frule{
        inputs={src_file},
        command='^co^ '..CXX..' -c %f '..tostring(CFLAGS)..' -o %o',
        outputs={obj_file}
    }
    return obj_file
end

pkg = get_fibre_package({
    enable_server=false,
    enable_client=true,
    enable_tcp_server_backend=get_bool_config("ENABLE_TCP_SERVER_BACKEND", true),
    enable_tcp_client_backend=get_bool_config("ENABLE_TCP_CLIENT_BACKEND", true),
    enable_libusb_backend=get_bool_config("ENABLE_LIBUSB_BACKEND", true),
    allow_heap=true,
    use_pkgconf=get_bool_config("USE_PKGCONF", true)
})

CFLAGS += pkg.cflags
LDFLAGS += pkg.ldflags

for _, inc in pairs(pkg.include_dirs) do
    CFLAGS += '-I./'..inc
end

for _, src_file in pairs(pkg.code_files) do
    object_files += compile(src_file)
end
object_files += compile('libfibre.cpp')

if not STRIP then
    compile_outname=outname
else
    compile_outname=outname..'.fat'
end

if tup.ext(outname) == 'js' then
    extra_outputs = {tup.base(compile_outname)..'.wasm'}
else
    extra_outputs = {}
end

tup.frule{
    inputs=object_files,
    command='^c^ '..LINKER..' %f '..tostring(CFLAGS)..' '..tostring(LDFLAGS)..' -o %o',
    outputs={compile_outname, extra_outputs=extra_outputs}
}

if STRIP then
    tup.frule{
        inputs={compile_outname},
        command='strip --strip-all --discard-all %f -o %o',
        outputs={outname}
    }
end
