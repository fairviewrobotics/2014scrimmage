#include "WPILib.h"
#include <Relay.h>

// Integer globals
float shooterSpeed = 0.0;
float loaderSpeed = 0.0;

class RobotDemo : public IterativeRobot {
	// Drive system motor controllers
    Talon *rightFront;
    Talon *leftFront;
    Talon *rightBack;
    Talon *leftBack;

    // Loader and shooter motor controllers
    Talon *loader;
    Victor *shooter;

    // Shooter timer
    Timer *shooterTimer;

    // Joysticks
    Joystick *controllerLeft;
    Joystick *controllerRight;

public:
    RobotDemo(void) {
  		printf("RobotDemo Constructor Started\n");

  		rightFront = new Talon(1);
  		leftFront  = new Talon(2);
  		rightBack  = new Talon(3);
  		leftBack   = new Talon(4);

  		loader     = new Talon(5);
  		shooter    = new Victor(6);
  		
  		shooterTimer = new Timer();

  		controllerLeft  = new Joystick(1);
  		controllerRight = new Joystick(2);  		
 
  		// Acquire the Driver Station object
  		m_ds = DriverStation::GetInstance();

  	    printf("RobotDemo Constructor Completed\n");
  	}

  	/********************************** Init Routines *************************************/

  	void RobotInit(void) {}
  	void DisabledInit(void) {}
  	void AutonomousInit(void) {}

  	void TeleopInit(void) {
  		// Set all motor controllers to be not moving.
  		leftFront->SetSpeed(0.0);
  		rightFront->SetSpeed(0.0);
  		leftBack->SetSpeed(0.0);
  		rightBack->SetSpeed(0.0);

  		shooter->SetSpeed(0.0);
  		loader->SetSpeed(0.0);
  	}
  	
  	void motorControlLeft(float speed) {
		leftFront->SetSpeed(speed);
		leftBack->SetSpeed(speed);
	}

	void motorControlRight(float speed) {
		rightFront->SetSpeed(speed);
		rightBack->SetSpeed(speed);
	}

  	/********************************** Periodic Routines *************************************/

  	void DisabledPeriodic(void) {}
  	void AutonomousPeriodic(void) {}

  	void TeleopPeriodic(void) {
  		float leftStick  = controllerLeft->GetRawAxis(2);  // Drive system
  		float rightStick = controllerRight->GetRawAxis(2); // Drive system
  		bool moveLeft  = controllerLeft->GetRawButton(1);  // Trigger to slide left
  		bool moveRight = controllerRight->GetRawButton(1); // Trigger to slide right
  		bool moveHalfLeft  = controllerLeft->GetRawButton(2);  // Half speed slide left
  		bool moveHalfRight = controllerRight->GetRawButton(2); // Half speed slide right
  		bool loadButton = controllerLeft->GetRawButton(3);   // Loader
  		bool shootButton = controllerRight->GetRawButton(3); // Shooter

  		// Loader control (hold down button)
  		if(loadButton) {
			loaderSpeed = -1.0;
		} else {
			loaderSpeed = 0.0;
		}

  		// Shooter control (toggle button with timer)
		if(shootButton) {
			shooterTimer->Reset(); // Set timer back to 0
			shooterTimer->Start();
		} else if(shooterTimer->Get() > 0) {
			shooterSpeed = 1.0;

			if(shooterTimer->Get() > 1.5) { // Threshold when shooter will be going near max speed
				loaderSpeed = 0.2;
			} else if(shooterTimer->Get() > 2.0) { // Threshold when disk will have been launched
				shooterSpeed = 0.0;
				shooterTimer->Stop();
				shooterTimer->Reset(); // Set timer back to 0
			}
		} else {
			shooterSpeed = 0.0;
		}

  		// Print values (rate limited to 1/20)
  		// if(printCounter % 20 == 0) {
  		//     if(!limitSwitchExtended) printf("Lower Limit Hit\n");
  		//     if(!limitSwitchRetracted) printf("Upper Limit Hit\n");
  		// }
  		// printCounter++;


  		// Drive system deadzone
  		if(leftStick < 0.1 && leftStick > -0.1) {
  			leftStick = 0.0;
  		}
  		if(rightStick < 0.1 && rightStick > -0.1) {
  			rightStick = 0.0;
  		}

  		// Motor speed declarations done at the end to ensure watchdog is continually updated.
  		if (moveLeft) {
  			leftFront->SetSpeed(-1.0); // backwards
  			leftBack->SetSpeed(1.0);
  			rightFront->SetSpeed(1.0);
  			rightBack->SetSpeed(-1.0);
  		} else if (moveRight) {
  			leftFront->SetSpeed(1.0);
  			leftBack->SetSpeed(-1.0);
  			rightFront->SetSpeed(-1.0);
  			rightBack->SetSpeed(1.0);
  		} else if (moveHalfLeft) {
			leftFront->SetSpeed(-0.5); // backwards
			leftBack->SetSpeed(0.5);
			rightFront->SetSpeed(0.5);
			rightBack->SetSpeed(-0.5);
		} else if (moveHalfRight) {
			leftFront->SetSpeed(0.5);
			leftBack->SetSpeed(-0.5);
			rightFront->SetSpeed(-0.5);
			rightBack->SetSpeed(0.5);
  		} else {
  			motorControlLeft(leftStick);
  			motorControlRight(rightStick);
  		}
  		shooter->SetSpeed(shooterSpeed);
  		loader->SetSpeed(loaderSpeed);
  	}

  	void DisabledContinuous(void) {}
  	void AutonomousContinuous(void) {}
  	void TeleopContinuous(void) {}
  };

  START_ROBOT_CLASS(RobotDemo)
