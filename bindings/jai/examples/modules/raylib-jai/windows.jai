//
// This file was auto-generated using the following command:
//
// jai generate.jai
//



RAYLIB_VERSION_MAJOR :: 5;
RAYLIB_VERSION_MINOR :: 1;
RAYLIB_VERSION_PATCH :: 0;
RAYLIB_VERSION :: "5.1-dev";

DEG2RAD :: PI/180.0;

RAD2DEG :: 180.0/PI;

GetMouseRay :: GetScreenToWorldRay;

EPSILON :: 0.000001;

RLGL_VERSION :: "5.0";

RL_DEFAULT_BATCH_BUFFER_ELEMENTS :: 8192;

RL_DEFAULT_BATCH_BUFFERS :: 1;

RL_DEFAULT_BATCH_DRAWCALLS :: 256;

RL_DEFAULT_BATCH_MAX_TEXTURE_UNITS :: 4;

RL_MAX_MATRIX_STACK_SIZE :: 32;

RL_MAX_SHADER_LOCATIONS :: 32;

RL_CULL_DISTANCE_NEAR :: 0.01;

RL_CULL_DISTANCE_FAR :: 1000.0;

RL_TEXTURE_WRAP_S :: 0x2802;
RL_TEXTURE_WRAP_T :: 0x2803;
RL_TEXTURE_MAG_FILTER :: 0x2800;
RL_TEXTURE_MIN_FILTER :: 0x2801;

RL_TEXTURE_FILTER_NEAREST :: 0x2600;
RL_TEXTURE_FILTER_LINEAR :: 0x2601;
RL_TEXTURE_FILTER_MIP_NEAREST :: 0x2700;
RL_TEXTURE_FILTER_NEAREST_MIP_LINEAR :: 0x2702;
RL_TEXTURE_FILTER_LINEAR_MIP_NEAREST :: 0x2701;
RL_TEXTURE_FILTER_MIP_LINEAR :: 0x2703;
RL_TEXTURE_FILTER_ANISOTROPIC :: 0x3000;
RL_TEXTURE_MIPMAP_BIAS_RATIO :: 0x4000;

RL_TEXTURE_WRAP_REPEAT :: 0x2901;
RL_TEXTURE_WRAP_CLAMP :: 0x812F;
RL_TEXTURE_WRAP_MIRROR_REPEAT :: 0x8370;
RL_TEXTURE_WRAP_MIRROR_CLAMP :: 0x8742;

RL_MODELVIEW :: 0x1700;
RL_PROJECTION :: 0x1701;
RL_TEXTURE :: 0x1702;

RL_LINES :: 0x0001;
RL_TRIANGLES :: 0x0004;
RL_QUADS :: 0x0007;

RL_UNSIGNED_BYTE :: 0x1401;
RL_FLOAT :: 0x1406;

RL_STREAM_DRAW :: 0x88E0;
RL_STREAM_READ :: 0x88E1;
RL_STREAM_COPY :: 0x88E2;
RL_STATIC_DRAW :: 0x88E4;
RL_STATIC_READ :: 0x88E5;
RL_STATIC_COPY :: 0x88E6;
RL_DYNAMIC_DRAW :: 0x88E8;
RL_DYNAMIC_READ :: 0x88E9;
RL_DYNAMIC_COPY :: 0x88EA;

RL_FRAGMENT_SHADER :: 0x8B30;
RL_VERTEX_SHADER :: 0x8B31;
RL_COMPUTE_SHADER :: 0x91B9;

RL_ZERO :: 0;
RL_ONE :: 1;
RL_SRC_COLOR :: 0x0300;
RL_ONE_MINUS_SRC_COLOR :: 0x0301;
RL_SRC_ALPHA :: 0x0302;
RL_ONE_MINUS_SRC_ALPHA :: 0x0303;
RL_DST_ALPHA :: 0x0304;
RL_ONE_MINUS_DST_ALPHA :: 0x0305;
RL_DST_COLOR :: 0x0306;
RL_ONE_MINUS_DST_COLOR :: 0x0307;
RL_SRC_ALPHA_SATURATE :: 0x0308;
RL_CONSTANT_COLOR :: 0x8001;
RL_ONE_MINUS_CONSTANT_COLOR :: 0x8002;
RL_CONSTANT_ALPHA :: 0x8003;
RL_ONE_MINUS_CONSTANT_ALPHA :: 0x8004;

RL_FUNC_ADD :: 0x8006;
RL_MIN :: 0x8007;
RL_MAX :: 0x8008;
RL_FUNC_SUBTRACT :: 0x800A;
RL_FUNC_REVERSE_SUBTRACT :: 0x800B;
RL_BLEND_EQUATION :: 0x8009;
RL_BLEND_EQUATION_RGB :: 0x8009;
RL_BLEND_EQUATION_ALPHA :: 0x883D;
RL_BLEND_DST_RGB :: 0x80C8;
RL_BLEND_SRC_RGB :: 0x80C9;
RL_BLEND_DST_ALPHA :: 0x80CA;
RL_BLEND_SRC_ALPHA :: 0x80CB;
RL_BLEND_COLOR :: 0x8005;

RL_READ_FRAMEBUFFER :: 0x8CA8;
RL_DRAW_FRAMEBUFFER :: 0x8CA9;

RL_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION :: 0;

RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD :: 1;

RL_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL :: 2;

RL_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR :: 3;

RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT :: 4;

RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2 :: 5;

// Matrix, 4x4 components, column major, OpenGL style, right-handed
Matrix :: struct {
    m0:  float; // Matrix first row (4 components)
    m4:  float; // Matrix first row (4 components)
    m8:  float; // Matrix first row (4 components)
    m12: float; // Matrix first row (4 components)
    m1:  float; // Matrix second row (4 components)
    m5:  float; // Matrix second row (4 components)
    m9:  float; // Matrix second row (4 components)
    m13: float; // Matrix second row (4 components)
    m2:  float; // Matrix third row (4 components)
    m6:  float; // Matrix third row (4 components)
    m10: float; // Matrix third row (4 components)
    m14: float; // Matrix third row (4 components)
    m3:  float; // Matrix fourth row (4 components)
    m7:  float; // Matrix fourth row (4 components)
    m11: float; // Matrix fourth row (4 components)
    m15: float; // Matrix fourth row (4 components)
}

// Color, 4 components, R8G8B8A8 (32bit)
Color :: struct {
    r: u8; // Color red value
    g: u8; // Color green value
    b: u8; // Color blue value
    a: u8; // Color alpha value
}

// Rectangle, 4 components
Rectangle :: struct {
    x:      float; // Rectangle top-left corner position x
    y:      float; // Rectangle top-left corner position y
    width:  float; // Rectangle width
    height: float; // Rectangle height
}

// Image, pixel data stored in CPU memory (RAM)
Image :: struct {
    data:    *void; // Image raw data
    width:   s32; // Image base width
    height:  s32; // Image base height
    mipmaps: s32; // Mipmap levels, 1 by default
    format:  s32; // Data format (PixelFormat type)
}

// Texture, tex data stored in GPU memory (VRAM)
Texture :: struct {
    id:      u32; // OpenGL texture id
    width:   s32; // Texture base width
    height:  s32; // Texture base height
    mipmaps: s32; // Mipmap levels, 1 by default
    format:  s32; // Data format (PixelFormat type)
}

// Texture2D, same as Texture
Texture2D :: Texture;

// TextureCubemap, same as Texture
TextureCubemap :: Texture;

// RenderTexture, fbo for texture rendering
RenderTexture :: struct {
    id:      u32; // OpenGL framebuffer object id
    texture: Texture; // Color buffer attachment texture
    depth:   Texture; // Depth buffer attachment texture
}

// RenderTexture2D, same as RenderTexture
RenderTexture2D :: RenderTexture;

// NPatchInfo, n-patch layout info
NPatchInfo :: struct {
    source: Rectangle; // Texture source rectangle
    left:   s32; // Left border offset
    top:    s32; // Top border offset
    right:  s32; // Right border offset
    bottom: s32; // Bottom border offset
    layout: s32; // Layout of the n-patch: 3x3, 1x3 or 3x1
}

// GlyphInfo, font characters glyphs info
GlyphInfo :: struct {
    value:    s32; // Character value (Unicode)
    offsetX:  s32; // Character offset X when drawing
    offsetY:  s32; // Character offset Y when drawing
    advanceX: s32; // Character advance position X
    image:    Image; // Character image data
}

// Font, font texture and GlyphInfo array data
Font :: struct {
    baseSize:     s32; // Base size (default chars height)
    glyphCount:   s32; // Number of glyph characters
    glyphPadding: s32; // Padding around the glyph characters
    texture:      Texture2D; // Texture atlas containing the glyphs
    recs:         *Rectangle; // Rectangles in texture for the glyphs
    glyphs:       *GlyphInfo; // Glyphs info data
}

// Camera, defines position/orientation in 3d space
Camera3D :: struct {
    position:   Vector3; // Camera position
    target:     Vector3; // Camera target it looks-at
    up:         Vector3; // Camera up vector (rotation over its axis)
    fovy:       float; // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
    projection: s32; // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
}

Camera :: Camera3D;

// Camera2D, defines position/orientation in 2d space
Camera2D :: struct {
    offset:   Vector2; // Camera offset (displacement from target)
    target:   Vector2; // Camera target (rotation and zoom origin)
    rotation: float; // Camera rotation in degrees
    zoom:     float; // Camera zoom (scaling), should be 1.0f by default
}

// Mesh, vertex data and vao/vbo
Mesh :: struct {
    vertexCount:   s32; // Number of vertices stored in arrays
    triangleCount: s32; // Number of triangles stored (indexed or not)

    vertices:      *float; // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    texcoords:     *float; // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    texcoords2:    *float; // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    normals:       *float; // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    tangents:      *float; // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    colors:        *u8; // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    indices:       *u16; // Vertex indices (in case vertex data comes indexed)

    animVertices:  *float; // Animated vertex positions (after bones transformations)
    animNormals:   *float; // Animated normals (after bones transformations)
    boneIds:       *u8; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning)
    boneWeights:   *float; // Vertex bone weight, up to 4 bones influence by vertex (skinning)

    vaoId:         u32; // OpenGL Vertex Array Object id
    vboId:         *u32; // OpenGL Vertex Buffer Objects id (default vertex data)
}

// Shader
Shader :: struct {
    id:   u32; // Shader program id
    locs: *s32; // Shader locations array (RL_MAX_SHADER_LOCATIONS)
}

// MaterialMap
MaterialMap :: struct {
    texture: Texture2D; // Material map texture
    color:   Color; // Material map color
    value:   float; // Material map value
}

// Material, includes shader and maps
Material :: struct {
    shader: Shader; // Material shader
    maps:   *MaterialMap; // Material maps array (MAX_MATERIAL_MAPS)
    params: [4] float; // Material generic parameters (if required)
}

// Transform, vertex transformation data
Transform :: struct {
    translation: Vector3; // Translation
    rotation:    Quaternion; // Rotation
    scale:       Vector3; // Scale
}

// Bone, skeletal animation bone
BoneInfo :: struct {
    name:   [32] u8; // Bone name
    parent: s32; // Bone parent
}

// Model, meshes, materials and animation data
Model :: struct {
    transform:     Matrix; // Local transform matrix

    meshCount:     s32; // Number of meshes
    materialCount: s32; // Number of materials
    meshes:        *Mesh; // Meshes array
    materials:     *Material; // Materials array
    meshMaterial:  *s32; // Mesh material number

    boneCount:     s32; // Number of bones
    bones:         *BoneInfo; // Bones information (skeleton)
    bindPose:      *Transform; // Bones base transformation (pose)
}

// ModelAnimation
ModelAnimation :: struct {
    boneCount:  s32; // Number of bones
    frameCount: s32; // Number of animation frames
    bones:      *BoneInfo; // Bones information (skeleton)
    framePoses: **Transform; // Poses array by frame
    name:       [32] u8; // Animation name
}

// Ray, ray for raycasting
Ray :: struct {
    position:  Vector3; // Ray position (origin)
    direction: Vector3; // Ray direction
}

// RayCollision, ray hit information
RayCollision :: struct {
    hit:      bool; // Did the ray hit something?
    distance: float; // Distance to the nearest hit
    point:    Vector3; // Point of the nearest hit
    normal:   Vector3; // Surface normal of hit
}

// BoundingBox
BoundingBox :: struct {
    min: Vector3; // Minimum vertex box-corner
    max: Vector3; // Maximum vertex box-corner
}

// Wave, audio wave data
Wave :: struct {
    frameCount: u32; // Total number of frames (considering channels)
    sampleRate: u32; // Frequency (samples per second)
    sampleSize: u32; // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    channels:   u32; // Number of channels (1-mono, 2-stereo, ...)
    data:       *void; // Buffer data pointer
}

rAudioBuffer :: struct {}
rAudioProcessor :: struct {}

// AudioStream, custom audio stream
AudioStream :: struct {
    buffer:     *rAudioBuffer; // Pointer to internal data used by the audio system
    processor:  *rAudioProcessor; // Pointer to internal data processor, useful for audio effects

    sampleRate: u32; // Frequency (samples per second)
    sampleSize: u32; // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    channels:   u32; // Number of channels (1-mono, 2-stereo, ...)
}

// Sound
Sound :: struct {
    stream:     AudioStream; // Audio stream
    frameCount: u32; // Total number of frames (considering channels)
}

// Music, audio stream, anything longer than ~10 seconds should be streamed
Music :: struct {
    stream:     AudioStream; // Audio stream
    frameCount: u32; // Total number of frames (considering channels)
    looping:    bool; // Music looping enable

    ctxType:    s32; // Type of music context (audio filetype)
    ctxData:    *void; // Audio context data, depends on type
}

// VrDeviceInfo, Head-Mounted-Display device parameters
VrDeviceInfo :: struct {
    hResolution:            s32; // Horizontal resolution in pixels
    vResolution:            s32; // Vertical resolution in pixels
    hScreenSize:            float; // Horizontal size in meters
    vScreenSize:            float; // Vertical size in meters
    eyeToScreenDistance:    float; // Distance between eye and display in meters
    lensSeparationDistance: float; // Lens separation distance in meters
    interpupillaryDistance: float; // IPD (distance between pupils) in meters
    lensDistortionValues:   [4] float; // Lens distortion constant parameters
    chromaAbCorrection:     [4] float; // Chromatic aberration correction parameters
}

// VrStereoConfig, VR stereo rendering configuration for simulator
VrStereoConfig :: struct {
    projection:        [2] Matrix; // VR projection matrices (per eye)
    viewOffset:        [2] Matrix; // VR view offset matrices (per eye)
    leftLensCenter:    [2] float; // VR left lens center
    rightLensCenter:   [2] float; // VR right lens center
    leftScreenCenter:  [2] float; // VR left screen center
    rightScreenCenter: [2] float; // VR right screen center
    scale:             [2] float; // VR distortion scale
    scaleIn:           [2] float; // VR distortion scale in
}

// File path list
FilePathList :: struct {
    capacity: u32; // Filepaths max entries
    count:    u32; // Filepaths entries count
    paths:    **u8; // Filepaths entries
}

// Automation event
AutomationEvent :: struct {
    frame:  u32; // Event frame
    type:   u32; // Event type (AutomationEventType)
    params: [4] s32; // Event parameters (if required)
}

// Automation event list
AutomationEventList :: struct {
    capacity: u32; // Events max entries (MAX_AUTOMATION_EVENTS)
    count:    u32; // Events entries count
    events:   *AutomationEvent; // Events entries
}

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
ConfigFlags :: enum s32 {
    FLAG_VSYNC_HINT               :: 64;
    FLAG_FULLSCREEN_MODE          :: 2;
    FLAG_WINDOW_RESIZABLE         :: 4;
    FLAG_WINDOW_UNDECORATED       :: 8;
    FLAG_WINDOW_HIDDEN            :: 128;
    FLAG_WINDOW_MINIMIZED         :: 512;
    FLAG_WINDOW_MAXIMIZED         :: 1024;
    FLAG_WINDOW_UNFOCUSED         :: 2048;
    FLAG_WINDOW_TOPMOST           :: 4096;
    FLAG_WINDOW_ALWAYS_RUN        :: 256;
    FLAG_WINDOW_TRANSPARENT       :: 16;
    FLAG_WINDOW_HIGHDPI           :: 8192;
    FLAG_WINDOW_MOUSE_PASSTHROUGH :: 16384;
    FLAG_BORDERLESS_WINDOWED_MODE :: 32768;
    FLAG_MSAA_4X_HINT             :: 32;
    FLAG_INTERLACED_HINT          :: 65536;
}

// Keyboard keys (US keyboard layout)
// NOTE: Use GetKeyPressed() to allow redefining
// required keys for alternative layouts
KeyboardKey :: enum s32 {
    KEY_NULL          :: 0;

    KEY_APOSTROPHE    :: 39;
    KEY_COMMA         :: 44;
    KEY_MINUS         :: 45;
    KEY_PERIOD        :: 46;
    KEY_SLASH         :: 47;
    KEY_ZERO          :: 48;
    KEY_ONE           :: 49;
    KEY_TWO           :: 50;
    KEY_THREE         :: 51;
    KEY_FOUR          :: 52;
    KEY_FIVE          :: 53;
    KEY_SIX           :: 54;
    KEY_SEVEN         :: 55;
    KEY_EIGHT         :: 56;
    KEY_NINE          :: 57;
    KEY_SEMICOLON     :: 59;
    KEY_EQUAL         :: 61;
    KEY_A             :: 65;
    KEY_B             :: 66;
    KEY_C             :: 67;
    KEY_D             :: 68;
    KEY_E             :: 69;
    KEY_F             :: 70;
    KEY_G             :: 71;
    KEY_H             :: 72;
    KEY_I             :: 73;
    KEY_J             :: 74;
    KEY_K             :: 75;
    KEY_L             :: 76;
    KEY_M             :: 77;
    KEY_N             :: 78;
    KEY_O             :: 79;
    KEY_P             :: 80;
    KEY_Q             :: 81;
    KEY_R             :: 82;
    KEY_S             :: 83;
    KEY_T             :: 84;
    KEY_U             :: 85;
    KEY_V             :: 86;
    KEY_W             :: 87;
    KEY_X             :: 88;
    KEY_Y             :: 89;
    KEY_Z             :: 90;
    KEY_LEFT_BRACKET  :: 91;
    KEY_BACKSLASH     :: 92;
    KEY_RIGHT_BRACKET :: 93;
    KEY_GRAVE         :: 96;

    KEY_SPACE         :: 32;
    KEY_ESCAPE        :: 256;
    KEY_ENTER         :: 257;
    KEY_TAB           :: 258;
    KEY_BACKSPACE     :: 259;
    KEY_INSERT        :: 260;
    KEY_DELETE        :: 261;
    KEY_RIGHT         :: 262;
    KEY_LEFT          :: 263;
    KEY_DOWN          :: 264;
    KEY_UP            :: 265;
    KEY_PAGE_UP       :: 266;
    KEY_PAGE_DOWN     :: 267;
    KEY_HOME          :: 268;
    KEY_END           :: 269;
    KEY_CAPS_LOCK     :: 280;
    KEY_SCROLL_LOCK   :: 281;
    KEY_NUM_LOCK      :: 282;
    KEY_PRINT_SCREEN  :: 283;
    KEY_PAUSE         :: 284;
    KEY_F1            :: 290;
    KEY_F2            :: 291;
    KEY_F3            :: 292;
    KEY_F4            :: 293;
    KEY_F5            :: 294;
    KEY_F6            :: 295;
    KEY_F7            :: 296;
    KEY_F8            :: 297;
    KEY_F9            :: 298;
    KEY_F10           :: 299;
    KEY_F11           :: 300;
    KEY_F12           :: 301;
    KEY_LEFT_SHIFT    :: 340;
    KEY_LEFT_CONTROL  :: 341;
    KEY_LEFT_ALT      :: 342;
    KEY_LEFT_SUPER    :: 343;
    KEY_RIGHT_SHIFT   :: 344;
    KEY_RIGHT_CONTROL :: 345;
    KEY_RIGHT_ALT     :: 346;
    KEY_RIGHT_SUPER   :: 347;
    KEY_KB_MENU       :: 348;

    KEY_KP_0          :: 320;
    KEY_KP_1          :: 321;
    KEY_KP_2          :: 322;
    KEY_KP_3          :: 323;
    KEY_KP_4          :: 324;
    KEY_KP_5          :: 325;
    KEY_KP_6          :: 326;
    KEY_KP_7          :: 327;
    KEY_KP_8          :: 328;
    KEY_KP_9          :: 329;
    KEY_KP_DECIMAL    :: 330;
    KEY_KP_DIVIDE     :: 331;
    KEY_KP_MULTIPLY   :: 332;
    KEY_KP_SUBTRACT   :: 333;
    KEY_KP_ADD        :: 334;
    KEY_KP_ENTER      :: 335;
    KEY_KP_EQUAL      :: 336;

    KEY_BACK          :: 4;
    KEY_MENU          :: 5;
    KEY_VOLUME_UP     :: 24;
    KEY_VOLUME_DOWN   :: 25;
}

// Mouse buttons
MouseButton :: enum s32 {
    MOUSE_BUTTON_LEFT    :: 0;
    MOUSE_BUTTON_RIGHT   :: 1;
    MOUSE_BUTTON_MIDDLE  :: 2;
    MOUSE_BUTTON_SIDE    :: 3;
    MOUSE_BUTTON_EXTRA   :: 4;
    MOUSE_BUTTON_FORWARD :: 5;
    MOUSE_BUTTON_BACK    :: 6;
}

// Mouse cursor
MouseCursor :: enum s32 {
    MOUSE_CURSOR_DEFAULT       :: 0;
    MOUSE_CURSOR_ARROW         :: 1;
    MOUSE_CURSOR_IBEAM         :: 2;
    MOUSE_CURSOR_CROSSHAIR     :: 3;
    MOUSE_CURSOR_POINTING_HAND :: 4;
    MOUSE_CURSOR_RESIZE_EW     :: 5;
    MOUSE_CURSOR_RESIZE_NS     :: 6;
    MOUSE_CURSOR_RESIZE_NWSE   :: 7;
    MOUSE_CURSOR_RESIZE_NESW   :: 8;
    MOUSE_CURSOR_RESIZE_ALL    :: 9;
    MOUSE_CURSOR_NOT_ALLOWED   :: 10;
}

// Gamepad buttons
GamepadButton :: enum s32 {
    GAMEPAD_BUTTON_UNKNOWN          :: 0;
    GAMEPAD_BUTTON_LEFT_FACE_UP     :: 1;
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT  :: 2;
    GAMEPAD_BUTTON_LEFT_FACE_DOWN   :: 3;
    GAMEPAD_BUTTON_LEFT_FACE_LEFT   :: 4;
    GAMEPAD_BUTTON_RIGHT_FACE_UP    :: 5;
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT :: 6;
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN  :: 7;
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT  :: 8;
    GAMEPAD_BUTTON_LEFT_TRIGGER_1   :: 9;
    GAMEPAD_BUTTON_LEFT_TRIGGER_2   :: 10;
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1  :: 11;
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2  :: 12;
    GAMEPAD_BUTTON_MIDDLE_LEFT      :: 13;
    GAMEPAD_BUTTON_MIDDLE           :: 14;
    GAMEPAD_BUTTON_MIDDLE_RIGHT     :: 15;
    GAMEPAD_BUTTON_LEFT_THUMB       :: 16;
    GAMEPAD_BUTTON_RIGHT_THUMB      :: 17;
}

// Gamepad axis
GamepadAxis :: enum s32 {
    GAMEPAD_AXIS_LEFT_X        :: 0;
    GAMEPAD_AXIS_LEFT_Y        :: 1;
    GAMEPAD_AXIS_RIGHT_X       :: 2;
    GAMEPAD_AXIS_RIGHT_Y       :: 3;
    GAMEPAD_AXIS_LEFT_TRIGGER  :: 4;
    GAMEPAD_AXIS_RIGHT_TRIGGER :: 5;
}

// Material map index
MaterialMapIndex :: enum s32 {
    MATERIAL_MAP_ALBEDO     :: 0;
    MATERIAL_MAP_METALNESS  :: 1;
    MATERIAL_MAP_NORMAL     :: 2;
    MATERIAL_MAP_ROUGHNESS  :: 3;
    MATERIAL_MAP_OCCLUSION  :: 4;
    MATERIAL_MAP_EMISSION   :: 5;
    MATERIAL_MAP_HEIGHT     :: 6;
    MATERIAL_MAP_CUBEMAP    :: 7;
    MATERIAL_MAP_IRRADIANCE :: 8;
    MATERIAL_MAP_PREFILTER  :: 9;
    MATERIAL_MAP_BRDF       :: 10;
}

// Texture parameters: wrap mode
TextureWrap :: enum s32 {
    TEXTURE_WRAP_REPEAT        :: 0;
    TEXTURE_WRAP_CLAMP         :: 1;
    TEXTURE_WRAP_MIRROR_REPEAT :: 2;
    TEXTURE_WRAP_MIRROR_CLAMP  :: 3;
}

// Cubemap layouts
CubemapLayout :: enum s32 {
    CUBEMAP_LAYOUT_AUTO_DETECT         :: 0;
    CUBEMAP_LAYOUT_LINE_VERTICAL       :: 1;
    CUBEMAP_LAYOUT_LINE_HORIZONTAL     :: 2;
    CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR :: 3;
    CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE :: 4;
    CUBEMAP_LAYOUT_PANORAMA            :: 5;
}

// Font type, defines generation method
FontType :: enum s32 {
    FONT_DEFAULT :: 0;
    FONT_BITMAP  :: 1;
    FONT_SDF     :: 2;
}

// Gesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
Gesture :: enum s32 {
    GESTURE_NONE        :: 0;
    GESTURE_TAP         :: 1;
    GESTURE_DOUBLETAP   :: 2;
    GESTURE_HOLD        :: 4;
    GESTURE_DRAG        :: 8;
    GESTURE_SWIPE_RIGHT :: 16;
    GESTURE_SWIPE_LEFT  :: 32;
    GESTURE_SWIPE_UP    :: 64;
    GESTURE_SWIPE_DOWN  :: 128;
    GESTURE_PINCH_IN    :: 256;
    GESTURE_PINCH_OUT   :: 512;
}

// Camera system modes
CameraMode :: enum s32 {
    CAMERA_CUSTOM       :: 0;
    CAMERA_FREE         :: 1;
    CAMERA_ORBITAL      :: 2;
    CAMERA_FIRST_PERSON :: 3;
    CAMERA_THIRD_PERSON :: 4;
}

// Camera projection
CameraProjection :: enum s32 {
    CAMERA_PERSPECTIVE  :: 0;
    CAMERA_ORTHOGRAPHIC :: 1;
}

// N-patch layout
NPatchLayout :: enum s32 {
    NPATCH_NINE_PATCH             :: 0;
    NPATCH_THREE_PATCH_VERTICAL   :: 1;
    NPATCH_THREE_PATCH_HORIZONTAL :: 2;
}

// Callbacks to hook some internal functions
// WARNING: These callbacks are intended for advanced users
TraceLogCallback :: *void /* function type contained C va_list argument */;
LoadFileDataCallback :: #type (fileName: *u8, dataSize: *s32) -> *u8 #c_call;
SaveFileDataCallback :: #type (fileName: *u8, data: *void, dataSize: s32) -> bool #c_call;
LoadFileTextCallback :: #type (fileName: *u8) -> *u8 #c_call;
SaveFileTextCallback :: #type (fileName: *u8, text: *u8) -> bool #c_call;

// Window-related functions
InitWindow :: (width: s32, height: s32, title: *u8) -> void #foreign raylib;
CloseWindow :: () -> void #foreign raylib;
WindowShouldClose :: () -> bool #foreign raylib;
IsWindowReady :: () -> bool #foreign raylib;
IsWindowFullscreen :: () -> bool #foreign raylib;
IsWindowHidden :: () -> bool #foreign raylib;
IsWindowMinimized :: () -> bool #foreign raylib;
IsWindowMaximized :: () -> bool #foreign raylib;
IsWindowFocused :: () -> bool #foreign raylib;
IsWindowResized :: () -> bool #foreign raylib;
IsWindowState :: (flag: u32) -> bool #foreign raylib;
SetWindowState :: (flags: u32) -> void #foreign raylib;
ClearWindowState :: (flags: u32) -> void #foreign raylib;
ToggleFullscreen :: () -> void #foreign raylib;
ToggleBorderlessWindowed :: () -> void #foreign raylib;
MaximizeWindow :: () -> void #foreign raylib;
MinimizeWindow :: () -> void #foreign raylib;
RestoreWindow :: () -> void #foreign raylib;
SetWindowIcon :: (image: Image) -> void #foreign raylib;
SetWindowIcons :: (images: *Image, count: s32) -> void #foreign raylib;
SetWindowTitle :: (title: *u8) -> void #foreign raylib;
SetWindowPosition :: (x: s32, y: s32) -> void #foreign raylib;
SetWindowMonitor :: (monitor: s32) -> void #foreign raylib;
SetWindowMinSize :: (width: s32, height: s32) -> void #foreign raylib;
SetWindowMaxSize :: (width: s32, height: s32) -> void #foreign raylib;
SetWindowSize :: (width: s32, height: s32) -> void #foreign raylib;
SetWindowOpacity :: (opacity: float) -> void #foreign raylib;
SetWindowFocused :: () -> void #foreign raylib;
GetWindowHandle :: () -> *void #foreign raylib;
GetScreenWidth :: () -> s32 #foreign raylib;
GetScreenHeight :: () -> s32 #foreign raylib;
GetRenderWidth :: () -> s32 #foreign raylib;
GetRenderHeight :: () -> s32 #foreign raylib;
GetMonitorCount :: () -> s32 #foreign raylib;
GetCurrentMonitor :: () -> s32 #foreign raylib;
GetMonitorPosition :: (monitor: s32) -> Vector2 #foreign raylib;
GetMonitorWidth :: (monitor: s32) -> s32 #foreign raylib;
GetMonitorHeight :: (monitor: s32) -> s32 #foreign raylib;
GetMonitorPhysicalWidth :: (monitor: s32) -> s32 #foreign raylib;
GetMonitorPhysicalHeight :: (monitor: s32) -> s32 #foreign raylib;
GetMonitorRefreshRate :: (monitor: s32) -> s32 #foreign raylib;
GetWindowPosition :: () -> Vector2 #foreign raylib;
GetWindowScaleDPI :: () -> Vector2 #foreign raylib;
GetMonitorName :: (monitor: s32) -> *u8 #foreign raylib;
SetClipboardText :: (text: *u8) -> void #foreign raylib;
GetClipboardText :: () -> *u8 #foreign raylib;
EnableEventWaiting :: () -> void #foreign raylib;
DisableEventWaiting :: () -> void #foreign raylib;

// Cursor-related functions
ShowCursor :: () -> void #foreign raylib;
HideCursor :: () -> void #foreign raylib;
IsCursorHidden :: () -> bool #foreign raylib;
EnableCursor :: () -> void #foreign raylib;
DisableCursor :: () -> void #foreign raylib;
IsCursorOnScreen :: () -> bool #foreign raylib;

// Drawing-related functions
ClearBackground :: (color: Color) -> void #foreign raylib;
BeginDrawing :: () -> void #foreign raylib;
EndDrawing :: () -> void #foreign raylib;
BeginMode2D :: (camera: Camera2D) -> void #foreign raylib;
EndMode2D :: () -> void #foreign raylib;
BeginMode3D :: (camera: Camera3D) -> void #foreign raylib;
EndMode3D :: () -> void #foreign raylib;
BeginTextureMode :: (target: RenderTexture2D) -> void #foreign raylib;
EndTextureMode :: () -> void #foreign raylib;
BeginShaderMode :: (shader: Shader) -> void #foreign raylib;
EndShaderMode :: () -> void #foreign raylib;
BeginBlendMode :: (mode: s32) -> void #foreign raylib;
EndBlendMode :: () -> void #foreign raylib;
BeginScissorMode :: (x: s32, y: s32, width: s32, height: s32) -> void #foreign raylib;
EndScissorMode :: () -> void #foreign raylib;
BeginVrStereoMode :: (config: VrStereoConfig) -> void #foreign raylib;
EndVrStereoMode :: () -> void #foreign raylib;

// VR stereo config functions for VR simulator
LoadVrStereoConfig :: (device: VrDeviceInfo) -> VrStereoConfig #foreign raylib;
UnloadVrStereoConfig :: (config: VrStereoConfig) -> void #foreign raylib;

// Shader management functions
// NOTE: Shader functionality is not available on OpenGL 1.1
LoadShader :: (vsFileName: *u8, fsFileName: *u8) -> Shader #foreign raylib;
LoadShaderFromMemory :: (vsCode: *u8, fsCode: *u8) -> Shader #foreign raylib;
IsShaderReady :: (shader: Shader) -> bool #foreign raylib;
GetShaderLocation :: (shader: Shader, uniformName: *u8) -> s32 #foreign raylib;
GetShaderLocationAttrib :: (shader: Shader, attribName: *u8) -> s32 #foreign raylib;
SetShaderValue :: (shader: Shader, locIndex: s32, value: *void, uniformType: s32) -> void #foreign raylib;
SetShaderValueV :: (shader: Shader, locIndex: s32, value: *void, uniformType: s32, count: s32) -> void #foreign raylib;
SetShaderValueMatrix :: (shader: Shader, locIndex: s32, mat: Matrix) -> void #foreign raylib;
SetShaderValueTexture :: (shader: Shader, locIndex: s32, texture: Texture2D) -> void #foreign raylib;
UnloadShader :: (shader: Shader) -> void #foreign raylib;

GetScreenToWorldRay :: (position: Vector2, camera: Camera) -> Ray #foreign raylib;
GetScreenToWorldRayEx :: (position: Vector2, camera: Camera, width: s32, height: s32) -> Ray #foreign raylib;
GetWorldToScreen :: (position: Vector3, camera: Camera) -> Vector2 #foreign raylib;
GetWorldToScreenEx :: (position: Vector3, camera: Camera, width: s32, height: s32) -> Vector2 #foreign raylib;
GetWorldToScreen2D :: (position: Vector2, camera: Camera2D) -> Vector2 #foreign raylib;
GetScreenToWorld2D :: (position: Vector2, camera: Camera2D) -> Vector2 #foreign raylib;
GetCameraMatrix :: (camera: Camera) -> Matrix #foreign raylib;
GetCameraMatrix2D :: (camera: Camera2D) -> Matrix #foreign raylib;

// Timing-related functions
SetTargetFPS :: (fps: s32) -> void #foreign raylib;
GetFrameTime :: () -> float #foreign raylib;
GetTime :: () -> float64 #foreign raylib;
GetFPS :: () -> s32 #foreign raylib;

// Custom frame control functions
// NOTE: Those functions are intended for advanced users that want full control over the frame processing
// By default EndDrawing() does this job: draws everything + SwapScreenBuffer() + manage frame timing + PollInputEvents()
// To avoid that behaviour and control frame processes manually, enable in config.h: SUPPORT_CUSTOM_FRAME_CONTROL
SwapScreenBuffer :: () -> void #foreign raylib;
PollInputEvents :: () -> void #foreign raylib;
WaitTime :: (seconds: float64) -> void #foreign raylib;

// Random values generation functions
SetRandomSeed :: (seed: u32) -> void #foreign raylib;
GetRandomValue :: (min: s32, max: s32) -> s32 #foreign raylib;
LoadRandomSequence :: (count: u32, min: s32, max: s32) -> *s32 #foreign raylib;
UnloadRandomSequence :: (sequence: *s32) -> void #foreign raylib;

// Misc. functions
TakeScreenshot :: (fileName: *u8) -> void #foreign raylib;
SetConfigFlags :: (flags: u32) -> void #foreign raylib;
OpenURL :: (url: *u8) -> void #foreign raylib;

// NOTE: Following functions implemented in module [utils]
//------------------------------------------------------------------
TraceLog_CFormat :: (logLevel: s32, text: *u8, __args: ..Any) -> void #foreign raylib "TraceLog";
TraceLog :: (logLevel: s32, text: string, __args: ..Any) {
    push_allocator(temp);
    formatted_text_builder: String_Builder;
    print_to_builder(*formatted_text_builder, text, ..__args);
    append(*formatted_text_builder, "\0");
    formatted_text := builder_to_string(*formatted_text_builder);
    TraceLog_CFormat(logLevel, "%s", formatted_text.data);
} @PrintLike
SetTraceLogLevel :: (logLevel: s32) -> void #foreign raylib;
MemAlloc :: (size: u32) -> *void #foreign raylib;
MemRealloc :: (ptr: *void, size: u32) -> *void #foreign raylib;
MemFree :: (ptr: *void) -> void #foreign raylib;

// Set custom callbacks
// WARNING: Callbacks setup is intended for advanced users
SetTraceLogCallback :: (callback: TraceLogCallback) -> void #foreign raylib;
SetLoadFileDataCallback :: (callback: LoadFileDataCallback) -> void #foreign raylib;
SetSaveFileDataCallback :: (callback: SaveFileDataCallback) -> void #foreign raylib;
SetLoadFileTextCallback :: (callback: LoadFileTextCallback) -> void #foreign raylib;
SetSaveFileTextCallback :: (callback: SaveFileTextCallback) -> void #foreign raylib;

// Files management functions
LoadFileData :: (fileName: *u8, dataSize: *s32) -> *u8 #foreign raylib;
UnloadFileData :: (data: *u8) -> void #foreign raylib;
SaveFileData :: (fileName: *u8, data: *void, dataSize: s32) -> bool #foreign raylib;
ExportDataAsCode :: (data: *u8, dataSize: s32, fileName: *u8) -> bool #foreign raylib;
LoadFileText :: (fileName: *u8) -> *u8 #foreign raylib;
UnloadFileText :: (text: *u8) -> void #foreign raylib;
SaveFileText :: (fileName: *u8, text: *u8) -> bool #foreign raylib;

// File system functions
FileExists :: (fileName: *u8) -> bool #foreign raylib;
DirectoryExists :: (dirPath: *u8) -> bool #foreign raylib;
IsFileExtension :: (fileName: *u8, ext: *u8) -> bool #foreign raylib;
GetFileLength :: (fileName: *u8) -> s32 #foreign raylib;
GetFileExtension :: (fileName: *u8) -> *u8 #foreign raylib;
GetFileName :: (filePath: *u8) -> *u8 #foreign raylib;
GetFileNameWithoutExt :: (filePath: *u8) -> *u8 #foreign raylib;
GetDirectoryPath :: (filePath: *u8) -> *u8 #foreign raylib;
GetPrevDirectoryPath :: (dirPath: *u8) -> *u8 #foreign raylib;
GetWorkingDirectory :: () -> *u8 #foreign raylib;
GetApplicationDirectory :: () -> *u8 #foreign raylib;
ChangeDirectory :: (dir: *u8) -> bool #foreign raylib;
IsPathFile :: (path: *u8) -> bool #foreign raylib;
IsFileNameValid :: (fileName: *u8) -> bool #foreign raylib;
LoadDirectoryFiles :: (dirPath: *u8) -> FilePathList #foreign raylib;
LoadDirectoryFilesEx :: (basePath: *u8, filter: *u8, scanSubdirs: bool) -> FilePathList #foreign raylib;
UnloadDirectoryFiles :: (files: FilePathList) -> void #foreign raylib;
IsFileDropped :: () -> bool #foreign raylib;
LoadDroppedFiles :: () -> FilePathList #foreign raylib;
UnloadDroppedFiles :: (files: FilePathList) -> void #foreign raylib;
GetFileModTime :: (fileName: *u8) -> s32 #foreign raylib;

// Compression/Encoding functionality
CompressData :: (data: *u8, dataSize: s32, compDataSize: *s32) -> *u8 #foreign raylib;
DecompressData :: (compData: *u8, compDataSize: s32, dataSize: *s32) -> *u8 #foreign raylib;
EncodeDataBase64 :: (data: *u8, dataSize: s32, outputSize: *s32) -> *u8 #foreign raylib;
DecodeDataBase64 :: (data: *u8, outputSize: *s32) -> *u8 #foreign raylib;

// Automation events functionality
LoadAutomationEventList :: (fileName: *u8) -> AutomationEventList #foreign raylib;
UnloadAutomationEventList :: (list: AutomationEventList) -> void #foreign raylib;
ExportAutomationEventList :: (list: AutomationEventList, fileName: *u8) -> bool #foreign raylib;
SetAutomationEventList :: (list: *AutomationEventList) -> void #foreign raylib;
SetAutomationEventBaseFrame :: (frame: s32) -> void #foreign raylib;
StartAutomationEventRecording :: () -> void #foreign raylib;
StopAutomationEventRecording :: () -> void #foreign raylib;
PlayAutomationEvent :: (event: AutomationEvent) -> void #foreign raylib;

// Input-related functions: keyboard
IsKeyPressed :: (key: s32) -> bool #foreign raylib;
IsKeyPressedRepeat :: (key: s32) -> bool #foreign raylib;
IsKeyDown :: (key: s32) -> bool #foreign raylib;
IsKeyReleased :: (key: s32) -> bool #foreign raylib;
IsKeyUp :: (key: s32) -> bool #foreign raylib;
GetKeyPressed :: () -> s32 #foreign raylib;
GetCharPressed :: () -> s32 #foreign raylib;
SetExitKey :: (key: s32) -> void #foreign raylib;

// Input-related functions: gamepads
IsGamepadAvailable :: (gamepad: s32) -> bool #foreign raylib;
GetGamepadName :: (gamepad: s32) -> *u8 #foreign raylib;
IsGamepadButtonPressed :: (gamepad: s32, button: s32) -> bool #foreign raylib;
IsGamepadButtonDown :: (gamepad: s32, button: s32) -> bool #foreign raylib;
IsGamepadButtonReleased :: (gamepad: s32, button: s32) -> bool #foreign raylib;
IsGamepadButtonUp :: (gamepad: s32, button: s32) -> bool #foreign raylib;
GetGamepadButtonPressed :: () -> s32 #foreign raylib;
GetGamepadAxisCount :: (gamepad: s32) -> s32 #foreign raylib;
GetGamepadAxisMovement :: (gamepad: s32, axis: s32) -> float #foreign raylib;
SetGamepadMappings :: (mappings: *u8) -> s32 #foreign raylib;
SetGamepadVibration :: (gamepad: s32, leftMotor: float, rightMotor: float) -> void #foreign raylib;

// Input-related functions: mouse
IsMouseButtonPressed :: (button: s32) -> bool #foreign raylib;
IsMouseButtonDown :: (button: s32) -> bool #foreign raylib;
IsMouseButtonReleased :: (button: s32) -> bool #foreign raylib;
IsMouseButtonUp :: (button: s32) -> bool #foreign raylib;
GetMouseX :: () -> s32 #foreign raylib;
GetMouseY :: () -> s32 #foreign raylib;
GetMousePosition :: () -> Vector2 #foreign raylib;
GetMouseDelta :: () -> Vector2 #foreign raylib;
SetMousePosition :: (x: s32, y: s32) -> void #foreign raylib;
SetMouseOffset :: (offsetX: s32, offsetY: s32) -> void #foreign raylib;
SetMouseScale :: (scaleX: float, scaleY: float) -> void #foreign raylib;
GetMouseWheelMove :: () -> float #foreign raylib;
GetMouseWheelMoveV :: () -> Vector2 #foreign raylib;
SetMouseCursor :: (cursor: s32) -> void #foreign raylib;

// Input-related functions: touch
GetTouchX :: () -> s32 #foreign raylib;
GetTouchY :: () -> s32 #foreign raylib;
GetTouchPosition :: (index: s32) -> Vector2 #foreign raylib;
GetTouchPointId :: (index: s32) -> s32 #foreign raylib;
GetTouchPointCount :: () -> s32 #foreign raylib;

//------------------------------------------------------------------------------------
// Gestures and Touch Handling Functions (Module: rgestures)
//------------------------------------------------------------------------------------
SetGesturesEnabled :: (flags: u32) -> void #foreign raylib;
IsGestureDetected :: (gesture: u32) -> bool #foreign raylib;
GetGestureDetected :: () -> s32 #foreign raylib;
GetGestureHoldDuration :: () -> float #foreign raylib;
GetGestureDragVector :: () -> Vector2 #foreign raylib;
GetGestureDragAngle :: () -> float #foreign raylib;
GetGesturePinchVector :: () -> Vector2 #foreign raylib;
GetGesturePinchAngle :: () -> float #foreign raylib;

//------------------------------------------------------------------------------------
// Camera System Functions (Module: rcamera)
//------------------------------------------------------------------------------------
UpdateCamera :: (camera: *Camera, mode: s32) -> void #foreign raylib;
UpdateCameraPro :: (camera: *Camera, movement: Vector3, rotation: Vector3, zoom: float) -> void #foreign raylib;

//------------------------------------------------------------------------------------
// Basic Shapes Drawing Functions (Module: shapes)
//------------------------------------------------------------------------------------
// Set texture and rectangle to be used on shapes drawing
// NOTE: It can be useful when using basic shapes and one single font,
// defining a font char white rectangle would allow drawing everything in a single draw call
SetShapesTexture :: (texture: Texture2D, source: Rectangle) -> void #foreign raylib;
GetShapesTexture :: () -> Texture2D #foreign raylib;
GetShapesTextureRectangle :: () -> Rectangle #foreign raylib;

// Basic shapes drawing functions
DrawPixel :: (posX: s32, posY: s32, color: Color) -> void #foreign raylib;
DrawPixelV :: (position: Vector2, color: Color) -> void #foreign raylib;
DrawLine :: (startPosX: s32, startPosY: s32, endPosX: s32, endPosY: s32, color: Color) -> void #foreign raylib;
DrawLineV :: (startPos: Vector2, endPos: Vector2, color: Color) -> void #foreign raylib;
DrawLineEx :: (startPos: Vector2, endPos: Vector2, thick: float, color: Color) -> void #foreign raylib;
DrawLineStrip :: (points: *Vector2, pointCount: s32, color: Color) -> void #foreign raylib;
DrawLineBezier :: (startPos: Vector2, endPos: Vector2, thick: float, color: Color) -> void #foreign raylib;
DrawCircle :: (centerX: s32, centerY: s32, radius: float, color: Color) -> void #foreign raylib;
DrawCircleSector :: (center: Vector2, radius: float, startAngle: float, endAngle: float, segments: s32, color: Color) -> void #foreign raylib;
DrawCircleSectorLines :: (center: Vector2, radius: float, startAngle: float, endAngle: float, segments: s32, color: Color) -> void #foreign raylib;
DrawCircleGradient :: (centerX: s32, centerY: s32, radius: float, color1: Color, color2: Color) -> void #foreign raylib;
DrawCircleV :: (center: Vector2, radius: float, color: Color) -> void #foreign raylib;
DrawCircleLines :: (centerX: s32, centerY: s32, radius: float, color: Color) -> void #foreign raylib;
DrawCircleLinesV :: (center: Vector2, radius: float, color: Color) -> void #foreign raylib;
DrawEllipse :: (centerX: s32, centerY: s32, radiusH: float, radiusV: float, color: Color) -> void #foreign raylib;
DrawEllipseLines :: (centerX: s32, centerY: s32, radiusH: float, radiusV: float, color: Color) -> void #foreign raylib;
DrawRing :: (center: Vector2, innerRadius: float, outerRadius: float, startAngle: float, endAngle: float, segments: s32, color: Color) -> void #foreign raylib;
DrawRingLines :: (center: Vector2, innerRadius: float, outerRadius: float, startAngle: float, endAngle: float, segments: s32, color: Color) -> void #foreign raylib;
DrawRectangle :: (posX: s32, posY: s32, width: s32, height: s32, color: Color) -> void #foreign raylib;
DrawRectangleV :: (position: Vector2, size: Vector2, color: Color) -> void #foreign raylib;
DrawRectangleRec :: (rec: Rectangle, color: Color) -> void #foreign raylib;
DrawRectanglePro :: (rec: Rectangle, origin: Vector2, rotation: float, color: Color) -> void #foreign raylib;
DrawRectangleGradientV :: (posX: s32, posY: s32, width: s32, height: s32, color1: Color, color2: Color) -> void #foreign raylib;
DrawRectangleGradientH :: (posX: s32, posY: s32, width: s32, height: s32, color1: Color, color2: Color) -> void #foreign raylib;
DrawRectangleGradientEx :: (rec: Rectangle, col1: Color, col2: Color, col3: Color, col4: Color) -> void #foreign raylib;
DrawRectangleLines :: (posX: s32, posY: s32, width: s32, height: s32, color: Color) -> void #foreign raylib;
DrawRectangleLinesEx :: (rec: Rectangle, lineThick: float, color: Color) -> void #foreign raylib;
DrawRectangleRounded :: (rec: Rectangle, roundness: float, segments: s32, color: Color) -> void #foreign raylib;
DrawRectangleRoundedLines :: (rec: Rectangle, roundness: float, segments: s32, color: Color) -> void #foreign raylib;
DrawRectangleRoundedLinesEx :: (rec: Rectangle, roundness: float, segments: s32, lineThick: float, color: Color) -> void #foreign raylib;
DrawTriangle :: (v1: Vector2, v2: Vector2, v3: Vector2, color: Color) -> void #foreign raylib;
DrawTriangleLines :: (v1: Vector2, v2: Vector2, v3: Vector2, color: Color) -> void #foreign raylib;
DrawTriangleFan :: (points: *Vector2, pointCount: s32, color: Color) -> void #foreign raylib;
DrawTriangleStrip :: (points: *Vector2, pointCount: s32, color: Color) -> void #foreign raylib;
DrawPoly :: (center: Vector2, sides: s32, radius: float, rotation: float, color: Color) -> void #foreign raylib;
DrawPolyLines :: (center: Vector2, sides: s32, radius: float, rotation: float, color: Color) -> void #foreign raylib;
DrawPolyLinesEx :: (center: Vector2, sides: s32, radius: float, rotation: float, lineThick: float, color: Color) -> void #foreign raylib;

// Splines drawing functions
DrawSplineLinear :: (points: *Vector2, pointCount: s32, thick: float, color: Color) -> void #foreign raylib;
DrawSplineBasis :: (points: *Vector2, pointCount: s32, thick: float, color: Color) -> void #foreign raylib;
DrawSplineCatmullRom :: (points: *Vector2, pointCount: s32, thick: float, color: Color) -> void #foreign raylib;
DrawSplineBezierQuadratic :: (points: *Vector2, pointCount: s32, thick: float, color: Color) -> void #foreign raylib;
DrawSplineBezierCubic :: (points: *Vector2, pointCount: s32, thick: float, color: Color) -> void #foreign raylib;
DrawSplineSegmentLinear :: (p1: Vector2, p2: Vector2, thick: float, color: Color) -> void #foreign raylib;
DrawSplineSegmentBasis :: (p1: Vector2, p2: Vector2, p3: Vector2, p4: Vector2, thick: float, color: Color) -> void #foreign raylib;
DrawSplineSegmentCatmullRom :: (p1: Vector2, p2: Vector2, p3: Vector2, p4: Vector2, thick: float, color: Color) -> void #foreign raylib;
DrawSplineSegmentBezierQuadratic :: (p1: Vector2, c2: Vector2, p3: Vector2, thick: float, color: Color) -> void #foreign raylib;
DrawSplineSegmentBezierCubic :: (p1: Vector2, c2: Vector2, c3: Vector2, p4: Vector2, thick: float, color: Color) -> void #foreign raylib;

// Spline segment point evaluation functions, for a given t [0.0f .. 1.0f]
GetSplinePointLinear :: (startPos: Vector2, endPos: Vector2, t: float) -> Vector2 #foreign raylib;
GetSplinePointBasis :: (p1: Vector2, p2: Vector2, p3: Vector2, p4: Vector2, t: float) -> Vector2 #foreign raylib;
GetSplinePointCatmullRom :: (p1: Vector2, p2: Vector2, p3: Vector2, p4: Vector2, t: float) -> Vector2 #foreign raylib;
GetSplinePointBezierQuad :: (p1: Vector2, c2: Vector2, p3: Vector2, t: float) -> Vector2 #foreign raylib;
GetSplinePointBezierCubic :: (p1: Vector2, c2: Vector2, c3: Vector2, p4: Vector2, t: float) -> Vector2 #foreign raylib;

// Basic shapes collision detection functions
CheckCollisionRecs :: (rec1: Rectangle, rec2: Rectangle) -> bool #foreign raylib;
CheckCollisionCircles :: (center1: Vector2, radius1: float, center2: Vector2, radius2: float) -> bool #foreign raylib;
CheckCollisionCircleRec :: (center: Vector2, radius: float, rec: Rectangle) -> bool #foreign raylib;
CheckCollisionPointRec :: (point: Vector2, rec: Rectangle) -> bool #foreign raylib;
CheckCollisionPointCircle :: (point: Vector2, center: Vector2, radius: float) -> bool #foreign raylib;
CheckCollisionPointTriangle :: (point: Vector2, p1: Vector2, p2: Vector2, p3: Vector2) -> bool #foreign raylib;
CheckCollisionPointPoly :: (point: Vector2, points: *Vector2, pointCount: s32) -> bool #foreign raylib;
CheckCollisionLines :: (startPos1: Vector2, endPos1: Vector2, startPos2: Vector2, endPos2: Vector2, collisionPoint: *Vector2) -> bool #foreign raylib;
CheckCollisionPointLine :: (point: Vector2, p1: Vector2, p2: Vector2, threshold: s32) -> bool #foreign raylib;
CheckCollisionCircleLine :: (center: Vector2, radius: float, p1: Vector2, p2: Vector2) -> bool #foreign raylib;
GetCollisionRec :: (rec1: Rectangle, rec2: Rectangle) -> Rectangle #foreign raylib;

// Image loading functions
// NOTE: These functions do not require GPU access
LoadImage :: (fileName: *u8) -> Image #foreign raylib;
LoadImageRaw :: (fileName: *u8, width: s32, height: s32, format: s32, headerSize: s32) -> Image #foreign raylib;
LoadImageSvg :: (fileNameOrString: *u8, width: s32, height: s32) -> Image #foreign raylib;
LoadImageAnim :: (fileName: *u8, frames: *s32) -> Image #foreign raylib;
LoadImageAnimFromMemory :: (fileType: *u8, fileData: *u8, dataSize: s32, frames: *s32) -> Image #foreign raylib;
LoadImageFromMemory :: (fileType: *u8, fileData: *u8, dataSize: s32) -> Image #foreign raylib;
LoadImageFromTexture :: (texture: Texture2D) -> Image #foreign raylib;
LoadImageFromScreen :: () -> Image #foreign raylib;
IsImageReady :: (image: Image) -> bool #foreign raylib;
UnloadImage :: (image: Image) -> void #foreign raylib;
ExportImage :: (image: Image, fileName: *u8) -> bool #foreign raylib;
ExportImageToMemory :: (image: Image, fileType: *u8, fileSize: *s32) -> *u8 #foreign raylib;
ExportImageAsCode :: (image: Image, fileName: *u8) -> bool #foreign raylib;

// Image generation functions
GenImageColor :: (width: s32, height: s32, color: Color) -> Image #foreign raylib;
GenImageGradientLinear :: (width: s32, height: s32, direction: s32, start: Color, end: Color) -> Image #foreign raylib;
GenImageGradientRadial :: (width: s32, height: s32, density: float, inner: Color, outer: Color) -> Image #foreign raylib;
GenImageGradientSquare :: (width: s32, height: s32, density: float, inner: Color, outer: Color) -> Image #foreign raylib;
GenImageChecked :: (width: s32, height: s32, checksX: s32, checksY: s32, col1: Color, col2: Color) -> Image #foreign raylib;
GenImageWhiteNoise :: (width: s32, height: s32, factor: float) -> Image #foreign raylib;
GenImagePerlinNoise :: (width: s32, height: s32, offsetX: s32, offsetY: s32, scale: float) -> Image #foreign raylib;
GenImageCellular :: (width: s32, height: s32, tileSize: s32) -> Image #foreign raylib;
GenImageText :: (width: s32, height: s32, text: *u8) -> Image #foreign raylib;

// Image manipulation functions
ImageCopy :: (image: Image) -> Image #foreign raylib;
ImageFromImage :: (image: Image, rec: Rectangle) -> Image #foreign raylib;
ImageText :: (text: *u8, fontSize: s32, color: Color) -> Image #foreign raylib;
ImageTextEx :: (font: Font, text: *u8, fontSize: float, spacing: float, tint: Color) -> Image #foreign raylib;
ImageFormat :: (image: *Image, newFormat: s32) -> void #foreign raylib;
ImageToPOT :: (image: *Image, fill: Color) -> void #foreign raylib;
ImageCrop :: (image: *Image, crop: Rectangle) -> void #foreign raylib;
ImageAlphaCrop :: (image: *Image, threshold: float) -> void #foreign raylib;
ImageAlphaClear :: (image: *Image, color: Color, threshold: float) -> void #foreign raylib;
ImageAlphaMask :: (image: *Image, alphaMask: Image) -> void #foreign raylib;
ImageAlphaPremultiply :: (image: *Image) -> void #foreign raylib;
ImageBlurGaussian :: (image: *Image, blurSize: s32) -> void #foreign raylib;
ImageKernelConvolution :: (image: *Image, kernel: *float, kernelSize: s32) -> void #foreign raylib;
ImageResize :: (image: *Image, newWidth: s32, newHeight: s32) -> void #foreign raylib;
ImageResizeNN :: (image: *Image, newWidth: s32, newHeight: s32) -> void #foreign raylib;
ImageResizeCanvas :: (image: *Image, newWidth: s32, newHeight: s32, offsetX: s32, offsetY: s32, fill: Color) -> void #foreign raylib;
ImageMipmaps :: (image: *Image) -> void #foreign raylib;
ImageDither :: (image: *Image, rBpp: s32, gBpp: s32, bBpp: s32, aBpp: s32) -> void #foreign raylib;
ImageFlipVertical :: (image: *Image) -> void #foreign raylib;
ImageFlipHorizontal :: (image: *Image) -> void #foreign raylib;
ImageRotate :: (image: *Image, degrees: s32) -> void #foreign raylib;
ImageRotateCW :: (image: *Image) -> void #foreign raylib;
ImageRotateCCW :: (image: *Image) -> void #foreign raylib;
ImageColorTint :: (image: *Image, color: Color) -> void #foreign raylib;
ImageColorInvert :: (image: *Image) -> void #foreign raylib;
ImageColorGrayscale :: (image: *Image) -> void #foreign raylib;
ImageColorContrast :: (image: *Image, contrast: float) -> void #foreign raylib;
ImageColorBrightness :: (image: *Image, brightness: s32) -> void #foreign raylib;
ImageColorReplace :: (image: *Image, color: Color, replace: Color) -> void #foreign raylib;
LoadImageColors :: (image: Image) -> *Color #foreign raylib;
LoadImagePalette :: (image: Image, maxPaletteSize: s32, colorCount: *s32) -> *Color #foreign raylib;
UnloadImageColors :: (colors: *Color) -> void #foreign raylib;
UnloadImagePalette :: (colors: *Color) -> void #foreign raylib;
GetImageAlphaBorder :: (image: Image, threshold: float) -> Rectangle #foreign raylib;
GetImageColor :: (image: Image, x: s32, y: s32) -> Color #foreign raylib;

// Image drawing functions
// NOTE: Image software-rendering functions (CPU)
ImageClearBackground :: (dst: *Image, color: Color) -> void #foreign raylib;
ImageDrawPixel :: (dst: *Image, posX: s32, posY: s32, color: Color) -> void #foreign raylib;
ImageDrawPixelV :: (dst: *Image, position: Vector2, color: Color) -> void #foreign raylib;
ImageDrawLine :: (dst: *Image, startPosX: s32, startPosY: s32, endPosX: s32, endPosY: s32, color: Color) -> void #foreign raylib;
ImageDrawLineV :: (dst: *Image, start: Vector2, end: Vector2, color: Color) -> void #foreign raylib;
ImageDrawCircle :: (dst: *Image, centerX: s32, centerY: s32, radius: s32, color: Color) -> void #foreign raylib;
ImageDrawCircleV :: (dst: *Image, center: Vector2, radius: s32, color: Color) -> void #foreign raylib;
ImageDrawCircleLines :: (dst: *Image, centerX: s32, centerY: s32, radius: s32, color: Color) -> void #foreign raylib;
ImageDrawCircleLinesV :: (dst: *Image, center: Vector2, radius: s32, color: Color) -> void #foreign raylib;
ImageDrawRectangle :: (dst: *Image, posX: s32, posY: s32, width: s32, height: s32, color: Color) -> void #foreign raylib;
ImageDrawRectangleV :: (dst: *Image, position: Vector2, size: Vector2, color: Color) -> void #foreign raylib;
ImageDrawRectangleRec :: (dst: *Image, rec: Rectangle, color: Color) -> void #foreign raylib;
ImageDrawRectangleLines :: (dst: *Image, rec: Rectangle, thick: s32, color: Color) -> void #foreign raylib;
ImageDraw :: (dst: *Image, src: Image, srcRec: Rectangle, dstRec: Rectangle, tint: Color) -> void #foreign raylib;
ImageDrawText :: (dst: *Image, text: *u8, posX: s32, posY: s32, fontSize: s32, color: Color) -> void #foreign raylib;
ImageDrawTextEx :: (dst: *Image, font: Font, text: *u8, position: Vector2, fontSize: float, spacing: float, tint: Color) -> void #foreign raylib;

// Texture loading functions
// NOTE: These functions require GPU access
LoadTexture :: (fileName: *u8) -> Texture2D #foreign raylib;
LoadTextureFromImage :: (image: Image) -> Texture2D #foreign raylib;
LoadTextureCubemap :: (image: Image, layout: s32) -> TextureCubemap #foreign raylib;
LoadRenderTexture :: (width: s32, height: s32) -> RenderTexture2D #foreign raylib;
IsTextureReady :: (texture: Texture2D) -> bool #foreign raylib;
UnloadTexture :: (texture: Texture2D) -> void #foreign raylib;
IsRenderTextureReady :: (target: RenderTexture2D) -> bool #foreign raylib;
UnloadRenderTexture :: (target: RenderTexture2D) -> void #foreign raylib;
UpdateTexture :: (texture: Texture2D, pixels: *void) -> void #foreign raylib;
UpdateTextureRec :: (texture: Texture2D, rec: Rectangle, pixels: *void) -> void #foreign raylib;

// Texture configuration functions
GenTextureMipmaps :: (texture: *Texture2D) -> void #foreign raylib;
SetTextureFilter :: (texture: Texture2D, filter: s32) -> void #foreign raylib;
SetTextureWrap :: (texture: Texture2D, wrap: s32) -> void #foreign raylib;

// Texture drawing functions
DrawTexture :: (texture: Texture2D, posX: s32, posY: s32, tint: Color) -> void #foreign raylib;
DrawTextureV :: (texture: Texture2D, position: Vector2, tint: Color) -> void #foreign raylib;
DrawTextureEx :: (texture: Texture2D, position: Vector2, rotation: float, scale: float, tint: Color) -> void #foreign raylib;
DrawTextureRec :: (texture: Texture2D, source: Rectangle, position: Vector2, tint: Color) -> void #foreign raylib;
DrawTexturePro :: (texture: Texture2D, source: Rectangle, dest: Rectangle, origin: Vector2, rotation: float, tint: Color) -> void #foreign raylib;
DrawTextureNPatch :: (texture: Texture2D, nPatchInfo: NPatchInfo, dest: Rectangle, origin: Vector2, rotation: float, tint: Color) -> void #foreign raylib;

// Color/pixel related functions
ColorIsEqual :: (col1: Color, col2: Color) -> bool #foreign raylib;
Fade :: (color: Color, alpha: float) -> Color #foreign raylib;
ColorToInt :: (color: Color) -> s32 #foreign raylib;
ColorNormalize :: (color: Color) -> Vector4 #foreign raylib;
ColorFromNormalized :: (normalized: Vector4) -> Color #foreign raylib;
ColorToHSV :: (color: Color) -> Vector3 #foreign raylib;
ColorFromHSV :: (hue: float, saturation: float, value: float) -> Color #foreign raylib;
ColorTint :: (color: Color, tint: Color) -> Color #foreign raylib;
ColorBrightness :: (color: Color, factor: float) -> Color #foreign raylib;
ColorContrast :: (color: Color, contrast: float) -> Color #foreign raylib;
ColorAlpha :: (color: Color, alpha: float) -> Color #foreign raylib;
ColorAlphaBlend :: (dst: Color, src: Color, tint: Color) -> Color #foreign raylib;
GetColor :: (hexValue: u32) -> Color #foreign raylib;
GetPixelColor :: (srcPtr: *void, format: s32) -> Color #foreign raylib;
SetPixelColor :: (dstPtr: *void, color: Color, format: s32) -> void #foreign raylib;
GetPixelDataSize :: (width: s32, height: s32, format: s32) -> s32 #foreign raylib;

// Font loading/unloading functions
GetFontDefault :: () -> Font #foreign raylib;
LoadFont :: (fileName: *u8) -> Font #foreign raylib;
LoadFontEx :: (fileName: *u8, fontSize: s32, codepoints: *s32, codepointCount: s32) -> Font #foreign raylib;
LoadFontFromImage :: (image: Image, key: Color, firstChar: s32) -> Font #foreign raylib;
LoadFontFromMemory :: (fileType: *u8, fileData: *u8, dataSize: s32, fontSize: s32, codepoints: *s32, codepointCount: s32) -> Font #foreign raylib;
IsFontReady :: (font: Font) -> bool #foreign raylib;
LoadFontData :: (fileData: *u8, dataSize: s32, fontSize: s32, codepoints: *s32, codepointCount: s32, type: s32) -> *GlyphInfo #foreign raylib;
GenImageFontAtlas :: (glyphs: *GlyphInfo, glyphRecs: **Rectangle, glyphCount: s32, fontSize: s32, padding: s32, packMethod: s32) -> Image #foreign raylib;
UnloadFontData :: (glyphs: *GlyphInfo, glyphCount: s32) -> void #foreign raylib;
UnloadFont :: (font: Font) -> void #foreign raylib;
ExportFontAsCode :: (font: Font, fileName: *u8) -> bool #foreign raylib;

// Text drawing functions
DrawFPS :: (posX: s32, posY: s32) -> void #foreign raylib;
DrawText :: (text: *u8, posX: s32, posY: s32, fontSize: s32, color: Color) -> void #foreign raylib;
DrawTextEx :: (font: Font, text: *u8, position: Vector2, fontSize: float, spacing: float, tint: Color) -> void #foreign raylib;
DrawTextPro :: (font: Font, text: *u8, position: Vector2, origin: Vector2, rotation: float, fontSize: float, spacing: float, tint: Color) -> void #foreign raylib;
DrawTextCodepoint :: (font: Font, codepoint: s32, position: Vector2, fontSize: float, tint: Color) -> void #foreign raylib;
DrawTextCodepoints :: (font: Font, codepoints: *s32, codepointCount: s32, position: Vector2, fontSize: float, spacing: float, tint: Color) -> void #foreign raylib;

// Text font info functions
SetTextLineSpacing :: (spacing: s32) -> void #foreign raylib;
MeasureText :: (text: *u8, fontSize: s32) -> s32 #foreign raylib;
MeasureTextEx :: (font: Font, text: *u8, fontSize: float, spacing: float) -> Vector2 #foreign raylib;
GetGlyphIndex :: (font: Font, codepoint: s32) -> s32 #foreign raylib;
GetGlyphInfo :: (font: Font, codepoint: s32) -> GlyphInfo #foreign raylib;
GetGlyphAtlasRec :: (font: Font, codepoint: s32) -> Rectangle #foreign raylib;

// Text codepoints management functions (unicode characters)
LoadUTF8 :: (codepoints: *s32, length: s32) -> *u8 #foreign raylib;
UnloadUTF8 :: (text: *u8) -> void #foreign raylib;
LoadCodepoints :: (text: *u8, count: *s32) -> *s32 #foreign raylib;
UnloadCodepoints :: (codepoints: *s32) -> void #foreign raylib;
GetCodepointCount :: (text: *u8) -> s32 #foreign raylib;
GetCodepoint :: (text: *u8, codepointSize: *s32) -> s32 #foreign raylib;
GetCodepointNext :: (text: *u8, codepointSize: *s32) -> s32 #foreign raylib;
GetCodepointPrevious :: (text: *u8, codepointSize: *s32) -> s32 #foreign raylib;
CodepointToUTF8 :: (codepoint: s32, utf8Size: *s32) -> *u8 #foreign raylib;

// Text strings management functions (no UTF-8 strings, only byte chars)
// NOTE: Some strings allocate memory internally for returned strings, just be careful!
TextCopy :: (dst: *u8, src: *u8) -> s32 #foreign raylib;
TextIsEqual :: (text1: *u8, text2: *u8) -> bool #foreign raylib;
TextLength :: (text: *u8) -> u32 #foreign raylib;
TextFormat_CFormat :: (text: *u8, __args: ..Any) -> *u8 #foreign raylib "TextFormat";
TextFormat :: (text: string, __args: ..Any) -> *u8 {
    push_allocator(temp);
    formatted_text_builder: String_Builder;
    print_to_builder(*formatted_text_builder, text, ..__args);
    append(*formatted_text_builder, "\0");
    formatted_text := builder_to_string(*formatted_text_builder);
    return TextFormat_CFormat("%s", formatted_text.data);
} @PrintLike
TextSubtext :: (text: *u8, position: s32, length: s32) -> *u8 #foreign raylib;
TextReplace :: (text: *u8, replace: *u8, by: *u8) -> *u8 #foreign raylib;
TextInsert :: (text: *u8, insert: *u8, position: s32) -> *u8 #foreign raylib;
TextJoin :: (textList: **u8, count: s32, delimiter: *u8) -> *u8 #foreign raylib;
TextSplit :: (text: *u8, delimiter: u8, count: *s32) -> **u8 #foreign raylib;
TextAppend :: (text: *u8, append: *u8, position: *s32) -> void #foreign raylib;
TextFindIndex :: (text: *u8, find: *u8) -> s32 #foreign raylib;
TextToUpper :: (text: *u8) -> *u8 #foreign raylib;
TextToLower :: (text: *u8) -> *u8 #foreign raylib;
TextToPascal :: (text: *u8) -> *u8 #foreign raylib;
TextToSnake :: (text: *u8) -> *u8 #foreign raylib;
TextToCamel :: (text: *u8) -> *u8 #foreign raylib;

TextToInteger :: (text: *u8) -> s32 #foreign raylib;
TextToFloat :: (text: *u8) -> float #foreign raylib;

// Basic geometric 3D shapes drawing functions
DrawLine3D :: (startPos: Vector3, endPos: Vector3, color: Color) -> void #foreign raylib;
DrawPoint3D :: (position: Vector3, color: Color) -> void #foreign raylib;
DrawCircle3D :: (center: Vector3, radius: float, rotationAxis: Vector3, rotationAngle: float, color: Color) -> void #foreign raylib;
DrawTriangle3D :: (v1: Vector3, v2: Vector3, v3: Vector3, color: Color) -> void #foreign raylib;
DrawTriangleStrip3D :: (points: *Vector3, pointCount: s32, color: Color) -> void #foreign raylib;
DrawCube :: (position: Vector3, width: float, height: float, length: float, color: Color) -> void #foreign raylib;
DrawCubeV :: (position: Vector3, size: Vector3, color: Color) -> void #foreign raylib;
DrawCubeWires :: (position: Vector3, width: float, height: float, length: float, color: Color) -> void #foreign raylib;
DrawCubeWiresV :: (position: Vector3, size: Vector3, color: Color) -> void #foreign raylib;
DrawSphere :: (centerPos: Vector3, radius: float, color: Color) -> void #foreign raylib;
DrawSphereEx :: (centerPos: Vector3, radius: float, rings: s32, slices: s32, color: Color) -> void #foreign raylib;
DrawSphereWires :: (centerPos: Vector3, radius: float, rings: s32, slices: s32, color: Color) -> void #foreign raylib;
DrawCylinder :: (position: Vector3, radiusTop: float, radiusBottom: float, height: float, slices: s32, color: Color) -> void #foreign raylib;
DrawCylinderEx :: (startPos: Vector3, endPos: Vector3, startRadius: float, endRadius: float, sides: s32, color: Color) -> void #foreign raylib;
DrawCylinderWires :: (position: Vector3, radiusTop: float, radiusBottom: float, height: float, slices: s32, color: Color) -> void #foreign raylib;
DrawCylinderWiresEx :: (startPos: Vector3, endPos: Vector3, startRadius: float, endRadius: float, sides: s32, color: Color) -> void #foreign raylib;
DrawCapsule :: (startPos: Vector3, endPos: Vector3, radius: float, slices: s32, rings: s32, color: Color) -> void #foreign raylib;
DrawCapsuleWires :: (startPos: Vector3, endPos: Vector3, radius: float, slices: s32, rings: s32, color: Color) -> void #foreign raylib;
DrawPlane :: (centerPos: Vector3, size: Vector2, color: Color) -> void #foreign raylib;
DrawRay :: (ray: Ray, color: Color) -> void #foreign raylib;
DrawGrid :: (slices: s32, spacing: float) -> void #foreign raylib;

// Model management functions
LoadModel :: (fileName: *u8) -> Model #foreign raylib;
LoadModelFromMesh :: (mesh: Mesh) -> Model #foreign raylib;
IsModelReady :: (model: Model) -> bool #foreign raylib;
UnloadModel :: (model: Model) -> void #foreign raylib;
GetModelBoundingBox :: (model: Model) -> BoundingBox #foreign raylib;

// Model drawing functions
DrawModel :: (model: Model, position: Vector3, scale: float, tint: Color) -> void #foreign raylib;
DrawModelEx :: (model: Model, position: Vector3, rotationAxis: Vector3, rotationAngle: float, scale: Vector3, tint: Color) -> void #foreign raylib;
DrawModelWires :: (model: Model, position: Vector3, scale: float, tint: Color) -> void #foreign raylib;
DrawModelWiresEx :: (model: Model, position: Vector3, rotationAxis: Vector3, rotationAngle: float, scale: Vector3, tint: Color) -> void #foreign raylib;
DrawBoundingBox :: (box: BoundingBox, color: Color) -> void #foreign raylib;
DrawBillboard :: (camera: Camera, texture: Texture2D, position: Vector3, size: float, tint: Color) -> void #foreign raylib;
DrawBillboardRec :: (camera: Camera, texture: Texture2D, source: Rectangle, position: Vector3, size: Vector2, tint: Color) -> void #foreign raylib;
DrawBillboardPro :: (camera: Camera, texture: Texture2D, source: Rectangle, position: Vector3, up: Vector3, size: Vector2, origin: Vector2, rotation: float, tint: Color) -> void #foreign raylib;

// Mesh management functions
UploadMesh :: (mesh: *Mesh, dynamic: bool) -> void #foreign raylib;
UpdateMeshBuffer :: (mesh: Mesh, index: s32, data: *void, dataSize: s32, offset: s32) -> void #foreign raylib;
UnloadMesh :: (mesh: Mesh) -> void #foreign raylib;
DrawMesh :: (mesh: Mesh, material: Material, transform: Matrix) -> void #foreign raylib;
DrawMeshInstanced :: (mesh: Mesh, material: Material, transforms: *Matrix, instances: s32) -> void #foreign raylib;
GetMeshBoundingBox :: (mesh: Mesh) -> BoundingBox #foreign raylib;
GenMeshTangents :: (mesh: *Mesh) -> void #foreign raylib;
ExportMesh :: (mesh: Mesh, fileName: *u8) -> bool #foreign raylib;
ExportMeshAsCode :: (mesh: Mesh, fileName: *u8) -> bool #foreign raylib;

// Mesh generation functions
GenMeshPoly :: (sides: s32, radius: float) -> Mesh #foreign raylib;
GenMeshPlane :: (width: float, length: float, resX: s32, resZ: s32) -> Mesh #foreign raylib;
GenMeshCube :: (width: float, height: float, length: float) -> Mesh #foreign raylib;
GenMeshSphere :: (radius: float, rings: s32, slices: s32) -> Mesh #foreign raylib;
GenMeshHemiSphere :: (radius: float, rings: s32, slices: s32) -> Mesh #foreign raylib;
GenMeshCylinder :: (radius: float, height: float, slices: s32) -> Mesh #foreign raylib;
GenMeshCone :: (radius: float, height: float, slices: s32) -> Mesh #foreign raylib;
GenMeshTorus :: (radius: float, size: float, radSeg: s32, sides: s32) -> Mesh #foreign raylib;
GenMeshKnot :: (radius: float, size: float, radSeg: s32, sides: s32) -> Mesh #foreign raylib;
GenMeshHeightmap :: (heightmap: Image, size: Vector3) -> Mesh #foreign raylib;
GenMeshCubicmap :: (cubicmap: Image, cubeSize: Vector3) -> Mesh #foreign raylib;

// Material loading/unloading functions
LoadMaterials :: (fileName: *u8, materialCount: *s32) -> *Material #foreign raylib;
LoadMaterialDefault :: () -> Material #foreign raylib;
IsMaterialReady :: (material: Material) -> bool #foreign raylib;
UnloadMaterial :: (material: Material) -> void #foreign raylib;
SetMaterialTexture :: (material: *Material, mapType: s32, texture: Texture2D) -> void #foreign raylib;
SetModelMeshMaterial :: (model: *Model, meshId: s32, materialId: s32) -> void #foreign raylib;

// Model animations loading/unloading functions
LoadModelAnimations :: (fileName: *u8, animCount: *s32) -> *ModelAnimation #foreign raylib;
UpdateModelAnimation :: (model: Model, anim: ModelAnimation, frame: s32) -> void #foreign raylib;
UnloadModelAnimation :: (anim: ModelAnimation) -> void #foreign raylib;
UnloadModelAnimations :: (animations: *ModelAnimation, animCount: s32) -> void #foreign raylib;
IsModelAnimationValid :: (model: Model, anim: ModelAnimation) -> bool #foreign raylib;

// Collision detection functions
CheckCollisionSpheres :: (center1: Vector3, radius1: float, center2: Vector3, radius2: float) -> bool #foreign raylib;
CheckCollisionBoxes :: (box1: BoundingBox, box2: BoundingBox) -> bool #foreign raylib;
CheckCollisionBoxSphere :: (box: BoundingBox, center: Vector3, radius: float) -> bool #foreign raylib;
GetRayCollisionSphere :: (ray: Ray, center: Vector3, radius: float) -> RayCollision #foreign raylib;
GetRayCollisionBox :: (ray: Ray, box: BoundingBox) -> RayCollision #foreign raylib;
GetRayCollisionMesh :: (ray: Ray, mesh: Mesh, transform: Matrix) -> RayCollision #foreign raylib;
GetRayCollisionTriangle :: (ray: Ray, p1: Vector3, p2: Vector3, p3: Vector3) -> RayCollision #foreign raylib;
GetRayCollisionQuad :: (ray: Ray, p1: Vector3, p2: Vector3, p3: Vector3, p4: Vector3) -> RayCollision #foreign raylib;

//------------------------------------------------------------------------------------
// Audio Loading and Playing Functions (Module: audio)
//------------------------------------------------------------------------------------
AudioCallback :: #type (bufferData: *void, frames: u32) -> void #c_call;

// Audio device management functions
InitAudioDevice :: () -> void #foreign raylib;
CloseAudioDevice :: () -> void #foreign raylib;
IsAudioDeviceReady :: () -> bool #foreign raylib;
SetMasterVolume :: (volume: float) -> void #foreign raylib;
GetMasterVolume :: () -> float #foreign raylib;

// Wave/Sound loading/unloading functions
LoadWave :: (fileName: *u8) -> Wave #foreign raylib;
LoadWaveFromMemory :: (fileType: *u8, fileData: *u8, dataSize: s32) -> Wave #foreign raylib;
IsWaveReady :: (wave: Wave) -> bool #foreign raylib;
LoadSound :: (fileName: *u8) -> Sound #foreign raylib;
LoadSoundFromWave :: (wave: Wave) -> Sound #foreign raylib;
LoadSoundAlias :: (source: Sound) -> Sound #foreign raylib;
IsSoundReady :: (sound: Sound) -> bool #foreign raylib;
UpdateSound :: (sound: Sound, data: *void, sampleCount: s32) -> void #foreign raylib;
UnloadWave :: (wave: Wave) -> void #foreign raylib;
UnloadSound :: (sound: Sound) -> void #foreign raylib;
UnloadSoundAlias :: (alias: Sound) -> void #foreign raylib;
ExportWave :: (wave: Wave, fileName: *u8) -> bool #foreign raylib;
ExportWaveAsCode :: (wave: Wave, fileName: *u8) -> bool #foreign raylib;

// Wave/Sound management functions
PlaySound :: (sound: Sound) -> void #foreign raylib;
StopSound :: (sound: Sound) -> void #foreign raylib;
PauseSound :: (sound: Sound) -> void #foreign raylib;
ResumeSound :: (sound: Sound) -> void #foreign raylib;
IsSoundPlaying :: (sound: Sound) -> bool #foreign raylib;
SetSoundVolume :: (sound: Sound, volume: float) -> void #foreign raylib;
SetSoundPitch :: (sound: Sound, pitch: float) -> void #foreign raylib;
SetSoundPan :: (sound: Sound, pan: float) -> void #foreign raylib;
WaveCopy :: (wave: Wave) -> Wave #foreign raylib;
WaveCrop :: (wave: *Wave, initFrame: s32, finalFrame: s32) -> void #foreign raylib;
WaveFormat :: (wave: *Wave, sampleRate: s32, sampleSize: s32, channels: s32) -> void #foreign raylib;
LoadWaveSamples :: (wave: Wave) -> *float #foreign raylib;
UnloadWaveSamples :: (samples: *float) -> void #foreign raylib;

// Music management functions
LoadMusicStream :: (fileName: *u8) -> Music #foreign raylib;
LoadMusicStreamFromMemory :: (fileType: *u8, data: *u8, dataSize: s32) -> Music #foreign raylib;
IsMusicReady :: (music: Music) -> bool #foreign raylib;
UnloadMusicStream :: (music: Music) -> void #foreign raylib;
PlayMusicStream :: (music: Music) -> void #foreign raylib;
IsMusicStreamPlaying :: (music: Music) -> bool #foreign raylib;
UpdateMusicStream :: (music: Music) -> void #foreign raylib;
StopMusicStream :: (music: Music) -> void #foreign raylib;
PauseMusicStream :: (music: Music) -> void #foreign raylib;
ResumeMusicStream :: (music: Music) -> void #foreign raylib;
SeekMusicStream :: (music: Music, position: float) -> void #foreign raylib;
SetMusicVolume :: (music: Music, volume: float) -> void #foreign raylib;
SetMusicPitch :: (music: Music, pitch: float) -> void #foreign raylib;
SetMusicPan :: (music: Music, pan: float) -> void #foreign raylib;
GetMusicTimeLength :: (music: Music) -> float #foreign raylib;
GetMusicTimePlayed :: (music: Music) -> float #foreign raylib;

// AudioStream management functions
LoadAudioStream :: (sampleRate: u32, sampleSize: u32, channels: u32) -> AudioStream #foreign raylib;
IsAudioStreamReady :: (stream: AudioStream) -> bool #foreign raylib;
UnloadAudioStream :: (stream: AudioStream) -> void #foreign raylib;
UpdateAudioStream :: (stream: AudioStream, data: *void, frameCount: s32) -> void #foreign raylib;
IsAudioStreamProcessed :: (stream: AudioStream) -> bool #foreign raylib;
PlayAudioStream :: (stream: AudioStream) -> void #foreign raylib;
PauseAudioStream :: (stream: AudioStream) -> void #foreign raylib;
ResumeAudioStream :: (stream: AudioStream) -> void #foreign raylib;
IsAudioStreamPlaying :: (stream: AudioStream) -> bool #foreign raylib;
StopAudioStream :: (stream: AudioStream) -> void #foreign raylib;
SetAudioStreamVolume :: (stream: AudioStream, volume: float) -> void #foreign raylib;
SetAudioStreamPitch :: (stream: AudioStream, pitch: float) -> void #foreign raylib;
SetAudioStreamPan :: (stream: AudioStream, pan: float) -> void #foreign raylib;
SetAudioStreamBufferSizeDefault :: (size: s32) -> void #foreign raylib;
SetAudioStreamCallback :: (stream: AudioStream, callback: AudioCallback) -> void #foreign raylib;

AttachAudioStreamProcessor :: (stream: AudioStream, processor: AudioCallback) -> void #foreign raylib;
DetachAudioStreamProcessor :: (stream: AudioStream, processor: AudioCallback) -> void #foreign raylib;

AttachAudioMixedProcessor :: (processor: AudioCallback) -> void #foreign raylib;
DetachAudioMixedProcessor :: (processor: AudioCallback) -> void #foreign raylib;

// NOTE: Helper types to be used instead of array return types for *ToFloat functions
float3 :: struct {
    v: [3] float;
}

float16 :: struct {
    v: [16] float;
}

// Clamp float value
Clamp :: (value: float, min: float, max: float) -> float #foreign raylib;

// Calculate linear interpolation between two floats
Lerp :: (start: float, end: float, amount: float) -> float #foreign raylib;

// Normalize input value within input range
Normalize :: (value: float, start: float, end: float) -> float #foreign raylib;

// Remap input value within input range to output range
Remap :: (value: float, inputStart: float, inputEnd: float, outputStart: float, outputEnd: float) -> float #foreign raylib;

// Wrap input value from min to max
Wrap :: (value: float, min: float, max: float) -> float #foreign raylib;

// Check whether two given floats are almost equal
FloatEquals :: (x: float, y: float) -> s32 #foreign raylib;

// Vector with components value 0.0f
Vector2Zero :: () -> Vector2 #foreign raylib;

// Vector with components value 1.0f
Vector2One :: () -> Vector2 #foreign raylib;

// Add two vectors (v1 + v2)
Vector2Add :: (v1: Vector2, v2: Vector2) -> Vector2 #foreign raylib;

// Add vector and float value
Vector2AddValue :: (v: Vector2, add: float) -> Vector2 #foreign raylib;

// Subtract two vectors (v1 - v2)
Vector2Subtract :: (v1: Vector2, v2: Vector2) -> Vector2 #foreign raylib;

// Subtract vector by float value
Vector2SubtractValue :: (v: Vector2, sub: float) -> Vector2 #foreign raylib;

// Calculate vector length
Vector2Length :: (v: Vector2) -> float #foreign raylib;

// Calculate vector square length
Vector2LengthSqr :: (v: Vector2) -> float #foreign raylib;

// Calculate two vectors dot product
Vector2DotProduct :: (v1: Vector2, v2: Vector2) -> float #foreign raylib;

// Calculate distance between two vectors
Vector2Distance :: (v1: Vector2, v2: Vector2) -> float #foreign raylib;

// Calculate square distance between two vectors
Vector2DistanceSqr :: (v1: Vector2, v2: Vector2) -> float #foreign raylib;

// Calculate angle between two vectors
// NOTE: Angle is calculated from origin point (0, 0)
Vector2Angle :: (v1: Vector2, v2: Vector2) -> float #foreign raylib;

// Calculate angle defined by a two vectors line
// NOTE: Parameters need to be normalized
// Current implementation should be aligned with glm::angle
Vector2LineAngle :: (start: Vector2, end: Vector2) -> float #foreign raylib;

// Scale vector (multiply by value)
Vector2Scale :: (v: Vector2, scale: float) -> Vector2 #foreign raylib;

// Multiply vector by vector
Vector2Multiply :: (v1: Vector2, v2: Vector2) -> Vector2 #foreign raylib;

// Negate vector
Vector2Negate :: (v: Vector2) -> Vector2 #foreign raylib;

// Divide vector by vector
Vector2Divide :: (v1: Vector2, v2: Vector2) -> Vector2 #foreign raylib;

// Normalize provided vector
Vector2Normalize :: (v: Vector2) -> Vector2 #foreign raylib;

// Transforms a Vector2 by a given Matrix
Vector2Transform :: (v: Vector2, mat: Matrix) -> Vector2 #foreign raylib;

// Calculate linear interpolation between two vectors
Vector2Lerp :: (v1: Vector2, v2: Vector2, amount: float) -> Vector2 #foreign raylib;

// Calculate reflected vector to normal
Vector2Reflect :: (v: Vector2, normal: Vector2) -> Vector2 #foreign raylib;

// Get min value for each pair of components
Vector2Min :: (v1: Vector2, v2: Vector2) -> Vector2 #foreign raylib;

// Get max value for each pair of components
Vector2Max :: (v1: Vector2, v2: Vector2) -> Vector2 #foreign raylib;

// Rotate vector by angle
Vector2Rotate :: (v: Vector2, angle: float) -> Vector2 #foreign raylib;

// Move Vector towards target
Vector2MoveTowards :: (v: Vector2, target: Vector2, maxDistance: float) -> Vector2 #foreign raylib;

// Invert the given vector
Vector2Invert :: (v: Vector2) -> Vector2 #foreign raylib;

// Clamp the components of the vector between
// min and max values specified by the given vectors
Vector2Clamp :: (v: Vector2, min: Vector2, max: Vector2) -> Vector2 #foreign raylib;

// Clamp the magnitude of the vector between two min and max values
Vector2ClampValue :: (v: Vector2, min: float, max: float) -> Vector2 #foreign raylib;

// Check whether two given vectors are almost equal
Vector2Equals :: (p: Vector2, q: Vector2) -> s32 #foreign raylib;

// Compute the direction of a refracted ray
// v: normalized direction of the incoming ray
// n: normalized normal vector of the interface of two optical media
// r: ratio of the refractive index of the medium from where the ray comes
//    to the refractive index of the medium on the other side of the surface
Vector2Refract :: (v: Vector2, n: Vector2, r: float) -> Vector2 #foreign raylib;

// Vector with components value 0.0f
Vector3Zero :: () -> Vector3 #foreign raylib;

// Vector with components value 1.0f
Vector3One :: () -> Vector3 #foreign raylib;

// Add two vectors
Vector3Add :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

// Add vector and float value
Vector3AddValue :: (v: Vector3, add: float) -> Vector3 #foreign raylib;

// Subtract two vectors
Vector3Subtract :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

// Subtract vector by float value
Vector3SubtractValue :: (v: Vector3, sub: float) -> Vector3 #foreign raylib;

// Multiply vector by scalar
Vector3Scale :: (v: Vector3, scalar: float) -> Vector3 #foreign raylib;

// Multiply vector by vector
Vector3Multiply :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

// Calculate two vectors cross product
Vector3CrossProduct :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

// Calculate one vector perpendicular vector
Vector3Perpendicular :: (v: Vector3) -> Vector3 #foreign raylib;

// Calculate vector length
Vector3Length :: (v: Vector3) -> float #foreign raylib;

// Calculate vector square length
Vector3LengthSqr :: (v: Vector3) -> float #foreign raylib;

// Calculate two vectors dot product
Vector3DotProduct :: (v1: Vector3, v2: Vector3) -> float #foreign raylib;

// Calculate distance between two vectors
Vector3Distance :: (v1: Vector3, v2: Vector3) -> float #foreign raylib;

// Calculate square distance between two vectors
Vector3DistanceSqr :: (v1: Vector3, v2: Vector3) -> float #foreign raylib;

// Calculate angle between two vectors
Vector3Angle :: (v1: Vector3, v2: Vector3) -> float #foreign raylib;

// Negate provided vector (invert direction)
Vector3Negate :: (v: Vector3) -> Vector3 #foreign raylib;

// Divide vector by vector
Vector3Divide :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

// Normalize provided vector
Vector3Normalize :: (v: Vector3) -> Vector3 #foreign raylib;

//Calculate the projection of the vector v1 on to v2
Vector3Project :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

//Calculate the rejection of the vector v1 on to v2
Vector3Reject :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

// Orthonormalize provided vectors
// Makes vectors normalized and orthogonal to each other
// Gram-Schmidt function implementation
Vector3OrthoNormalize :: (v1: *Vector3, v2: *Vector3) -> void #foreign raylib;

// Transforms a Vector3 by a given Matrix
Vector3Transform :: (v: Vector3, mat: Matrix) -> Vector3 #foreign raylib;

// Transform a vector by quaternion rotation
Vector3RotateByQuaternion :: (v: Vector3, q: Quaternion) -> Vector3 #foreign raylib;

// Rotates a vector around an axis
Vector3RotateByAxisAngle :: (v: Vector3, axis: Vector3, angle: float) -> Vector3 #foreign raylib;

// Move Vector towards target
Vector3MoveTowards :: (v: Vector3, target: Vector3, maxDistance: float) -> Vector3 #foreign raylib;

// Calculate linear interpolation between two vectors
Vector3Lerp :: (v1: Vector3, v2: Vector3, amount: float) -> Vector3 #foreign raylib;

// Calculate cubic hermite interpolation between two vectors and their tangents
// as described in the GLTF 2.0 specification: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#interpolation-cubic
Vector3CubicHermite :: (v1: Vector3, tangent1: Vector3, v2: Vector3, tangent2: Vector3, amount: float) -> Vector3 #foreign raylib;

// Calculate reflected vector to normal
Vector3Reflect :: (v: Vector3, normal: Vector3) -> Vector3 #foreign raylib;

// Get min value for each pair of components
Vector3Min :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

// Get max value for each pair of components
Vector3Max :: (v1: Vector3, v2: Vector3) -> Vector3 #foreign raylib;

// Compute barycenter coordinates (u, v, w) for point p with respect to triangle (a, b, c)
// NOTE: Assumes P is on the plane of the triangle
Vector3Barycenter :: (p: Vector3, a: Vector3, b: Vector3, c: Vector3) -> Vector3 #foreign raylib;

// Projects a Vector3 from screen space into object space
// NOTE: We are avoiding calling other raymath functions despite available
Vector3Unproject :: (source: Vector3, projection: Matrix, view: Matrix) -> Vector3 #foreign raylib;

// Get Vector3 as float array
Vector3ToFloatV :: (v: Vector3) -> float3 #foreign raylib;

// Invert the given vector
Vector3Invert :: (v: Vector3) -> Vector3 #foreign raylib;

// Clamp the components of the vector between
// min and max values specified by the given vectors
Vector3Clamp :: (v: Vector3, min: Vector3, max: Vector3) -> Vector3 #foreign raylib;

// Clamp the magnitude of the vector between two values
Vector3ClampValue :: (v: Vector3, min: float, max: float) -> Vector3 #foreign raylib;

// Check whether two given vectors are almost equal
Vector3Equals :: (p: Vector3, q: Vector3) -> s32 #foreign raylib;

// Compute the direction of a refracted ray
// v: normalized direction of the incoming ray
// n: normalized normal vector of the interface of two optical media
// r: ratio of the refractive index of the medium from where the ray comes
//    to the refractive index of the medium on the other side of the surface
Vector3Refract :: (v: Vector3, n: Vector3, r: float) -> Vector3 #foreign raylib;

//----------------------------------------------------------------------------------
// Module Functions Definition - Vector4 math
//----------------------------------------------------------------------------------
Vector4Zero :: () -> Vector4 #foreign raylib;

Vector4One :: () -> Vector4 #foreign raylib;

Vector4Add :: (v1: Vector4, v2: Vector4) -> Vector4 #foreign raylib;

Vector4AddValue :: (v: Vector4, add: float) -> Vector4 #foreign raylib;

Vector4Subtract :: (v1: Vector4, v2: Vector4) -> Vector4 #foreign raylib;

Vector4SubtractValue :: (v: Vector4, add: float) -> Vector4 #foreign raylib;

Vector4Length :: (v: Vector4) -> float #foreign raylib;

Vector4LengthSqr :: (v: Vector4) -> float #foreign raylib;

Vector4DotProduct :: (v1: Vector4, v2: Vector4) -> float #foreign raylib;

// Calculate distance between two vectors
Vector4Distance :: (v1: Vector4, v2: Vector4) -> float #foreign raylib;

// Calculate square distance between two vectors
Vector4DistanceSqr :: (v1: Vector4, v2: Vector4) -> float #foreign raylib;

Vector4Scale :: (v: Vector4, scale: float) -> Vector4 #foreign raylib;

// Multiply vector by vector
Vector4Multiply :: (v1: Vector4, v2: Vector4) -> Vector4 #foreign raylib;

// Negate vector
Vector4Negate :: (v: Vector4) -> Vector4 #foreign raylib;

// Divide vector by vector
Vector4Divide :: (v1: Vector4, v2: Vector4) -> Vector4 #foreign raylib;

// Normalize provided vector
Vector4Normalize :: (v: Vector4) -> Vector4 #foreign raylib;

// Get min value for each pair of components
Vector4Min :: (v1: Vector4, v2: Vector4) -> Vector4 #foreign raylib;

// Get max value for each pair of components
Vector4Max :: (v1: Vector4, v2: Vector4) -> Vector4 #foreign raylib;

// Calculate linear interpolation between two vectors
Vector4Lerp :: (v1: Vector4, v2: Vector4, amount: float) -> Vector4 #foreign raylib;

// Move Vector towards target
Vector4MoveTowards :: (v: Vector4, target: Vector4, maxDistance: float) -> Vector4 #foreign raylib;

// Invert the given vector
Vector4Invert :: (v: Vector4) -> Vector4 #foreign raylib;

// Check whether two given vectors are almost equal
Vector4Equals :: (p: Vector4, q: Vector4) -> s32 #foreign raylib;

// Compute matrix determinant
MatrixDeterminant :: (mat: Matrix) -> float #foreign raylib;

// Get the trace of the matrix (sum of the values along the diagonal)
MatrixTrace :: (mat: Matrix) -> float #foreign raylib;

// Transposes provided matrix
MatrixTranspose :: (mat: Matrix) -> Matrix #foreign raylib;

// Invert provided matrix
MatrixInvert :: (mat: Matrix) -> Matrix #foreign raylib;

// Get identity matrix
MatrixIdentity :: () -> Matrix #foreign raylib;

// Add two matrices
MatrixAdd :: (left: Matrix, right: Matrix) -> Matrix #foreign raylib;

// Subtract two matrices (left - right)
MatrixSubtract :: (left: Matrix, right: Matrix) -> Matrix #foreign raylib;

// Get two matrix multiplication
// NOTE: When multiplying matrices... the order matters!
MatrixMultiply :: (left: Matrix, right: Matrix) -> Matrix #foreign raylib;

// Get translation matrix
MatrixTranslate :: (x: float, y: float, z: float) -> Matrix #foreign raylib;

// Create rotation matrix from axis and angle
// NOTE: Angle should be provided in radians
MatrixRotate :: (axis: Vector3, angle: float) -> Matrix #foreign raylib;

// Get x-rotation matrix
// NOTE: Angle must be provided in radians
MatrixRotateX :: (angle: float) -> Matrix #foreign raylib;

// Get y-rotation matrix
// NOTE: Angle must be provided in radians
MatrixRotateY :: (angle: float) -> Matrix #foreign raylib;

// Get z-rotation matrix
// NOTE: Angle must be provided in radians
MatrixRotateZ :: (angle: float) -> Matrix #foreign raylib;

// Get xyz-rotation matrix
// NOTE: Angle must be provided in radians
MatrixRotateXYZ :: (angle: Vector3) -> Matrix #foreign raylib;

// Get zyx-rotation matrix
// NOTE: Angle must be provided in radians
MatrixRotateZYX :: (angle: Vector3) -> Matrix #foreign raylib;

// Get scaling matrix
MatrixScale :: (x: float, y: float, z: float) -> Matrix #foreign raylib;

// Get perspective projection matrix
MatrixFrustum :: (left: float64, right: float64, bottom: float64, top: float64, nearPlane: float64, farPlane: float64) -> Matrix #foreign raylib;

// Get perspective projection matrix
// NOTE: Fovy angle must be provided in radians
MatrixPerspective :: (fovY: float64, aspect: float64, nearPlane: float64, farPlane: float64) -> Matrix #foreign raylib;

// Get orthographic projection matrix
MatrixOrtho :: (left: float64, right: float64, bottom: float64, top: float64, nearPlane: float64, farPlane: float64) -> Matrix #foreign raylib;

// Get camera look-at matrix (view matrix)
MatrixLookAt :: (eye: Vector3, target: Vector3, up: Vector3) -> Matrix #foreign raylib;

// Get float array of matrix data
MatrixToFloatV :: (mat: Matrix) -> float16 #foreign raylib;

// Add two quaternions
QuaternionAdd :: (q1: Quaternion, q2: Quaternion) -> Quaternion #foreign raylib;

// Add quaternion and float value
QuaternionAddValue :: (q: Quaternion, add: float) -> Quaternion #foreign raylib;

// Subtract two quaternions
QuaternionSubtract :: (q1: Quaternion, q2: Quaternion) -> Quaternion #foreign raylib;

// Subtract quaternion and float value
QuaternionSubtractValue :: (q: Quaternion, sub: float) -> Quaternion #foreign raylib;

// Get identity quaternion
QuaternionIdentity :: () -> Quaternion #foreign raylib;

// Computes the length of a quaternion
QuaternionLength :: (q: Quaternion) -> float #foreign raylib;

// Normalize provided quaternion
QuaternionNormalize :: (q: Quaternion) -> Quaternion #foreign raylib;

// Invert provided quaternion
QuaternionInvert :: (q: Quaternion) -> Quaternion #foreign raylib;

// Calculate two quaternion multiplication
QuaternionMultiply :: (q1: Quaternion, q2: Quaternion) -> Quaternion #foreign raylib;

// Scale quaternion by float value
QuaternionScale :: (q: Quaternion, mul: float) -> Quaternion #foreign raylib;

// Divide two quaternions
QuaternionDivide :: (q1: Quaternion, q2: Quaternion) -> Quaternion #foreign raylib;

// Calculate linear interpolation between two quaternions
QuaternionLerp :: (q1: Quaternion, q2: Quaternion, amount: float) -> Quaternion #foreign raylib;

// Calculate slerp-optimized interpolation between two quaternions
QuaternionNlerp :: (q1: Quaternion, q2: Quaternion, amount: float) -> Quaternion #foreign raylib;

// Calculates spherical linear interpolation between two quaternions
QuaternionSlerp :: (q1: Quaternion, q2: Quaternion, amount: float) -> Quaternion #foreign raylib;

// Calculate quaternion cubic spline interpolation using Cubic Hermite Spline algorithm
// as described in the GLTF 2.0 specification: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#interpolation-cubic
QuaternionCubicHermiteSpline :: (q1: Quaternion, outTangent1: Quaternion, q2: Quaternion, inTangent2: Quaternion, t: float) -> Quaternion #foreign raylib;

// Calculate quaternion based on the rotation from one vector to another
QuaternionFromVector3ToVector3 :: (from: Vector3, to: Vector3) -> Quaternion #foreign raylib;

// Get a quaternion for a given rotation matrix
QuaternionFromMatrix :: (mat: Matrix) -> Quaternion #foreign raylib;

// Get a matrix for a given quaternion
QuaternionToMatrix :: (q: Quaternion) -> Matrix #foreign raylib;

// Get rotation quaternion for an angle and axis
// NOTE: Angle must be provided in radians
QuaternionFromAxisAngle :: (axis: Vector3, angle: float) -> Quaternion #foreign raylib;

// Get the rotation angle and axis for a given quaternion
QuaternionToAxisAngle :: (q: Quaternion, outAxis: *Vector3, outAngle: *float) -> void #foreign raylib;

// Get the quaternion equivalent to Euler angles
// NOTE: Rotation order is ZYX
QuaternionFromEuler :: (pitch: float, yaw: float, roll: float) -> Quaternion #foreign raylib;

// Get the Euler angles equivalent to quaternion (roll, pitch, yaw)
// NOTE: Angles are returned in a Vector3 struct in radians
QuaternionToEuler :: (q: Quaternion) -> Vector3 #foreign raylib;

// Transform a quaternion given a transformation matrix
QuaternionTransform :: (q: Quaternion, mat: Matrix) -> Quaternion #foreign raylib;

// Check whether two given quaternions are almost equal
QuaternionEquals :: (p: Quaternion, q: Quaternion) -> s32 #foreign raylib;

// Dynamic vertex buffers (position + texcoords + colors + indices arrays)
VertexBuffer :: struct {
    elementCount: s32; // Number of elements in the buffer (QUADS)

    vertices:     *float; // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    texcoords:    *float; // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    normals:      *float; // Vertex normal (XYZ - 3 components per vertex) (shader-location = 2)
    colors:       *u8; // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)

    indices:      *u32; // Vertex indices (in case vertex data comes indexed) (6 indices per quad)

    vaoId:        u32; // OpenGL Vertex Array Object id
    vboId:        [5] u32; // OpenGL Vertex Buffer Objects id (5 types of vertex data)
}

// Draw call type
// NOTE: Only texture changes register a new draw, other state-change-related elements are not
// used at this moment (vaoId, shaderId, matrices), raylib just forces a batch draw call if any
// of those state-change happens (this is done in core module)
DrawCall :: struct {
    mode:            s32; // Drawing mode: LINES, TRIANGLES, QUADS
    vertexCount:     s32; // Number of vertex of the draw
    vertexAlignment: s32; // Number of vertex required for index alignment (LINES, TRIANGLES)

    textureId:       u32; // Texture id to be used on the draw -> Use to create new draw call if changes
}

// rlRenderBatch type
RenderBatch :: struct {
    bufferCount:   s32; // Number of vertex buffers (multi-buffering support)
    currentBuffer: s32; // Current buffer tracking in case of multi-buffering
    vertexBuffer:  *VertexBuffer; // Dynamic buffer(s) for vertex data

    draws:         *DrawCall; // Draw calls array, depends on textureId
    drawCounter:   s32; // Draw calls counter
    currentDepth:  float; // Current depth value for next draw
}

// OpenGL version
GlVersion :: enum s32 {
    RL_OPENGL_11    :: 1;
    RL_OPENGL_21    :: 2;
    RL_OPENGL_33    :: 3;
    RL_OPENGL_43    :: 4;
    RL_OPENGL_ES_20 :: 5;
    RL_OPENGL_ES_30 :: 6;
}

// Trace log level
// NOTE: Organized by priority level
TraceLogLevel :: enum s32 {
    RL_LOG_ALL     :: 0;
    RL_LOG_TRACE   :: 1;
    RL_LOG_DEBUG   :: 2;
    RL_LOG_INFO    :: 3;
    RL_LOG_WARNING :: 4;
    RL_LOG_ERROR   :: 5;
    RL_LOG_FATAL   :: 6;
    RL_LOG_NONE    :: 7;
}

// Texture pixel formats
// NOTE: Support depends on OpenGL version
PixelFormat :: enum s32 {
    RL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE    :: 1;
    RL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA   :: 2;
    RL_PIXELFORMAT_UNCOMPRESSED_R5G6B5       :: 3;
    RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8       :: 4;
    RL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1     :: 5;
    RL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4     :: 6;
    RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8     :: 7;
    RL_PIXELFORMAT_UNCOMPRESSED_R32          :: 8;
    RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32    :: 9;
    RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32 :: 10;
    RL_PIXELFORMAT_UNCOMPRESSED_R16          :: 11;
    RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16    :: 12;
    RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16 :: 13;
    RL_PIXELFORMAT_COMPRESSED_DXT1_RGB       :: 14;
    RL_PIXELFORMAT_COMPRESSED_DXT1_RGBA      :: 15;
    RL_PIXELFORMAT_COMPRESSED_DXT3_RGBA      :: 16;
    RL_PIXELFORMAT_COMPRESSED_DXT5_RGBA      :: 17;
    RL_PIXELFORMAT_COMPRESSED_ETC1_RGB       :: 18;
    RL_PIXELFORMAT_COMPRESSED_ETC2_RGB       :: 19;
    RL_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA  :: 20;
    RL_PIXELFORMAT_COMPRESSED_PVRT_RGB       :: 21;
    RL_PIXELFORMAT_COMPRESSED_PVRT_RGBA      :: 22;
    RL_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA  :: 23;
    RL_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA  :: 24;
}

// Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
TextureFilter :: enum s32 {
    RL_TEXTURE_FILTER_POINT           :: 0;
    RL_TEXTURE_FILTER_BILINEAR        :: 1;
    RL_TEXTURE_FILTER_TRILINEAR       :: 2;
    RL_TEXTURE_FILTER_ANISOTROPIC_4X  :: 3;
    RL_TEXTURE_FILTER_ANISOTROPIC_8X  :: 4;
    RL_TEXTURE_FILTER_ANISOTROPIC_16X :: 5;
}

// Color blending modes (pre-defined)
BlendMode :: enum s32 {
    RL_BLEND_ALPHA             :: 0;
    RL_BLEND_ADDITIVE          :: 1;
    RL_BLEND_MULTIPLIED        :: 2;
    RL_BLEND_ADD_COLORS        :: 3;
    RL_BLEND_SUBTRACT_COLORS   :: 4;
    RL_BLEND_ALPHA_PREMULTIPLY :: 5;
    RL_BLEND_CUSTOM            :: 6;
    RL_BLEND_CUSTOM_SEPARATE   :: 7;
}

// Shader location point type
ShaderLocationIndex :: enum s32 {
    RL_SHADER_LOC_VERTEX_POSITION   :: 0;
    RL_SHADER_LOC_VERTEX_TEXCOORD01 :: 1;
    RL_SHADER_LOC_VERTEX_TEXCOORD02 :: 2;
    RL_SHADER_LOC_VERTEX_NORMAL     :: 3;
    RL_SHADER_LOC_VERTEX_TANGENT    :: 4;
    RL_SHADER_LOC_VERTEX_COLOR      :: 5;
    RL_SHADER_LOC_MATRIX_MVP        :: 6;
    RL_SHADER_LOC_MATRIX_VIEW       :: 7;
    RL_SHADER_LOC_MATRIX_PROJECTION :: 8;
    RL_SHADER_LOC_MATRIX_MODEL      :: 9;
    RL_SHADER_LOC_MATRIX_NORMAL     :: 10;
    RL_SHADER_LOC_VECTOR_VIEW       :: 11;
    RL_SHADER_LOC_COLOR_DIFFUSE     :: 12;
    RL_SHADER_LOC_COLOR_SPECULAR    :: 13;
    RL_SHADER_LOC_COLOR_AMBIENT     :: 14;
    RL_SHADER_LOC_MAP_ALBEDO        :: 15;
    RL_SHADER_LOC_MAP_METALNESS     :: 16;
    RL_SHADER_LOC_MAP_NORMAL        :: 17;
    RL_SHADER_LOC_MAP_ROUGHNESS     :: 18;
    RL_SHADER_LOC_MAP_OCCLUSION     :: 19;
    RL_SHADER_LOC_MAP_EMISSION      :: 20;
    RL_SHADER_LOC_MAP_HEIGHT        :: 21;
    RL_SHADER_LOC_MAP_CUBEMAP       :: 22;
    RL_SHADER_LOC_MAP_IRRADIANCE    :: 23;
    RL_SHADER_LOC_MAP_PREFILTER     :: 24;
    RL_SHADER_LOC_MAP_BRDF          :: 25;
}

// Shader uniform data type
ShaderUniformDataType :: enum s32 {
    RL_SHADER_UNIFORM_FLOAT     :: 0;
    RL_SHADER_UNIFORM_VEC2      :: 1;
    RL_SHADER_UNIFORM_VEC3      :: 2;
    RL_SHADER_UNIFORM_VEC4      :: 3;
    RL_SHADER_UNIFORM_INT       :: 4;
    RL_SHADER_UNIFORM_IVEC2     :: 5;
    RL_SHADER_UNIFORM_IVEC3     :: 6;
    RL_SHADER_UNIFORM_IVEC4     :: 7;
    RL_SHADER_UNIFORM_SAMPLER2D :: 8;
}

// Shader attribute data types
ShaderAttributeDataType :: enum s32 {
    RL_SHADER_ATTRIB_FLOAT :: 0;
    RL_SHADER_ATTRIB_VEC2  :: 1;
    RL_SHADER_ATTRIB_VEC3  :: 2;
    RL_SHADER_ATTRIB_VEC4  :: 3;
}

// Framebuffer attachment type
// NOTE: By default up to 8 color channels defined, but it can be more
FramebufferAttachType :: enum s32 {
    RL_ATTACHMENT_COLOR_CHANNEL0 :: 0;
    RL_ATTACHMENT_COLOR_CHANNEL1 :: 1;
    RL_ATTACHMENT_COLOR_CHANNEL2 :: 2;
    RL_ATTACHMENT_COLOR_CHANNEL3 :: 3;
    RL_ATTACHMENT_COLOR_CHANNEL4 :: 4;
    RL_ATTACHMENT_COLOR_CHANNEL5 :: 5;
    RL_ATTACHMENT_COLOR_CHANNEL6 :: 6;
    RL_ATTACHMENT_COLOR_CHANNEL7 :: 7;
    RL_ATTACHMENT_DEPTH          :: 100;
    RL_ATTACHMENT_STENCIL        :: 200;
}

// Framebuffer texture attachment type
FramebufferAttachTextureType :: enum s32 {
    RL_ATTACHMENT_CUBEMAP_POSITIVE_X :: 0;
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_X :: 1;
    RL_ATTACHMENT_CUBEMAP_POSITIVE_Y :: 2;
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_Y :: 3;
    RL_ATTACHMENT_CUBEMAP_POSITIVE_Z :: 4;
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_Z :: 5;
    RL_ATTACHMENT_TEXTURE2D          :: 100;
    RL_ATTACHMENT_RENDERBUFFER       :: 200;
}

// Face culling mode
CullMode :: enum s32 {
    RL_CULL_FACE_FRONT :: 0;
    RL_CULL_FACE_BACK  :: 1;
}

MatrixMode :: (mode: s32) -> void #foreign raylib "rlMatrixMode";
PushMatrix :: () -> void #foreign raylib "rlPushMatrix";
PopMatrix :: () -> void #foreign raylib "rlPopMatrix";
LoadIdentity :: () -> void #foreign raylib "rlLoadIdentity";
Translatef :: (x: float, y: float, z: float) -> void #foreign raylib "rlTranslatef";
Rotatef :: (angle: float, x: float, y: float, z: float) -> void #foreign raylib "rlRotatef";
Scalef :: (x: float, y: float, z: float) -> void #foreign raylib "rlScalef";
MultMatrixf :: (matf: *float) -> void #foreign raylib "rlMultMatrixf";
Frustum :: (left: float64, right: float64, bottom: float64, top: float64, znear: float64, zfar: float64) -> void #foreign raylib "rlFrustum";
Ortho :: (left: float64, right: float64, bottom: float64, top: float64, znear: float64, zfar: float64) -> void #foreign raylib "rlOrtho";
Viewport :: (x: s32, y: s32, width: s32, height: s32) -> void #foreign raylib "rlViewport";
SetClipPlanes :: (nearPlane: float64, farPlane: float64) -> void #foreign raylib "rlSetClipPlanes";
GetCullDistanceNear :: () -> float64 #foreign raylib "rlGetCullDistanceNear";
GetCullDistanceFar :: () -> float64 #foreign raylib "rlGetCullDistanceFar";

//------------------------------------------------------------------------------------
// Functions Declaration - Vertex level operations
//------------------------------------------------------------------------------------
Begin :: (mode: s32) -> void #foreign raylib "rlBegin";
End :: () -> void #foreign raylib "rlEnd";
Vertex2i :: (x: s32, y: s32) -> void #foreign raylib "rlVertex2i";
Vertex2f :: (x: float, y: float) -> void #foreign raylib "rlVertex2f";
Vertex3f :: (x: float, y: float, z: float) -> void #foreign raylib "rlVertex3f";
TexCoord2f :: (x: float, y: float) -> void #foreign raylib "rlTexCoord2f";
Normal3f :: (x: float, y: float, z: float) -> void #foreign raylib "rlNormal3f";
Color4ub :: (r: u8, g: u8, b: u8, a: u8) -> void #foreign raylib "rlColor4ub";
Color3f :: (x: float, y: float, z: float) -> void #foreign raylib "rlColor3f";
Color4f :: (x: float, y: float, z: float, w: float) -> void #foreign raylib "rlColor4f";

// Vertex buffers state
EnableVertexArray :: (vaoId: u32) -> bool #foreign raylib "rlEnableVertexArray";
DisableVertexArray :: () -> void #foreign raylib "rlDisableVertexArray";
EnableVertexBuffer :: (id: u32) -> void #foreign raylib "rlEnableVertexBuffer";
DisableVertexBuffer :: () -> void #foreign raylib "rlDisableVertexBuffer";
EnableVertexBufferElement :: (id: u32) -> void #foreign raylib "rlEnableVertexBufferElement";
DisableVertexBufferElement :: () -> void #foreign raylib "rlDisableVertexBufferElement";
EnableVertexAttribute :: (index: u32) -> void #foreign raylib "rlEnableVertexAttribute";
DisableVertexAttribute :: (index: u32) -> void #foreign raylib "rlDisableVertexAttribute";

// Textures state
ActiveTextureSlot :: (slot: s32) -> void #foreign raylib "rlActiveTextureSlot";
EnableTexture :: (id: u32) -> void #foreign raylib "rlEnableTexture";
DisableTexture :: () -> void #foreign raylib "rlDisableTexture";
EnableTextureCubemap :: (id: u32) -> void #foreign raylib "rlEnableTextureCubemap";
DisableTextureCubemap :: () -> void #foreign raylib "rlDisableTextureCubemap";
TextureParameters :: (id: u32, param: s32, value: s32) -> void #foreign raylib "rlTextureParameters";
CubemapParameters :: (id: u32, param: s32, value: s32) -> void #foreign raylib "rlCubemapParameters";

// Shader state
EnableShader :: (id: u32) -> void #foreign raylib "rlEnableShader";
DisableShader :: () -> void #foreign raylib "rlDisableShader";

// Framebuffer state
EnableFramebuffer :: (id: u32) -> void #foreign raylib "rlEnableFramebuffer";
DisableFramebuffer :: () -> void #foreign raylib "rlDisableFramebuffer";
GetActiveFramebuffer :: () -> u32 #foreign raylib "rlGetActiveFramebuffer";
ActiveDrawBuffers :: (count: s32) -> void #foreign raylib "rlActiveDrawBuffers";
BlitFramebuffer :: (srcX: s32, srcY: s32, srcWidth: s32, srcHeight: s32, dstX: s32, dstY: s32, dstWidth: s32, dstHeight: s32, bufferMask: s32) -> void #foreign raylib "rlBlitFramebuffer";
BindFramebuffer :: (target: u32, framebuffer: u32) -> void #foreign raylib "rlBindFramebuffer";

// General render state
EnableColorBlend :: () -> void #foreign raylib "rlEnableColorBlend";
DisableColorBlend :: () -> void #foreign raylib "rlDisableColorBlend";
EnableDepthTest :: () -> void #foreign raylib "rlEnableDepthTest";
DisableDepthTest :: () -> void #foreign raylib "rlDisableDepthTest";
EnableDepthMask :: () -> void #foreign raylib "rlEnableDepthMask";
DisableDepthMask :: () -> void #foreign raylib "rlDisableDepthMask";
EnableBackfaceCulling :: () -> void #foreign raylib "rlEnableBackfaceCulling";
DisableBackfaceCulling :: () -> void #foreign raylib "rlDisableBackfaceCulling";
ColorMask :: (r: bool, g: bool, b: bool, a: bool) -> void #foreign raylib "rlColorMask";
SetCullFace :: (mode: s32) -> void #foreign raylib "rlSetCullFace";
EnableScissorTest :: () -> void #foreign raylib "rlEnableScissorTest";
DisableScissorTest :: () -> void #foreign raylib "rlDisableScissorTest";
Scissor :: (x: s32, y: s32, width: s32, height: s32) -> void #foreign raylib "rlScissor";
EnableWireMode :: () -> void #foreign raylib "rlEnableWireMode";
EnablePointMode :: () -> void #foreign raylib "rlEnablePointMode";
DisableWireMode :: () -> void #foreign raylib "rlDisableWireMode";
SetLineWidth :: (width: float) -> void #foreign raylib "rlSetLineWidth";
GetLineWidth :: () -> float #foreign raylib "rlGetLineWidth";
EnableSmoothLines :: () -> void #foreign raylib "rlEnableSmoothLines";
DisableSmoothLines :: () -> void #foreign raylib "rlDisableSmoothLines";
EnableStereoRender :: () -> void #foreign raylib "rlEnableStereoRender";
DisableStereoRender :: () -> void #foreign raylib "rlDisableStereoRender";
IsStereoRenderEnabled :: () -> bool #foreign raylib "rlIsStereoRenderEnabled";

ClearColor :: (r: u8, g: u8, b: u8, a: u8) -> void #foreign raylib "rlClearColor";
ClearScreenBuffers :: () -> void #foreign raylib "rlClearScreenBuffers";
CheckErrors :: () -> void #foreign raylib "rlCheckErrors";
SetBlendMode :: (mode: s32) -> void #foreign raylib "rlSetBlendMode";
SetBlendFactors :: (glSrcFactor: s32, glDstFactor: s32, glEquation: s32) -> void #foreign raylib "rlSetBlendFactors";
SetBlendFactorsSeparate :: (glSrcRGB: s32, glDstRGB: s32, glSrcAlpha: s32, glDstAlpha: s32, glEqRGB: s32, glEqAlpha: s32) -> void #foreign raylib "rlSetBlendFactorsSeparate";

//------------------------------------------------------------------------------------
// Functions Declaration - rlgl functionality
//------------------------------------------------------------------------------------
// rlgl initialization functions
glInit :: (width: s32, height: s32) -> void #foreign raylib "rlglInit";
glClose :: () -> void #foreign raylib "rlglClose";
LoadExtensions :: (loader: *void) -> void #foreign raylib "rlLoadExtensions";
GetVersion :: () -> s32 #foreign raylib "rlGetVersion";
SetFramebufferWidth :: (width: s32) -> void #foreign raylib "rlSetFramebufferWidth";
GetFramebufferWidth :: () -> s32 #foreign raylib "rlGetFramebufferWidth";
SetFramebufferHeight :: (height: s32) -> void #foreign raylib "rlSetFramebufferHeight";
GetFramebufferHeight :: () -> s32 #foreign raylib "rlGetFramebufferHeight";

GetTextureIdDefault :: () -> u32 #foreign raylib "rlGetTextureIdDefault";
GetShaderIdDefault :: () -> u32 #foreign raylib "rlGetShaderIdDefault";
GetShaderLocsDefault :: () -> *s32 #foreign raylib "rlGetShaderLocsDefault";

// Render batch management
// NOTE: rlgl provides a default render batch to behave like OpenGL 1.1 immediate mode
// but this render batch API is exposed in case of custom batches are required
LoadRenderBatch :: (numBuffers: s32, bufferElements: s32) -> RenderBatch #foreign raylib "rlLoadRenderBatch";
UnloadRenderBatch :: (batch: RenderBatch) -> void #foreign raylib "rlUnloadRenderBatch";
DrawRenderBatch :: (batch: *RenderBatch) -> void #foreign raylib "rlDrawRenderBatch";
SetRenderBatchActive :: (batch: *RenderBatch) -> void #foreign raylib "rlSetRenderBatchActive";
DrawRenderBatchActive :: () -> void #foreign raylib "rlDrawRenderBatchActive";
CheckRenderBatchLimit :: (vCount: s32) -> bool #foreign raylib "rlCheckRenderBatchLimit";

SetTexture :: (id: u32) -> void #foreign raylib "rlSetTexture";

// Vertex buffers management
LoadVertexArray :: () -> u32 #foreign raylib "rlLoadVertexArray";
LoadVertexBuffer :: (buffer: *void, size: s32, dynamic: bool) -> u32 #foreign raylib "rlLoadVertexBuffer";
LoadVertexBufferElement :: (buffer: *void, size: s32, dynamic: bool) -> u32 #foreign raylib "rlLoadVertexBufferElement";
UpdateVertexBuffer :: (bufferId: u32, data: *void, dataSize: s32, offset: s32) -> void #foreign raylib "rlUpdateVertexBuffer";
UpdateVertexBufferElements :: (id: u32, data: *void, dataSize: s32, offset: s32) -> void #foreign raylib "rlUpdateVertexBufferElements";
UnloadVertexArray :: (vaoId: u32) -> void #foreign raylib "rlUnloadVertexArray";
UnloadVertexBuffer :: (vboId: u32) -> void #foreign raylib "rlUnloadVertexBuffer";
SetVertexAttribute :: (index: u32, compSize: s32, type: s32, normalized: bool, stride: s32, offset: s32) -> void #foreign raylib "rlSetVertexAttribute";
SetVertexAttributeDivisor :: (index: u32, divisor: s32) -> void #foreign raylib "rlSetVertexAttributeDivisor";
SetVertexAttributeDefault :: (locIndex: s32, value: *void, attribType: s32, count: s32) -> void #foreign raylib "rlSetVertexAttributeDefault";
DrawVertexArray :: (offset: s32, count: s32) -> void #foreign raylib "rlDrawVertexArray";
DrawVertexArrayElements :: (offset: s32, count: s32, buffer: *void) -> void #foreign raylib "rlDrawVertexArrayElements";
DrawVertexArrayInstanced :: (offset: s32, count: s32, instances: s32) -> void #foreign raylib "rlDrawVertexArrayInstanced";
DrawVertexArrayElementsInstanced :: (offset: s32, count: s32, buffer: *void, instances: s32) -> void #foreign raylib "rlDrawVertexArrayElementsInstanced";

// Textures management
LoadTexture :: (data: *void, width: s32, height: s32, format: s32, mipmapCount: s32) -> u32 #foreign raylib "rlLoadTexture";
LoadTextureDepth :: (width: s32, height: s32, useRenderBuffer: bool) -> u32 #foreign raylib "rlLoadTextureDepth";
LoadTextureCubemap :: (data: *void, size: s32, format: s32) -> u32 #foreign raylib "rlLoadTextureCubemap";
UpdateTexture :: (id: u32, offsetX: s32, offsetY: s32, width: s32, height: s32, format: s32, data: *void) -> void #foreign raylib "rlUpdateTexture";
GetGlTextureFormats :: (format: s32, glInternalFormat: *u32, glFormat: *u32, glType: *u32) -> void #foreign raylib "rlGetGlTextureFormats";
GetPixelFormatName :: (format: u32) -> *u8 #foreign raylib "rlGetPixelFormatName";
UnloadTexture :: (id: u32) -> void #foreign raylib "rlUnloadTexture";
GenTextureMipmaps :: (id: u32, width: s32, height: s32, format: s32, mipmaps: *s32) -> void #foreign raylib "rlGenTextureMipmaps";
ReadTexturePixels :: (id: u32, width: s32, height: s32, format: s32) -> *void #foreign raylib "rlReadTexturePixels";
ReadScreenPixels :: (width: s32, height: s32) -> *u8 #foreign raylib "rlReadScreenPixels";

// Framebuffer management (fbo)
LoadFramebuffer :: () -> u32 #foreign raylib "rlLoadFramebuffer";
FramebufferAttach :: (fboId: u32, texId: u32, attachType: s32, texType: s32, mipLevel: s32) -> void #foreign raylib "rlFramebufferAttach";
FramebufferComplete :: (id: u32) -> bool #foreign raylib "rlFramebufferComplete";
UnloadFramebuffer :: (id: u32) -> void #foreign raylib "rlUnloadFramebuffer";

// Shaders management
LoadShaderCode :: (vsCode: *u8, fsCode: *u8) -> u32 #foreign raylib "rlLoadShaderCode";
CompileShader :: (shaderCode: *u8, type: s32) -> u32 #foreign raylib "rlCompileShader";
LoadShaderProgram :: (vShaderId: u32, fShaderId: u32) -> u32 #foreign raylib "rlLoadShaderProgram";
UnloadShaderProgram :: (id: u32) -> void #foreign raylib "rlUnloadShaderProgram";
GetLocationUniform :: (shaderId: u32, uniformName: *u8) -> s32 #foreign raylib "rlGetLocationUniform";
GetLocationAttrib :: (shaderId: u32, attribName: *u8) -> s32 #foreign raylib "rlGetLocationAttrib";
SetUniform :: (locIndex: s32, value: *void, uniformType: s32, count: s32) -> void #foreign raylib "rlSetUniform";
SetUniformMatrix :: (locIndex: s32, mat: Matrix) -> void #foreign raylib "rlSetUniformMatrix";
SetUniformSampler :: (locIndex: s32, textureId: u32) -> void #foreign raylib "rlSetUniformSampler";
SetShader :: (id: u32, locs: *s32) -> void #foreign raylib "rlSetShader";

// Compute shader management
LoadComputeShaderProgram :: (shaderId: u32) -> u32 #foreign raylib "rlLoadComputeShaderProgram";
ComputeShaderDispatch :: (groupX: u32, groupY: u32, groupZ: u32) -> void #foreign raylib "rlComputeShaderDispatch";

// Shader buffer storage object management (ssbo)
LoadShaderBuffer :: (size: u32, data: *void, usageHint: s32) -> u32 #foreign raylib "rlLoadShaderBuffer";
UnloadShaderBuffer :: (ssboId: u32) -> void #foreign raylib "rlUnloadShaderBuffer";
UpdateShaderBuffer :: (id: u32, data: *void, dataSize: u32, offset: u32) -> void #foreign raylib "rlUpdateShaderBuffer";
BindShaderBuffer :: (id: u32, index: u32) -> void #foreign raylib "rlBindShaderBuffer";
ReadShaderBuffer :: (id: u32, dest: *void, count: u32, offset: u32) -> void #foreign raylib "rlReadShaderBuffer";
CopyShaderBuffer :: (destId: u32, srcId: u32, destOffset: u32, srcOffset: u32, count: u32) -> void #foreign raylib "rlCopyShaderBuffer";
GetShaderBufferSize :: (id: u32) -> u32 #foreign raylib "rlGetShaderBufferSize";

// Buffer management
BindImageTexture :: (id: u32, index: u32, format: s32, readonly: bool) -> void #foreign raylib "rlBindImageTexture";

// Matrix state management
GetMatrixModelview :: () -> Matrix #foreign raylib "rlGetMatrixModelview";
GetMatrixProjection :: () -> Matrix #foreign raylib "rlGetMatrixProjection";
GetMatrixTransform :: () -> Matrix #foreign raylib "rlGetMatrixTransform";
GetMatrixProjectionStereo :: (eye: s32) -> Matrix #foreign raylib "rlGetMatrixProjectionStereo";
GetMatrixViewOffsetStereo :: (eye: s32) -> Matrix #foreign raylib "rlGetMatrixViewOffsetStereo";
SetMatrixProjection :: (proj: Matrix) -> void #foreign raylib "rlSetMatrixProjection";
SetMatrixModelview :: (view: Matrix) -> void #foreign raylib "rlSetMatrixModelview";
SetMatrixProjectionStereo :: (right: Matrix, left: Matrix) -> void #foreign raylib "rlSetMatrixProjectionStereo";
SetMatrixViewOffsetStereo :: (right: Matrix, left: Matrix) -> void #foreign raylib "rlSetMatrixViewOffsetStereo";

// Quick and dirty cube/quad buffers load->draw->unload
LoadDrawCube :: () -> void #foreign raylib "rlLoadDrawCube";
LoadDrawQuad :: () -> void #foreign raylib "rlLoadDrawQuad";

#scope_file

#import "Basic"; // For push_context

raylib :: #library "windows/raylib";
