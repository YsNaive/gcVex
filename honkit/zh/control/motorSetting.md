# <span style="color:#229999">gcVex::motorSetting</span>

**用於儲存馬達設定資料**

---

## <span style="color:#ffaa44">variable</span>
|      type      | name                 | description |
|:--------------:|:-------------------- |:----------- |
|      int       | port_init            | 馬達的 port 孔            |
|     float3     | pid                  | PID 參數權重            |
| vex::brakeType | breakType            |  煞車模式           |
|      bool      | isReversed_init      |   是否馬達反轉          |
|      bool      | isResetPosition_init |    是否重置馬達 Position         |

{% hint style='working' %}
帶有 _init 後綴的參數無法進行熱更新
{% endhint %}