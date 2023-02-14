# 安裝 gcVex
## 1. VS Code 安裝 VEX Robotics 插件
![](https://hackmd.io/_uploads/rJ1KTPMTi.png)
<br>

## 2. 建立 VEX 專案
### i. 左側欄切換至 VEX 插件介面
![](https://hackmd.io/_uploads/HkICaDfao.png)
<br>

### ii. 選擇 New Project
![](https://hackmd.io/_uploads/ByMHAvzTj.png)
<br>

### iii. 選取 IQ / V5
![](https://hackmd.io/_uploads/Hkpw0wzTo.png)
<br>

### vi. project 檔案結構
![](https://hackmd.io/_uploads/HJfkkdf6s.png)
<br>


## 3. 下載 gcVex 並放置於src中
![](https://hackmd.io/_uploads/HyEXyuMaj.png)
<br>

## 4. 開啟 makefile 並新增以下兩行程式碼
```makefile
SRC_C += $(wildcard src/*/*/*.cpp) 
SRC_C += $(wildcard src/*/*/*.c)
```
![](https://hackmd.io/_uploads/BkuKy_Maj.png)
<br>

## 5. 引用 gcVex.h
若按照以上步驟，引入路徑為
```cpp
#include "gcVex\gcVex.h"
```
<br>

---

{% hint style='tip' %}
恭喜~ 你已經完成安裝，立即開始使用吧 !
{% endhint %}