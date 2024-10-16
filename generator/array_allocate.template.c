$NAME$ $NAME$_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT($NAME$){.capacity = capacity, .length = 0, .internalArray = ($TYPE$ *)Clay__Array_Allocate_Arena(capacity, sizeof($TYPE$), CLAY__ALIGNMENT($TYPE$), arena)};
}