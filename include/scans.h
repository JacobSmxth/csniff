#ifndef SCANS_H
#define SCANS_H

#include "structs.h"

void scan_target(ScanArgs *arguments);
void *threaded_scan(void *args);

void scan_target(ScanArgs *arguments);
#endif
