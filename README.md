# flutter_breakpad

An example Flutter project for demostrating how to intergrate [Google Breakpad](https://chromium.googlesource.com/breakpad/breakpad)

## Android

- Build `libbreakpad_client.a` on Linux (e.g. https://multipass.run/)

> $BREAKPAD is local `fetch & gclient sync` of https://chromium.googlesource.com/breakpad/breakpad/

> $NDK is local path of your Android NDK directory

```sh
cd $BREAKPAD/src/android
cp -r google_breakpad jni
$NDK/ndk-build
```

- Install `libbreakpad_client.a` of all architectures

```sh
mkdir -p ./android/app/src/main/cmakeLibs
cp -r $BREAKPAD/src/android/obj/local/* ./android/app/src/main/cmakeLibs/
```

- run on macOS/Linux

```sh
# Device/emulator connected
$ android_abi=`adb shell getprop ro.product.cpu.abi`
$ flutter run
✓ Built build/app/outputs/flutter-apk/app-debug.apk.
I/flutter_breakpad(31631): JNI_OnLoad
D/flutter_breakpad(31631): Dump path: /data/data/com.example.flutter_breakpad/files/f5258c0e-eff3-433a-7ea47880-c756fc17.dmp
$ adb shell "run-as com.example.flutter_breakpad sh -c 'cat /data/data/com.example.flutter_breakpad/files/f5258c0e-eff3-433a-7ea47880-c756fc17.dmp'" >| libflutter-breakpad.so.dmp
```

- run on Linux (e.g. https://multipass.run/)

> $CLI_BREAKPAD is local clone of https://github.com/Sunbreak/cli-breakpad.trial

```sh
$ $CLI_BREAKPAD/breakpad/linux/$(arch)/dump_syms build/app/intermediates/cmake/debug/obj/${android_abi}/libflutter-breakpad.so > libflutter-breakpad.so.sym
$ uuid=`awk 'FNR==1{print \$4}' libflutter-breakpad.so.sym`
$ mkdir -p symbols/libflutter-breakpad.so/$uuid/
$ mv ./libflutter-breakpad.so.sym symbols/libflutter-breakpad.so/$uuid/
$ $CLI_BREAKPAD/breakpad/linux/$(arch)/minidump_stackwalk libflutter-breakpad.so.dmp symbols/ > libflutter-breakpad.so.log
```