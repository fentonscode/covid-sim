/** \file  Files.cpp
 *  \brief Provide file routines that terminate on failure
 */

#include "Files.h"

size_t Files::fwrite_big(void* buffer, size_t size, size_t count, FILE* stream)
{
	const unsigned long long mx = 0x80000000;
	unsigned long long j, n, st;
	size_t ret = 0;
	char* buf2;

	st = mx / ((unsigned long long) size);
	n = count / st;
	for (j = 0; j < n; j++)
	{
		buf2 = ((char*)buffer) + j * st * size;
		ret += (fwrite(buf2, size, (size_t)st, stream));
	}
	buf2 = ((char*)buffer) + n * st * size;
	ret += (fwrite(buf2, size, (size_t)(count - n * st), stream));
	return ret;
}

size_t Files::fread_big(void* buffer, size_t size, size_t count, FILE* stream)
{
	const unsigned long long mx = 0x80000000;
	unsigned long long j, n, st;
	size_t ret = 0;
	char* buf2;

	st = mx / ((unsigned long long) size);
	n = count / st;
	for (j = 0; j < n; j++)
	{
		buf2 = ((char*)buffer) + j * st * size;
		ret += (fread(buf2, size, (size_t)st, stream));
	}
	buf2 = ((char*)buffer) + n * st * size;
	ret += (fread(buf2, size, (size_t)(count - n * st), stream));
	return ret;
}

void Files::xfclose(FILE* stream) noexcept
{
  int rc = 1;
  if (stream != NULL)
  {
    rc = fclose(stream);
  }
  if (rc != 0)
  {
    ERR_CRITICAL_FMT("Error %d closing filestream - %s\n", errno, strerror(errno));
  }
}

FILE* Files::xfopen(const char* filename, const char* mode) noexcept
{
  FILE* fp = fopen(filename, mode);
  if (fp == NULL)
  {
    ERR_CRITICAL_FMT("Error %d opening file %s - %s\n", errno, filename, strerror(errno));
  }
  return fp;
}

FILE* Files::xfopen_fallback(const char* filename, const char* mode, FILE* fallback) noexcept
{
  FILE* fp = fopen(filename, mode);
  if (fp == NULL)
  {
    fp = fallback;
  }
  return fp;
}

FILE* Files::xfopen_if_exists(const char* filename, const char* mode) noexcept
{
  return fopen(filename, mode);
}

void Files::xfprintf(FILE* stream, const char* format, ...) noexcept
{
  int rc = -1;
  if (stream != NULL) {
    va_list args;
    va_start(args, format);
    rc = vfprintf(stream, format, args);
    va_end(args);
  }
  if (rc < 0) {
    ERR_CRITICAL_FMT("Error %d doing fprintf %s - %s\n", errno, format, strerror(errno));
  }
}

void Files::xfprintf_stderr(const char* format, ...) noexcept
{
  va_list args;
  va_start(args, format);
  int rc = vfprintf(stderr, format, args);
  va_end(args);
  if (rc < 0) {
    ERR_CRITICAL_FMT("Error %d doing fprintf %s - %s\n", errno, format, strerror(errno));
  }
}

void Files::xfscanf(FILE* stream, int n_expected, const char* format, ...) noexcept
{
  int rc = -1;
  if (stream != NULL) {
    va_list args;
    va_start(args, format);
    rc = vfscanf(stream, format, args);
    va_end(args);
  }
  if (rc != n_expected) {
    ERR_CRITICAL_FMT("Error, fsscanf looking for %s, expected %d matches, got %d\n", format, n_expected, rc);
  }
}

void Files::xrename(const char* oldname, const char* newname) noexcept
{
  int rc = rename(oldname, newname);
  if (rc != 0)
  {
    ERR_CRITICAL_FMT("Error %d renaming file %s to %s - %s\n", errno, oldname, newname, strerror(errno));
  }
}

void Files::xremove(const char* filename) noexcept
{
  int rc = remove(filename);
  if (rc != 0)
  {
    ERR_CRITICAL_FMT("Error %d removing file %s - %s\n", errno, filename, strerror(errno));
  }
}

void Files::xsprintf(char* str, const char* format, ...) noexcept
{
  va_list args;
  va_start(args, format);
  int rc = vsprintf(str, format, args);
  va_end(args);
  if (rc < 0) {
    ERR_CRITICAL_FMT("Error %d doing sprintf %s - %s\n", errno, format, strerror(errno));
  }
}

void Files::xsscanf(const char* s, int n_expected, const char* format, ...) noexcept
{
  va_list args;
  va_start(args, format);
  int rc = vsscanf(s, format, args);
  va_end(args);
  if (rc != n_expected) {
    ERR_CRITICAL_FMT("Error, xsscanf looking for %s in %s, expected %d matches, got %d\n", format, s, n_expected, rc);
  }
}
