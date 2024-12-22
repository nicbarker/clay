
LIGHTGRAY :: Color.{  200, 200, 200, 255  };
GRAY :: Color.{  130, 130, 130, 255  };
DARKGRAY :: Color.{  80, 80, 80, 255  };
YELLOW :: Color.{  253, 249, 0, 255  };
GOLD :: Color.{  255, 203, 0, 255  };
ORANGE :: Color.{  255, 161, 0, 255  };
PINK :: Color.{  255, 109, 194, 255  };
RED :: Color.{  230, 41, 55, 255  };
MAROON :: Color.{  190, 33, 55, 255  };
GREEN :: Color.{  0, 228, 48, 255  };
LIME :: Color.{  0, 158, 47, 255  };
DARKGREEN :: Color.{  0, 117, 44, 255  };
SKYBLUE :: Color.{  102, 191, 255, 255  };
BLUE :: Color.{  0, 121, 241, 255  };
DARKBLUE :: Color.{  0, 82, 172, 255  };
PURPLE :: Color.{  200, 122, 255, 255  };
VIOLET :: Color.{  135, 60, 190, 255  };
DARKPURPLE :: Color.{  112, 31, 126, 255  };
BEIGE :: Color.{  211, 176, 131, 255  };
BROWN :: Color.{  127, 106, 79, 255  };
DARKBROWN :: Color.{  76, 63, 47, 255  };
WHITE :: Color.{  255, 255, 255, 255  };
BLACK :: Color.{  0, 0, 0, 255  };
BLANK :: Color.{  0, 0, 0, 0  };
MAGENTA :: Color.{  255, 0, 255, 255  };
RAYWHITE :: Color.{  245, 245, 245, 255  };

IsMouseButtonPressed :: (button: MouseButton) -> bool { return IsMouseButtonPressed(cast(s32) button); }
IsMouseButtonDown :: (button: MouseButton) -> bool { return IsMouseButtonDown(cast(s32) button); }
IsMouseButtonReleased :: (button: MouseButton) -> bool { return IsMouseButtonReleased(cast(s32) button); }
IsMouseButtonUp :: (button: MouseButton) -> bool { return IsMouseButtonUp(cast(s32) button); }

IsKeyPressed :: (key: KeyboardKey) -> bool { return IsKeyPressed(cast(s32) key); }
IsKeyPressedRepeat :: (key: KeyboardKey) -> bool { return IsKeyPressedRepeat(cast(s32) key); }
IsKeyDown :: (key: KeyboardKey) -> bool { return IsKeyDown(cast(s32) key); }
IsKeyReleased :: (key: KeyboardKey) -> bool { return IsKeyReleased(cast(s32) key); }
IsKeyUp :: (key: KeyboardKey) -> bool { return IsKeyUp(cast(s32) key); }
SetExitKey :: (key: KeyboardKey) -> void { return SetExitKey(cast(s32) key); }

#scope_module

#if OS == .WINDOWS {
    #load "windows.jai";
}
#if OS == .WASM {
    #load "wasm.jai";
}

#import "Basic";
#import "Math";