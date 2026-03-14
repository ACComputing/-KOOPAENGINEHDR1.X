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
 * - App state (window, renderer, phase, frame, menu cursor)
 * - Easing helpers, embedded 8x8 font, draw_text
 * - render_boot_splash(), render_boot_credits() — boot screens
 * - render_intro_title(), render_intro_hold() — title reveal
 * - render_main_menu() — cursor menu (1P, 2P, Versus, Options, Exit)
 * - advance_phase(), process_input(), render(), main()
 *
 * BUILD: g++ -std=c++17 -O2 -o ultrasmashengine4k ultrasmashengine4k.cpp $(pkg-config --cflags --libs sdl2)
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

// ─── Configuration ────────────────────────────────────────────────────────
namespace Config {
    const int WINDOW_W     = 640;
    const int WINDOW_H     = 480;
    const int TARGET_FPS   = 60;
    const int MS_PER_FRAME = 1000 / TARGET_FPS;
}

// ─── Boot / Intro / Menu phases ─────────────────────────────────────────────
enum class Phase {
    BootSplash,
    BootCredits,
    IntroTitle,
    IntroHold,
    MainMenu,
    CharacterSelect,
    FightReady,
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

static void render_fight_ready(App* app) {
    SDL_SetRenderDrawColor(app->renderer, 8, 8, 16, 255);
    SDL_RenderClear(app->renderer);
    int cx = Config::WINDOW_W / 2, cy = Config::WINDOW_H / 2;
    int scale = 3;
    draw_text(app->renderer, cx - text_width("READY!", scale)/2, cy - 60, "READY!", scale, 100, 255, 100, 255);
    const char* name = s_fighter_names[(int)app->selected_fighter];
    draw_text(app->renderer, cx - text_width(name, scale)/2, cy - 10, name, scale, 255, 255, 255, 255);
    draw_text(app->renderer, cx - text_width("PRESS ENTER TO RETURN", 2)/2, cy + 50,
              "PRESS ENTER TO RETURN", 2, 150, 150, 170, 255);
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
            }
            break;
        case Phase::MainMenu:
        case Phase::CharacterSelect:
        case Phase::FightReady:
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
                        break;
                    case SDLK_DOWN:
                        app->menu_cursor = (MenuOption)(((int)app->menu_cursor + 1) % (int)MenuOption::Count);
                        break;
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                        if (app->menu_cursor == MenuOption::ExitGame) {
                            app->running = false;
                            app->phase = Phase::Exit;
                        } else if (app->menu_cursor == MenuOption::OnePlayer ||
                                   app->menu_cursor == MenuOption::TwoPlayer ||
                                   app->menu_cursor == MenuOption::Versus) {
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
                        break;
                    case SDLK_RIGHT:
                        app->char_select_cursor = (app->char_select_cursor + 1) % (int)FighterID::Count;
                        break;
                    case SDLK_UP:
                        app->char_select_cursor = (app->char_select_cursor - ROSTER_COLS + (int)FighterID::Count) % (int)FighterID::Count;
                        break;
                    case SDLK_DOWN:
                        app->char_select_cursor = (app->char_select_cursor + ROSTER_COLS) % (int)FighterID::Count;
                        break;
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                        app->selected_fighter = (FighterID)app->char_select_cursor;
                        if (app->versus_select_step == 0) {
                            app->p1_fighter = app->selected_fighter;
                            app->versus_select_step = 1;
                            if (app->char_select_mode == MenuOption::OnePlayer) {
                                app->phase = Phase::FightReady;
                            }
                        } else {
                            app->p2_fighter = app->selected_fighter;
                            app->selected_fighter = app->p1_fighter;
                            app->phase = Phase::FightReady;
                        }
                        if (app->char_select_mode == MenuOption::OnePlayer)
                            app->phase = Phase::FightReady;
                        else if (app->char_select_mode == MenuOption::TwoPlayer || app->char_select_mode == MenuOption::Versus) {
                            if (app->versus_select_step == 0) {
                                app->p1_fighter = app->selected_fighter;
                                app->versus_select_step = 1;
                            } else {
                                app->p2_fighter = app->selected_fighter;
                                app->phase = Phase::FightReady;
                            }
                        }
                        break;
                    default:
                        break;
                }
            } else if (app->phase == Phase::FightReady) {
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_ESCAPE) {
                    app->phase = Phase::MainMenu;
                    app->versus_select_step = 0;
                }
            }
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
        case Phase::FightReady:   render_fight_ready(app); break;
        case Phase::Exit:         break;
    }
}

// ─── Main loop ──────────────────────────────────────────────────────────────
int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

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

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
    return 0;
}
