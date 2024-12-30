
Vector2 :: Math.Vector2;

#scope_module

Math :: #import "Math";

#if OS == .WINDOWS {
    #load "windows.jai";
} else {
    assert(false);
}