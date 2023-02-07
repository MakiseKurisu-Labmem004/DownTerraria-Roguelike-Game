# DownTerraria-Roguelike-Game
大一期末项目模拟Downwell自制一款肉鸽游戏

## 平台
Windows11
## C语言环境
mingw64(GCC 12.1.0)
## vscode编译指令
```
cd "......" ; if ($?) { gcc DownTerraria.c -fexec-charset=gbk -o DownTerraria -Wall -O2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer} ; if ($?) { .\DownTerraria }
```
>注意 ...... 为DownTerraria.c文件所在路径,例：  e:\DownTerraria\ （E盘DownTerraria文件夹）
## 使用的第三方库
1. SDL2-2.0.18
2. 南大云盘中的ttf,image,mixer插件
## 游戏方式
1. ad或左右方向键均支持,空格跳跃或射击,E键与NPC交互
2. 注意初始界面和原版相同,需要自己向右跑到井里(异度侵入/错乱)
3. 注意有些石头需要射击两次就碎了,不要傻傻不动
4. 第三关BOSS战之前最底部石头无法破坏,E键与向导交互召唤BOSS后可破坏下去和BOSS对战
5. 游戏结束后需要鼠标点击“Retry”或“Exit”,购买或选择道具也需鼠标点击
6. 拾取钱币和红心不需要额外操作，一定范围内自动收集
7. 屏幕大小700*720
## 设计思路
灵感来源于Downwell和Terraria,怪物行为模式和道具基本尊重Terraria并贴合背景,使用了大量音效,有很多小细节(自认为有点辛苦😢)希望玩得开心!🤤\
[b站视频(https://www.bilibili.com/video/BV1MG4y1S7iz/?spm_id_from=333.1007.top_right_bar_window_default_collection.content.click&vd_source=42cd11de1c1ccb0642bf7102fa2800a1)]
