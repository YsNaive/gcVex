# <span style="color:#229999">gcVex::motorController</span>

**馬達的控制器，帶有PID控制器以提高其表現**

---

## <span style="color:#ffaa44">variable</span>
|         type          | name        | description    |
|:---------------------:|:----------- |:-------------- |
|       vex::motor       | motor | 此控制器控制的馬達 |

---

## <span style="color:#ffaa44">function</span>

### motorState getState();
> 取得當前控制器的狀態，詳見 motorState

### int getPosition();
> 取得當前馬達角度 (deg)

### void setSetting(motorSetting setting);
> 設定此控制器的基本參數，**部分參數不支援熱更新，需要調用 init() 進行重整**

### void setPID(float3 pid);
> 設定此控制器的 PID 參數權重

### void setBreakType(vex::brakeType brakeType);
> 設定馬達的煞車模式

### void init();
> 將 motorSetting 的參數應用，在建構此物件時會進行調用

### void off();
> 關閉馬達，**這將同時停止多工的其他指令**

### void off(vex::brakeType breakType);
> 關閉馬達並設定新的煞車模式

### bool isEncArrived();
> 馬達是否已達到角度目標，使用者通常不需調用此函式

### motorController* waitCmd();
> 調用此函式時將宣告下一個指令需等待其執行完畢
> 可受此影響的函式如下
> - onEnc()
> - onTime()
> - onToPosition()

{% hint style='danger' %}
此指令將會造成 tick 系統暫停，此時若有其他執行中的多工函式，請確保不會因其受到影響
{% endhint %}

#### 範例
``` cpp
// 往前100度並等待其執行完畢
motorController.waitCmd().onEnc(50, 100);

// 以多工方式往前100度
motorController.onEnc(50, 100);

```

### void on(int power);
> 持續以指定馬力旋轉

### void onEnc(int power, int enc, vex::brakeType breakType = vex::brakeType::undefined);
> 旋轉指定角度，預設到達後將以之前模式煞車，或指定新的煞車模式

### void onTime(int power, int sencend, vex::brakeType breakType = vex::brakeType::undefined);
> 旋轉指定時間，預設到達後將以之前模式煞車，或指定新的煞車模式

### void onToPosition(int power, int position); 
> 旋轉到指定角度，**到達後將不會煞車**

### void close();
> 關閉此設備，若要再次啟動需調用`init()`否則無法使用多工功能

### bool isOff();
> 回傳目前是否是 off 模式

---

## <span style="color:#ffaa44">constructor</span>
### motorController();
> 預設建構 port0

### motorController(int port);
> 指定馬達孔位

### motorController(motorSetting setting);
> 完整建構，詳細設定見 motorSetting

#### 範例
```cpp
/* 建立基本設定 */
motorSetting mSetting;
mSetting.breakType = vex::brakeType::hold;
mSetting.isResetPosition_init = true;
mSetting.isReversed_init = true;
mSetting.pid = float3(0.4, 0, 0.12);
mSetting.port = PORT12;

/* 建構馬達控制器 */
motorController motor = motorController(mSetting);
```
