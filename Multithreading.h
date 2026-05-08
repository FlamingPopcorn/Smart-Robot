#ifndef _Multithreading
#define _Multithreading

// I2C0 defines
#define I2C0_PORT i2c0
#define I2C0_SDA 4
#define I2C0_SCL 5

// I2C1 defines
#define I2C1_PORT i2c1
#define I2C1_SDA 14
#define I2C1_SCL 15

enum Core1Mode{
    Idle,
    Line_Following,
    Maze_Solving,
    Remote_Control,
    Music,
    Skip
};

extern struct PID_Controller* drive_pid;
extern struct PID_Controller* turn_pid;
extern volatile float drive_pid_input;
extern volatile float turn_pid_input;
extern volatile enum Core1Mode core1CurrentMode;
extern volatile bool core1Booting;

void initCore1(void);
void change_core1_mode(enum Core1Mode new_mode);
static void core1_main_entry_point(void);
void start_control_loop(void);
bool pidUpdate_timer_callback(struct repeating_timer *t);
bool control_motors_timer_callback(struct repeating_timer *t);

#endif