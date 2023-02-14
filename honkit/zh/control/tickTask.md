# <span style="color:#229999">gcVex::tickTask</span>

**用於儲存多工函式資料，以 linked-list 方式存儲**

---

## <span style="color:#ffaa44">variable</span>
|         type          | name        | description    |
|:---------------------:|:----------- |:-------------- |
|       tickTask*       | nextTaskPtr | 指向下一個task |
|       tickTask*       | lastTaskPtr | 指向上一個task |
| std::function<bool()> | taskFunc    | 儲存的函式     |
|         bool          | isPause     | 是否暫停       |