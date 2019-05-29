#define IN1  	8
#define IN2 	9
#define IN3  	10
#define IN4  	11

#define WRITE_DST 1

/*
 * STEP MOTOR 28YJ-48
 * STEP DRIVER ZC-A0591
 * SENSOR SHARP 0A41SK F 3Z
 */

enum direction
{
    CLOCKWISE = 1,
    A_CLOCKWISE = -1
};


struct command
{
    uint32_t steps;
    direction dir;
    uint32_t speed_ms;
    bool done;
};


void step(int8_t dir, uint16_t delay_mili);
void do_scheduled_cmd();
void receive_cmd_s(command* target_cmd);
void send_data(char op1, float v1, char op2, float v2);

float sensor_dst();

command scheduled_cmd;
int32_t step_cnt = 0;

void setup()
{
	Serial.begin(9600); 
	pinMode(IN1, OUTPUT); 
	pinMode(IN2, OUTPUT); 
	pinMode(IN3, OUTPUT); 
	pinMode(IN4, OUTPUT); 
    scheduled_cmd = {0, A_CLOCKWISE, 0, true};
}
void loop()
{
    if (Serial.available() > 0) 
    {
        receive_cmd_s(&scheduled_cmd);  
    }
    do_scheduled_cmd();
}

void receive_cmd_s(command* target_cmd)
{
   

    String input = Serial.readString();
    if(input == "stop")
    {
        *target_cmd = {0, CLOCKWISE, 0, true};
        return;
    }
    int dels[4];
    dels[0] = 0;
    for(int i = 1; i < 4; i++)
    {
        dels[i] = input.indexOf('.', dels[i - 1] + 1);
    }
    String c1 = input.substring(0,4);

    if(c1 == "movc")
    {
        target_cmd->dir = CLOCKWISE;
    } 
    else if(c1 == "mova")
    {   
        target_cmd->dir = A_CLOCKWISE;
    } 
    target_cmd->steps = input.substring(dels[1] + 1, dels[2]).toInt();
    target_cmd->speed_ms = input.substring(dels[3] + 1, input.length()-1).toInt();
    if(target_cmd->steps > 0)
    {
        target_cmd->done = false;
    }
}

void do_scheduled_cmd()
{
    if(scheduled_cmd.steps > 0)
    {
        step(scheduled_cmd.dir, scheduled_cmd.speed_ms);
        scheduled_cmd.steps--;

        #if WRITE_DST == 1
        send_data('s', step_cnt, 'd', sensor_dst());       
        #endif
    }
    else
    {
        if(!scheduled_cmd.done)
        {
            scheduled_cmd.done = true;
            send_data('e', 0, 0, 0);
            
        }
    }

}

void send_data(char op1, float v1, char op2, float v2)
{
    Serial.print(op1);
    Serial.print(" ");
    Serial.print(v1);
    Serial.print(" ");
    Serial.print(op2);
    Serial.print(" ");
    Serial.println(v2);
}

float sensor_dst()
{
    float volts = analogRead(A0) * 0.0048828125;
    float distance = 13 * pow(volts, -1);
    return distance;
}

void step(int8_t dir, uint16_t delay_mili)
{
	static int8_t cmd_counter = 0; 
	cmd_counter = cmd_counter > 4 ? 1 : cmd_counter;
	cmd_counter = cmd_counter < 1 ? 4 : cmd_counter;
	
    step_cnt += dir;	


	uint8_t cmd = 0x10 >> cmd_counter;
	
	digitalWrite(IN1, (cmd >> 3) & 0x01); 
	digitalWrite(IN2, (cmd >> 2) & 0x01);
	digitalWrite(IN3, (cmd >> 1) & 0x01);
	digitalWrite(IN4, cmd & 0x01);
	cmd_counter += dir;
	delay(delay_mili);
}

