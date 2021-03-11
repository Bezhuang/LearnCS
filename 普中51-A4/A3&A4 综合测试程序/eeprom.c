#include "eeprom.h"
#include "smg.h"
#include "key.h"
#include "led.h"

void Delay10us()
{
	unsigned char a,b;
	for(b=1;b>0;b--)
		for(a=2;a>0;a--);

}

void I2cStart()
{
	SDA=1;
	Delay10us();
	SCL=1;
	Delay10us();//建立时间是SDA保持时间>4.7us
	SDA=0;
	Delay10us();//保持时间是>4us
	SCL=0;			
	Delay10us();		
}

void I2cStop()
{
	SDA=0;
	Delay10us();
	SCL=1;
	Delay10us();//建立时间大于4.7us
	SDA=1;
	Delay10us();		
}

unsigned char I2cSendByte(unsigned char dat)
{
	unsigned char a=0,b=0;//最大255，一个机器周期为1us，最大延时255us。		
	for(a=0;a<8;a++)//要发送8位，从最高位开始
	{
		SDA=dat>>7;	 //起始信号之后SCL=0，所以可以直接改变SDA信号
		dat=dat<<1;
		Delay10us();
		SCL=1;
		Delay10us();//建立时间>4.7us
		SCL=0;
		Delay10us();//时间大于4us		
	}
	SDA=1;
	Delay10us();
	SCL=1;
	while(SDA)//等待应答，也就是等待从设备把SDA拉低
	{
		b++;
		if(b>200)	 //如果超过2000us没有应答发送失败，或者为非应答，表示接收结束
		{
			SCL=0;
			Delay10us();
			return 0;
		}
	}
	SCL=0;
	Delay10us();
 	return 1;		
}

unsigned char I2cReadByte()
{
	unsigned char a=0,dat=0;
	SDA=1;			//起始和发送一个字节之后SCL都是0
	Delay10us();
	for(a=0;a<8;a++)//接收8个字节
	{
		SCL=1;
		Delay10us();
		dat<<=1;
		dat|=SDA;
		Delay10us();
		SCL=0;
		Delay10us();
	}
	return dat;		
}

void At24c02Write(unsigned char addr,unsigned char dat)
{
	I2cStart();
	I2cSendByte(0xa0);//发送写器件地址
	I2cSendByte(addr);//发送要写入内存地址
	I2cSendByte(dat);	//发送数据
	I2cStop();
}

unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num;
	I2cStart();
	I2cSendByte(0xa0); //发送写器件地址
	I2cSendByte(addr); //发送要读取的地址
	I2cStart();
	I2cSendByte(0xa1); //发送读器件地址
	num=I2cReadByte(); //读取数据
	I2cStop();
	return num;	
}

u8 eeprom_num=0;

void Keypros()
{
	u8 key=0;
	key=KeyScan(0);
	
	if(key==KEY1)
	{
		At24c02Write(1,eeprom_num);   //在地址1内写入数据num	
	}
	else if(key==KEY2)
	{
		eeprom_num=0;		 //数据清零	
	}	
	else if(key==KEY3)
	{
		eeprom_num=At24c02Read(1);	  //读取EEPROM地址1内的数据保存在num中	
	}
	else if(key==KEY4)
	{
		eeprom_num++;	   //数据加1
		if(eeprom_num>255)eeprom_num=0;
	}
		
}

void EEPROM_datapros()
{
	DisPlayData[0]=smgduan[eeprom_num/1000];//千位
	DisPlayData[1]=smgduan[eeprom_num%1000/100];//百位
	DisPlayData[2]=smgduan[eeprom_num%1000%100/10];//个位
	DisPlayData[3]=smgduan[eeprom_num%1000%100%10];	
	DisPlayData[4]=0;
	DisPlayData[5]=0;
	DisPlayData[6]=0;
	DisPlayData[7]=0;	
}

void EEPROM_Test(void)
{
	EA=0;
	KEY_PORT=0xff;
	while(1)
	{
		Keypros();	 //按键处理函数
		EEPROM_datapros();	 //数据处理函数
		DigDisplay();//数码管显示函数
		if(KeyValue!=EEPROM_TEST)
		{
			return;	
		}
	}
}
