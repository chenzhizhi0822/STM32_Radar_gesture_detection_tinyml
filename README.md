# STM32_gesture_detection
# TinyML 的 STM32 实现流程 & 手势识别项目实现

### 流程图：
![image](https://github.com/user-attachments/assets/c3bd0b98-ef03-44b3-9a84-54c5ff51f698)

### 主循环逻辑：
![image](https://github.com/user-attachments/assets/a53e0982-1471-436a-8e42-4a9655617c1c)

### 准备: 
软件: STM32CubeMX最新(生成项目框架代码，导入AI模型)，Keil uVision5(编译，模拟环境中debug), STM32CubeProgrammer(连板子，烧录)，Matlab(雷达原始数据处理、特征提取，封装函数转C语言)，Google Colab网页 或其他 Python3+tensorflow2.0环境(训练ai分类模型)
模型：sine_model.tflite, gesture_model.tflite
数据集：手势识别特征数据，6000x91，
一共6个手势每个手势，每个手势1000个样本，每个样本6特征x15帧=90个特征，1个对应标签
train数据集：x_train(4800x90), y_train(4800x1)
test数据集：test_data.txt(1200x90), test_label.txt(1200x1)
