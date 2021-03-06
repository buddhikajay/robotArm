/*
 * MotorFunctions.h
 *
 * Created: 10/5/2014 10:41:30 AM
 *  Author: Buddhika Jayawardhana
 */ 
 /*This file contains all necessary functions to drive a motor. All functions are written in 
 Object oriented manner EXCEPT "void checkHomeSwiches(struct Motor *motor)". For all other functions
 you only have to pass the reference of the motor.
 *Depends on 
	"ConvensionDefine.h","PinDefine.h","Motor.h","MotorDeclaration.h"
	<avr/io.h>,<avr/interrupt.h>,<util/delay.h>
 
 More work to done on "void checkHomeSwiches(struct Motor *motor)"
 */
 
 
/*FUNCTIONS*/
void motorObjectSetup(struct Motor *motor);//assign initial attributes to Motors
void pinSetup();//Setup DDR pins
void initialize();//Checks state of all Motors before executing the program.
void checkHomeSwiches(struct Motor *motor);//check the state of the position indicating switch of the motor.
void pollMotor(struct Motor *motor);//THE LOGIC IS HERE. Checks all the Motors and decide what to do.

void motorObjectSetup(struct Motor *motor){
	motor->running = TRUE;
	motor->targetDirection = RIGHT;
	motor->maxRevolutionsLeft = -50;
	motor->maxRevolutionsRight = 50;
	motor->relativeRevolutions = 0;
	motor->targetPosition = -300;

}

void pinSetup(){
	DDRJ = 0x0;//PCINT for switches
	//DDRD = 0x1E;//INT for Encoder-A
	//DDRE = 0xF0;//INT for Encoder-A
	DDRA = 0;
	
	//temp code
	DDRB = 0xFF;//PWM and Direction controlling
	
	//end temp code
	
	
	//Setup external interrupts
	EIMSK = 0x01;//Enabling interrupts 7:0
	EICRA = 0xFF;//Enabling Rising edge interrupts for INT 3:0
	EICRB = 0xFF;//Enabling Rising edge interrupts for INT 7:4
	
	//setup pin change interrupts
	PCMSK1 = 0xFF;//Enabling PCinterrupts 15:8, but we want only 15:9/J6:J0
	PCICR = (1<<PCIE1);//Enabling PCinterrupts 15:8, but we want only 15:9/J6:J0 
}
void initialize(){
	checkHomeSwiches(M0);
	
}
//more work here:How to pass SWM0????
void checkHomeSwiches(struct Motor *motor){
	if(SWM0 == SWITCHE_PRESSED){
		motor->HomeSwitchState = SWITCHE_PRESSED;
	}
	else{
		motor->HomeSwitchState = !SWITCHE_PRESSED;
	}
}

void pollMotor(struct Motor *motor){
	//run/stop motor
	if(motor->running == TRUE){
		ENABLE_PORT1 |= (TRUE<<ENM0);
	}
	else{
		ENABLE_PORT1 &= ~(TRUE<<ENM0);
	}
	//change the rotating direction
	if(motor->targetDirection == RIGHT){//rotate right
		DIRECTION_PORT |= (RIGHT<<DIRM0);
	}
	else if(motor->targetDirection == LEFT){//rotate left
		DIRECTION_PORT &= ~(RIGHT<<DIRM0);
	}
	//stop the motor if safety limits reached
	if(((motor->relativeRevolutions)>= (motor->maxRevolutionsRight))||((motor->relativeRevolutions)<=(motor->maxRevolutionsLeft))){
		motor->running = FALSE;
	}
	//stop at target position
// 	if(!((motor->relativeRevolutions)-(motor->targetPosition))){
// 		motor->running = FALSE;
	//}
}
