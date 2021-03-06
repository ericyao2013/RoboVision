#ifndef MOTORHAL_H_INCLUDED
#define MOTORHAL_H_INCLUDED


#include "../WorldMapping/MasterpathPlanning/MasterpathPlanning.h"

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int robot_height; // mm
extern unsigned int robot_length; // mm
extern unsigned int robot_width; // mm
extern unsigned int motors_distance; // mm
extern unsigned int wheel_diameter; // mm


unsigned int RobotInit(char * md23_device_id,char * arduino_device_id);
unsigned int RobotClose();
unsigned int RobotIsClosed();
void RobotWait(unsigned int msecs);

unsigned int RobotBaseOk();
unsigned int RobotHeadOk();

unsigned int RobotMoveJoystick(signed int joy_x,signed int joy_y);
unsigned int RobotRotate(unsigned char power,signed int degrees);
unsigned int RobotStartRotating(unsigned char power,signed int direction);
unsigned int RobotSetHeadNod(char * pose_string);
unsigned int RobotSetHeadPose(unsigned int heading,unsigned int pitch);
unsigned int RobotGetHeadPose(unsigned int * heading,unsigned int * pitch);
unsigned int RobotMove(unsigned char power,signed int distance);
unsigned int RobotStartMoving(unsigned char power,signed int direction);
unsigned int RobotManoeuvresPending();
void RobotStopMovement();

int RobotGetUltrasonic(unsigned int which_one);
int RobotGetAccelerometerX();
int RobotGetAccelerometerY();
int RobotSetLightsState(unsigned int light_num,unsigned int light_state);
int RobotIRTransmit(char * code,unsigned int code_size);


unsigned int RobotPrintPosition();

struct Map * RobotGetMapPointer();

#ifdef __cplusplus
}
#endif


#endif // MOTORHAL_H_INCLUDED
