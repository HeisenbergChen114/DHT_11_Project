#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <time.h>

#include <sys/time.h>

// gcc dht11_reciver.c -o dht11_app.out -Wall -lwiringPi

// 如果要编译成dll，应用在python，则需要做如下的gcc
// gcc -shared -Wl,-o dht11.so  -lwiringPi -fPIC dht11_reciver.c ,-soname

// gcc -shared -Wl,-soname,<myfilename.c> -o <setfilename.so> -fPIC <myfilename.c> -lwiringPi

// 发送，接受并解析温湿度传感器的数据

void myusleep(unsigned int us)
{
    struct timespec ts;
    ts.tv_sec = us / 1000000;
    ts.tv_nsec = (us % 1000000) * 1000;
    nanosleep(&ts, NULL);
}

int read_bit(int pin) // 接受一个数据接收的针脚
{
    // time_t t1, t2; // 记录一个bit的发送开始时间和结束时间
    struct timespec start, end;

    while (digitalRead(pin) == LOW)
        ; // 等待低电平的间隙结束
    // delayMicroseconds(28); // 延时28us，跳过数据0应有的持续时间
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (digitalRead(pin) == HIGH)
    {
        clock_gettime(CLOCK_MONOTONIC, &end);
        long elapsed = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        if (elapsed > 100000)
        {
            return 2; // 高电平时间100ms太长，超时
        }
    }

    long elapsed =((end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));

    return (elapsed > 55000) ? 1 : 0; // 高电平时间大于55us，返回1，否则返回0

    // if (digitalRead(pin) == HIGH)
    // {
    //     t1 = time(NULL);
    //     while (digitalRead(pin) == HIGH) // 等待高电平的结束
    //     {
    //         t2 = time(NULL);
    //         if (t2 - t1)
    //         {
    //             return 2; // 高电平时间太长，超时
    //         }
    //     }
    //     return 1; // 读取1
    // }
    // else
    // {
    //     return 0; // 读取0
    // }
}

int read_byte(int pin)
{
    int val = 0;
    for (int i = 0; i < 8; i++)
    {
        val <<= 1; // 空出位移一位
        int retval = read_bit(pin);
        if (retval == 2)
        {
            return -1; // 超时
        }
        val |= retval; // 相当于+=retval
    }
    return val;
}
// 读取数据,return 1 成功，0 校验失败，2 未响应，3 超时
int dht11_read_data(int pin, int *buff)
{
    // 初始化针脚
    pinMode(pin, OUTPUT); // 输出
    delay(30);
    digitalWrite(pin, HIGH);
    delay(30);

    // 开始信号
    digitalWrite(pin, LOW);  // 输出低电平
    usleep(20000);           // 延长低电平时间至20ms
    digitalWrite(pin, HIGH); // 输出高电平
    // delayMicroseconds(30);   // 暂停30us保证高电平变化完整
    usleep(40); // 高电平40us

    // 反馈接受
    pinMode(pin, INPUT); // 输入模式，把dht11的数据传入程序
    /*


    if (digitalRead(pin) == LOW)
    {
        while (digitalRead(pin) == LOW)
            ;
        while (digitalRead(pin) == HIGH)
            ; // 防止呆滞

        // 读取数据
        for (int i = 0; i < 5; i++)
        {
            buff[i] = read_byte(pin);
            if (buff[i] == -1)
            {
                return 3; // 超时退出
            }
        }

        while (digitalRead(pin) == LOW)
            ;
        pinMode(pin,OUTPUT);
        digitalWrite(pin,HIGH);

        //校验
        int checksum=0;
        for(int i=0;i<4;i++){
            checksum+=buff[i];
        }
        if(checksum!=buff[4]){
            return 0;//校验出错
        }else{
            return 1;
        }

    }
*/

    // 防呆模式
    while (digitalRead(pin) == LOW)
    {
        // 等待低电平结束
    }
    while (digitalRead(pin) == HIGH)
    {
        usleep(10);
        // 等待高电平结束
        if (digitalRead(pin) == LOW)
        {
            break;
        }
    }

    // 读取数据
    int data[5];
    int valid = 1;

    for (int i = 0; i < 5; i++)
    {
        data[i] = read_byte(pin);
        if (data[i] == -1)
        {
            valid = 0;
            break;
        }
    }

    if (!valid)
    {
        return 3; // 超时
    }

    int checksum = 0; // 校验和
    for (int i = 0; i < 4; i++)
    {
        checksum += data[i];
    }
    if (checksum != data[4])
    {
        return 0; // 校验失败
    }
    // 输出数据
    for (int i = 0; i < 5; i++)
    {
        buff[i] = data[i];
    }

    return 1; // 输出成功
}
/*

int main()
{
    int data[5]={};
    int pin=24;
    wiringPiSetupGpio();//BCM 模式
    while(1){
        switch(dht11_read_data(pin,data))
        {
            case 0:
                {
                    printf("校验失败\n");
                    printf("================\n");
                    break;
                }
            case 1:
            {
                printf("温度：%d.%d°C\n",data[2],data[3]);
                printf("湿度：%d.%d%%RH\n",data[0],data[1]);
                printf("================\n");
                break;

            }
            case 2:
            {
                printf("dht11未响应..\n");
                printf("================\n");
                break;
            }
            case 3:
            {
                printf("读取超时\n");
                printf("================\n");
                break;
            }
            default:
                break;
        }
        sleep(1);
    }

    return 0;
}

*/

int dht11_reciver_init()
{
    return wiringPiSetupGpio(); // BCM 模式
}
