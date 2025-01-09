//
// This file was auto-generated using the following command:
//
// jai generate.jai - -compile
//



SUPPORT_MODULE_RSHAPES :: 1;
SUPPORT_MODULE_RTEXTURES :: 1;
SUPPORT_MODULE_RTEXT :: 1;
SUPPORT_MODULE_RMODELS :: 1;
SUPPORT_MODULE_RAUDIO :: 1;
PLATFORM_DESKTOP_GLFW :: 1;
GRAPHICS_API_OPENGL_43 :: 1;
RAYLIB_VERSION_MAJOR :: 5;
RAYLIB_VERSION_MINOR :: 5;
RAYLIB_VERSION_PATCH :: 0;
RAYLIB_VERSION :: "5.5";

DEG2RAD :: PI/180.0;

RAD2DEG :: 180.0/PI;

GetMouseRay :: GetScreenToWorldRay;

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

RL_DEFAULT_SHADER_ATTRIB_LOCATION_INDICES :: 6;

EPSILON :: 0.000001;

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
    boneIds:       *u8; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning) (shader-location = 6)
    boneWeights:   *float; // Vertex bone weight, up to 4 bones influence by vertex (skinning) (shader-location = 7)
    boneMatrices:  *Matrix; // Bones animated transformation matrices
    boneCount:     s32; // Number of bones

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
    direction: Vector3; // Ray direction (normalized)
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
ConfigFlags :: enum_flags u32 {
    VSYNC_HINT               :: 0x40;
    FULLSCREEN_MODE          :: 0x2;
    WINDOW_RESIZABLE         :: 0x4;
    WINDOW_UNDECORATED       :: 0x8;
    WINDOW_HIDDEN            :: 0x80;
    WINDOW_MINIMIZED         :: 0x200;
    WINDOW_MAXIMIZED         :: 0x400;
    WINDOW_UNFOCUSED         :: 0x800;
    WINDOW_TOPMOST           :: 0x1000;
    WINDOW_ALWAYS_RUN        :: 0x100;
    WINDOW_TRANSPARENT       :: 0x10;
    WINDOW_HIGHDPI           :: 0x2000;
    WINDOW_MOUSE_PASSTHROUGH :: 0x4000;
    BORDERLESS_WINDOWED_MODE :: 0x8000;
    MSAA_4X_HINT             :: 0x20;
    INTERLACED_HINT          :: 0x10000;

    FLAG_VSYNC_HINT               :: VSYNC_HINT;
    FLAG_FULLSCREEN_MODE          :: FULLSCREEN_MODE;
    FLAG_WINDOW_RESIZABLE         :: WINDOW_RESIZABLE;
    FLAG_WINDOW_UNDECORATED       :: WINDOW_UNDECORATED;
    FLAG_WINDOW_HIDDEN            :: WINDOW_HIDDEN;
    FLAG_WINDOW_MINIMIZED         :: WINDOW_MINIMIZED;
    FLAG_WINDOW_MAXIMIZED         :: WINDOW_MAXIMIZED;
    FLAG_WINDOW_UNFOCUSED         :: WINDOW_UNFOCUSED;
    FLAG_WINDOW_TOPMOST           :: WINDOW_TOPMOST;
    FLAG_WINDOW_ALWAYS_RUN        :: WINDOW_ALWAYS_RUN;
    FLAG_WINDOW_TRANSPARENT       :: WINDOW_TRANSPARENT;
    FLAG_WINDOW_HIGHDPI           :: WINDOW_HIGHDPI;
    FLAG_WINDOW_MOUSE_PASSTHROUGH :: WINDOW_MOUSE_PASSTHROUGH;
    FLAG_BORDERLESS_WINDOWED_MODE :: BORDERLESS_WINDOWED_MODE;
    FLAG_MSAA_4X_HINT             :: MSAA_4X_HINT;
    FLAG_INTERLACED_HINT          :: INTERLACED_HINT;
}

// Trace log level
// NOTE: Organized by priority level
TraceLogLevel :: enum s32 {
    ALL     :: 0;
    TRACE   :: 1;
    DEBUG   :: 2;
    INFO    :: 3;
    WARNING :: 4;
    ERROR   :: 5;
    FATAL   :: 6;
    NONE    :: 7;

    LOG_ALL     :: ALL;
    LOG_TRACE   :: TRACE;
    LOG_DEBUG   :: DEBUG;
    LOG_INFO    :: INFO;
    LOG_WARNING :: WARNING;
    LOG_ERROR   :: ERROR;
    LOG_FATAL   :: FATAL;
    LOG_NONE    :: NONE;
}

// Keyboard keys (US keyboard layout)
// NOTE: Use GetKeyPressed() to allow redefining
// required keys for alternative layouts
KeyboardKey :: enum s32 {
    NULL          :: 0;

    APOSTROPHE    :: 39;
    COMMA         :: 44;
    MINUS         :: 45;
    PERIOD        :: 46;
    SLASH         :: 47;
    ZERO          :: 48;
    ONE           :: 49;
    TWO           :: 50;
    THREE         :: 51;
    FOUR          :: 52;
    FIVE          :: 53;
    SIX           :: 54;
    SEVEN         :: 55;
    EIGHT         :: 56;
    NINE          :: 57;
    SEMICOLON     :: 59;
    EQUAL         :: 61;
    A             :: 65;
    B             :: 66;
    C             :: 67;
    D             :: 68;
    E             :: 69;
    F             :: 70;
    G             :: 71;
    H             :: 72;
    I             :: 73;
    J             :: 74;
    K             :: 75;
    L             :: 76;
    M             :: 77;
    N             :: 78;
    O             :: 79;
    P             :: 80;
    Q             :: 81;
    R             :: 82;
    S             :: 83;
    T             :: 84;
    U             :: 85;
    V             :: 86;
    W             :: 87;
    X             :: 88;
    Y             :: 89;
    Z             :: 90;
    LEFT_BRACKET  :: 91;
    BACKSLASH     :: 92;
    RIGHT_BRACKET :: 93;
    GRAVE         :: 96;

    SPACE         :: 32;
    ESCAPE        :: 256;
    ENTER         :: 257;
    TAB           :: 258;
    BACKSPACE     :: 259;
    INSERT        :: 260;
    DELETE        :: 261;
    RIGHT         :: 262;
    LEFT          :: 263;
    DOWN          :: 264;
    UP            :: 265;
    PAGE_UP       :: 266;
    PAGE_DOWN     :: 267;
    HOME          :: 268;
    END           :: 269;
    CAPS_LOCK     :: 280;
    SCROLL_LOCK   :: 281;
    NUM_LOCK      :: 282;
    PRINT_SCREEN  :: 283;
    PAUSE         :: 284;
    F1            :: 290;
    F2            :: 291;
    F3            :: 292;
    F4            :: 293;
    F5            :: 294;
    F6            :: 295;
    F7            :: 296;
    F8            :: 297;
    F9            :: 298;
    F10           :: 299;
    F11           :: 300;
    F12           :: 301;
    LEFT_SHIFT    :: 340;
    LEFT_CONTROL  :: 341;
    LEFT_ALT      :: 342;
    LEFT_SUPER    :: 343;
    RIGHT_SHIFT   :: 344;
    RIGHT_CONTROL :: 345;
    RIGHT_ALT     :: 346;
    RIGHT_SUPER   :: 347;
    KB_MENU       :: 348;

    KP_0          :: 320;
    KP_1          :: 321;
    KP_2          :: 322;
    KP_3          :: 323;
    KP_4          :: 324;
    KP_5          :: 325;
    KP_6          :: 326;
    KP_7          :: 327;
    KP_8          :: 328;
    KP_9          :: 329;
    KP_DECIMAL    :: 330;
    KP_DIVIDE     :: 331;
    KP_MULTIPLY   :: 332;
    KP_SUBTRACT   :: 333;
    KP_ADD        :: 334;
    KP_ENTER      :: 335;
    KP_EQUAL      :: 336;

    BACK          :: 4;
    MENU          :: 5;
    VOLUME_UP     :: 24;
    VOLUME_DOWN   :: 25;

    KEY_NULL          :: NULL;

    KEY_APOSTROPHE    :: APOSTROPHE;
    KEY_COMMA         :: COMMA;
    KEY_MINUS         :: MINUS;
    KEY_PERIOD        :: PERIOD;
    KEY_SLASH         :: SLASH;
    KEY_ZERO          :: ZERO;
    KEY_ONE           :: ONE;
    KEY_TWO           :: TWO;
    KEY_THREE         :: THREE;
    KEY_FOUR          :: FOUR;
    KEY_FIVE          :: FIVE;
    KEY_SIX           :: SIX;
    KEY_SEVEN         :: SEVEN;
    KEY_EIGHT         :: EIGHT;
    KEY_NINE          :: NINE;
    KEY_SEMICOLON     :: SEMICOLON;
    KEY_EQUAL         :: EQUAL;
    KEY_A             :: A;
    KEY_B             :: B;
    KEY_C             :: C;
    KEY_D             :: D;
    KEY_E             :: E;
    KEY_F             :: F;
    KEY_G             :: G;
    KEY_H             :: H;
    KEY_I             :: I;
    KEY_J             :: J;
    KEY_K             :: K;
    KEY_L             :: L;
    KEY_M             :: M;
    KEY_N             :: N;
    KEY_O             :: O;
    KEY_P             :: P;
    KEY_Q             :: Q;
    KEY_R             :: R;
    KEY_S             :: S;
    KEY_T             :: T;
    KEY_U             :: U;
    KEY_V             :: V;
    KEY_W             :: W;
    KEY_X             :: X;
    KEY_Y             :: Y;
    KEY_Z             :: Z;
    KEY_LEFT_BRACKET  :: LEFT_BRACKET;
    KEY_BACKSLASH     :: BACKSLASH;
    KEY_RIGHT_BRACKET :: RIGHT_BRACKET;
    KEY_GRAVE         :: GRAVE;

    KEY_SPACE         :: SPACE;
    KEY_ESCAPE        :: ESCAPE;
    KEY_ENTER         :: ENTER;
    KEY_TAB           :: TAB;
    KEY_BACKSPACE     :: BACKSPACE;
    KEY_INSERT        :: INSERT;
    KEY_DELETE        :: DELETE;
    KEY_RIGHT         :: RIGHT;
    KEY_LEFT          :: LEFT;
    KEY_DOWN          :: DOWN;
    KEY_UP            :: UP;
    KEY_PAGE_UP       :: PAGE_UP;
    KEY_PAGE_DOWN     :: PAGE_DOWN;
    KEY_HOME          :: HOME;
    KEY_END           :: END;
    KEY_CAPS_LOCK     :: CAPS_LOCK;
    KEY_SCROLL_LOCK   :: SCROLL_LOCK;
    KEY_NUM_LOCK      :: NUM_LOCK;
    KEY_PRINT_SCREEN  :: PRINT_SCREEN;
    KEY_PAUSE         :: PAUSE;
    KEY_F1            :: F1;
    KEY_F2            :: F2;
    KEY_F3            :: F3;
    KEY_F4            :: F4;
    KEY_F5            :: F5;
    KEY_F6            :: F6;
    KEY_F7            :: F7;
    KEY_F8            :: F8;
    KEY_F9            :: F9;
    KEY_F10           :: F10;
    KEY_F11           :: F11;
    KEY_F12           :: F12;
    KEY_LEFT_SHIFT    :: LEFT_SHIFT;
    KEY_LEFT_CONTROL  :: LEFT_CONTROL;
    KEY_LEFT_ALT      :: LEFT_ALT;
    KEY_LEFT_SUPER    :: LEFT_SUPER;
    KEY_RIGHT_SHIFT   :: RIGHT_SHIFT;
    KEY_RIGHT_CONTROL :: RIGHT_CONTROL;
    KEY_RIGHT_ALT     :: RIGHT_ALT;
    KEY_RIGHT_SUPER   :: RIGHT_SUPER;
    KEY_KB_MENU       :: KB_MENU;

    KEY_KP_0          :: KP_0;
    KEY_KP_1          :: KP_1;
    KEY_KP_2          :: KP_2;
    KEY_KP_3          :: KP_3;
    KEY_KP_4          :: KP_4;
    KEY_KP_5          :: KP_5;
    KEY_KP_6          :: KP_6;
    KEY_KP_7          :: KP_7;
    KEY_KP_8          :: KP_8;
    KEY_KP_9          :: KP_9;
    KEY_KP_DECIMAL    :: KP_DECIMAL;
    KEY_KP_DIVIDE     :: KP_DIVIDE;
    KEY_KP_MULTIPLY   :: KP_MULTIPLY;
    KEY_KP_SUBTRACT   :: KP_SUBTRACT;
    KEY_KP_ADD        :: KP_ADD;
    KEY_KP_ENTER      :: KP_ENTER;
    KEY_KP_EQUAL      :: KP_EQUAL;

    KEY_BACK          :: BACK;
    KEY_MENU          :: MENU;
    KEY_VOLUME_UP     :: VOLUME_UP;
    KEY_VOLUME_DOWN   :: VOLUME_DOWN;
}

// Mouse buttons
MouseButton :: enum s32 {
    LEFT    :: 0;
    RIGHT   :: 1;
    MIDDLE  :: 2;
    SIDE    :: 3;
    EXTRA   :: 4;
    FORWARD :: 5;
    BACK    :: 6;

    MOUSE_BUTTON_LEFT    :: LEFT;
    MOUSE_BUTTON_RIGHT   :: RIGHT;
    MOUSE_BUTTON_MIDDLE  :: MIDDLE;
    MOUSE_BUTTON_SIDE    :: SIDE;
    MOUSE_BUTTON_EXTRA   :: EXTRA;
    MOUSE_BUTTON_FORWARD :: FORWARD;
    MOUSE_BUTTON_BACK    :: BACK;
}

// Mouse cursor
MouseCursor :: enum s32 {
    DEFAULT       :: 0;
    ARROW         :: 1;
    IBEAM         :: 2;
    CROSSHAIR     :: 3;
    POINTING_HAND :: 4;
    RESIZE_EW     :: 5;
    RESIZE_NS     :: 6;
    RESIZE_NWSE   :: 7;
    RESIZE_NESW   :: 8;
    RESIZE_ALL    :: 9;
    NOT_ALLOWED   :: 10;

    MOUSE_CURSOR_DEFAULT       :: DEFAULT;
    MOUSE_CURSOR_ARROW         :: ARROW;
    MOUSE_CURSOR_IBEAM         :: IBEAM;
    MOUSE_CURSOR_CROSSHAIR     :: CROSSHAIR;
    MOUSE_CURSOR_POINTING_HAND :: POINTING_HAND;
    MOUSE_CURSOR_RESIZE_EW     :: RESIZE_EW;
    MOUSE_CURSOR_RESIZE_NS     :: RESIZE_NS;
    MOUSE_CURSOR_RESIZE_NWSE   :: RESIZE_NWSE;
    MOUSE_CURSOR_RESIZE_NESW   :: RESIZE_NESW;
    MOUSE_CURSOR_RESIZE_ALL    :: RESIZE_ALL;
    MOUSE_CURSOR_NOT_ALLOWED   :: NOT_ALLOWED;
}

// Gamepad buttons
GamepadButton :: enum s32 {
    UNKNOWN          :: 0;
    LEFT_FACE_UP     :: 1;
    LEFT_FACE_RIGHT  :: 2;
    LEFT_FACE_DOWN   :: 3;
    LEFT_FACE_LEFT   :: 4;
    RIGHT_FACE_UP    :: 5;
    RIGHT_FACE_RIGHT :: 6;
    RIGHT_FACE_DOWN  :: 7;
    RIGHT_FACE_LEFT  :: 8;
    LEFT_TRIGGER_1   :: 9;
    LEFT_TRIGGER_2   :: 10;
    RIGHT_TRIGGER_1  :: 11;
    RIGHT_TRIGGER_2  :: 12;
    MIDDLE_LEFT      :: 13;
    MIDDLE           :: 14;
    MIDDLE_RIGHT     :: 15;
    LEFT_THUMB       :: 16;
    RIGHT_THUMB      :: 17;

    GAMEPAD_BUTTON_UNKNOWN          :: UNKNOWN;
    GAMEPAD_BUTTON_LEFT_FACE_UP     :: LEFT_FACE_UP;
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT  :: LEFT_FACE_RIGHT;
    GAMEPAD_BUTTON_LEFT_FACE_DOWN   :: LEFT_FACE_DOWN;
    GAMEPAD_BUTTON_LEFT_FACE_LEFT   :: LEFT_FACE_LEFT;
    GAMEPAD_BUTTON_RIGHT_FACE_UP    :: RIGHT_FACE_UP;
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT :: RIGHT_FACE_RIGHT;
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN  :: RIGHT_FACE_DOWN;
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT  :: RIGHT_FACE_LEFT;
    GAMEPAD_BUTTON_LEFT_TRIGGER_1   :: LEFT_TRIGGER_1;
    GAMEPAD_BUTTON_LEFT_TRIGGER_2   :: LEFT_TRIGGER_2;
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1  :: RIGHT_TRIGGER_1;
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2  :: RIGHT_TRIGGER_2;
    GAMEPAD_BUTTON_MIDDLE_LEFT      :: MIDDLE_LEFT;
    GAMEPAD_BUTTON_MIDDLE           :: MIDDLE;
    GAMEPAD_BUTTON_MIDDLE_RIGHT     :: MIDDLE_RIGHT;
    GAMEPAD_BUTTON_LEFT_THUMB       :: LEFT_THUMB;
    GAMEPAD_BUTTON_RIGHT_THUMB      :: RIGHT_THUMB;
}

// Gamepad axis
GamepadAxis :: enum s32 {
    LEFT_X        :: 0;
    LEFT_Y        :: 1;
    RIGHT_X       :: 2;
    RIGHT_Y       :: 3;
    LEFT_TRIGGER  :: 4;
    RIGHT_TRIGGER :: 5;

    GAMEPAD_AXIS_LEFT_X        :: LEFT_X;
    GAMEPAD_AXIS_LEFT_Y        :: LEFT_Y;
    GAMEPAD_AXIS_RIGHT_X       :: RIGHT_X;
    GAMEPAD_AXIS_RIGHT_Y       :: RIGHT_Y;
    GAMEPAD_AXIS_LEFT_TRIGGER  :: LEFT_TRIGGER;
    GAMEPAD_AXIS_RIGHT_TRIGGER :: RIGHT_TRIGGER;
}

// Material map index
MaterialMapIndex :: enum s32 {
    ALBEDO     :: 0;
    METALNESS  :: 1;
    NORMAL     :: 2;
    ROUGHNESS  :: 3;
    OCCLUSION  :: 4;
    EMISSION   :: 5;
    HEIGHT     :: 6;
    CUBEMAP    :: 7;
    IRRADIANCE :: 8;
    PREFILTER  :: 9;
    BRDF       :: 10;

    MATERIAL_MAP_ALBEDO     :: ALBEDO;
    MATERIAL_MAP_METALNESS  :: METALNESS;
    MATERIAL_MAP_NORMAL     :: NORMAL;
    MATERIAL_MAP_ROUGHNESS  :: ROUGHNESS;
    MATERIAL_MAP_OCCLUSION  :: OCCLUSION;
    MATERIAL_MAP_EMISSION   :: EMISSION;
    MATERIAL_MAP_HEIGHT     :: HEIGHT;
    MATERIAL_MAP_CUBEMAP    :: CUBEMAP;
    MATERIAL_MAP_IRRADIANCE :: IRRADIANCE;
    MATERIAL_MAP_PREFILTER  :: PREFILTER;
    MATERIAL_MAP_BRDF       :: BRDF;
}

// Shader location index
ShaderLocationIndex :: enum s32 {
    VERTEX_POSITION    :: 0;
    VERTEX_TEXCOORD01  :: 1;
    VERTEX_TEXCOORD02  :: 2;
    VERTEX_NORMAL      :: 3;
    VERTEX_TANGENT     :: 4;
    VERTEX_COLOR       :: 5;
    MATRIX_MVP         :: 6;
    MATRIX_VIEW        :: 7;
    MATRIX_PROJECTION  :: 8;
    MATRIX_MODEL       :: 9;
    MATRIX_NORMAL      :: 10;
    VECTOR_VIEW        :: 11;
    COLOR_DIFFUSE      :: 12;
    COLOR_SPECULAR     :: 13;
    COLOR_AMBIENT      :: 14;
    MAP_ALBEDO         :: 15;
    MAP_METALNESS      :: 16;
    MAP_NORMAL         :: 17;
    MAP_ROUGHNESS      :: 18;
    MAP_OCCLUSION      :: 19;
    MAP_EMISSION       :: 20;
    MAP_HEIGHT         :: 21;
    MAP_CUBEMAP        :: 22;
    MAP_IRRADIANCE     :: 23;
    MAP_PREFILTER      :: 24;
    MAP_BRDF           :: 25;
    VERTEX_BONEIDS     :: 26;
    VERTEX_BONEWEIGHTS :: 27;
    BONE_MATRICES      :: 28;

    SHADER_LOC_VERTEX_POSITION    :: VERTEX_POSITION;
    SHADER_LOC_VERTEX_TEXCOORD01  :: VERTEX_TEXCOORD01;
    SHADER_LOC_VERTEX_TEXCOORD02  :: VERTEX_TEXCOORD02;
    SHADER_LOC_VERTEX_NORMAL      :: VERTEX_NORMAL;
    SHADER_LOC_VERTEX_TANGENT     :: VERTEX_TANGENT;
    SHADER_LOC_VERTEX_COLOR       :: VERTEX_COLOR;
    SHADER_LOC_MATRIX_MVP         :: MATRIX_MVP;
    SHADER_LOC_MATRIX_VIEW        :: MATRIX_VIEW;
    SHADER_LOC_MATRIX_PROJECTION  :: MATRIX_PROJECTION;
    SHADER_LOC_MATRIX_MODEL       :: MATRIX_MODEL;
    SHADER_LOC_MATRIX_NORMAL      :: MATRIX_NORMAL;
    SHADER_LOC_VECTOR_VIEW        :: VECTOR_VIEW;
    SHADER_LOC_COLOR_DIFFUSE      :: COLOR_DIFFUSE;
    SHADER_LOC_COLOR_SPECULAR     :: COLOR_SPECULAR;
    SHADER_LOC_COLOR_AMBIENT      :: COLOR_AMBIENT;
    SHADER_LOC_MAP_ALBEDO         :: MAP_ALBEDO;
    SHADER_LOC_MAP_METALNESS      :: MAP_METALNESS;
    SHADER_LOC_MAP_NORMAL         :: MAP_NORMAL;
    SHADER_LOC_MAP_ROUGHNESS      :: MAP_ROUGHNESS;
    SHADER_LOC_MAP_OCCLUSION      :: MAP_OCCLUSION;
    SHADER_LOC_MAP_EMISSION       :: MAP_EMISSION;
    SHADER_LOC_MAP_HEIGHT         :: MAP_HEIGHT;
    SHADER_LOC_MAP_CUBEMAP        :: MAP_CUBEMAP;
    SHADER_LOC_MAP_IRRADIANCE     :: MAP_IRRADIANCE;
    SHADER_LOC_MAP_PREFILTER      :: MAP_PREFILTER;
    SHADER_LOC_MAP_BRDF           :: MAP_BRDF;
    SHADER_LOC_VERTEX_BONEIDS     :: VERTEX_BONEIDS;
    SHADER_LOC_VERTEX_BONEWEIGHTS :: VERTEX_BONEWEIGHTS;
    SHADER_LOC_BONE_MATRICES      :: BONE_MATRICES;
}

// Shader uniform data type
ShaderUniformDataType :: enum s32 {
    FLOAT     :: 0;
    VEC2      :: 1;
    VEC3      :: 2;
    VEC4      :: 3;
    INT       :: 4;
    IVEC2     :: 5;
    IVEC3     :: 6;
    IVEC4     :: 7;
    SAMPLER2D :: 8;

    SHADER_UNIFORM_FLOAT     :: FLOAT;
    SHADER_UNIFORM_VEC2      :: VEC2;
    SHADER_UNIFORM_VEC3      :: VEC3;
    SHADER_UNIFORM_VEC4      :: VEC4;
    SHADER_UNIFORM_INT       :: INT;
    SHADER_UNIFORM_IVEC2     :: IVEC2;
    SHADER_UNIFORM_IVEC3     :: IVEC3;
    SHADER_UNIFORM_IVEC4     :: IVEC4;
    SHADER_UNIFORM_SAMPLER2D :: SAMPLER2D;
}

// Shader attribute data types
ShaderAttributeDataType :: enum s32 {
    FLOAT :: 0;
    VEC2  :: 1;
    VEC3  :: 2;
    VEC4  :: 3;

    SHADER_ATTRIB_FLOAT :: FLOAT;
    SHADER_ATTRIB_VEC2  :: VEC2;
    SHADER_ATTRIB_VEC3  :: VEC3;
    SHADER_ATTRIB_VEC4  :: VEC4;
}

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
PixelFormat :: enum s32 {
    UNCOMPRESSED_GRAYSCALE    :: 1;
    UNCOMPRESSED_GRAY_ALPHA   :: 2;
    UNCOMPRESSED_R5G6B5       :: 3;
    UNCOMPRESSED_R8G8B8       :: 4;
    UNCOMPRESSED_R5G5B5A1     :: 5;
    UNCOMPRESSED_R4G4B4A4     :: 6;
    UNCOMPRESSED_R8G8B8A8     :: 7;
    UNCOMPRESSED_R32          :: 8;
    UNCOMPRESSED_R32G32B32    :: 9;
    UNCOMPRESSED_R32G32B32A32 :: 10;
    UNCOMPRESSED_R16          :: 11;
    UNCOMPRESSED_R16G16B16    :: 12;
    UNCOMPRESSED_R16G16B16A16 :: 13;
    COMPRESSED_DXT1_RGB       :: 14;
    COMPRESSED_DXT1_RGBA      :: 15;
    COMPRESSED_DXT3_RGBA      :: 16;
    COMPRESSED_DXT5_RGBA      :: 17;
    COMPRESSED_ETC1_RGB       :: 18;
    COMPRESSED_ETC2_RGB       :: 19;
    COMPRESSED_ETC2_EAC_RGBA  :: 20;
    COMPRESSED_PVRT_RGB       :: 21;
    COMPRESSED_PVRT_RGBA      :: 22;
    COMPRESSED_ASTC_4x4_RGBA  :: 23;
    COMPRESSED_ASTC_8x8_RGBA  :: 24;

    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE    :: UNCOMPRESSED_GRAYSCALE;
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA   :: UNCOMPRESSED_GRAY_ALPHA;
    PIXELFORMAT_UNCOMPRESSED_R5G6B5       :: UNCOMPRESSED_R5G6B5;
    PIXELFORMAT_UNCOMPRESSED_R8G8B8       :: UNCOMPRESSED_R8G8B8;
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1     :: UNCOMPRESSED_R5G5B5A1;
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4     :: UNCOMPRESSED_R4G4B4A4;
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8     :: UNCOMPRESSED_R8G8B8A8;
    PIXELFORMAT_UNCOMPRESSED_R32          :: UNCOMPRESSED_R32;
    PIXELFORMAT_UNCOMPRESSED_R32G32B32    :: UNCOMPRESSED_R32G32B32;
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32 :: UNCOMPRESSED_R32G32B32A32;
    PIXELFORMAT_UNCOMPRESSED_R16          :: UNCOMPRESSED_R16;
    PIXELFORMAT_UNCOMPRESSED_R16G16B16    :: UNCOMPRESSED_R16G16B16;
    PIXELFORMAT_UNCOMPRESSED_R16G16B16A16 :: UNCOMPRESSED_R16G16B16A16;
    PIXELFORMAT_COMPRESSED_DXT1_RGB       :: COMPRESSED_DXT1_RGB;
    PIXELFORMAT_COMPRESSED_DXT1_RGBA      :: COMPRESSED_DXT1_RGBA;
    PIXELFORMAT_COMPRESSED_DXT3_RGBA      :: COMPRESSED_DXT3_RGBA;
    PIXELFORMAT_COMPRESSED_DXT5_RGBA      :: COMPRESSED_DXT5_RGBA;
    PIXELFORMAT_COMPRESSED_ETC1_RGB       :: COMPRESSED_ETC1_RGB;
    PIXELFORMAT_COMPRESSED_ETC2_RGB       :: COMPRESSED_ETC2_RGB;
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA  :: COMPRESSED_ETC2_EAC_RGBA;
    PIXELFORMAT_COMPRESSED_PVRT_RGB       :: COMPRESSED_PVRT_RGB;
    PIXELFORMAT_COMPRESSED_PVRT_RGBA      :: COMPRESSED_PVRT_RGBA;
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA  :: COMPRESSED_ASTC_4x4_RGBA;
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA  :: COMPRESSED_ASTC_8x8_RGBA;
}

// Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
TextureFilter :: enum s32 {
    POINT           :: 0;
    BILINEAR        :: 1;
    TRILINEAR       :: 2;
    ANISOTROPIC_4X  :: 3;
    ANISOTROPIC_8X  :: 4;
    ANISOTROPIC_16X :: 5;

    TEXTURE_FILTER_POINT           :: POINT;
    TEXTURE_FILTER_BILINEAR        :: BILINEAR;
    TEXTURE_FILTER_TRILINEAR       :: TRILINEAR;
    TEXTURE_FILTER_ANISOTROPIC_4X  :: ANISOTROPIC_4X;
    TEXTURE_FILTER_ANISOTROPIC_8X  :: ANISOTROPIC_8X;
    TEXTURE_FILTER_ANISOTROPIC_16X :: ANISOTROPIC_16X;
}

// Texture parameters: wrap mode
TextureWrap :: enum s32 {
    REPEAT        :: 0;
    CLAMP         :: 1;
    MIRROR_REPEAT :: 2;
    MIRROR_CLAMP  :: 3;

    TEXTURE_WRAP_REPEAT        :: REPEAT;
    TEXTURE_WRAP_CLAMP         :: CLAMP;
    TEXTURE_WRAP_MIRROR_REPEAT :: MIRROR_REPEAT;
    TEXTURE_WRAP_MIRROR_CLAMP  :: MIRROR_CLAMP;
}

// Cubemap layouts
CubemapLayout :: enum s32 {
    AUTO_DETECT         :: 0;
    LINE_VERTICAL       :: 1;
    LINE_HORIZONTAL     :: 2;
    CROSS_THREE_BY_FOUR :: 3;
    CROSS_FOUR_BY_THREE :: 4;

    CUBEMAP_LAYOUT_AUTO_DETECT         :: AUTO_DETECT;
    CUBEMAP_LAYOUT_LINE_VERTICAL       :: LINE_VERTICAL;
    CUBEMAP_LAYOUT_LINE_HORIZONTAL     :: LINE_HORIZONTAL;
    CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR :: CROSS_THREE_BY_FOUR;
    CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE :: CROSS_FOUR_BY_THREE;
}

// Font type, defines generation method
FontType :: enum s32 {
    DEFAULT :: 0;
    BITMAP  :: 1;
    SDF     :: 2;

    FONT_DEFAULT :: DEFAULT;
    FONT_BITMAP  :: BITMAP;
    FONT_SDF     :: SDF;
}

// Color blending modes (pre-defined)
BlendMode :: enum s32 {
    ALPHA             :: 0;
    ADDITIVE          :: 1;
    MULTIPLIED        :: 2;
    ADD_COLORS        :: 3;
    SUBTRACT_COLORS   :: 4;
    ALPHA_PREMULTIPLY :: 5;
    CUSTOM            :: 6;
    CUSTOM_SEPARATE   :: 7;

    BLEND_ALPHA             :: ALPHA;
    BLEND_ADDITIVE          :: ADDITIVE;
    BLEND_MULTIPLIED        :: MULTIPLIED;
    BLEND_ADD_COLORS        :: ADD_COLORS;
    BLEND_SUBTRACT_COLORS   :: SUBTRACT_COLORS;
    BLEND_ALPHA_PREMULTIPLY :: ALPHA_PREMULTIPLY;
    BLEND_CUSTOM            :: CUSTOM;
    BLEND_CUSTOM_SEPARATE   :: CUSTOM_SEPARATE;
}

// Gesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
Gesture :: enum_flags u32 {
    NONE        :: 0x0;
    TAP         :: 0x1;
    DOUBLETAP   :: 0x2;
    HOLD        :: 0x4;
    DRAG        :: 0x8;
    SWIPE_RIGHT :: 0x10;
    SWIPE_LEFT  :: 0x20;
    SWIPE_UP    :: 0x40;
    SWIPE_DOWN  :: 0x80;
    PINCH_IN    :: 0x100;
    PINCH_OUT   :: 0x200;

    GESTURE_NONE        :: NONE;
    GESTURE_TAP         :: TAP;
    GESTURE_DOUBLETAP   :: DOUBLETAP;
    GESTURE_HOLD        :: HOLD;
    GESTURE_DRAG        :: DRAG;
    GESTURE_SWIPE_RIGHT :: SWIPE_RIGHT;
    GESTURE_SWIPE_LEFT  :: SWIPE_LEFT;
    GESTURE_SWIPE_UP    :: SWIPE_UP;
    GESTURE_SWIPE_DOWN  :: SWIPE_DOWN;
    GESTURE_PINCH_IN    :: PINCH_IN;
    GESTURE_PINCH_OUT   :: PINCH_OUT;
}

// Camera system modes
CameraMode :: enum s32 {
    CUSTOM       :: 0;
    FREE         :: 1;
    ORBITAL      :: 2;
    FIRST_PERSON :: 3;
    THIRD_PERSON :: 4;

    CAMERA_CUSTOM       :: CUSTOM;
    CAMERA_FREE         :: FREE;
    CAMERA_ORBITAL      :: ORBITAL;
    CAMERA_FIRST_PERSON :: FIRST_PERSON;
    CAMERA_THIRD_PERSON :: THIRD_PERSON;
}

// Camera projection
CameraProjection :: enum s32 {
    PERSPECTIVE  :: 0;
    ORTHOGRAPHIC :: 1;

    CAMERA_PERSPECTIVE  :: PERSPECTIVE;
    CAMERA_ORTHOGRAPHIC :: ORTHOGRAPHIC;
}

// N-patch layout
NPatchLayout :: enum s32 {
    NINE_PATCH             :: 0;
    THREE_PATCH_VERTICAL   :: 1;
    THREE_PATCH_HORIZONTAL :: 2;

    NPATCH_NINE_PATCH             :: NINE_PATCH;
    NPATCH_THREE_PATCH_VERTICAL   :: THREE_PATCH_VERTICAL;
    NPATCH_THREE_PATCH_HORIZONTAL :: THREE_PATCH_HORIZONTAL;
}

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
GetClipboardImage :: () -> Image #foreign raylib;
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
IsShaderValid :: (shader: Shader) -> bool #foreign raylib;
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
MakeDirectory :: (dirPath: *u8) -> s32 #foreign raylib;
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
ComputeCRC32 :: (data: *u8, dataSize: s32) -> u32 #foreign raylib;
ComputeMD5 :: (data: *u8, dataSize: s32) -> *u32 #foreign raylib;
ComputeSHA1 :: (data: *u8, dataSize: s32) -> *u32 #foreign raylib;

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

GetGamepadAxisCount :: (gamepad: s32) -> s32 #foreign raylib;
GetGamepadAxisMovement :: (gamepad: s32, axis: s32) -> float #foreign raylib;
SetGamepadMappings :: (mappings: *u8) -> s32 #foreign raylib;
SetGamepadVibration :: (gamepad: s32, leftMotor: float, rightMotor: float, duration: float) -> void #foreign raylib;

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
DrawCircleGradient :: (centerX: s32, centerY: s32, radius: float, inner: Color, outer: Color) -> void #foreign raylib;
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
DrawRectangleGradientV :: (posX: s32, posY: s32, width: s32, height: s32, top: Color, bottom: Color) -> void #foreign raylib;
DrawRectangleGradientH :: (posX: s32, posY: s32, width: s32, height: s32, left: Color, right: Color) -> void #foreign raylib;
DrawRectangleGradientEx :: (rec: Rectangle, topLeft: Color, bottomLeft: Color, topRight: Color, bottomRight: Color) -> void #foreign raylib;
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
CheckCollisionCircleLine :: (center: Vector2, radius: float, p1: Vector2, p2: Vector2) -> bool #foreign raylib;
CheckCollisionPointRec :: (point: Vector2, rec: Rectangle) -> bool #foreign raylib;
CheckCollisionPointCircle :: (point: Vector2, center: Vector2, radius: float) -> bool #foreign raylib;
CheckCollisionPointTriangle :: (point: Vector2, p1: Vector2, p2: Vector2, p3: Vector2) -> bool #foreign raylib;
CheckCollisionPointLine :: (point: Vector2, p1: Vector2, p2: Vector2, threshold: s32) -> bool #foreign raylib;
CheckCollisionPointPoly :: (point: Vector2, points: *Vector2, pointCount: s32) -> bool #foreign raylib;
CheckCollisionLines :: (startPos1: Vector2, endPos1: Vector2, startPos2: Vector2, endPos2: Vector2, collisionPoint: *Vector2) -> bool #foreign raylib;
GetCollisionRec :: (rec1: Rectangle, rec2: Rectangle) -> Rectangle #foreign raylib;

// Image loading functions
// NOTE: These functions do not require GPU access
LoadImage :: (fileName: *u8) -> Image #foreign raylib;
LoadImageRaw :: (fileName: *u8, width: s32, height: s32, format: s32, headerSize: s32) -> Image #foreign raylib;
LoadImageAnim :: (fileName: *u8, frames: *s32) -> Image #foreign raylib;
LoadImageAnimFromMemory :: (fileType: *u8, fileData: *u8, dataSize: s32, frames: *s32) -> Image #foreign raylib;
LoadImageFromMemory :: (fileType: *u8, fileData: *u8, dataSize: s32) -> Image #foreign raylib;
LoadImageFromTexture :: (texture: Texture2D) -> Image #foreign raylib;
LoadImageFromScreen :: () -> Image #foreign raylib;
IsImageValid :: (image: Image) -> bool #foreign raylib;
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
ImageFromChannel :: (image: Image, selectedChannel: s32) -> Image #foreign raylib;
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
ImageDrawLineEx :: (dst: *Image, start: Vector2, end: Vector2, thick: s32, color: Color) -> void #foreign raylib;
ImageDrawCircle :: (dst: *Image, centerX: s32, centerY: s32, radius: s32, color: Color) -> void #foreign raylib;
ImageDrawCircleV :: (dst: *Image, center: Vector2, radius: s32, color: Color) -> void #foreign raylib;
ImageDrawCircleLines :: (dst: *Image, centerX: s32, centerY: s32, radius: s32, color: Color) -> void #foreign raylib;
ImageDrawCircleLinesV :: (dst: *Image, center: Vector2, radius: s32, color: Color) -> void #foreign raylib;
ImageDrawRectangle :: (dst: *Image, posX: s32, posY: s32, width: s32, height: s32, color: Color) -> void #foreign raylib;
ImageDrawRectangleV :: (dst: *Image, position: Vector2, size: Vector2, color: Color) -> void #foreign raylib;
ImageDrawRectangleRec :: (dst: *Image, rec: Rectangle, color: Color) -> void #foreign raylib;
ImageDrawRectangleLines :: (dst: *Image, rec: Rectangle, thick: s32, color: Color) -> void #foreign raylib;
ImageDrawTriangle :: (dst: *Image, v1: Vector2, v2: Vector2, v3: Vector2, color: Color) -> void #foreign raylib;
ImageDrawTriangleEx :: (dst: *Image, v1: Vector2, v2: Vector2, v3: Vector2, c1: Color, c2: Color, c3: Color) -> void #foreign raylib;
ImageDrawTriangleLines :: (dst: *Image, v1: Vector2, v2: Vector2, v3: Vector2, color: Color) -> void #foreign raylib;
ImageDrawTriangleFan :: (dst: *Image, points: *Vector2, pointCount: s32, color: Color) -> void #foreign raylib;
ImageDrawTriangleStrip :: (dst: *Image, points: *Vector2, pointCount: s32, color: Color) -> void #foreign raylib;
ImageDraw :: (dst: *Image, src: Image, srcRec: Rectangle, dstRec: Rectangle, tint: Color) -> void #foreign raylib;
ImageDrawText :: (dst: *Image, text: *u8, posX: s32, posY: s32, fontSize: s32, color: Color) -> void #foreign raylib;
ImageDrawTextEx :: (dst: *Image, font: Font, text: *u8, position: Vector2, fontSize: float, spacing: float, tint: Color) -> void #foreign raylib;

// Texture loading functions
// NOTE: These functions require GPU access
LoadTexture :: (fileName: *u8) -> Texture2D #foreign raylib;
LoadTextureFromImage :: (image: Image) -> Texture2D #foreign raylib;
LoadTextureCubemap :: (image: Image, layout: s32) -> TextureCubemap #foreign raylib;
LoadRenderTexture :: (width: s32, height: s32) -> RenderTexture2D #foreign raylib;
IsTextureValid :: (texture: Texture2D) -> bool #foreign raylib;
UnloadTexture :: (texture: Texture2D) -> void #foreign raylib;
IsRenderTextureValid :: (target: RenderTexture2D) -> bool #foreign raylib;
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
ColorLerp :: (color1: Color, color2: Color, factor: float) -> Color #foreign raylib;
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
IsFontValid :: (font: Font) -> bool #foreign raylib;
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
IsModelValid :: (model: Model) -> bool #foreign raylib;
UnloadModel :: (model: Model) -> void #foreign raylib;
GetModelBoundingBox :: (model: Model) -> BoundingBox #foreign raylib;

// Model drawing functions
DrawModel :: (model: Model, position: Vector3, scale: float, tint: Color) -> void #foreign raylib;
DrawModelEx :: (model: Model, position: Vector3, rotationAxis: Vector3, rotationAngle: float, scale: Vector3, tint: Color) -> void #foreign raylib;
DrawModelWires :: (model: Model, position: Vector3, scale: float, tint: Color) -> void #foreign raylib;
DrawModelWiresEx :: (model: Model, position: Vector3, rotationAxis: Vector3, rotationAngle: float, scale: Vector3, tint: Color) -> void #foreign raylib;
DrawModelPoints :: (model: Model, position: Vector3, scale: float, tint: Color) -> void #foreign raylib;
DrawModelPointsEx :: (model: Model, position: Vector3, rotationAxis: Vector3, rotationAngle: float, scale: Vector3, tint: Color) -> void #foreign raylib;
DrawBoundingBox :: (box: BoundingBox, color: Color) -> void #foreign raylib;
DrawBillboard :: (camera: Camera, texture: Texture2D, position: Vector3, scale: float, tint: Color) -> void #foreign raylib;
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
IsMaterialValid :: (material: Material) -> bool #foreign raylib;
UnloadMaterial :: (material: Material) -> void #foreign raylib;
SetMaterialTexture :: (material: *Material, mapType: s32, texture: Texture2D) -> void #foreign raylib;
SetModelMeshMaterial :: (model: *Model, meshId: s32, materialId: s32) -> void #foreign raylib;

// Model animations loading/unloading functions
LoadModelAnimations :: (fileName: *u8, animCount: *s32) -> *ModelAnimation #foreign raylib;
UpdateModelAnimation :: (model: Model, anim: ModelAnimation, frame: s32) -> void #foreign raylib;
UpdateModelAnimationBones :: (model: Model, anim: ModelAnimation, frame: s32) -> void #foreign raylib;
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
IsWaveValid :: (wave: Wave) -> bool #foreign raylib;
LoadSound :: (fileName: *u8) -> Sound #foreign raylib;
LoadSoundFromWave :: (wave: Wave) -> Sound #foreign raylib;
LoadSoundAlias :: (source: Sound) -> Sound #foreign raylib;
IsSoundValid :: (sound: Sound) -> bool #foreign raylib;
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
IsMusicValid :: (music: Music) -> bool #foreign raylib;
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
IsAudioStreamValid :: (stream: AudioStream) -> bool #foreign raylib;
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
    _11   :: 1;
    _21   :: 2;
    _33   :: 3;
    _43   :: 4;
    ES_20 :: 5;
    ES_30 :: 6;

    RL_OPENGL_11    :: _11;
    RL_OPENGL_21    :: _21;
    RL_OPENGL_33    :: _33;
    RL_OPENGL_43    :: _43;
    RL_OPENGL_ES_20 :: ES_20;
    RL_OPENGL_ES_30 :: ES_30;
}

// Framebuffer attachment type
// NOTE: By default up to 8 color channels defined, but it can be more
FramebufferAttachType :: enum s32 {
    COLOR_CHANNEL0 :: 0;
    COLOR_CHANNEL1 :: 1;
    COLOR_CHANNEL2 :: 2;
    COLOR_CHANNEL3 :: 3;
    COLOR_CHANNEL4 :: 4;
    COLOR_CHANNEL5 :: 5;
    COLOR_CHANNEL6 :: 6;
    COLOR_CHANNEL7 :: 7;
    DEPTH          :: 100;
    STENCIL        :: 200;

    RL_ATTACHMENT_COLOR_CHANNEL0 :: COLOR_CHANNEL0;
    RL_ATTACHMENT_COLOR_CHANNEL1 :: COLOR_CHANNEL1;
    RL_ATTACHMENT_COLOR_CHANNEL2 :: COLOR_CHANNEL2;
    RL_ATTACHMENT_COLOR_CHANNEL3 :: COLOR_CHANNEL3;
    RL_ATTACHMENT_COLOR_CHANNEL4 :: COLOR_CHANNEL4;
    RL_ATTACHMENT_COLOR_CHANNEL5 :: COLOR_CHANNEL5;
    RL_ATTACHMENT_COLOR_CHANNEL6 :: COLOR_CHANNEL6;
    RL_ATTACHMENT_COLOR_CHANNEL7 :: COLOR_CHANNEL7;
    RL_ATTACHMENT_DEPTH          :: DEPTH;
    RL_ATTACHMENT_STENCIL        :: STENCIL;
}

// Framebuffer texture attachment type
FramebufferAttachTextureType :: enum s32 {
    CUBEMAP_POSITIVE_X :: 0;
    CUBEMAP_NEGATIVE_X :: 1;
    CUBEMAP_POSITIVE_Y :: 2;
    CUBEMAP_NEGATIVE_Y :: 3;
    CUBEMAP_POSITIVE_Z :: 4;
    CUBEMAP_NEGATIVE_Z :: 5;
    TEXTURE2D          :: 100;
    RENDERBUFFER       :: 200;

    RL_ATTACHMENT_CUBEMAP_POSITIVE_X :: CUBEMAP_POSITIVE_X;
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_X :: CUBEMAP_NEGATIVE_X;
    RL_ATTACHMENT_CUBEMAP_POSITIVE_Y :: CUBEMAP_POSITIVE_Y;
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_Y :: CUBEMAP_NEGATIVE_Y;
    RL_ATTACHMENT_CUBEMAP_POSITIVE_Z :: CUBEMAP_POSITIVE_Z;
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_Z :: CUBEMAP_NEGATIVE_Z;
    RL_ATTACHMENT_TEXTURE2D          :: TEXTURE2D;
    RL_ATTACHMENT_RENDERBUFFER       :: RENDERBUFFER;
}

// Face culling mode
CullMode :: enum s32 {
    FRONT :: 0;
    BACK  :: 1;

    RL_CULL_FACE_FRONT :: FRONT;
    RL_CULL_FACE_BACK  :: BACK;
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
LoadTextureCubemap :: (data: *void, size: s32, format: s32, mipmapCount: s32) -> u32 #foreign raylib "rlLoadTextureCubemap";
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
SetUniformMatrices :: (locIndex: s32, mat: *Matrix, count: s32) -> void #foreign raylib "rlSetUniformMatrices";
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

// Decompose a transformation matrix into its rotational, translational and scaling components
MatrixDecompose :: (mat: Matrix, translation: *Vector3, rotation: *Quaternion, scale: *Vector3) -> void #foreign raylib;

#scope_file

#import "Basic"; // For assert, push_context


#run {
    {
        instance: Color;
        assert(((cast(*void)(*instance.r)) - cast(*void)(*instance)) == 0, "Color.r has unexpected offset % instead of 0", ((cast(*void)(*instance.r)) - cast(*void)(*instance)));
        assert(size_of(type_of(Color.r)) == 1, "Color.r has unexpected size % instead of 1", size_of(type_of(Color.r)));
        assert(((cast(*void)(*instance.g)) - cast(*void)(*instance)) == 1, "Color.g has unexpected offset % instead of 1", ((cast(*void)(*instance.g)) - cast(*void)(*instance)));
        assert(size_of(type_of(Color.g)) == 1, "Color.g has unexpected size % instead of 1", size_of(type_of(Color.g)));
        assert(((cast(*void)(*instance.b)) - cast(*void)(*instance)) == 2, "Color.b has unexpected offset % instead of 2", ((cast(*void)(*instance.b)) - cast(*void)(*instance)));
        assert(size_of(type_of(Color.b)) == 1, "Color.b has unexpected size % instead of 1", size_of(type_of(Color.b)));
        assert(((cast(*void)(*instance.a)) - cast(*void)(*instance)) == 3, "Color.a has unexpected offset % instead of 3", ((cast(*void)(*instance.a)) - cast(*void)(*instance)));
        assert(size_of(type_of(Color.a)) == 1, "Color.a has unexpected size % instead of 1", size_of(type_of(Color.a)));
        assert(size_of(Color) == 4, "Color has size % instead of 4", size_of(Color));
    }

    {
        instance: Rectangle;
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 0, "Rectangle.x has unexpected offset % instead of 0", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(Rectangle.x)) == 4, "Rectangle.x has unexpected size % instead of 4", size_of(type_of(Rectangle.x)));
        assert(((cast(*void)(*instance.y)) - cast(*void)(*instance)) == 4, "Rectangle.y has unexpected offset % instead of 4", ((cast(*void)(*instance.y)) - cast(*void)(*instance)));
        assert(size_of(type_of(Rectangle.y)) == 4, "Rectangle.y has unexpected size % instead of 4", size_of(type_of(Rectangle.y)));
        assert(((cast(*void)(*instance.width)) - cast(*void)(*instance)) == 8, "Rectangle.width has unexpected offset % instead of 8", ((cast(*void)(*instance.width)) - cast(*void)(*instance)));
        assert(size_of(type_of(Rectangle.width)) == 4, "Rectangle.width has unexpected size % instead of 4", size_of(type_of(Rectangle.width)));
        assert(((cast(*void)(*instance.height)) - cast(*void)(*instance)) == 12, "Rectangle.height has unexpected offset % instead of 12", ((cast(*void)(*instance.height)) - cast(*void)(*instance)));
        assert(size_of(type_of(Rectangle.height)) == 4, "Rectangle.height has unexpected size % instead of 4", size_of(type_of(Rectangle.height)));
        assert(size_of(Rectangle) == 16, "Rectangle has size % instead of 16", size_of(Rectangle));
    }

    {
        instance: Image;
        assert(((cast(*void)(*instance.data)) - cast(*void)(*instance)) == 0, "Image.data has unexpected offset % instead of 0", ((cast(*void)(*instance.data)) - cast(*void)(*instance)));
        assert(size_of(type_of(Image.data)) == 8, "Image.data has unexpected size % instead of 8", size_of(type_of(Image.data)));
        assert(((cast(*void)(*instance.width)) - cast(*void)(*instance)) == 8, "Image.width has unexpected offset % instead of 8", ((cast(*void)(*instance.width)) - cast(*void)(*instance)));
        assert(size_of(type_of(Image.width)) == 4, "Image.width has unexpected size % instead of 4", size_of(type_of(Image.width)));
        assert(((cast(*void)(*instance.height)) - cast(*void)(*instance)) == 12, "Image.height has unexpected offset % instead of 12", ((cast(*void)(*instance.height)) - cast(*void)(*instance)));
        assert(size_of(type_of(Image.height)) == 4, "Image.height has unexpected size % instead of 4", size_of(type_of(Image.height)));
        assert(((cast(*void)(*instance.mipmaps)) - cast(*void)(*instance)) == 16, "Image.mipmaps has unexpected offset % instead of 16", ((cast(*void)(*instance.mipmaps)) - cast(*void)(*instance)));
        assert(size_of(type_of(Image.mipmaps)) == 4, "Image.mipmaps has unexpected size % instead of 4", size_of(type_of(Image.mipmaps)));
        assert(((cast(*void)(*instance.format)) - cast(*void)(*instance)) == 20, "Image.format has unexpected offset % instead of 20", ((cast(*void)(*instance.format)) - cast(*void)(*instance)));
        assert(size_of(type_of(Image.format)) == 4, "Image.format has unexpected size % instead of 4", size_of(type_of(Image.format)));
        assert(size_of(Image) == 24, "Image has size % instead of 24", size_of(Image));
    }

    {
        instance: Texture;
        assert(((cast(*void)(*instance.id)) - cast(*void)(*instance)) == 0, "Texture.id has unexpected offset % instead of 0", ((cast(*void)(*instance.id)) - cast(*void)(*instance)));
        assert(size_of(type_of(Texture.id)) == 4, "Texture.id has unexpected size % instead of 4", size_of(type_of(Texture.id)));
        assert(((cast(*void)(*instance.width)) - cast(*void)(*instance)) == 4, "Texture.width has unexpected offset % instead of 4", ((cast(*void)(*instance.width)) - cast(*void)(*instance)));
        assert(size_of(type_of(Texture.width)) == 4, "Texture.width has unexpected size % instead of 4", size_of(type_of(Texture.width)));
        assert(((cast(*void)(*instance.height)) - cast(*void)(*instance)) == 8, "Texture.height has unexpected offset % instead of 8", ((cast(*void)(*instance.height)) - cast(*void)(*instance)));
        assert(size_of(type_of(Texture.height)) == 4, "Texture.height has unexpected size % instead of 4", size_of(type_of(Texture.height)));
        assert(((cast(*void)(*instance.mipmaps)) - cast(*void)(*instance)) == 12, "Texture.mipmaps has unexpected offset % instead of 12", ((cast(*void)(*instance.mipmaps)) - cast(*void)(*instance)));
        assert(size_of(type_of(Texture.mipmaps)) == 4, "Texture.mipmaps has unexpected size % instead of 4", size_of(type_of(Texture.mipmaps)));
        assert(((cast(*void)(*instance.format)) - cast(*void)(*instance)) == 16, "Texture.format has unexpected offset % instead of 16", ((cast(*void)(*instance.format)) - cast(*void)(*instance)));
        assert(size_of(type_of(Texture.format)) == 4, "Texture.format has unexpected size % instead of 4", size_of(type_of(Texture.format)));
        assert(size_of(Texture) == 20, "Texture has size % instead of 20", size_of(Texture));
    }

    {
        instance: RenderTexture;
        assert(((cast(*void)(*instance.id)) - cast(*void)(*instance)) == 0, "RenderTexture.id has unexpected offset % instead of 0", ((cast(*void)(*instance.id)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderTexture.id)) == 4, "RenderTexture.id has unexpected size % instead of 4", size_of(type_of(RenderTexture.id)));
        assert(((cast(*void)(*instance.texture)) - cast(*void)(*instance)) == 4, "RenderTexture.texture has unexpected offset % instead of 4", ((cast(*void)(*instance.texture)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderTexture.texture)) == 20, "RenderTexture.texture has unexpected size % instead of 20", size_of(type_of(RenderTexture.texture)));
        assert(((cast(*void)(*instance.depth)) - cast(*void)(*instance)) == 24, "RenderTexture.depth has unexpected offset % instead of 24", ((cast(*void)(*instance.depth)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderTexture.depth)) == 20, "RenderTexture.depth has unexpected size % instead of 20", size_of(type_of(RenderTexture.depth)));
        assert(size_of(RenderTexture) == 44, "RenderTexture has size % instead of 44", size_of(RenderTexture));
    }

    {
        instance: NPatchInfo;
        assert(((cast(*void)(*instance.source)) - cast(*void)(*instance)) == 0, "NPatchInfo.source has unexpected offset % instead of 0", ((cast(*void)(*instance.source)) - cast(*void)(*instance)));
        assert(size_of(type_of(NPatchInfo.source)) == 16, "NPatchInfo.source has unexpected size % instead of 16", size_of(type_of(NPatchInfo.source)));
        assert(((cast(*void)(*instance.left)) - cast(*void)(*instance)) == 16, "NPatchInfo.left has unexpected offset % instead of 16", ((cast(*void)(*instance.left)) - cast(*void)(*instance)));
        assert(size_of(type_of(NPatchInfo.left)) == 4, "NPatchInfo.left has unexpected size % instead of 4", size_of(type_of(NPatchInfo.left)));
        assert(((cast(*void)(*instance.top)) - cast(*void)(*instance)) == 20, "NPatchInfo.top has unexpected offset % instead of 20", ((cast(*void)(*instance.top)) - cast(*void)(*instance)));
        assert(size_of(type_of(NPatchInfo.top)) == 4, "NPatchInfo.top has unexpected size % instead of 4", size_of(type_of(NPatchInfo.top)));
        assert(((cast(*void)(*instance.right)) - cast(*void)(*instance)) == 24, "NPatchInfo.right has unexpected offset % instead of 24", ((cast(*void)(*instance.right)) - cast(*void)(*instance)));
        assert(size_of(type_of(NPatchInfo.right)) == 4, "NPatchInfo.right has unexpected size % instead of 4", size_of(type_of(NPatchInfo.right)));
        assert(((cast(*void)(*instance.bottom)) - cast(*void)(*instance)) == 28, "NPatchInfo.bottom has unexpected offset % instead of 28", ((cast(*void)(*instance.bottom)) - cast(*void)(*instance)));
        assert(size_of(type_of(NPatchInfo.bottom)) == 4, "NPatchInfo.bottom has unexpected size % instead of 4", size_of(type_of(NPatchInfo.bottom)));
        assert(((cast(*void)(*instance.layout)) - cast(*void)(*instance)) == 32, "NPatchInfo.layout has unexpected offset % instead of 32", ((cast(*void)(*instance.layout)) - cast(*void)(*instance)));
        assert(size_of(type_of(NPatchInfo.layout)) == 4, "NPatchInfo.layout has unexpected size % instead of 4", size_of(type_of(NPatchInfo.layout)));
        assert(size_of(NPatchInfo) == 36, "NPatchInfo has size % instead of 36", size_of(NPatchInfo));
    }

    {
        instance: GlyphInfo;
        assert(((cast(*void)(*instance.value)) - cast(*void)(*instance)) == 0, "GlyphInfo.value has unexpected offset % instead of 0", ((cast(*void)(*instance.value)) - cast(*void)(*instance)));
        assert(size_of(type_of(GlyphInfo.value)) == 4, "GlyphInfo.value has unexpected size % instead of 4", size_of(type_of(GlyphInfo.value)));
        assert(((cast(*void)(*instance.offsetX)) - cast(*void)(*instance)) == 4, "GlyphInfo.offsetX has unexpected offset % instead of 4", ((cast(*void)(*instance.offsetX)) - cast(*void)(*instance)));
        assert(size_of(type_of(GlyphInfo.offsetX)) == 4, "GlyphInfo.offsetX has unexpected size % instead of 4", size_of(type_of(GlyphInfo.offsetX)));
        assert(((cast(*void)(*instance.offsetY)) - cast(*void)(*instance)) == 8, "GlyphInfo.offsetY has unexpected offset % instead of 8", ((cast(*void)(*instance.offsetY)) - cast(*void)(*instance)));
        assert(size_of(type_of(GlyphInfo.offsetY)) == 4, "GlyphInfo.offsetY has unexpected size % instead of 4", size_of(type_of(GlyphInfo.offsetY)));
        assert(((cast(*void)(*instance.advanceX)) - cast(*void)(*instance)) == 12, "GlyphInfo.advanceX has unexpected offset % instead of 12", ((cast(*void)(*instance.advanceX)) - cast(*void)(*instance)));
        assert(size_of(type_of(GlyphInfo.advanceX)) == 4, "GlyphInfo.advanceX has unexpected size % instead of 4", size_of(type_of(GlyphInfo.advanceX)));
        assert(((cast(*void)(*instance.image)) - cast(*void)(*instance)) == 16, "GlyphInfo.image has unexpected offset % instead of 16", ((cast(*void)(*instance.image)) - cast(*void)(*instance)));
        assert(size_of(type_of(GlyphInfo.image)) == 24, "GlyphInfo.image has unexpected size % instead of 24", size_of(type_of(GlyphInfo.image)));
        assert(size_of(GlyphInfo) == 40, "GlyphInfo has size % instead of 40", size_of(GlyphInfo));
    }

    {
        instance: Font;
        assert(((cast(*void)(*instance.baseSize)) - cast(*void)(*instance)) == 0, "Font.baseSize has unexpected offset % instead of 0", ((cast(*void)(*instance.baseSize)) - cast(*void)(*instance)));
        assert(size_of(type_of(Font.baseSize)) == 4, "Font.baseSize has unexpected size % instead of 4", size_of(type_of(Font.baseSize)));
        assert(((cast(*void)(*instance.glyphCount)) - cast(*void)(*instance)) == 4, "Font.glyphCount has unexpected offset % instead of 4", ((cast(*void)(*instance.glyphCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Font.glyphCount)) == 4, "Font.glyphCount has unexpected size % instead of 4", size_of(type_of(Font.glyphCount)));
        assert(((cast(*void)(*instance.glyphPadding)) - cast(*void)(*instance)) == 8, "Font.glyphPadding has unexpected offset % instead of 8", ((cast(*void)(*instance.glyphPadding)) - cast(*void)(*instance)));
        assert(size_of(type_of(Font.glyphPadding)) == 4, "Font.glyphPadding has unexpected size % instead of 4", size_of(type_of(Font.glyphPadding)));
        assert(((cast(*void)(*instance.texture)) - cast(*void)(*instance)) == 12, "Font.texture has unexpected offset % instead of 12", ((cast(*void)(*instance.texture)) - cast(*void)(*instance)));
        assert(size_of(type_of(Font.texture)) == 20, "Font.texture has unexpected size % instead of 20", size_of(type_of(Font.texture)));
        assert(((cast(*void)(*instance.recs)) - cast(*void)(*instance)) == 32, "Font.recs has unexpected offset % instead of 32", ((cast(*void)(*instance.recs)) - cast(*void)(*instance)));
        assert(size_of(type_of(Font.recs)) == 8, "Font.recs has unexpected size % instead of 8", size_of(type_of(Font.recs)));
        assert(((cast(*void)(*instance.glyphs)) - cast(*void)(*instance)) == 40, "Font.glyphs has unexpected offset % instead of 40", ((cast(*void)(*instance.glyphs)) - cast(*void)(*instance)));
        assert(size_of(type_of(Font.glyphs)) == 8, "Font.glyphs has unexpected size % instead of 8", size_of(type_of(Font.glyphs)));
        assert(size_of(Font) == 48, "Font has size % instead of 48", size_of(Font));
    }

    {
        instance: Camera2D;
        assert(((cast(*void)(*instance.offset)) - cast(*void)(*instance)) == 0, "Camera2D.offset has unexpected offset % instead of 0", ((cast(*void)(*instance.offset)) - cast(*void)(*instance)));
        assert(size_of(type_of(Camera2D.offset)) == 8, "Camera2D.offset has unexpected size % instead of 8", size_of(type_of(Camera2D.offset)));
        assert(((cast(*void)(*instance.target)) - cast(*void)(*instance)) == 8, "Camera2D.target has unexpected offset % instead of 8", ((cast(*void)(*instance.target)) - cast(*void)(*instance)));
        assert(size_of(type_of(Camera2D.target)) == 8, "Camera2D.target has unexpected size % instead of 8", size_of(type_of(Camera2D.target)));
        assert(((cast(*void)(*instance.rotation)) - cast(*void)(*instance)) == 16, "Camera2D.rotation has unexpected offset % instead of 16", ((cast(*void)(*instance.rotation)) - cast(*void)(*instance)));
        assert(size_of(type_of(Camera2D.rotation)) == 4, "Camera2D.rotation has unexpected size % instead of 4", size_of(type_of(Camera2D.rotation)));
        assert(((cast(*void)(*instance.zoom)) - cast(*void)(*instance)) == 20, "Camera2D.zoom has unexpected offset % instead of 20", ((cast(*void)(*instance.zoom)) - cast(*void)(*instance)));
        assert(size_of(type_of(Camera2D.zoom)) == 4, "Camera2D.zoom has unexpected size % instead of 4", size_of(type_of(Camera2D.zoom)));
        assert(size_of(Camera2D) == 24, "Camera2D has size % instead of 24", size_of(Camera2D));
    }

    {
        instance: Mesh;
        assert(((cast(*void)(*instance.vertexCount)) - cast(*void)(*instance)) == 0, "Mesh.vertexCount has unexpected offset % instead of 0", ((cast(*void)(*instance.vertexCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.vertexCount)) == 4, "Mesh.vertexCount has unexpected size % instead of 4", size_of(type_of(Mesh.vertexCount)));
        assert(((cast(*void)(*instance.triangleCount)) - cast(*void)(*instance)) == 4, "Mesh.triangleCount has unexpected offset % instead of 4", ((cast(*void)(*instance.triangleCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.triangleCount)) == 4, "Mesh.triangleCount has unexpected size % instead of 4", size_of(type_of(Mesh.triangleCount)));
        assert(((cast(*void)(*instance.vertices)) - cast(*void)(*instance)) == 8, "Mesh.vertices has unexpected offset % instead of 8", ((cast(*void)(*instance.vertices)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.vertices)) == 8, "Mesh.vertices has unexpected size % instead of 8", size_of(type_of(Mesh.vertices)));
        assert(((cast(*void)(*instance.texcoords)) - cast(*void)(*instance)) == 16, "Mesh.texcoords has unexpected offset % instead of 16", ((cast(*void)(*instance.texcoords)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.texcoords)) == 8, "Mesh.texcoords has unexpected size % instead of 8", size_of(type_of(Mesh.texcoords)));
        assert(((cast(*void)(*instance.texcoords2)) - cast(*void)(*instance)) == 24, "Mesh.texcoords2 has unexpected offset % instead of 24", ((cast(*void)(*instance.texcoords2)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.texcoords2)) == 8, "Mesh.texcoords2 has unexpected size % instead of 8", size_of(type_of(Mesh.texcoords2)));
        assert(((cast(*void)(*instance.normals)) - cast(*void)(*instance)) == 32, "Mesh.normals has unexpected offset % instead of 32", ((cast(*void)(*instance.normals)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.normals)) == 8, "Mesh.normals has unexpected size % instead of 8", size_of(type_of(Mesh.normals)));
        assert(((cast(*void)(*instance.tangents)) - cast(*void)(*instance)) == 40, "Mesh.tangents has unexpected offset % instead of 40", ((cast(*void)(*instance.tangents)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.tangents)) == 8, "Mesh.tangents has unexpected size % instead of 8", size_of(type_of(Mesh.tangents)));
        assert(((cast(*void)(*instance.colors)) - cast(*void)(*instance)) == 48, "Mesh.colors has unexpected offset % instead of 48", ((cast(*void)(*instance.colors)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.colors)) == 8, "Mesh.colors has unexpected size % instead of 8", size_of(type_of(Mesh.colors)));
        assert(((cast(*void)(*instance.indices)) - cast(*void)(*instance)) == 56, "Mesh.indices has unexpected offset % instead of 56", ((cast(*void)(*instance.indices)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.indices)) == 8, "Mesh.indices has unexpected size % instead of 8", size_of(type_of(Mesh.indices)));
        assert(((cast(*void)(*instance.animVertices)) - cast(*void)(*instance)) == 64, "Mesh.animVertices has unexpected offset % instead of 64", ((cast(*void)(*instance.animVertices)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.animVertices)) == 8, "Mesh.animVertices has unexpected size % instead of 8", size_of(type_of(Mesh.animVertices)));
        assert(((cast(*void)(*instance.animNormals)) - cast(*void)(*instance)) == 72, "Mesh.animNormals has unexpected offset % instead of 72", ((cast(*void)(*instance.animNormals)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.animNormals)) == 8, "Mesh.animNormals has unexpected size % instead of 8", size_of(type_of(Mesh.animNormals)));
        assert(((cast(*void)(*instance.boneIds)) - cast(*void)(*instance)) == 80, "Mesh.boneIds has unexpected offset % instead of 80", ((cast(*void)(*instance.boneIds)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.boneIds)) == 8, "Mesh.boneIds has unexpected size % instead of 8", size_of(type_of(Mesh.boneIds)));
        assert(((cast(*void)(*instance.boneWeights)) - cast(*void)(*instance)) == 88, "Mesh.boneWeights has unexpected offset % instead of 88", ((cast(*void)(*instance.boneWeights)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.boneWeights)) == 8, "Mesh.boneWeights has unexpected size % instead of 8", size_of(type_of(Mesh.boneWeights)));
        assert(((cast(*void)(*instance.boneMatrices)) - cast(*void)(*instance)) == 96, "Mesh.boneMatrices has unexpected offset % instead of 96", ((cast(*void)(*instance.boneMatrices)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.boneMatrices)) == 8, "Mesh.boneMatrices has unexpected size % instead of 8", size_of(type_of(Mesh.boneMatrices)));
        assert(((cast(*void)(*instance.boneCount)) - cast(*void)(*instance)) == 104, "Mesh.boneCount has unexpected offset % instead of 104", ((cast(*void)(*instance.boneCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.boneCount)) == 4, "Mesh.boneCount has unexpected size % instead of 4", size_of(type_of(Mesh.boneCount)));
        assert(((cast(*void)(*instance.vaoId)) - cast(*void)(*instance)) == 108, "Mesh.vaoId has unexpected offset % instead of 108", ((cast(*void)(*instance.vaoId)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.vaoId)) == 4, "Mesh.vaoId has unexpected size % instead of 4", size_of(type_of(Mesh.vaoId)));
        assert(((cast(*void)(*instance.vboId)) - cast(*void)(*instance)) == 112, "Mesh.vboId has unexpected offset % instead of 112", ((cast(*void)(*instance.vboId)) - cast(*void)(*instance)));
        assert(size_of(type_of(Mesh.vboId)) == 8, "Mesh.vboId has unexpected size % instead of 8", size_of(type_of(Mesh.vboId)));
        assert(size_of(Mesh) == 120, "Mesh has size % instead of 120", size_of(Mesh));
    }

    {
        instance: Shader;
        assert(((cast(*void)(*instance.id)) - cast(*void)(*instance)) == 0, "Shader.id has unexpected offset % instead of 0", ((cast(*void)(*instance.id)) - cast(*void)(*instance)));
        assert(size_of(type_of(Shader.id)) == 4, "Shader.id has unexpected size % instead of 4", size_of(type_of(Shader.id)));
        assert(((cast(*void)(*instance.locs)) - cast(*void)(*instance)) == 8, "Shader.locs has unexpected offset % instead of 8", ((cast(*void)(*instance.locs)) - cast(*void)(*instance)));
        assert(size_of(type_of(Shader.locs)) == 8, "Shader.locs has unexpected size % instead of 8", size_of(type_of(Shader.locs)));
        assert(size_of(Shader) == 16, "Shader has size % instead of 16", size_of(Shader));
    }

    {
        instance: MaterialMap;
        assert(((cast(*void)(*instance.texture)) - cast(*void)(*instance)) == 0, "MaterialMap.texture has unexpected offset % instead of 0", ((cast(*void)(*instance.texture)) - cast(*void)(*instance)));
        assert(size_of(type_of(MaterialMap.texture)) == 20, "MaterialMap.texture has unexpected size % instead of 20", size_of(type_of(MaterialMap.texture)));
        assert(((cast(*void)(*instance.color)) - cast(*void)(*instance)) == 20, "MaterialMap.color has unexpected offset % instead of 20", ((cast(*void)(*instance.color)) - cast(*void)(*instance)));
        assert(size_of(type_of(MaterialMap.color)) == 4, "MaterialMap.color has unexpected size % instead of 4", size_of(type_of(MaterialMap.color)));
        assert(((cast(*void)(*instance.value)) - cast(*void)(*instance)) == 24, "MaterialMap.value has unexpected offset % instead of 24", ((cast(*void)(*instance.value)) - cast(*void)(*instance)));
        assert(size_of(type_of(MaterialMap.value)) == 4, "MaterialMap.value has unexpected size % instead of 4", size_of(type_of(MaterialMap.value)));
        assert(size_of(MaterialMap) == 28, "MaterialMap has size % instead of 28", size_of(MaterialMap));
    }

    {
        instance: Material;
        assert(((cast(*void)(*instance.shader)) - cast(*void)(*instance)) == 0, "Material.shader has unexpected offset % instead of 0", ((cast(*void)(*instance.shader)) - cast(*void)(*instance)));
        assert(size_of(type_of(Material.shader)) == 16, "Material.shader has unexpected size % instead of 16", size_of(type_of(Material.shader)));
        assert(((cast(*void)(*instance.maps)) - cast(*void)(*instance)) == 16, "Material.maps has unexpected offset % instead of 16", ((cast(*void)(*instance.maps)) - cast(*void)(*instance)));
        assert(size_of(type_of(Material.maps)) == 8, "Material.maps has unexpected size % instead of 8", size_of(type_of(Material.maps)));
        assert(((cast(*void)(*instance.params)) - cast(*void)(*instance)) == 24, "Material.params has unexpected offset % instead of 24", ((cast(*void)(*instance.params)) - cast(*void)(*instance)));
        assert(size_of(type_of(Material.params)) == 16, "Material.params has unexpected size % instead of 16", size_of(type_of(Material.params)));
        assert(size_of(Material) == 40, "Material has size % instead of 40", size_of(Material));
    }

    {
        instance: Transform;
        assert(((cast(*void)(*instance.translation)) - cast(*void)(*instance)) == 0, "Transform.translation has unexpected offset % instead of 0", ((cast(*void)(*instance.translation)) - cast(*void)(*instance)));
        assert(size_of(type_of(Transform.translation)) == 12, "Transform.translation has unexpected size % instead of 12", size_of(type_of(Transform.translation)));
        assert(((cast(*void)(*instance.rotation)) - cast(*void)(*instance)) == 12, "Transform.rotation has unexpected offset % instead of 12", ((cast(*void)(*instance.rotation)) - cast(*void)(*instance)));
        assert(size_of(type_of(Transform.rotation)) == 16, "Transform.rotation has unexpected size % instead of 16", size_of(type_of(Transform.rotation)));
        assert(((cast(*void)(*instance.scale)) - cast(*void)(*instance)) == 28, "Transform.scale has unexpected offset % instead of 28", ((cast(*void)(*instance.scale)) - cast(*void)(*instance)));
        assert(size_of(type_of(Transform.scale)) == 12, "Transform.scale has unexpected size % instead of 12", size_of(type_of(Transform.scale)));
        assert(size_of(Transform) == 40, "Transform has size % instead of 40", size_of(Transform));
    }

    {
        instance: BoneInfo;
        assert(((cast(*void)(*instance.name)) - cast(*void)(*instance)) == 0, "BoneInfo.name has unexpected offset % instead of 0", ((cast(*void)(*instance.name)) - cast(*void)(*instance)));
        assert(size_of(type_of(BoneInfo.name)) == 32, "BoneInfo.name has unexpected size % instead of 32", size_of(type_of(BoneInfo.name)));
        assert(((cast(*void)(*instance.parent)) - cast(*void)(*instance)) == 32, "BoneInfo.parent has unexpected offset % instead of 32", ((cast(*void)(*instance.parent)) - cast(*void)(*instance)));
        assert(size_of(type_of(BoneInfo.parent)) == 4, "BoneInfo.parent has unexpected size % instead of 4", size_of(type_of(BoneInfo.parent)));
        assert(size_of(BoneInfo) == 36, "BoneInfo has size % instead of 36", size_of(BoneInfo));
    }

    {
        instance: Model;
        assert(((cast(*void)(*instance.transform)) - cast(*void)(*instance)) == 0, "Model.transform has unexpected offset % instead of 0", ((cast(*void)(*instance.transform)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.transform)) == 64, "Model.transform has unexpected size % instead of 64", size_of(type_of(Model.transform)));
        assert(((cast(*void)(*instance.meshCount)) - cast(*void)(*instance)) == 64, "Model.meshCount has unexpected offset % instead of 64", ((cast(*void)(*instance.meshCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.meshCount)) == 4, "Model.meshCount has unexpected size % instead of 4", size_of(type_of(Model.meshCount)));
        assert(((cast(*void)(*instance.materialCount)) - cast(*void)(*instance)) == 68, "Model.materialCount has unexpected offset % instead of 68", ((cast(*void)(*instance.materialCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.materialCount)) == 4, "Model.materialCount has unexpected size % instead of 4", size_of(type_of(Model.materialCount)));
        assert(((cast(*void)(*instance.meshes)) - cast(*void)(*instance)) == 72, "Model.meshes has unexpected offset % instead of 72", ((cast(*void)(*instance.meshes)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.meshes)) == 8, "Model.meshes has unexpected size % instead of 8", size_of(type_of(Model.meshes)));
        assert(((cast(*void)(*instance.materials)) - cast(*void)(*instance)) == 80, "Model.materials has unexpected offset % instead of 80", ((cast(*void)(*instance.materials)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.materials)) == 8, "Model.materials has unexpected size % instead of 8", size_of(type_of(Model.materials)));
        assert(((cast(*void)(*instance.meshMaterial)) - cast(*void)(*instance)) == 88, "Model.meshMaterial has unexpected offset % instead of 88", ((cast(*void)(*instance.meshMaterial)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.meshMaterial)) == 8, "Model.meshMaterial has unexpected size % instead of 8", size_of(type_of(Model.meshMaterial)));
        assert(((cast(*void)(*instance.boneCount)) - cast(*void)(*instance)) == 96, "Model.boneCount has unexpected offset % instead of 96", ((cast(*void)(*instance.boneCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.boneCount)) == 4, "Model.boneCount has unexpected size % instead of 4", size_of(type_of(Model.boneCount)));
        assert(((cast(*void)(*instance.bones)) - cast(*void)(*instance)) == 104, "Model.bones has unexpected offset % instead of 104", ((cast(*void)(*instance.bones)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.bones)) == 8, "Model.bones has unexpected size % instead of 8", size_of(type_of(Model.bones)));
        assert(((cast(*void)(*instance.bindPose)) - cast(*void)(*instance)) == 112, "Model.bindPose has unexpected offset % instead of 112", ((cast(*void)(*instance.bindPose)) - cast(*void)(*instance)));
        assert(size_of(type_of(Model.bindPose)) == 8, "Model.bindPose has unexpected size % instead of 8", size_of(type_of(Model.bindPose)));
        assert(size_of(Model) == 120, "Model has size % instead of 120", size_of(Model));
    }

    {
        instance: ModelAnimation;
        assert(((cast(*void)(*instance.boneCount)) - cast(*void)(*instance)) == 0, "ModelAnimation.boneCount has unexpected offset % instead of 0", ((cast(*void)(*instance.boneCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(ModelAnimation.boneCount)) == 4, "ModelAnimation.boneCount has unexpected size % instead of 4", size_of(type_of(ModelAnimation.boneCount)));
        assert(((cast(*void)(*instance.frameCount)) - cast(*void)(*instance)) == 4, "ModelAnimation.frameCount has unexpected offset % instead of 4", ((cast(*void)(*instance.frameCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(ModelAnimation.frameCount)) == 4, "ModelAnimation.frameCount has unexpected size % instead of 4", size_of(type_of(ModelAnimation.frameCount)));
        assert(((cast(*void)(*instance.bones)) - cast(*void)(*instance)) == 8, "ModelAnimation.bones has unexpected offset % instead of 8", ((cast(*void)(*instance.bones)) - cast(*void)(*instance)));
        assert(size_of(type_of(ModelAnimation.bones)) == 8, "ModelAnimation.bones has unexpected size % instead of 8", size_of(type_of(ModelAnimation.bones)));
        assert(((cast(*void)(*instance.framePoses)) - cast(*void)(*instance)) == 16, "ModelAnimation.framePoses has unexpected offset % instead of 16", ((cast(*void)(*instance.framePoses)) - cast(*void)(*instance)));
        assert(size_of(type_of(ModelAnimation.framePoses)) == 8, "ModelAnimation.framePoses has unexpected size % instead of 8", size_of(type_of(ModelAnimation.framePoses)));
        assert(((cast(*void)(*instance.name)) - cast(*void)(*instance)) == 24, "ModelAnimation.name has unexpected offset % instead of 24", ((cast(*void)(*instance.name)) - cast(*void)(*instance)));
        assert(size_of(type_of(ModelAnimation.name)) == 32, "ModelAnimation.name has unexpected size % instead of 32", size_of(type_of(ModelAnimation.name)));
        assert(size_of(ModelAnimation) == 56, "ModelAnimation has size % instead of 56", size_of(ModelAnimation));
    }

    {
        instance: Ray;
        assert(((cast(*void)(*instance.position)) - cast(*void)(*instance)) == 0, "Ray.position has unexpected offset % instead of 0", ((cast(*void)(*instance.position)) - cast(*void)(*instance)));
        assert(size_of(type_of(Ray.position)) == 12, "Ray.position has unexpected size % instead of 12", size_of(type_of(Ray.position)));
        assert(((cast(*void)(*instance.direction)) - cast(*void)(*instance)) == 12, "Ray.direction has unexpected offset % instead of 12", ((cast(*void)(*instance.direction)) - cast(*void)(*instance)));
        assert(size_of(type_of(Ray.direction)) == 12, "Ray.direction has unexpected size % instead of 12", size_of(type_of(Ray.direction)));
        assert(size_of(Ray) == 24, "Ray has size % instead of 24", size_of(Ray));
    }

    {
        instance: RayCollision;
        assert(((cast(*void)(*instance.hit)) - cast(*void)(*instance)) == 0, "RayCollision.hit has unexpected offset % instead of 0", ((cast(*void)(*instance.hit)) - cast(*void)(*instance)));
        assert(size_of(type_of(RayCollision.hit)) == 1, "RayCollision.hit has unexpected size % instead of 1", size_of(type_of(RayCollision.hit)));
        assert(((cast(*void)(*instance.distance)) - cast(*void)(*instance)) == 4, "RayCollision.distance has unexpected offset % instead of 4", ((cast(*void)(*instance.distance)) - cast(*void)(*instance)));
        assert(size_of(type_of(RayCollision.distance)) == 4, "RayCollision.distance has unexpected size % instead of 4", size_of(type_of(RayCollision.distance)));
        assert(((cast(*void)(*instance.point)) - cast(*void)(*instance)) == 8, "RayCollision.point has unexpected offset % instead of 8", ((cast(*void)(*instance.point)) - cast(*void)(*instance)));
        assert(size_of(type_of(RayCollision.point)) == 12, "RayCollision.point has unexpected size % instead of 12", size_of(type_of(RayCollision.point)));
        assert(((cast(*void)(*instance.normal)) - cast(*void)(*instance)) == 20, "RayCollision.normal has unexpected offset % instead of 20", ((cast(*void)(*instance.normal)) - cast(*void)(*instance)));
        assert(size_of(type_of(RayCollision.normal)) == 12, "RayCollision.normal has unexpected size % instead of 12", size_of(type_of(RayCollision.normal)));
        assert(size_of(RayCollision) == 32, "RayCollision has size % instead of 32", size_of(RayCollision));
    }

    {
        instance: BoundingBox;
        assert(((cast(*void)(*instance.min)) - cast(*void)(*instance)) == 0, "BoundingBox.min has unexpected offset % instead of 0", ((cast(*void)(*instance.min)) - cast(*void)(*instance)));
        assert(size_of(type_of(BoundingBox.min)) == 12, "BoundingBox.min has unexpected size % instead of 12", size_of(type_of(BoundingBox.min)));
        assert(((cast(*void)(*instance.max)) - cast(*void)(*instance)) == 12, "BoundingBox.max has unexpected offset % instead of 12", ((cast(*void)(*instance.max)) - cast(*void)(*instance)));
        assert(size_of(type_of(BoundingBox.max)) == 12, "BoundingBox.max has unexpected size % instead of 12", size_of(type_of(BoundingBox.max)));
        assert(size_of(BoundingBox) == 24, "BoundingBox has size % instead of 24", size_of(BoundingBox));
    }

    {
        instance: Wave;
        assert(((cast(*void)(*instance.frameCount)) - cast(*void)(*instance)) == 0, "Wave.frameCount has unexpected offset % instead of 0", ((cast(*void)(*instance.frameCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Wave.frameCount)) == 4, "Wave.frameCount has unexpected size % instead of 4", size_of(type_of(Wave.frameCount)));
        assert(((cast(*void)(*instance.sampleRate)) - cast(*void)(*instance)) == 4, "Wave.sampleRate has unexpected offset % instead of 4", ((cast(*void)(*instance.sampleRate)) - cast(*void)(*instance)));
        assert(size_of(type_of(Wave.sampleRate)) == 4, "Wave.sampleRate has unexpected size % instead of 4", size_of(type_of(Wave.sampleRate)));
        assert(((cast(*void)(*instance.sampleSize)) - cast(*void)(*instance)) == 8, "Wave.sampleSize has unexpected offset % instead of 8", ((cast(*void)(*instance.sampleSize)) - cast(*void)(*instance)));
        assert(size_of(type_of(Wave.sampleSize)) == 4, "Wave.sampleSize has unexpected size % instead of 4", size_of(type_of(Wave.sampleSize)));
        assert(((cast(*void)(*instance.channels)) - cast(*void)(*instance)) == 12, "Wave.channels has unexpected offset % instead of 12", ((cast(*void)(*instance.channels)) - cast(*void)(*instance)));
        assert(size_of(type_of(Wave.channels)) == 4, "Wave.channels has unexpected size % instead of 4", size_of(type_of(Wave.channels)));
        assert(((cast(*void)(*instance.data)) - cast(*void)(*instance)) == 16, "Wave.data has unexpected offset % instead of 16", ((cast(*void)(*instance.data)) - cast(*void)(*instance)));
        assert(size_of(type_of(Wave.data)) == 8, "Wave.data has unexpected size % instead of 8", size_of(type_of(Wave.data)));
        assert(size_of(Wave) == 24, "Wave has size % instead of 24", size_of(Wave));
    }

    {
        instance: AudioStream;
        assert(((cast(*void)(*instance.buffer)) - cast(*void)(*instance)) == 0, "AudioStream.buffer has unexpected offset % instead of 0", ((cast(*void)(*instance.buffer)) - cast(*void)(*instance)));
        assert(size_of(type_of(AudioStream.buffer)) == 8, "AudioStream.buffer has unexpected size % instead of 8", size_of(type_of(AudioStream.buffer)));
        assert(((cast(*void)(*instance.processor)) - cast(*void)(*instance)) == 8, "AudioStream.processor has unexpected offset % instead of 8", ((cast(*void)(*instance.processor)) - cast(*void)(*instance)));
        assert(size_of(type_of(AudioStream.processor)) == 8, "AudioStream.processor has unexpected size % instead of 8", size_of(type_of(AudioStream.processor)));
        assert(((cast(*void)(*instance.sampleRate)) - cast(*void)(*instance)) == 16, "AudioStream.sampleRate has unexpected offset % instead of 16", ((cast(*void)(*instance.sampleRate)) - cast(*void)(*instance)));
        assert(size_of(type_of(AudioStream.sampleRate)) == 4, "AudioStream.sampleRate has unexpected size % instead of 4", size_of(type_of(AudioStream.sampleRate)));
        assert(((cast(*void)(*instance.sampleSize)) - cast(*void)(*instance)) == 20, "AudioStream.sampleSize has unexpected offset % instead of 20", ((cast(*void)(*instance.sampleSize)) - cast(*void)(*instance)));
        assert(size_of(type_of(AudioStream.sampleSize)) == 4, "AudioStream.sampleSize has unexpected size % instead of 4", size_of(type_of(AudioStream.sampleSize)));
        assert(((cast(*void)(*instance.channels)) - cast(*void)(*instance)) == 24, "AudioStream.channels has unexpected offset % instead of 24", ((cast(*void)(*instance.channels)) - cast(*void)(*instance)));
        assert(size_of(type_of(AudioStream.channels)) == 4, "AudioStream.channels has unexpected size % instead of 4", size_of(type_of(AudioStream.channels)));
        assert(size_of(AudioStream) == 32, "AudioStream has size % instead of 32", size_of(AudioStream));
    }

    {
        instance: Sound;
        assert(((cast(*void)(*instance.stream)) - cast(*void)(*instance)) == 0, "Sound.stream has unexpected offset % instead of 0", ((cast(*void)(*instance.stream)) - cast(*void)(*instance)));
        assert(size_of(type_of(Sound.stream)) == 32, "Sound.stream has unexpected size % instead of 32", size_of(type_of(Sound.stream)));
        assert(((cast(*void)(*instance.frameCount)) - cast(*void)(*instance)) == 32, "Sound.frameCount has unexpected offset % instead of 32", ((cast(*void)(*instance.frameCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Sound.frameCount)) == 4, "Sound.frameCount has unexpected size % instead of 4", size_of(type_of(Sound.frameCount)));
        assert(size_of(Sound) == 40, "Sound has size % instead of 40", size_of(Sound));
    }

    {
        instance: Music;
        assert(((cast(*void)(*instance.stream)) - cast(*void)(*instance)) == 0, "Music.stream has unexpected offset % instead of 0", ((cast(*void)(*instance.stream)) - cast(*void)(*instance)));
        assert(size_of(type_of(Music.stream)) == 32, "Music.stream has unexpected size % instead of 32", size_of(type_of(Music.stream)));
        assert(((cast(*void)(*instance.frameCount)) - cast(*void)(*instance)) == 32, "Music.frameCount has unexpected offset % instead of 32", ((cast(*void)(*instance.frameCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(Music.frameCount)) == 4, "Music.frameCount has unexpected size % instead of 4", size_of(type_of(Music.frameCount)));
        assert(((cast(*void)(*instance.looping)) - cast(*void)(*instance)) == 36, "Music.looping has unexpected offset % instead of 36", ((cast(*void)(*instance.looping)) - cast(*void)(*instance)));
        assert(size_of(type_of(Music.looping)) == 1, "Music.looping has unexpected size % instead of 1", size_of(type_of(Music.looping)));
        assert(((cast(*void)(*instance.ctxType)) - cast(*void)(*instance)) == 40, "Music.ctxType has unexpected offset % instead of 40", ((cast(*void)(*instance.ctxType)) - cast(*void)(*instance)));
        assert(size_of(type_of(Music.ctxType)) == 4, "Music.ctxType has unexpected size % instead of 4", size_of(type_of(Music.ctxType)));
        assert(((cast(*void)(*instance.ctxData)) - cast(*void)(*instance)) == 48, "Music.ctxData has unexpected offset % instead of 48", ((cast(*void)(*instance.ctxData)) - cast(*void)(*instance)));
        assert(size_of(type_of(Music.ctxData)) == 8, "Music.ctxData has unexpected size % instead of 8", size_of(type_of(Music.ctxData)));
        assert(size_of(Music) == 56, "Music has size % instead of 56", size_of(Music));
    }

    {
        instance: VrDeviceInfo;
        assert(((cast(*void)(*instance.hResolution)) - cast(*void)(*instance)) == 0, "VrDeviceInfo.hResolution has unexpected offset % instead of 0", ((cast(*void)(*instance.hResolution)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.hResolution)) == 4, "VrDeviceInfo.hResolution has unexpected size % instead of 4", size_of(type_of(VrDeviceInfo.hResolution)));
        assert(((cast(*void)(*instance.vResolution)) - cast(*void)(*instance)) == 4, "VrDeviceInfo.vResolution has unexpected offset % instead of 4", ((cast(*void)(*instance.vResolution)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.vResolution)) == 4, "VrDeviceInfo.vResolution has unexpected size % instead of 4", size_of(type_of(VrDeviceInfo.vResolution)));
        assert(((cast(*void)(*instance.hScreenSize)) - cast(*void)(*instance)) == 8, "VrDeviceInfo.hScreenSize has unexpected offset % instead of 8", ((cast(*void)(*instance.hScreenSize)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.hScreenSize)) == 4, "VrDeviceInfo.hScreenSize has unexpected size % instead of 4", size_of(type_of(VrDeviceInfo.hScreenSize)));
        assert(((cast(*void)(*instance.vScreenSize)) - cast(*void)(*instance)) == 12, "VrDeviceInfo.vScreenSize has unexpected offset % instead of 12", ((cast(*void)(*instance.vScreenSize)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.vScreenSize)) == 4, "VrDeviceInfo.vScreenSize has unexpected size % instead of 4", size_of(type_of(VrDeviceInfo.vScreenSize)));
        assert(((cast(*void)(*instance.eyeToScreenDistance)) - cast(*void)(*instance)) == 16, "VrDeviceInfo.eyeToScreenDistance has unexpected offset % instead of 16", ((cast(*void)(*instance.eyeToScreenDistance)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.eyeToScreenDistance)) == 4, "VrDeviceInfo.eyeToScreenDistance has unexpected size % instead of 4", size_of(type_of(VrDeviceInfo.eyeToScreenDistance)));
        assert(((cast(*void)(*instance.lensSeparationDistance)) - cast(*void)(*instance)) == 20, "VrDeviceInfo.lensSeparationDistance has unexpected offset % instead of 20", ((cast(*void)(*instance.lensSeparationDistance)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.lensSeparationDistance)) == 4, "VrDeviceInfo.lensSeparationDistance has unexpected size % instead of 4", size_of(type_of(VrDeviceInfo.lensSeparationDistance)));
        assert(((cast(*void)(*instance.interpupillaryDistance)) - cast(*void)(*instance)) == 24, "VrDeviceInfo.interpupillaryDistance has unexpected offset % instead of 24", ((cast(*void)(*instance.interpupillaryDistance)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.interpupillaryDistance)) == 4, "VrDeviceInfo.interpupillaryDistance has unexpected size % instead of 4", size_of(type_of(VrDeviceInfo.interpupillaryDistance)));
        assert(((cast(*void)(*instance.lensDistortionValues)) - cast(*void)(*instance)) == 28, "VrDeviceInfo.lensDistortionValues has unexpected offset % instead of 28", ((cast(*void)(*instance.lensDistortionValues)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.lensDistortionValues)) == 16, "VrDeviceInfo.lensDistortionValues has unexpected size % instead of 16", size_of(type_of(VrDeviceInfo.lensDistortionValues)));
        assert(((cast(*void)(*instance.chromaAbCorrection)) - cast(*void)(*instance)) == 44, "VrDeviceInfo.chromaAbCorrection has unexpected offset % instead of 44", ((cast(*void)(*instance.chromaAbCorrection)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrDeviceInfo.chromaAbCorrection)) == 16, "VrDeviceInfo.chromaAbCorrection has unexpected size % instead of 16", size_of(type_of(VrDeviceInfo.chromaAbCorrection)));
        assert(size_of(VrDeviceInfo) == 60, "VrDeviceInfo has size % instead of 60", size_of(VrDeviceInfo));
    }

    {
        instance: VrStereoConfig;
        assert(((cast(*void)(*instance.projection)) - cast(*void)(*instance)) == 0, "VrStereoConfig.projection has unexpected offset % instead of 0", ((cast(*void)(*instance.projection)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrStereoConfig.projection)) == 128, "VrStereoConfig.projection has unexpected size % instead of 128", size_of(type_of(VrStereoConfig.projection)));
        assert(((cast(*void)(*instance.viewOffset)) - cast(*void)(*instance)) == 128, "VrStereoConfig.viewOffset has unexpected offset % instead of 128", ((cast(*void)(*instance.viewOffset)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrStereoConfig.viewOffset)) == 128, "VrStereoConfig.viewOffset has unexpected size % instead of 128", size_of(type_of(VrStereoConfig.viewOffset)));
        assert(((cast(*void)(*instance.leftLensCenter)) - cast(*void)(*instance)) == 256, "VrStereoConfig.leftLensCenter has unexpected offset % instead of 256", ((cast(*void)(*instance.leftLensCenter)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrStereoConfig.leftLensCenter)) == 8, "VrStereoConfig.leftLensCenter has unexpected size % instead of 8", size_of(type_of(VrStereoConfig.leftLensCenter)));
        assert(((cast(*void)(*instance.rightLensCenter)) - cast(*void)(*instance)) == 264, "VrStereoConfig.rightLensCenter has unexpected offset % instead of 264", ((cast(*void)(*instance.rightLensCenter)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrStereoConfig.rightLensCenter)) == 8, "VrStereoConfig.rightLensCenter has unexpected size % instead of 8", size_of(type_of(VrStereoConfig.rightLensCenter)));
        assert(((cast(*void)(*instance.leftScreenCenter)) - cast(*void)(*instance)) == 272, "VrStereoConfig.leftScreenCenter has unexpected offset % instead of 272", ((cast(*void)(*instance.leftScreenCenter)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrStereoConfig.leftScreenCenter)) == 8, "VrStereoConfig.leftScreenCenter has unexpected size % instead of 8", size_of(type_of(VrStereoConfig.leftScreenCenter)));
        assert(((cast(*void)(*instance.rightScreenCenter)) - cast(*void)(*instance)) == 280, "VrStereoConfig.rightScreenCenter has unexpected offset % instead of 280", ((cast(*void)(*instance.rightScreenCenter)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrStereoConfig.rightScreenCenter)) == 8, "VrStereoConfig.rightScreenCenter has unexpected size % instead of 8", size_of(type_of(VrStereoConfig.rightScreenCenter)));
        assert(((cast(*void)(*instance.scale)) - cast(*void)(*instance)) == 288, "VrStereoConfig.scale has unexpected offset % instead of 288", ((cast(*void)(*instance.scale)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrStereoConfig.scale)) == 8, "VrStereoConfig.scale has unexpected size % instead of 8", size_of(type_of(VrStereoConfig.scale)));
        assert(((cast(*void)(*instance.scaleIn)) - cast(*void)(*instance)) == 296, "VrStereoConfig.scaleIn has unexpected offset % instead of 296", ((cast(*void)(*instance.scaleIn)) - cast(*void)(*instance)));
        assert(size_of(type_of(VrStereoConfig.scaleIn)) == 8, "VrStereoConfig.scaleIn has unexpected size % instead of 8", size_of(type_of(VrStereoConfig.scaleIn)));
        assert(size_of(VrStereoConfig) == 304, "VrStereoConfig has size % instead of 304", size_of(VrStereoConfig));
    }

    {
        instance: FilePathList;
        assert(((cast(*void)(*instance.capacity)) - cast(*void)(*instance)) == 0, "FilePathList.capacity has unexpected offset % instead of 0", ((cast(*void)(*instance.capacity)) - cast(*void)(*instance)));
        assert(size_of(type_of(FilePathList.capacity)) == 4, "FilePathList.capacity has unexpected size % instead of 4", size_of(type_of(FilePathList.capacity)));
        assert(((cast(*void)(*instance.count)) - cast(*void)(*instance)) == 4, "FilePathList.count has unexpected offset % instead of 4", ((cast(*void)(*instance.count)) - cast(*void)(*instance)));
        assert(size_of(type_of(FilePathList.count)) == 4, "FilePathList.count has unexpected size % instead of 4", size_of(type_of(FilePathList.count)));
        assert(((cast(*void)(*instance.paths)) - cast(*void)(*instance)) == 8, "FilePathList.paths has unexpected offset % instead of 8", ((cast(*void)(*instance.paths)) - cast(*void)(*instance)));
        assert(size_of(type_of(FilePathList.paths)) == 8, "FilePathList.paths has unexpected size % instead of 8", size_of(type_of(FilePathList.paths)));
        assert(size_of(FilePathList) == 16, "FilePathList has size % instead of 16", size_of(FilePathList));
    }

    {
        instance: AutomationEvent;
        assert(((cast(*void)(*instance.frame)) - cast(*void)(*instance)) == 0, "AutomationEvent.frame has unexpected offset % instead of 0", ((cast(*void)(*instance.frame)) - cast(*void)(*instance)));
        assert(size_of(type_of(AutomationEvent.frame)) == 4, "AutomationEvent.frame has unexpected size % instead of 4", size_of(type_of(AutomationEvent.frame)));
        assert(((cast(*void)(*instance.type)) - cast(*void)(*instance)) == 4, "AutomationEvent.type has unexpected offset % instead of 4", ((cast(*void)(*instance.type)) - cast(*void)(*instance)));
        assert(size_of(type_of(AutomationEvent.type)) == 4, "AutomationEvent.type has unexpected size % instead of 4", size_of(type_of(AutomationEvent.type)));
        assert(((cast(*void)(*instance.params)) - cast(*void)(*instance)) == 8, "AutomationEvent.params has unexpected offset % instead of 8", ((cast(*void)(*instance.params)) - cast(*void)(*instance)));
        assert(size_of(type_of(AutomationEvent.params)) == 16, "AutomationEvent.params has unexpected size % instead of 16", size_of(type_of(AutomationEvent.params)));
        assert(size_of(AutomationEvent) == 24, "AutomationEvent has size % instead of 24", size_of(AutomationEvent));
    }

    {
        instance: AutomationEventList;
        assert(((cast(*void)(*instance.capacity)) - cast(*void)(*instance)) == 0, "AutomationEventList.capacity has unexpected offset % instead of 0", ((cast(*void)(*instance.capacity)) - cast(*void)(*instance)));
        assert(size_of(type_of(AutomationEventList.capacity)) == 4, "AutomationEventList.capacity has unexpected size % instead of 4", size_of(type_of(AutomationEventList.capacity)));
        assert(((cast(*void)(*instance.count)) - cast(*void)(*instance)) == 4, "AutomationEventList.count has unexpected offset % instead of 4", ((cast(*void)(*instance.count)) - cast(*void)(*instance)));
        assert(size_of(type_of(AutomationEventList.count)) == 4, "AutomationEventList.count has unexpected size % instead of 4", size_of(type_of(AutomationEventList.count)));
        assert(((cast(*void)(*instance.events)) - cast(*void)(*instance)) == 8, "AutomationEventList.events has unexpected offset % instead of 8", ((cast(*void)(*instance.events)) - cast(*void)(*instance)));
        assert(size_of(type_of(AutomationEventList.events)) == 8, "AutomationEventList.events has unexpected size % instead of 8", size_of(type_of(AutomationEventList.events)));
        assert(size_of(AutomationEventList) == 16, "AutomationEventList has size % instead of 16", size_of(AutomationEventList));
    }

    {
        instance: VertexBuffer;
        assert(((cast(*void)(*instance.elementCount)) - cast(*void)(*instance)) == 0, "VertexBuffer.elementCount has unexpected offset % instead of 0", ((cast(*void)(*instance.elementCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(VertexBuffer.elementCount)) == 4, "VertexBuffer.elementCount has unexpected size % instead of 4", size_of(type_of(VertexBuffer.elementCount)));
        assert(((cast(*void)(*instance.vertices)) - cast(*void)(*instance)) == 8, "VertexBuffer.vertices has unexpected offset % instead of 8", ((cast(*void)(*instance.vertices)) - cast(*void)(*instance)));
        assert(size_of(type_of(VertexBuffer.vertices)) == 8, "VertexBuffer.vertices has unexpected size % instead of 8", size_of(type_of(VertexBuffer.vertices)));
        assert(((cast(*void)(*instance.texcoords)) - cast(*void)(*instance)) == 16, "VertexBuffer.texcoords has unexpected offset % instead of 16", ((cast(*void)(*instance.texcoords)) - cast(*void)(*instance)));
        assert(size_of(type_of(VertexBuffer.texcoords)) == 8, "VertexBuffer.texcoords has unexpected size % instead of 8", size_of(type_of(VertexBuffer.texcoords)));
        assert(((cast(*void)(*instance.normals)) - cast(*void)(*instance)) == 24, "VertexBuffer.normals has unexpected offset % instead of 24", ((cast(*void)(*instance.normals)) - cast(*void)(*instance)));
        assert(size_of(type_of(VertexBuffer.normals)) == 8, "VertexBuffer.normals has unexpected size % instead of 8", size_of(type_of(VertexBuffer.normals)));
        assert(((cast(*void)(*instance.colors)) - cast(*void)(*instance)) == 32, "VertexBuffer.colors has unexpected offset % instead of 32", ((cast(*void)(*instance.colors)) - cast(*void)(*instance)));
        assert(size_of(type_of(VertexBuffer.colors)) == 8, "VertexBuffer.colors has unexpected size % instead of 8", size_of(type_of(VertexBuffer.colors)));
        assert(((cast(*void)(*instance.indices)) - cast(*void)(*instance)) == 40, "VertexBuffer.indices has unexpected offset % instead of 40", ((cast(*void)(*instance.indices)) - cast(*void)(*instance)));
        assert(size_of(type_of(VertexBuffer.indices)) == 8, "VertexBuffer.indices has unexpected size % instead of 8", size_of(type_of(VertexBuffer.indices)));
        assert(((cast(*void)(*instance.vaoId)) - cast(*void)(*instance)) == 48, "VertexBuffer.vaoId has unexpected offset % instead of 48", ((cast(*void)(*instance.vaoId)) - cast(*void)(*instance)));
        assert(size_of(type_of(VertexBuffer.vaoId)) == 4, "VertexBuffer.vaoId has unexpected size % instead of 4", size_of(type_of(VertexBuffer.vaoId)));
        assert(((cast(*void)(*instance.vboId)) - cast(*void)(*instance)) == 52, "VertexBuffer.vboId has unexpected offset % instead of 52", ((cast(*void)(*instance.vboId)) - cast(*void)(*instance)));
        assert(size_of(type_of(VertexBuffer.vboId)) == 20, "VertexBuffer.vboId has unexpected size % instead of 20", size_of(type_of(VertexBuffer.vboId)));
        assert(size_of(VertexBuffer) == 72, "VertexBuffer has size % instead of 72", size_of(VertexBuffer));
    }

    {
        instance: DrawCall;
        assert(((cast(*void)(*instance.mode)) - cast(*void)(*instance)) == 0, "DrawCall.mode has unexpected offset % instead of 0", ((cast(*void)(*instance.mode)) - cast(*void)(*instance)));
        assert(size_of(type_of(DrawCall.mode)) == 4, "DrawCall.mode has unexpected size % instead of 4", size_of(type_of(DrawCall.mode)));
        assert(((cast(*void)(*instance.vertexCount)) - cast(*void)(*instance)) == 4, "DrawCall.vertexCount has unexpected offset % instead of 4", ((cast(*void)(*instance.vertexCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(DrawCall.vertexCount)) == 4, "DrawCall.vertexCount has unexpected size % instead of 4", size_of(type_of(DrawCall.vertexCount)));
        assert(((cast(*void)(*instance.vertexAlignment)) - cast(*void)(*instance)) == 8, "DrawCall.vertexAlignment has unexpected offset % instead of 8", ((cast(*void)(*instance.vertexAlignment)) - cast(*void)(*instance)));
        assert(size_of(type_of(DrawCall.vertexAlignment)) == 4, "DrawCall.vertexAlignment has unexpected size % instead of 4", size_of(type_of(DrawCall.vertexAlignment)));
        assert(((cast(*void)(*instance.textureId)) - cast(*void)(*instance)) == 12, "DrawCall.textureId has unexpected offset % instead of 12", ((cast(*void)(*instance.textureId)) - cast(*void)(*instance)));
        assert(size_of(type_of(DrawCall.textureId)) == 4, "DrawCall.textureId has unexpected size % instead of 4", size_of(type_of(DrawCall.textureId)));
        assert(size_of(DrawCall) == 16, "DrawCall has size % instead of 16", size_of(DrawCall));
    }

    {
        instance: RenderBatch;
        assert(((cast(*void)(*instance.bufferCount)) - cast(*void)(*instance)) == 0, "RenderBatch.bufferCount has unexpected offset % instead of 0", ((cast(*void)(*instance.bufferCount)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderBatch.bufferCount)) == 4, "RenderBatch.bufferCount has unexpected size % instead of 4", size_of(type_of(RenderBatch.bufferCount)));
        assert(((cast(*void)(*instance.currentBuffer)) - cast(*void)(*instance)) == 4, "RenderBatch.currentBuffer has unexpected offset % instead of 4", ((cast(*void)(*instance.currentBuffer)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderBatch.currentBuffer)) == 4, "RenderBatch.currentBuffer has unexpected size % instead of 4", size_of(type_of(RenderBatch.currentBuffer)));
        assert(((cast(*void)(*instance.vertexBuffer)) - cast(*void)(*instance)) == 8, "RenderBatch.vertexBuffer has unexpected offset % instead of 8", ((cast(*void)(*instance.vertexBuffer)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderBatch.vertexBuffer)) == 8, "RenderBatch.vertexBuffer has unexpected size % instead of 8", size_of(type_of(RenderBatch.vertexBuffer)));
        assert(((cast(*void)(*instance.draws)) - cast(*void)(*instance)) == 16, "RenderBatch.draws has unexpected offset % instead of 16", ((cast(*void)(*instance.draws)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderBatch.draws)) == 8, "RenderBatch.draws has unexpected size % instead of 8", size_of(type_of(RenderBatch.draws)));
        assert(((cast(*void)(*instance.drawCounter)) - cast(*void)(*instance)) == 24, "RenderBatch.drawCounter has unexpected offset % instead of 24", ((cast(*void)(*instance.drawCounter)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderBatch.drawCounter)) == 4, "RenderBatch.drawCounter has unexpected size % instead of 4", size_of(type_of(RenderBatch.drawCounter)));
        assert(((cast(*void)(*instance.currentDepth)) - cast(*void)(*instance)) == 28, "RenderBatch.currentDepth has unexpected offset % instead of 28", ((cast(*void)(*instance.currentDepth)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderBatch.currentDepth)) == 4, "RenderBatch.currentDepth has unexpected size % instead of 4", size_of(type_of(RenderBatch.currentDepth)));
        assert(size_of(RenderBatch) == 32, "RenderBatch has size % instead of 32", size_of(RenderBatch));
    }

    {
        instance: float3;
        assert(((cast(*void)(*instance.v)) - cast(*void)(*instance)) == 0, "float3.v has unexpected offset % instead of 0", ((cast(*void)(*instance.v)) - cast(*void)(*instance)));
        assert(size_of(type_of(float3.v)) == 12, "float3.v has unexpected size % instead of 12", size_of(type_of(float3.v)));
        assert(size_of(float3) == 12, "float3 has size % instead of 12", size_of(float3));
    }

    {
        instance: float16;
        assert(((cast(*void)(*instance.v)) - cast(*void)(*instance)) == 0, "float16.v has unexpected offset % instead of 0", ((cast(*void)(*instance.v)) - cast(*void)(*instance)));
        assert(size_of(type_of(float16.v)) == 64, "float16.v has unexpected size % instead of 64", size_of(type_of(float16.v)));
        assert(size_of(float16) == 64, "float16 has size % instead of 64", size_of(float16));
    }
}

