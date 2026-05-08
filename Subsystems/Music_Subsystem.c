#include <stdio.h>
// #include <math.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "Multithreading.h"

#include "Subsystems/Drive_Subsystem.h"

#include "Subsystems/Music_Subsystem.h"
#include "Subsystems/songs.c"


uint getWholeNoteDuration(uint8_t bpm, float time_signature){
    return (uint) ((1000.0 * 60000.0 * time_signature / bpm));
}

void playNote(uint16_t note, uint8_t level, struct Motor* motor){
    if (note > 25.0){
        uint16_t pwm_f = note;
        // const uint16_t pwm_wrap = 999;
        // float clkdiv = 150000000.0 / (float)((pwm_wrap + 1) * pwm_f);

        const float clkdiv = 100.0;
        uint16_t pwm_wrap = (150000000.0 / (float)(pwm_f * clkdiv)) - 1;
        pwm_set_wrap(motor->pwmSliceFWD, pwm_wrap);

        if (note < 90.0){
            pwm_set_both_levels(motor->pwmSliceFWD, pwm_wrap * level * 1.03 / 100, pwm_wrap);
            // pwm_set_gpio_level(motor->pinFWD, pwm_wrap * 16 / 16);
            // pwm_set_gpio_level(motor->pinRVS, pwm_wrap + 1);
        } else {
            pwm_set_both_levels(motor->pwmSliceFWD, pwm_wrap * level / 100, pwm_wrap);
            // pwm_set_gpio_level(motor->pinFWD, pwm_wrap * 6 / 6);
            // pwm_set_gpio_level(motor->pinRVS, pwm_wrap + 1);
        }
    } else {
        pwm_set_both_levels(motor->pwmSliceFWD, 0, 0);
    }
}

static const struct Music songs[] = {
    {   // 0
        .bpm = 120,
        .time_signature = 4.0,
        .notesL = sweep_notes,
        .notesR = sweep_notes,
        .notesDuration = sweep_durations,
        .note_percent = 0.87,
        .length = sizeof(sweep_notes) / sizeof(uint16_t)
    },
    {   // 1
        .bpm = 120,
        .time_signature = 3.0,
        .notesL = anthem_melody,
        .notesR = anthem_bass,
        .notesDuration = anthem_durations,
        .note_percent = 0.99,
        .length = sizeof(anthem_durations) / sizeof(uint8_t)
    },
    {   // 2
        .bpm = 180,
        .time_signature = 3.0,
        .notesL = pirate_melody,
        .notesR = pirate_bass,
        .notesDuration = pirate_durations,
        .note_percent = 0.91, // Keeps it tight and driving
        .length = sizeof(pirate_durations) / sizeof(uint8_t)
    },
    {   // 3
        .bpm = 190,
        .time_signature = 4.0,
        .notesL = mario_melody,
        .notesR = mario_bass,
        .notesDuration = mario_durations,
        .note_percent = 0.87,
        .length = sizeof(mario_durations) / sizeof(uint8_t)
    },
    {   // 4
        .bpm = 125,
        .time_signature = 4.0,
        .notesL = zelda_melody,
        .notesR = zelda_bass,
        .notesDuration = zelda_durations,
        .note_percent = 0.90,
        .length = sizeof(zelda_melody) / sizeof(uint16_t)
    },
    {   // 5
        .bpm = 120, // Gentle, steady tempo
        .time_signature = 4.0,
        .notesL = pokemon_melody,
        .notesR = pokemon_harmony,
        .notesDuration = pokemon_durations,
        .note_percent = 0.87,
        .length = sizeof(pokemon_durations) / sizeof(uint8_t)
    },
    {   // 6
        .bpm = 180, // High speed for battle tension!
        .time_signature = 4.0,
        .notesL = pokemon_battle_melody,
        .notesR = pokemon_battle_bass,
        .notesDuration = pokemon_battle_durations,
        .note_percent = 0.87,
        .length = sizeof(pokemon_battle_durations) / sizeof(uint8_t)
    },
    {   // 7
        .bpm = 240, // Standard BPM
        .time_signature = 4.0,
        .notesL = megalo_melody,
        .notesR = megalo_bass,
        .notesDuration = megalo_durations,
        .note_percent = 0.90,
        .length = sizeof(megalo_durations) / sizeof(uint8_t)
    },
    {   // 8
        .bpm = 104, // Smooth pop tempo
        .time_signature = 4.0,
        .notesL = espresso_melody,
        .notesR = espresso_bass,
        .notesDuration = espresso_durations,
        .note_percent = 0.87,
        .length = sizeof(espresso_durations) / sizeof(uint8_t)
    },
    {   // 9
        .bpm = 149, // Classic Tetris speed
        .time_signature = 4.0,
        .notesL = tetris_melody,
        .notesR = tetris_bass,
        .notesDuration = tetris_durations,
        .note_percent = 0.85,
        .length = sizeof(tetris_durations) / sizeof(uint8_t)
    },
    {
        .bpm = 90, // Slow, epic tempo
        .time_signature = 4.0,
        .notesL = halo_melody,
        .notesR = halo_bass,
        .notesDuration = halo_durations,
        .note_percent = 0.87,
        .length = sizeof(halo_durations) / sizeof(uint8_t)
    },
    {
        .bpm = 114, 
        .time_signature = 4.0,
        .notesL = rick_melody,
        .notesR = rick_bass,
        .notesDuration = rick_durations,
        .note_percent = 0.75,
        .length = sizeof(rick_durations) / sizeof(uint8_t)
    }
};

void playMusic(uint8_t songNum){
    if (songNum >= (sizeof(songs) / sizeof(songs[1]))) return;
    core1CurrentMode = Music_mode;

    global_drivetrain->mode = Music;

    // Get song and whole note timing
    struct Music song = songs[songNum];
    uint wholeNoteTime = getWholeNoteDuration(song.bpm, song.time_signature);
    uint activeTime = (uint)(wholeNoteTime * song.note_percent);
    uint restTime = (uint)((wholeNoteTime * (1.0 - song.note_percent)));

    const float clkdiv = 100.0;
    pwm_set_clkdiv(global_drivetrain->motorL->pwmSliceFWD, clkdiv);
    pwm_set_clkdiv(global_drivetrain->motorR->pwmSliceFWD, clkdiv);

    // Play song
    for (int i = 0; i < song.length; i++){
        printf("Playing Note: %d, %d\n", song.notesL[i], song.notesR[i]);
        playNote(song.notesL[i]*4, 78, global_drivetrain->motorL); // 84
        playNote(song.notesR[i]*4, 84, global_drivetrain->motorR); // 92
        sleep_us(activeTime / song.notesDuration[i]);
        playNote(0, 100, global_drivetrain->motorL);
        playNote(0, 100, global_drivetrain->motorR);
        sleep_us(restTime / song.notesDuration[i]);
    }

    // Restore Motors to drive mode
    setDrivePWM(global_drivetrain->motorL);
    setDrivePWM(global_drivetrain->motorR);
    global_drivetrain->mode = Drive_mode;
}