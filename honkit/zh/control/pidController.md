<!-- toc -->
# <span style="color:#229999">gcVex::pidController</span>

**用於PID控制器的計算，除了一般的利用函式回傳使用外，也能選擇用函式指標簡化整體調用。**

---

## <span style="color:#ffaa44">variable</span>
此類別不提供公有變量

---

## <span style="color:#ffaa44">function</span>


### void setOnCalculateError (std::function<float()> onCalculateError);
> 設定在調用 update() 前計算誤差的函式

### void setOnUpdated (std::function<void(float)> onUpdated);
> 設定在 update() 後應用回饋參數的函式

### void setPID(float3 pid);
> 使用 float3 格式設定新的 PID

### void setPID(float p, float i, float d);
> 使用3個 float 設定新的 PID

### float nextError(float nextError);
> 輸入下個迭代的誤差，回傳本次反饋參數，通常不會和 update() 同時使用

### void update();
> 調用後進入下一個迭代
> **你必須先設定好 OnUpdated 和 OnCalculateError 函式才能使用此功能**

### float3 getPID();
> 取得當前 PID

### float3 getError();
> 取得當前誤差，格式為 **{ 當前, 累積, (上次-當前) }**

### float getFixValue();
> 取得當前的反饋參數

### void reset();
> 重製控制器，會被重製的內容包括:
> - 當前誤差
> - 累積誤差
> - 上次誤差
---

## <span style="color:#ffaa44">constructor</span>
### pidController();
> 建構所有參數為 0 / nullptr 的控制器

### pidController(float3 pid);
> 建構帶有PID參數的控制器

### pidController(float3 pid,std::function<float()> onCalculateError,std::function<void(float)> onUpdated);
> 建構完整的控制器，可使用 update()簡化調用

#### sample
```cpp
pidController controller = pidController(
    float3(0.2, 0, 0.04),
    [&](void) mutable -> float { 
        /* return the error */
        return 
        },
    [&](float fix) mutable -> void {
        /* apply change with var "fix" */

    });
```

---

## <span style="color:#ffaa44">程式範例</span>

### 不使用簡化調用 ( 使用 nextError() )
```cpp
/*--- 馬達幾本設定 ---*/
motor leftMotor = motor(PORT12);
motor rightMotor = motor(PORT7);
leftMotor.resetPosition();
rightMotor.resetPosition();
leftMotor.setReversed(true);
rightMotor.setReversed(false);
leftMotor.spin(directionType::fwd);
rightMotor.spin(directionType::fwd);

/*--- pid 設定 ---*/
pidController controller = pidController(float3(0.2, 0, 0.04))

/*--- 持續執行兩秒 ---*/
vex::timer time = vex::timer();
while (time.time(vex::timeUnits::sec)<2)
{
    controller.nextError(leftMotor.position(rotationUnits::deg) - rightMotor.position(rotationUnits::deg));
    
    leftMotor.setVelocity(40 - controller.getFixValue(),percentUnits::pct);
    rightMotor.setVelocity(40 + controller.getFixValue(),percentUnits::pct);

    vex::wait(15, vex::timeUnits::msec);
}

/*--- 停止動作 ---*/
leftMotor.stop();
rightMotor.stop();
```


### 使用簡化調用 ( 使用 update() )
```cpp
/*--- 馬達幾本設定 ---*/
motor leftMotor = motor(PORT12);
motor rightMotor = motor(PORT7);
leftMotor.resetPosition();
rightMotor.resetPosition();
leftMotor.setReversed(true);
rightMotor.setReversed(false);
leftMotor.spin(directionType::fwd);
rightMotor.spin(directionType::fwd);

/*--- 直線 tank drive 行為設定 ---*/
pidController controller = pidController(
    float3(0.2, 0, 0.04),
    // 使用 lambda 定義誤差來源函式
    [&](void) mutable -> float { 
        return leftMotor.position(rotationUnits::deg) - rightMotor.position(rotationUnits::deg); 
        },
    // 使用 lambda 定義反饋值利用
    [&](float fix) mutable -> void {
        leftMotor.setVelocity(40 - fix,percentUnits::pct);
        rightMotor.setVelocity(40 + fix,percentUnits::pct);
    });

/*--- 持續調用 Update 兩秒 ---*/
vex::timer time = vex::timer();
while (time.time(vex::timeUnits::sec)<2)
{
    controller.update();
    vex::wait(15, vex::timeUnits::msec);
}

/*--- 停止動作 ---*/
leftMotor.stop();
rightMotor.stop();
```