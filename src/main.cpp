/*
 * This sample provides a very basic demonstration of 3D rendering on the Xbox,
 * using pbkit. Based on the pbkit demo sources.
 */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
#include <hal/debug.h>
#pragma clang diagnostic pop
#include <hal/fileio.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <nxdk/mount.h>
#include <pbkit/pbkit.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmacro-redefined"
#include <windows.h>
#pragma clang diagnostic pop

#include <string>
#include <vector>
#include <tests/set_specular_enable.h>

#include "debug_output.h"
#include "pgraph_diff_token.h"
#include "tests/set_light_control.h"
#include "tests/set_light_enable.h"

#define MAX_FILE_PATH_SIZE 248
#define MAX_FILENAME_SIZE 42

static constexpr int kDelayOnFailureMilliseconds = 4000;

static constexpr int kFramebufferWidth = 640;
static constexpr int kFramebufferHeight = 480;

static constexpr const char *kOutputDirectoryPath =
    "e:\\DEVKIT\\nv2a_register_finder";
static constexpr const char *kLogFileName = "log.txt";

static void RunTests();
static bool EnsureDriveMounted(char drive_letter);
static void EnsureFolderExists(const std::string &folder_path);

extern "C" __cdecl int automount_d_drive(void);

/* Main program function */
int main() {
  automount_d_drive();
  debugPrint("Set video mode");
  XVideoSetMode(kFramebufferWidth, kFramebufferHeight, 32, REFRESH_DEFAULT);

  int status = pb_init();
  if (status) {
    debugPrint("pb_init Error %d\n", status);
    pb_show_debug_screen();
    Sleep(kDelayOnFailureMilliseconds);
    return 1;
  }

  if (!EnsureDriveMounted(kOutputDirectoryPath[0])) {
    debugPrint("Failed to mount %s, please make sure output directory is on a writable drive.\n",
               kOutputDirectoryPath);
    pb_show_debug_screen();
    Sleep(kDelayOnFailureMilliseconds);
    pb_kill();
    return 1;
  };

  EnsureFolderExists(kOutputDirectoryPath);

  debugClearScreen();

  pb_show_front_screen();
  RunTests();

  debugPrint("Results written to %s\n\nRebooting in 4 seconds...\n",
             kOutputDirectoryPath);
  pb_show_debug_screen();
  PrintMsg("Tests completed, sleeping and rebooting\n");
  Sleep(4000);

  XReboot();

  return 0;
}

static bool EnsureDriveMounted(char drive_letter) {
  if (nxIsDriveMounted(drive_letter)) {
    return true;
  }

  char dos_path[4] = "x:\\";
  dos_path[0] = drive_letter;
  char device_path[256] = {0};
  if (XConvertDOSFilenameToXBOX(dos_path, device_path) != STATUS_SUCCESS) {
    return false;
  }

  if (!strstr(device_path, R"(\Device\Harddisk0\Partition)")) {
    return false;
  }
  device_path[28] = 0;

  return nxMountDrive(drive_letter, device_path);
}

#define TEST(name) \
  do { \
    auto test = name(fp); \
    test.Initialize(); \
    test.Run(); \
    test.Cleanup(); \
  } while (0)

static void RunTests() {
  std::string log_file =
      std::string(kOutputDirectoryPath) + "\\" + kLogFileName;

  FILE *fp = fopen(log_file.c_str(), "w");
  if (!fp) {
    debugPrint("Failed to open logfile at %s\n", log_file.c_str());
    pb_show_debug_screen();
    Sleep(4000);
    return;
  }

  TEST(SetLightControl);
  TEST(SetLightEnable);
  TEST(SetSpecularEnable);

  if (fp) {
    fclose(fp);
  }
}

static void EnsureFolderExists(const std::string &folder_path) {
  if (folder_path.length() > MAX_FILE_PATH_SIZE) {
    ASSERT(!"Folder Path is too long.");
  }

  char buffer[MAX_FILE_PATH_SIZE + 1] = {0};
  const char *path_start = folder_path.c_str();
  const char *slash = strchr(path_start, '\\');
  slash = strchr(slash + 1, '\\');

  while (slash) {
    strncpy(buffer, path_start, slash - path_start);
    if (!CreateDirectory(buffer, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS) {
      ASSERT(!"Failed to create output directory.");
    }

    slash = strchr(slash + 1, '\\');
  }

  // Handle case where there was no trailing slash.
  if (!CreateDirectory(path_start, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS) {
    ASSERT(!"Failed to create output directory.");
  }
}
