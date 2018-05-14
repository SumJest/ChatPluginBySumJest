#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <windows.h>
#include <string>
#include <assert.h>
#include <process.h>
#include <sys/stat.h>

#include "SAMPFUNCS_API.h"
#include "game_api\game_api.h"
#include "IniReader.h"

SAMPFUNCS *SF = new SAMPFUNCS();
stFontInfo *pFont;

//bool isInChat()
//{
//	DWORD dwPtr = SF->getSAMP()->getSAMPAddr() + 0x21a10c;
//	DWORD* dwAddress = ((DWORD*)dwPtr) + 0x55;
//	DWORD* InChat = (DWORD*)dwAddress;
//	SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(0, 0xAA, 0), "%d",InChat);
//	if (InChat>0)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//
//}
inline bool exists_test3(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

bool CALLBACK Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride,
	CONST RGNDATA *pDirtyRegion)
{
	if (SUCCEEDED(SF->getRender()->BeginRender())) 
	{
		if (SF->getSAMP()->getInput()->iInputEnabled)
		{
			char text[260];
			DWORD b = *(DWORD *)(*(DWORD *)(((DWORD)GetModuleHandleA("samp.dll")) + 0x21A0E8) + 0x8);
			DWORD X = *(DWORD *)(b + 0x8);
			DWORD Y = *(DWORD *)(b + 0xC);
			Y += 50;
			int id = SF->getSAMP()->getPlayers()->sLocalPlayerID;
			SF->getSAMP()->getInfo()->UpdateScoreAndPing();
			sprintf(text,"Ваш ID: {00ff00} %d {ffffff}| Ваш ЛВЛ: {ff8000}%d {ffffff}| Ваш Пинг: {800080}%d {ffffff}| Ваш Ник: %s",id, SF->getSAMP()->getPlayers()->iLocalPlayerScore,SF->getSAMP()->getPlayers()->iLocalPlayerPing,SF->getSAMP()->getPlayers()->szLocalPlayerName);
			pFont->Print(text, D3DCOLOR_ARGB(255, 255, 255, 255),X, Y, false);
		}
		SF->getRender()->EndRender(); 
	};

	return true; 
};

void CALLBACK mainloop()
{
	static bool init = false;
	if (!init)
	{
		if (GAME == nullptr)
			return;
		if (GAME->GetSystemState() != eSystemState::GS_PLAYING_GAME)
			return;
		if (!SF->getSAMP()->IsInitialized())
		return;

		if (!exists_test3("SAMPFUNCS\\chatplugin.ini")) 
		{ 
			SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(0xFF, 0xB3, 0x00), "[ChatPlugin] {ffffff}File{FF0000} \"chatplugin.ini\"{ffffff} not found\!");
			pFont = SF->getRender()->CreateNewFont("Segoe UI", 14, FCR_BORDER);
		}
		else
		{
			CIniReader iniReader("SAMPFUNCS\\chatplugin.ini");
			int iHeight = iniReader.ReadInteger("main","Height",14);
			int iFlag = iniReader.ReadInteger("main","Flag",4); 
			pFont = SF->getRender()->CreateNewFont("Segoe UI", iHeight, iFlag);
		}
		SF->getRender()->registerD3DCallback(eDirect3DDeviceMethods::D3DMETHOD_PRESENT, Present);

		SF->getSAMP()->getChat()->AddChatMessage(D3DCOLOR_XRGB(0xFF, 0xB3, 0x00), "[ChatPlugin] {ffffff}loaded by {FF0000} SumJest {ffffff}!");
		

		init = true;
	}


}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	switch (dwReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
			SF->initPlugin(mainloop, hModule);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
