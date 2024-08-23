void $NAME$_Set($NAME$ *array, int index, $TYPE$ value) {
	if (index < array->capacity && index >= 0) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	} else {
        Clay_StringArray_Add(&Clay_warnings, CLAY_STRING("Attempting to allocate array in arena, but arena is already at capacity and would overflow."));
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
	}
}