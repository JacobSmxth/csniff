#include "parsing.h"
#include "scans.h"

int main(int argc, char *argv[]) {
  ScanArgs *arg = parse_arguments(argc, argv);
  scan_target(
      arg); // This function initiates the scan as well as frees the ScanArgs.

  return 0;
}
