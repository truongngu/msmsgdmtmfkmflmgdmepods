//#define Win32
//#define Android
#define WindowPhone
//#define iOS;

#if defined WindowPhone
#define UNICODE
#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>
#define WindowPhoneWithXAML
//#define WindowPhoneWithoutXAML
#endif

#define LogOut(s) {printf("Log : %s\n",s);}
#define  LOG_TAG    "APP"
#define Delete(a) if (a){ delete a; a=0;}

#define Delete_Array(a) if (a){ delete[] a; a=0;}

#define Delete_Array_Num(a,n) {if (a) {for (int i=0;i<n;i++){ if (a[i]) {delete a[i]; a[i]=0;}}}

#define PI 3.1415926535898

#define MAX_LEN 255
#define MAXINT 100000000

#ifdef Android
#include <jni.h>
#include <android/log.h>
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif
//#define Debug

