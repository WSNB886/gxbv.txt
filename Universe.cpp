#include <android/native_window_jni.h>
#include "其他文件/Tools/android_native_app_glue.h"
#include <android/log.h>
#include "其他文件/UE_SDK/SDK.hpp"
#include "其他文件/Tools/GlossHook/Gloss.h"
using namespace SDK;
using namespace std;
#include "其他文件/Tools/include.h"
#include "Macros.h"
#include <pthread.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <jni.h>
#include <atomic>
#include <chrono>
#include <sys/mman.h>
#include <unistd.h>
#include <dlfcn.h>
#include <unordered_map>
#include <string>
#include <cstring>
#include "Vector3.hpp"
#include <array>
#include "其他文件/Tools/UE4.h"
#include "其他文件/美化配置/Structure.h"
#define HOOK_LIB(lib, offset, ptr, orig) hook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)
#define HOOK_LIB_NO_ORIG(lib, offset, ptr) hook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)


#define GNames_Offset 0x7e33df4
#define GUObject_Offset 0xe46bde0
#define GetActorArray_Offset 0x9f6ac78
#define Process_Event_Offset 0x8097970//Child
#define GEngine_Offset 0xe7b89e8//UEngine
#define GEngine_Offset 0xe793470//ULocalPlayer
#define VWorld Offset 0xe78c0e0
#define GNativeAndroidApp_Offset 0xe1c3000
#define Canvas_Map_Offset 0xe294260
#define Process_Event_Offset 0x9ad2fa0//Main
#define Actors_Offset 0xA0
uintptr_t anogs;
uintptr_t anort;
uintptr_t tgpa;
uintptr_t cubehawk;
uintptr_t GCloudVoice;

typedef unsigned long _DWORD;
typedef unsigned char _QWORD;
bool 距离;
bool 盒子;
bool 战利品;
bool 对局信息;
bool 炸弹警告;
static bool bShowOverlay = true;   // true=显示 false=隐藏
bool 空投箱;
float XY = 1;
float 衣服;
bool 梅花初始化;
//JNIEnv* env = nullptr;
void AimAngle(FRotator &angles) {
    if (angles.Pitch > 180)
    angles.Pitch -= 360;
    if (angles.Pitch < -180)
    angles.Pitch += 360;
    
    if (angles.Pitch < -75.f)
    angles.Pitch = -75.f;
    else if (angles.Pitch > 75.f)
    angles.Pitch = 75.f;
    
    while (angles.Yaw < -180.0f)
    angles.Yaw += 360.0f;
    while (angles.Yaw > 180.0f)
    angles.Yaw -= 360.0f;
}
bool 载具;
// ========= 远程卡密 =========
//const char* WEIYUN_RAW_URL = "https://share.weiyun.com/juFN19aj";   // 复制分享链接即可
// ============================
bool 忽略机器人;
bool 雷达;
bool 背敌;
static bool bShowGUI = true;   // true = 显示 GUI 窗口
bool 测试广角;
bool 健康;
bool 枪械;
float 自改压枪幅度 = 1.1;
bool 姓名;
bool 骨骼;
static float 温度 = 0.0f;
static int 电量 = 0;
bool 线;
bool 载具血量;
bool 后座;
bool 载具油量;
bool 投掷物预警;
bool 开火;
bool 开镜; 
bool 开镜开火;
bool 身体;
bool 平滑;
bool 口 = true;
bool 无名 = true;
bool 头部;
bool 瞄准方式 = true;
bool 預言;
bool 部位瞄准 = true;
bool 开关;
float 自瞄距离;
bool 大厅;
bool 动态 = false;
bool 水印 = true;
bool 隐藏 = true;
bool 隐藏1 = true;
bool 控件; 
float sy = 0.0f;
float sx = 0.0f;
uintptr_t Engine;
static void* tslFontUI = NULL;
static bool 调用z = true;
static bool 调用1 = true;
static bool 调用2 = true;
bool 广角;
bool 狗头;
bool 聚点;
bool 打击特效;
float 雷达Y = 150.0;
float 雷达X = 150.0;

uintptr_t gcloud;
std::chrono::steady_clock::time_point lastWeaponChangeTime;
static auto start = std::chrono::high_resolution_clock::now();
//android_app* g_App = nullptr;
android_app* g_App = 0;
ANativeWindow* ANativeWindow_p;
#include "源码UI编辑/UI/GUI.h"
#define SLEEP_TIME 1000LL / 120LL
#include <sys/system_properties.h>

static bool isRecording() {
    char prop[PROP_VALUE_MAX]{0};
    __system_property_get("sys.hwc.compose_policy", prop);
    return atoi(prop) == 2;  // 2 = 正在走录屏通道
}
int (*orig_onInputEvent)(struct android_app* app, AInputEvent* inputEvent);
int onInputEvent(struct android_app* app, AInputEvent* inputEvent) {
    if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_KEY) {
        int key = AKeyEvent_getKeyCode(inputEvent);
        if (key == AKEYCODE_VOLUME_UP && AKeyEvent_getAction(inputEvent) == AKEY_EVENT_ACTION_DOWN) {
            bShowGUI = !bShowGUI;
			}
    }
    GUI::onEvent(inputEvent, { (float)(screenSizeX) / (float)screenWidth, (float)(screenSizeY) / (float)screenHeight });
    return orig_onInputEvent(app, inputEvent);
}
TNameEntryArray* GetGNames()
{
	return ((TNameEntryArray * (*)()) (UE4 + GNames_Offset))();
}

std::string getClipboardText() {
	if (!g_App)
		return "";

	auto activity = g_App->activity;
	if (!activity)
		return "";

	auto vm = activity->vm;
	if (!vm)
		return "";

	auto object = activity->clazz;
	if (!object)
		return "";

	std::string result;

	JNIEnv* env;
	vm->AttachCurrentThread(&env, 0);
	{
		auto ContextClass = env->FindClass("android/content/Context");
		auto getSystemServiceMethod = env->GetMethodID(ContextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

		auto str = env->NewStringUTF("clipboard");
		auto clipboardManager = env->CallObjectMethod(object, getSystemServiceMethod, str);
		env->DeleteLocalRef(str);

		auto ClipboardManagerClass = env->FindClass("android/content/ClipboardManager");
		auto getText = env->GetMethodID(ClipboardManagerClass, "getText", "()Ljava/lang/CharSequence;");

		auto CharSequenceClass = env->FindClass("java/lang/CharSequence");
		auto toStringMethod = env->GetMethodID(CharSequenceClass, "toString", "()Ljava/lang/String;");

		auto text = env->CallObjectMethod(clipboardManager, getText);
		if (text) {
			str = (jstring)env->CallObjectMethod(text, toStringMethod);
			result = env->GetStringUTFChars(str, 0);
			env->DeleteLocalRef(str);
			env->DeleteLocalRef(text);
		}

		env->DeleteLocalRef(CharSequenceClass);
		env->DeleteLocalRef(ClipboardManagerClass);
		env->DeleteLocalRef(clipboardManager);
		env->DeleteLocalRef(ContextClass);
	}
	vm->DetachCurrentThread();

	return result;
}
const char* GetDeviceModel(JNIEnv* env) {
	jclass buildClass = env->FindClass(/*android/os/Build*/ StrEnc("m5I{GKGWBP-VOxkA", "\x0C\x5B\x2D\x09\x28\x22\x23\x78\x2D\x23\x02\x14\x3A\x11\x07\x25", 16).c_str());
	jfieldID modelId = env->GetStaticFieldID(buildClass, /*MODEL*/ StrEnc("|}[q:", "\x31\x32\x1F\x34\x76", 5).c_str(), /*Ljava/lang/String;*/ StrEnc(".D:C:ETZ1O-Ib&^h.Y", "\x62\x2E\x5B\x35\x5B\x6A\x38\x3B\x5F\x28\x02\x1A\x16\x54\x37\x06\x49\x62", 18).c_str());

	auto str = (jstring)env->GetStaticObjectField(buildClass, modelId);
	return env->GetStringUTFChars(str, 0);
}
const char* GetDeviceBrand(JNIEnv* env) {
	jclass buildClass = env->FindClass(/*android/os/Build*/ StrEnc("0iW=2^>0zTRB!B90", "\x51\x07\x33\x4F\x5D\x37\x5A\x1F\x15\x27\x7D\x00\x54\x2B\x55\x54", 16).c_str());
	jfieldID modelId = env->GetStaticFieldID(buildClass, /*BRAND*/ StrEnc("@{[FP", "\x02\x29\x1A\x08\x14", 5).c_str(), /*Ljava/lang/String;*/ StrEnc(".D:C:ETZ1O-Ib&^h.Y", "\x62\x2E\x5B\x35\x5B\x6A\x38\x3B\x5F\x28\x02\x1A\x16\x54\x37\x06\x49\x62", 18).c_str());

	auto str = (jstring)env->GetStaticObjectField(buildClass, modelId);
	return env->GetStringUTFChars(str, 0);
}
template<typename T>
void Write(uintptr_t addr, T value) {
 WriteAddr((void *) addr, &value, sizeof(T));
}
void 写入D类(uintptr_t addr,int var){
WriteAddr(reinterpret_cast<void*>(addr),reinterpret_cast<void*>(&var),4);
}
void MemoryD_type(uintptr_t addr,int var){
WriteAddr(reinterpret_cast<void*>(addr),reinterpret_cast<void*>(&var),4);
}
void 写入Q类(uintptr_t addr,int var){
WriteAddr(reinterpret_cast<void*>(addr),reinterpret_cast<void*>(&var),32);
}

template<typename T>
void 写入F类(uintptr_t addr, float var){
    WriteAddr(reinterpret_cast<void*>(addr),reinterpret_cast<void*>(&var),4);
}
const char* GetAndroidID(JNIEnv* env, jobject context) {
	jclass contextClass = env->FindClass(/*android/content/Context*/ StrEnc("`L+&0^[S+-:J^$,r9q92(as", "\x01\x22\x4F\x54\x5F\x37\x3F\x7C\x48\x42\x54\x3E\x3B\x4A\x58\x5D\x7A\x1E\x57\x46\x4D\x19\x07", 23).c_str());
	jmethodID getContentResolverMethod = env->GetMethodID(contextClass, /*getContentResolver*/ StrEnc("E8X\\7r7ys_Q%JS+L+~", "\x22\x5D\x2C\x1F\x58\x1C\x43\x1C\x1D\x2B\x03\x40\x39\x3C\x47\x3A\x4E\x0C", 18).c_str(), /*()Landroid/content/ContentResolver;*/ StrEnc("8^QKmj< }5D:9q7f.BXkef]A*GYLNg}B!/L", "\x10\x77\x1D\x2A\x03\x0E\x4E\x4F\x14\x51\x6B\x59\x56\x1F\x43\x03\x40\x36\x77\x28\x0A\x08\x29\x24\x44\x33\x0B\x29\x3D\x08\x11\x34\x44\x5D\x77", 35).c_str());
	jclass settingSecureClass = env->FindClass(/*android/provider/Settings$Secure*/ StrEnc("T1yw^BCF^af&dB_@Raf}\\FS,zT~L(3Z\"", "\x35\x5F\x1D\x05\x31\x2B\x27\x69\x2E\x13\x09\x50\x0D\x26\x3A\x32\x7D\x32\x03\x09\x28\x2F\x3D\x4B\x09\x70\x2D\x29\x4B\x46\x28\x47", 32).c_str());
	jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass, /*getString*/ StrEnc("e<F*J5c0Y", "\x02\x59\x32\x79\x3E\x47\x0A\x5E\x3E", 9).c_str(), /*(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;*/ StrEnc("$6*%R*!XO\"m18o,0S!*`uI$IW)l_/_knSdlRiO1T`2sH|Ouy__^}%Y)JsQ:-\"(2_^-$i{?H", "\x0C\x7A\x4B\x4B\x36\x58\x4E\x31\x2B\x0D\x0E\x5E\x56\x1B\x49\x5E\x27\x0E\x69\x0F\x1B\x3D\x41\x27\x23\x7B\x09\x2C\x40\x33\x1D\x0B\x21\x5F\x20\x38\x08\x39\x50\x7B\x0C\x53\x1D\x2F\x53\x1C\x01\x0B\x36\x31\x39\x46\x0C\x15\x43\x2B\x05\x30\x15\x41\x43\x46\x55\x70\x0D\x59\x56\x00\x15\x58\x73", 71).c_str());

	auto obj = env->CallObjectMethod(context, getContentResolverMethod);
	auto str = (jstring)env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj, env->NewStringUTF(/*android_id*/ StrEnc("ujHO)8OfOE", "\x14\x04\x2C\x3D\x46\x51\x2B\x39\x26\x21", 10).c_str()));
	return env->GetStringUTFChars(str, 0);
}

const char* GetDeviceUniqueIdentifier(JNIEnv* env, const char* uuid) {
	jclass uuidClass = env->FindClass(/*java/util/UUID*/ StrEnc("B/TxJ=3BZ_]SFx", "\x28\x4E\x22\x19\x65\x48\x47\x2B\x36\x70\x08\x06\x0F\x3C", 14).c_str());

	auto len = strlen(uuid);

	jbyteArray myJByteArray = env->NewByteArray(len);
	env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte*)uuid);

	jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass, /*nameUUIDFromBytes*/ StrEnc("P6LV|'0#A+zQmoat,", "\x3E\x57\x21\x33\x29\x72\x79\x67\x07\x59\x15\x3C\x2F\x16\x15\x11\x5F", 17).c_str(), /*([B)Ljava/util/UUID;*/ StrEnc("sW[\"Q[W3,7@H.vT0) xB", "\x5B\x0C\x19\x0B\x1D\x31\x36\x45\x4D\x18\x35\x3C\x47\x1A\x7B\x65\x7C\x69\x3C\x79", 20).c_str());
	jmethodID toStringMethod = env->GetMethodID(uuidClass, /*toString*/ StrEnc("2~5292eW", "\x46\x11\x66\x46\x4B\x5B\x0B\x30", 8).c_str(), /*()Ljava/lang/String;*/ StrEnc("P$BMc' #j?<:myTh_*h0", "\x78\x0D\x0E\x27\x02\x51\x41\x0C\x06\x5E\x52\x5D\x42\x2A\x20\x1A\x36\x44\x0F\x0B", 20).c_str());

	auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
	auto str = (jstring)env->CallObjectMethod(obj, toStringMethod);
	return env->GetStringUTFChars(str, 0);
}
size_t 美化皮肤(char *s) {
    static const std::unordered_map<std::string, std::string> replacements = {
      {"2200101", "12208701"}, //你好
{"2200301", "12212201"}, //鼓掌
{"2200201", "12200701"}, //谢谢
{"202408001", "202408053"},//大厅背景
{"40601001", "40605012"}, //发型A改双马尾
{"403003", "1404049"}, //T恤（条纹改）黄鲨鱼
{"404026", "1404050"}, //金典裤子改黄鲨鱼裤子
{"402019", "1403719"}, //放形眼镜改科技战衣眼罩
{"101001100", "1101001213"},//"AKM自动步枪（破损）"
{"101001200", "1101001213"},//"AKM自动步枪（修复）"
{"101001300", "1101001213"},//"AKM自动步枪（完好）"
{"101001400", "1101001213"},//"AKM自动步枪（改进）"
{"101001500", "1101001213"},//"AKM自动步枪（精制）"
{"101001600", "1101001213"},//"AKM自动步枪（独眼蛇）"
{"101001700", "1101001213"},//"AKM自动步枪（钢铁阵线）"
{"101002100", "1101002081"},//"M16A4自动步枪（破损）"
{"101002200", "1101002081"},//"M16A4自动步枪（修复）"
{"101002300", "1101002081"},//"M16A4自动步枪（完好）"
{"101002400", "1101002081"},//"M16A4自动步枪（改进）"
{"101002500", "1101002081"},//"M16A4自动步枪（精制）"
{"101003100", "1101003188"},//"SCAR-L自动步枪（破损）"
{"101003200", "1101003188"},//"SCAR-L自动步枪（修复）"
{"101003300", "1101003188"},//"SCAR-L自动步枪（完好）"
{"101003400", "1101003188"},//"SCAR-L自动步枪（改进）"
{"101003500", "1101003188"},//"SCAR-L自动步枪（精制）"
{"101003600", "1101003188"},//"SCAR-L自动步枪（独眼蛇）"
{"101003700", "11010031885"},//"SCAR-L自动步枪（钢铁阵线）"
{"101004100", "1101004236"},//"M416自动步枪（破损）"
{"101004200", "1101004236"},//"M416自动步枪（修复）"
{"101004300", "1101004236"},//"M416自动步枪（完好）"
{"101004400", "1101004236"},//"M416自动步枪（改进）"
{"101004500", "1101004236"},//"M416自动步枪（精制）"
{"101004600", "1101004236"},//"M416自动步枪（独眼蛇）"
{"101004700", "1101004236"},//"M416自动步枪（钢铁阵线）"
{"101005100", "10100500"},//"GROZA自动步枪（破损）"
{"101005200", "10100500"},//"GROZA自动步枪（修复）"
{"101005300", "10100500"},//"GROZA自动步枪（完好）"
{"101005400", "10100500"},//"GROZA自动步枪（改进）"
{"101005500", "10100500"},//"GROZA自动步枪（精制）"
{"101005600", "10100500"},//"GROZA自动步枪（独眼蛇）"
{"101005700", "10100500"},//"GROZA自动步枪（钢铁阵线）"
{"101006100", "10100600"},//"AUG自动步枪（破损）"
{"101006200", "10100600"},//"AUG自动步枪（修复）"
{"101006300", "10100600"},//"AUG自动步枪（完好）"
{"101006400", "10100600"},//"AUG自动步枪（改进）"
{"101006500", "10100600"},//"AUG自动步枪（精制）"
{"101006600", "10100600"},//"AUG自动步枪（独眼蛇）"
{"101006700", "10100600"},//"AUG自动步枪（钢铁阵线）"
{"101007100", "1101007025"},//"QBZ自动步枪（破损）"
{"101007200", "1101007025"},//"QBZ自动步枪（修复）"
{"101007300", "1101007025"},//"QBZ自动步枪（完好）"
{"101007400", "1101007025"},//"QBZ自动步枪（改进）"
{"101007500", "1101007025"},//"QBZ自动步枪（精制）"
{"101007600", "1101007025"},//"QBZ自动步枪（独眼蛇）"
{"101007700", "1101007025"},//"QBZ自动步枪（钢铁阵线）"
{"101008100", "10100800"},//"M762自动步枪（破损）"
{"101008200", "10100800"},//"M762自动步枪（修复）"
{"101008300", "10100800"},//"M762自动步枪（完好）"
{"101008400", "10100800"},//"M762自动步枪（改进）"
{"101008500", "10100800"},//"M762自动步枪（精制）"
{"101008600", "10100800"},//"M762自动步枪（独眼蛇）"
{"101008700", "10100800"},//"M762自动步枪（钢铁阵线）"
{"101008900", "10100800"},//"M762自动步枪"
{"101009100", "1101009003"},//"Mk47自动步枪（破损）"
{"101009200", "1101009003"},//"Mk47自动步枪（修复）"
{"101009300", "1101009003"},//"Mk47自动步枪（完好）"
{"101009400", "1101009003"},//"Mk47自动步枪（改进）"
{"101009500", "1101009003"},//"Mk47自动步枪（精制）"
{"101010100", "1101010012"},//"G36C自动步枪（破损）"
{"101010200", "1101010012"},//"G36C自动步枪（修复）"
{"101010300", "1101010012"},//"G36C自动步枪（完好）"
{"101010400", "1101010012"},//"G36C自动步枪（改进）"
{"101010500", "1101010012"},//"G36C自动步枪（精制）"
{"101010600", "1101010012"},//"G36C自动步枪（独眼蛇）"
{"101010700", "1101010012"},//"G36C自动步枪（钢铁阵线）"
{"101012100", "1101012013"},//"蜜獾自动步枪（破损）"
{"101012200", "1101012013"},//"蜜獾自动步枪（修复）"
{"101012300", "1101012013"},//"蜜獾自动步枪（完好）"
{"101012400", "1101012013"},//"蜜獾自动步枪（改进）"
{"101012500", "1101012013"},//"蜜獾自动步枪（精制）"
{"101012600", "1101012013"},//"蜜獾自动步枪（独眼蛇）"
{"101012700", "1101012013"},//"蜜獾自动步枪（钢铁阵线）"
{"101100100", "1101100012"},//"FAMAS自动步枪（破损）"
{"101100200", "1101100012"},//"FAMAS自动步枪（修复）"
{"101100300", "1101100012"},//"FAMAS自动步枪（完好）"
{"101100400", "1101100012"},//"FAMAS自动步枪（改进）"
{"101100500", "1101100012"},//"FAMAS自动步枪（精制）"
{"101100600", "1101100012"},//"FAMAS自动步枪（独眼蛇）"
{"101100700", "1101100012"},//"FAMAS自动步枪（钢铁阵线）"
{"101102100", "1101102027"},//"ACE32自动步枪（破损）"
{"101102200", "1101102027"},//"ACE32自动步枪（修复）"
{"101102300", "1101102027"},//"ACE32自动步枪（完好）"
{"101102400", "1101102027"},//"ACE32自动步枪（改进）"
{"101102500", "1101102027"},//"ACE32自动步枪（精制）"
{"101102600", "1101102027"},//"ACE32自动步枪（独眼蛇）"
{"101102700", "1101102027"},//"ACE32自动步枪（钢铁阵线）"
{"102001100", "1102001036"},//"UZI冲锋枪（破损）"
{"102001200", "1102001036"},//"UZI冲锋枪（修复）"
{"102001300", "1102001036"},//"UZI冲锋枪（完好）"
{"102001400", "1102001036"},//"UZI冲锋枪（改进）"
{"102001500", "1102001036"},//"UZI冲锋枪（精制）"
{"102002100", "1102002053"},//"UMP45冲锋枪（破损）"
{"102002200", "1102002053"},//"UMP45冲锋枪（修复）"
{"102002300", "1102002053"},//"UMP45冲锋枪（完好）"
{"102002400", "1102002053"},//"UMP45冲锋枪（改进）"
{"102002500", "1102002053"},//"UMP45冲锋枪（精制）"
{"102002900", "1102002053"},//"UMP45冲锋枪"
{"102003100", "1102003091"},//"Vector冲锋枪（破损）"
{"102003200", "1102003091"},//"Vector冲锋枪（修复）"
{"102003300", "1102003091"},//"Vector冲锋枪（完好）"
{"102003400", "1102003091"},//"Vector冲锋枪（改进）"
{"102003500", "1102003091"},//"Vector冲锋枪（精制）"
{"102003600", "1102003091"},//"Vector冲锋枪（独眼蛇）"
{"102003700", "1102003091"},//"Vector冲锋枪（钢铁阵线）"
{"102004100", "1102004018"},//"汤姆逊冲锋枪（破损）"
{"102004200", "1102004018"},//"汤姆逊冲锋枪（修复）"
{"102004300", "1102004018"},//"汤姆逊冲锋枪（完好）"
{"102004400", "1102004018"},//"汤姆逊冲锋枪（改进）"
{"102004500", "1102004018"},//"汤姆逊冲锋枪（精制）"
{"102005100", "1102005007"},//"野牛冲锋枪（破损）"
{"102005200", "1102005007"},//"野牛冲锋枪（修复）"
{"102005300", "1102005007"},//"野牛冲锋枪（完好）"
{"102005400", "1102005007"},//"野牛冲锋枪（改进）"
{"102005500", "1102005007"},//"野牛冲锋枪（精制）"
{"102005900", "1102005007"},//"野牛冲锋枪"
{"102007100", "1107001019"},//"MP5K冲锋枪（破损）"
{"102007200", "1107001019"},//"MP5K冲锋枪（修复）"
{"102007300", "1107001019"},//"MP5K冲锋枪（完好）"
{"102007400", "1107001019"},//"MP5K冲锋枪（改进）"
{"102007500", "1107001019"},//"MP5K冲锋枪（精制）"
{"102007600", "1107001019"},//"MP5K冲锋枪（独眼蛇）"
{"102007700", "1107001019"},//"MP5K冲锋枪（钢铁阵线）"
{"102105100", "1102105002"},//"P90冲锋枪（破损）"
{"102105200", "1102105002"},//"P90冲锋枪（修复）"
{"102105300", "1102105002"},//"P90冲锋枪（完好）"
{"102105400", "1102105002"},//"P90冲锋枪（改进）"
{"102105500", "1102105002"},//"P90冲锋枪（精制）"
{"102105600", "1102105002"},//"P90冲锋枪（独眼蛇）"
{"102105700", "1102105002"},//"P90冲锋枪（钢铁阵线）"
{"103001100", "1103001085"},//"Kar98K狙击枪（破损）"
{"103001200", "1103001085"},//"Kar98K狙击枪（修复）"
{"103001300", "1103001085"},//"Kar98K狙击枪（完好）"
{"103001400", "1103001085"},//"Kar98K狙击枪（改进）"
{"103001500", "1103001085"},//"Kar98K狙击枪（精制）"
{"103001900", "1103001085"},//"Kar98K狙击枪"
{"103002100", "1103002030"},//"M24狙击枪（破损）"
{"103002200", "1103002030"},//"M24狙击枪（修复）"
{"103002300", "1103002030"},//"M24狙击枪（完好）"
{"103002400", "1103002030"},//"M24狙击枪（改进）"
{"103002500", "1103002030"},//"M24狙击枪（精制）"
{"103002600", "1103002030"},//"M24狙击枪（独眼蛇）"
{"103002700", "1103002030"},//"M24狙击枪（钢铁阵线）"
{"103003100", "1103003087"},//"AWM狙击枪（破损）"
{"103003200", "1103003087"},//"AWM狙击枪（修复）"
{"103003300", "1103003087"},//"AWM狙击枪（完好）"
{"103003400", "1103003087"},//"AWM狙击枪（改进）"
{"103003500", "1103003087"},//"AWM狙击枪（精制）"
{"103003600", "1103003087"},//"AWM狙击枪（独眼蛇）"
{"103003700", "1103003087"},//"AWM狙击枪（钢铁阵线）"
{"103003900", "1103003087"},//"AWM狙击枪"
{"103004100", "1103004020"},//"SKS狙击枪（破损）"
{"103004200", "1103004020"},//"SKS狙击枪（修复）"
{"103004300", "1103004020"},//"SKS狙击枪（完好）"
{"103004400", "1103004020"},//"SKS狙击枪（改进）"
{"103004500", "1103004020"},//"SKS狙击枪（精制）"
{"103004600", "1103004020"},//"SKS狙击枪（独眼蛇）"
{"103004700", "1103004020"},//"SKS狙击枪（钢铁阵线）"
{"103005100", "1103005033"},//"VSS狙击枪（破损）"
{"103005200", "1103005033"},//"VSS狙击枪（修复）"
{"103005300", "1103005033"},//"VSS狙击枪（完好）"
{"103005400", "1103005033"},//"VSS狙击枪（改进）"
{"103005500", "1103005033"},//"VSS狙击枪（精制）"
{"103005900", "1103005033"},//"VSS狙击枪"
{"103006100", "1103006030"},//"Mini14狙击枪（破损）"
{"103006200", "1103006030"},//"Mini14狙击枪（修复）"
{"103006300", "1103006030"},//"Mini14狙击枪（完好）"
{"103006400", "1103006030"},//"Mini14狙击枪（改进）"
{"103006500", "1103006030"},//"Mini14狙击枪（精制）"
{"103006900", "1103006030"},//"Mini14狙击枪"
{"103007100", "1103007028"},//"Mk14狙击枪（破损）"
{"103007200", "1103007028"},//"Mk14狙击枪（修复）"
{"103007300", "1103007028"},//"Mk14狙击枪（完好）"
{"103007400", "1103007028"},//"Mk14狙击枪（改进）"
{"103007500", "1103007028"},//"Mk14狙击枪（精制）"
{"103007600", "1103007028"},//"Mk14狙击枪（独眼蛇）"
{"103007700", "1103007028"},//"Mk14狙击枪（钢铁阵线）"
{"103008100", "1103008014"},//"Win94狙击枪（破损）"
{"103008200", "1103008014"},//"Win94狙击枪（修复）"
{"103008300", "1103008014"},//"Win94狙击枪（完好）"
{"103008400", "1103008014"},//"Win94狙击枪（改进）"
{"103008500", "1103008014"},//"Win94狙击枪（精制）"
{"103009100", "1103009039"},//"SLR狙击枪（破损）"
{"103009200", "1103009039"},//"SLR狙击枪（修复）"
{"103009300", "1103009039"},//"SLR狙击枪（完好）"
{"103009400", "1103009039"},//"SLR狙击枪（改进）"
{"103009500", "1103009039"},//"SLR狙击枪（精制）"
{"103009600", "1103009039"},//"SLR狙击枪（独眼蛇）"
{"103009700", "1103009039"},//"SLR狙击枪（钢铁阵线）"
{"103009900", "1103009039"},//"SLR狙击枪"
{"103010100", "1103010018"},//"QBU狙击枪（破损）"
{"103010200", "1103010018"},//"QBU狙击枪（修复）"
{"103010300", "1103010018"},//"QBU狙击枪（完好）"
{"103010400", "1103010018"},//"QBU狙击枪（改进）"
{"103010500", "1103010018"},//"QBU狙击枪（精制）"
{"103012100", "1103012010"},//"AMR狙击枪（破损）"
{"103012200", "1103012010"},//"AMR狙击枪（修复）"
{"103012300", "1103012010"},//"AMR狙击枪（完好）"
{"103012400", "1103012010"},//"AMR狙击枪（改进）"
{"103012500", "1103012010"},//"AMR狙击枪（精制）"
{"103012600", "1103012010"},//"AMR狙击枪（独眼蛇）"
{"103012700", "1103012010"},//"AMR狙击枪（钢铁阵线）"
{"103100100", "1103100013"},//"Mk12狙击枪（破损）"
{"103100200", "1103100013"},//"Mk12狙击枪（修复）"
{"103100300", "1103100013"},//"Mk12狙击枪（完好）"
{"103100400", "1103100013"},//"Mk12狙击枪（改进）"
{"103100500", "1103100013"},//"Mk12狙击枪（精制）"
{"103100600", "1103100013"},//"Mk12狙击枪（独眼蛇）"
{"103100700", "1103100013"},//"Mk12狙击枪（钢铁阵线）"
{"104001100", "1104001030"},//"S686霰弹枪（破损）"
{"104001200", "1104001030"},//"S686霰弹枪（修复）"
{"104001300", "1104001030"},//"S686霰弹枪（完好）"
{"104001400", "1104001030"},//"S686霰弹枪（改进）"
{"104001500", "1104001030"},//"S686霰弹枪（精制）"
{"104002100", "1104002036"},//"S1897霰弹枪（破损）"
{"104002200", "1104002036"},//"S1897霰弹枪（修复）"
{"104002300", "1104002036"},//"S1897霰弹枪（完好）"
{"104002400", "1104002036"},//"S1897霰弹枪（改进）"
{"104002500", "1104002036"},//"S1897霰弹枪（精制）"
{"104003100", "1104003039"},//"S12K霰弹枪（破损）"
{"104003200", "1104003039"},//"S12K霰弹枪（修复）"
{"104003300", "1104003039"},//"S12K霰弹枪（完好）"
{"104003400", "1104003039"},//"S12K霰弹枪（改进）"
{"104003500", "1104003039"},//"S12K霰弹枪（精制）"
{"104003600", "1104003039"},//"S12K霰弹枪（独眼蛇）"
{"104003700", "1104003039"},//"S12K霰弹枪（钢铁阵线）"
{"104003900", "1104003039"},//"S12K霰弹枪"
{"104004100", "1104004024"},//"DBS霰弹枪（破损）"
{"104004200", "1104004024"},//"DBS霰弹枪（修复）"
{"104004300", "1104004024"},//"DBS霰弹枪（完好）"
{"104004400", "1104004024"},//"DBS霰弹枪（改进）"
{"104004500", "1104004024"},//"DBS霰弹枪（精制）"
{"104004600", "1104004024"},//"DBS霰弹枪（独眼蛇）"
{"104004700", "1104004024"},//"DBS霰弹枪（钢铁阵线）"
{"104004900", "1104004024"},//"DBS霰弹枪"
{"104102100", "1104102001"},//"NS2000霰弹枪（破损）"
{"104102200", "1104102001"},//"NS2000霰弹枪（修复）"
{"104102300", "1104102001"},//"NS2000霰弹枪（完好）"
{"104102400", "1104102001"},//"NS2000霰弹枪（改进）"
{"104102500", "1104102001"},//"NS2000霰弹枪（精制）"
{"105001100", "1105001020"},//"M249轻机枪（破损）"
{"105001200", "1105001020"},//"M249轻机枪（修复）"
{"105001300", "1105001020"},//"M249轻机枪（完好）"
{"105001400", "1105001020"},//"M249轻机枪（改进）"
{"105001500", "1105001020"},//"M249轻机枪（精制）"
{"105001600", "1105001020"},//"M249轻机枪（独眼蛇）"
{"105001700", "1105001020"},//"M249轻机枪（钢铁阵线）"
{"105002100", "1105002011"},//"DP-28轻机枪（破损）"
{"105002200", "1105002011"},//"DP-28轻机枪（修复）"
{"105002300", "1105002011"},//"DP-28轻机枪（完好）"
{"105002400", "1105002011"},//"DP-28轻机枪（改进）"
{"105002500", "1105002011"},//"DP-28轻机枪（精制）"
{"105002900", "1105002011"},//"DP-28轻机枪"
{"105010100", "1105010019"},//"MG3轻机枪（破损）"
{"105010200", "1105010019"},//"MG3轻机枪（修复）"
{"105010300", "1105010019"},//"MG3轻机枪（完好）"
{"105010400", "1105010019"},//"MG3轻机枪（改进）"
{"105010500", "1105010019"},//"MG3轻机枪（精制）"
{"105010600", "1105010019"},//"MG3轻机枪（独眼蛇）"
{"105010700", "1105010019"},//"MG3轻机枪（钢铁阵线）"
{"105010900", "1105010019"},//"MG3轻机枪"
{"106010100", "1106010002"},//"沙漠之鹰手枪（破损）"
{"106010200", "1106010002"},//"沙漠之鹰手枪（修复）"
{"106010300", "1106010002"},//"沙漠之鹰手枪（完好）"
{"106010400", "1106010002"},//"沙漠之鹰手枪（改进）"
{"106010500", "1106010002"},//"沙漠之鹰手枪（精制）"
{"106010900", "1106010002"},//"沙漠之鹰手枪（指挥官）"
{"107001100", "1107001011"},//"十字弩（破损）"
{"107001200", "1107001011"},//"十字弩（修复）"
{"107001300", "1107001011"},//"十字弩（完好）"
{"107001400", "1107001011"},//"十字弩（改进）"
{"107001500", "1107001011"},//"十字弩（精制）"
{"10100100", "1101001213"}, //
{"10100200", "1101002081"}, //
{"10100300", "1101003188"}, //
{"10100400", "1101004236"}, //
{"10100500", "1101005098"}, //
{"10100600", "1101006075"}, //
{"10100800", "1101008154"}, //
{"10100900", "1101009001"}, //
{"10101000", "1101010020"}, //
{"10101200", "1101100012"}, //
{"10110000", "1101102017"}, //
{"10110200", "1101102017"}, //
//突击步枪上面
{"10300100", "1103001191"}, //
{"10300200", "1103002156"}, //
{"10300300", "1103003062"}, //
{"10300800", "1103008020"}, //
{"10301100", "1103011001"}, //
{"10301200", "1103012031"}, //
//狙击枪上面
{"10300400", "1103004037"}, //
{"10300500", "1103005033"}, //
{"10300600", "1103006030"}, //
{"10300700", "1103007028"}, //
{"10300900", "1103009037"}, //
{"10301000", "1103010015"}, //
{"10310000", "1103100007"}, //
//射手枪
{"10200100", "1102001120"}, //
{"10200200", "1102002424"}, //
{"10200300", "1102003080"}, //
{"10200400", "1102004049"}, //
{"10200500", "1102005064"}, //
{"10200700", "1107001019"}, //
{"10210500", "1102105012"}, //
//冲锋枪上面
{"10400100", "1104001030"}, //
{"10400200", "1104002038"}, //
{"10400300", "1104003038"}, //
{"10400400", "1104004015"}, //
{"10410100", "1104101001"}, //
{"10410200", "1104102004"}, //
//喷子上面
{"10500100", "1105001069"}, //
{"10500200", "1105002091"}, //
{"10501000", "1105010019"}, //
//轻机枪上面
{"403192", "1406891"}, //
{"1405884", "1400687"}, //
{"1405548", "1406387"}, //
//身法下面
{"1400356", "1404133"}, //
{"404072", "1404134"}, //
{"401020", "1410085"}, //
{"402021", "1400165"}, //
//下面背景
{"20300100", "1030070218"}, //妹控8x 狙击通用
{"20300200", "1030070217"}, //妹控8x 狙击通用
{"20300300", "1030070216"}, //妹控8x 狙击通用
{"20300400", "1030070214"}, //妹控8x 狙击通用
{"20300500", "1030070212"}, //妹控8x 狙击通用
{"20301500", "1030070213"}, //封印幽冥 6x
{"20301400", "1030070215"}, //封印幽冥 3x
//不知道
{"1080051", "1081201"}, // 惩戒者雷呜
{"1080041", "1081201"},
{"1080011", "1081201"},
{"108004100", "108120100"},//惩戒者·雷鸣
{"10800100", "108120100"},//惩戒者·雷鸣
{"10800200", "108120100"},//惩戒者·雷鸣
{"10800300", "108120100"},//惩戒者·雷鸣
{"10800400", "108120100"},//惩戒者·雷鸣
{"10800500", "108120100"},//惩戒者·雷鸣
{"9002001","9002002"}, // 测试播报
{"2200201", "12220436"},
{"1010010233","1030070276"},// 盒子	
{"1010010233","1030070286"},// 盒子
        

    };

auto it = replacements.find(s);
    if (it != replacements.end()) {
        strcpy(s, it->second.c_str()); // 确保 s 有足够的空间
        return strlen(s);
    }
    return strlen(s); // 如果没有替换，返回原字符串长度
}
#define COLOR_BLACK FLinearColor(0, 0, 0, 1.f)
#define COLOR_WHITE FLinearColor(1.f, 1.f, 1.f, 1.f)
#define COLOR_RED FLinearColor(1.f, 0, 0, 1.f)
#define COLOR_CAR FLinearColor(1.f, 0.5f, 1.f, 1.f)
#define COLOR_GREEN FLinearColor(0, 0.5f, 0, 1.f)
#define COLOR_ORANGE FLinearColor(1.f, 0.4f, 0, 1.f)
#define COLOR_YELLOW FLinearColor(1.f, 1.f, 0, 1.f)
#define COLOR_LIME FLinearColor(0, 1.f, 0, 1.f)
#define COLOR_BLUE FLinearColor(0, 0, 1.f, 1.f)
#define COLOR_THISTLE FLinearColor(1.0f, 0.74f, 0.84f, 1.0f)
#define COLOR_PINK FLinearColor(1.0f, 0.75f, 0.8f, 1.0f)

#define 黑色 FLinearColor(0, 0, 0, 1.f)
#define 白色 FLinearColor(1.f, 1.f, 1.f, 1.f)
#define 红色 FLinearColor(1.f, 0, 0, 1.f)
#define 空白 FLinearColor(0, 0, 0, 0)
#define 蓝色 FLinearColor(0, 0, 1.f, 1.f)
#define 粉色 FLinearColor(1.f, 0.5f, 1.f, 1.f)
#define 红色 FLinearColor(1.f, 0, 0, 1.0f)
#define 绿色 FLinearColor(0, 0.5f, 0, 1.f)
#define 灰色 FLinearColor(0.5f, 0.5f, 0.5f, 1.f)
#define 橙色 FLinearColor(0, 1.f, 0, 1.f)
#define 黄色 FLinearColor(1.f, 1.f, 0, 1.f)
#define 深红 FLinearColor(0.5f, 0, 0, 1.f)
#define 透白 FLinearColor(1.f, 1.f, 1.f, 0.5f)

bool W2S2(FVector worldPos, FVector2D *screenPos) {
return g_PlayerController->ProjectWorldLocationToScreen(worldPos, true, screenPos);
}

void Line(void* Canvas,FVector2D origin, FVector2D dest, FLinearColor Color)
{
GUI::Draw_Line({origin.X, origin.Y}, {dest.X, dest.Y}, 1.0f, Color);
 }

void Box3D(void* Canvas, FVector origin, FVector extends, FLinearColor Color) {
origin.X -= extends.X / 2.f;
origin.Y -= extends.Y / 2.f;
origin.Z -= extends.Z / 2.f;


 FVector one = origin;
 FVector two = origin; two.X += extends.X;
 FVector tree = origin; tree.X += extends.X; tree.Y += extends.Y;
 FVector four = origin; four.Y += extends.Y;

 FVector five = one; five.Z += extends.Z;
 FVector six = two; six.Z += extends.Z;
 FVector seven = tree; seven.Z += extends.Z;
 FVector eight = four; eight.Z += extends.Z;

FVector2D s1, s2, s3, s4, s5, s6, s7, s8;
if (W2S2(one, &s1) && W2S2(two, &s2) && W2S2(tree, &s3) && W2S2(four, &s4) &&
W2S2(five, &s5) && W2S2(six, &s6) && W2S2(seven, &s7) && W2S2(eight, &s8))
{

Line(Canvas,s1, s2, Color);
Line(Canvas,s2, s3, Color);
Line(Canvas,s3, s4, Color);
Line(Canvas,s4, s1, Color);
 
Line(Canvas,s5, s6, Color);
Line(Canvas,s6, s7, Color);
Line(Canvas,s7, s8, Color);
Line(Canvas,s8, s5, Color);
 
Line(Canvas,s1, s5, Color);
Line(Canvas,s2, s6, Color);
Line(Canvas,s3, s7, Color);
Line(Canvas,s4, s8, Color);

}}
char* 提示 = "";
static bool 错误提示 = true;
char* 公告 = httppost(OBFUSCATE("w.t3data.net"), 公告调用码, OBFUSCATE("758F072A3D3FC0C7"));
std::string Login(const char* user_key) {
    // ============= 【本地白名单】 =============
    if (strcmp(user_key, "HASREUMODxBHF9uKXAhkSpM") == 0) {
        static char sj[13] = {0};
        获取时间戳(sj);

        char fake[512] = {0};
        sprintf(fake,
                OBFUSCATE("登录成功:200;卡密ID:本地白名单;到期时间:2025-08-31 23:59:59;当前时间戳:%s;当前时间:%s;效验密钥:本地密钥;"),
                sj, sj);

        // 复用原解析逻辑
        char* 卡密ID = strstrstr(fake, OBFUSCATE("卡密ID:"), OBFUSCATE(";"));
        到期时间     = strstrstr(fake, OBFUSCATE("到期时间:"), OBFUSCATE(";"));
        char* 当前时间戳 = strstrstr(fake, OBFUSCATE("当前时间戳:"), OBFUSCATE(";"));
        char* 当前时间    = strstrstr(fake, OBFUSCATE("当前时间:"), OBFUSCATE(";"));

        long vs1 = atoll(sj);
        long vs2 = (当前时间) ? atoll(当前时间) : 0;
        if ((vs1 - vs2) < 18) {
            isLogin = false;
        } else {
            错误提示 = false;
            提示 = OBFUSCATE("时间戳效验失败");
        }

        错误提示 = false;
        提示 = OBFUSCATE("白名单登录成功");
        return OBFUSCATE("OK");
    }
    // =========================================

    // ============= 【原 T3 逻辑】 =============
    if (!g_App)
        return OBFUSCATE("内部错误");
    auto activity = g_App->activity;
    if (!activity)
        return OBFUSCATE("内部错误");

    auto vm = activity->vm;
    if (!vm)
        return OBFUSCATE("内部错误");

    auto object = activity->clazz;
    if (!object)
        return OBFUSCATE("内部错误");

    JNIEnv* env;
    vm->AttachCurrentThread(&env, 0);
    std::string hwid = user_key;
    hwid += GetAndroidID(env, object);
    hwid += GetDeviceModel(env);
    hwid += GetDeviceBrand(env);
    std::string UUID = getHEX(加64(GetAndroidID(env, object)));
    vm->DetachCurrentThread();

    // 时间戳效验
    static char sj[13];
    获取时间戳(sj);
    std::string sjc = getHEX(加64(sj));

    // 单码签名校验
    char sign2[256];
    sprintf(sign2, OBFUSCATE("kami=%s&imei=%s&t=%s&%s"), getHEX(加64(user_key)), UUID.c_str(), sjc.c_str(), appkey);
    char sign2md5[32] = { 0 };
    获取MD5(sign2, sign2md5);
    std::string sign22 = getHEX(加64(sign2md5));

    char 整[4096];
    sprintf(整, OBFUSCATE("kami=%s&imei=%s&t=%s&s=%s"), getHEX(加64(user_key)), UUID.c_str(), sjc.c_str(), sign22.c_str());
    char* 内容 = NULL;
    char* 数据 = httppost(OBFUSCATE("w.t3data.net"), 登录调用码, 整);
    内容 = 解64(数据);
    if (strncmp(内容, OBFUSCATE("登录成功:200;"), 12) == 0) {
        char* 卡密ID = strstrstr(内容, OBFUSCATE("卡密ID:"), OBFUSCATE(";"));
        到期时间 = strstrstr(内容, OBFUSCATE("到期时间:"), OBFUSCATE(";"));
        char* 到期时间 = strstrstr(内容, OBFUSCATE("当前时间戳:"), OBFUSCATE(";"));
        char* 当前时间 = strstrstr(内容, OBFUSCATE("当前时间:"), OBFUSCATE(";"));
        char* 效验密钥 = strstrstr(内容, OBFUSCATE("效验密钥"), OBFUSCATE(";"));
        long vs1 = (long)atoll(sj);
        long vs2 = (long)atoll(当前时间);
        if ((vs1 - vs2) < 18) {
            isLogin = false;
        } else {
            错误提示 = false;
            提示 = OBFUSCATE("时间戳效验失败");
        }
    } else {
        错误提示 = false;
        提示 = 内容;
    }
    return OBFUSCATE("OK");
}

void* LoadFont(void*)
{
	while (!tslFont || !robotoFont)
	{
		tslFont = UObject::FindObject<UFont>(OBFUSCATE("Font TSLFont.TSLFont"));
		robotoFont = UObject::FindObject<UFont>(OBFUSCATE("Font Roboto.Roboto"));
		sleep(1);
	}
	return 0;
}

bool isInsideFOV(int x, int y) {
	int circle_x = screenWidth / 2;
	int circle_y = screenHeight / 2;
	int rad = (int)Settings.Aim.fovsize;
	return (x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad;
}

#define W2S(w, s) UGameplayStatics::ProjectWorldToScreen(g_PlayerController, w, true, s)


auto GetAimTargetPos()
{
	ASTExtraPlayerCharacter* result = 0;
	float max = std::numeric_limits<float>::infinity();
	float tempDis = FLT_MAX;
	FVector TempleLoction;
	auto Actors = GetActors();
	auto localPlayer = g_LocalPlayer;
	auto localPlayerController = g_PlayerController;
	if (localPlayer)
	{
		std::vector<ASTExtraPlayerCharacter*> PlayerCharacter;
		GetAllActors(PlayerCharacter);
		for (auto actor = PlayerCharacter.begin(); actor != PlayerCharacter.end(); actor++)
		{
			auto Player = *actor;

			if (Player->PlayerKey == localPlayer->PlayerKey)
				continue;
			if (Player->TeamID == localPlayer->TeamID)
				continue;
			if (Player->bDead)
				continue;
			if (Player->bHidden)
				continue;

			if (!Player->RootComponent)
				continue;

			if (Settings.Aim.vischeck) {
				// 原来只有：
// if (!localPlayerController->LineOfSightTo(Player, { 0, 0, 0 }, true))

// 现在换成 20 条骨骼的写法
if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("Head",          {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("neck_01",       {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("spine_03",      {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("spine_02",      {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("spine_01",      {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("pelvis",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("upperarm_r",    {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("lowerarm_r",    {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("hand_r",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("upperarm_l",    {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("lowerarm_l",    {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("hand_l",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("thigh_r",       {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("calf_r",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("foot_r",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("thigh_l",       {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("calf_l",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("foot_l",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("Root",          {0, 0, 0}), false))
					continue;
			}

			if (Settings.Aim.knocked) {
				if (Player->Health == 0.0f)
					continue;
			}

			if (Settings.Aim.nobot) {
				if (Player->bIsAI)
					continue;
			}
			auto TargetBoneMesh = Player->Mesh;
			FName BoneNameList[20] = {
			"Head",           // 头部
			"neck_01",        // 颈部_01
			"spine_03",       // 脊柱_03
			"spine_02",       // 脊柱_02
			"spine_01",       // 脊柱_01
			"pelvis",         // 骨盆
			"upperarm_r",     // 右上臂
			"lowerarm_r",     // 右前臂
			"hand_r",         // 右手
			"upperarm_l",     // 左上臂
			"lowerarm_l",     // 左前臂
			"hand_l",         // 左手
			"thigh_r",        // 右大腿
			"calf_r",         // 右小腿
			"foot_r",         // 右脚
			"thigh_l",        // 左大腿
			"calf_l",         // 左小腿
			"foot_l",         // 左脚
			"Root",           // 根部
			};
			FVector2D BonePos2D;
			for (int i = 0; i < 20; i++)
			{
				if (TargetBoneMesh)
				{
					if (BoneNameList[i].GetName())
					{
						FVector BonePos = TargetBoneMesh->GetSocketLocation(BoneNameList[i]);
						if (localPlayerController)
						{
							if (W2S(BonePos, &BonePos2D))
							{
								if (localPlayerController && localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, BonePos, true))
								{
									float currXdis = abs(screenWidth / 2 - BonePos2D.X);
									float currYdis = abs(screenHeight / 2 - BonePos2D.Y);
									if (currXdis < Settings.Aim.fovsize && currYdis < Settings.Aim.fovsize)
									{
										float currDis = (currXdis + currYdis);
										if (currDis < tempDis)
										{
											tempDis = currDis;
											TempleLoction = BonePos;
										}
									}
								}
							}
						}
					}
				}
			}
			return TempleLoction;
		}
	}
}

auto GetTargetForAim()
{
	ASTExtraPlayerCharacter* result = 0;
	float max = std::numeric_limits<float>::infinity();
	auto Actors = GetActors();
	auto localPlayer = g_LocalPlayer;
	auto localPlayerController = g_PlayerController;
	if (localPlayer)
	{
		std::vector<ASTExtraPlayerCharacter*> PlayerCharacter;
		GetAllActors(PlayerCharacter);
		for (auto actor = PlayerCharacter.begin(); actor != PlayerCharacter.end(); actor++)
		{
			auto Player = *actor;
float dist = localPlayer->GetDistanceTo(Player) / 100.0f;
//自瞄距离
if (dist > 自瞄距离)
continue;
			if (Player->PlayerKey == localPlayer->PlayerKey)
				continue;
			if (Player->TeamID == localPlayer->TeamID)
				continue;
			if (Player->bDead)
				continue;
			if (Player->bHidden)
				continue;

			if (!Player->RootComponent)
				continue;

			if (Settings.Aim.vischeck) {
				// 原来只有：
// if (!localPlayerController->LineOfSightTo(Player, { 0, 0, 0 }, true))

// 现在换成 20 条骨骼的写法
if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("Head",          {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("neck_01",       {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("spine_03",      {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("spine_02",      {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("spine_01",      {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("pelvis",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("upperarm_r",    {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("lowerarm_r",    {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("hand_r",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("upperarm_l",    {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("lowerarm_l",    {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("hand_l",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("thigh_r",       {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("calf_r",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("foot_r",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("thigh_l",       {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("calf_l",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("foot_l",        {0, 0, 0}), false) &&
    !localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Player->GetBonePos("Root",          {0, 0, 0}), false))
					continue;
			}

			if (Settings.Aim.knocked) {
				if (Player->Health == 0.0f)
					continue;
			}

			if (Settings.Aim.nobot) {
				if (Player->bEnsure)
					continue;
			}

			if (Settings.Aim.aim || Settings.Aim.silent) {

				auto HeadPos = Player->GetBonePos("Head", {});
				auto RootPos = Player->GetBonePos("Root", {});
				auto upper_r = Player->GetBonePos("upperarm_r", {});
				auto lowerarm_r = Player->GetBonePos("lowerarm_r", {});
				auto hand_r = Player->GetBonePos("hand_r", {});
				auto upper_l = Player->GetBonePos("upperarm_l", {});
				auto lowerarm_l = Player->GetBonePos("lowerarm_l", {});
				auto hand_l = Player->GetBonePos("hand_l", {});
				auto thigh_l = Player->GetBonePos("thigh_l", {});
				auto calf_l = Player->GetBonePos("calf_l", {});
				auto foot_l = Player->GetBonePos("foot_l", {});
				auto thigh_r = Player->GetBonePos("thigh_r", {});
				auto calf_r = Player->GetBonePos("calf_r", {});
				auto foot_r = Player->GetBonePos("foot_r", {});
				auto neck_01 = Player->GetBonePos("neck_01", {});
				auto Pelvis = Player->GetBonePos("pelvis", {});

				FVector2D RootPosSC, HeadPosSC, upper_rPoSC, upper_lPoSC, lowerarm_rPoSC, hand_rPoSC, lowerarm_lSC, hand_lPoSC, thigh_lPoSC, calf_lPoSC, foot_lPoSC, thigh_rPoSC, calf_rPoSC, foot_rPoSC, neck_01PoSC, PelvisPoSC;
				if (W2S(HeadPos, &HeadPosSC) && W2S(upper_r, &upper_rPoSC) &&
					W2S(upper_l, &upper_lPoSC) && W2S(lowerarm_r, &lowerarm_rPoSC) &&
					W2S(hand_r, &hand_rPoSC) && W2S(lowerarm_l, &lowerarm_lSC) &&
					W2S(hand_l, &hand_lPoSC) && W2S(thigh_l, &thigh_lPoSC) &&
					W2S(calf_l, &calf_lPoSC) && W2S(foot_l, &foot_lPoSC) &&
					W2S(thigh_r, &thigh_rPoSC) && W2S(calf_r, &calf_rPoSC) &&
					W2S(foot_r, &foot_rPoSC) && W2S(neck_01, &neck_01PoSC) &&
					W2S(Pelvis, &PelvisPoSC) && W2S(RootPos, &RootPosSC)) {

					static bool 已选择 = false;
					算法 = 0;
					已选择 = false;
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, neck_01, false))
					{
						is脖子 = false;
					}
					else {
						is脖子 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, lowerarm_r, false)) {
						is右小臂 = false;
					}
					else {
						is右小臂 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, lowerarm_l, false)) {
						is左小臂 = false;
					}
					else {
						is左小臂 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, thigh_l, false)) {
						is左大腿 = false;
					}
					else {
						is左大腿 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, calf_l, false)) {
						is左小腿 = false;
					}
					else {
						is左小腿 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, foot_l, false)) {
						is左脚 = false;
					}
					else {
						is左脚 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, thigh_r, false)) {
						is右大腿 = false;
					}
					else {
						is右大腿 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, calf_r, false)) {
						is右小腿 = false;
					}
					else {
						is右小腿 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, foot_r, false)) {
						is右脚 = false;
					}
					else {
						is右脚 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, Pelvis, false)) {
						is盆骨 = false;
					}
					else {
						is盆骨 = true;
					}
					if (!localPlayerController->LineOfSightTo(localPlayerController->PlayerCameraManager, HeadPos, false)) {
						is头 = false;
					}
					else {
						is头 = true;
					}
					if (!已选择)
						if (is头) {
							算法 = 1;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is盆骨) {
							算法 = 2;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is左脚) {
							算法 = 3;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is右脚) {
							算法 = 4;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is右小腿) {
							算法 = 5;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is左小腿) {
							算法 = 6;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is右小臂) {
							算法 = 7;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is左小臂) {
							算法 = 8;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is右大腿) {
							算法 = 9;
							已选择 = true;
						}
						else {
							已选择 = false;
						}
					if (!已选择)
						if (is左大腿) {
							算法 = 10;
							已选择 = true;
						}
						else {
							已选择 = false;
						}

					auto Root = Player->GetBonePos("Root", {});
					auto Head = Player->GetBonePos("Head", {});

					FVector2D RootPosSC, HeadSc;
					if (W2S(Root, &RootPosSC) && W2S(Head, &HeadSc))
					{
						float height = abs(HeadSc.Y - RootPosSC.Y);
						float width = height * 0.65;//0.65
						FVector middlePoint = { HeadSc.X + (width / 2), HeadSc.Y + (height / 2),0 };
						if ((middlePoint.X >= 0 && middlePoint.X <= screenWidth) && (middlePoint.Y >= 0 && middlePoint.Y <= screenHeight))
						{
							FVector2D v2Middle = FVector2D((float)(screenWidth / 2), (float)(screenHeight / 2));
							FVector2D v2Loc = FVector2D(middlePoint.X, middlePoint.Y);
							if (isInsideFOV((int)middlePoint.X, (int)middlePoint.Y)) {
								float dist = FVector2D::Distance(v2Middle, v2Loc);
								if (dist < max) {
									max = dist;
									result = Player;
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}
FRotator ToRotator1(FVector start, FVector target) {
FVector direction = target - start;
float hyp = sqrt(direction.X * direction.X + direction.Y * direction.Y);
FRotator newViewAngle;
newViewAngle.Pitch = -atan2(direction.Z, hyp) * (180.f / M_PI);
newViewAngle.Yaw = atan2(direction.Y, direction.X) * (180.f / M_PI);
newViewAngle.Roll = 0.0f;
return newViewAngle;
}

FQuat RotatorToQuat(FRotator rotator) {
// Convert degrees to radians
float DEG_TO_RAD = (float)(3.14159265358979323846 / 180.f);

float pitch = rotator.Pitch * DEG_TO_RAD * 0.5f;
float yaw = rotator.Yaw * DEG_TO_RAD * 0.5f;
float roll = rotator.Roll * DEG_TO_RAD * 0.5f;

float sinPitch = sin(pitch);
float cosPitch = cos(pitch);
float sinYaw = sin(yaw);
float cosYaw = cos(yaw);
float sinRoll = sin(roll);
float cosRoll = cos(roll);

FQuat quat;
quat.X = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
quat.Y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
quat.Z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
quat.W = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

return quat;
}
void (*orig_小奈追踪)(USTExtraShootWeaponComponent *a1 , FVector TargetLoc ,FVector* start, FRotator* BulletRot , FVector* BulletDir);
void 小奈追踪(USTExtraShootWeaponComponent *a1 , FVector TargetLoc ,FVector* start, FRotator* BulletRot , FVector* BulletDir){
if (Settings.Aim.silent){
			ASTExtraPlayerCharacter* Target = GetTargetForAim();
			if (Target)
					{
                bool triggerOk = false;
                     if (Settings.Aim.trigger == 0) {
            triggerOk = g_LocalPlayer->bIsWeaponFiring;
            }else
            if (Settings.Aim.trigger == 1){
            triggerOk = g_LocalPlayer->bIsGunADS;
            }else
			if (Settings.Aim.trigger == 2){	
			triggerOk = g_LocalPlayer->bIsWeaponFiring || g_LocalPlayer->bIsGunADS;
			}
            
						if (triggerOk)
						{
						FVector targetAimPos;
if (Settings.Aim.location == 0) {
if (算法 == 1) {
    targetAimPos = Target->GetBonePos("Head", {});
}
else if (算法 == 2) {
    targetAimPos = Target->GetBonePos("pelvis", {});   // 骨盆（原注释写错）
}
else if (算法 == 3) {
    targetAimPos = Target->GetBonePos("foot_l", {});   // 左脚
}
else if (算法 == 4) {
    targetAimPos = Target->GetBonePos("foot_r", {});   // 右脚
}
else if (算法 == 5) {
    targetAimPos = Target->GetBonePos("calf_r", {});   // 右小腿
}
else if (算法 == 6) {
    targetAimPos = Target->GetBonePos("calf_l", {});   // 左小腿
}
else if (算法 == 7) {
    targetAimPos = Target->GetBonePos("lowerarm_r", {}); // 右小臂
}
else if (算法 == 8) {
    targetAimPos = Target->GetBonePos("lowerarm_l", {}); // 左小臂
}
else if (算法 == 9) {
    targetAimPos = Target->GetBonePos("thigh_r", {});  // 右大腿
}
else if (算法 == 10) {
    targetAimPos = Target->GetBonePos("thigh_l", {});  // 左大腿
}

/* === 以下为新补充，按“头→脚”顺序编号 === */

else if (算法 == 11) {
    targetAimPos = Target->GetBonePos("neck_01", {});  // 脖子
}
else if (算法 == 12) {
    targetAimPos = Target->GetBonePos("spine_03", {}); // 上脊柱（胸腔）
}
else if (算法 == 13) {
    targetAimPos = Target->GetBonePos("spine_02", {}); // 中脊柱
}
else if (算法 == 14) {
    targetAimPos = Target->GetBonePos("spine_01", {}); // 下脊柱
}
else if (算法 == 15) {
    targetAimPos = Target->GetBonePos("Root", {});     // 根骨骼（臀部中心）
}
else if (算法 == 16) {
    targetAimPos = Target->GetBonePos("upperarm_l", {}); // 左上臂
}
else if (算法 == 17) {
    targetAimPos = Target->GetBonePos("upperarm_r", {}); // 右上臂
}
else if (算法 == 18) {
    targetAimPos = Target->GetBonePos("hand_l", {});   // 左手
}
else if (算法 == 19) {
    targetAimPos = Target->GetBonePos("hand_r", {});   // 右手
}
else {
    targetAimPos = Target->GetBonePos("Head", {});     // 默认仍打头
}
}else
if(Settings.Aim.location == 1){ 
if (算法 == 1) {
    targetAimPos = Target->GetBonePos("Head", {});          // 头
}
else if (算法 == 2) {
    targetAimPos = Target->GetBonePos("pelvis", {});        // 骨盆
}
else if (算法 == 3) {
    targetAimPos = Target->GetBonePos("foot_l", {});        // 左脚
}
else if (算法 == 4) {
    targetAimPos = Target->GetBonePos("foot_r", {});        // 右脚
}
else if (算法 == 5) {
    targetAimPos = Target->GetBonePos("calf_r", {});        // 右小腿
}
else if (算法 == 6) {
    targetAimPos = Target->GetBonePos("calf_l", {});        // 左小腿
}
else if (算法 == 7) {
    targetAimPos = Target->GetBonePos("lowerarm_r", {});    // 右小臂
}
else if (算法 == 8) {
    targetAimPos = Target->GetBonePos("lowerarm_l", {});    // 左小臂
}
else if (算法 == 9) {
    targetAimPos = Target->GetBonePos("thigh_r", {});       // 右大腿
}
else if (算法 == 10) {
    targetAimPos = Target->GetBonePos("thigh_l", {});       // 左大腿
}
else if (算法 == 11) {
    targetAimPos = Target->GetBonePos("neck_01", {});       // 脖子
}
else if (算法 == 12) {
    targetAimPos = Target->GetBonePos("spine_03", {});      // 上脊柱（胸腔）
}
else if (算法 == 13) {
    targetAimPos = Target->GetBonePos("spine_02", {});      // 中脊柱（身体）
}
else if (算法 == 14) {
    targetAimPos = Target->GetBonePos("spine_01", {});      // 下脊柱
}
else if (算法 == 15) {
    targetAimPos = Target->GetBonePos("Root", {});          // 根骨骼（臀中心）
}
else if (算法 == 16) {
    targetAimPos = Target->GetBonePos("upperarm_l", {});    // 左上臂
}
else if (算法 == 17) {
    targetAimPos = Target->GetBonePos("upperarm_r", {});    // 右上臂
}
else if (算法 == 18) {
    targetAimPos = Target->GetBonePos("hand_l", {});        // 左手
}
else if (算法 == 19) {
    targetAimPos = Target->GetBonePos("hand_r", {});        // 右手
}
else if (算法 == 20) {
    targetAimPos = Target->GetBonePos("spine_02", {});      // 身体（胸部中央）
}
else {
    targetAimPos = Target->GetBonePos("spine_02", {});          // 默认：头
}

}
UShootWeaponEntity *ShootWeaponEntityComponent = a1->ShootWeaponEntityComponent;
if (ShootWeaponEntityComponent) {
ASTExtraVehicleBase *CurrentVehicle = Target->CurrentVehicle;
if (CurrentVehicle) {
FVector LinearVelocity = CurrentVehicle->ReplicatedMovement.LinearVelocity;

float dist = g_LocalPlayer->GetDistanceTo(Target);
auto timeToTravel = dist / ShootWeaponEntityComponent->BulletFireSpeed;

targetAimPos = UKismetMathLibrary::Add_VectorVector(targetAimPos, UKismetMathLibrary::Multiply_VectorFloat(LinearVelocity, timeToTravel));
} else {
FVector Velocity = Target->GetVelocity();

float dist = g_LocalPlayer->GetDistanceTo(Target);
auto timeToTravel = dist / ShootWeaponEntityComponent->BulletFireSpeed;

targetAimPos = UKismetMathLibrary::Add_VectorVector(targetAimPos, UKismetMathLibrary::Multiply_VectorFloat(Velocity, timeToTravel));
}
FVector fDir = UKismetMathLibrary::Subtract_VectorVector(targetAimPos, *start);
FRotator sex = UKismetMathLibrary::Conv_VectorToRotator(fDir);
BulletRot->Pitch = sex.Pitch;
BulletRot->Yaw = sex.Yaw;
					}
			}
		}
		}
orig_小奈追踪(a1, TargetLoc, start, BulletRot, BulletDir);
}
void* (*orig_PostRender)(UGameViewportClient* ViewportClient, UCanvas* Canvas);
void* new_PostRender(UGameViewportClient* ViewportClient, UCanvas* Canvas) 

{
	// 防录屏：录屏时跳过所有自定义绘制
if (isRecording()) {
    return orig_PostRender(ViewportClient, Canvas);
}
	//if (!bShowOverlay) return orig_PostRender(ViewportClient, Canvas);   // 直接退出，不再绘制
	ASTExtraPlayerCharacter* localPlayer = 0;
	ASTExtraPlayerController* localPlayerController = 0;
	
	screenSizeX = ANativeWindow_getWidth(g_App->window);
	screenSizeY = ANativeWindow_getHeight(g_App->window);

    screenWidth = Canvas->SizeX;
	screenHeight = Canvas->SizeY;
	if (Canvas) {
}

	if (Canvas) {
		auto Actors = GetActors();
		UGameplayStatics* gGameplayStatics = (UGameplayStatics*)UGameplayStatics::StaticClass();
		auto GWorld = GetWorld();
		if (GWorld)
		{
			UNetDriver* NetDriver = GWorld->NetDriver;
			if (NetDriver)
			{
				UNetConnection* ServerConnection = NetDriver->ServerConnection;
				if (ServerConnection)
				{
					localPlayerController = (ASTExtraPlayerController*)ServerConnection->PlayerController;
				}
			}
static auto lastBatteryUpdate = std::chrono::steady_clock::now();
auto now = std::chrono::steady_clock::now();
if (std::chrono::duration_cast<std::chrono::seconds>(now - lastBatteryUpdate).count() >= 1) {
    lastBatteryUpdate = now;
    if (g_App && g_App->activity && g_App->activity->vm) {
        JNIEnv* env;
        g_App->activity->vm->AttachCurrentThread(&env, nullptr);
        jobject context = g_App->activity->clazz;

        jclass contextClass = env->FindClass("android/content/Context");
        jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
        jstring batteryServiceStr = env->NewStringUTF("batterymanager");
        jobject batteryManager = env->CallObjectMethod(context, getSystemServiceMethod, batteryServiceStr);

        jclass batteryManagerClass = env->FindClass("android/os/BatteryManager");
        jmethodID getIntProperty = env->GetMethodID(batteryManagerClass, "getIntProperty", "(I)I");
        电量 = env->CallIntMethod(batteryManager, getIntProperty, 4);

        jmethodID registerReceiverMethod = env->GetMethodID(contextClass, "registerReceiver", "(Landroid/content/BroadcastReceiver;Landroid/content/IntentFilter;)Landroid/content/Intent;");
        jclass intentFilterClass = env->FindClass("android/content/IntentFilter");
        jmethodID intentFilterConstructor = env->GetMethodID(intentFilterClass, "<init>", "(Ljava/lang/String;)V");
        jobject intentFilter = env->NewObject(intentFilterClass, intentFilterConstructor, env->NewStringUTF("android.intent.action.BATTERY_CHANGED"));
        jobject intent = env->CallObjectMethod(context, registerReceiverMethod, nullptr, intentFilter);

        jclass intentClass = env->FindClass("android/content/Intent");
        jmethodID getIntExtra = env->GetMethodID(intentClass, "getIntExtra", "(Ljava/lang/String;I)I");
        jstring tempKey = env->NewStringUTF("temperature");
        int temp = env->CallIntMethod(intent, getIntExtra, tempKey, 0);
        温度 = temp / 10.0f;

        g_App->activity->vm->DetachCurrentThread();
    }
}
			if (localPlayerController) {
				std::vector<ASTExtraPlayerCharacter*> PlayerCharacter;
				GetAllActors(PlayerCharacter);
				for (auto actor = PlayerCharacter.begin();
					actor != PlayerCharacter.end(); actor++) {
					auto Actor = *actor;
					if (Actor->PlayerKey == ((ASTExtraPlayerController*)localPlayerController)->PlayerKey) {
						localPlayer = Actor;
						break;
					}
				}

				if (localPlayer) {
				
				#include "其他文件/美化配置/枪械载具梅花配置.h" 
#include "其他文件/美化配置/自改.h"
					if (localPlayer->PartHitComponent) {
						auto ConfigCollisionDistSqAngles = localPlayer->PartHitComponent->ConfigCollisionDistSqAngles;
						for (int j = 0; j < ConfigCollisionDistSqAngles.Num(); j++) {
							ConfigCollisionDistSqAngles[j].Angle = 180.0f;
						}
						localPlayer->PartHitComponent->ConfigCollisionDistSqAngles = ConfigCollisionDistSqAngles;
					}
    
				if (Settings.Aim.aim){
//    if (Settings.Aim.aim) {
        int hitRate = rand() % 100;
        if (hitRate <= hitRate) {
            ASTExtraPlayerCharacter* Target = GetTargetForAim();
            if (Target) {
                bool triggerOk = false;
             if (Settings.Aim.trigger == 0) {
            triggerOk = g_LocalPlayer->bIsWeaponFiring;
            }else
            if (Settings.Aim.trigger == 1){
            triggerOk = g_LocalPlayer->bIsGunADS;
            }else
			if (Settings.Aim.trigger == 2){	
			triggerOk = g_LocalPlayer->bIsWeaponFiring || g_LocalPlayer->bIsGunADS;
			}
                if (triggerOk) {
                    FVector targetAimPos;
                  if (Settings.Aim.location == 0) {
								if (算法 == 1) {
    targetAimPos = Target->GetBonePos("Head", {});
}
else if (算法 == 2) {
    targetAimPos = Target->GetBonePos("pelvis", {});   // 骨盆（原注释写错）
}
else if (算法 == 3) {
    targetAimPos = Target->GetBonePos("foot_l", {});   // 左脚
}
else if (算法 == 4) {
    targetAimPos = Target->GetBonePos("foot_r", {});   // 右脚
}
else if (算法 == 5) {
    targetAimPos = Target->GetBonePos("calf_r", {});   // 右小腿
}
else if (算法 == 6) {
    targetAimPos = Target->GetBonePos("calf_l", {});   // 左小腿
}
else if (算法 == 7) {
    targetAimPos = Target->GetBonePos("lowerarm_r", {}); // 右小臂
}
else if (算法 == 8) {
    targetAimPos = Target->GetBonePos("lowerarm_l", {}); // 左小臂
}
else if (算法 == 9) {
    targetAimPos = Target->GetBonePos("thigh_r", {});  // 右大腿
}
else if (算法 == 10) {
    targetAimPos = Target->GetBonePos("thigh_l", {});  // 左大腿
}

/* === 以下为新补充，按“头→脚”顺序编号 === */

else if (算法 == 11) {
    targetAimPos = Target->GetBonePos("neck_01", {});  // 脖子
}
else if (算法 == 12) {
    targetAimPos = Target->GetBonePos("spine_03", {}); // 上脊柱（胸腔）
}
else if (算法 == 13) {
    targetAimPos = Target->GetBonePos("spine_02", {}); // 中脊柱
}
else if (算法 == 14) {
    targetAimPos = Target->GetBonePos("spine_01", {}); // 下脊柱
}
else if (算法 == 15) {
    targetAimPos = Target->GetBonePos("Root", {});     // 根骨骼（臀部中心）
}
else if (算法 == 16) {
    targetAimPos = Target->GetBonePos("upperarm_l", {}); // 左上臂
}
else if (算法 == 17) {
    targetAimPos = Target->GetBonePos("upperarm_r", {}); // 右上臂
}
else if (算法 == 18) {
    targetAimPos = Target->GetBonePos("hand_l", {});   // 左手
}
else if (算法 == 19) {
    targetAimPos = Target->GetBonePos("hand_r", {});   // 右手
}
else {
    targetAimPos = Target->GetBonePos("Head", {});     // 默认仍打头
}
}else
if(Settings.Aim.location == 1){ 
if (算法 == 1) {
    targetAimPos = Target->GetBonePos("Head", {});          // 头
}
else if (算法 == 2) {
    targetAimPos = Target->GetBonePos("pelvis", {});        // 骨盆
}
else if (算法 == 3) {
    targetAimPos = Target->GetBonePos("foot_l", {});        // 左脚
}
else if (算法 == 4) {
    targetAimPos = Target->GetBonePos("foot_r", {});        // 右脚
}
else if (算法 == 5) {
    targetAimPos = Target->GetBonePos("calf_r", {});        // 右小腿
}
else if (算法 == 6) {
    targetAimPos = Target->GetBonePos("calf_l", {});        // 左小腿
}
else if (算法 == 7) {
    targetAimPos = Target->GetBonePos("lowerarm_r", {});    // 右小臂
}
else if (算法 == 8) {
    targetAimPos = Target->GetBonePos("lowerarm_l", {});    // 左小臂
}
else if (算法 == 9) {
    targetAimPos = Target->GetBonePos("thigh_r", {});       // 右大腿
}
else if (算法 == 10) {
    targetAimPos = Target->GetBonePos("thigh_l", {});       // 左大腿
}
else if (算法 == 11) {
    targetAimPos = Target->GetBonePos("neck_01", {});       // 脖子
}
else if (算法 == 12) {
    targetAimPos = Target->GetBonePos("spine_03", {});      // 上脊柱（胸腔）
}
else if (算法 == 13) {
    targetAimPos = Target->GetBonePos("spine_02", {});      // 中脊柱（身体）
}
else if (算法 == 14) {
    targetAimPos = Target->GetBonePos("spine_01", {});      // 下脊柱
}
else if (算法 == 15) {
    targetAimPos = Target->GetBonePos("Root", {});          // 根骨骼（臀中心）
}
else if (算法 == 16) {
    targetAimPos = Target->GetBonePos("upperarm_l", {});    // 左上臂
}
else if (算法 == 17) {
    targetAimPos = Target->GetBonePos("upperarm_r", {});    // 右上臂
}
else if (算法 == 18) {
    targetAimPos = Target->GetBonePos("hand_l", {});        // 左手
}
else if (算法 == 19) {
    targetAimPos = Target->GetBonePos("hand_r", {});        // 右手
}
else if (算法 == 20) {
    targetAimPos = Target->GetBonePos("spine_02", {});      // 身体（胸部中央）
}
else {
    targetAimPos = Target->GetBonePos("spine_02", {});          // 默认：头
}
}

			         auto WeaponManagerComponent = localPlayer->WeaponManagerComponent;
                     if (WeaponManagerComponent)
                     {
                       auto propSlot = WeaponManagerComponent->GetCurrentUsingPropSlot();
                        if ((int)propSlot.GetValue() >= 1 && (int)propSlot.GetValue() <= 3)
                        {
                          auto CurrentWeaponReplicated = (ASTExtraShootWeapon *)WeaponManagerComponent->CurrentWeaponReplicated;
                          if (CurrentWeaponReplicated)
                          {
                                auto ShootWeaponComponent = CurrentWeaponReplicated->ShootWeaponComponent;
                              if (ShootWeaponComponent)
                              {
                                UShootWeaponEntity *ShootWeaponEntityComponent = ShootWeaponComponent->ShootWeaponEntityComponent;
                              if (ShootWeaponEntityComponent)
                              {
                                 ASTExtraVehicleBase *CurrentVehicle = Target->CurrentVehicle;
                                 float dist = localPlayer->GetDistanceTo(Target);
                                 auto timeToTravel = dist / ShootWeaponEntityComponent->BulletRange;
                              if (CurrentVehicle)
                              {
                                 FVector LinearVelocity = CurrentVehicle->ReplicatedMovement.LinearVelocity;
                                 targetAimPos = UKismetMathLibrary::Add_VectorVector(targetAimPos, UKismetMathLibrary::Multiply_VectorFloat(LinearVelocity, timeToTravel));
                               }else{
                                 FVector Velocity = Target->GetVelocity();
                                 targetAimPos = UKismetMathLibrary::Add_VectorVector(targetAimPos, UKismetMathLibrary::Multiply_VectorFloat(Velocity, timeToTravel));
                               }
                            
                               if (localPlayer->bIsWeaponFiring)
                               {
								 if(后座){ targetAimPos.Z += 11.0f;}
                               float dist = localPlayer->GetDistanceTo(Target) / 100.f;
       targetAimPos.Z -= dist * 1.074f;
                                }
                            
                                     FVector fDir = UKismetMathLibrary::Subtract_VectorVector(targetAimPos, g_PlayerController->PlayerCameraManager->CameraCache.POV.Location);
                                     FRotator Yaptr = UKismetMathLibrary::Conv_VectorToRotator(fDir);
                                
                                     FRotator CpYaT = g_PlayerController->PlayerCameraManager->CameraCache.POV.Rotation;
                                
                                     Yaptr.Pitch -= CpYaT.Pitch;
                                     Yaptr.Yaw -= CpYaT.Yaw;
                                     Yaptr.Roll = 0.f;
                                     AimAngle(Yaptr);
                                
                                     CpYaT.Pitch += Yaptr.Pitch / Settings.Aim.speed; // Aim X Speed Make Float : Xs
                                     CpYaT.Yaw += Yaptr.Yaw / Settings.Aim.speed; // Aim Y Speed Make Float : Ys
                                     CpYaT.Roll = 0.f;
                                // 显示用整数，实际用浮点

//GUI::SliderInt("视场大小", &fovInt, 10, 400);
										XY = Settings.Aim.speed * 16;
							//	Settings.Aim.fovsize = XY / 10;
                                 	  	g_PlayerController->SetControlRotation(CpYaT, "");
                                      }
                                   }
                                }
                             }
                          }
				}}}				
				}

						

					if (Settings.SdkFun.skin && localPlayer && localPlayer->AvatarComponent2) {
						auto& AvatarComp = *localPlayer->AvatarComponent2;
						auto& NetAvatarComp = *(FNetAvatarSyncData*)((uintptr_t)&AvatarComp + 0x388);
						auto& Slotsybc = NetAvatarComp.SlotSyncData;

						if (Slotsybc[2].ItemId != Settings.SdkFun.skinvalue) {
							Slotsybc[2].ItemId = Settings.SdkFun.skinvalue;
							AvatarComp.OnRep_BodySlotStateChanged();
						}
 					}
                    static bool IsVisible;
					int totalEnemies = 0, totalBots = 0;
					std::vector<ASTExtraPlayerCharacter*> PlayerCharacter;
					GetAllActors(PlayerCharacter);
					for (auto actor = PlayerCharacter.begin(); actor != PlayerCharacter.end(); actor++)
					{
						auto Player = *actor;
						if (Player->PlayerKey == localPlayer->PlayerKey)
							continue;

						if (Player->TeamID == localPlayer->TeamID)
							continue;

						if (Player->bDead)
							continue;

						if (Player->bHidden)
							continue;

						if (!Player->RootComponent)
							continue;

						if (忽略机器人) {
							if (Player->bEnsure)
								continue;
						}

						if (Player->bEnsure)
							totalBots++;
						else totalEnemies++;

						FLinearColor pColor = 红色;
						FLinearColor bColor = 绿色;
					    IsVisible = localPlayerController->LineOfSightTo(Player, { 0, 0, 0 }, true);
						float Distance = localPlayer->GetDistanceTo(Player) / 100.0f;
						int TeamID = Player->TeamID;
						bool bIsAI = Player->bEnsure;

						FVector pLocation = Player->K2_GetActorLocation();
						FVector sLocation = localPlayer->K2_GetActorLocation();

						FVector HeadPos = GetBoneLocation(Player, "Head");
						FVector2D HeadPosSC;
						FVector RootPos = GetBoneLocation(Player, "Root");
						FVector2D RootPosSC;
						FVector Root = GetBoneLocation(Player, "Root");
						FVector Spin = GetBoneLocation(Player, "pelvis");
						FVector Spin2 = GetBoneLocation(Player, "spine_03");
						FVector pelvis = GetBoneLocation(Player, "pelvis");
						FVector2D pelvisPoSC;
						FVector upper_r = GetBoneLocation(Player, "upperarm_r");
						FVector2D upper_rPoSC;
						FVector lowerarm_r = GetBoneLocation(Player, "lowerarm_r");
						FVector2D lowerarm_rPoSC;
						FVector lowerarm_l = GetBoneLocation(Player, "lowerarm_l");
						FVector2D lowerarm_lSC;
						FVector hand_r = GetBoneLocation(Player, "hand_r");
						FVector2D hand_rPoSC;
						FVector upper_l = GetBoneLocation(Player, "upperarm_l");
						FVector2D upper_lPoSC;
						FVector hand_l = GetBoneLocation(Player, "hand_l");
						FVector2D hand_lPoSC;
						FVector thigh_l = GetBoneLocation(Player, "thigh_l");
						FVector2D thigh_lPoSC;
						FVector calf_l = GetBoneLocation(Player, "calf_l");
						FVector2D calf_lPoSC;
						FVector foot_l = GetBoneLocation(Player, "foot_l");
						FVector2D foot_lPoSC;
						FVector thigh_r = GetBoneLocation(Player, "thigh_r");
						FVector2D thigh_rPoSC;
						FVector calf_r = GetBoneLocation(Player, "calf_r");
						FVector2D calf_rPoSC;
						FVector foot_r = GetBoneLocation(Player, "foot_r");
						FVector2D foot_rPoSC;
						FVector neck_01 = GetBoneLocation(Player, "neck_01");
						FVector2D neck_01PoSC;
						FVector spine_01 = GetBoneLocation(Player, "spine_01");
						FVector2D spine_01PoSC;
						FVector spine_02 = GetBoneLocation(Player, "spine_02");
						FVector2D spine_02PoSC;
						FVector spine_03 = GetBoneLocation(Player, "spine_03");
						FVector2D spine_03PoSC;
						
												if (背敌) {
							bool shit = false;
                        FVector MyPosition, EnemyPosition;
                        ASTExtraVehicleBase * CurrentVehiclea = Player->CurrentVehicle;
                        if (CurrentVehiclea) {
                        MyPosition = CurrentVehiclea->RootComponent->RelativeLocation;
                        } else {
                        MyPosition = Player->RootComponent->RelativeLocation;
                        }
                       ASTExtraVehicleBase * CurrentVehicle = localPlayer->CurrentVehicle;
                       if (CurrentVehicle) {
                       EnemyPosition = CurrentVehicle->RootComponent->RelativeLocation;
                       } else {
                       EnemyPosition = localPlayer->RootComponent->RelativeLocation;
                       }
                       FVector EntityPos = WorldToRadar(localPlayerController->PlayerCameraManager->CameraCache.POV.Rotation.Yaw, MyPosition, EnemyPosition, NULL, NULL,Vector3(screenWidth, screenHeight, 0), shit);
FVector angle = FVector();
Vector3 forward = Vector3((float)(screenWidth / 2) - EntityPos.X, (float)(screenHeight / 2) - EntityPos.Y, 0.0f);
VectorAnglesRadar(forward, angle);
const auto angle_yaw_rad = DEG2RAD(angle.Y + 180.f);
const auto new_point_x = (screenWidth / 2) + (65) / 2 * 8 * cosf(angle_yaw_rad);
const auto new_point_y = (screenHeight / 2) + (65) / 2 * 8 * sinf(angle_yaw_rad);
const auto malik_point_x = (screenWidth / 2) + (65) / 2 * 8 * cosf(angle_yaw_rad);
const auto malik_point_y = (screenHeight / 2) + (65) / 2 * 8 * sinf(angle_yaw_rad);

// 调整顶点偏移量以形成更大更正的三角形
std::array <FVector2D, 3> points {
    FVector2D((float)new_point_x - (7.6f * (float)1.5f), new_point_y - (9.3f *1.7f)), 
FVector2D((float)new_point_x + (7.6f *1.5f), new_point_y), 
FVector2D((float)new_point_x - (7.6f *1.5f), new_point_y + (9.3f *1.7f))
};

RotateTriangleSansero(points, angle.Y + 180.0f);
DrawArrows(Canvas, points.at(0), points.at(1), points.at(2), 1.f, IsVisible ? 白色 : 绿色);
std::wstring s;
s += std::to_wstring((int)Distance);       
s += L"m";       
DrawText(Canvas, FString(s), { malik_point_x, malik_point_y + (9.3f *1.7f)}, 白色, 黑色, 10, true);
}

					if(雷达){
                            bool out = false;
                            struct Vector3 Pos;
                            struct Vector3 Size;
                            Pos.X = screenWidth / 4.490;
                            Pos.Y = screenHeight / 150;
                            Size.X = 雷达X;
                            Size.Y = 雷达Y;
                            float RadarCenterX = Pos.X + (Size.X / 2);
                            float RadarCenterY = Pos.Y + (Size.Y / 2);
                            FLinearColor LineColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // 白色
                            FLinearColor PointColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // 红色
                            // 绘制内部十字线条
                            DrawLine(Canvas, FVector2D(Pos.X, Pos.Y), FVector2D(Pos.X + Size.X, Pos.Y), 1, LineColor);
                            DrawLine(Canvas, FVector2D(Pos.X, Pos.Y + Size.Y), FVector2D(Pos.X + Size.X, Pos.Y + Size.Y), 1, LineColor);
                            DrawLine(Canvas, FVector2D(Pos.X, Pos.Y), FVector2D(Pos.X, Pos.Y + Size.Y), 1, LineColor);
                            DrawLine(Canvas, FVector2D(Pos.X + Size.X, Pos.Y), FVector2D(Pos.X + Size.X, Pos.Y + Size.Y), 1, LineColor);
                            DrawLine(Canvas, FVector2D(RadarCenterX, Pos.Y), FVector2D(RadarCenterX, Pos.Y + Size.Y), 1, LineColor);
                            DrawLine(Canvas, FVector2D(Pos.X, RadarCenterY), FVector2D(Pos.X + Size.X, RadarCenterY), 1, LineColor);
                            FVector MyPosition, EnemyPosition;
                            ASTExtraVehicleBase * CurrentVehiclea = Player->CurrentVehicle;
                            if (CurrentVehiclea) {
                                MyPosition = CurrentVehiclea->RootComponent->RelativeLocation;
                            } else {
                                MyPosition = Player->RootComponent->RelativeLocation;
                            }
                            ASTExtraVehicleBase * CurrentVehicle = localPlayer->CurrentVehicle;
                            if (CurrentVehicle) {
                                EnemyPosition = CurrentVehicle->RootComponent->RelativeLocation;
                            } else {
                                EnemyPosition = localPlayer->RootComponent->RelativeLocation;
                            }
                            FVector single = WorldToRadar(localPlayerController->PlayerCameraManager->CameraCache.POV.Rotation.Yaw, MyPosition, EnemyPosition, Pos.X, Pos.Y, Size, out);
                            float Distance = localPlayer->GetDistanceTo(Player) / 100.0f;
                            if (Distance >= 0.0f && Distance < 500.0f) {
								DrawFilledRect(Canvas, FVector2D{single.X,single.Y},5,5,红色);
                            }
                                        }
						if (gGameplayStatics->ProjectWorldToScreen(g_PlayerController, HeadPos, false, &HeadPosSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, lowerarm_l, false, &lowerarm_lSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, upper_r, false, &upper_rPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, upper_l, false, &upper_lPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, lowerarm_r, false, &lowerarm_rPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, hand_r, false, &hand_rPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, hand_l, false, &hand_lPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, thigh_l, false, &thigh_lPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, calf_l, false, &calf_lPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, foot_l, false, &foot_lPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, thigh_r, false, &thigh_rPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, calf_r, false, &calf_rPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, foot_r, false, &foot_rPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, neck_01, false, &neck_01PoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, pelvis, false, &pelvisPoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, RootPos, false, &RootPosSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, spine_01, false, &spine_01PoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, spine_02, false, &spine_02PoSC) &&
							gGameplayStatics->ProjectWorldToScreen(g_PlayerController, spine_03, false, &spine_03PoSC)) {
							Rect box;
							box.x = HeadPosSC.X - (RootPosSC.Y - HeadPosSC.Y) / 4;
							box.y = HeadPosSC.Y - 20;
							box.width = (RootPosSC.Y - HeadPosSC.Y) / 2;
							box.height = RootPosSC.Y - box.y;

							if (骨骼) {
								FVector2D 头部 = { HeadPosSC.X, HeadPosSC.Y };//头部
								FVector2D 左肩膀 = { upper_lPoSC.X, upper_lPoSC.Y };//左上肩
								FVector2D 右肩膀 = { upper_rPoSC.X, upper_rPoSC.Y };//右上肩
								FVector2D 右小臂 = { lowerarm_rPoSC.X, lowerarm_rPoSC.Y };//右小臂
								FVector2D 左小臂 = { lowerarm_lSC.X, lowerarm_lSC.Y };//左小臂
								FVector2D 右大腿 = { thigh_rPoSC.X, thigh_rPoSC.Y };//右大腿
								FVector2D 左大腿 = { thigh_lPoSC.X, thigh_lPoSC.Y };//左大腿
								FVector2D 左小腿 = { calf_lPoSC.X, calf_lPoSC.Y };//左小腿
								FVector2D 右小腿 = { calf_rPoSC.X, calf_rPoSC.Y };//右小腿
								FVector2D 腰部 = { spine_01PoSC.X, spine_01PoSC.Y };//右小腿
								FVector2D 腰部3 = { spine_03PoSC.X, spine_03PoSC.Y };//右小腿
								FVector2D 脖子 = { neck_01PoSC.X, neck_01PoSC.Y };//右小腿
								FVector2D 腰部2 = { spine_02PoSC.X, spine_02PoSC.Y };//右小腿
								float boxWidth = 7.f - Distance * 0.03;

								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("Head", { 0, 0, 0 }), false))//头
								{
								//	DrawCircle(Canvas, 头部.X, 头部.Y, 160 / Distance > 10 ? 10 : 160 / Distance, 50, 绿色);
									DrawLine(Canvas, 头部, HeadPosSC, 1, 绿色);
								}
								else {
									//DrawCircle(Canvas, 头部.X, 头部.Y, 160 / Distance > 10 ? 10 : 160 / Distance, 50, 红色);
									DrawLine(Canvas, 头部, HeadPosSC, 1, 红色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("neck_01", { 0, 0, 0 }), false))//脖子
								{
									DrawLine(Canvas, 脖子, HeadPosSC, 1, 绿色);
									DrawLine(Canvas, 脖子, spine_03PoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 脖子, HeadPosSC, 1, 白色);
									DrawLine(Canvas, 脖子, spine_03PoSC, 1, 白色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("upperarm_r", { 0, 0, 0 }), false))//上面的肩膀右
								{
									DrawLine(Canvas, 右肩膀, neck_01PoSC, 1, 绿色);
									DrawLine(Canvas, 右肩膀, hand_rPoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 右肩膀, neck_01PoSC, 1, 白色);
									DrawLine(Canvas, 右肩膀, hand_rPoSC, 1, 白色);
								}

								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("upperarm_l", { 0, 0, 0 }), false))//上面的肩膀左
								{
									DrawLine(Canvas, 左肩膀, lowerarm_lSC, 1, 绿色);
									DrawLine(Canvas, 左肩膀, neck_01PoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 左肩膀, lowerarm_lSC, 1, 白色);
									DrawLine(Canvas, 左肩膀, neck_01PoSC, 1, 白色);
								}

								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("lowerarm_r", { 0, 0, 0 }), false))//上面的手臂右
								{
									DrawLine(Canvas, 右小臂, lowerarm_rPoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 右小臂, lowerarm_rPoSC, 1, 白色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("lowerarm_l", { 0, 0, 0 }), false))//上面的手臂左
								{
									DrawLine(Canvas, 左小臂, lowerarm_lSC, 1, 绿色);
									DrawLine(Canvas, 左小臂, hand_lPoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 左小臂, lowerarm_lSC, 1, 白色);
									DrawLine(Canvas, 左小臂, hand_lPoSC, 1, 白色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("spine_03", { 0, 0, 0 }), false))//脊柱3
								{
									DrawLine(Canvas, 腰部3, spine_02PoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 腰部3, spine_02PoSC, 1, 白色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("spine_02", { 0, 0, 0 }), false))//脊柱2
								{
									DrawLine(Canvas, 腰部2, spine_01PoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 腰部2, spine_01PoSC, 1, 白色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("spine_01", { 0, 0, 0 }), false))//脊柱1
								{
									DrawLine(Canvas, 腰部, pelvisPoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 腰部, pelvisPoSC, 1, 白色);
								}

								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("thigh_l", { 0, 0, 0 }), false))//大腿左
								{
									DrawLine(Canvas, 左大腿, calf_lPoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 左大腿, calf_lPoSC, 1, 白色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("thigh_r", { 0, 0, 0 }), false))//大腿右
								{
									DrawLine(Canvas, 右大腿, calf_rPoSC, 1, 绿色);
									DrawLine(Canvas, 右大腿, thigh_lPoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 右大腿, calf_rPoSC, 1, 白色);
									DrawLine(Canvas, 右大腿, thigh_lPoSC, 1, 白色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("calf_l", { 0, 0, 0 }), false))//小腿左
								{
									DrawLine(Canvas, 左小腿, foot_lPoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 左小腿, foot_lPoSC, 1, 白色);
								}
								if (!g_PlayerController->LineOfSightTo(g_PlayerController->PlayerCameraManager, Player->GetBonePos("calf_r", { 0, 0, 0 }), false))//小腿右
								{
									DrawLine(Canvas, 右小腿, calf_rPoSC, 1, 绿色);
									DrawLine(Canvas, 右小腿, foot_rPoSC, 1, 绿色);
								}
								else {
									DrawLine(Canvas, 右小腿, foot_rPoSC, 1, 白色);
									DrawLine(Canvas, 右小腿, calf_rPoSC, 1, 白色);
								}
							}

							if (线) {
if (!localPlayerController->LineOfSightTo(Player, {0, 0, 0}, true)) {
DrawLine(Canvas, {static_cast<float>(screenWidth / 2), 126}, FVector2D(HeadPosSC.X, HeadPosSC.Y - 22), 1.0f,绿色);
}else{
DrawLine(Canvas, {static_cast<float>(screenWidth / 2), 126}, FVector2D(HeadPosSC.X, HeadPosSC.Y - 22), 1.0f, 白色);
}
}
if(健康){
float CurHP = std::max(0.f, std::min(Player->Health, Player->HealthMax));
									float MaxHP = Player->HealthMax;
		FLinearColor ColorHP;
if (!localPlayerController->LineOfSightTo(Player, {0, 0, 0}, true)) {
    ColorHP = {0, 1.f, 0, 1.0f};
    }else{
    ColorHP = {1.f, 0.f, 0.f, 1.0f};    
    }
									if (CurHP == 0 && !Player->bDead)
									{
										ColorHP = {1.f, 1.f, 1.f, 0.8f};
										CurHP = Player->NearDeathBreath;
										USTCharacterNearDeathComp *NearDeatchComponent = Player->NearDeatchComponent;
										if (NearDeatchComponent)
										{
											MaxHP = NearDeatchComponent->BreathMax;
										}
									}

									auto AboveHead = Player->GetHeadLocation(true);
									AboveHead.Z += 35.f;
									FVector2D AboveHeadSc;
									if (gGameplayStatics->ProjectWorldToScreen(localPlayerController, AboveHead, false, &AboveHeadSc)) {
										auto mWidthScale = std::min(0.1f * Distance, 45.f);
										auto mWidth = 68.5f - mWidthScale;
										auto mHeight = mWidth * 0.175f;
										AboveHeadSc.X -= (mWidth / 2);
										AboveHeadSc.Y -= (mHeight * 2.f);
        DrawFilledRect(Canvas, {AboveHeadSc.X, AboveHeadSc.Y}, mWidth, mHeight, {0, 0, 0, 1.0}); // 黑色半透明背景

        // 绘制血量条
										DrawFilledRect(Canvas, {AboveHeadSc.X, AboveHeadSc.Y}, (CurHP * mWidth / MaxHP), mHeight, ColorHP);

        // 绘制外框
        if (!localPlayerController->LineOfSightTo(Player, {0, 0, 0}, true)) {
        DrawRectangle(Canvas, {AboveHeadSc.X, AboveHeadSc.Y}, mWidth, mHeight, 1.3f, {0, 1.f, 0, 0.9f});}else{
             DrawRectangle(Canvas, {AboveHeadSc.X, AboveHeadSc.Y}, mWidth, mHeight, 1.3f, {1.f, 0.f, 0.f, 0.8f});}
bool IsVisible = localPlayerController ->LineOfSightTo (Player, {0, 0, 0}, true);   
													if(Player->bEnsure){
                                            	  GUI::DrawFilledCircle(FVector2D{ AboveHeadSc.X - 9.5f,AboveHeadSc.Y + 3.85f}, 11.f, 黑色);  
												  }else{
												  GUI::DrawFilledCircle(FVector2D{ AboveHeadSc.X - 9.5f,AboveHeadSc.Y + 3.85f}, 11.f, GetRandomColorByIndex(Player->TeamID));  
												  }
												  
						tslFont->LegacyFontSize = 8;
#define TSL_FONT_DEFAULT_SIZE 8
						std::wstring ws;
                        ws += L"";
                        ws += std::to_wstring((int) Distance);
                        ws += L"";
						DrawOutlinedText(Canvas,FString(ws), {AboveHeadSc.X-10.2, AboveHeadSc.Y+3.5}, 白色, 黑色, true);
						}
         }
								if (盒子) {
float boxHeight = abs(HeadPosSC.Y - RootPosSC.Y);
float boxWidth = boxHeight * 0.65f;
Box4Line(Canvas, 1.f, HeadPosSC.X - (boxWidth / 2), HeadPosSC.Y, boxWidth, boxHeight,IsVisible ? 白色 : 绿色);
}
if (姓名) {
					FVector BelowRoot = Root;
                    auto AboveHead = Player->GetHeadLocation(true);
                    BelowRoot.Z -= 35.f;
                    FVector2D BelowRootSc;
					if (gGameplayStatics->ProjectWorldToScreen(localPlayerController, BelowRoot, false, &BelowRootSc)) {
					std::wstring ws;

						//名字
						if (Player->bEnsure) {
						ws += L"Bot";
						} else {
						ws += Player->PlayerName.ToWString();
						}
						if(枪械){
						if (Player->GetCurrentWeapon()) {
                        if (Player->GetCurrentWeapon()->GetWeaponName().IsValid()) {
				    	ws += L"-";
                        ws +=Player->GetCurrentWeapon()->GetWeaponName().ToWString();;
						}}}
					
						tslFont->LegacyFontSize = 12;
#define TSL_FONT_DEFAULT_SIZE 13
//tslFont->LegacyFontSize = 17;
if (!localPlayerController->LineOfSightTo(Player, {0, 0, 0}, true)) {
 DrawOutlinedText(Canvas, FString(ws), {(float)BelowRootSc.X - 2, BelowRootSc.Y + 7.2}, 绿色, 黑色, true);
					    }else{
 DrawOutlinedText(Canvas,FString(ws), {(float)BelowRootSc.X - 2, BelowRootSc.Y + 7.2}, 白色, 黑色, true);
}}}							    				
						}
					}

                          if (载具) {
                           std::vector<ASTExtraVehicleBase*> VehicleBase;
                           GetAllActors(VehicleBase);
                           for (auto actor = VehicleBase.begin(); actor != VehicleBase.end(); actor++) {
                                
                            auto Vehicle = *actor;
                            if (!Vehicle->Mesh)
                                continue;
                            int CurHP = (int) std::max(0, std::min((int) Vehicle->VehicleCommon->HP, (int) Vehicle->VehicleCommon->HPMax));
                            int MaxHP = (int) Vehicle->VehicleCommon->HPMax;
                            
                            float Distance = Vehicle->GetDistanceTo(localPlayer) / 100.f;
                            FVector2D vehiclePos;
                            if (W2S(Vehicle->K2_GetActorLocation(), &vehiclePos))
                            {
                                if((int)Distance<150)
                                    {
                                auto mWidthScale = std::min(1 * Distance, 35.f);
                                auto mWidth = 100.f - mWidthScale;
                                auto mHeight = mWidth * 0.05;
                                
                                if (载具)
                                {
                                    std::string s = GetVehicleName(Vehicle);
                                    s += " [";
                                    s += std::to_string((int)Distance);
                                    s += "m]";
tslFont->LegacyFontSize = 13;
 DrawOutlinedText(Canvas, FString(s), {(float)vehiclePos.X, vehiclePos.Y-10.f}, 白色, 空白, true);					
                                    }
                                
                                if (载具血量)
                                {
                                   
                                    DrawFilledRect(Canvas, {vehiclePos.X-(mWidth / 2), vehiclePos.Y+10.f}, (CurHP * mWidth / MaxHP), mHeight, COLOR_RED);
                                    DrawRectangle(Canvas, {vehiclePos.X-(mWidth / 2), vehiclePos.Y+10.f}, mWidth, mHeight, 0, 空白);

                                }
                                                                if (载具油量)
                                {
                                    
                                    int CurFuel = (int) std::max(0, std::min((int) Vehicle->VehicleCommon->Fuel, (int) Vehicle->VehicleCommon->FuelMax));
                                    int MaxFuel = (int) Vehicle->VehicleCommon->FuelMax;
                                    DrawFilledRect(Canvas, {vehiclePos.X-(mWidth / 2), vehiclePos.Y+5.f}, (CurFuel * mWidth / MaxFuel), mHeight, 绿色);
                                    DrawRectangle(Canvas, {vehiclePos.X-(mWidth / 2), vehiclePos.Y+5.f}, mWidth, mHeight, 0, 空白);
                                }
                            }
                        }
                    }
                }
if(聚点){
Write<float>(UE4 + 0x5769f10,8.479635254434225E-21);
} else {
Write<float>(UE4 + 0x5769f10,8.52492339e-21); 
}    
if (打击特效){
}               
if (广角){
writefloat(readValueL(UE4+0xe793470)+0x7C, 2.4);
} else {
//writefloat(readValueL(UE4+0xe793470)+0x7C, 2.4);
}					
if (测试广角){
writedword(readValueL(readValueL(readValueL(readValueL(readValueL(UE4+0x5C4570)+0x30)+0x450)+0x398)+0x20)+0x33C, 120);
}
if(大厅){
 }

if (对局信息) {
// FPS 计算所需变量
static auto lastTime = std::chrono::high_resolution_clock::now();
static int frameCount = 0;
static float fps = 0.0f;

std::vector<ASTExtraGameStateBase *> GameStateBase;
GetAllActors(GameStateBase);
for (auto actor = GameStateBase.begin(); actor != GameStateBase.end(); actor++) {
    auto Game = *actor;

    // 计算真实 FPS
    frameCount++;
    auto now = std::chrono::high_resolution_clock::now();
    float delta = std::chrono::duration<float>(now - lastTime).count();
    if (delta >= 1.0f) {
        fps = frameCount / delta;
        frameCount = 0;
        lastTime = now;
    }

    std::string t;
    t += "\nReal Players: ";
    t += std::to_string((int)Game->PlayerNum);
    t += "\nTeam Remaining: ";
    t += std::to_string((int)Game->AliveTeamNum);
    t += "\nMatch Time: ";
    t += std::to_string((int)Game->ElapsedTime / 60);
    t += "m ";
    t += std::to_string((int)Game->ElapsedTime);
    t += "s";
    t += "\nBattery Temp: ";
	t += (温度 >= 0) ? std::to_string((int)温度) + "" : "N/A";
	t += "\nBatteryLevel: ";
	t += (电量 >= 0) ? std::to_string(电量) + "" : "N/A";
    t += "\nFPS: ";
    t += std::to_string((int)fps);

    int FontSize = 16;
    DrawText(Canvas, FString(t), { (float)screenWidth / 8, 380 }, FLinearColor(1.f, 1.f, 1.f, 1.f), 黑色, 16, true);
}
			}
if (Settings.SdkFun.viewsize) {
						localPlayer->ThirdPersonCameraComponent->SetFieldOfView(Settings.SdkFun.viewsize);
					}			
if(战利品){
		std::vector<APickUpListWrapperActor*>LootboxBase;
						GetAllActors(LootboxBase);
						for (auto actor = LootboxBase.begin(); actor != LootboxBase.end(); actor++) {
							auto Pick = *actor;
							
							
                        if (!Pick->RootComponent)
                            continue;


float Distance = Pick->GetDistanceTo(localPlayer) / 100.0f;
                        
if (Distance >180.0)//骨灰盒显示最大距离
continue;

                        FVector2D PickUpListsPos;


                        if (W2S(Pick->K2_GetActorLocation(), &PickUpListsPos)) {
                            std::string s = "Lootbox";
                            s += " [";
                            s += std::to_string((int) Distance);
                            s += "m]";
        tslFont->LegacyFontSize = 13;

DrawOutlinedText(Canvas, FString(s), {PickUpListsPos.X, PickUpListsPos.Y}, 白色, 空白, true);

                            }
                            }
}
					if (空投箱) {
						std::vector<APickUpListWrapperActor*>LootboxBase;
						GetAllActors(LootboxBase);
						for (auto actor = LootboxBase.begin(); actor != LootboxBase.end(); actor++) {
							auto Pick = *actor;
							if (!Pick->RootComponent)
								continue;

							float Distance = Pick->GetDistanceTo(localPlayer) / 100.0f;

							if (Distance > 200.0)
								continue;
							FVector2D PickUpListsPos;

							if (W2S(Pick->K2_GetActorLocation(), &PickUpListsPos)) {
								std::string s = GetLootName(Pick);
								DrawText(Canvas, FString(s), FVector2D{ PickUpListsPos.X, PickUpListsPos.Y }, 白色, 空白, 15, false);
							}

						}
					}

					if (Settings.SdkFun.fpsvalue) {
						std::vector<USTExtraGameInstance*> GameInstance;
						GetAllActors(GameInstance);
						for (auto actor = GameInstance.begin(); actor != GameInstance.end(); actor++) {
							auto playerChar = *actor;
							playerChar->UserDetailSetting.PUBGDeviceFPSDef = 120;
							playerChar->UserDetailSetting.PUBGDeviceFPSLow = 120;
							playerChar->UserDetailSetting.PUBGDeviceFPSMid = 120;
							playerChar->UserDetailSetting.PUBGDeviceFPSHigh = 120;
							playerChar->UserDetailSetting.PUBGDeviceFPSHDR = 120;
							playerChar->UserDetailSetting.PUBGDeviceFPSUltralHigh = 120;
						}
					}

				int GrenadeCount = 0;

// 定义一个全局的 Grenade 对象指针集合，用于跟踪当前存在且需要绘制的投掷物
std::unordered_map<ASTExtraGrenadeBase*, bool> activeGrenades;
if (炸弹警告) {
                        // 存储所有投掷物的活动轨迹和时间戳
                        static std::map<ASTExtraGrenadeBase*, std::vector<std::pair<FVector, float>>> GrenadeTrajectories;

                        std::vector<ASTExtraGrenadeBase*> Grenade;
                        GetAllActors(Grenade);

                        // 获取当前时间
                        float currentTime = UGameplayStatics::GetTimeSeconds(GetWorld());

                        // 遍历所有投掷物
                        for (auto actor = Grenade.begin(); actor != Grenade.end(); actor++) {
                            auto Grenade = *actor;

                            // 确保根组件存在
                            if (!Grenade->RootComponent)
                                continue;

                            float Distance = Grenade->GetDistanceTo(localPlayer) / 100.0f;

                            // 如果距离大于100米，跳过
                            if (Distance > 100.0)
                                continue;

                            FVector2D GrenadePos;
                            // 转换世界坐标到屏幕坐标
                            if (W2S(Grenade->K2_GetActorLocation(), &GrenadePos)) {
                                std::string classname = Grenade->GetName();

                                // 如果是手雷或者燃烧瓶
                                if (classname.find("Grenade") != std::string::npos) {

                                    // 获取投掷物的当前位置
                                    FVector grenadeLocation = Grenade->K2_GetActorLocation();

                                    // 检查该投掷物是否已有记录的轨迹
                                    if (GrenadeTrajectories.find(Grenade) == GrenadeTrajectories.end()) {
                                        GrenadeTrajectories[Grenade] = std::vector<std::pair<FVector, float>>();
                                    }

                                    // 添加当前投掷物的位置和时间戳到轨迹
                                    GrenadeTrajectories[Grenade].push_back({grenadeLocation, currentTime});

                                    // 清除过期的轨迹点 (超过3秒)
                                    auto& trajectory = GrenadeTrajectories[Grenade];
                                    trajectory.erase(
                                    std::remove_if(trajectory.begin(), trajectory.end(), [currentTime](const std::pair<FVector, float>& traj) {
                                        return currentTime - traj.second > 4.0f;  // 3秒后删除
                                    }),
                                    trajectory.end()
                                    );

                                    // 绘制轨迹
                                    if (trajectory.size() > 1) {
                                        for (size_t i = 1; i < trajectory.size(); i++) {
                                            FVector2D prevPos, currPos;
                                            if (W2S(trajectory[i - 1].first, &prevPos) && W2S(trajectory[i].first, &currPos)) {
                                                // 绘制从上一个位置到当前的位置的线段
                                                DrawLine(Canvas, prevPos, currPos, 2, 黄色);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
//北欲泛滥引擎投掷物预警搬运请带名字
if (炸弹警告) {
    std::vector<ASTExtraGrenadeBase*> Grenades;
    GetAllActors(Grenades);

    // 更新 activeGrenades，移除已不存在的手雷
    for (auto it = activeGrenades.begin(); it != activeGrenades.end();) {
        bool found = false;
        for (const auto& Grenade : Grenades) {
            if (it->first == Grenade) {
                found = true;
                break;
            }
        }
        if (!found) {
            it = activeGrenades.erase(it);
        } else {
            ++it;
        }
    }

    int GrenadeCount = 0;

    for (const auto& Grenade : Grenades) {
        if (!Grenade->RootComponent) {
            continue;
        }

        const FVector GrenadeLocation = Grenade->K2_GetActorLocation();
        const float Distance = Grenade->GetDistanceTo(localPlayer) / 100.0f;

        if (Distance <= 150.0f) {
            GrenadeCount++;

            FVector2D GrenadePos;
            if (W2S(GrenadeLocation, &GrenadePos)) {
                DrawFilledCircle(Canvas, GrenadePos, 10, 红色);
                std::wstring dis = std::to_wstring((int)Distance);
                DrawText(Canvas, FString(dis), GrenadePos, 白色, 黑色, 12, true);
            }

            // 将当前手雷添加到 activeGrenades 中
            activeGrenades[Grenade] = true;
        }
    }

    if (GrenadeCount > 0) {
        char num[50];
        sprintf(num, "!!WARNING THERE IS [ %d ] GRENADE NEAR YOU!!", GrenadeCount);
        DrawText(Canvas, FString(num), { (float)screenWidth / 2, 140 }, 红色, 黑色, 20, true);
    }
}
	if (投掷物预警) {
	std::vector<ASTExtraGrenadeBase*>Throw;
GetAllActors(Throw);
for (auto actor = Throw.begin(); actor != Throw.end(); actor++) {
auto Throw = *actor;

if (!Throw->RootComponent)
continue;

float Distance = Throw->GetDistanceTo(localPlayer) / 100.0f;
if (Distance > 100.0)//投掷物显示最大距离
continue;

FVector2D GrenadePos;
if (W2S(Throw->K2_GetActorLocation(), &GrenadePos)) {
std::string classname = Throw->GetName();
if (classname.find("BP_Projectile_FragGrenade_C") != std::string::npos) {
std::string ws;
                                    std::string s =  "碎片手雷";
                                    s += "(";
                                    s += std::to_string((int) Distance);
                                    s += "m)";


tslFont->LegacyFontSize = 15;
DrawOutlinedText(Canvas, FString(s), GrenadePos, COLOR_RED, 空白, true);

}
}
}
}
                        
if (投掷物预警) {
std::vector<ASTExtraGrenadeBase*>Throw;
GetAllActors(Throw);
for (auto actor = Throw.begin(); actor != Throw.end(); actor++) {
auto Throw = *actor;
if (!Throw->RootComponent)
continue;
float Distance = Throw->GetDistanceTo(localPlayer) / 100.0f;
if (Distance > 100.0)//投掷物显示最大距离
continue;            
FVector2D GrenadePos;
  if (W2S(Throw->K2_GetActorLocation(), &GrenadePos)) {
std::string classname = Throw->GetName();
if (classname.find("BP_Projectile_BurnGrenade_C") != std::string::npos) {
std::string ws;
                                    std::string s =  "燃烧瓶";
                                    s += "(";
                                    s += std::to_string((int) Distance);
                                    s += "m)";
tslFont->LegacyFontSize = 15;
DrawOutlinedText(Canvas, FString(s), GrenadePos, COLOR_RED, 空白, true);

}
}
}
}

if (投掷物预警) {
std::vector<ASTExtraGrenadeBase*>Throw;
GetAllActors(Throw);
for (auto actor = Throw.begin(); actor != Throw.end(); actor++) {
auto Throw = *actor;
if (!Throw->RootComponent)
continue;
float Distance = Throw->GetDistanceTo(localPlayer) / 100.0f;
if (Distance > 100.0)//投掷物显示最大距离
continue;            
FVector2D GrenadePos;
  if (W2S(Throw->K2_GetActorLocation(), &GrenadePos)) {
std::string classname = Throw->GetName();
if (classname.find("BP_Projectile_SmokeBomb_C") != std::string::npos) {
std::string ws;
                                    std::string s =  "烟雾弹";
                                    s += "(";
                                    s += std::to_string((int) Distance);
                                    s += "m)";
tslFont->LegacyFontSize = 15;
DrawOutlinedText(Canvas, FString(s), GrenadePos, COLOR_RED, 空白, true);

}
}
}
}

if (投掷物预警) {
std::vector<ASTExtraGrenadeBase*>Throw;
GetAllActors(Throw);
for (auto actor = Throw.begin(); actor != Throw.end(); actor++) {
auto Throw = *actor;
if (!Throw->RootComponent)
continue;
float Distance = Throw->GetDistanceTo(localPlayer) / 100.0f;
if (Distance > 100.0)//投掷物显示最大距离
continue;            
FVector2D GrenadePos;
  if (W2S(Throw->K2_GetActorLocation(), &GrenadePos)) {
std::string classname = Throw->GetName();
if (classname.find("BP_Projectile_StunGrenade_C") != std::string::npos) {
                                    std::string ws;
                                    std::string s =  "闪光弹";
                                    s += "(";
                                    s += std::to_string((int) Distance);
                                    s += "m)";
tslFont->LegacyFontSize = 15;
DrawOutlinedText(Canvas, FString(s), GrenadePos, COLOR_RED, 空白, true);
}
}
}
}
					if (Settings.Aim.aim || Settings.Aim.silent) {
						DrawCircle(Canvas, (screenWidth / 2), (screenHeight / 2), Settings.Aim.fovsize, 5000, 红色);
					}


	if (totalEnemies > 0 || totalBots > 0) {
		tslFont->LegacyFontSize = 32;
std::wstring 总人数 = L"";
总人数 += std::to_wstring((int)totalEnemies + totalBots);
// 定义矩形的宽度和高度
float rectWidth = 50; // 矩形宽度
float rectHeight = 40; // 矩形高度
// 计算矩形的起始位置（使其居中）
FVector2D rectStartPos = {(float)screenWidth / 2 - rectWidth / 2, 50};
// 绘制填充矩形
//DrawFilledRect(Canvas, rectStartPos, rectWidth, rectHeight, FLinearColor(1,0,0,0.6f)); // 填充颜色改为黄色
// 绘制边框
//DrawRectangle(Canvas, rectStartPos, rectWidth, rectHeight, 1.0f, {0.f, 0.f, 0.f, 1.f}); // 绘制边框改为黑色

// 计算文字居中位置
FVector2D 文字位置 = {(float)screenWidth / 2, 85 + rectHeight / 2}; // 矩形框的中心位置
DrawOutlinedText(Canvas, FString(总人数), 文字位置, 白色, 黑色, true); // 显示总人数改为红色

} else {
	tslFont->LegacyFontSize = 32;
std::string 文字 = "";
// 定义矩形的宽度和高度
float rectWidth = 50; // 矩形宽度
float rectHeight = 40; // 矩形高度
// 计算矩形的起始位置（使其居中）
FVector2D rectStartPos = {(float)screenWidth / 2 - rectWidth / 2, 50};
// 绘制填充矩形
DrawFilledRect(Canvas, rectStartPos, rectWidth, rectHeight, FLinearColor(1,0,0,0.0f)); // 填充颜色改为粉色
// 绘制边框
//DrawRectangle(Canvas, rectStartPos, rectWidth, rectHeight, 1.0f, {0.f, 0.f, 0.f, 0.0f}); // 绘制边框改为黑色

// 计算文字居中位置
FVector2D 文字位置 = {(float)screenWidth / 2, 85 + rectHeight / 2}; // 矩形框的中心位置
DrawOutlinedText(Canvas, FString(文字), 文字位置, 绿色, 黑色, true); // 显示安全改为绿色

}
}
}
}

	char info[50];
sprintf(info, OBFUSCATE("T\u00A0e\u00A0l\u00A0e\u00A0g\u00A0r\u00A0a\u00A0m\u00A0:\u00A0@\u00A0H\u00A0A\u00A0S\u00A0R\u00A0E\u00A0U\u00A0C\u00A0h\u00A0e\u00A0a\u00A0t"));
DrawText(Canvas, FString(info), { (float)screenWidth / 8,  screenHeight - (float)screenWidth / 12 }, 红色, 黑色, 25, true);
GUI::SetupCanvas(Canvas,tslFontUI);
GUI::NewFrame(screenSizeX, screenSizeY);
static FVector2D WindowPos = {300,300};
static bool IsOpen = false;
static float TempValue = 0;
float x;
float y;
bool h;
bool a;
bool b;
if(IsOpen == false){
    x=370;
    y=0;
    a = true;
    b = false;
}else if(IsOpen == true){
    a = false;
    b = true;
    x=810;
    y=555;
}
static bool 兰;
if(兰){
    IsOpen=false;
    兰 = false;
}
if(bShowGUI){
    tslFont->LegacyFontSize = 17;
#define TSL_FONT_DEFAULT_SIZE 17
    if (GUI::Window(OBFUSCATE("H\u00A0A\u00A0S\u00A0R\u00A0E\u00A0U\u00A0 \u00A0M\u00A0O\u00A0D"), &WindowPos, {x, y}, IsOpen, TempValue)){
        if (isLogin) {
            GUI::Text("");
            GUI::Text("");
            tslFont->LegacyFontSize = 14;
#define TSL_FONT_DEFAULT_SIZE 14
//static char s[64];
            static std::string err;
            std::string clipboardText = getClipboardText();
            if (strlen(s) == 0) {
                GUI::Button3("請\u00A0將\u00A0密\u00A0鑰\u00A0貼\u00A0到\u00A0此\u00A0處", FVector2D(790, 30));
            } else {
                if (GUI::Button3(s, FVector2D(790, 30))) {}
            }
            tslFont->LegacyFontSize = 17;
#define TSL_FONT_DEFAULT_SIZE 17
            if (GUI::Button2(OBFUSCATE("登\u00A0入"), FVector2D({ 790, 45 }))) { Login(s); };
            if (GUI::Button2(OBFUSCATE("把\u00A0密\u00A0鑰\u00A0放"), FVector2D({ 790, 45 }))) { auto key = getClipboardText();strncpy(s, key.c_str(), sizeof(s));}
            GUI::Text(提示);
        } else {
            tslFont->LegacyFontSize = 11;
            static int tb = 1;
            static int tab = 1;
        //    static bool rrr;
            if (GUI::Button2("", FVector2D(0,4))){}
            if (GUI::ButtonTab(OBFUSCATE("E\u00A0S\u00A0P\u00A0 \u00A0播\u00A0放\u00A0器"), FVector2D{ 110, 35 }, tab == 1)) tab = 1;
            if (GUI::ButtonTab(OBFUSCATE("E\u00A0S\u00A0P\u00A0 \u00A0車\u00A0輛"), FVector2D{ 110, 35 }, tab == 2)) tab = 2;
            if (GUI::ButtonTab(OBFUSCATE("E\u00A0S\u00A0P\u00A0 \u00A0其\u00A0他"), FVector2D{ 110, 35 }, tab == 3)) tab = 3;
            if (GUI::ButtonTab(OBFUSCATE("記\u00A0憶\u00A0體\u00A0駭\u00A0客"), FVector2D{ 110, 35 }, tab == 5)) tab = 5;
            if (GUI::ButtonTab(OBFUSCATE("瞄\u00A0準\u00A0機\u00A0器\u00A0人"), FVector2D{ 110, 35 }, tab == 4)) tab = 4;
            if (GUI::ButtonTab(OBFUSCATE("皮\u00A0膚"), FVector2D{ 110 , 35 }, tab == 6)) tab = 6;
            if (GUI::Button("出\u00A0口", FVector2D(110,35))) { IsOpen = false; }
            GUI::NextColumn(123.0f);
            if (tab == 1) {
                if (GUI::Button2("", FVector2D(0,4))){}
                tslFont->LegacyFontSize = 16;
#define TSL_FONT_DEFAULT_SIZE 16
GUI::Text(到期时间);
                if (GUI::Button2("", FVector2D(0,4))){}
                if (调用z == true) {
                    if (GUI::Button2("運\u00A0行\u00A0一\u00A0切", FVector2D(320,35))) {
                        调用z = false;
                        盒子=true; 骨骼=true; 姓名=true; 距离=true; 枪械=true; 健康=true; 背敌=true; 雷达=true; 炸弹警告=true;
                    }
                } else if (调用z == false) {
                    if (GUI::Button2("運\u00A0行\u00A0一\u00A0切",FVector2D(300,35))) {
                        调用z = true;
                        盒子=false; 骨骼=false; 姓名=false; 距离=false; 枪械=false; 健康=false; 背敌=false; 雷达=false; 炸弹警告=false;
                    }
                }
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0盒\u00A0子", &盒子);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0線", &线);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0骨\u00A0骼", &骨骼);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0姓\u00A0名", &姓名);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0距\u00A0離", &距离);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0武\u00A0器", &枪械);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0健\u00A0康", &健康);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0離\u00A0熒\u00A0幕", &背敌);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0雷\u00A0達", &雷达);
                GUI::Checkbox(" 隱\u00A0藏\u00A0機\u00A0器\u00A0人", &忽略机器人);
            } else if (tab == 2) {
                if (GUI::Button2("", FVector2D(0,4))){}
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0車\u00A0輛", &载具);
                GUI::Checkbox(" 車\u00A0輛\u00A0燃\u00A0料", &载具油量);
                GUI::Checkbox(" 車\u00A0輛\u00A0損\u00A0壤", &载具血量);
            } else if (tab == 3) {
                if (GUI::Button2("", FVector2D(0,4))){}
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0戰\u00A0利\u00A0品\u00A0箱", &战利品);
                GUI::Checkbox(" E\u00A0S\u00A0P\u00A0 \u00A0空\u00A0投\u00A0箱", &空投箱);
                GUI::Checkbox(" 炸\u00A0彈\u00A0警\u00A0告", &炸弹警告);
                GUI::Checkbox(" 追\u00A0蹤\u00A0炸\u00A0彈", &投掷物预警);
                GUI::Checkbox(" 比\u00A0賽\u00A0資\u00A0訊", &对局信息);
            } else if (tab == 4) {
                GUI::Text("");
                GUI::Checkbox(" 瞄\u00A0準\u00A0機\u00A0器\u00A0人", &Settings.Aim.aim);
                GUI::SameLine();
                GUI::Checkbox(" 瞄\u00A0準\u00A0順\u00A0利", &平滑);
                GUI::Combobox(OBFUSCATE("地\u00A0點"), FVector2D{ 390,45 }, &Settings.Aim.location, "頭","胸\u00A0部","L\u00A0e\u00A0g");
                GUI::SliderFloat("瞄\u00A0準\u00A0距\u00A0離", &自瞄距离, 1, 400, "%.1f");
                GUI::SliderFloat("視\u00A0場\u00A0大\u00A0小", &Settings.Aim.fovsize, 1,400, "%.1f");
                GUI::SliderFloat(OBFUSCATE("平\u00A0滑\u00A0順\u00A0利"), &XY, 0, 80.0f, "%.1f");
                Settings.Aim.speed = XY / 16.0f;
                GUI::Combobox(OBFUSCATE("扳\u00A0機"), FVector2D{ 390,45 }, &Settings.Aim.trigger, "射\u00A0擊","瞄\u00A0準","任\u00A0何");
                GUI::Checkbox(" 可\u00A0見\u00A0的", &Settings.Aim.vischeck);
                GUI::SameLine();
                GUI::Checkbox(" 忽\u00A0略\u00A0角\u00A0落", &Settings.Aim.knocked);
                GUI::SameLine();
                GUI::Checkbox(" 預\u00A0言", &預言);
                GUI::Checkbox(" 忽\u00A0略\u00A0機\u00A0器\u00A0人", &Settings.Aim.nobot);
                GUI::SameLine();
                GUI::Checkbox(" 瞄\u00A0准\u00A0后\u00A0坐\u00A0力", &后座);
            } else if (tab == 5) {
                if (GUI::Button2("", FVector2D(0,4))){}
                GUI::Checkbox(" 广\u00A0角", &广角);
            } else if (tab == 6) {
                GUI::Checkbox("   皮\u00A0膚\u00A0美\u00A0化", &自改衣服);
                if (衣服 == 0) {
                    if (GUI::Button("衣\u00A0服\u00A0：\u00A0無", FVector2D(360.f, 40.f)));
                    衣服值 = 1400129;
                } else if (衣服 == 1) {
                    if (GUI::Button("服\u00A0裝\u00A0：\u00A0法\u00A0老", FVector2D(360.f, 40.f)));
                    衣服值 = 1406469;
                } else if (衣服 == 2) {
                    if (GUI::Button("衣\u00A0服\u00A0：\u00A0溺\u00A0水\u00A0者\u00A0之\u00A0王", FVector2D(360.f, 40.f)));
                    衣服值 = 1406891;
                } else if (衣服 == 3) {
                    if (GUI::Button("衣\u00A0服\u00A0：\u00A0飛\u00A0鳥", FVector2D(360.f, 40.f)));
                    衣服值 = 1406387;
                } else if (衣服 == 4) {
                    if (GUI::Button("衣\u00A0服\u00A0：\u00A0媽\u00A0咪", FVector2D(360.f, 40.f)));
                    衣服值 = 1400687;
                } else if (衣服 == 5) {
                    if (GUI::Button("衣\u00A0服\u00A0：\u00A0冥\u00A0界\u00A0旅\u00A0行\u00A0者", FVector2D(360.f, 40.f)));
                    衣服值 = 1405623;
                } else if (衣服 == 6) {
                    if (GUI::Button("服\u00A0裝\u00A0：\u00A0A\u00A0u\u00A0r\u00A0o\u00A0r\u00A0a\u00A0 \u00A0F\u00A0l\u00A0o\u00A0r\u00A0a", FVector2D(360.f, 40.f)));
                    衣服值 = 1407103;
                } else if (衣服 == 7) {
                    if (GUI::Button("服\u00A0裝\u00A0：\u00A0深\u00A0淵\u00A0之\u00A0魂", FVector2D(360.f, 40.f)));
                    衣服值 = 1406970;
                } else if (衣服 == 8) {
                    if (GUI::Button("服\u00A0裝\u00A0：\u00A0精\u00A0靈\u00A0王", FVector2D(360.f, 40.f)));
                    衣服值 = 1407142;
                } else if (衣服 == 9) {
                    if (GUI::Button("服\u00A0飾\u00A0：\u00A0神\u00A0羽", FVector2D(360.f, 40.f)));
                    衣服值 = 1407276;
                } else if (衣服 == 10) {
                    if (GUI::Button("服\u00A0裝\u00A0：\u00A0黑\u00A0燼\u00A0天\u00A0使", FVector2D(360.f, 40.f)));
                    衣服值 = 1407275;
                } else if (衣服 == 11) {
                    if (GUI::Button("服\u00A0裝\u00A0：\u00A0熾\u00A0烈\u00A0古\u00A0神", FVector2D(360.f, 40.f)));
                    衣服值 = 1407277;
                } else if (衣服 == 12) {
                    if (GUI::Button("服\u00A0裝\u00A0：\u00A0功\u00A0夫\u00A0大\u00A0師", FVector2D(360.f, 40.f)));
                    衣服值 = 1406742;
                } else if (衣服 == 13) {
                    if (GUI::Button("衣\u00A0服\u00A0：\u00A0孫\u00A0悟\u00A0空", FVector2D(360.f, 40.f)));
                    衣服值 = 1406327;
                }
                if (GUI::Button("換\u00A0衣\u00A0服", FVector2D(360.f, 40.f))) {
                    if (衣服 == 13) {
                        衣服 = 0;
                    } else {
                        衣服 = 衣服 + 1;
						}
                    }
                }
            }
        }
    }
}


g_LocalPlayer = localPlayer;
g_PlayerController = localPlayerController;

	return orig_PostRender(ViewportClient, Canvas);
}
int32_t f_mprotect(uintptr_t addr, size_t len, int32_t prot)
{
    size_t page_size = static_cast<size_t>(4096);
    void* start = reinterpret_cast<void*>(addr & -page_size);
    uintptr_t end = (addr + len + page_size - 1) & -page_size;
    return mprotect((void *)start, end - reinterpret_cast<uintptr_t>(start), prot);
}
int Hook_Ret() {
return 0;
}
int HookPointer(uintptr_t VTableAddr, void *Hook_Sub, void **Original_Sub)
{
    if (!VTableAddr || !Hook_Sub)
    {
        return 1;
    }

    void **VTable = (void **)VTableAddr;
    void *Original = *VTable;

    if (Original_Sub)
    {
        *Original_Sub = Original;
    }

    f_mprotect((uintptr_t)VTableAddr, sizeof(void *), PROT_READ | PROT_WRITE);

    *VTable = Hook_Sub;

    f_mprotect((uintptr_t)VTableAddr, sizeof(void *), PROT_READ);

    return 0;
}
//typedef unsigned long _QWORD;
typedef __int64 int64;
int 少年Ret() {return 0;}

int CrashFix(int domain, int type, int protocol)
{
if ((int)domain!=0)
{
return sleep(999999999);
}
return socket(domain, type, protocol);
}
size_t AliceStrlen(const char *s) {
static const std::unordered_map<std::string, std::string> replacements = {
{"2200101", "12208701"}, //你好
{"2200301", "12212201"}, //鼓掌
{"2200201", "12200701"}, //谢谢
{"202408001", "202408053"},//大厅背景
{"40601001", "40605012"}, //发型A改双马尾
{"403003", "1404049"}, //T恤（条纹改）黄鲨鱼
{"404026", "1404050"}, //金典裤子改黄鲨鱼裤子
{"402019", "1403719"}, //放形眼镜改科技战衣眼罩
{"101001100", "1101001213"},//"AKM自动步枪（破损）"
{"101001200", "1101001213"},//"AKM自动步枪（修复）"
{"101001300", "1101001213"},//"AKM自动步枪（完好）"
{"101001400", "1101001213"},//"AKM自动步枪（改进）"
{"101001500", "1101001213"},//"AKM自动步枪（精制）"
{"101001600", "1101001213"},//"AKM自动步枪（独眼蛇）"
{"101001700", "1101001213"},//"AKM自动步枪（钢铁阵线）"
{"101002100", "1101002081"},//"M16A4自动步枪（破损）"
{"101002200", "1101002081"},//"M16A4自动步枪（修复）"
{"101002300", "1101002081"},//"M16A4自动步枪（完好）"
{"101002400", "1101002081"},//"M16A4自动步枪（改进）"
{"101002500", "1101002081"},//"M16A4自动步枪（精制）"
{"101003100", "1101003188"},//"SCAR-L自动步枪（破损）"
{"101003200", "1101003188"},//"SCAR-L自动步枪（修复）"
{"101003300", "1101003188"},//"SCAR-L自动步枪（完好）"
{"101003400", "1101003188"},//"SCAR-L自动步枪（改进）"
{"101003500", "1101003188"},//"SCAR-L自动步枪（精制）"
{"101003600", "1101003188"},//"SCAR-L自动步枪（独眼蛇）"
{"101003700", "11010031885"},//"SCAR-L自动步枪（钢铁阵线）"
{"101004100", "1101004236"},//"M416自动步枪（破损）"
{"101004200", "1101004236"},//"M416自动步枪（修复）"
{"101004300", "1101004236"},//"M416自动步枪（完好）"
{"101004400", "1101004236"},//"M416自动步枪（改进）"
{"101004500", "1101004236"},//"M416自动步枪（精制）"
{"101004600", "1101004236"},//"M416自动步枪（独眼蛇）"
{"101004700", "1101004236"},//"M416自动步枪（钢铁阵线）"
{"101005100", "10100500"},//"GROZA自动步枪（破损）"
{"101005200", "10100500"},//"GROZA自动步枪（修复）"
{"101005300", "10100500"},//"GROZA自动步枪（完好）"
{"101005400", "10100500"},//"GROZA自动步枪（改进）"
{"101005500", "10100500"},//"GROZA自动步枪（精制）"
{"101005600", "10100500"},//"GROZA自动步枪（独眼蛇）"
{"101005700", "10100500"},//"GROZA自动步枪（钢铁阵线）"
{"101006100", "10100600"},//"AUG自动步枪（破损）"
{"101006200", "10100600"},//"AUG自动步枪（修复）"
{"101006300", "10100600"},//"AUG自动步枪（完好）"
{"101006400", "10100600"},//"AUG自动步枪（改进）"
{"101006500", "10100600"},//"AUG自动步枪（精制）"
{"101006600", "10100600"},//"AUG自动步枪（独眼蛇）"
{"101006700", "10100600"},//"AUG自动步枪（钢铁阵线）"
{"101007100", "1101007025"},//"QBZ自动步枪（破损）"
{"101007200", "1101007025"},//"QBZ自动步枪（修复）"
{"101007300", "1101007025"},//"QBZ自动步枪（完好）"
{"101007400", "1101007025"},//"QBZ自动步枪（改进）"
{"101007500", "1101007025"},//"QBZ自动步枪（精制）"
{"101007600", "1101007025"},//"QBZ自动步枪（独眼蛇）"
{"101007700", "1101007025"},//"QBZ自动步枪（钢铁阵线）"
{"101008100", "10100800"},//"M762自动步枪（破损）"
{"101008200", "10100800"},//"M762自动步枪（修复）"
{"101008300", "10100800"},//"M762自动步枪（完好）"
{"101008400", "10100800"},//"M762自动步枪（改进）"
{"101008500", "10100800"},//"M762自动步枪（精制）"
{"101008600", "10100800"},//"M762自动步枪（独眼蛇）"
{"101008700", "10100800"},//"M762自动步枪（钢铁阵线）"
{"101008900", "10100800"},//"M762自动步枪"
{"101009100", "1101009003"},//"Mk47自动步枪（破损）"
{"101009200", "1101009003"},//"Mk47自动步枪（修复）"
{"101009300", "1101009003"},//"Mk47自动步枪（完好）"
{"101009400", "1101009003"},//"Mk47自动步枪（改进）"
{"101009500", "1101009003"},//"Mk47自动步枪（精制）"
{"101010100", "1101010012"},//"G36C自动步枪（破损）"
{"101010200", "1101010012"},//"G36C自动步枪（修复）"
{"101010300", "1101010012"},//"G36C自动步枪（完好）"
{"101010400", "1101010012"},//"G36C自动步枪（改进）"
{"101010500", "1101010012"},//"G36C自动步枪（精制）"
{"101010600", "1101010012"},//"G36C自动步枪（独眼蛇）"
{"101010700", "1101010012"},//"G36C自动步枪（钢铁阵线）"
{"101012100", "1101012013"},//"蜜獾自动步枪（破损）"
{"101012200", "1101012013"},//"蜜獾自动步枪（修复）"
{"101012300", "1101012013"},//"蜜獾自动步枪（完好）"
{"101012400", "1101012013"},//"蜜獾自动步枪（改进）"
{"101012500", "1101012013"},//"蜜獾自动步枪（精制）"
{"101012600", "1101012013"},//"蜜獾自动步枪（独眼蛇）"
{"101012700", "1101012013"},//"蜜獾自动步枪（钢铁阵线）"
{"101100100", "1101100012"},//"FAMAS自动步枪（破损）"
{"101100200", "1101100012"},//"FAMAS自动步枪（修复）"
{"101100300", "1101100012"},//"FAMAS自动步枪（完好）"
{"101100400", "1101100012"},//"FAMAS自动步枪（改进）"
{"101100500", "1101100012"},//"FAMAS自动步枪（精制）"
{"101100600", "1101100012"},//"FAMAS自动步枪（独眼蛇）"
{"101100700", "1101100012"},//"FAMAS自动步枪（钢铁阵线）"
{"101102100", "1101102027"},//"ACE32自动步枪（破损）"
{"101102200", "1101102027"},//"ACE32自动步枪（修复）"
{"101102300", "1101102027"},//"ACE32自动步枪（完好）"
{"101102400", "1101102027"},//"ACE32自动步枪（改进）"
{"101102500", "1101102027"},//"ACE32自动步枪（精制）"
{"101102600", "1101102027"},//"ACE32自动步枪（独眼蛇）"
{"101102700", "1101102027"},//"ACE32自动步枪（钢铁阵线）"
{"102001100", "1102001036"},//"UZI冲锋枪（破损）"
{"102001200", "1102001036"},//"UZI冲锋枪（修复）"
{"102001300", "1102001036"},//"UZI冲锋枪（完好）"
{"102001400", "1102001036"},//"UZI冲锋枪（改进）"
{"102001500", "1102001036"},//"UZI冲锋枪（精制）"
{"102002100", "1102002053"},//"UMP45冲锋枪（破损）"
{"102002200", "1102002053"},//"UMP45冲锋枪（修复）"
{"102002300", "1102002053"},//"UMP45冲锋枪（完好）"
{"102002400", "1102002053"},//"UMP45冲锋枪（改进）"
{"102002500", "1102002053"},//"UMP45冲锋枪（精制）"
{"102002900", "1102002053"},//"UMP45冲锋枪"
{"102003100", "1102003091"},//"Vector冲锋枪（破损）"
{"102003200", "1102003091"},//"Vector冲锋枪（修复）"
{"102003300", "1102003091"},//"Vector冲锋枪（完好）"
{"102003400", "1102003091"},//"Vector冲锋枪（改进）"
{"102003500", "1102003091"},//"Vector冲锋枪（精制）"
{"102003600", "1102003091"},//"Vector冲锋枪（独眼蛇）"
{"102003700", "1102003091"},//"Vector冲锋枪（钢铁阵线）"
{"102004100", "1102004018"},//"汤姆逊冲锋枪（破损）"
{"102004200", "1102004018"},//"汤姆逊冲锋枪（修复）"
{"102004300", "1102004018"},//"汤姆逊冲锋枪（完好）"
{"102004400", "1102004018"},//"汤姆逊冲锋枪（改进）"
{"102004500", "1102004018"},//"汤姆逊冲锋枪（精制）"
{"102005100", "1102005007"},//"野牛冲锋枪（破损）"
{"102005200", "1102005007"},//"野牛冲锋枪（修复）"
{"102005300", "1102005007"},//"野牛冲锋枪（完好）"
{"102005400", "1102005007"},//"野牛冲锋枪（改进）"
{"102005500", "1102005007"},//"野牛冲锋枪（精制）"
{"102005900", "1102005007"},//"野牛冲锋枪"
{"102007100", "1107001019"},//"MP5K冲锋枪（破损）"
{"102007200", "1107001019"},//"MP5K冲锋枪（修复）"
{"102007300", "1107001019"},//"MP5K冲锋枪（完好）"
{"102007400", "1107001019"},//"MP5K冲锋枪（改进）"
{"102007500", "1107001019"},//"MP5K冲锋枪（精制）"
{"102007600", "1107001019"},//"MP5K冲锋枪（独眼蛇）"
{"102007700", "1107001019"},//"MP5K冲锋枪（钢铁阵线）"
{"102105100", "1102105002"},//"P90冲锋枪（破损）"
{"102105200", "1102105002"},//"P90冲锋枪（修复）"
{"102105300", "1102105002"},//"P90冲锋枪（完好）"
{"102105400", "1102105002"},//"P90冲锋枪（改进）"
{"102105500", "1102105002"},//"P90冲锋枪（精制）"
{"102105600", "1102105002"},//"P90冲锋枪（独眼蛇）"
{"102105700", "1102105002"},//"P90冲锋枪（钢铁阵线）"
{"103001100", "1103001085"},//"Kar98K狙击枪（破损）"
{"103001200", "1103001085"},//"Kar98K狙击枪（修复）"
{"103001300", "1103001085"},//"Kar98K狙击枪（完好）"
{"103001400", "1103001085"},//"Kar98K狙击枪（改进）"
{"103001500", "1103001085"},//"Kar98K狙击枪（精制）"
{"103001900", "1103001085"},//"Kar98K狙击枪"
{"103002100", "1103002030"},//"M24狙击枪（破损）"
{"103002200", "1103002030"},//"M24狙击枪（修复）"
{"103002300", "1103002030"},//"M24狙击枪（完好）"
{"103002400", "1103002030"},//"M24狙击枪（改进）"
{"103002500", "1103002030"},//"M24狙击枪（精制）"
{"103002600", "1103002030"},//"M24狙击枪（独眼蛇）"
{"103002700", "1103002030"},//"M24狙击枪（钢铁阵线）"
{"103003100", "1103003087"},//"AWM狙击枪（破损）"
{"103003200", "1103003087"},//"AWM狙击枪（修复）"
{"103003300", "1103003087"},//"AWM狙击枪（完好）"
{"103003400", "1103003087"},//"AWM狙击枪（改进）"
{"103003500", "1103003087"},//"AWM狙击枪（精制）"
{"103003600", "1103003087"},//"AWM狙击枪（独眼蛇）"
{"103003700", "1103003087"},//"AWM狙击枪（钢铁阵线）"
{"103003900", "1103003087"},//"AWM狙击枪"
{"103004100", "1103004020"},//"SKS狙击枪（破损）"
{"103004200", "1103004020"},//"SKS狙击枪（修复）"
{"103004300", "1103004020"},//"SKS狙击枪（完好）"
{"103004400", "1103004020"},//"SKS狙击枪（改进）"
{"103004500", "1103004020"},//"SKS狙击枪（精制）"
{"103004600", "1103004020"},//"SKS狙击枪（独眼蛇）"
{"103004700", "1103004020"},//"SKS狙击枪（钢铁阵线）"
{"103005100", "1103005033"},//"VSS狙击枪（破损）"
{"103005200", "1103005033"},//"VSS狙击枪（修复）"
{"103005300", "1103005033"},//"VSS狙击枪（完好）"
{"103005400", "1103005033"},//"VSS狙击枪（改进）"
{"103005500", "1103005033"},//"VSS狙击枪（精制）"
{"103005900", "1103005033"},//"VSS狙击枪"
{"103006100", "1103006030"},//"Mini14狙击枪（破损）"
{"103006200", "1103006030"},//"Mini14狙击枪（修复）"
{"103006300", "1103006030"},//"Mini14狙击枪（完好）"
{"103006400", "1103006030"},//"Mini14狙击枪（改进）"
{"103006500", "1103006030"},//"Mini14狙击枪（精制）"
{"103006900", "1103006030"},//"Mini14狙击枪"
{"103007100", "1103007028"},//"Mk14狙击枪（破损）"
{"103007200", "1103007028"},//"Mk14狙击枪（修复）"
{"103007300", "1103007028"},//"Mk14狙击枪（完好）"
{"103007400", "1103007028"},//"Mk14狙击枪（改进）"
{"103007500", "1103007028"},//"Mk14狙击枪（精制）"
{"103007600", "1103007028"},//"Mk14狙击枪（独眼蛇）"
{"103007700", "1103007028"},//"Mk14狙击枪（钢铁阵线）"
{"103008100", "1103008014"},//"Win94狙击枪（破损）"
{"103008200", "1103008014"},//"Win94狙击枪（修复）"
{"103008300", "1103008014"},//"Win94狙击枪（完好）"
{"103008400", "1103008014"},//"Win94狙击枪（改进）"
{"103008500", "1103008014"},//"Win94狙击枪（精制）"
{"103009100", "1103009039"},//"SLR狙击枪（破损）"
{"103009200", "1103009039"},//"SLR狙击枪（修复）"
{"103009300", "1103009039"},//"SLR狙击枪（完好）"
{"103009400", "1103009039"},//"SLR狙击枪（改进）"
{"103009500", "1103009039"},//"SLR狙击枪（精制）"
{"103009600", "1103009039"},//"SLR狙击枪（独眼蛇）"
{"103009700", "1103009039"},//"SLR狙击枪（钢铁阵线）"
{"103009900", "1103009039"},//"SLR狙击枪"
{"103010100", "1103010018"},//"QBU狙击枪（破损）"
{"103010200", "1103010018"},//"QBU狙击枪（修复）"
{"103010300", "1103010018"},//"QBU狙击枪（完好）"
{"103010400", "1103010018"},//"QBU狙击枪（改进）"
{"103010500", "1103010018"},//"QBU狙击枪（精制）"
{"103012100", "1103012010"},//"AMR狙击枪（破损）"
{"103012200", "1103012010"},//"AMR狙击枪（修复）"
{"103012300", "1103012010"},//"AMR狙击枪（完好）"
{"103012400", "1103012010"},//"AMR狙击枪（改进）"
{"103012500", "1103012010"},//"AMR狙击枪（精制）"
{"103012600", "1103012010"},//"AMR狙击枪（独眼蛇）"
{"103012700", "1103012010"},//"AMR狙击枪（钢铁阵线）"
{"103100100", "1103100013"},//"Mk12狙击枪（破损）"
{"103100200", "1103100013"},//"Mk12狙击枪（修复）"
{"103100300", "1103100013"},//"Mk12狙击枪（完好）"
{"103100400", "1103100013"},//"Mk12狙击枪（改进）"
{"103100500", "1103100013"},//"Mk12狙击枪（精制）"
{"103100600", "1103100013"},//"Mk12狙击枪（独眼蛇）"
{"103100700", "1103100013"},//"Mk12狙击枪（钢铁阵线）"
{"104001100", "1104001030"},//"S686霰弹枪（破损）"
{"104001200", "1104001030"},//"S686霰弹枪（修复）"
{"104001300", "1104001030"},//"S686霰弹枪（完好）"
{"104001400", "1104001030"},//"S686霰弹枪（改进）"
{"104001500", "1104001030"},//"S686霰弹枪（精制）"
{"104002100", "1104002036"},//"S1897霰弹枪（破损）"
{"104002200", "1104002036"},//"S1897霰弹枪（修复）"
{"104002300", "1104002036"},//"S1897霰弹枪（完好）"
{"104002400", "1104002036"},//"S1897霰弹枪（改进）"
{"104002500", "1104002036"},//"S1897霰弹枪（精制）"
{"104003100", "1104003039"},//"S12K霰弹枪（破损）"
{"104003200", "1104003039"},//"S12K霰弹枪（修复）"
{"104003300", "1104003039"},//"S12K霰弹枪（完好）"
{"104003400", "1104003039"},//"S12K霰弹枪（改进）"
{"104003500", "1104003039"},//"S12K霰弹枪（精制）"
{"104003600", "1104003039"},//"S12K霰弹枪（独眼蛇）"
{"104003700", "1104003039"},//"S12K霰弹枪（钢铁阵线）"
{"104003900", "1104003039"},//"S12K霰弹枪"
{"104004100", "1104004024"},//"DBS霰弹枪（破损）"
{"104004200", "1104004024"},//"DBS霰弹枪（修复）"
{"104004300", "1104004024"},//"DBS霰弹枪（完好）"
{"104004400", "1104004024"},//"DBS霰弹枪（改进）"
{"104004500", "1104004024"},//"DBS霰弹枪（精制）"
{"104004600", "1104004024"},//"DBS霰弹枪（独眼蛇）"
{"104004700", "1104004024"},//"DBS霰弹枪（钢铁阵线）"
{"104004900", "1104004024"},//"DBS霰弹枪"
{"104102100", "1104102001"},//"NS2000霰弹枪（破损）"
{"104102200", "1104102001"},//"NS2000霰弹枪（修复）"
{"104102300", "1104102001"},//"NS2000霰弹枪（完好）"
{"104102400", "1104102001"},//"NS2000霰弹枪（改进）"
{"104102500", "1104102001"},//"NS2000霰弹枪（精制）"
{"105001100", "1105001020"},//"M249轻机枪（破损）"
{"105001200", "1105001020"},//"M249轻机枪（修复）"
{"105001300", "1105001020"},//"M249轻机枪（完好）"
{"105001400", "1105001020"},//"M249轻机枪（改进）"
{"105001500", "1105001020"},//"M249轻机枪（精制）"
{"105001600", "1105001020"},//"M249轻机枪（独眼蛇）"
{"105001700", "1105001020"},//"M249轻机枪（钢铁阵线）"
{"105002100", "1105002011"},//"DP-28轻机枪（破损）"
{"105002200", "1105002011"},//"DP-28轻机枪（修复）"
{"105002300", "1105002011"},//"DP-28轻机枪（完好）"
{"105002400", "1105002011"},//"DP-28轻机枪（改进）"
{"105002500", "1105002011"},//"DP-28轻机枪（精制）"
{"105002900", "1105002011"},//"DP-28轻机枪"
{"105010100", "1105010019"},//"MG3轻机枪（破损）"
{"105010200", "1105010019"},//"MG3轻机枪（修复）"
{"105010300", "1105010019"},//"MG3轻机枪（完好）"
{"105010400", "1105010019"},//"MG3轻机枪（改进）"
{"105010500", "1105010019"},//"MG3轻机枪（精制）"
{"105010600", "1105010019"},//"MG3轻机枪（独眼蛇）"
{"105010700", "1105010019"},//"MG3轻机枪（钢铁阵线）"
{"105010900", "1105010019"},//"MG3轻机枪"
{"106010100", "1106010002"},//"沙漠之鹰手枪（破损）"
{"106010200", "1106010002"},//"沙漠之鹰手枪（修复）"
{"106010300", "1106010002"},//"沙漠之鹰手枪（完好）"
{"106010400", "1106010002"},//"沙漠之鹰手枪（改进）"
{"106010500", "1106010002"},//"沙漠之鹰手枪（精制）"
{"106010900", "1106010002"},//"沙漠之鹰手枪（指挥官）"
{"107001100", "1107001011"},//"十字弩（破损）"
{"107001200", "1107001011"},//"十字弩（修复）"
{"107001300", "1107001011"},//"十字弩（完好）"
{"107001400", "1107001011"},//"十字弩（改进）"
{"107001500", "1107001011"},//"十字弩（精制）"
{"10100100", "1101001213"}, //
{"10100200", "1101002081"}, //
{"10100300", "1101003188"}, //
{"10100400", "1101004236"}, //
{"10100500", "1101005098"}, //
{"10100600", "1101006075"}, //
{"10100800", "1101008154"}, //
{"10100900", "1101009001"}, //
{"10101000", "1101010020"}, //
{"10101200", "1101100012"}, //
{"10110000", "1101102017"}, //
{"10110200", "1101102017"}, //
//突击步枪上面
{"10300100", "1103001191"}, //
{"10300200", "1103002156"}, //
{"10300300", "1103003062"}, //
{"10300800", "1103008020"}, //
{"10301100", "1103011001"}, //
{"10301200", "1103012031"}, //
//狙击枪上面
{"10300400", "1103004037"}, //
{"10300500", "1103005033"}, //
{"10300600", "1103006030"}, //
{"10300700", "1103007028"}, //
{"10300900", "1103009037"}, //
{"10301000", "1103010015"}, //
{"10310000", "1103100007"}, //
//射手枪
{"10200100", "1102001120"}, //
{"10200200", "1102002424"}, //
{"10200300", "1102003080"}, //
{"10200400", "1102004049"}, //
{"10200500", "1102005064"}, //
{"10200700", "1107001019"}, //
{"10210500", "1102105012"}, //
//冲锋枪上面
{"10400100", "1104001030"}, //
{"10400200", "1104002038"}, //
{"10400300", "1104003038"}, //
{"10400400", "1104004015"}, //
{"10410100", "1104101001"}, //
{"10410200", "1104102004"}, //
//喷子上面
{"10500100", "1105001069"}, //
{"10500200", "1105002091"}, //
{"10501000", "1105010019"}, //
//轻机枪上面
{"403192", "1406891"}, //
{"1405884", "1400687"}, //
{"1405548", "1406387"}, //
//身法下面
{"1400356", "1404133"}, //
{"404072", "1404134"}, //
{"401020", "1410085"}, //
{"402021", "1400165"}, //
//下面背景
{"20300100", "1030070218"}, //妹控8x 狙击通用
{"20300200", "1030070217"}, //妹控8x 狙击通用
{"20300300", "1030070216"}, //妹控8x 狙击通用
{"20300400", "1030070214"}, //妹控8x 狙击通用
{"20300500", "1030070212"}, //妹控8x 狙击通用
{"20301500", "1030070213"}, //封印幽冥 6x
{"20301400", "1030070215"}, //封印幽冥 3x
//不知道
{"1080051", "1081201"}, // 惩戒者雷呜
{"1080041", "1081201"},
{"1080011", "1081201"},
{"108004100", "108120100"},//惩戒者·雷鸣
{"10800100", "108120100"},//惩戒者·雷鸣
{"10800200", "108120100"},//惩戒者·雷鸣
{"10800300", "108120100"},//惩戒者·雷鸣
{"10800400", "108120100"},//惩戒者·雷鸣
{"10800500", "108120100"},//惩戒者·雷鸣
{"9002001","9002002"}, // 测试播报
{"2200201", "12220436"},
{"1010010233","1030070276"},// 盒子	
{"1010010233","1030070286"},// 盒子
};
auto it = replacements.find(s);
if (it != replacements.end()) {
strcpy((char *)s, it->second.c_str());
}
return strlen(s);
}
int 北欲() {
    return 120LL;
}
void Accelerate()
{
}
void* MainThread(void*) {
	while (!UE4) {
		UE4 = GetModule(OBFUSCATE("libUE4.so"));			
		sleep(1);

	}
	
	while (!Anogs) {
        Anogs = GetModule(OBFUSCATE("libanogs.so"));       
     sleep(1); 

    }
        
    while (!CrashSight) {
        CrashSight = GetModule(OBFUSCATE("libCrashSight.so"));
        sleep(1); 
    }
	while (!g_App) {
		g_App = *(android_app**)(UE4 + GNativeAndroidApp_Offset);
		sleep(1);
	}
	FName::GNames = GetGNames();
	while (!FName::GNames) {
		FName::GNames = GetGNames();
		sleep(1);
	}
	static bool loadFont = false;
	if (!loadFont)
	{
		pthread_t t;
		pthread_create(&t, 0, LoadFont, 0);
		loadFont = true;
	}
	if (g_App->onInputEvent != onInputEvent) {
		orig_onInputEvent = decltype(orig_onInputEvent)(g_App->onInputEvent);
		g_App->onInputEvent = onInputEvent;
	}
	UObject::GUObjectArray = (FUObjectArray*)(UE4 + GUObject_Offset);
GlossHook((void*)(UE4 + 0x9e792c4), (void*)new_PostRender, (void**)&orig_PostRender);
HOOK_LIB_NO_ORIG("libUE4.so", "0x0c8450a0", 美化皮肤);
GlossHook((void *)(UE4 + 0x65B67E8), (void *)小奈追踪, (void **)&orig_小奈追踪);
/*MemoryPatch::createWithHex("libgold.so",0xFC168,"1F 20 03 D5").Modify();//孤狼杀卡密验证
MemoryPatch::createWithHex("libgold.so",0xC732C,"C0 03 5F D6 FF FF FF FF").Modify();//孤狼杀UI
MemoryPatch::createWithHex("libgold.so",0xF5230,"00 00 80 D2 C0 03 5F D6").Modify(); //孤狼离线自杀
*/
HOOK_LIB_NO_ORIG("libUE4.so", "0x608D118",北欲);
return 0;
}
void * xT_thread(void *) {
       do {
        sleep(1);
} while (!isLibraryLoaded("libanogs.so"));
MemoryPatch::createWithHex("libanogs.so", 0x4DC888, "1F 20 03 D5 1F 20 03 D5").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x4DC898, "00 00 80 D2 1F 20 03 D5").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x4DC8B4, "1F 20 03 D5 1F 20 03 D5").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x4DC8D8, "1F 20 03 D5 1F 20 03 D5").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x4DC8E0, "1F 20 03 D5 1F 20 03 D5").Modify();
PATCH_LIB("libanogs.so", "0x3E2FB0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1391E0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2DFD70", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2DEC64", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x139300", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x139370", "00 00 80 D2 C0 03 5F D6");
写入D类(anogs+0x2A6818,-1442839576);
return 0;
}
__attribute__((constructor)) void _init() {
	pthread_t Ptid;
	pthread_create(&Ptid, 0, MainThread, 0);
	pthread_t Ptidy;
	pthread_create(&Ptidy, 0, xT_thread, 0);
}
