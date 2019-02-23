**This file will be rewritten. Currently only draft**

## 可能需要用到的Qt类
+ QPainter：用来绘制图形
+ QPainterPath：控制Painter绘制的路径
+ QRegion：控制图像刷新的范围
+ QGraphicsObject
+ QGraphicsView
+ QGraphicsScene
+ QGraphicsPixmapItem
+ QSettings

## Introduction and Objective
1. 该游戏为类似于我的世界（Minecraft）的一款2D沙盒游戏
2. 游戏支持由给定的地图种子创建唯一确定的地图
3. 玩家拥有物品栏，可以更换装备
4. 玩家可以通过破坏地图中的方块对方块进行收集
5. 玩家可以使用收集到的方块对地图进行自定义的改造，并可以用来合成设备与装备
6. 玩家的进度可以保存
7. 游戏中存在敌对生物与中立生物，敌对生物主要包括骷髅射手（远程进攻），僵尸（近战）以及爬行者（靠近玩家然后爆炸）；中立生物主要包括牛（提供皮革可以用来合成护甲，提供肉作为食物），羊（提供肉类），鸡（提供羽毛可以用来合成箭矢，提供肉类）。
8. 玩家可以建造工具台，用来合成各种工具。
9. 玩家可以建造箱子用来储存物品。
## Requirements and Functionality
**TODO**

## Logic Flow 
1. 由种子产生地图或者加载之前以及储存的地图
2. 游戏主循环：
   1. 处理键盘输入，控制玩家与环境交互
   2. 控制NPC活动，对玩家活动做出反应
   3. 渲染更新后的地图以及玩家状态
3. 保存玩家状态以及更新后的地图
## Relation between Data Structure
1. 由一个给定的地图种子（seed）创建一个合理的1000*1000的地图
2. 地图的更新与储存
3. 敌对生物的寻路算法
## Plan and Schedule
1. 大家一起确定主题以及程序架构
2. ？？同学负责中期报告
3. ？？？同学负责最终报告
4. ？？？同学负责展示
5. ？？？同学负责程序的编写

## 草稿区
### 确定的方块种类
+ 基岩方块：0
+ 草方块：1
+ 土方块：2
+ 圆石方块：3
+ 木头方块：4
+ 树叶方块：5
+ 水方块：6

### 确定的生物种类
+ 玩家
+ 牛
+ 羊
+ 僵尸
+ 骷髅
+ 爬行者
+ 蜘蛛

### 生物掉落物
+ 腐肉
+ 骨头
+ 蛛丝
+ 生肉
+ 火药

### 确定的武器/工具种类
+ 剑
+ 斧
+ 铲
+ 镐
+ 弓

### 所支持的玩家行为
+ 移动
+ 破坏方块
+ 摆放方块
+ 使用近战武器
+ 使用远程武器
+ 合成物品

### UI
+ 玩家的物品栏
+ 玩家的状态栏
+ 生物的移动

### 地图的组成
+ 树：由一个树叶方块和一个木头方块组成
+ 森林：一片树
+ 河：一片条形的水方块所组成
+ 大地：一层基岩方块，一层土方块，一层草方块
+ 山：一片圆石

### 键盘控制
+ 使用up，down，left，donw控制窗口移动
+ 使用w，a，s，d控制主角移动
+ 使用左键进行攻击/破坏
+ 使用右键放置方块
+ 使用滚轮切换手持物品
+ 使用E键打开物品栏