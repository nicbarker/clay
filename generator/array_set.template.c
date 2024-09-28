void $NAME$_Set($NAME$ *array, int index, $TYPE$ value) {
	if (index < array->capacity && index >= 0) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	} else {
	    if (Clay__warningsEnabled) {
            Clay__WarningArray_Add(&Clay_warnings, CLAY__INIT(Clay__Warning) { CLAY_STRING("Attempting to allocate array in arena, but arena is already at capacity and would overflow.") });
	    }
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
	}
}