#include "WPILib.h"
#include <Relay.h>

// Integer globals
float shooterSpeed = 0.0;
float loaderSpeed = 0.0;

class RobotDemo : public IterativeRobot {

    // RobotDrive myRobot; // robot drive system
    Joystick *controllerOne;
    Joystick *controllerTwo;
    
    Talon *rightFront;
    Talon *leftFront;
    Talon *rightBack;
    Talon *leftBack;
    Victor *shooter;
    Talon *loader;

    //Relay *shooterLights; not needed

public:
    RobotDemo(void) {
  		printf("RobotDemo Constructor Started\n");

  		rightFront = new Talon(1);
  		leftFront  = new Talon(2);
  		rightBack = new Talon(3);
  		leftBack = new Talon(4);
  		loader = new Talon(5);
  		shooter = new Victor(6);
  		
  		controllerOne = new Joystick(1);
  		controllerTwo = new Joystick(2);
  	//	shooterLights = new Relay(1, Relay::kForwardOnly);
  		
 
  		// Acquire the Driver Station object
  		m_ds = DriverStation::GetInstance();

  	    printf("RobotDemo Constructor Completed\n");
  	}

  	/********************************** Init Routines *************************************/

  	void RobotInit(void) {
  	}

  	void DisabledInit(void) {
  	}

  	void AutonomousInit(void) {
  	}

  	void TeleopInit(void) {
  		// Set all motor controllers to not moving.
  		// shooter->SetSpeed(0.0);
  		leftFront->SetSpeed(0.0);
  		rightFront->SetSpeed(0.0);
  		leftBack->SetSpeed(0.0);
  		rightBack->SetSpeed(0.0);
  		shooter->SetSpeed(0.0);
  		loader->SetSpeed(0.0);
  		//shooterLights->Set(Relay::kOn);
  	}

  	/********************************** Periodic Routines *************************************/

  	void DisabledPeriodic(void) {
  	}



  	void AutonomousPeriodic(void) {
  		
  		
  	}

  	void TeleopPeriodic(void) {
  		// http://www.chiefdelphi.com/forums/showthread.php?t=82825
  		// 1 - LeftX
  	    // 2 - LeftY
        // 3 - Triggers (Each trigger = 0 to 1, axis value = right - left)
        // 4 - RightX
        // 5 - RightY
        // 6 - DPad Left/Right

  		float leftStick  = -1 * controller->GetRawAxis(2); // Drive system
  		float rightStick = controller->GetRawAxis(5); // Drive system
  		// bool buttonA  = controller->GetRawButton(1);
  		// bool buttonB  = controller->GetRawButton(2);
  		// bool buttonX  = controller->GetRawButton(3);
  		// bool buttonY  = controller->GetRawButton(4);
  		bool leftBumper = controller->GetRawButton(5); // Fire shooter
  		bool rightBumper = controller->GetRawButton(6); // Move loader
  		// bool buttonBack  = controller->GetRawButton(7);
  		// bool buttonStart = controller->GetRawButton(8);
  		// bool leftStickPress = controller->GetRawButton(9);
  		// bool rightStickPress = controller->GetRawButton(10);

  		// Shooter control
  		if(leftBumper) {
  			shooterSpeed = -1.0; // Forward (-)
  		} else {
  			shooterSpeed = 0.0;
  		}
  		
  		// Loader control
  		if(rightBumper) {
			loaderSpeed = 1.0; // Forward (+)
		} else {
			loaderSpeed = 0.0;
		} 

  		// Print values (rate limited to 1/20)
  		// if(printCounter % 20 == 0) {
  		//     if(!limitSwitchExtended) printf("Lower Limit Hit\n");
  		//     if(!limitSwitchRetracted) printf("Upper Limit Hit\n");
  		// }
  		// printCounter++;

  		// Drive system deadzone
  		if(leftStick < 0.3 && leftStick > -0.3) {
  			leftStick = 0.0;
  		}
  		if(rightStick < 0.3 && rightStick > -0.3) {
  			rightStick = 0.0;
  		}

  		// Motor speed declarations done at the end to ensure watchdog is continually updated.
  		motorControlLeft(leftStick);
  		motorControlRight(rightStick);
  		shooter->SetSpeed(shooterSpeed);
  		loader->SetSpeed(loaderSpeed);
  	}


  	void DisabledContinuous(void) {
  	}
          
  	void AutonomousContinuous(void) {
  	}

  	void TeleopContinuous(void) {
  	}
  	
  	void motorControlLeft(float speed) {
  		left->SetSpeed(speed);
  	}

  	void motorControlRight(float speed) {
  		right->SetSpeed(speed);
  	}
  	
  	
  };

  START_ROBOT_CLASS(RobotDemo)
