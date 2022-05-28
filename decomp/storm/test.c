#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "storm.h"

#define MPQ_FILE "diabdat.mpq"
#define FILE_TO_EXTRACT "music\\dtowne.wav"
#define OUTPUT_FILE "dtowne.wav"

struct span {
  BYTE *buffer;
  size_t size;
};

// Use an open MPQ to load a particular file into memory.
// Returned buffer must be `free`'d by caller.
// Precondition: an MPQ is open
struct span LoadFileFromMpq(const char *filename) {
  HANDLE file = NULL;
  struct span span = {NULL, 0};

  if (!SFileOpenFile(filename, &file)) {
    fprintf(stderr, "Failed to open %s\n", filename);
    goto error;
  }

  span.size = SFileGetFileSize(file, NULL);
  span.buffer = (char *)malloc(span.size);

  if (!SFileReadFile(file, span.buffer, span.size, 0, 0)) {
    fprintf(stderr, "Failed to read %s\n", filename);
    goto error;
  }

  goto cleanup;

error:
  if (span.buffer) {
    free(span.buffer);
    span.buffer = NULL;
    span.size = 0;
  }

cleanup:
  if (file) SFileCloseFile(file);

  return span;
}

// Put a byte buffer to disk.
// Precondition: span is a valid pointer
BOOL DumpToFile(struct span const *const span, const char *filename) {
  FILE *file = NULL;
  BOOL ret = TRUE;

  file = fopen(filename, "wb");
  if (!file) {
    fprintf(stderr, "Failed to open for write %s\n", filename);
    goto error;
  }

  fwrite(span->buffer, 1, span->size, file);

  goto cleanup;

error:
  ret = FALSE;

cleanup:
  if (file) fclose(file);

  return ret;
}

int main(int argc, char **argv) {
  int err = 0;
  HANDLE mpq = NULL;
  struct span span = {NULL, 0};

  if (!SFileOpenArchive(MPQ_FILE, 0, 0, &mpq)) {
    fprintf(stderr, "Failed to open MPQ " MPQ_FILE "\n");
    goto error;
  }

  span = LoadFileFromMpq(FILE_TO_EXTRACT);
  if (!span.buffer) {
    goto error;
  }

  if (!DumpToFile(&span, OUTPUT_FILE)) {
    goto error;
  }

  goto cleanup;

error:
  err = 1;

cleanup:
  if (span.buffer) free(span.buffer);
  if (mpq) SFileCloseArchive(mpq);

  return err;
}
