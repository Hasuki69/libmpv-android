#define UTIL_EXTERN
#include "jni_utils.h"

#include <jni.h>
#include <cstdlib>

bool acquire_jni_env(JavaVM *vm, JNIEnv **env)
{
    int ret = vm->GetEnv((void**) env, JNI_VERSION_1_6);
    if (ret == JNI_EDETACHED)
        return vm->AttachCurrentThread(env, NULL) == 0;
    else
        return ret == JNI_OK;
}

// Apparently it's considered slow to FindClass and GetMethodID every time we need them,
// so let's have a nice cache here
void init_methods_cache(JNIEnv *env) {
    static bool methods_initialized = false;
    if (methods_initialized)
        return;

    #define FIND_CLASS(name) reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass(name)))
    java_Integer = FIND_CLASS("java/lang/Integer");
    java_Integer_init = env->GetMethodID(java_Integer, "<init>", "(I)V");
    java_Integer_intValue = env->GetMethodID(java_Integer, "intValue", "()I");
    java_Double = FIND_CLASS("java/lang/Double");
    java_Double_init = env->GetMethodID(java_Double, "<init>", "(D)V");
    java_Double_doubleValue = env->GetMethodID(java_Double, "doubleValue", "()D");
    java_Boolean = FIND_CLASS("java/lang/Boolean");
    java_Boolean_init = env->GetMethodID(java_Boolean, "<init>", "(Z)V");
    java_Boolean_booleanValue = env->GetMethodID(java_Boolean, "booleanValue", "()Z");

    mpv_MPVLib = FIND_CLASS("dev/jdtech/mpv/MPVLib");
    mpv_MPVLib_eventProperty_S  = env->GetStaticMethodID(mpv_MPVLib, "eventProperty", "(Ljava/lang/String;)V"); // eventProperty(String)
    mpv_MPVLib_eventProperty_Sb = env->GetStaticMethodID(mpv_MPVLib, "eventProperty", "(Ljava/lang/String;Z)V"); // eventProperty(String, boolean)
    mpv_MPVLib_eventProperty_Sl = env->GetStaticMethodID(mpv_MPVLib, "eventProperty", "(Ljava/lang/String;J)V"); // eventProperty(String, long)
    mpv_MPVLib_eventProperty_Sd = env->GetStaticMethodID(mpv_MPVLib, "eventProperty", "(Ljava/lang/String;D)V"); // eventProperty(String, double)
    mpv_MPVLib_eventProperty_SS = env->GetStaticMethodID(mpv_MPVLib, "eventProperty", "(Ljava/lang/String;Ljava/lang/String;)V"); // eventProperty(String, String)
    mpv_MPVLib_event = env->GetStaticMethodID(mpv_MPVLib, "event", "(I)V"); // event(int)
    mpv_MPVLib_logMessage_SiS = env->GetStaticMethodID(mpv_MPVLib, "logMessage", "(Ljava/lang/String;ILjava/lang/String;)V"); // logMessage(String, int, String)
    #undef FIND_CLASS

    methods_initialized = true;
}
