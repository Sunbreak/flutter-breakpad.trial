# flutter_breakpad

An example Flutter project for demostrating how to intergrate [Google Breakpad](https://chromium.googlesource.com/breakpad/breakpad)

> $BREAKPAD is local `fetch & gclient sync` of https://chromium.googlesource.com/breakpad/breakpad/

## Android

- Build `libbreakpad_client.a` on Linux (e.g. https://multipass.run/)

> $NDK is local path of your Android NDK directory
>
> $CLI_BREAKPAD is local clone of https://github.com/Sunbreak/cli-breakpad.trial

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

```sh
$ $CLI_BREAKPAD/breakpad/linux/$(arch)/dump_syms build/app/intermediates/cmake/debug/obj/${android_abi}/libflutter-breakpad.so > libflutter-breakpad.so.sym
$ uuid=`awk 'FNR==1{print \$4}' libflutter-breakpad.so.sym`
$ mkdir -p symbols/libflutter-breakpad.so/$uuid/
$ mv ./libflutter-breakpad.so.sym symbols/libflutter-breakpad.so/$uuid/
$ $CLI_BREAKPAD/breakpad/linux/$(arch)/minidump_stackwalk libflutter-breakpad.so.dmp symbols/ > libflutter-breakpad.so.log
```

## iOS

- Build universal `libBreakpad.a` on macOS

> Patch [fix missing encoding_util.h/m in iOS client project](https://github.com/Sunbreak/breakpad/commit/63619f6225f4c1083e58a9b83263451b617d0703) onto $BREAKPAD/src

```sh
cd $BREAKPAD/src/client/ios
xcodebuild -sdk iphoneos -arch arm64 && xcodebuild -sdk iphonesimulator -arch x86_64
lipo -create build/Release-iphoneos/libBreakpad.a build/Release-iphonesimulator/libBreakpad.a -output libBreakpad.a
```

- Install `libBreakpad.a`

1. `cp $BREAKPAD/src/client/ios/libBreakpad.a ios/Runner`
2. Go target `Runner`'s Build Phases
3. Click on the `+` under `Link Binary With Libraries`, then click on the `Add Other` button
4. Navigate to the `libBreakpad.a` and add it
5. Click on the `+` under `Link Binary With Libraries` and add `libc++.tbd`
6. Add `BreakpadUrl` into `Info.plist`

- run on macOS

1. Get simulator UUID and run on it

```sh
$ flutter devices
1 connected device:
iPhone SE (2nd generation) (mobile) • C7E50B0A-D9AE-4073-9C3C-14DAF9D93329 • ios        • com.apple.CoreSimulator.SimRuntime.iOS-14-5 (simulator)
$ device=C7E50B0A-D9AE-4073-9C3C-14DAF9D93329
$ flutter run -d $device
Launching lib/main.dart on iPhone SE (2nd generation) in debug mode...
Running Xcode build...                                                  
 └─Compiling, linking and signing...                      2,878ms
Xcode build done.                                            6.5s
Swift/x86_64-apple-ios-simulator.swiftinterface:32647: Fatal error: Division by zero
Lost connection to device.
```

2. Find application data and get dump file

```sh
$ data=`xcrun simctl get_app_container booted com.example.flutterBreakpad data`
$ ls $data/Library/Caches/Breakpad
A1D2CF75-848E-42C4-8F5C-0406E8520647.dmp        Config-FsNxCZ
$ cp $data/Library/Caches/Breakpad/A1D2CF75-848E-42C4-8F5C-0406E8520647.dmp .
```

3. Parse the dump file via symbols of `Runner`

> Only C/C++/Objective-C crash for now

```sh
$ $CLI_BREAKPAD/breakpad/mac/dump_syms build/ios/Debug-iphonesimulator/Runner.app/Runner > Runner.sym
$ uuid=`awk 'FNR==1{print \$4}' Runner.sym`
$ mkdir -p symbols/Runner/$uuid/
$ mv ./Runner.sym symbols/Runner/$uuid/
$ $CLI_BREAKPAD/breakpad/mac/$(arch)/minidump_stackwalk A1D2CF75-848E-42C4-8F5C-0406E8520647.dmp symbols > Runner.log
```