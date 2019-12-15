#define LOG_TAG "LedService"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware_legacy/vibrator.h>

#include <stdio.h>

#include <stdlib.h>

#include <android/log.h> /* liblog */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/ioctl.h>

namespace android
{
static jint fd;


jint ledOpen(JNIEnv *env, jclass cls)
{	
	fd = open("dev/leds",O_RDWR);
	ALOGI("native ledOpen: %d", fd);

	if(fd >= 0)
		return 0;
	else
		return -1;
}

void ledClose(JNIEnv *env, jclass cls)
{	
	ALOGI("native ledClose ...");
	close(fd);
}

jint ledCtrl(JNIEnv *env, jclass cls,jint which, jint status)
{	
	int ret = ioctl(fd, status, which);
	ALOGI("native ledCtrl: %d %d %d",which,status,ret);
	return ret;
}

static const JNINativeMethod methods[] = {
	{"native_ledOpen", "()I", (void *)ledOpen},
	{"native_ledClose", "()V", (void *)ledClose},
	{"native_ledCtrl", "(II)I", (void *)ledCtrl},
};

int register_android_server_LedService(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "com/android/server/LedService",
            methods, NELEM(methods));
}

};

