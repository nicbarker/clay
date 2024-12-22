
Vector2 :: Math.Vector2;
Color :: Math.Vector4;

#scope_module

Math :: #import "Math";

#if OS == .WINDOWS {
    #load "windows.jai";
} else {
    assert(false);
}