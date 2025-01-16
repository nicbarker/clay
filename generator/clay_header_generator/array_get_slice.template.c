$TYPE$ *$NAME$Slice_Get($NAME$Slice *slice, int32_t index) {
    return Clay__Array_RangeCheck(index, slice->length) ? &slice->internalArray[index] : $DEFAULT_VALUE$;
}