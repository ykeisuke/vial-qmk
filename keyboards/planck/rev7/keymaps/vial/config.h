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

#pragma once

#ifdef AUDIO_ENABLE
#    define STARTUP_SONG SONG(WORKMAN_SOUND)
#    define DEFAULT_LAYER_SONGS \
        { SONG(QWERTY_SOUND), SONG(COLEMAK_SOUND), SONG(DVORAK_SOUND) }
#endif

/*
 * MIDI options
 */

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
// #define MIDI_ADVANCED

// VIAL向けデフォルト設定
#define VIAL_KEYBOARD_UID {0x29, 0xA3, 0xE1, 0x59, 0xF2, 0xA6, 0xB5, 0x65}
#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

// マウスボタンの感度関連
#define MOUSEKEY_WHEEL_INTERVAL 50
#define MOUSEKEY_WHEEL_DELTA 2

/*
 * Encoder options
 */
#define PLANCK_ENCODER_SETTLE_PIN_STATE_DELAY 11
#define ENCODER_MAP_KEY_DELAY 10
#define ENCODER_RESOLUTION 4

// #define ACTION_DEBUG 1
// #define TAP_CODE_DELAY 100
