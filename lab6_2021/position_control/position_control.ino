/**********************************************************************************

  Arduino code to control a DC motor
  MIT Department of Mechanical Engineering, 2.004
  Rev. 1.0 -- 08/03/2020 H.C.
  Rev. 2.0 -- 08/25/2020 H.C.

***********************************************************************************/

//#define OPEN_LOOP
#define vc2pwm 51.0    // pwm = 255 corresponds to 5v from pwm pin (255/5)

// ================================================================
// ===               PID FEEDBACK CONTROLLER                    ===
// ================================================================

// Provide PID controller gains here

float kp = 150.0;
float kd = 10.0;
//float kp = 0.0;
//float kd = 0.0;
float ki = 0.0;

// ================================================================
// ===               SETPOINT TYPE                              ===
// ================================================================

// Define setpoint

#define desired_pos 1           // desired velocity in rad/s

#define square_period 3000      // square wave period in milli-seconds
#define square_on 1500          // square wave on time in milli-seconds

#define sine_amp PI/2         // sine wave amplitude
#define sine_freq_hz 0.5        // sine wave frequency in Hz

//#define SQUARE_WAVE
#define SINE_WAVE
//#define TIC
int n = -1, pre_n = -1;

// ================================================================
// ===               SERIAL OUTPUT CONTROL                      ===
// ================================================================

// Switch between built-in Serial Plotter and Matlab streaming
// comment out both to disable serial output

#define PRINT_DATA
//#define MATLAB_SERIAL_READ

// ================================================================
// ===               DFROBOT MOTOR SHIELD DEFINITION            ===
// ================================================================

const int
// motor connected to M1
PWM_A   = 11,
DIR_A   = 13;

// ================================================================
// ===               ENCODER DEFINITION                         ===
// ================================================================

// The motor has a dual channel encoder and each has 120 counts per revolution
// The effective resolution is 480 CPR with quadrature decoding.
// Calibrate the conversion factor by hand.

float C2Rad =  1176*4/ (2 * PI); // TO DO: replace it with your conversion factor

// Encoder outputs to interrupt pins with channel A to D2 (INT0) and B to D3 (INT1)
const byte encoder0pinA = 2;
const byte encoder0pinB = 5;

long   count = 0;
volatile long counter = 0;    // Inrerrupt counter (position)

// ================================================================
// ===               VARIABLE DEFINITION                        ===
// ================================================================

float wheel_pos = 0.0, wheel_vel = 0.0, pre_wheel_pos = 0.0, filt_vel = 0.0;
float error, sum_error = 0.0, d_error = 0.0, filt_d_error = 0.0, error_pre;
float alpha = 0.25;    // low pass filter constant
unsigned long timer;
double loop_time = 0.01;
float Pcontrol, Icontrol, Dcontrol;
float pwm;
float vc;
float set_point = desired_pos;   // initial set point

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {

  Serial.begin(115200);

  // configure motor shield M1 outputs
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_A, OUTPUT);

  // initialize encoder
  pinMode(encoder0pinA, INPUT);
  pinMode(encoder0pinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder0pinA), ISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder0pinB), ISR_B, CHANGE);

  delay(10);    // delay 0.01 second
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {

  timer = micros();
  count = counter;

  // wheel position and velocity from the encoder counts
  wheel_pos += count;
  wheel_pos = wheel_pos / C2Rad;    // convert to radians
  wheel_vel = (wheel_pos - pre_wheel_pos) / loop_time;
  filt_vel = alpha * wheel_vel + (1 - alpha) * filt_vel;
  pre_wheel_pos = wheel_pos;

  // ================================================================
  // ===                    GET SETPOINT                          ===
  // ================================================================

  // define set point as a square wave input
#ifdef SQUARE_WAVE
  if (millis() % square_period < square_on)
    set_point = desired_pos;
  else
    set_point = 0.0;
#endif

  // define set point as a sine wave with an amplitude and frequency
#ifdef SINE_WAVE
  set_point = sine_amp * sin(sine_freq_hz * (2 * M_PI) * millis() / 1000.0);
#endif

  // EXTRA CREDIT: code a "second" hand setpoint
#ifdef TIC
//  int period = 6000;    // milli-sec
//  int step_time = 1000;   // milli-sec
//
//// detect the begining of each period and increment n by 1
//// sampling period is 0.01 so less than 9 ms is a good value
//// occationaly there maybe a skip so check the previous value
//  if (millis() % period <= 9) {
//    n++;
//    if (n == pre_n){
//      n++;
//      pre_n = n;
//    }
//    else 
//    pre_n = n;
//  }
//
//  if (millis() % period < step_time)
//    set_point = PI / 3 + (n * 2 * PI);
//  else if (millis() % period < 2 * step_time)
//    set_point = 2 * PI / 3 + (n * 2 * PI);
//  else if (millis() % period < 3 * step_time)
//    set_point = 3 * PI / 3 + (n * 2 * PI);
//  else if (millis() % period < 4 * step_time)
//    set_point = 4 * PI / 3 + (n * 2 * PI);
//  else if (millis() % period < 5 * step_time)
//    set_point = 5 * PI / 3 + (n * 2 * PI);
//  else if (millis() % period < 6 * step_time)
//    set_point = 6 * PI / 3 + (n * 2 * PI);

//set_point = (2*PI/6) * floor(millis() / 1000.0);
//if (millis() % 1000 < 20)
//set_point += PI/3;
//else
//set_point = set_point;
set_point = millis()/1000 * PI/3;
#endif

  // ================================================================
  // ===                    CONTROLLER CODE                       ===
  // ================================================================

  // TO DO: PID controller code
  error = set_point - wheel_pos;
  d_error = (error - error_pre) / loop_time;
  filt_d_error = alpha * d_error + (1 - alpha) * filt_d_error;
  error_pre = error;
  sum_error += error * loop_time;

  Pcontrol = error * kp;
  Icontrol = sum_error * ki;
  Dcontrol = d_error * kd;    // use d_error to avoid delay

  Icontrol = constrain(Icontrol, -255, 255);    // anti-windup for integrator
  pwm =  Pcontrol + Icontrol + Dcontrol;

#ifdef OPEN_LOOP
  pwm = 150;
#endif

  // ================================================================
  // ===                    MOTOR PWM CODE                        ===
  // ================================================================

  vc = constrain(pwm / vc2pwm, -5, 5);

  // set the correct direction
  if (pwm > 0) {
    digitalWrite(DIR_A, HIGH);
  }
  else {
    digitalWrite(DIR_A, LOW);
  }

  // constrain pwm to +/- 230
  pwm = constrain(pwm, -255, 255);

  analogWrite(PWM_A, (int) abs(pwm));     // Set speed

#ifdef PRINT_DATA
  Serial.print(set_point);  Serial.print("\t");
  Serial.print(wheel_pos); Serial.print("\t");
  //    Serial.print(filt_vel); Serial.print("\t");
  //    Serial.print(error);  Serial.print("\t");
  //    Serial.print(d_error);  Serial.print("\t");
  Serial.print(vc);  Serial.print("\t");
  //    Serial.print(pwm);  Serial.print("\t");
  //    Serial.print(loop_time,6); Serial.print("\t");
  Serial.print("\n");
#endif

#ifdef MATLAB_SERIAL_READ
  Serial.print(timer / 1000000.0);   Serial.print("\t");
  Serial.print(set_point);    Serial.print("\t");
  Serial.print(wheel_pos);    Serial.print("\t");
  Serial.print(vc);
  Serial.print("\n");
#endif

  delay(8);
  loop_time = (micros() - timer) / 1000000.0;  //compute actual sample time

}

// ================================================================
// ===                   Interrupt Service Routines             ===
// ================================================================

// Interrupt Service Routine for encoder A (pin 2)
void ISR_A() {
  boolean stateA = digitalRead(encoder0pinA) == HIGH;
  boolean stateB = digitalRead(encoder0pinB) == HIGH;
  if ((!stateA && stateB) || (stateA && !stateB)) counter++;
  else counter--;
}

// Interrupt Service Routine for encoder B (pin 3)
void ISR_B() {
  boolean stateA = digitalRead(encoder0pinA) == HIGH;
  boolean stateB = digitalRead(encoder0pinB) == HIGH;
  if ((!stateA && !stateB) || (stateA && stateB)) counter++;
  else counter--;
}
