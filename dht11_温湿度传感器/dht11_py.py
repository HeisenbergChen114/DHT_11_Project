from ctypes import *
import time
import os
import datetime


class class_dht11:
    def __init__(self, pin):   
        self.__pin=pin 
        
        self.__lib=cdll.LoadLibrary(os.path.dirname(os.path.realpath(__file__)) + '/lib/libdht11_weather.so')
        self.__data= (c_int*5)()#生成c的数组
        self.__ptr= pointer(self.__data)#指向数组的指针

    
    def dht11_read_data(self):
        self.__lib.dht11_reciver_init()
        #函数指针保存
        f_read_data =self.__lib.dht11_read_data
        #设定函数的参数类型和返回值类型
        f_read_data.argtypes=[c_int,POINTER(c_int*5)]
        f_read_data.restype=c_int

         
        return f_read_data(self.__pin,self.__ptr)
    
    def get_data(self):
        return self.__data  # 提供一个方法来访问私有属性 __data
    
    # def save_dht11_data(self):#创建一个dht11_data.txt来保存时间和温湿度数据
    #     with open("dht11_data.txt","a") as f:
    #         now =datetime.datetime.now()
    #         #格式化时间
    #         time_str=now.strftime('%Y-%m-%d %H:%M:%S')
    #         humidity_data=str("humidity :")+str(self.__data[0])+"."+str(self.__data[1])+"%"
    #         temperature_data=str("temperature :")+str(self.__data[2])+"."+str(self.__data[3])+"°C"
    #         f.write(time_str+';'+humidity_data+';'+temperature_data+'\n')
       
    def save_dht11_data(self, save_strategy='txt'):
        # 根据传入的参数选择保存策略
        if save_strategy == 'txt':
            self.save_as_txt()
        elif save_strategy == 'csv':
            self.save_as_csv()
        else:
            print(f"Unknown save strategy: {save_strategy}")

    def save_as_txt(self):
        # 保存为txt文件
        with open("dht11_data.txt", "a") as f:
            now = datetime.datetime.now()
            time_str = now.strftime('%Y-%m-%d %H:%M:%S')
            humidity_data = f"humidity :{self.__data[0]}.{self.__data[1]}%"
            temperature_data = f"temperature :{self.__data[2]}.{self.__data[3]}°C"
            f.write(time_str + ';' + humidity_data + ';' + temperature_data + '\n')

    def save_as_csv(self):
        # 保存为csv文件
        with open("dht11_data.csv", "a") as f:
            now = datetime.datetime.now()
            time_str = now.strftime('%Y-%m-%d %H:%M:%S')
            humidity_data = f"{self.__data[0]}.{self.__data[1]}%"
            temperature_data = f"{self.__data[2]}.{self.__data[3]}°C"
            f.write(f"{time_str},{humidity_data},{temperature_data}\n")


        
        
        
        
        
if __name__ == '__main__':
    pin=24 # 设置数据pin位置
    dht11=class_dht11(pin)
    #print(dht11.__lib)
    #f_read_data=dht11.dht11_read_data#把函数地址放入变量
    
    try:
        while True:
            res=dht11.dht11_read_data()
            os.system('cls' if os.name == 'nt' else 'clear')#clear terimal
            # 格式化时间，不显示毫秒
            current_time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            print(f"The Current Time is - [{current_time}]")  # 输出日期和时间
            
            if res==1:
                data = dht11.get_data()
                print(f"Temperature is {data[2]}.{data[3]}°C, Humidity is {data[0]}.{data[1]}%")#输出温湿度
                # dht11.save_dht11_data(save_strategy='csv')
                
            if res==0:
                print("校验失败")
            if res==2:
                print("未响应")
            if res==3:
                print("响应超时")
            else:
                pass
            time.sleep(5)
            #print("========================")
    except KeyboardInterrupt:
        
        pass
        