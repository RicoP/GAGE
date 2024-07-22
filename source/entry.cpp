#ifdef GAGE_BUILD_DLL
#define GAGE_API __declspec(dllimport)
#else
#define GAGE_API
#endif

GAGE_API int gage_main();

int main() {
    return gage_main();
}