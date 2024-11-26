includes := "-I quickjs_guest/include/include -I quickjs_guest/include/libc/musl/include -I quickjs_guest/include/libc/musl/arch/x86_64/ -I quickjs_guest/include/printf -I quickjs_guest/include -I quickjs-2024-01-13"
files := "quickjs_guest/main.c quickjs-2024-01-13/quickjs.c quickjs-2024-01-13/libregexp.c quickjs-2024-01-13/libunicode.c quickjs-2024-01-13/cutils.c quickjs-2024-01-13/libbf.c"

alias b := build-quickjs-guest

build-quickjs-guest:
    clang {{includes}} {{files}} -DCONFIG_VERSION=\"2024-01-13\" -D_GNU_SOURCE -DCONFIG_BIGNUM -nostdinc -nostdlib -pie -D putchar=_putchar -Wno-macro-redefined -Wno-ignored-attributes -Wno-implicit-const-int-float-conversion --target=x86_64-unknown-linux-none -Wl,-entry,entrypoint -l hyperlight_guest_capi -L quickjs_guest/libs/debug -o guest

# this will overwrite any code changes you've made to quickjs
download-qjs:
    wget https://bellard.org/quickjs/quickjs-2024-01-13.tar.xz
    tar -xvf quickjs-2024-01-13.tar.xz
    rm quickjs-2024-01-13.tar.xz