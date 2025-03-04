includes := "-I guest/include/include -I guest/include/musl/include -I guest/include/musl/arch/x86_64/ -I guest/include/printf -I guest/include -I quickjs-2024-01-13"
files := "guest/main.c quickjs-2024-01-13/quickjs.c quickjs-2024-01-13/libregexp.c quickjs-2024-01-13/libunicode.c quickjs-2024-01-13/cutils.c quickjs-2024-01-13/libbf.c"

build: (build-qjs-guest) (build-outside-hl)

# This will build a hyperlight guest by linking hyperlight's libc, and the resulting binary can be run inside hyperlight.
# The resulting binary can be used by running `cargo run -- --eval '5+5'` inside 'host' directory.
build-qjs-guest:
    clang {{includes}} {{files}} -O3 -DHYPERLIGHT -DCONFIG_VERSION=\"2024-01-13\" -D_GNU_SOURCE -DCONFIG_BIGNUM -nostdinc -nostdlib -fpie -D putchar=_putchar -Wno-macro-redefined -Wno-ignored-attributes -Wno-implicit-const-int-float-conversion --target=x86_64-unknown-elf -e entrypoint -l hyperlight_guest_capi -L guest/libs/release -o quickjs-guest

run file:
    cd host && cargo run < ../{{ file }}

# this will overwrite any code changes you've made to quickjs
download-qjs:
    wget https://bellard.org/quickjs/quickjs-2024-01-13.tar.xz
    tar -xvf quickjs-2024-01-13.tar.xz
    rm quickjs-2024-01-13.tar.xz

patch-qjs:
    cd quickjs-2024-01-13 && patch -p1 < ../quickjs_hyperlight.patch

download-hl-headers-and-lib:
    wget https://github.com/hyperlight-dev/hyperlight/releases/download/v0.2.0/include.tar.gz -P guest/include
    tar -xvf guest/include/include.tar.gz -C guest/include/
    rm guest/include/include.tar.gz
    wget https://github.com/hyperlight-dev/hyperlight/releases/download/v0.2.0/hyperlight-guest-c-api-linux.tar.gz -P guest/libs
    tar -xvf guest/libs/hyperlight-guest-c-api-linux.tar.gz -C guest/libs
    rm guest/libs/hyperlight-guest-c-api-linux.tar.gz