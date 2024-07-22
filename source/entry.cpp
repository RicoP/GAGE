#ifdef GAGE_BUILD_DLL
#define GAGE_ENTRY_API __declspec(dllimport)
#else
#define GAGE_ENTRY_API
#endif

GAGE_ENTRY_API int gage_main();

int main() {
    return gage_main();
}