#include "strings.hpp"

// ------------------ //
// Internal Functions //
// ------------------ //

const char *Strings::internalToString(u64 value, char *buffer) {
  u64 temp = value;
  char *ptr = buffer;

  // Find end (at least one digit)
  do {
    ++ptr;
    temp /= 10;
  } while (temp);

  *ptr = '\0';

  // Write digits backwards
  do {
    *(--ptr) = char('0' + (value % 10));
    value /= 10;
  } while (value);

  return buffer;
}

const char *Strings::internalToString(i64 value, char *buffer) {
  u64 absValue;
  u8 offset = 0;

  if (value < 0) {
    buffer[0] = '-';
    offset = 1;
    absValue = (u64)(-(value + 1)) + 1;
  } else {
    absValue = (u64)value;
  }

  u64 temp = absValue;
  char *ptr = buffer + offset;

  // Find end (at least one digit)
  do {
    ++ptr;
    temp /= 10;
  } while (temp);

  *ptr = '\0';

  // Write digits backwards
  do {
    *(--ptr) = char('0' + (absValue % 10));
    absValue /= 10;
  } while (absValue);

  return buffer;
}

// ------------------ //
// External Functions //
// ------------------ //

const char *Strings::toString(u64 value, char (&buffer)[b64BufferSize]) {
  return internalToString(value, buffer);
}

const char *Strings::toString(u32 value, char (&buffer)[b32BufferSize]) {
  return internalToString((u64)value, buffer);
}

const char *Strings::toString(u16 value, char (&buffer)[b16BufferSize]) {
  return internalToString((u64)value, buffer);
}

const char *Strings::toString(u8 value, char (&buffer)[b8BufferSize]) {
  return internalToString((u64)value, buffer);
}

const char *Strings::toString(i64 value, char (&buffer)[b64BufferSize]) {
  return internalToString(value, buffer);
}

const char *Strings::toString(i32 value, char (&buffer)[b32BufferSize]) {
  return internalToString((i64)value, buffer);
}

const char *Strings::toString(i16 value, char (&buffer)[b16BufferSize]) {
  return internalToString((i64)value, buffer);
}

const char *Strings::toString(i8 value, char (&buffer)[b8BufferSize]) {
  return internalToString((i64)value, buffer);
}
