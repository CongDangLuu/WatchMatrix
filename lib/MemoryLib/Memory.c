#include "Memory.h"

const CHAR8  mHex[]   = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
/*
VOID
DumpHex (
  UINTN        Indent,
  UINTN        Offset,
  UINTN        DataSize,
  VOID         *UserData
  )
{
    UINT8 *Data;

  CHAR8 Val[50];

  CHAR8 Str[20];

  UINT8 TempByte;
  UINTN Size;
  UINTN Index;

  if (UserData == NULL){
    return;
  }

  Data = UserData;
  while (DataSize != 0) {
    Size = 16;
    if (Size > DataSize) {
      Size = DataSize;
    }

    for (Index = 0; Index < Size; Index += 1) {
      TempByte            = Data[Index];
      Val[Index * 3 + 0]  = mHex[TempByte >> 4];
      Val[Index * 3 + 1]  = mHex[TempByte & 0xF];
      Val[Index * 3 + 2]  = (CHAR8) ((Index == 7) ? '-' : ' ');
      Str[Index]          = (CHAR8) ((TempByte < ' ' || TempByte > 'z') ? '.' : TempByte);
    }

    Val[Index * 3]  = 0;
    Str[Index]      = 0;
    printf("%*a%08X: %-48a *%a*\n", Indent, "", Offset, Val, Str);

    Data += Size;
    Offset += Size;
    DataSize -= Size;
  }
}
*/

VOID
CopyMem (
  VOID   *Destination,
  VOID   *Source,
  UINTN  Length
  )
{
  CHAR8 *Destination8;
  CHAR8 *Source8;

  Destination8  = Destination;
  Source8       = Source;
  while (Length--) {
    *(Destination8++) = *(Source8++);
  }
}


VOID ZeroMem (
  VOID   *Buffer,
  UINTN  Size
  )
{
  INT8  *Ptr;

  Ptr = Buffer;
  while (Size--) {
    *(Ptr++) = 0;
  }
}

VOID *
AllocatePool (
   UINTN   AllocationSize
  )
{
  VOID * Memory;

  Memory = malloc(AllocationSize);
  // ASSERT(Memory != NULL);
  return Memory;
}