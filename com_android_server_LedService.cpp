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

#include <hardware/led_hal.h>

namespace android
{

led_device_t* led_device;


jint ledOpen(JNIEnv *env, jclass cls)
{	
	jint err;
	hw_module_t* module;
	hw_device_t* device;
	
	ALOGI("native ledOpen ...");
	
	/* 1. hw_get_module */
	err = hw_get_module("led", (hw_module_t const**)&module);
	if(err == 0)  {
	    /* 2. get device : module->methods->open */
	    err = module->methods->open(module, NULL, &device);
	    if (err == 0) {
	        led_device =  (led_device_t *)device;
		 /* 3. call led_open */
		 return  led_device->led_open(led_device);
	    } else {
	        return -1;
	    }
	}

	return -1;
}

void ledClose(JNIEnv *env, jclass cls)
{	
	
}

jint ledCtrl(JNIEnv *env, jclass cls,jint which, jint status)
{	
	ALOGI("native ledCtrl %d %d", which, status);

	return led_device->led_ctrl(led_device, which, status);
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

