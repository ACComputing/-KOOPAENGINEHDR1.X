/*
 * ╔══════════════════════════════════════════════════════════════════════════╗
 * ║  UltraSmashEngine4k 1.x — Platform Fighter Game Engine                 ║
 * ║  Boot sequence, intro, and main menu                                    ║
 * ║  Modern C++ · SDL2 (Windows/Linux) · PR                                ║
 * ╚══════════════════════════════════════════════════════════════════════════╝
 *
 * FILE STRUCTURE (single file)
 * ---------------------------
 * - Config / Phase enum / Timing constants / Menu options
 * - Dynamic sound engine (SoundEngine: init, volume, play_sfx, play_music_ost, queue limit)
 * - SMBZ-style sprite engine (SpriteEngine: sheet load from assets/sprites/, frame grid, AnimState, draw_sprite)
 * - App state (window, renderer, phase, frame, menu cursor)
 * - Easing helpers, embedded 8x8 font, draw_text
 * - render_boot_splash(), render_boot_credits() — boot screens
 * - render_intro_title(), render_intro_hold() — title reveal
 * - render_main_menu() — cursor menu (1P, 2P, Versus, Options, Exit)
 * - advance_phase(), process_input(), render(), main()
 *
 * BUILD: g++ -std=c++17 -O2 -o ultrasmashengine4k ultrasmashengine4k.cpp $(pkg-config --cflags --libs sdl2)
 * Dynamic sound engine: volume control (master/sfx/music), queue limit, procedural SFX; OST via SDL2_mixer.
 *
 * ═══════════════════════════════════════════════════════════════════════════
 *  IMPORT PROMPT — Havoc Engine + Smash 64 Full Roster & Content
 *  Use this block as the specification when extending UltraSmashEngine4k.
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * You are extending UltraSmashEngine4k 1.x. Import and implement the following:
 *
 * ─── 1. HAVOC ENGINE (physics / collision) ─────────────────────────────────
 * - Integrate Havoc-style rigid-body physics: velocity, acceleration, mass,
 *   gravity, ground/air state, knockback, hitstun, tumble.
 * - Collision: AABB or capsule per fighter; stage collision (platforms, edges,
 *   blast zones); hitbox vs hurtbox resolution; shield collision.
 * - Response: pushback, wall tech, ledge grab, platform drop-through.
 * - Run a fixed timestep physics step (e.g. 60 Hz) and interpolate for render.
 *
 * ─── 2. SMASH 64 — ALL FIGHTERS (12 characters) ─────────────────────────────
 * Implement every fighter with full moveset, weight, fall speed, and traits:
 *
 * - MARIO:    Fireball (B), Super Jump Punch (Up-B), Cape (Down-B), Mario Tornado (Side-B).
 *             Weight mid, balanced. Jab, dash, tilts, smashes, N-air, F-air, B-air, U-air, D-air.
 * - DONKEY KONG: Giant Punch (charge B), Spinning Kong (Up-B), Headbutt (Down-B), Hand Slap (Side-B).
 *             Heavy, strong. Punch/kick normals, cargo throw.
 * - LINK:     Bow (B), Spin Attack (Up-B), Bomb (Down-B), Boomerang (Side-B).
 *             Mid-heavy, sword normals, projectiles.
 * - SAMUS:    Charge Shot (B), Screw Attack (Up-B), Bomb (Down-B), Missile (Side-B).
 *             Floaty, zoning. Grapple beam, morph ball.
 * - YOSHI:    Egg Lay (B), Egg Throw (Up-B), Egg Roll (Down-B). Light, jump armor, tongue.
 * - KIRBY:    Inhale (B), Final Cutter (Up-B), Stone (Down-B), Hammer (Side-B). Copy ability.
 *             Light, multiple jumps.
 * - FOX:      Blaster (B), Fire Fox (Up-B), Reflector (Down-B), Fox Illusion (Side-B).
 *             Fast faller, shine, laser.
 * - PIKACHU:  Thunder Jolt (B), Quick Attack (Up-B), Thunder (Down-B). Light, electric.
 * - LUIGI:    Fireball (B), Super Jump Punch (Up-B), Luigi Cyclone (Down-B). Floaty, misfire.
 * - NESS:     PK Flash (B), PK Thunder (Up-B), PSI Magnet (Down-B), PK Fire (Side-B).
 *             Bat, yo-yo, absorb.
 * - CAPTAIN FALCON: Falcon Punch (B), Falcon Dive (Up-B), Falcon Kick (Down-B), Raptor Boost (Side-B).
 *             Heavy fast, knee, stomp.
 * - JIGGLYPUFF: Pound (B), Sing (Up-B), Rest (Down-B), Rollout (Side-B). Light, multiple jumps, rest.
 *
 * For each fighter: define FighterID enum, weight class, walk/run/dash speed,
 * jump count, fall speed, fast-fall multiplier; state machine (idle, walk, run,
 * jump, fall, attack, hit, tumble, ledge); animation frame indices or keyframes;
 * full normal moves (jab, tilts, smashes, aerials), specials, throws, taunt.
 *
 * ─── 3. STAGES ─────────────────────────────────────────────────────────────
 * - Implement all Smash 64 stages: Peach's Castle, Kongo Jungle, Hyrule Castle,
 *   Great Bay (or equivalent), Yoshi's Story, Dream Land, Saffron City, Sector Z,
 *   Mushroom Kingdom, plus a simple Battlefield-style and Final Destination-style.
 * - Per stage: collision mesh or platform list, blast zones, spawn points,
 *   stage hazards (if any), background/foreground layers.
 *
 * ─── 4. COMBAT SYSTEM ──────────────────────────────────────────────────────
 * - Damage: percent (0–999), knockback formula (base + scaling * percent), DI.
 * - Hitboxes: radius or rect, damage, angle, base knockback, knockback scaling,
 *   hitstun frames, priority. Multi-hit and single-hit.
 * - Hurtboxes: per limb or single capsule, invincible during certain states.
 * - Shield: size, health, stun on break. Grab vs shield.
 * - Ledge: invincibility frames, getup options. Ledge hog.
 *
 * ─── 5. GAME MODES & RULES ───────────────────────────────────────────────────
 * - Versus: stocks (1–99) or time; FFA or teams; items on/off.
 * - 1P: Classic-style or target test; 2P: versus only or co-op.
 * - Pause menu, result screen, character select (grid or roster), stage select.
 *
 * ─── 6. AUDIO & VISUALS ─────────────────────────────────────────────────────
 * - Per fighter: placeholder or real sprites/models; attack effects (slashes,
 *   explosions, sparks). Stage music and SFX (hit, KO, menu).
 *
 * ─── 7. OUTPUT ──────────────────────────────────────────────────────────────
 * - Extend ultrasmashengine4k.cpp (or split into .h/.cpp modules) with:
 *   - HavocPhysics (or equivalent) struct and step function.
 *   - FighterDB with all 12 fighters and their moves.
 *   - StageDB with all stages and collision.
 *   - GameState: fighters[], stage, phase (fight, result, select), rules.
 *   - Main loop: input → physics → combat resolution → render.
 * - Keep the existing boot/intro/menu flow; from MainMenu, start game into
 *   character select, then stage select, then fight. Return to menu on quit.
 *
 * End of IMPORT PROMPT.
 * ═══════════════════════════════════════════════════════════════════════════
 */

#include <SDL2/SDL.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ─── Configuration ────────────────────────────────────────────────────────
namespace Config {
    const int WINDOW_W     = 640;
    const int WINDOW_H     = 480;
    const int TARGET_FPS   = 60;
    const int MS_PER_FRAME = 1000 / TARGET_FPS;
    const int SAMPLE_RATE  = 44100;
}

// ─── Dynamic Sound Engine (PR) ─────────────────────────────────────────────
// Runtime volume, queue limits, procedural SFX + optional WAV/OST. Safe no-op when disabled.
enum class SfxId { MenuMove, MenuSelect, Hit, Jump, KO, Result };

class SoundEngine {
public:
    static constexpr unsigned kMaxQueueBytes = 256 * 1024;  // cap queued audio

    SDL_AudioDeviceID dev = 0;
    SDL_AudioSpec spec = {};
    bool enabled = false;
    float master_vol = 1.0f;
    float sfx_vol = 1.0f;
    float music_vol = 1.0f;

    bool init() {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) return false;
        spec.freq = Config::SAMPLE_RATE;
        spec.format = AUDIO_S16SYS;
        spec.channels = 1;
        spec.samples = 512;
        spec.callback = nullptr;
        dev = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
        if (dev == 0) return false;
        SDL_PauseAudioDevice(dev, 0);
        enabled = true;
        return true;
    }

    void shutdown() {
        if (dev) { SDL_CloseAudioDevice(dev); dev = 0; }
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        enabled = false;
    }

    void set_master_volume(float v) { master_vol = v < 0 ? 0 : (v > 1 ? 1 : v); }
    void set_sfx_volume(float v)    { sfx_vol   = v < 0 ? 0 : (v > 1 ? 1 : v); }
    void set_music_volume(float v)  { music_vol = v < 0 ? 0 : (v > 1 ? 1 : v); }
    bool is_active() const          { return enabled && dev != 0; }

    void play_sfx(SfxId id) {
        if (!is_active()) return;
        unsigned queued = SDL_GetQueuedAudioSize(dev);
        if (queued >= kMaxQueueBytes) return;

        int freq = 440, ms = 80;
        float vol = 0.3f;
        switch (id) {
            case SfxId::MenuMove:   freq = 330; ms = 40;  break;
            case SfxId::MenuSelect: freq = 523; ms = 100; break;
            case SfxId::Hit:        freq = 200; ms = 60;  vol = 0.25f; break;
            case SfxId::Jump:       freq = 400; ms = 50;  break;
            case SfxId::KO:        freq = 150; ms = 200; vol = 0.35f; break;
            case SfxId::Result:     freq = 880; ms = 150; break;
        }
        vol *= master_vol * sfx_vol;
        int samples = (Config::SAMPLE_RATE * ms) / 1000;
        if (samples > 22050) samples = 22050;
        std::vector<Sint16> buf(samples);
        for (int i = 0; i < samples; i++) {
            float t = (float)i / (float)Config::SAMPLE_RATE;
            float env = 1.0f - (float)i / (float)samples;
            float s = (float)(std::sin(2.0 * M_PI * freq * t) * env * vol * 32767.0f);
            buf[i] = (Sint16)s;
        }
        SDL_QueueAudio(dev, buf.data(), (Uint32)(samples * sizeof(Sint16)));
    }

    void play_music_ost(const char* path) {
        (void)path;
        // OST: link SDL2_mixer; Mix_LoadMUS(path), Mix_VolumeMusic((int)(music_vol*master_vol*128)), Mix_PlayMusic(..., -1).
    }

    void stop_music() {
        // When using SDL2_mixer: Mix_HaltMusic().
    }
};

static SoundEngine s_sound;

// ─── SMBZ-style Sprite Engine ───────────────────────────────────────────────
// Sprite sheets: row/col grid, frame-based animation. Place assets in assets/sprites/.
namespace SpriteEngine {
    struct SpriteSheet {
        SDL_Texture* tex = nullptr;
        int frame_w = 32;
        int frame_h = 32;
        int cols = 4;
        int frame_count = 4;
    };

    enum class AnimState { Idle, Walk, Jump, Fall, Attack };

    static SpriteSheet s_fighter_sheet;
    static bool s_fighter_sheet_loaded = false;

    void load_fighter_sheet(SDL_Renderer* ren) {
        if (s_fighter_sheet_loaded) return;
        s_fighter_sheet_loaded = true;
        const char* path = "assets/sprites/fighters.bmp";
        SDL_Surface* surf = SDL_LoadBMP(path);
        if (!surf) return;
        s_fighter_sheet.tex = SDL_CreateTextureFromSurface(ren, surf);
        SDL_FreeSurface(surf);
        if (!s_fighter_sheet.tex) return;
        int w = 0, h = 0;
        SDL_QueryTexture(s_fighter_sheet.tex, nullptr, nullptr, &w, &h);
        s_fighter_sheet.frame_w = 32;
        s_fighter_sheet.frame_h = 32;
        s_fighter_sheet.cols = w / 32;
        s_fighter_sheet.frame_count = (w / 32) * (h / 32);
        if (s_fighter_sheet.frame_count < 1) s_fighter_sheet.frame_count = 1;
    }

    void shutdown() {
        if (s_fighter_sheet.tex) {
            SDL_DestroyTexture(s_fighter_sheet.tex);
            s_fighter_sheet.tex = nullptr;
        }
        s_fighter_sheet_loaded = false;
    }

    int frame_from_state(AnimState state, int frame_time) {
        switch (state) {
            case AnimState::Idle:  return 0;
            case AnimState::Walk:  return 1 + (frame_time / 8) % 3;
            case AnimState::Jump:  return 4;
            case AnimState::Fall:  return 5;
            case AnimState::Attack: return 6;
        }
        return 0;
    }

    void draw_sprite(SDL_Renderer* ren, const SpriteSheet& sheet, int frame_index,
                     int screen_x, int screen_y, float scale, bool flip_h) {
        if (!sheet.tex || frame_index < 0) return;
        int f = frame_index % sheet.frame_count;
        int col = f % sheet.cols;
        int row = f / sheet.cols;
        SDL_Rect src = { col * sheet.frame_w, row * sheet.frame_h, sheet.frame_w, sheet.frame_h };
        int dw = (int)(sheet.frame_w * scale);
        int dh = (int)(sheet.frame_h * scale);
        SDL_Rect dst = { screen_x - dw/2, screen_y - dh, dw, dh };
        SDL_RendererFlip flip = flip_h ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(ren, sheet.tex, &src, &dst, 0.0, nullptr, flip);
    }

    bool has_fighter_sheet() { return s_fighter_sheet.tex != nullptr; }
}

// ─── Boot / Intro / Menu phases ─────────────────────────────────────────────
enum class Phase {
    BootSplash,
    BootCredits,
    IntroTitle,
    IntroHold,
    MainMenu,
    CharacterSelect,
    StageSelect,
    Fight,
    Result,
    Exit
};

// ─── Smash 64 roster (12 fighters) ──────────────────────────────────────────
enum class FighterID {
    Mario,
    DonkeyKong,
    Link,
    Samus,
    Yoshi,
    Kirby,
    Fox,
    Pikachu,
    Luigi,
    Ness,
    CaptainFalcon,
    Jigglypuff,
    Count
};

static const char* const s_fighter_names[(int)FighterID::Count] = {
    "MARIO",
    "DONKEY KONG",
    "LINK",
    "SAMUS",
    "YOSHI",
    "KIRBY",
    "FOX",
    "PIKACHU",
    "LUIGI",
    "NESS",
    "CAPTAIN FALCON",
    "JIGGLYPUFF"
};

// ─── Smash 64 stages (all 9 + Battlefield) ──────────────────────────────────
enum class StageID {
    PeachsCastle,
    KongoJungle,
    HyruleCastle,
    YoshisStory,
    DreamLand,
    SaffronCity,
    SectorZ,
    MushroomKingdom,
    Battlefield,
    Count
};

static const char* const s_stage_names[(int)StageID::Count] = {
    "PEACHS CASTLE",
    "KONGO JUNGLE",
    "HYRULE CASTLE",
    "YOSHIS STORY",
    "DREAM LAND",
    "SAFFRON CITY",
    "SECTOR Z",
    "MUSHROOM KINGDOM",
    "BATTLEFIELD"
};

// Per-stage background gradient (sky top -> ground bottom). RGB top, RGB bottom.
struct StageBg { Uint8 r1,g1,b1, r2,g2,b2; };
static const StageBg s_stage_bg[(int)StageID::Count] = {
    { 135, 206, 235,  80, 120, 180 },  // Peach's Castle — sky blue -> dusk
    {  34, 139,  34,  20,  80,  20 },  // Kongo Jungle — green canopy
    {  70, 130, 180,  50,  60,  90 },  // Hyrule Castle — steel blue
    {  144, 238, 144,  60, 140,  60 }, // Yoshi's Story — light green
    {  173, 216, 230,  90, 140, 160 }, // Dream Land — pastel sky
    {  255, 165,   0,  80,  40,  40 }, // Saffron City — orange/grey
    {  25,  25, 112,  10,  10,  40 },  // Sector Z — space dark
    {  124, 252,   0,  34, 139,  34 }, // Mushroom Kingdom — bright green
    {  65, 105, 225,  40,  50,  80 },  // Battlefield — royal blue
};

static void draw_stage_gradient(SDL_Renderer* ren, StageID sid) {
    const StageBg& bg = s_stage_bg[(int)sid];
    int w = Config::WINDOW_W, h = Config::WINDOW_H;
    for (int y = 0; y < h; y++) {
        float t = (float)y / (float)h;
        Uint8 r = (Uint8)(bg.r1 + t * (bg.r2 - bg.r1));
        Uint8 g = (Uint8)(bg.g1 + t * (bg.g2 - bg.g1));
        Uint8 b = (Uint8)(bg.b1 + t * (bg.b2 - bg.b1));
        SDL_SetRenderDrawColor(ren, r, g, b, 255);
        SDL_Rect row = { 0, y, w, 1 };
        SDL_RenderFillRect(ren, &row);
    }
}

// ─── Timing (frames at 60 fps) ────────────────────────────────────────────
namespace Timing {
    const int BOOT_SPLASH_FADE_IN   = 45;
    const int BOOT_SPLASH_HOLD      = 90;
    const int BOOT_SPLASH_FADE_OUT = 45;
    const int BOOT_CREDITS_FADE_IN  = 30;
    const int BOOT_CREDITS_HOLD     = 120;
    const int BOOT_CREDITS_FADE_OUT = 30;
    const int INTRO_TITLE_ZOOM_FRAMES = 90;
    const int INTRO_HOLD_FRAMES     = 120;
    const int MENU_CURSOR_BLINK     = 30;
}

// ─── Menu options ─────────────────────────────────────────────────────────
enum class MenuOption {
    OnePlayer,
    TwoPlayer,
    Versus,
    Options,
    ExitGame,
    Count
};

static const char* const s_menu_labels[] = {
    "1 PLAYER GAME",
    "2 PLAYER GAME",
    "VERSUS MODE",
    "OPTIONS",
    "EXIT GAME"
};

// ─── Application state ─────────────────────────────────────────────────────
struct App {
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    Phase         phase    = Phase::BootSplash;
    int           frame    = 0;
    bool          running  = true;

    MenuOption    menu_cursor = MenuOption::OnePlayer;
    int           menu_blink  = 0;

    int           char_select_cursor = 0;
    MenuOption    char_select_mode   = MenuOption::OnePlayer;
    FighterID     selected_fighter   = FighterID::Mario;
    FighterID     p1_fighter         = FighterID::Mario;
    FighterID     p2_fighter         = FighterID::Mario;
    int           versus_select_step = 0;

    int           stage_select_cursor = 0;
    StageID       current_stage       = StageID::Battlefield;

    float         p1_x = 0, p1_y = 0, p1_vx = 0, p1_vy = 0;
    float         p2_x = 0, p2_y = 0, p2_vx = 0, p2_vy = 0;
    int           p1_damage = 0, p2_damage = 0;
    int           p1_stocks = 3, p2_stocks = 3;
    bool          p1_grounded = false, p2_grounded = false;
    int           fight_timer = 0;
    int           result_winner = 0;
};

// ─── Easing (for authentic feel) ────────────────────────────────────────────
static float ease_out_quad(float t) {
    return 1.0f - (1.0f - t) * (1.0f - t);
}
static float ease_in_quad(float t) {
    return t * t;
}

// ─── Embedded 8x8 bitmap font (space, 0-9, A-Z, ( ) . ) ─────────────────────
// Each glyph 8 bytes, 1 byte per row, MSB = left pixel.
static const unsigned char s_font_8x8[40][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // space
    {0x3E,0x63,0x73,0x7B,0x6F,0x67,0x3E,0x00}, // 0
    {0x0C,0x1C,0x0C,0x0C,0x0C,0x0C,0x3F,0x00}, // 1
    {0x3E,0x63,0x03,0x0E,0x38,0x60,0x7F,0x00}, // 2
    {0x3E,0x63,0x03,0x1E,0x03,0x63,0x3E,0x00}, // 3
    {0x06,0x0E,0x1E,0x36,0x66,0x7F,0x06,0x00}, // 4
    {0x7F,0x60,0x7E,0x03,0x03,0x63,0x3E,0x00}, // 5
    {0x1E,0x30,0x60,0x7E,0x63,0x63,0x3E,0x00}, // 6
    {0x7F,0x63,0x06,0x0C,0x18,0x18,0x18,0x00}, // 7
    {0x3E,0x63,0x63,0x3E,0x63,0x63,0x3E,0x00}, // 8
    {0x3E,0x63,0x63,0x3F,0x03,0x06,0x3C,0x00}, // 9
    {0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00}, // A
    {0x7E,0x63,0x63,0x7E,0x63,0x63,0x7E,0x00}, // B
    {0x1E,0x33,0x60,0x60,0x60,0x33,0x1E,0x00}, // C
    {0x7C,0x66,0x63,0x63,0x63,0x66,0x7C,0x00}, // D
    {0x7F,0x60,0x60,0x7E,0x60,0x60,0x7F,0x00}, // E
    {0x7F,0x60,0x60,0x7E,0x60,0x60,0x60,0x00}, // F
    {0x1E,0x33,0x60,0x67,0x63,0x33,0x1F,0x00}, // G
    {0x63,0x63,0x63,0x7F,0x63,0x63,0x63,0x00}, // H
    {0x3F,0x0C,0x0C,0x0C,0x0C,0x0C,0x3F,0x00}, // I
    {0x03,0x03,0x03,0x03,0x03,0x63,0x3E,0x00}, // J
    {0x63,0x66,0x6C,0x78,0x6C,0x66,0x63,0x00}, // K
    {0x60,0x60,0x60,0x60,0x60,0x60,0x7F,0x00}, // L
    {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00}, // M
    {0x63,0x73,0x7B,0x6F,0x67,0x63,0x63,0x00}, // N
    {0x3E,0x63,0x63,0x63,0x63,0x63,0x3E,0x00}, // O
    {0x7E,0x63,0x63,0x7E,0x60,0x60,0x60,0x00}, // P
    {0x3E,0x63,0x63,0x63,0x6B,0x66,0x3B,0x00}, // Q
    {0x7E,0x63,0x63,0x7E,0x6C,0x66,0x63,0x00}, // R
    {0x3E,0x63,0x60,0x3E,0x03,0x63,0x3E,0x00}, // S
    {0x3F,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x00}, // T
    {0x63,0x63,0x63,0x63,0x63,0x63,0x3E,0x00}, // U
    {0x63,0x63,0x63,0x36,0x1C,0x08,0x00,0x00}, // V
    {0x63,0x63,0x6B,0x7F,0x77,0x63,0x63,0x00}, // W
    {0x63,0x36,0x1C,0x1C,0x1C,0x36,0x63,0x00}, // X
    {0x33,0x33,0x33,0x1E,0x0C,0x0C,0x0C,0x00}, // Y
    {0x7F,0x06,0x0C,0x18,0x30,0x60,0x7F,0x00}, // Z
    {0x1C,0x30,0x30,0x30,0x30,0x30,0x1C,0x00}, // (
    {0x38,0x0C,0x0C,0x0C,0x0C,0x0C,0x38,0x00}, // )
    {0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x00}, // .
};

static int font_glyph_index(char c) {
    if (c == ' ') return 0;
    if (c >= '0' && c <= '9') return 1 + (c - '0');
    if (c >= 'A' && c <= 'Z') return 11 + (c - 'A');
    if (c == '(') return 37;
    if (c == ')') return 38;
    if (c == '.') return 39;
    return 0;
}

static void draw_text(SDL_Renderer* ren, int x, int y, const char* str, int scale,
                      Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(ren, r, g, b, a);
    for (; *str; str++) {
        char c = *str;
        if (c >= 'a' && c <= 'z') c = (char)(c - 32);
        int idx = font_glyph_index(c);
        const unsigned char* glyph = s_font_8x8[idx];
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if ((glyph[row] >> (7 - col)) & 1) {
                    SDL_Rect rect = { x + col * scale, y + row * scale, scale, scale };
                    SDL_RenderFillRect(ren, &rect);
                }
            }
        }
        x += 8 * scale + (scale / 2);
    }
}

// Return width in pixels of drawn string (for centering).
static int text_width(const char* str, int scale) {
    int n = 0;
    for (; *str; str++) n++;
    return n * (8 * scale + scale / 2) - (scale / 2);
}

// ─── Render: Boot splash (engine logo) ──────────────────────────────────────
static void render_boot_splash(App* app) {
    int f = app->frame;
    float alpha = 0.0f;
    if (f < Timing::BOOT_SPLASH_FADE_IN)
        alpha = (float)f / (float)Timing::BOOT_SPLASH_FADE_IN;
    else if (f < Timing::BOOT_SPLASH_FADE_IN + Timing::BOOT_SPLASH_HOLD)
        alpha = 1.0f;
    else if (f < Timing::BOOT_SPLASH_FADE_IN + Timing::BOOT_SPLASH_HOLD + Timing::BOOT_SPLASH_FADE_OUT) {
        int out = f - Timing::BOOT_SPLASH_FADE_IN - Timing::BOOT_SPLASH_HOLD;
        alpha = 1.0f - (float)out / (float)Timing::BOOT_SPLASH_FADE_OUT;
    }

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    SDL_SetRenderDrawColor(app->renderer, 30, 30, 45, (Uint8)(255 * alpha));
    SDL_Rect bg = { 0, 0, Config::WINDOW_W, Config::WINDOW_H };
    SDL_RenderFillRect(app->renderer, &bg);

    int cx = Config::WINDOW_W / 2, cy = Config::WINDOW_H / 2;
    int scale = 3;
    int w = text_width("ULTRA SMASH ENGINE", scale);
    draw_text(app->renderer, cx - w/2, cy - (8 * scale)/2 - 8, "ULTRA SMASH ENGINE", scale,
              (Uint8)(255 * alpha), (Uint8)(255 * alpha), (Uint8)(255 * alpha), (Uint8)(255 * alpha));
    int w2 = text_width("4K 1.X", scale);
    draw_text(app->renderer, cx - w2/2, cy + (8 * scale)/2 + 4, "4K 1.X", scale,
              (Uint8)(180 * alpha), (Uint8)(200 * alpha), (Uint8)(255 * alpha), (Uint8)(255 * alpha));
}

// ─── Render: Boot credits (engine / version) ────────────────────────────────
static void render_boot_credits(App* app) {
    int f = app->frame;
    float alpha = 0.0f;
    if (f < Timing::BOOT_CREDITS_FADE_IN)
        alpha = (float)f / (float)Timing::BOOT_CREDITS_FADE_IN;
    else if (f < Timing::BOOT_CREDITS_FADE_IN + Timing::BOOT_CREDITS_HOLD)
        alpha = 1.0f;
    else if (f < Timing::BOOT_CREDITS_FADE_IN + Timing::BOOT_CREDITS_HOLD + Timing::BOOT_CREDITS_FADE_OUT) {
        int out = f - Timing::BOOT_CREDITS_FADE_IN - Timing::BOOT_CREDITS_HOLD;
        alpha = 1.0f - (float)out / (float)Timing::BOOT_CREDITS_FADE_OUT;
    }

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    int cx = Config::WINDOW_W / 2, cy = Config::WINDOW_H / 2;
    int scale = 3;
    int w1 = text_width("ULTRASMASHENGINE4K", scale);
    draw_text(app->renderer, cx - w1/2, cy - 50, "ULTRASMASHENGINE4K", scale,
              255, 255, 255, (Uint8)(255 * alpha));
    int w2 = text_width("VERSION 1.X", scale);
    draw_text(app->renderer, cx - w2/2, cy + 10, "VERSION 1.X", scale,
              200, 200, 220, (Uint8)(255 * alpha));
}

// ─── Render: Intro title reveal ───────────────────────────────────────────
static void render_intro_title(App* app) {
    int f = app->frame;
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    float t = (float)f / (float)Timing::INTRO_TITLE_ZOOM_FRAMES;
    t = std::min(t, 1.0f);
    float scale = 0.3f + 0.7f * ease_out_quad(t);
    float alpha = ease_out_quad(t);

    int cx = Config::WINDOW_W / 2, cy = Config::WINDOW_H / 2;
    int title_scale = (int)(3.0f + 3.0f * scale);
    int tw = text_width("ULTRA SMASH ENGINE", title_scale);
    draw_text(app->renderer, cx - tw/2, cy - (8 * title_scale)/2 - 10,
              "ULTRA SMASH ENGINE", title_scale,
              80, 120, 220, (Uint8)(255 * alpha));
    int sw = text_width("4K 1.X", (int)(1.5f * scale + 1));
    draw_text(app->renderer, cx - sw/2, cy + (8 * title_scale)/2 + 8,
              "4K 1.X", (int)(1.5f * scale + 1),
              200, 220, 255, (Uint8)(220 * alpha));
}

// ─── Render: Intro hold (same as title, no animation) ───────────────────────
static void render_intro_hold(App* app) {
    (void)app;
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);
    int cx = Config::WINDOW_W / 2, cy = Config::WINDOW_H / 2;
    int ts = 4;
    int tw = text_width("ULTRA SMASH ENGINE", ts);
    draw_text(app->renderer, cx - tw/2, cy - (8 * ts)/2 - 8, "ULTRA SMASH ENGINE", ts, 80, 120, 220, 255);
    int sw = text_width("4K 1.X", 2);
    draw_text(app->renderer, cx - text_width("4K 1.X", 2)/2, cy + (8 * ts)/2 + 12, "4K 1.X", 2, 200, 220, 255, 220);
}

// ─── Render: Main menu ────────────────────────────────────────────────────
static void render_main_menu(App* app) {
    SDL_SetRenderDrawColor(app->renderer, 20, 20, 40, 255);
    SDL_RenderClear(app->renderer);

    int cx = Config::WINDOW_W / 2;
    int y0 = 140;
    int line_h = 44;

    app->menu_blink++;
    if (app->menu_blink >= Timing::MENU_CURSOR_BLINK) app->menu_blink = 0;
    bool cursor_visible = app->menu_blink < Timing::MENU_CURSOR_BLINK / 2;

    for (int i = 0; i < (int)MenuOption::Count; i++) {
        int y = y0 + i * line_h;
        bool selected = (MenuOption)i == app->menu_cursor;

        if (selected && cursor_visible) {
            SDL_SetRenderDrawColor(app->renderer, 255, 220, 80, 255);
            SDL_Rect hl = { cx - 182, y - 4, 364, 32 };
            SDL_RenderDrawRect(app->renderer, &hl);
        }

        int label_scale = 2;
        const char* label = s_menu_labels[i];
        int lw = text_width(label, label_scale);
        draw_text(app->renderer, cx - lw/2, y, label, label_scale,
                  (Uint8)(selected ? 255 : 200), (Uint8)(selected ? 255 : 200), 255, 255);
    }

    // Title at top
    int title_scale = 3;
    int tit_w = text_width("ULTRA SMASH ENGINE", title_scale);
    draw_text(app->renderer, cx - tit_w/2, 36, "ULTRA SMASH ENGINE", title_scale, 80, 120, 220, 255);
}

// ─── Phase advancement ────────────────────────────────────────────────────
static const int ROSTER_COLS = 4;

static void render_character_select(App* app) {
    SDL_SetRenderDrawColor(app->renderer, 12, 12, 24, 255);
    SDL_RenderClear(app->renderer);
    int cx = Config::WINDOW_W / 2;
    int scale = 2;
    draw_text(app->renderer, cx - text_width("SELECT YOUR FIGHTER", scale)/2, 24,
              "SELECT YOUR FIGHTER", scale, 255, 220, 100, 255);
    int cell_w = Config::WINDOW_W / (ROSTER_COLS + 1);
    int cell_h = 56;
    int y0 = 70;
    app->menu_blink++;
    if (app->menu_blink >= Timing::MENU_CURSOR_BLINK) app->menu_blink = 0;
    bool cursor_visible = app->menu_blink < Timing::MENU_CURSOR_BLINK / 2;
    for (int i = 0; i < (int)FighterID::Count; i++) {
        int col = i % ROSTER_COLS, row = i / ROSTER_COLS;
        int x = col * cell_w + cell_w / 2, y = y0 + row * cell_h;
        bool selected = (i == app->char_select_cursor);
        if (selected && cursor_visible) {
            SDL_SetRenderDrawColor(app->renderer, 255, 200, 60, 255);
            SDL_Rect hl = { x - 90, y - 6, 180, 28 };
            SDL_RenderDrawRect(app->renderer, &hl);
        }
        int nw = text_width(s_fighter_names[i], scale);
        draw_text(app->renderer, x - nw/2, y, s_fighter_names[i], scale,
                  (Uint8)(selected ? 255 : 180), (Uint8)(selected ? 255 : 200), 255, 255);
    }
    draw_text(app->renderer, 20, Config::WINDOW_H - 28, "ENTER CONFIRM  ESC BACK", 1, 120, 120, 140, 255);
}

static void render_stage_select(App* app) {
    SDL_SetRenderDrawColor(app->renderer, 10, 14, 28, 255);
    SDL_RenderClear(app->renderer);
    int cx = Config::WINDOW_W / 2;
    draw_text(app->renderer, cx - text_width("SELECT STAGE", 2)/2, 20, "SELECT STAGE", 2, 255, 220, 100, 255);
    app->menu_blink++;
    if (app->menu_blink >= Timing::MENU_CURSOR_BLINK) app->menu_blink = 0;
    bool cv = app->menu_blink < Timing::MENU_CURSOR_BLINK / 2;
    int y0 = 70;
    for (int i = 0; i < (int)StageID::Count; i++) {
        int y = y0 + i * 36;
        bool sel = (i == app->stage_select_cursor);
        if (sel && cv) {
            SDL_SetRenderDrawColor(app->renderer, 255, 200, 60, 255);
            SDL_Rect r = { cx - 150, y - 4, 300, 26 };
            SDL_RenderDrawRect(app->renderer, &r);
        }
        int nw = text_width(s_stage_names[i], 2);
        draw_text(app->renderer, cx - nw/2, y, s_stage_names[i], 2,
                  (Uint8)(sel ? 255 : 180), (Uint8)(sel ? 255 : 200), 255, 255);
    }
    draw_text(app->renderer, 20, Config::WINDOW_H - 28, "ENTER FIGHT  ESC BACK", 1, 120, 120, 140, 255);
}

static const float FIGHT_GRAVITY = 0.35f;
static const float FIGHT_GROUND_Y = 0;
static const float FIGHT_PLATFORM_LEFT = -120;
static const float FIGHT_PLATFORM_RIGHT = 120;
static const float FIGHT_BLAST_LEFT = -200;
static const float FIGHT_BLAST_RIGHT = 200;
static const float FIGHT_BLAST_TOP = 150;
static const float FIGHT_BLAST_BOTTOM = -120;
static const float FIGHT_JUMP_VY = -6.0f;
static const float FIGHT_WALK_SPEED = 2.2f;
static const float FIGHT_CHAR_R = 12.0f;
static const float FIGHT_HIT_RANGE = 40.0f;
static const int   FIGHT_HIT_DAMAGE = 8;

static int s_p1_attack_cooldown = 0;
static int s_p2_attack_cooldown = 0;

static void fight_attacks(App* app) {
    const Uint8* key = SDL_GetKeyboardState(nullptr);
    float dx = app->p1_x - app->p2_x;
    float dy = app->p1_y - app->p2_y;
    float dist = std::sqrt(dx*dx + dy*dy);

    if (s_p1_attack_cooldown > 0) s_p1_attack_cooldown--;
    if (key[SDL_SCANCODE_Z] && s_p1_attack_cooldown == 0 && dist < FIGHT_HIT_RANGE) {
        app->p2_damage += FIGHT_HIT_DAMAGE;
        if (app->p2_damage > 999) app->p2_damage = 999;
        s_sound.play_sfx(SfxId::Hit);
        s_p1_attack_cooldown = 25;
    }

    if (s_p2_attack_cooldown > 0) s_p2_attack_cooldown--;
    if (key[SDL_SCANCODE_G] && s_p2_attack_cooldown == 0 && dist < FIGHT_HIT_RANGE) {
        app->p1_damage += FIGHT_HIT_DAMAGE;
        if (app->p1_damage > 999) app->p1_damage = 999;
        s_sound.play_sfx(SfxId::Hit);
        s_p2_attack_cooldown = 25;
    }
}

static void fight_physics(App* app) {
    app->p1_vy += FIGHT_GRAVITY;
    app->p1_x += app->p1_vx;
    app->p1_y += app->p1_vy;
    app->p1_grounded = false;
    if (app->p1_y >= FIGHT_GROUND_Y && app->p1_x >= FIGHT_PLATFORM_LEFT && app->p1_x <= FIGHT_PLATFORM_RIGHT) {
        app->p1_y = FIGHT_GROUND_Y;
        app->p1_vy = 0;
        app->p1_grounded = true;
    }
    if (app->p1_x < FIGHT_BLAST_LEFT || app->p1_x > FIGHT_BLAST_RIGHT || app->p1_y < FIGHT_BLAST_BOTTOM || app->p1_y > FIGHT_BLAST_TOP) {
        app->p1_stocks--;
        s_sound.play_sfx(SfxId::KO);
        if (app->p1_stocks <= 0) { app->result_winner = 2; app->phase = Phase::Result; s_sound.play_sfx(SfxId::Result); return; }
        app->p1_x = -80; app->p1_y = 0; app->p1_vx = 0; app->p1_vy = 0;
    }

    app->p2_vy += FIGHT_GRAVITY;
    app->p2_x += app->p2_vx;
    app->p2_y += app->p2_vy;
    app->p2_grounded = false;
    if (app->p2_y >= FIGHT_GROUND_Y && app->p2_x >= FIGHT_PLATFORM_LEFT && app->p2_x <= FIGHT_PLATFORM_RIGHT) {
        app->p2_y = FIGHT_GROUND_Y;
        app->p2_vy = 0;
        app->p2_grounded = true;
    }
    if (app->p2_x < FIGHT_BLAST_LEFT || app->p2_x > FIGHT_BLAST_RIGHT || app->p2_y < FIGHT_BLAST_BOTTOM || app->p2_y > FIGHT_BLAST_TOP) {
        app->p2_stocks--;
        s_sound.play_sfx(SfxId::KO);
        if (app->p2_stocks <= 0) { app->result_winner = 1; app->phase = Phase::Result; s_sound.play_sfx(SfxId::Result); return; }
        app->p2_x = 80; app->p2_y = 0; app->p2_vx = 0; app->p2_vy = 0;
    }
    app->fight_timer++;
}

static void render_fight(App* app) {
    fight_attacks(app);
    fight_physics(app);

    draw_stage_gradient(app->renderer, app->current_stage);

    int cx = Config::WINDOW_W / 2, cy = Config::WINDOW_H / 2;
    float scale = 1.2f;
    int px1 = cx + (int)(app->p1_x * scale);
    int py1 = cy - (int)(app->p1_y * scale);
    int px2 = cx + (int)(app->p2_x * scale);
    int py2 = cy - (int)(app->p2_y * scale);

    int pl = cx + (int)(FIGHT_PLATFORM_LEFT * scale);
    int pr = cx + (int)(FIGHT_PLATFORM_RIGHT * scale);
    int py = cy - (int)(FIGHT_GROUND_Y * scale);
    SDL_Rect platform = { pl, py, pr - pl, 16 };
    SDL_SetRenderDrawColor(app->renderer, 60, 45, 30, 255);
    SDL_RenderFillRect(app->renderer, &platform);
    SDL_SetRenderDrawColor(app->renderer, 100, 80, 50, 255);
    SDL_RenderDrawRect(app->renderer, &platform);

    if (SpriteEngine::has_fighter_sheet()) {
        SpriteEngine::AnimState a1 = SpriteEngine::AnimState::Idle;
        if (s_p1_attack_cooldown > 20) a1 = SpriteEngine::AnimState::Attack;
        else if (!app->p1_grounded) a1 = app->p1_vy < 0 ? SpriteEngine::AnimState::Jump : SpriteEngine::AnimState::Fall;
        else if (std::fabs(app->p1_vx) > 0.1f) a1 = SpriteEngine::AnimState::Walk;
        int f1 = SpriteEngine::frame_from_state(a1, app->fight_timer);
        SpriteEngine::draw_sprite(app->renderer, SpriteEngine::s_fighter_sheet, f1, px1, py1, 1.5f, app->p1_vx < 0);

        SpriteEngine::AnimState a2 = SpriteEngine::AnimState::Idle;
        if (s_p2_attack_cooldown > 20) a2 = SpriteEngine::AnimState::Attack;
        else if (!app->p2_grounded) a2 = app->p2_vy < 0 ? SpriteEngine::AnimState::Jump : SpriteEngine::AnimState::Fall;
        else if (std::fabs(app->p2_vx) > 0.1f) a2 = SpriteEngine::AnimState::Walk;
        int f2 = SpriteEngine::frame_from_state(a2, app->fight_timer);
        SpriteEngine::draw_sprite(app->renderer, SpriteEngine::s_fighter_sheet, f2, px2, py2, 1.5f, app->p2_vx > 0);
    } else {
        SDL_SetRenderDrawColor(app->renderer, 255, 100, 100, 255);
        SDL_Rect p1_rect = { px1 - (int)FIGHT_CHAR_R, py1 - (int)FIGHT_CHAR_R, (int)(FIGHT_CHAR_R*2), (int)(FIGHT_CHAR_R*2) };
        SDL_RenderFillRect(app->renderer, &p1_rect);
        SDL_SetRenderDrawColor(app->renderer, 100, 100, 255, 255);
        SDL_Rect p2_rect = { px2 - (int)FIGHT_CHAR_R, py2 - (int)FIGHT_CHAR_R, (int)(FIGHT_CHAR_R*2), (int)(FIGHT_CHAR_R*2) };
        SDL_RenderFillRect(app->renderer, &p2_rect);
    }

    char buf[32];
    snprintf(buf, sizeof(buf), "P1 %d%%  %d", app->p1_damage, app->p1_stocks);
    draw_text(app->renderer, 20, 20, buf, 2, 255, 200, 200, 255);
    snprintf(buf, sizeof(buf), "P2 %d%%  %d", app->p2_damage, app->p2_stocks);
    draw_text(app->renderer, Config::WINDOW_W - 120, 20, buf, 2, 200, 200, 255, 255);

    const char* stage_name = s_stage_names[(int)app->current_stage];
    draw_text(app->renderer, cx - text_width(stage_name, 1)/2, Config::WINDOW_H - 20, stage_name, 1, 150, 150, 180, 255);
}

static void render_result(App* app) {
    SDL_SetRenderDrawColor(app->renderer, 8, 8, 16, 255);
    SDL_RenderClear(app->renderer);
    int cx = Config::WINDOW_W / 2, cy = Config::WINDOW_H / 2;
    if (app->result_winner == 1)
        draw_text(app->renderer, cx - text_width("PLAYER 1 WINS!", 3)/2, cy - 30, "PLAYER 1 WINS!", 3, 255, 200, 100, 255);
    else
        draw_text(app->renderer, cx - text_width("PLAYER 2 WINS!", 3)/2, cy - 30, "PLAYER 2 WINS!", 3, 100, 200, 255, 255);
    draw_text(app->renderer, cx - text_width("PRESS ENTER TO RETURN", 2)/2, cy + 30, "PRESS ENTER TO RETURN", 2, 150, 150, 170, 255);
}

static void advance_phase(App* app) {
    switch (app->phase) {
        case Phase::BootSplash: {
            int total = Timing::BOOT_SPLASH_FADE_IN + Timing::BOOT_SPLASH_HOLD + Timing::BOOT_SPLASH_FADE_OUT;
            if (app->frame >= total) {
                app->phase = Phase::BootCredits;
                app->frame = 0;
            }
            break;
        }
        case Phase::BootCredits: {
            int total = Timing::BOOT_CREDITS_FADE_IN + Timing::BOOT_CREDITS_HOLD + Timing::BOOT_CREDITS_FADE_OUT;
            if (app->frame >= total) {
                app->phase = Phase::IntroTitle;
                app->frame = 0;
            }
            break;
        }
        case Phase::IntroTitle:
            if (app->frame >= Timing::INTRO_TITLE_ZOOM_FRAMES) {
                app->phase = Phase::IntroHold;
                app->frame = 0;
            }
            break;
        case Phase::IntroHold:
            if (app->frame >= Timing::INTRO_HOLD_FRAMES) {
                app->phase = Phase::MainMenu;
                app->frame = 0;
                s_sound.play_music_ost("music/menu.ogg");
            }
            break;
        case Phase::MainMenu:
        case Phase::CharacterSelect:
        case Phase::StageSelect:
        case Phase::Fight:
        case Phase::Result:
        case Phase::Exit:
            break;
    }
}

// ─── Input ─────────────────────────────────────────────────────────────────
static void process_input(App* app) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            app->running = false;
            app->phase = Phase::Exit;
            return;
        }
        if (e.type == SDL_KEYDOWN) {
            if (app->phase == Phase::MainMenu) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        app->menu_cursor = (MenuOption)(((int)app->menu_cursor + (int)MenuOption::Count - 1) % (int)MenuOption::Count);
                        s_sound.play_sfx(SfxId::MenuMove);
                        break;
                    case SDLK_DOWN:
                        app->menu_cursor = (MenuOption)(((int)app->menu_cursor + 1) % (int)MenuOption::Count);
                        s_sound.play_sfx(SfxId::MenuMove);
                        break;
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                        if (app->menu_cursor == MenuOption::ExitGame) {
                            app->running = false;
                            app->phase = Phase::Exit;
                        } else if (app->menu_cursor == MenuOption::OnePlayer ||
                                   app->menu_cursor == MenuOption::TwoPlayer ||
                                   app->menu_cursor == MenuOption::Versus) {
                            s_sound.play_sfx(SfxId::MenuSelect);
                            app->phase = Phase::CharacterSelect;
                            app->char_select_mode = app->menu_cursor;
                            app->char_select_cursor = 0;
                            app->versus_select_step = 0;
                        }
                        break;
                    default:
                        break;
                }
            } else if (app->phase == Phase::CharacterSelect) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        app->phase = Phase::MainMenu;
                        break;
                    case SDLK_LEFT:
                        app->char_select_cursor = (app->char_select_cursor + (int)FighterID::Count - 1) % (int)FighterID::Count;
                        s_sound.play_sfx(SfxId::MenuMove);
                        break;
                    case SDLK_RIGHT:
                        app->char_select_cursor = (app->char_select_cursor + 1) % (int)FighterID::Count;
                        s_sound.play_sfx(SfxId::MenuMove);
                        break;
                    case SDLK_UP:
                        app->char_select_cursor = (app->char_select_cursor - ROSTER_COLS + (int)FighterID::Count) % (int)FighterID::Count;
                        s_sound.play_sfx(SfxId::MenuMove);
                        break;
                    case SDLK_DOWN:
                        app->char_select_cursor = (app->char_select_cursor + ROSTER_COLS) % (int)FighterID::Count;
                        s_sound.play_sfx(SfxId::MenuMove);
                        break;
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                        s_sound.play_sfx(SfxId::MenuSelect);
                        app->selected_fighter = (FighterID)app->char_select_cursor;
                        if (app->char_select_mode == MenuOption::OnePlayer) {
                            app->p1_fighter = app->selected_fighter;
                            app->phase = Phase::StageSelect;
                            app->stage_select_cursor = 0;
                        } else {
                            if (app->versus_select_step == 0) {
                                app->p1_fighter = app->selected_fighter;
                                app->versus_select_step = 1;
                            } else {
                                app->p2_fighter = app->selected_fighter;
                                app->phase = Phase::StageSelect;
                                app->stage_select_cursor = 0;
                            }
                        }
                        break;
                    default:
                        break;
                }
            } else if (app->phase == Phase::StageSelect) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        app->phase = Phase::MainMenu;
                        app->versus_select_step = 0;
                        break;
                    case SDLK_LEFT:
                        app->stage_select_cursor = (app->stage_select_cursor + (int)StageID::Count - 1) % (int)StageID::Count;
                        s_sound.play_sfx(SfxId::MenuMove);
                        break;
                    case SDLK_RIGHT:
                        app->stage_select_cursor = (app->stage_select_cursor + 1) % (int)StageID::Count;
                        s_sound.play_sfx(SfxId::MenuMove);
                        break;
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                        s_sound.play_sfx(SfxId::MenuSelect);
                        app->current_stage = (StageID)app->stage_select_cursor;
                        app->phase = Phase::Fight;
                        app->p1_x = -80; app->p1_y = 0; app->p1_vx = 0; app->p1_vy = 0; app->p1_damage = 0; app->p1_stocks = 3;
                        app->p2_x = 80;  app->p2_y = 0; app->p2_vx = 0; app->p2_vy = 0; app->p2_damage = 0; app->p2_stocks = 3;
                        app->fight_timer = 0;
                        break;
                    default:
                        break;
                }
            } else if (app->phase == Phase::Result) {
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_ESCAPE) {
                    app->phase = Phase::MainMenu;
                    app->versus_select_step = 0;
                }
            }
        }
    }

    if (app->phase == Phase::Fight) {
        const Uint8* key = SDL_GetKeyboardState(nullptr);
        app->p1_vx = 0;
        if (key[SDL_SCANCODE_LEFT])  app->p1_vx = -FIGHT_WALK_SPEED;
        if (key[SDL_SCANCODE_RIGHT]) app->p1_vx =  FIGHT_WALK_SPEED;
        if (key[SDL_SCANCODE_UP] && app->p1_grounded) {
            app->p1_vy = FIGHT_JUMP_VY;
            s_sound.play_sfx(SfxId::Jump);
        }
        app->p2_vx = 0;
        if (key[SDL_SCANCODE_A]) app->p2_vx = -FIGHT_WALK_SPEED;
        if (key[SDL_SCANCODE_D]) app->p2_vx =  FIGHT_WALK_SPEED;
        if (key[SDL_SCANCODE_W] && app->p2_grounded) {
            app->p2_vy = FIGHT_JUMP_VY;
            s_sound.play_sfx(SfxId::Jump);
        }
    }
}

// ─── Render dispatch ───────────────────────────────────────────────────────
static void render(App* app) {
    switch (app->phase) {
        case Phase::BootSplash:   render_boot_splash(app); break;
        case Phase::BootCredits:  render_boot_credits(app); break;
        case Phase::IntroTitle:   render_intro_title(app); break;
        case Phase::IntroHold:    render_intro_hold(app); break;
        case Phase::MainMenu:     render_main_menu(app); break;
        case Phase::CharacterSelect: render_character_select(app); break;
        case Phase::StageSelect:  render_stage_select(app); break;
        case Phase::Fight:        render_fight(app); break;
        case Phase::Result:      render_result(app); break;
        case Phase::Exit:         break;
    }
}

// ─── Main loop ──────────────────────────────────────────────────────────────
int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    s_sound.init();

    App app = {};
    app.window = SDL_CreateWindow(
        "UltraSmashEngine4k 1.x",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        Config::WINDOW_W, Config::WINDOW_H,
        SDL_WINDOW_SHOWN
    );
    if (!app.window) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app.renderer) {
        app.renderer = SDL_CreateRenderer(app.window, -1, 0);
    }
    if (!app.renderer) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        return 1;
    }

    SpriteEngine::load_fighter_sheet(app.renderer);

    Uint32 last = SDL_GetTicks();

    while (app.running && app.phase != Phase::Exit) {
        process_input(&app);
        advance_phase(&app);
        render(&app);
        SDL_RenderPresent(app.renderer);
        app.frame++;

        Uint32 now = SDL_GetTicks();
        int elapsed = (int)(now - last);
        if (elapsed < Config::MS_PER_FRAME) {
            SDL_Delay((Uint32)(Config::MS_PER_FRAME - elapsed));
        }
        last = SDL_GetTicks();
    }

    SpriteEngine::shutdown();
    s_sound.shutdown();
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
    return 0;
}
