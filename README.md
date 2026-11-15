# RGB-DRIVER-2027

NEU-ACTION 2027 赛季 RGB 灯带驱动固件，基于 STM32F030F4P6。

## 硬件

| 项目 | 参数 |
| --- | --- |
| MCU | STM32F030F4P6（Cortex-M0，TSSOP20） |
| 主频 | 48 MHz（HSI + PLL） |
| Flash / RAM | 16 KB / 4 KB |
| 灯带 | 2 路 RGB，共 6 个 PWM 通道 |
| 上位机通信 | USART1，115200 8N1 |

## 引脚与 PWM 通道映射

| 灯带 | 颜色 | 定时器通道 | 引脚 |
| --- | --- | --- | --- |
| wheel1 | R | TIM1_CH2 | PA9 |
| wheel1 | G | TIM1_CH3 | PA10 |
| wheel1 | B | TIM3_CH4 | PB1 |
| wheel2 | R | TIM3_CH1 | PA6 |
| wheel2 | G | TIM3_CH2 | PA7 |
| wheel2 | B | TIM14_CH1 | PA4 |

PWM 参数：TIM1 预分频 48-1、TIM3/TIM14 预分频 47，周期（ARR）均为 225，
计数时钟 1 MHz，PWM 频率约 4.4 kHz，占空比分辨率 0–225。

## 串口协议

每帧 9 字节，以 `'\n'` 结尾，帧头为 `'I' 'O'`：

| 字节 | 含义 |
| --- | --- |
| 0 | `'I'`（0x49） |
| 1 | `'O'`（0x4F） |
| 2 | wheel1 R（0–225） |
| 3 | wheel1 G |
| 4 | wheel1 B |
| 5 | wheel2 R |
| 6 | wheel2 G |
| 7 | wheel2 B |
| 8 | `'\n'`（0x0A） |

上位机示例（Python）：

```python
import serial

ser = serial.Serial("COM3", 115200)
frame = bytes([ord('I'), ord('O'), 255, 0, 0, 0, 0, 255, ord('\n')])
ser.write(frame)   # wheel1 红、wheel2 蓝
```

接收实现：USART1 以 `HAL_UART_Receive_IT` 单字节中断收数，主循环轮询
`receive()` 组帧；校验通过后更新 6 个颜色变量，主循环随即刷新 6 路 PWM 占空比。
串口出错（ORE/FE/NE/PE）时由 `HAL_UART_ErrorCallback` 清标志并重新开启接收。

## 目录结构

```
RGB-DRIVER-2027/
├── Core/
│   ├── Inc/            # main.h、UART.h、HAL 配置与中断声明
│   └── Src/            # main.c（初始化 + PWM 输出）、UART.c（组帧协议）
├── Drivers/
│   ├── CMSIS/          # ARM CMSIS 内核与 DSP 库
│   └── STM32F0xx_HAL_Driver/   # ST HAL 驱动
├── MDK-ARM/            # Keil MDK 工程
│   ├── RGB-DRIVER-2027.uvprojx # 双击打开
│   └── startup_stm32f030x6.s
├── RGB-DRIVER-2027.ioc # STM32CubeMX 工程文件
└── .mxproject
```

## 编译与下载

1. 安装 Keil MDK 5，并装好 `Keil.STM32F0xx_DFP` 器件包。
2. 打开 `MDK-ARM/RGB-DRIVER-2027.uvprojx`。
3. `Project → Build Target`（F7）编译，产物在 `MDK-ARM/RGB-DRIVER-2027/`（已被 .gitignore 忽略）。
4. 用 J-Link / ST-Link 下载：`Flash → Download`（F8）。

命令行编译（可选）：

```bat
"C:\Keil_v5\UV4\UV4.exe" -b MDK-ARM\RGB-DRIVER-2027.uvprojx -j0 -o build.log
```

## 用 STM32CubeMX 修改配置

`.ioc` 文件可直接用 CubeMX 打开重新生成代码；重新生成时注意保留
`/* USER CODE BEGIN */ ... /* USER CODE END */` 之间的代码。
