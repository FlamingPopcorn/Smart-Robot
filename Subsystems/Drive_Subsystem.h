#ifndef _Drive_Subsystem
#define _Drive_Subsystem

// Motor defines
#define MOTORL_FWD 10
#define MOTORL_RVS 11
#define MOTORR_FWD 12
#define MOTORR_RVS 13

#define WHEEL_CIRCUMFERENCE 8.5 // the wheels' circumference are about 8.5 inches each
#define PWM_MAX 59999
#define PWM_MAX_f (float)(PWM_MAX)

extern struct Drivetrain* global_drivetrain;

enum MotorMode{
    Drive_mode,
    Music_mode
};

struct Motor{
    struct MagEncoder* encoder;
    int pinFWD;
    int pinRVS;
    uint pwmSliceFWD;
    uint pwmSliceRVS;
    volatile int velocity;
    float last_dir;
    float filtered_rps;
    struct PID_Controller* pid;
};

struct Drivetrain{
    struct Motor* motorL;
    struct Motor* motorR;
    volatile int velocity;
    volatile int heading;
    enum MotorMode mode;
};

struct Drivetrain* initDrivetrain(int pinLFWD, int pinLRVS, int pinRFWD, int pinRRVS);
static struct Motor* initMotor(int pinFWD, int pinRVS);
void setDrivePWM(struct Motor* motor);
void setDrivetrainVelocity(struct Drivetrain*, float left_rps, float right_rps);
void DifferentialDrive(struct Drivetrain* drivetrain, float, float);
void setMotorVelocity(struct Motor*, int32_t);
bool rotateDistance(struct Drivetrain* drivetrain, bool drive);

#endif