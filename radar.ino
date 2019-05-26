#define IN1  	8
#define IN2 	9
#define IN3  	10
#define IN4  	11

#define STOP 0

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

enum task_status
{
    IN_PROGRESS,
    DONE
};

struct command
{
    uint32_t steps;
    direction dir;
    uint32_t speed_ms;
};


void step(int8_t dir, uint16_t delay_mili);
void do_scheduled_cmd();
void receive_cmd(command* target_cmd);

command scheduled_cmd;

void setup()
{
	Serial.begin(9600); 
	pinMode(IN1, OUTPUT); 
	pinMode(IN2, OUTPUT); 
	pinMode(IN3, OUTPUT); 
	pinMode(IN4, OUTPUT); 
    scheduled_cmd = {0, A_CLOCKWISE, 0};
}
void loop()
{
    if (Serial.available() > 0) 
    {
        receive_cmd(&scheduled_cmd);
  
    //float volts = analogRead(A0)*0.0048828125;
    //float distance = 13*pow(volts, -1);
    //Serial.println(distance);
    }
    do_scheduled_cmd();
}

void receive_cmd(command* target_cmd)
{
   

    String input = Serial.readString();
    if(input == "stop")
    {
        scheduled_cmd = {0, CLOCKWISE, 0};
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
        scheduled_cmd.dir = CLOCKWISE;
    } 
    else if(c1 == "mova")
    {   
        scheduled_cmd.dir = A_CLOCKWISE;
    } 
    uint32_t _steps = input.substring(dels[1] + 1, dels[2]).toInt();
    uint32_t _delay_ms = input.substring(dels[3] + 1, input.length()-1).toInt();
    scheduled_cmd.steps = _steps;
    scheduled_cmd.speed_ms = _delay_ms;
    Serial.println("rec: " + input.substring(dels[3] + 1, input.length()-1)); 
}

void do_scheduled_cmd()
{
    if(scheduled_cmd.steps > 0)
    {
        step(scheduled_cmd.dir, scheduled_cmd.speed_ms);
        scheduled_cmd.steps--;
    }
}

void step(int8_t dir, uint16_t delay_mili)
{
	static int8_t cmd_counter = 0; 
	cmd_counter = cmd_counter > 4 ? 1 : cmd_counter;
	cmd_counter = cmd_counter < 1 ? 4 : cmd_counter;
	
	
	uint8_t cmd = 0x10 >> cmd_counter;
	
	digitalWrite(IN1, (cmd >> 3) & 0x01); 
	digitalWrite(IN2, (cmd >> 2) & 0x01);
	digitalWrite(IN3, (cmd >> 1) & 0x01);
	digitalWrite(IN4, cmd & 0x01);
	cmd_counter += dir;
	delay(delay_mili);
}

