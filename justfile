includes := "-I quickjs_guest/include/include -I quickjs_guest/include/libc/musl/include -I quickjs_guest/include/libc/musl/arch/x86_64/ -I quickjs_guest/include/printf -I quickjs_guest/include -I quickjs-2024-01-13"
files := "quickjs_guest/main.c quickjs-2024-01-13/quickjs.c quickjs-2024-01-13/libregexp.c quickjs-2024-01-13/libunicode.c quickjs-2024-01-13/cutils.c quickjs-2024-01-13/libbf.c"

build:
    clang {{includes}} {{files}} -DCONFIG_VERSION=\"2024-01-13\" -D_GNU_SOURCE -DCONFIG_BIGNUM -nostdinc -nostdlib -pie -H -D putchar=_putchar --target=x86_64-unknown-linux-none -Wl,-entry,entrypoint -l hyperlight_guest_capi -L quickjs_guest/libs/release -o guest

download-qjs:
    wget https://bellard.org/quickjs/quickjs-2024-01-13.tar.xz
    tar -xvf quickjs-2024-01-13.tar.xz
    rm quickjs-2024-01-13.tar.xz

test:
    clang test.c -I quickjs-2024-01-13/ quickjs-2024-01-13/quickjs.c quickjs-2024-01-13/libregexp.c quickjs-2024-01-13/libunicode.c quickjs-2024-01-13/cutils.c quickjs-2024-01-13/libbf.c -DCONFIG_VERSION=\"2024-01-13\" -D_GNU_SOURCE -lm -DCONFIG_BIGNUM 