typedef struct
{
	uint32_t capacity;
	uint32_t length;
	$TYPE$ *internalArray;
} $NAME$;

$NAME$ $NAME$_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return ($NAME$){.capacity = capacity, .length = 0, .internalArray = ($TYPE$ *)Clay__Array_Allocate_Arena(capacity, sizeof($TYPE$), CLAY__ALIGNMENT($TYPE$), arena)};
}