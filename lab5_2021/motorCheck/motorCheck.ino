/* Use this code to check the function of your motors when they are powered on. Proper function will result in each joint moving back and fourth around its initial position. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/
 
#include <Encoder.h>

//Arduino PWM Speed Control：
  //Motor 1
    int E1 = 10;  //Pin to set PWM Ratio
    int M1 = 12;  //Pin to set Direction
  // Motor 2
    int E2 = 11;  //Pin to set PWM Ratio
    int M2 = 13;  //Pin to set Direction


//Define Encoder Pins. First pin must be an interupt pin (see, https://www.pjrc.com/teensy/td_libs_Encoder.html).
  Encoder Mot2(2,5);
  Encoder Mot1(3,4);

//Parameters
  bool direction=HIGH;
  int value=100;  // PWM Duty Cycle, 0 (always off) and 255 (always on)
  int guard=100; // Soft-stop encoder count
  
void setup(){
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  Serial.begin(9600);
  Serial.println("start");
}

void loop(){  
  // Read Encoders
      Serial.print("Mot 1: "); //Read encoder
      Serial.print(Mot1.read()); //Read encoder
      Serial.print(" | Mot 2: "); //Read encoder
      Serial.println(Mot2.read()); //Read encoder
  
  // Check Directions at Soft Stop
    // Motor 1
      if (Mot1.read()<=-guard){ //Change direction at low end 
        digitalWrite(M1,LOW); 
      }
      else if ((Mot1.read()>=guard)){
        digitalWrite(M1,HIGH); 
      }
      
    // Motor 2
      if (Mot2.read()<=-guard){ //Change direction at high end 
        digitalWrite(M2,LOW); 
      }
      else if ((Mot2.read()>=guard)){
        digitalWrite(M2,HIGH); 
      }
    
      analogWrite(E1, value);  
      analogWrite(E2, value);  
}
