$TYPE$ $NAME$_RemoveSwapback($NAME$ *array, int32_t index) {
	if (Clay__Array_RangeCheck(index, array->length)) {
		array->length--;
		$TYPE$ removed = array->internalArray[index];
		array->internalArray[index] = array->internalArray[array->length];
		return removed;
	}
	return $DEFAULT_VALUE$;
}