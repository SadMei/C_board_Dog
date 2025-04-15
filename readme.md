# 9.12

# 板载外设（bsp_peripheral）

## 串口用途

### 底盘板

- 调试
- 裁判系统

### 云台板


- 调试 
- 遥控器
- 图传
- minipc

# 移植说明

***以下内容亲测有效，已在f407/f427/f405板上测试成功（f405因串口4有问题打印不了数据，故还未测试，因没有自定义按键和遥控器遥控的需求，故这两方面不需要移植）***

## 硬件

### LED

### KEY

### 调试用串口

看位号图和原理图接好线

### 遥控器

- 协议等见文档

https://rm-static.djicdn.com/tem/4.RoboMaster%20%E6%9C%BA%E5%99%A8%E4%BA%BA%E4%B8%93%E7%94%A8%E9%81%A5%E6%8E%A7%E5%99%A8%EF%BC%88%E6%8E%A5%E6%94%B6%E6%9C%BA%EF%BC%89%E7%94%A8%E6%88%B7%E6%89%8B%E5%86%8C.pdf

- 对频方法：长按接收机指示灯上的小按键，直到指示灯常绿
- 需要用24v电池给板子供电
- 注意dbus线线序

### 裁判系统

需要自己做串口线（3pin-4pin  /  3pin-3pin）

***一定要注意线序***

## 软件

### cubemx

#### LED

看自己对应板子的原理图，找到端口，初始化为输出即可

#### KEY

看自己对应板子的原理图，找到端口，初始化为输入即可（***一定要设置为上拉或下拉***）

#### 调试用串口

- 随便选一个串口，添加dma的rx和tx，其中rx中选择circular
- 勾选该串口的全局中断
- 检查默认GPIO是否与原理图上的相同

#### 遥控器

- 使能串口（对应于板子上的dbus接口）（***具体是啥看原理图的dbus***），添加dma的rx，选择circular，priority最好选择very high
- 勾选该串口的全局中断
- 检查默认GPIO是否与原理图上的相同

***注意：遥控器通信波特率默认为100000***

#### 裁判系统

- 随便选一个串口，添加dma的rx和tx，其中rx中选择circular
- 勾选该串口的全局中断
- 检查默认GPIO是否与原理图上的相同

### clion

#### LED

- 将User/Framework/LED，User/MCUDriver/LED下的文件拷贝
- 修改cmake（include和file）
- LedC类的构造函数中三个参数分别是GPIOx  GPIO_PIN 和状态
- 要根据自己板子led灯是什么电平点亮修改ledio.cpp文件中的LEDIO_PortSetLedLevel()函数
- 一定要在main函数中初始化，即添加LEDIO_ConfigInit()

#### KEY

- 将User/Framework/KEY，User/MCUDriver/KEY下的文件拷贝
- 修改cmake（include和file）
- KeyC类中的构造函数中三个参数分别是GPIOx  GPIO_PIN 和触发时电平（根据实际情况修改）
- 一定要在main函数中初始化，即添加KEYIO_ConfigInit()

#### 调试用串口

- 将User/Framework/DEBUG中的文件拷贝
- 修改cmake（include和file）
- 将DEBUGC_UartInit()和DEBUGC_UartIrqHandler(UART_HandleTypeDef *huart)函数中有关串口huart的地方改成自己的串口
- 将debugc.cpp中的这三个函数DEBUGC_UartInit()  DEBUGC_UartIrqHandler(UART_HandleTypeDef *huart)  DEBUGC_UartIdleCallback(UART_HandleTypeDef *huart)在usart.h中声明
- 在stm32f4xx_it.c中的对应串口中断函数中调用DEBUGC_UartIrqHandler(UART_HandleTypeDef *huart) 
- 在main函数中加入DEBUGC_UartInit()  初始化对应串口dma中断

***说明：debugc.cpp中对应vofa调参部分，可参考debugc.h中的注释来设置vofa中的控件，也可自行修改***

![image-20220914153311245](/home/cr/.config/Typora/typora-user-images/image-20220914153311245.png)

#### 遥控器

- 将User/Framework/REMOTEC，User/MCUDriver/REMOTEC下的文件拷贝
- 修改cmake（include和file）
- 将REMOTEC_UartIrqHandler(void)和REMOTEIO_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)函数中的有关串口名称改成自己的
- remotec.cpp中REMOTEC_UartIrqHandler(void)函数里设定缓冲区的语句，其中的dma通道要改成自己的串口的rx通道

````c++
//设定缓冲区0
DMA2_Stream2->CR &= ~(DMA_SxCR_CT);
````

- 在usart.h中声明void REMOTEC_UartIrqHandler();
- 在usart.h中添加以下语句

```c
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
```

usart1换成自己所用串口

- 在stm32f4xx_it.c中的对应串口中断函数中调用REMOTEC_UartIrqHandler();
- 在主函数中调用REMOTEC_Init();

#### 裁判系统

- 将User/Framework/REFREE下的文件拷贝
- 修改cmake（include和file）
- 在stm32f4xx_it.c中的串口相关中断函数中添加以下语句

```c
//在串口rx的dma IRQHandler函数中
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */
  Judge_UseDMA_IRQHandler_RX();
  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}
```

```c
//在串口tx的dma IRQHandler函数中
void DMA2_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */
    Judge_UseDMA_IRQHandler_TX();
  /* USER CODE END DMA2_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

  /* USER CODE END DMA2_Stream7_IRQn 1 */
}
```

```c
//在串口全局中断函数中
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
    Judge_UseUART_IRQHandler();
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
```

- 在usart.h中添加以下语句

```c
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;
```

usart6换成自己的串口

- 在主函数中调用如下语句

```c++
Judge_UART_DMA_SET(huart6, hdma_usart6_rx, hdma_usart6_tx);
Judge_Init();
```

串口换成对应串口

#### c板陀螺仪

- 遇到__packed直接无脑删除，无影响（千万别乱添加头文件）
- 注意主函数中初始化顺序，dma的初始化一定要放在spi前面
- 注意stm32f4xx_it.c中的此函数，自己定义的函数一定要放在他默认生成的函数前面

```c
/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */
  SPI_RxCallBack();
  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */
  /* USER CODE END DMA2_Stream2_IRQn 1 */
}
```

### 测试效果

#### LED

#### KEY

#### 调试用串口

#### 遥控器

- 可通过以下语句测试是否移植成功

```c
usart_printf("%d, %d, %d, %d, %d, %d\r\n", rc_ctrl->rc.ch[0], rc_ctrl->rc.ch[1], rc_ctrl->rc.ch[2], rc_ctrl->rc.ch[3], rc_ctrl->rc.s[0], rc_ctrl->rc.s[1]);
```

- 更多遥控器接口请参考remotec.cpp中的遥控器协议解析函数sbus_to_rc()

#### 裁判系统

- 将板子与主控板连接（主控板未上电），串口会有如下提示

![image-20220914200012516](/home/cr/.config/Typora/typora-user-images/image-20220914200012516.png)

- 将主控板上电，可通过如下语句打印比赛剩余时间等变量（更多裁判系统接口函数请见judge.c文件的后半部分）

```c
usart_printf("%d,%d,%d,%d,%d,%d\r\n",Judge_Gametime()->stage_remain_time,Judge_GetRobotState()->robot_level,Judge_GetRobotState()->max_HP,Judge_GetRobotState()->remain_HP,Judge_GetRobotState()->mains_power_chassis_output,Judge_GetRobotState()->mains_power_gimbal_output);
```

- 未开始比赛时（主控板已上电），有如下信息（不应该全为0,全为0大概率是前面的步骤缺失）

![image-20220914200437923](/home/cr/.config/Typora/typora-user-images/image-20220914200437923.png)

- 开始比赛后，可以看到比赛剩余时间在减少。到此，裁判系统移植测试成功。

![image-20220914200539924](/home/cr/.config/Typora/typora-user-images/image-20220914200539924.png)

#### c板陀螺仪

- reset板子时，一定要保证陀螺仪（c板）处于静止状态。不然yaw轴会很偏

# 题外话

## git提交三部曲

- git add ...（若有其他文件夹中的文件需要一起上传，先cp到此本地仓库）
- git commit
- git push

***每日第一事：git pull***

# 调试日记

## PID

- for循环中，不要出现单独的常数下标，如0,1,2... （都统一用i作为下标）
- 
