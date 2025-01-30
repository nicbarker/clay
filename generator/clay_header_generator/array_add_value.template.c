void $NAME$_Add($NAME$ *array, $TYPE$ item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
	}
}