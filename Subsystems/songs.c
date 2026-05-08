static const uint16_t sweep_notes[] = {
    REST,
    NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
    NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
    NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5
};

// We use a constant duration for the sweep
static const uint8_t sweep_durations[] = {
    4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};

// const struct Music test_sweep = {
//     .bpm = 120,
//     .notes = sweep_notes,
//     .notesDuration = sweep_durations,
//     .length = sizeof(sweep_notes) / sizeof(uint16_t)
// };

// Melody for Motor L (Trumpet Lead - Octave 5/6)
static const uint16_t anthem_melody[] = {
    // Oh, say can you see...
    NOTE_G5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6,
    // By the dawn's early light...
    NOTE_E6, NOTE_D6, NOTE_C6, NOTE_E5, NOTE_FS5, NOTE_G5,
    // What so proudly we hailed...
    NOTE_G5, NOTE_G5, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_B5,
    // At the twilight's last gleaming...
    NOTE_A5, NOTE_B5, NOTE_C6, NOTE_C6, NOTE_G5, NOTE_E5, NOTE_C5,
    
    // The rocket's red glare...
    NOTE_E6, NOTE_E6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_G6,
    // The bombs bursting in air...
    NOTE_F6, NOTE_E6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_F6,
    
    // Gave proof through the night...
    NOTE_F6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_B5,
    // That our flag was still there...
    NOTE_A5, NOTE_B5, NOTE_C6, NOTE_E5, NOTE_FS5, NOTE_G5,
    
    // Oh, say does that star-spangled...
    NOTE_G5, NOTE_C6, NOTE_C6, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_A5, NOTE_A5,
    // Banner yet wave...
    NOTE_D6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_C6, NOTE_B5,
    
    // O'er the land of the free...
    NOTE_G5, NOTE_G5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6,
    // And the home of the brave?
    NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_D6, NOTE_C6
};

// Harmony for Motor R (Low Brass - Octave 3)
static const uint16_t anthem_bass[] = {
    NOTE_C3, NOTE_C3, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_C4,
    NOTE_C4, NOTE_G3, NOTE_E3, NOTE_C3, NOTE_D3, NOTE_G2,
    NOTE_G3, NOTE_G3, NOTE_C4, NOTE_G3, NOTE_E3, NOTE_G3,
    NOTE_F3, NOTE_G3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3,
    
    NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4,
    NOTE_B3, NOTE_B3, NOTE_B3, NOTE_B3, NOTE_B3, NOTE_B3,
    
    NOTE_A3, NOTE_A3, NOTE_A3, NOTE_E3, NOTE_G3,
    NOTE_F3, NOTE_G3, NOTE_C3, NOTE_C3, NOTE_D3, NOTE_G2,
    
    NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_F3, NOTE_F3, NOTE_F3,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_G3, NOTE_G3, NOTE_G3,
    
    NOTE_G3, NOTE_G3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3,
    NOTE_C4, NOTE_G3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_C3
};

// Durations (3/4 Time Signature - 120 BPM)
static const uint8_t anthem_durations[] = {
    6, 12, 4, 4, 4, 2,
    6, 12, 4, 4, 4, 2,
    8, 8, 4, 4, 4, 2,
    8, 8, 4, 4, 8, 8, 2,
    
    3, 6, 6, 3, 3, 1,
    3, 6, 6, 3, 3, 1,
    
    4, 4, 4, 4, 2,
    8, 8, 4, 8, 8, 2,
    
    4, 4, 8, 8, 8, 8, 4, 4,
    4, 4, 8, 8, 8, 8, 2,
    
    4, 8, 8, 8, 8, 8, 1,
    4, 4, 8, 8, 4, 1
};

// Motor L: Lead Melody (Full Performance)
static const uint16_t pirate_melody[] = {
    // --- INTRO CHUG ---
    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,

    // --- THE HOOK (Pass 1) ---
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
    NOTE_F5, NOTE_G5, NOTE_E5, NOTE_E5, REST, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_D5, REST,
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
    NOTE_F5, NOTE_G5, NOTE_E5, NOTE_E5, REST, NOTE_D5, NOTE_C5, NOTE_D5, REST,

    // --- THE HOOK (Pass 2) ---
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
    NOTE_F5, NOTE_G5, NOTE_E5, NOTE_E5, REST, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_D5, REST,
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST, NOTE_D5, NOTE_F5, NOTE_G5, NOTE_G5, REST,
    NOTE_G5, NOTE_A5, NOTE_AS5, NOTE_AS5, REST, NOTE_A5, NOTE_G5, NOTE_A5, NOTE_D5, REST,

    // --- THE BRIDGE ---
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_D5, REST,
    NOTE_D5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_D5, NOTE_E5, REST,
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_D5, REST,
    NOTE_D5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_D5, NOTE_E5, REST,

    // --- THE CHASE (High Tension) ---
    NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
    NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5,
    NOTE_AS5, NOTE_AS5, NOTE_AS5, NOTE_AS5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_G5,
    NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_E5,
    NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
    NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5,

    // --- SECONDARY THEME ---
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_F5, NOTE_G5, NOTE_G5,
    NOTE_G5, NOTE_A5, NOTE_AS5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_A5, REST,
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_F5, NOTE_G5, NOTE_G5,
    NOTE_G5, NOTE_A5, NOTE_AS5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_A5, REST,

    // --- THE ESCALATION (Repeated with High Octave) ---
    NOTE_A5, NOTE_C6, NOTE_D6, NOTE_D6, REST, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_F6, REST,
    NOTE_F6, NOTE_G6, NOTE_E6, NOTE_E6, REST, NOTE_D6, NOTE_C6, NOTE_C6, NOTE_D6, REST,

    // --- THE FINAL SIEGE ---
    NOTE_D6, NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_D5, REST,
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5,
    NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
    NOTE_D5, NOTE_D4, NOTE_D5, NOTE_D4, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5
};

// Motor R: Driving Bass (Full Performance)
static const uint16_t pirate_bass[] = {
    // --- INTRO ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,

    // --- THE HOOK (Pass 1) ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, REST, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, REST,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, REST, NOTE_A2, NOTE_A2, NOTE_A2, REST,

    // --- THE HOOK (Pass 2) ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, REST, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, REST,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_G2, NOTE_G2, NOTE_G2, NOTE_G2, REST, NOTE_G2, NOTE_G2, NOTE_G2, NOTE_G2, REST,

    // --- THE BRIDGE ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_D3, NOTE_D3, NOTE_A2, NOTE_A2, NOTE_D3, NOTE_D3, NOTE_A2, REST,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_D3, NOTE_D3, NOTE_A2, NOTE_A2, NOTE_D3, NOTE_D3, NOTE_A2, REST,

    // --- THE CHASE ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3,
    NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3,
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_A2, NOTE_A2, NOTE_G2, NOTE_G2,
    NOTE_F2, NOTE_F2, NOTE_E2, NOTE_E2, NOTE_D2, NOTE_D2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3,
    NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3,

    // --- SECONDARY THEME ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_G3, NOTE_G3,
    NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_G3, NOTE_G3,
    NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_D3, NOTE_D3, NOTE_D3, REST,

    // --- THE ESCALATION (Bass) ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, REST, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, REST,

    // --- THE FINAL SIEGE ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, REST,
    NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_D3, NOTE_D2, NOTE_D3, NOTE_D2, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3
};

// Durations
static const uint8_t pirate_durations[] = {
    // Intro
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    // Phase 1 Hook
    8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 8,
    8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 8,
    // Phase 2 Hook
    8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 8,
    8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 8,
    // Bridge
    8, 8, 4, 8, 8, 4, 4, 8, 4, 4, 4, 8, 8, 4, 4, 8,
    8, 8, 4, 8, 8, 4, 4, 8, 4, 4, 4, 8, 8, 4, 4, 8,
    // Chase
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    // Secondary Theme
    8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 8,
    8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 8,
    // Escalation
    8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 16, 8, 8, 4, 4, 8,
    // Finale
    4, 8, 8, 8, 8, 8, 2, 8,
    8, 8, 8, 8, 4, 8, 8, 8, 8, 2,
    8, 8, 8, 8, 4, 4, 4, 2,
    8, 8, 8, 8, 4, 4, 4, 1
};

// Melody for Motor L (Lead Pulse)
static const uint16_t mario_melody[] = {
    // --- Intro ---
    NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5, REST, NOTE_G5, REST, NOTE_G4,
    // --- Main Theme ---
    NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
    NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
    NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4,
    NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
    NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
    NOTE_E5, NOTE_C5, NOTE_D5, NOTE_B4
};

// Bassline for Motor R (Triangle/Rhythm)
static const uint16_t mario_bass[] = {
    // --- Intro ---
    NOTE_D3, NOTE_D3, REST, NOTE_D3, REST, NOTE_D3, NOTE_D3, REST, NOTE_G3, REST, NOTE_G2, REST,
    // --- Main Theme ---
    NOTE_G3, NOTE_E3, NOTE_C3, NOTE_F3, NOTE_G3, NOTE_FS3, NOTE_F3,
    NOTE_E3, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4,
    NOTE_C4, NOTE_A3, NOTE_B3, NOTE_G3,
    NOTE_G3, NOTE_E3, NOTE_C3, NOTE_F3, NOTE_G3, NOTE_FS3, NOTE_F3,
    NOTE_E3, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4,
    NOTE_C4, NOTE_A3, NOTE_B3, NOTE_G3
};

// Durations (8=Eighth, 16=Sixteenth, 4=Quarter)
static const uint8_t mario_durations[] = {
    // --- The Iconic Intro ---
    // E5, E5, (rest), E5, (rest), C5, E5, (rest), G5, (rest), G4
    8, 4, 8, 8, 8, 8, 4, 8, 4, 4, 4, 

    // --- Main Theme Loop ---
    // This uses the syncopated "DA-da-da-DA-da" rhythm
    8, 8, 8, 8, 8, 4, 4,       // "C5, G4, E4, A4, B4, AS4, A4"
    6, 16, 8, 8, 8, 8,         // The "swing" section
    8, 8, 8, 8,                // "E5, C5, D5, B4"
    
    8, 8, 8, 8, 8, 4, 4,       // Repeat of the first phrase
    6, 16, 8, 8, 8, 8, 
    8, 8, 8, 8                 
};

// struct Music mario_theme = {
//     .bpm = 150, // Mario is faster than Zelda!
//     .notesL = mario_melody,
//     .notesR = mario_bass,
//     .notesDuration = mario_durations,
//     .length = sizeof(mario_durations) / sizeof(uint8_t)
// };

// --- Legend of Zelda (Stereo Transposed) ---
// Motor L: Lead Melody (Unrolled)
static const uint16_t zelda_melody[] = {
    // --- INTRO FANFARE ---
    NOTE_AS4, NOTE_F4, NOTE_AS4, NOTE_AS4, NOTE_C5, NOTE_D5, NOTE_D5, NOTE_DS5, NOTE_F5, 
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,

    // --- MAIN OVERWORLD THEME ---
    NOTE_AS4, NOTE_F4, NOTE_AS4, NOTE_AS4, NOTE_C5, NOTE_D5, NOTE_DS5, NOTE_F5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_FS5, NOTE_GS5, NOTE_AS5, NOTE_AS5, NOTE_AS5,
    NOTE_AS5, NOTE_GS5, NOTE_FS5, NOTE_GS5, NOTE_FS5, NOTE_F5, NOTE_F5,
    NOTE_F5, NOTE_DS5, NOTE_DS5, NOTE_F5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5, 
    NOTE_CS5, NOTE_DS5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_E5,
    NOTE_G5, NOTE_F5,

    // --- MAIN OVERWORLD THEME (High Octave) ---
    NOTE_AS5, NOTE_F5, NOTE_AS5, NOTE_AS5, NOTE_C6, NOTE_D6, NOTE_DS6, NOTE_F6,
    NOTE_F6, NOTE_F6, NOTE_F6, NOTE_FS6, NOTE_GS6, NOTE_AS6, NOTE_AS6, NOTE_AS6,
    NOTE_AS6, NOTE_GS6, NOTE_FS6, NOTE_GS6, NOTE_FS6, NOTE_F6, NOTE_F6,
    NOTE_F6, NOTE_DS6, NOTE_DS6, NOTE_F6, NOTE_FS6, NOTE_F6, NOTE_DS6, NOTE_CS6, 
    NOTE_CS6, NOTE_DS6, NOTE_F6, NOTE_DS6, NOTE_CS6, NOTE_C6, NOTE_C6, NOTE_D6, NOTE_E6,
    NOTE_G6, NOTE_F6,

    // --- ADVENTURE BRIDGE ---
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
    NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_DS5, NOTE_DS5, NOTE_DS5, NOTE_DS5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_FS5, NOTE_FS5, NOTE_FS5, NOTE_FS5,
    NOTE_GS5, NOTE_GS5, NOTE_GS5, NOTE_GS5, NOTE_AS5, NOTE_AS5, NOTE_AS5, NOTE_AS5,

    // --- UNDERWORLD TRANSITION (Original Deep Scale) ---
    NOTE_AS4, NOTE_A4, NOTE_GS4, NOTE_G4, NOTE_FS4, NOTE_F4, NOTE_E4, NOTE_DS4,
    NOTE_D4, NOTE_CS4, NOTE_C4, NOTE_B3, NOTE_AS3, NOTE_AS3, NOTE_AS3, NOTE_AS3,

    // --- FINAL HEROIC RESOLVE ---
    NOTE_AS4, NOTE_F4, NOTE_AS4, NOTE_AS4, NOTE_C5, NOTE_D5, NOTE_DS5, NOTE_F5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_FS5, NOTE_GS5, NOTE_AS5, NOTE_AS5, NOTE_AS5,
    NOTE_AS5, NOTE_GS5, NOTE_FS5, NOTE_GS5, NOTE_FS5, NOTE_F5, NOTE_F5,
    NOTE_F5, NOTE_DS5, NOTE_DS5, NOTE_F5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5,
    NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4
};

// Motor R: Original Bassline (Unrolled)
static const uint16_t zelda_bass[] = {
    // --- INTRO ---
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2,
    NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,

    // --- MAIN BASS ---
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2,
    NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2,
    NOTE_C2, NOTE_F2,

    // --- REPEAT MAIN BASS ---
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2,
    NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2, NOTE_C2,
    NOTE_C2, NOTE_F2,

    // --- BRIDGE BASS ---
    NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,
    NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_FS2, NOTE_FS2, NOTE_FS2, NOTE_FS2,
    NOTE_GS2, NOTE_GS2, NOTE_GS2, NOTE_GS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2,

    // --- UNDERWORLD BASS (Restored Low Octave) ---
    NOTE_AS2, NOTE_A2, NOTE_GS2, NOTE_G2, NOTE_FS2, NOTE_F2, NOTE_E2, NOTE_DS2,
    NOTE_D2, NOTE_CS2, NOTE_C2, NOTE_B1, NOTE_AS1, NOTE_AS1, NOTE_AS1, NOTE_AS1,

    // --- REPRISE BASS ---
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_F2, NOTE_F2, NOTE_F2, NOTE_F2,
    NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
    NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_CS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2
};

static const uint8_t zelda_durations[] = {
    // Fanfare
    8, 16, 16, 4, 8, 8, 4, 8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 2,
    // Pass 1
    4, 8, 16, 16, 8, 8, 8, 2, 8, 16, 16, 8, 8, 8, 2, 8, 8, 8, 4, 8, 8, 2,
    8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2,
    // Pass 2
    4, 8, 16, 16, 8, 8, 8, 2, 8, 16, 16, 8, 8, 8, 2, 8, 8, 8, 4, 8, 8, 2,
    8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2,
    // Bridge
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    4, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8, 4,
    4, 8, 8, 4, 4, 8, 8, 4,
    // Transition
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 4, 2,
    // Final Reprise
    4, 8, 16, 16, 8, 8, 8, 2, 8, 16, 16, 8, 8, 8, 2, 8, 8, 8, 4, 8, 8, 2,
    8, 8, 8, 4, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4, 4, 1
};

// static const struct Music zelda_theme = {
//     .bpm = 125,
//     .notes = zelda_notes,
//     .notesDuration = zelda_durations,
//     .length = sizeof(zelda_notes) / sizeof(uint16_t)
// };

// Melody for Motor L (Octave 5/6)
static const uint16_t pokemon_melody[] = {
    NOTE_C6, NOTE_G5, NOTE_C6, NOTE_G5, NOTE_C6, NOTE_G5, NOTE_C6, REST,
    NOTE_D6, NOTE_A5, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_A5, NOTE_D6, REST,
    NOTE_E6, NOTE_C6, NOTE_E6, NOTE_C6, NOTE_E6, NOTE_C6, NOTE_E6, REST,
    NOTE_F6, NOTE_D6, NOTE_F6, NOTE_D6, NOTE_F6, NOTE_D6, NOTE_F6, REST
};

// Harmony for Motor R (Octave 4)
static const uint16_t pokemon_harmony[] = {
    NOTE_E4, NOTE_C4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_C4, NOTE_E4, REST,
    NOTE_F4, NOTE_D4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_D4, NOTE_F4, REST,
    NOTE_G4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_G4, REST,
    NOTE_A4, NOTE_F4, NOTE_A4, NOTE_F4, NOTE_A4, NOTE_F4, NOTE_A4, REST
};

// Standard 8th notes (8) with rests
static const uint8_t pokemon_durations[] = {
    8, 8, 8, 8, 8, 8, 4, 4,
    8, 8, 8, 8, 8, 8, 4, 4,
    8, 8, 8, 8, 8, 8, 4, 4,
    8, 8, 8, 8, 8, 8, 4, 4
};

// Melody for Motor L (High Octave Lead)
static const uint16_t pokemon_battle_melody[] = {
    // --- The "Encounter" Intro (Chromatic Climb) ---
    NOTE_AS5, NOTE_A5, NOTE_GS5, NOTE_G5, NOTE_GS5, NOTE_G5, NOTE_FS5, NOTE_F5,
    NOTE_FS5, NOTE_F5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_D5, NOTE_CS5,
    
    // --- The Main Loop (Urgent) ---
    NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
    NOTE_D5, NOTE_F5, NOTE_G5, NOTE_GS5, NOTE_AS5, NOTE_C6, NOTE_D6, REST,
    
    // --- Battle Theme Riff ---
    NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D5,
    NOTE_F6, NOTE_F5, NOTE_F5, NOTE_F6, NOTE_F5, NOTE_F5, NOTE_F6, NOTE_F5,
    NOTE_G6, NOTE_G5, NOTE_G5, NOTE_G6, NOTE_G5, NOTE_G5, NOTE_G6, NOTE_G5,
    NOTE_GS6, NOTE_GS5, NOTE_GS5, NOTE_GS6, NOTE_GS5, NOTE_GS5, NOTE_GS6, NOTE_GS5
};

// Driving Bass for Motor R (Octave 3)
static const uint16_t pokemon_battle_bass[] = {
    // --- Intro (Constant Pulse) ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    
    // --- Main Loop ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_GS3, NOTE_GS3, NOTE_GS3, NOTE_GS3,
    
    // --- Galloping Bassline ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3,
    NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3,
    NOTE_GS3, NOTE_GS3, NOTE_GS3, NOTE_GS3, NOTE_GS3, NOTE_GS3, NOTE_GS3, NOTE_GS3
};

// Durations (8=Eighth, 16=Sixteenth)
static const uint8_t pokemon_battle_durations[] = {
    // Intro (Very Fast)
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    // Main Loop
    8, 8, 8, 8, 8, 8, 8, 8,
    16, 16, 16, 16, 16, 16, 8, 8,
    // Battle Riff (The Gallop)
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16
};

// Melody for Motor L (Lead Pulse)
static const uint16_t megalo_melody[] = {
    // --- Phase 1: The Iconic Riff (Loop x2) ---
    NOTE_D4, NOTE_D4, NOTE_D5, NOTE_A4, REST, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
    NOTE_C4, NOTE_C4, NOTE_D5, NOTE_A4, REST, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
    NOTE_B3, NOTE_B3, NOTE_D5, NOTE_A4, REST, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
    NOTE_AS3, NOTE_AS3, NOTE_D5, NOTE_A4, REST, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,

    NOTE_D4, NOTE_D4, NOTE_D5, NOTE_A4, REST, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
    NOTE_C4, NOTE_C4, NOTE_D5, NOTE_A4, REST, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
    NOTE_B3, NOTE_B3, NOTE_D5, NOTE_A4, REST, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
    NOTE_AS3, NOTE_AS3, NOTE_D5, NOTE_A4, REST, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,

    // --- Phase 2: The High-Octave Escalation ---
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
    NOTE_GS5, NOTE_GS5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5,

    // --- Phase 3: The Melodic Drop ---
    NOTE_D6, NOTE_D6, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_C6, NOTE_A5, NOTE_G5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
    NOTE_D6, NOTE_D6, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_C6, NOTE_A5, NOTE_G5,
    NOTE_A5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_G5
};

// Bass for Motor R (Power Chords/Drums)
static const uint16_t megalo_bass[] = {
    // --- Phase 1 ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3,
    NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2,
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2,

    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3,
    NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2,
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2,

    // --- Phase 2 ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3,
    NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3,
    NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_A3, NOTE_A3, NOTE_A3, NOTE_A3,

    // --- Phase 3 ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3,
    NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2, NOTE_B2,
    NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2, NOTE_AS2
};

static const uint8_t megalo_durations[] = {
    // Phase 1 (Adjusted for 240 BPM: 16->8, 8->4, 6->3)
    8, 8, 4, 3, 8, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 8, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 8, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 8, 4, 4, 4, 8, 8, 8,

    8, 8, 4, 3, 8, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 8, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 8, 4, 4, 4, 8, 8, 8,
    8, 8, 4, 3, 8, 4, 4, 4, 8, 8, 8,
    
    // Phase 2 (8->4)
    4, 4, 4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    
    // Phase 3 (16->8, 8->4)
    8, 8, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    8, 8, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4
};

// Melody for Motor L (Vocals)
static const uint16_t espresso_melody[] = {
    // --- Verse 1: "Now he's thinkin' 'bout me..." ---
    NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_A4, REST,
    NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_E5, NOTE_D5, REST,
    NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4,
    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4,

    // --- Pre-Chorus: "I'm working late 'cause I'm a singer" ---
    NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, 
    NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_A5,

    // --- Chorus: "Now he's thinkin' 'bout me every night, oh" ---
    NOTE_D5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_E5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_B5,
    NOTE_G5, NOTE_G5, NOTE_G5, NOTE_B5, NOTE_C6, NOTE_B5, NOTE_B5, NOTE_A5,
    NOTE_C5, NOTE_C5, NOTE_C5, NOTE_E5, NOTE_A5, NOTE_A5, NOTE_A5, NOTE_B5,
    NOTE_G5, NOTE_G5, NOTE_B5, NOTE_C6, NOTE_B5, NOTE_B5, NOTE_A5
};

// Groovy Bass for Motor R (Drivetrain Harmony)
static const uint16_t espresso_bass[] = {
    // --- Verse 1 Bass ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,

    // --- Pre-Chorus Bass (Straight 8ths) ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3,
    NOTE_F3, NOTE_F3, NOTE_F3, NOTE_F3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3,

    // --- Chorus Bass (The Pocket) ---
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_G2, NOTE_G2, NOTE_G2, NOTE_G2, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_G2, NOTE_G2, NOTE_G2, NOTE_G2, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2
};

// Durations
static const uint8_t espresso_durations[] = {
    // Verse 1
    8, 8, 8, 16, 16, 4, 8,
    8, 8, 8, 16, 16, 4, 8,
    8, 8, 8, 16, 16, 8, 8,
    8, 8, 8, 8, 4, 4,

    // Pre-Chorus
    16, 16, 16, 16, 16, 8, 8,
    16, 16, 16, 16, 16, 8, 8,
    16, 16, 16, 16, 16, 8, 8,
    8, 8, 8, 8, 4, 4,

    // Chorus
    8, 8, 16, 16, 16, 8, 16, 16, 16, 8,
    16, 16, 16, 8, 8, 16, 16, 4,
    8, 8, 16, 16, 16, 8, 16, 16, 16, 8,
    16, 16, 8, 8, 16, 16, 4
};

// Melody for Motor L (Octave 5/6)
static const uint16_t tetris_melody[] = {
    NOTE_E6, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_C6, NOTE_B5, NOTE_A5,
    NOTE_A5, NOTE_C6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_B5,
    NOTE_C6, NOTE_D6, NOTE_E6, NOTE_C6, NOTE_A5, NOTE_A5, REST,
    NOTE_D6, NOTE_F6, NOTE_A6, NOTE_G6, NOTE_F6, NOTE_E6,
    NOTE_C6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_B5,
    NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_C6, NOTE_A5, NOTE_A5
};

// Walking Bass for Motor R (Octave 3)
static const uint16_t tetris_bass[] = {
    NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3,
    NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_A2, NOTE_A2, NOTE_A2, NOTE_A2
};

// Durations (4=Quarter, 8=Eighth, 16=Sixteenth)
static const uint8_t tetris_durations[] = {
    4, 8, 8, 4, 8, 8, 4,
    8, 8, 4, 8, 8, 4,
    4, 4, 4, 4, 4, 4, 4,
    4, 8, 4, 8, 8, 4,
    8, 4, 8, 8, 4,
    8, 8, 4, 4, 4, 4, 4
};

// Melody for Motor L (The "Strings" Lead)
static const uint16_t halo_melody[] = {
    REST, NOTE_E5, NOTE_FS5, NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_E5, 
    REST, NOTE_E5, NOTE_FS5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_FS5, NOTE_E5,
    NOTE_B5, NOTE_A5, NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_E5
};

// Chanting Bass for Motor R (Low and Sustained)
static const uint16_t halo_bass[] = {
    NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3,
    NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2, NOTE_E2
};

// Very long notes for the "Chant" effect (1=Whole, 2=Half)
static const uint8_t halo_durations[] = {
    1, 4, 4, 2, 4, 4, 4, 1,
    1, 4, 4, 2, 4, 4, 4, 1,
    2, 2, 2, 2, 2, 2, 1
};

// Melody for Motor L (Vocals: "Never gonna give you up...")
static const uint16_t rick_melody[] = {
    NOTE_A4, NOTE_B4, NOTE_D5, NOTE_B4, NOTE_FS5, NOTE_FS5, NOTE_E5, REST,
    NOTE_A4, NOTE_B4, NOTE_D5, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_CS5, NOTE_B4,
    NOTE_A4, NOTE_B4, NOTE_D5, NOTE_B4, NOTE_D5, NOTE_E5, NOTE_CS5, NOTE_A4, NOTE_A4, NOTE_E5, NOTE_D5
};

// Percussive Bass for Motor R (The "Thump")
static const uint16_t rick_bass[] = {
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_G2, NOTE_G2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_G2, NOTE_G2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_D3, NOTE_D3, NOTE_G2, NOTE_G2, NOTE_A2, NOTE_A2,
    NOTE_D3, NOTE_D3, NOTE_A2, NOTE_A2
};

// Durations (8=Eighth, 16=Sixteenth, 4=Quarter)
static const uint8_t rick_durations[] = {
    8, 8, 8, 8, 4, 4, 2, 8,
    8, 8, 8, 8, 4, 4, 8, 8, 4,
    8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 2
};