#include <jni.h>
#include <android/log.h>

#include "client/linux/handler/exception_handler.h"

#define LOG_TAG "flutter_breakpad"

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static bool dumpCallback(
        const google_breakpad::MinidumpDescriptor &descriptor,
        void *context,
        bool succeeded
) {
    ALOGD("Dump path: %s\n", descriptor.path());
    return succeeded;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    ALOGI("JNI_OnLoad");
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    google_breakpad::MinidumpDescriptor descriptor("/data/data/com.example.flutter_breakpad/files");
    static google_breakpad::ExceptionHandler handler(descriptor, nullptr, dumpCallback, nullptr,
                                                     true, -1);

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    ALOGI("JNI_OnUnload");
}

extern "C"
JNIEXPORT void JNICALL Java_com_example_flutter_1breakpad_Utils_crash(JNIEnv *, jobject) {
    volatile int *a = (int *) nullptr;
    *a = 1;
}
