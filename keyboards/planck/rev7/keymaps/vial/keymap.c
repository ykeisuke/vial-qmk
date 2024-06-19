/* Copyright 2015-2023 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "gpio.h"

enum planck_layers { _QWERTY, _LOWER, _RAISE, _ADJUST };

enum planck_keycodes { QWERTY = SAFE_RANGE, BACKLIT, EXT_PLV };

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define ADJUST MO(_ADJUST)

/* clang-format off */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Qwerty
    */
    [_QWERTY] = LAYOUT_planck_grid(
        KC_TAB,  KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,   KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,    KC_BSPC,
        KC_LCTL, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,   KC_H,    KC_J,    KC_K,    KC_L,   KC_SCLN, KC_ENT,
        KC_LSFT, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,   KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
        KC_MUTE, KC_ESC, KC_LALT, KC_LGUI, KC_LNG2, KC_SPC, LOWER, KC_LNG1,   KC_RGUI, RAISE,  KC_RALT, KC_MUTE
    ),

    /* Lowe
    */
    [_LOWER] = LAYOUT_planck_grid(
        KC_GRV,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_DEL,
        KC_LCTL, KC_PLUS, KC_MINS, KC_PAST, KC_PSLS, KC_PDOT, KC_INT3, KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,
        KC_LSFT, _______, _______, _______, _______, _______, KC_SCLN, KC_QUOT, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
        _______, _______, _______, _______, _______, _______, _______, KC_INT3, _______, _______, _______, _______
    ),

    /* Raise
    */
    [_RAISE] = LAYOUT_planck_grid(
        KC_GRV,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_DEL,
        KC_LCTL, KC_PLUS, KC_MINS, KC_PAST, KC_PSLS, KC_PDOT, KC_INT3, KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,
        KC_LSFT, _______, _______, _______, _______, _______, KC_SCLN, KC_QUOT, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
        _______, _______, _______, _______, _______, _______, _______, KC_INT3, _______, _______, _______, _______
    ),

    /* Adjust
    */
    [_ADJUST] = LAYOUT_planck_grid(
        _______, _______, _______, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,  KC_DEL,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    )

};
/* clang-format on */

#ifdef ENCODER_MAP_ENABLE
/* Rotary Encoders
 */
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    /* Qwerty
     *    v- (index) Clockwise / Counter Clockwise                        v- (index) Clockwise / Counter Clockwise
     * ,---------------------------------------------------------------------------------------.
     * | (0) Vol-    / Vol+    |   |   |   |   |   |   |   |   |   |   | (4) Vol-    / Vol+    |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (1) KC_MNXT / KC_MPRV |   |   |   |   |   |   |   |   |   |   | (5) KC_MNXT / KC_MPRV |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (2) KC_WBAK / KC_WFWD |   |   |   |   |   |   |   |   |   |   | (6) KC_SPC  / KC_ENT  |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (3) KC_LEFT / KC_RGHT |   |   |   |   |       |   |   |   |   | (7) KC_DOWN / KC_UP   |
     * `---------------------------------------------------------------------------------------'
     */
    [_QWERTY] = {
        // LEFT SIDE (index 0 to 3)
        ENCODER_CCW_CW(KC_VOLU, KC_VOLD),
        ENCODER_CCW_CW(KC_MNXT, KC_MPRV),
        ENCODER_CCW_CW(KC_WBAK, KC_WFWD),
        ENCODER_CCW_CW(KC_LEFT, KC_RGHT),
        // RIGHT SIDE (index 4 to 7)
        ENCODER_CCW_CW(KC_VOLU, KC_VOLD),
        ENCODER_CCW_CW(KC_MNXT, KC_MPRV),
        ENCODER_CCW_CW(KC_SPC,  KC_ENT),
        ENCODER_CCW_CW(KC_DOWN, KC_UP)
    },

    /* Adjust (Lower + Raise)
     *    v- (index) Clockwise / Counter Clockwise                        v- (index) Clockwise / Counter Clockwise
     * ,---------------------------------------------------------------------------------------.
     * | (0) _______ / _______ |   |   |   |   |   |   |   |   |   |   | (4) _______ / _______ |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (1) _______ / _______ |   |   |   |   |   |   |   |   |   |   | (5) _______ / _______ |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (2) RGB_MOD / RGB_RMOD|   |   |   |   |   |   |   |   |   |   | (6) SAT- / SAT+       |
     * |-----------------------+---+---+---+---+---+---+---+---+---+---+-----------------------|
     * | (3) BRGTH- / BRGTH+   |   |   |   |   |       |   |   |   |   | (7) HUE- / HUE+       |
     * `---------------------------------------------------------------------------------------'
     */
    [_ADJUST] = {
        // LEFT SIDE (index 0 to 3)
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(RGB_MOD, RGB_RMOD),
        ENCODER_CCW_CW(RGB_VAD, RGB_VAI),
        // RIGHT SIDE (index 4 to 7)
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(RGB_SAD,  RGB_SAI),
        ENCODER_CCW_CW(RGB_HUD, RGB_HUI)
    }
};
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}


#ifdef AUDIO_ENABLE
float plover_song[][2]    = SONG(PLOVER_SOUND);
float plover_gb_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
#endif


/* clang-format off */
float melody[8][2][2] = {
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
    {{440.0f, 8}, {440.0f, 24}},
};
/* clang-format on */

#define JUST_MINOR_THIRD 1.2
#define JUST_MAJOR_THIRD 1.25
#define JUST_PERFECT_FOURTH 1.33333333
#define JUST_TRITONE 1.42222222
#define JUST_PERFECT_FIFTH 1.33333333

#define ET12_MINOR_SECOND 1.059463
#define ET12_MAJOR_SECOND 1.122462
#define ET12_MINOR_THIRD 1.189207
#define ET12_MAJOR_THIRD 1.259921
#define ET12_PERFECT_FOURTH 1.33484
#define ET12_TRITONE 1.414214
#define ET12_PERFECT_FIFTH 1.498307

deferred_token tokens[8];

uint32_t reset_note(uint32_t trigger_time, void *note) {
    *(float*)note = 440.0f;
    return 0;
}

bool play_encoder_melody(uint8_t index, bool clockwise) {
    cancel_deferred_exec(tokens[index]);
    if (clockwise) {
        melody[index][1][0] = melody[index][1][0] * ET12_MINOR_SECOND;
        melody[index][0][0] = melody[index][1][0] / ET12_PERFECT_FIFTH;
        audio_play_melody(&melody[index], 2, false);
    } else {
        melody[index][1][0] = melody[index][1][0] / ET12_MINOR_SECOND;
        melody[index][0][0] = melody[index][1][0] * ET12_TRITONE;
        audio_play_melody(&melody[index], 2, false);
    }
    tokens[index] = defer_exec(1000, reset_note, &melody[index][1][0]);
    return false;
}




bool encoder_update_user(uint8_t index, bool clockwise) {
    return play_encoder_melody(index, clockwise);
}

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) {
                    PLAY_SONG(plover_song);
                }
#endif
                layer_on(_ADJUST);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) {
                    PLAY_SONG(plover_gb_song);
                }
#endif
                layer_off(_ADJUST);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
    }
    return true;
}

// レイヤーに応じてLEDの色を変更する


bool rgb_matrix_indicators_user(void) {
    // デフォルトの色
    HSV hsv = (HSV){81, 51, 28};
    switch (get_highest_layer(layer_state)) {
        case 1:
            // レイヤー1の色
            hsv = (HSV){22, 255, 255};

            break;
        case 2:
            hsv = (HSV){106, 255, 255};
            break;
        case 3:
            //
            hsv = (HSV){0, 92, 76};
            break;
        default:
//            hsv = (HSV){150, 100, 255};
            break;
    }
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }

    RGB rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(3, rgb.r, rgb.g, rgb.b);
    rgb_matrix_set_color(4, rgb.r, rgb.g, rgb.b);
    rgb_matrix_set_color(5, rgb.r, rgb.g, rgb.b);
    rgb_matrix_set_color(6, rgb.r, rgb.g, rgb.b);

    return true;
}
