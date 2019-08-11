
const uint32_t LEDC_TIMER_BIT = 8;
const uint32_t LEDC_BASE_FREQ = 1000;

const int32_t MOTOR_CH_FORWARD_L = 4;
const int32_t MOTOR_CH_REAR_L    = 5;
const int32_t MOTOR_CH_FORWARD_R = 6;
const int32_t MOTOR_CH_REAR_R    = 7;

const int32_t MOTOR_PIN_FORWARD_L = 25;
const int32_t MOTOR_PIN_REAR_L    = 26;
const int32_t MOTOR_PIN_FORWARD_R = 33;
const int32_t MOTOR_PIN_REAR_R    = 32;

int32_t speed_l = 0;
int32_t speed_r = 0;

enum {
    MOTOR_CHECK_STATE_STOP = 0,
    MOTOR_CHECK_STATE_FORWARD, 
    MOTOR_CHECK_STATE_BACK, 
    MOTOR_CHECK_STATE_RIGHT, 
    MOTOR_CHECK_STATE_LEFT, 
    MOTOR_CHECK_STATE_FIN, 
};

void setup() {
    Serial.begin(115200);
    Serial.println("setup");

    ledcSetup(MOTOR_CH_FORWARD_L, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
    ledcSetup(MOTOR_CH_REAR_L,    LEDC_BASE_FREQ, LEDC_TIMER_BIT);
    ledcSetup(MOTOR_CH_FORWARD_R, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
    ledcSetup(MOTOR_CH_REAR_R,    LEDC_BASE_FREQ, LEDC_TIMER_BIT);

    ledcAttachPin(MOTOR_PIN_FORWARD_L, MOTOR_CH_FORWARD_L);
    ledcAttachPin(MOTOR_PIN_REAR_L, MOTOR_CH_REAR_L);
    ledcAttachPin(MOTOR_PIN_FORWARD_R, MOTOR_CH_FORWARD_R);
    ledcAttachPin(MOTOR_PIN_REAR_R, MOTOR_CH_REAR_R);
}

void setMotorSpeed(int32_t forward_ch, int32_t rear_ch, int speed)
{
    if(speed >= 0){
        Serial.print("fwrd: ch:");
        Serial.print(forward_ch);
        Serial.print(" speed:");
        Serial.print(abs(speed));
        Serial.print(" ");
        ledcWrite(forward_ch, abs(speed));
        ledcWrite(rear_ch, 0);
    }else{
        Serial.print("back: ch:");
        Serial.print(rear_ch);
        Serial.print(" speed:");
        Serial.print(abs(speed));
        Serial.print(" ");
        ledcWrite(forward_ch, 0);
        ledcWrite(rear_ch, abs(speed));
    }
}
void rotateMotor(int8_t y, int8_t x)
{
    speed_l = y + (x / 2);
    speed_r = y - (x / 2);

    setMotorSpeed(MOTOR_CH_FORWARD_L, MOTOR_CH_REAR_L, speed_l);
    setMotorSpeed(MOTOR_CH_FORWARD_R, MOTOR_CH_REAR_R, speed_r);
    Serial.println("");

#if 0
    Serial.print("y:");
    Serial.print(y);
    Serial.print(" x:");
    Serial.print(x);
    Serial.print(" speed L:");
    Serial.print(speed_l);
    Serial.print(" speed R:");
    Serial.print(speed_r);
    Serial.println("");
#endif
}

bool checkTimerInterval(int32_t interval_time)
{
    int32_t cur_time = millis();
    static int32_t pre_check_time = 0;

    int32_t diff_time = cur_time - pre_check_time;
    if(diff_time >= interval_time){
        pre_check_time = cur_time;
        return true;
    }
    return false;
}

void loop()
{
    static uint32_t check_state = 0;

    if(checkTimerInterval(10 * 1000)){
        check_state++;
        if(check_state > MOTOR_CHECK_STATE_FIN){
            check_state = MOTOR_CHECK_STATE_FIN;
        }
        Serial.print("state:");
        Serial.println(check_state);
    }

    switch(check_state){
    case MOTOR_CHECK_STATE_STOP:
    case MOTOR_CHECK_STATE_FIN:
        rotateMotor(0, 0);
        break;
    case MOTOR_CHECK_STATE_FORWARD:
        rotateMotor(0, 100);
        break;
    case MOTOR_CHECK_STATE_BACK:
        rotateMotor(0, -100);
        break;
    case MOTOR_CHECK_STATE_RIGHT:
        rotateMotor(0, 0);
        break;
    case MOTOR_CHECK_STATE_LEFT:
        rotateMotor(0, -0);
        break;
    }

}
