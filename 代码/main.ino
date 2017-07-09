#define BUTTONS_address   "channel/widget4_0/cmd/control" //开关命令
#define LIGHT_STATUS_address  "channel/widget4_0/data/light"//开关状态
#define ITENSITY_DATA_address "channel/widget4_0/data/lightsensor"
#define LEDPIN1    D1    //定义灯泡控制引脚
#define LEDPIN2    D2
#define LEDPIN3    D3
#define LEDPIN4    D5
#define CHECKIN1   A0
#define CHECKIN2   D4


int autostate = 2;
int light_state = 2;
void buttons_function(uint8_t *payload, uint32_t len)//自动&浇水按钮
{
	uint8_t SwitchKey;
	uint8_t SwitchKey2;
	aJsonClass aJson;
	aJsonObject *root = aJson.parse((char *)payload);
	if(root == NULL)
	{
		aJson.deleteItem(root);
		return;
	}
	aJsonObject *_switch = aJson.getObjectItem(root, "mode");
	if(_switch != NULL)
	{
		SwitchKey = atoi(_switch->valuestring);
		if(SwitchKey)
		{
			SerialUSB.println("auto on");
			autostate=1;
			 IntoRobot.publish(LIGHT_STATUS_address,"1");
		}
		else
		{
			SerialUSB.println("auto off");
			autostate=0;
			 IntoRobot.publish(LIGHT_STATUS_address,"0");
		}
	}
	aJsonObject *_switch2 = aJson.getObjectItem(root, "manual");
	if(_switch2 != NULL)
	{
		SwitchKey2 = atoi(_switch2->valuestring);
		if(SwitchKey2)
		{
			SerialUSB.println("manual on");
			light_state=1;
			 IntoRobot.publish(LIGHT_STATUS_address,"1");
		}
		else
		{
			SerialUSB.println("manual off");
			light_state=0;
			 IntoRobot.publish(LIGHT_STATUS_address,"0");
		}
	}
	else
	{
	}
	aJson.deleteItem(root);
}
void lightup()
{
	digitalWrite(LEDPIN1, HIGH);	// 打开灯泡
	digitalWrite(LEDPIN2, HIGH);	// 打开灯泡
	digitalWrite(LEDPIN3, HIGH);	// 打开灯泡
	digitalWrite(LEDPIN4, HIGH);	// 打开灯泡
	
}
void light_half_up()
{
	analogWrite(LEDPIN1, 80);	// 打开灯泡
	analogWrite(LEDPIN2, 80);	// 打开灯泡
	analogWrite(LEDPIN3, 80);	// 打开灯泡
	analogWrite(LEDPIN4, 80);	// 打开灯泡

}
void lightdown()
{
	digitalWrite(LEDPIN1, LOW);	
	digitalWrite(LEDPIN2, LOW);
	digitalWrite(LEDPIN3, LOW);
	digitalWrite(LEDPIN4, LOW);	
	
}
int getlight()
{
	int k  = analogRead(CHECKIN1);
	
	SerialUSB.println(k);
	return k;
}
int get_IR_data()
{
	int b = digitalRead(CHECKIN2);
	SerialUSB.println(b);
	return b;
}
void automode()
{
	if(getlight()>=400)
	{
	    IntoRobot.publish(LIGHT_STATUS_address,"1");
		if (get_IR_data()==0)
		lightup();
		else
		light_half_up();
	}
	else
	{
	IntoRobot.publish(LIGHT_STATUS_address,"0");
	lightdown();
	}
}

void HUMIDITY_print_function(uint8_t *payload, uint32_t len) 
{
    

}

// IntoRobot.publish(LIGHT_STATUS_address,"1");
// IntoRobot.publish(LIGHT_STATUS_address,"0");
void setup()
{
    pinMode(D4,INPUT);
	SerialUSB.begin(115200);
	SerialUSB.println("hello world");
	pinMode(LEDPIN1, OUTPUT);	//初始化
	pinMode(LEDPIN2, OUTPUT);	//初始化
	pinMode(LEDPIN3, OUTPUT);	//初始化
	pinMode(LEDPIN4, OUTPUT);	//初始化
	//设备接收云平台的灯开关命令
	IntoRobot.subscribe(BUTTONS_address,NULL,buttons_function);
	IntoRobot.subscribe(ITENSITY_DATA_address,NULL,HUMIDITY_print_function);
}
void loop()
{
   int a =map(getlight() ,0,1024,100,0);
   IntoRobot.publish(LIGHT,a);
    SerialUSB.println(getlight());
	if(autostate==0)
	{
		if(light_state ==1)
		lightup();
		else 
		lightdown();  
	}
	else if (autostate==1)
	{
		SerialUSB.println("state=1");
		automode();
	}
	delay(100);
}