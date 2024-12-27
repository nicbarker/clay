void $NAME$_Set($NAME$ *array, int index, $TYPE$ value) {
	if (Clay__Array_RangeCheck(index, array->capacity)) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	}
}