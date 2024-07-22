#include <stdio.h>

typedef void (*Game_fp)();
void InitLoadGameLib();
Game_fp LoadGameLib(const char *);

#ifdef GAGE_BUILD_DLL
#include <windows.h>
static HMODULE g_game_dll = 0;

void InitLoadGameLib() {
    SetDllDirectoryA(".");
}
Game_fp LoadGameLib(const char * path) {
    g_game_dll = LoadLibraryA(path);
    if(!g_game_dll) {
        puts("Couldn't load DLL");
        return nullptr;
    }
    Game_fp ptr = (Game_fp) GetProcAddress(g_game_dll, "Chapter1");
    if(!ptr) puts("Couldn't load function");
    return ptr;
}

#else
void InitLoadGameLib() {}
Game_fp LoadGameLib(const char *) { return nullptr; }
#endif
