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
        KC_MUTE, KC_ESC, KC_LALT, KC_LGUI, KC_LNG2, KC_SPC, KC_LNG1, KC_RGUI, LOWER,   RAISE,  ADJUST,  KC_RALT
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

/* 以下の形式でもダメ....
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0]   = { {KC_DOWN, KC_UP},             {KC_VOLD, KC_VOLU}  },
    [1]   = { {KC_DOWN, KC_UP},             {KC_VOLD, KC_VOLU}  },
    [2]   = { {KC_DOWN, KC_UP},             {KC_VOLD, KC_VOLU}  },
    [3]   = { {KC_DOWN, KC_UP},             {KC_VOLD, KC_VOLU}  },
};
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)) },
    [1] = { ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)) },
    [2] = { ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)) },
    [3] = { ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)), ENCODER_CCW_CW(C(KC_DOWN), C(KC_UP)) },
};


 */

// こいつの設定を渡せないので困っている
// こいつが桁が足りなかった...!!!!
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    //
    //           コンパイラ的には                                                                          3番目                                                                                                                   7番目
    //           VIALの画面的には3番目          VIALの画面的には7番目
    [0] = { ENCODER_CCW_CW(KC_A, KC_S), ENCODER_CCW_CW(KC_D, KC_F), ENCODER_CCW_CW(KC_G, KC_H), ENCODER_CCW_CW(KC_J, KC_K), ENCODER_CCW_CW(KC_L, KC_Z), ENCODER_CCW_CW(KC_X, KC_C), ENCODER_CCW_CW(KC_V, KC_B),  ENCODER_CCW_CW(KC_N, KC_M) },
    [1] = { ENCODER_CCW_CW(KC_A, KC_S), ENCODER_CCW_CW(KC_D, KC_F), ENCODER_CCW_CW(KC_G, KC_H), ENCODER_CCW_CW(KC_J, KC_K), ENCODER_CCW_CW(KC_L, KC_Z), ENCODER_CCW_CW(KC_X, KC_C), ENCODER_CCW_CW(KC_V, KC_B),  ENCODER_CCW_CW(KC_N, KC_M) },
    [2] = { ENCODER_CCW_CW(KC_A, KC_S), ENCODER_CCW_CW(KC_D, KC_F), ENCODER_CCW_CW(KC_G, KC_H), ENCODER_CCW_CW(KC_J, KC_K), ENCODER_CCW_CW(KC_L, KC_Z), ENCODER_CCW_CW(KC_X, KC_C), ENCODER_CCW_CW(KC_V, KC_B),  ENCODER_CCW_CW(KC_N, KC_M) },
    [3] = { ENCODER_CCW_CW(KC_A, KC_S), ENCODER_CCW_CW(KC_D, KC_F), ENCODER_CCW_CW(KC_G, KC_H), ENCODER_CCW_CW(KC_J, KC_K), ENCODER_CCW_CW(KC_L, KC_Z), ENCODER_CCW_CW(KC_X, KC_C), ENCODER_CCW_CW(KC_V, KC_B),  ENCODER_CCW_CW(KC_N, KC_M) },
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
