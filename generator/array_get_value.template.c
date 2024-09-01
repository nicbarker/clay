$TYPE$ $NAME$_Get($NAME$ *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? array->internalArray[index] : $DEFAULT_VALUE$;
}