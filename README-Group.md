# 写在前面：
## 关于Markdown
本地md（Markdown）后缀的文件可以采用Notepad++进行编辑

下载后点击左上角“文件”进行文件创建

然后点击上方“语言”选择Markdown语言即可编写

项目中的新.md文件不会被推送上GitHub


## 关于github项目维护
在VS界面上方点击“Git”

点击“拉取”更改最新项目

点击“推送”推送最新项目，在推送前最好先拉取


## 关于合并文件

务必遵循以下原则

一头一元 先outer后inner

tile.h

standardtile

stringify

shanten

fan_calculator

Mahjong

StatusMemory

RequestInfor

FanCalculator

ResponseInfor

Main




# 项目状态：

已实现：

麻将类，状态存储器类部分,算番器调用,上听数计算器调用,响应部分



待修改：

诈胡现象（番数<8番时胡牌）

番数计算错误，以下为无法单纯依靠算番器计算，需要状态检查的番型：


特殊番型无法计算（需根据状态存储器检查或添设额外成员变量）：

单钓，边张，坎张，抢杠，杠上开花//海底捞月，妙手回春，和绝张



更多番型见：https://zh.wikipedia.org/wiki/%E5%9B%BD%E6%A0%87%E9%BA%BB%E5%B0%86%E7%95%AA%E7%A7%8D%E5%88%97%E8%A1%A8?oldformat=true



待实现：

胡牌逻辑的优化，响应部分的多段优化（高阶进攻，防守）



# 更新日志

2024/4/1 上传了包含麻将类和状态存储器在内基本框架

2024/4/1 上传了接收部分

2024/4/3 实现了响应部分（初级）

2024/4/7 实现基本胡牌

