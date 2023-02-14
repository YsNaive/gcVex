# <span style="color:#229999">gcVex::tick</span>

**用於全域的 tick clock ，可以視為一個 static class，不需要使用者進行任何設定。**
{% hint style='working' %}
你需要在程式一開始的地方調用 `tick::init()` 來初始建構這個功能，若沒有進行此動作將無法使用有關多工的內容。
{% endhint %}

---

## <span style="color:#ffaa44">variable</span>
此類別不提供公有變量

---

## <span style="color:#ffaa44">static function</span>

### static tickTask* newTask(std::function<bool()> func);
> 建立一個多工執行函式，回傳其指標，通常使用者不需要調用此函式

### static void pauseTask(tickTask* task);
> 暫停一個多工函式，傳入其指標

### static void resumeTask(tickTask* task);
> 繼續一個多工函式，傳入其指標

### static void colseTask(tickTask* task);
> 關閉一個多工函式，**這將會直接釋放其佔用的記憶體，不可回復**

### static void init();
> 初始化 tick system，再次調用將會重置系統

### static int getTick();
> 取得當前的 tick

### static int getTime();
> 取得當前時間(msec)

### static int countTask();
> 取得當前執行的多工函式數量(包含暫停中的函式)

### static tickTask* addTask(std::function<bool()> func);
> 新增一個多工函式，通常使用 lambda 直接宣告，詳見下方範例

### static tickTask* addTask(tickTask* task);
> 新增一個多工函式，來源於已建立的實體

### static void pause();
> 暫停系統，**所有 tickTask 將同時暫停**

### static void resume();
> 繼續系統

### static void debug();
> print 偵錯資訊

---

## <span style="color:#ffaa44">constructor</span>
調用 `tick::init()` 來進行初始建構

---

## <span style="color:#ffaa44">程式範例</span>

### 添加一個多工函式 (tickTask)
新建多工函式時需要回傳一個 bool
以決定是否要關閉此 tickTask (視同調用 tick::closeTask(this))
``` cpp
tick::addTask([&](void)mutable->bool{
            /* your code here */
    
            /* return wheather close the task */
            return isClosed;
        });
```