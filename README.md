我們的final project是在ARM的Tiny210上開發出一個連線對戰遊戲
透過wifi連上一台開起的server後
靠著點擊螢幕上的方向鍵，雙方可移動自己圖示的位置並放置炸彈
先炸死對方者獲勝，並有多種道具可以加速遊戲進行
目前這個版本是一開始做的，完整的版本交給教授後忘了備份

# Bomberman on Tiny210

### Introduction
We present a simple multi-player online game on ARM's Tiny210. It's a client-server application implemented in C++ with QT framework. Two players each control a bomberman while the one survived in the end win. The bombermen can place multiple time bombs on the map. When a bomb explode, the bombermen on the same row or column will lose their life points. Sometimes, useful gadgets will randomly appear on the map to make the game more exciting. 

![Screenshot](Screenshot.JPG)
