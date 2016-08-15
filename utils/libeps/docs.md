# libeps documentation page

This page intends to document the use and behavior of libeps, describing this way how the user can use the library and
interact with it.

* [Overview](#overview)
* [Function reference](#function-reference)
* [Error codes](#error-codes)
* [Pokemon values](#pokemon-values)
* [Notes about the bindings](#notes-about-the-bindings)

***

## Overview

This library is designed to interact with PBR savefiles; in particular, the library operates on the `PbrSaveData` files
that PBR generates. It can read and write those files, as well as manipulate the data they contain; in particular, it
has features to edit the Pokémon that are stored in the storage system in each of the save slots within a savefile.
While the library is capable of reading Pokémon data both from the party and from the boxes, it can only write to the
boxes; writing to the party is not implemented, and will return an `EPSS_NOT_IMPLEMENTED` error if attempted.

Each save actually contains two savefiles within it: an active one and a backup one. The library will always use one of
them for reading, and can use either or both when writing; this is specified via the operation mode. The default mode is
to read and write from the active savefile only. Likewise, the savefiles contain four save slots; many operations only
operate on the currently selected slot, so a slot must be selected before performing them (the default state has no slot
selected); otherwise, an `EPSS_NO_SLOT_SELECTED` error will be returned.

All functions return a non-negative result on success (in most cases simply 0, also defined as `EPSS_OK` for convenience)
or a negative error code on failure. The list of error codes will be defined further in the document; each function also
lists the error codes that it can return.

The library defines two kinds of objects: saves and Pokémon. (Since the library is written in C, these objects are logical;
they are represented by pointers. Nevertheless, the functions behave as if they were constructors, destructors and methods
for those objects; even though they aren't "real" objects, they are treated as such.) All functions either create one of
these objects (behaving in a similar way to constructors) or operate on them (behaving like methods). However, note that,
since there is no garbage collection, all objects must be destroyed explicitly by calling the corresponding function. This
is still true even if the bindings for the library is used, as the objects in question are not real objects.

***

## Function reference

Every function defined by the library is described here, along with its prototype, parameter description, return value and
behavior. The errors that functions can return are also listed here.

* [epsf_read_save_from_file](#epsf_read_save_from_file)
* [epsf_destroy_save](#epsf_destroy_save)
* [epsf_write_save_to_file](#epsf_write_save_to_file)
* [epsf_current_operation_mode](#epsf_current_operation_mode)
* [epsf_select_operation_mode](#epsf_select_operation_mode)
* [epsf_current_save_slot](#epsf_current_save_slot)
* [epsf_select_save_slot](#epsf_select_save_slot)
* [epsf_copy_save_slot](#epsf_copy_save_slot)
* [epsf_erase_save_slot](#epsf_erase_save_slot)
* [epsf_is_save_slot_empty](#epsf_is_save_slot_empty)
* [epsf_get_active_savefile](#epsf_get_active_savefile)
* [epsf_swap_savefiles](#epsf_swap_savefiles)
* [epsf_get_encryption_key](#epsf_get_encryption_key)
* [epsf_set_encryption_key](#epsf_set_encryption_key)
* [epsf_read_savefile_raw](#epsf_read_savefile_raw)
* [epsf_write_savefile_raw](#epsf_write_savefile_raw)
* [epsf_read_saveslot_raw](#epsf_read_saveslot_raw)
* [epsf_write_saveslot_raw](#epsf_write_saveslot_raw)
* [epsf_new_pokemon](#epsf_new_pokemon)
* [epsf_destroy_pokemon](#epsf_destroy_pokemon)
* [epsf_read_pokemon_from_file](#epsf_read_pokemon_from_file)
* [epsf_write_pokemon_to_file](#epsf_write_pokemon_to_file)
* [epsf_read_pokemon_from_save](#epsf_read_pokemon_from_save)
* [epsf_write_pokemon_to_save](#epsf_write_pokemon_to_save)
* [epsf_erase_pokemon_from_save](#epsf_erase_pokemon_from_save)
* [epsf_get_pokemon_name](#epsf_get_pokemon_name)
* [epsf_set_pokemon_name](#epsf_set_pokemon_name)
* [epsf_get_pokemon_value](#epsf_get_pokemon_value)
* [epsf_set_pokemon_value](#epsf_set_pokemon_value)
* [epsf_fix_pokemon_checksum](#epsf_fix_pokemon_checksum)

***

### epsf_read_save_from_file

Prototype:

```C
int epsf_read_save_from_file(const char * filename, void ** save);
```

Reads a save from a file, decrypts it and loads it into memory; returns (by a passed-by-reference argument) the new save
object created from the file.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `filename`: file from which the save will be read.
* `save`: passed-by-reference `void *` variable where the new save object will be stored.

Possible errors:
* `EPSS_NULL_POINTER`: one of the parameters is `NULL`.
* `EPSS_OUT_OF_MEMORY`: there isn't enough memory available to create an object.
* `EPSS_FILE_NOT_FOUND`: `filename` doesn't exist, or the library can't read it.
* `EPSS_READ_ERROR`: an error occurred while attempting to read the file.
* `EPSS_WRONG_FILE_SIZE`: the file isn't 3,670,016 bytes long.
* `EPSS_INVALID_HEADER_CHECKSUM`: the checksum for a savefile's header was incorrect.
* `EPSS_INVALID_CHECKSUM`: the checksum for a savefile was incorrect.

***

### epsf_write_save_to_file

Prototype:

```C
int epsf_write_save_to_file(void * save, const char * filename);
```

Writes a save object to a file after encrypting it, in the format expected by PBR. If the file already exists, it will
be overwritten by this function.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: the save object that will be written.
* `filename`: the location where the file will be written.

Possible errors:
* `EPSS_NULL_POINTER`: one of the parameters is `NULL`.
* `EPSS_OUT_OF_MEMORY`: there isn't enough memory available to complete the operation.
* `EPSS_ERROR_OPENING_FILE`: the library couldn't open `filename` for writing.
* `EPSS_WRITE_ERROR`: an error occurred while attempting to write the file.

***

### epsf_destroy_save

Prototype:

```C
int epsf_destroy_save(void * save);
```

Destroys a save object. This function must be called once (and exactly once) per save object, once the application is
done operating with it. The save object must not be used again after this function is called.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to be destroyed.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.

***

### epsf_current_operation_mode

Prototype:

```C
int epsf_current_operation_mode(void * save);
```

Returns the current operation mode (indicating which savefiles will be read and written) for a save object, or an error
code if it fails. Note that all error codes are negative, while operation modes are non-negative; this allows the caller
to tell them apart.

The operation modes are as follows:
* `EPSM_ACTIVE_SAVEFILE`: only read and write to the active savefile. This is the default.
* `EPSM_BACKUP_SAVEFILE`: only read and write to the backup savefile.
* `EPSM_WRITE_BOTH_READ_ACTIVE`: read from the active savefile, but write to both simultaneously.
* `EPSM_WRITE_BOTH_READ_BACKUP`: read from the backup savefile, but write to both simultaneously.

Parameters:
* `save`: save object to operate on.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.

***

### epsf_select_operation_mode

Prototype:

```C
int epsf_select_operation_mode(void * save, int mode);
```

Selects an operation mode in a save object.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `mode`: new operation mode. The valid values are the ones listed in the previous function.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `mode` is not one of the valid values listed above.

***

### epsf_current_save_slot

Prototype:

```C
int epsf_current_save_slot(void * save);
```

Returns the currently selected save slot for a save object (numbered 1 to 4), or 0 if no slot is selected. If an error
occurs, it will return a (negative) error code.

Parameters:
* `save`: save object to operate on.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.

***

### epsf_select_save_slot

Prototype:

```C
int epsf_select_save_slot(void * save, int slot);
```

Selects or deselects a save slot in a save object.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `slot`: slot to be selected (between 1 and 4), or 0 to deselect all slots.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.
* `EPSS_INDEX_OUT_OF_RANGE`: `slot` is not a value between 0 and 4.
* `EPSS_SLOT_IS_EMPTY`: the selected save slot is empty.

***

### epsf_copy_save_slot

Prototype:

```C
int epsf_copy_save_slot(void * save, int from, int to);
```

Copies a save slot onto another. If the destination save slot is not empty, it is overwritten. Attempting to copy a
save slot onto itself results in a harmless no-op and returns success.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `from`: slot to be copied (between 1 and 4).
* `to`: slot where the copy is to be placed (between 1 and 4).

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.
* `EPSS_INDEX_OUT_OF_RANGE`: either `from` or `to` is not a value between 1 and 4.
* `EPSS_OUT_OF_MEMORY`: there is not enough memory available to complete the operation.

***

### epsf_erase_save_slot

Prototype:

```C
int epsf_erase_save_slot(void * save, int slot);
```

Erases a save slot in a save object. If the currently selected slot is erased, it will be deselected.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `slot`: slot to be erased (between 1 and 4).

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.
* `EPSS_INDEX_OUT_OF_RANGE`: `slot` is not a value between 1 and 4.
* `EPSS_OUT_OF_MEMORY`: there is not enough memory available to complete the operation.

***

### epsf_is_save_slot_empty

Prototype:

```C
int epsf_is_save_slot_empty(void * save, int slot);
```

Returns 1 if the specified save slot is empty, or 0 if it isn't; returns a (negative) error code on failure.

Parameters:
* `save`: save object to operate on.
* `slot`: slot to be checked (between 1 and 4).

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.
* `EPSS_INDEX_OUT_OF_RANGE`: `slot` is not a value between 1 and 4.

***

### epsf_get_active_savefile

Prototype:

```C
int epsf_get_active_savefile(void * save);
```

Returns the position of the active savefile within the save object, which will be 0 or 1. (0 is the first savefile in
the save, 1 is the second one.) Returns a (negative) error code on failure.

Parameters:
* `save`: save object to operate on.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.

***

### epsf_swap_savefiles

Prototype:

```C
int epsf_swap_savefiles(void * save);
```

Swaps the position of the active and backup savefiles in the save object. Note that this doesn't change which file is
the active one; it only swaps the physical positions of the savefiles (which will only be reflected when the save is
written to a file).

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.

***

### epsf_get_encryption_key

Prototype:

```C
int epsf_get_encryption_key(void * save, unsigned long long * key);
```

Gets the current encryption key (into a passed-by-reference argument) for a save object.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `key`: passed-by-reference `unsigned long long` variable where the key will be stored.

Possible errors:
* `EPSS_NULL_POINTER`: one of the parameters is `NULL`.

***

### epsf_set_encryption_key

Prototype:

```C
int epsf_set_encryption_key(void * save, unsigned long long key);
```

Sets the encryption key for a save object.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `key`: new encryption key.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.

***

### epsf_read_savefile_raw

Prototype:

```C
int epsf_read_savefile_raw(void * save, unsigned offset, int width, unsigned * value);
```

Reads a value directly from a save object, identifying that value by its address and size. Multi-byte values are always
big-endian. Returns the read value into a passed-by-reference argument.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `offset`: address where the value to be read is located.
* `width`: size of the value in bytes. Must be between 1 and 4.
* `value`: passed-by-reference `unsigned` variable where the value will be stored.

Possible errors:
* `EPSS_NULL_POINTER`: `save` or `value` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `width` is not a value between 1 and 4.
* `EPSS_INDEX_OUT_OF_RANGE`: `offset` is negative, or it points to a value past the end of the save.

***

### epsf_write_savefile_raw

Prototype:

```C
int epsf_write_savefile_raw(void * save, unsigned offset, int width, unsigned value);
```

Writes a value directly to a save object, identifying that value by its address and size. Multi-byte values are always
big-endian.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `offset`: address where the value to be written is located.
* `width`: size of the value in bytes. Must be between 1 and 4.
* `value`: value to be written.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `width` is not a value between 1 and 4.
* `EPSS_INDEX_OUT_OF_RANGE`: `offset` is negative, or it points to a value past the end of the save.

***

### epsf_read_saveslot_raw

Prototype:

```C
int epsf_read_saveslot_raw(void * save, unsigned offset, int width, unsigned * value);
```

Reads a value directly from the currently selected save slot of a save object, identifying that value by its address
and size. Multi-byte values are always big-endian. Returns the read value into a passed-by-reference argument.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `offset`: address where the value to be read is located.
* `width`: size of the value in bytes. Must be between 1 and 4.
* `value`: passed-by-reference `unsigned` variable where the value will be stored.

Possible errors:
* `EPSS_NULL_POINTER`: either `save` or `value` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `width` is not a value between 1 and 4.
* `EPSS_INDEX_OUT_OF_RANGE`: `offset` is negative, or it points to a value past the end of the save slot.
* `EPSS_NO_SLOT_SELECTED`: the save object doesn't have any save slot selected.

***

### epsf_write_saveslot_raw

Prototype:

```C
int epsf_write_saveslot_raw(void * save, unsigned offset, int width, unsigned value);
```

Writes a value directly to the currently selected save slot of a save object, identifying that value by its address
and size. Multi-byte values are always big-endian.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `offset`: address where the value to be written is located.
* `width`: size of the value in bytes. Must be between 1 and 4.
* `value`: value to be written.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `width` is not a value between 1 and 4.
* `EPSS_INDEX_OUT_OF_RANGE`: `offset` is negative, or it points to a value past the end of the save slot.
* `EPSS_NO_SLOT_SELECTED`: the save object doesn't have any save slot selected.

***

### epsf_new_pokemon

Prototype:

```C
int epsf_new_pokemon(void ** pokemon);
```

Creates a new, blank Pokémon object and stores it in a passed-by-reference argument.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `pokemon`: passed-by-reference `void *` variable where the new Pokémon object will be stored.

Possible errors:
* `EPSS_NULL_POINTER`: `pokemon` is `NULL`.
* `EPSS_OUT_OF_MEMORY`: there isn't enough memory available to create an object.

***

### epsf_destroy_pokemon

Prototype:

```C
int epsf_destroy_pokemon(void * pokemon);
```

Destroys a Pokémon object. This function must be called once (and exactly once) per Pokémon object, once the
application is done operating with it. The Pokémon object must not be used again after this function is called.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `pokemon`: Pokémon object to be destroyed.

Possible errors:
* `EPSS_NULL_POINTER`: `pokemon` is `NULL`.

***

### epsf_read_pokemon_from_file

Prototype:

```C
int epsf_read_pokemon_from_file(const char * filename, void ** pokemon);
```

Creates a new Pokémon object by reading it from a file, and stores it in a passed-by-reference argument.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `filename`: file from which the save will be read.
* `pokemon`: passed-by-reference `void *` variable where the new Pokémon object will be stored.

Possible errors:
* `EPSS_NULL_POINTER`: one of the parameters is `NULL`.
* `EPSS_OUT_OF_MEMORY`: there isn't enough memory available to create an object.
* `EPSS_FILE_NOT_FOUND`: `filename` doesn't exist, or the library can't read it.
* `EPSS_READ_ERROR`: an error occurred while attempting to read the file.
* `EPSS_WRONG_FILE_SIZE`: the file isn't 136 bytes long.

***

### epsf_write_pokemon_to_file

Prototype:

```C
int epsf_write_pokemon_to_file(void * pokemon, const char * filename);
```

Writes a Pokémon object to a file. If the file already exists, it will be overwritten by this function.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `pokemon`: the Pokémon object that will be written.
* `filename`: the location where the file will be written.

Possible errors:
* `EPSS_NULL_POINTER`: one of the parameters is `NULL`.
* `EPSS_ERROR_OPENING_FILE`: the library couldn't open `filename` for writing.
* `EPSS_WRITE_ERROR`: an error occurred while attempting to write the file.

***

### epsf_read_pokemon_from_save

Prototype:

```C
int epsf_read_pokemon_from_save(void * save, int box, int position, void ** pokemon);
```

Creates a Pokémon object by reading it from the storage or the party in a save object. It stores the created Pokémon
object in a passed-by-reference argument.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `box`: box to read the Pokémon from. 0 indicates the party, or a value from 1 to 18 chooses a storage box.
* `position`: position in the box or party to read the Pokémon from. (1 to 30 for boxes, 1 to 6 for party.)
* `pokemon`: passed-by-reference `void *` variable where the new Pokémon object will be stored.

Possible errors:
* `EPSS_NULL_POINTER`: either `save` or `pokemon` is `NULL`.
* `EPSS_INDEX_OUT_OF_RANGE`: `box` or `position` are not in the required range.
* `EPSS_NO_SLOT_SELECTED`: the save object doesn't have any save slot selected.
* `EPSS_OUT_OF_MEMORY`: there isn't enough memory available to create an object.

***

### epsf_write_pokemon_to_save

Prototype:

```C
int epsf_write_pokemon_to_save(void * save, int box, int position, void * pokemon);
```

Writes a Pokémon object to the storage of a save object.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `box`: box to write the Pokémon to; must be between 1 and 18.
* `position`: position in the box to write the Pokémon to; must be between 1 and 30.
* `pokemon`: Pokémon object to be written.

Possible errors:
* `EPSS_NULL_POINTER`: either `save` or `pokemon` is `NULL`.
* `EPSS_INDEX_OUT_OF_RANGE`: `box` or `position` are not in the required range.
* `EPSS_NOT_IMPLEMENTED`: an attempt was made to write to the party, which is not currently supported.
* `EPSS_NO_SLOT_SELECTED`: the save object doesn't have any save slot selected.

***

### epsf_erase_pokemon_from_save

Prototype:

```C
int epsf_erase_pokemon_from_save(void * save, int box, int position);
```

Erases a Pokémon from the storage of a save object; this emulates a release.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Parameters:
* `save`: save object to operate on.
* `box`: box to write the Pokémon to; must be between 1 and 18.
* `position`: position in the box to write the Pokémon to; must be between 1 and 30.

Possible errors:
* `EPSS_NULL_POINTER`: `save` is `NULL`.
* `EPSS_INDEX_OUT_OF_RANGE`: `box` or `position` are not in the required range.
* `EPSS_NOT_IMPLEMENTED`: an attempt was made to erase from the party, which is not currently supported.
* `EPSS_NO_SLOT_SELECTED`: the save object doesn't have any save slot selected.

***

### epsf_get_pokemon_name

Prototype:

```C
int epsf_get_pokemon_name(void * pokemon, int which, char * name);
```

Retrieves the name or the OT name for a Pokémon object.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Note that only ASCII characters will be retrieved; other characters will be substituted by `^`. The resulting string
uses ASCII encoding. The characters for "male" and "female" are respectively mapped to `<` and `>`.

Parameters:
* `pokemon`: Pokémon object to operate on.
* `which`: 0 to retrieve the Pokémon's name, or 1 to retrieve the OT name.
* `name`: string buffer where the name will be stored. Must be at least 12 characters long.

Possible errors:
* `EPSS_NULL_POINTER`: either `pokemon` or `name` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `which` is not 0 or 1.

***

### epsf_set_pokemon_name

Prototype:

```C
int epsf_set_pokemon_name(void * pokemon, int which, const char * name);
```

Sets the name or OT name for a Pokémon object.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Note that only ASCII characters are valid; other characters will cause an error. The supplied string must use ASCII
encoding. The `<` and `>` characters will be mapped to the "male" and "female" characters respectively. The following
characters are not allowed and will raise an error: ``[\]^`|``.

Parameters:
* `pokemon`: Pokémon object to operate on.
* `which`: 0 to set the Pokémon's name, or 1 to set the OT name.
* `name`: name to set. Must be at most 10 characters long for a Pokémon name, or 7 characters long for an OT name.

Possible errors:
* `EPSS_NULL_POINTER`: either `pokemon` or `name` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `which` is not 0 or 1.
* `EPSS_STRING_TOO_LONG`: `name` is too long.
* `EPSS_INVALID_CHARACTERS`: `name` contains forbidden or non-ASCII characters.

***

### epsf_get_pokemon_value

Prototype:

```C
int epsf_get_pokemon_value(void * pokemon, int kind, int index, unsigned * value);
```

Gets a certain numerical value from a Pokémon object's data, and stores it in a passed-by-reference argument.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

The possible values for `kind` and `index` and their meanings are listed in another section of the document.

Parameters:
* `pokemon`: Pokémon object to operate on.
* `kind`: kind of value to retrieve.
* `index`: for kinds that represent more than one value, index of the value; must be 0 for kinds that don't have indices.
* `value`: pointer to an `unsigned` variable where the retrieved value will be stored.

Possible errors:
* `EPSS_NULL_POINTER`: `pokemon` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `kind` is not one of the allowed values.
* `EPSS_INDEX_OUT_OF_RANGE`: `index` is out of range for the selected kind.

***

### epsf_set_pokemon_value

Prototype:

```C
int epsf_set_pokemon_value(void * pokemon, int kind, int index, unsigned value);
```

Set a certain numerical value in a Pokémon object's data.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

The possible values for `kind` and `index` and their meanings are listed in another section of the document.

Parameters:
* `pokemon`: Pokémon object to operate on.
* `kind`: kind of value to set.
* `index`: for kinds that represent more than one value, index of the value; must be 0 for kinds that don't have indices.
* `value`: value to set in the Pokémon's data.

Possible errors:
* `EPSS_NULL_POINTER`: `pokemon` is `NULL`.
* `EPSS_INVALID_ARGUMENT`: `kind` is not one of the allowed values.
* `EPSS_INDEX_OUT_OF_RANGE`: `index` is out of range for the selected kind.

***

### epsf_fix_pokemon_checksum

Prototype:

```C
int epsf_fix_pokemon_checksum(void * pokemon);
```

Fixes the checksum value in a Pokémon object.

Returns 0 (`EPSS_OK`) on success, or an error code if it fails.

Note that PBR ignores checksums, so there is no need to actually call this function.

Parameters:
* `pokemon`: Pokémon object to operate on.

Possible errors:
* `EPSS_NULL_POINTER`: `pokemon` is `NULL`.

***

## Error codes

All the error codes that the library can return are listed here for the user's convenience, despite they are already
listed on the description of every function that can return them.

* `EPSS_OK`: not really an error code, and it has the value 0 (unlike true error codes which are negative). Indicates
  that the function call succeeded.
* `EPSS_OUT_OF_MEMORY`: there isn't enough memory available to complete the requested operation. The call should be
  repeated once there is enough memory available.
* `EPSS_INVALID_ARGUMENT`: an argument supplied to a call is not valid.
* `EPSS_INDEX_OUT_OF_RANGE`: an index supplied to a call is outside its acceptable range of values.
* `EPSS_NOT_IMPLEMENTED`: the requested operation is not currently supported by the library.
* `EPSS_NULL_POINTER`: a pointer supplied to a call is `NULL`.
* `EPSS_STRING_TOO_LONG`: the supplied string is longer than the maximum acceptable length.
* `EPSS_INVALID_CHARACTERS`: the supplied string contains forbidden or non-ASCII characters.
* `EPSS_INVALID_CHECKSUM`: a savefile's checksum did not match the expected value.
* `EPSS_INVALID_HEADER_CHECKSUM`: a savefile's header checksum did not match the expected value.
* `EPSS_FILE_NOT_FOUND`: the requested file was not found or not readable to the library.
* `EPSS_WRONG_FILE_SIZE`: the file did not have the size required by the library.
* `EPSS_READ_ERROR`: an error occurred while reading the file.
* `EPSS_ERROR_OPENING_FILE`: an error occurred while opening the file.
* `EPSS_WRITE_ERROR`: an error occurred while writing to the file.
* `EPSS_SLOT_IS_EMPTY`: the call attempted to select an empty slot.
* `EPSS_NO_SLOT_SELECTED`: the save object has no slot currently selected.

***

## Pokemon values

This section lists the valid values for the `kind` and `index` parameters in `epsf_get_pokemon_value()` and
`epsf_set_pokemon_value()`, which represent the various numerical pieces of data that the library can access in a
Pokémon object.

The following table lists each possible value of `kind`, along with the values of `index` supported by that kind; it
also shows the bit width of the value. Each value has a specific bit width in the Pokémon data structure; attempting
to write a larger value will silently ignore the extra bits. (For instance, attempting to write 46 (binary 101110) to
an IV (which is 5-bit) will silently write 14 (binary 01110) instead.)

| Kind of value                   | Valid indices | Bit width | Notes |
| ------------------------------- | :-----------: | --------: | ----- |
|`EPSK_BYTE`                      | 0 - 135       |         8 | Allows raw access to the (unscrambled) Pokémon data structure. The index is the offset.|
|`EPSK_TWO_BYTES`                 | 0 - 134       |        16 | Same as above, but access 2-byte values. All multi-byte values are big-endian.|
|`EPSK_THREE_BYTES`               | 0 - 133       |        24 | Same as above, for 3-byte values.|
|`EPSK_FOUR_BYTES`                | 0 - 132       |        32 | Same as above, for 4-byte values.|
|`EPSK_PERSONALITY_VALUE`         | not used (0)  |        32 | The Pokémon's nature is determined by this value, modulo 25.|
|`EPSK_CHECKSUM`                  | not used (0)  |        16 | |
|`EPSK_SPECIES_NUMBER`            | not used (0)  |        16 | |
|`EPSK_HELD_ITEM`                 | not used (0)  |        16 | |
|`EPSK_OT_ID`                     | 0 - 1         |        16 | The index indicates which ID (visible (0) or secret (1)) is being accessed.|
|`EPSK_EXPERIENCE_POINTS`         | not used (0)  |        32 | The level is calculated based on this value; it caps at 100 if this value is too high.|
|`EPSK_FRIENDSHIP`                | not used (0)  |         8 | Also used as the egg step counter when the Pokémon is in an egg.|
|`EPSK_ABILITY`                   | not used (0)  |         8 | |
|`EPSK_BOX_MARKS`                 | not used (0)  |         8 | |
|`EPSK_COUNTRY_OF_ORIGIN`         | not used (0)  |         8 | |
|`EPSK_EFFORT_VALUE`              | 0 - 5         |         8 | The index indicates the stat. Note that the overall cap of 510 is enforced upon entering battle.|
|`EPSK_CONTEST_STAT`              | 0 - 5         |         8 | The index indicates the stat.|
|`EPSK_RIBBON`                    | 0 - 63        |         1 | Bitflag indicating the presence of a certain ribbon; the index indicates which one.|
|`EPSK_MOVE`                      | 1 - 4         |        16 | The index indicates which moveslot. Set to 0 to leave the moveslot empty.|
|`EPSK_MOVE_PP`                   | 1 - 4         |         8 | The index indicates which moveslot.|
|`EPSK_MOVE_PP_UPS`               | 1 - 4         |         8 | The index indicates which moveslot.|
|`EPSK_INDIVIDUAL_VALUE`          | 0 - 5         |         5 | The index indicates the stat.|
|`EPSK_IS_EGG`                    | not used (0)  |         1 | |
|`EPSK_IS_NICKNAMED`              | not used (0)  |         1 | Doesn't seem to have any effect; nicknames seem to work even without setting this flag.|
|`EPSK_FATEFUL_ENCOUNTER`         | not used (0)  |         1 | |
|`EPSK_GENDER`                    | not used (0)  |         2 | 0 = male, 1 = female, 2 = genderless. 3 is probably genderless as well.|
|`EPSK_FORM`                      | not used (0)  |         5 | For Pokémon that only have one form, this is set to 0.|
|`EPSK_EGG_LOCATION_PLATINUM`     | not used (0)  |        16 | |
|`EPSK_MET_LOCATION_PLATINUM`     | not used (0)  |        16 | |
|`EPSK_HOMETOWN`                  | not used (0)  |        16 | |
|`EPSK_CONTEST_DATA`              | not used (0)  |        32 | The layout and format of this value is unknown.|
|`EPSK_EGG_RECEIVED_DATE`         | not used (0)  |        24 | The layout and format of this value is unknown.|
|`EPSK_MET_DATE`                  | not used (0)  |        24 | The layout and format of this value is unknown.|
|`EPSK_EGG_LOCATION_DIAMOND_PEARL`| not used (0)  |        16 | |
|`EPSK_MET_LOCATION_DIAMOND_PEARL`| not used (0)  |        16 | |
|`EPSK_POKERUS`                   | not used (0)  |         8 | |
|`EPSK_POKE_BALL`                 | not used (0)  |         8 | |
|`EPSK_MET_LEVEL`                 | not used (0)  |         7 | |
|`EPSK_OT_GENDER`                 | not used (0)  |         1 | Probably 0 = male, 1 = female.|
|`EPSK_ENCOUNTER_TYPE`            | not used (0)  |         8 | |
|`EPSK_POKE_BALL_HG_SS`           | not used (0)  |         8 | |
|`EPSK_PERFORMANCE`               | not used (0)  |         8 | Pokéathlon performance data.|
|`EPSK_HEADER_UNKNOWN`            | not used (0)  |        16 | 2-byte unknown value in the Pokémon data header.|
|`EPSK_SMALL_UNKNOWN`             | not used (0)  |        24 | 3-byte unknown value in block B of the Pokémon data.|
|`EPSK_BIG_UNKNOWN`               | not used (0)  |        32 | 4-byte unknown value in block C of the Pokémon data.|

In order to make the code clearer and easier to write, some constants are defined to represent the different values
of `index` for a few of those kinds.

For the `EPSK_OT_ID` kind, two constants are defined, `EPSN_VISIBLE_ID` and `EPSN_SECRET_ID`. These two constants
represent the two valid values of `index` and the two different OT IDs that can be accessed.

Also, for the kinds of values that access stats, constants are defined for each possible stat. These constants are
the following ones:

| Index | Battle stat             | Contest stat       |
| :---: | :---------------------: | :----------------: |
|     0 | `EPSN_HP`               | `EPSN_COOL`        |
|     1 | `EPSN_ATTACK`           | `EPSN_BEAUTY`      |
|     2 | `EPSN_DEFENSE`          | `EPSN_CUTE`        |
|     3 | `EPSN_SPEED`            | `EPSN_SMART`       |
|     4 | `EPSN_SPECIAL_ATTACK`   | `EPSN_TOUGH`       |
|     5 | `EPSN_SPECIAL_DEFENSE`  | `EPSN_FEEL`        |

***

## Notes about the bindings

* [Generalities](#generalities)
* [Python binding](#python-binding)
* [C# binding](#c%23-binding)

***

### Generalities

The bindings attempt to expose the above functions and constants in other languages. They intentionally expose an
interface nearly identical to the C interface, that therefore follows the above documentation; no new functionality is
defined and/or abstracted by the bindings. Therefore, they should be wrapped by proper interfaces that follow the
conventions and standards of the languages they are written.

As the bindings behave identically to the C interface, explicit object destruction is mandatory. The garbage collection
methods of the bindings' languages will not work on these objects, and therefore they will be never collected; the
corresponding destructor functions must be called when the program is done working with those objects.

***

### Python binding

The exposed functions take the same arguments as the corresponding C functions. The `ctypes` types `c_void_p` and
`c_char_p` take the place of C's `void *` and `char *` types. Note that several functions take pass-by-reference
arguments through which they return values to the caller, which will therefore require `ctypes.byref()` to be called;
also note that `byref` itself requires the variable that is passed to it to be initialized to a proper `ctypes` value.

The `c_char_p` type will only accept bytes, not strings; therefore, strings must be converted to bytes using the
`encode` method (and bytes back to strings using `decode`). The encoding is the usual encoding for C strings for the
platform: that is typically `windows-1252` for Windows and `utf-8` for Linux. For ASCII-only strings (e.g., names in
Pokémon data), either encoding is acceptable, since ASCII is a subset of both.

The following sample program should illustrate the various quirks of using the Python binding:

```python
from ctypes import *
from eps import *

save = c_void_p(None)
epsf_read_save_from_file(b'basesave', byref(save))
epsf_select_save_slot(save, 1)
for n in range(0, 30):
    pokemon = c_void_p(None)
    epsf_read_pokemon_from_save(save, 1, n + 1, byref(pokemon))
    name = create_string_buffer(12)
    species = c_uint(0)
    epsf_get_pokemon_name(pokemon, 0, name)
    epsf_get_pokemon_value(pokemon, EPSK_SPECIES_NUMBER, 0, byref(species))
    # print its current species number and nickname
    print('#%03u %s' % (species.value, name.value.decode('utf-8')))
    # change its nickname to Test #xxx, and make it level 1
    epsf_set_pokemon_name(pokemon, 0, ('Test #%03u' % species.value).encode('utf-8'))
    epsf_set_pokemon_value(pokemon, EPSK_EXPERIENCE_POINTS, 0, 0)
    # ...and make it a Chatot
    epsf_set_pokemon_value(pokemon, EPSK_SPECIES_NUMBER, 0, 441) # 441 is Chatot's ID
    epsf_write_pokemon_to_save(save, 1, n + 1, pokemon)
    epsf_destroy_pokemon(pokemon)
epsf_write_save_to_file(save, b'PbrSaveData')
epsf_destroy_save(save)
```

***

### C# binding

The exposed functions take the same arguments as the corresponding C functions; `IntPtr` (`System.IntPtr`) replaces
`void *` and `string` replaces `const char *`. The functions that return values via pass-by-reference arguments in the
C interface do so via `out` parameters in C#. The function that takes a `char *` argument (which is mutable, as opposed
to the `const char *` arguments) will take a `StringBuilder` in C#; this object must have been instantiated to a new
`StringBuilder` of an appropriate size prior to the call.

All exposed functions and constants are exposed via a class called `eps`, which is declared as static. The class is
defined in the global namespace; as only one class is defined, no namespaces are created by the binding.

The following sample program should illustrate the various quirks of using the C# binding:

```c#
﻿using System;
using System.Text;

public class Program {
  static void Main (string[] args) {
    IntPtr save, pokemon;
    eps.epsf_read_save_from_file("basesave", out save);
    eps.epsf_select_save_slot(save, 1);
    int n;
    StringBuilder name;
    uint species;
    for (n = 1; n <= 30; n ++) {
      eps.epsf_read_pokemon_from_save(save, 1, n, out pokemon);
      name = new StringBuilder(12);
      eps.epsf_get_pokemon_name(pokemon, 0, name);
      eps.epsf_get_pokemon_value(pokemon, eps.EPSK_SPECIES_NUMBER, 0, out species);
      // print its current species number and nickname
      Console.WriteLine("#{0:000} {1}", species, name.ToString());
      // change its nickname to Test #xxx, and make it level 1
      eps.epsf_set_pokemon_name(pokemon, 0, string.Format("Test #{0:000}", species));
      eps.epsf_set_pokemon_value(pokemon, eps.EPSK_EXPERIENCE_POINTS, 0, 0);
      // ...and make it a Chatot
      eps.epsf_set_pokemon_value(pokemon, eps.EPSK_SPECIES_NUMBER, 0, 441); // 441 is Chatot's ID
      eps.epsf_write_pokemon_to_save(save, 1, n, pokemon);
      eps.epsf_destroy_pokemon(pokemon);
    }
    eps.epsf_write_save_to_file(save, "PbrSaveData");
    eps.epsf_destroy_save(save);
  }
}
```
