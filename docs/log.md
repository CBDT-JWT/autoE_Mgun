# 项目规划
## 项目分解
```mermaid
        graph TD
        A1(智能电磁炮)-->B1[硬件]
        A1-->B2[软件]
        B1-->C1[功率系统]
        B1-->C2[信号回路]
        C1-->D1[升压电路]-->E0(ZVS/boost)
        C1-->D2[线圈回路]-->E4(IGBT关断,光电)
        C1-->D3[EMS]
        C2-->D3
        C2-->D4[机载摄像头]
        C2-->D5[手持终端]
        D5-->E1[陀螺仪]
        D5-->E2[摄像头]
        D5-->E3[显示及UI]
        B2-->C3[...]
        C2-->D6[姿态调整装置]
        C2-->D7[供弹装置等]
```
```mermaid
gantt
dateFormat MM-DD
axisFormat %m/%d
title 开发时间轴
section 禁赛
实践 :  crit,xx1, 07-04, 1w
section 电路硬件
升压电路设计 :hw11, 06-23, 2d
升压电路打板:hw12, after hw11, 5d
升压电路调试:hw13, after hw12 hw11 , 2d
驱动电路设计 :hw21, after hw11, 2d
驱动电路打板: hw22, after hw21, 5d
section 软件
```
## 前期准备
### 软件下载
- 电路仿真软件 `MULTISIM14` https://cloud.tsinghua.edu.cn/d/d82f36fe9607472283ef/
- PCB设计软件 `Altisim` https://www.altium.com/ 注册账号并申请教育版
- `AutoCAD` https://www.autodesk.com.hk/products/autocad 注册账号并申请教育版
## DEV timeline
### 240622
- 立项。创建githubrepo。
- 完成ZVS电路仿真设计