
Vector2 :: Math.Vector2;
Vector3 :: Math.Vector3;
Vector4 :: Math.Vector4;
Quaternion :: Math.Quaternion;
Matrix :: Math.Matrix4;
PI :: Math.PI;

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

GetGamepadButtonPressed :: () -> GamepadButton #foreign raylib;

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

SetConfigFlags :: (flags: ConfigFlags) -> void { return SetConfigFlags(cast(u32) flags); }

SetGesturesEnabled :: (flags: Gesture) -> void { return SetGesturesEnabled(cast(u32) flags); }
IsGestureDetected :: (gesture: Gesture) -> bool { return IsGestureDetected(cast(u32) gesture); }

IsWindowState :: (flag: ConfigFlags) -> bool { return IsWindowState(cast(u32) flag); }
SetWindowState :: (flags: ConfigFlags) -> void { return SetWindowState(cast(u32) flags); }
ClearWindowState :: (flags: ConfigFlags) -> void { return ClearWindowState(cast(u32) flags); }

UpdateCamera :: (camera: *Camera, mode: CameraMode) -> void { return UpdateCamera(camera, cast(s32) mode); }

SetTraceLogLevel :: (logLevel: TraceLogLevel) -> void { return SetTraceLogLevel(cast(s32) logLevel); }
TraceLog :: (logLevel: TraceLogLevel, text: string, __args: ..Any) { TraceLog(cast(s32) logLevel, text, __args); }

SetShaderValue :: (shader: Shader, locIndex: s32, value: *void, uniformType: ShaderUniformDataType) -> void {
    return SetShaderValue(shader, locIndex, value, cast(s32) uniformType);
}
SetShaderValueV :: (shader: Shader, locIndex: s32, value: *void, uniformType: ShaderUniformDataType, count: s32) -> void {
    return SetShaderValue(shader, locIndex, value, cast(s32) uniformType, count);
}

IsGamepadButtonPressed :: (gamepad: s32, button: GamepadButton) -> bool { return IsGamepadButtonPressed(gamepad, cast(s32) button); }
IsGamepadButtonDown :: (gamepad: s32, button: GamepadButton) -> bool { return IsGamepadButtonDown(gamepad, cast(s32) button); }
IsGamepadButtonReleased :: (gamepad: s32, button: GamepadButton) -> bool { return IsGamepadButtonReleased(gamepad, cast(s32) button); }
IsGamepadButtonUp :: (gamepad: s32, button: GamepadButton) -> bool { return IsGamepadButtonUp(gamepad, cast(s32) button); }

GetGamepadAxisMovement :: (gamepad: s32, axis: GamepadAxis) -> float { return GetGamepadAxisMovement(gamepad, cast(s32) axis); }

SetTextureFilter :: (texture: Texture2D, filter: TextureFilter) -> void { return SetTextureFilter(texture, cast(s32) filter); }
SetTextureWrap :: (texture: Texture2D, wrap: TextureWrap) -> void { return SetTextureWrap(textuer, cast(s32) wrap); }

BeginBlendMode :: (mode: BlendMode) -> void { return BeginBlendMode(cast(s32) mode); }

ImageFormat :: (image: *Image, newFormat: PixelFormat) -> void { return ImageFormat(image, cast(s32) newFormat); }

LoadImageRaw :: (fileName: *u8, width: s32, height: s32, format: PixelFormat, headerSize: s32) -> Image {
    return LoadImageRaw(fileName, width, height, cast(s32) format, headerSize);
}

LoadFontData :: (fileData: *u8, dataSize: s32, fontSize: s32, codepoints: *s32, codepointCount: s32, type: FontType) -> *GlyphInfo {
    return LoadFontData(fileData, dataSize, fontSize, codepoints, codepointCount, cast(s32) type);
}

SetMouseCursor :: (cursor: MouseCursor) -> void { return SetMouseCursor(cast(s32) cursor); }

LoadTexture :: (data: *void, width: s32, height: s32, format: PixelFormat, mipmapCount: s32) -> u32 {
    return LoadTexture(data, width, height, cast(s32) format, mipmapCount);
}

FramebufferAttach :: (fboId: u32, texId: u32, attachType: FramebufferAttachType, texType: FramebufferAttachTextureType, mipLevel: s32) -> void {
    return FramebufferAttach(fbiId, texId, cast(s32) attachType, cast(s32) texType, mipLevel);
}

SetUniform :: (locIndex: s32, value: *void, uniformType: ShaderUniformDataType, count: s32) -> void { return SetUniform(locIndex, value, cast(s32) uniformType, count); }

SetMaterialTexture :: (material: *Material, mapType: MaterialMapIndex, texture: Texture2D) -> void { return SetMaterialTexture(material, mapType, texture); }

Camera3D :: struct {
    position:   Vector3; // Camera position
    target:     Vector3; // Camera target it looks-at
    up:         Vector3; // Camera up vector (rotation over its axis)
    fovy:       float; // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
    projection: CameraProjection; // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
}

TraceLogCallback :: #type (logLevel: TraceLogLevel, text: *u8, args: .. Any) #c_call;

#scope_module

#if OS == .WINDOWS {
    user32 :: #system_library,link_always "user32";
    gdi32 :: #system_library,link_always "gdi32";
    shell32 :: #system_library,link_always "shell32";
    winmm :: #system_library,link_always "winmm";

    raylib :: #library,no_dll "windows/raylib";

    #load "windows.jai";
} else #if OS == .LINUX {
    math :: #system_library,link_always "m";
    raylib :: #library,no_dll "linux/raylib";

    #load "linux.jai";
} else #if OS == .MACOS {
    #system_library,link_always "Cocoa";
    #system_library,link_always "OpenGL";
    #system_library,link_always "CoreGraphics";
    #system_library,link_always "AppKit";
    #system_library,link_always "IOKit";

    raylib :: #library,no_dll "macos/raylib";

    #load "macos.jai";
} else {
    assert(false);
}

#import "Basic";
Math :: #import "Math";
