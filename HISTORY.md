# Update 2.0.0

### Breaking changes
- All `BTR_StringView_*` functions now accept `btr_string_view_s` and return a new view instead of modifying.
- `btr_string_view_s` refactor and optimization:
  - Field `start` removed.
  - Field `capacity` removed.
### Additions
- Added missing `#define`s for `#ifdef BTR_NO_PREFIX`.
### Fixes
- Fix truncation for `BTR_StringView_charAt`.

# Update 1.2.0

### Additions
- Added `BTR_VERSION` macro.
- Added `btrstd/string.h` module.
- Added `btrstd/utf8.h` module.
- Added `BTR_BAList_reserve` method in `btrstd/containers/borrowing_array_list.h`.
- Added `BTR_BAList_reserveNew` method in `btrstd/containers/borrowing_array_list.h`.
- Added `BTR_BAList_cropCapacity` method in `btrstd/containers/borrowing_array_list.h`.
- Added `BTR_OAList_reserve` method in `btrstd/containers/owning_array_list.h`.
- Added `BTR_OAList_reserveNew` method in `btrstd/containers/owning_array_list.h`.
- Added `BTR_OAList_cropCapacity` method in `btrstd/containers/owning_array_list.h`.
- Added `BTR_markedForRemoval` marco in `btrstd/error.h` module.
### Fixes
- `BTR_panicIf` now also aborts the program.
- Buffer overrun was fixed in `BTR_StringView_startsWith` and `BTR_StringView_endsWith`.
- Undefined behaviour was fixed in `BTR_Pool_deallocate`.

# Update 1.1.0

### Additions
- Added `BTR_BAList_toArray` method.
- Added `BTR_BLList_toArray` method.
- Added `BTR_OAList_toArray` method.
- Added `BTR_OLList_toArray` method.
### Fixes
- Added explicit null pointer exception handling for OLList
- Fixed many `BTR_NO_PREFIX` mistakes.
