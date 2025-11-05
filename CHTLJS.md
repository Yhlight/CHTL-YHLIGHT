## CHTL JS
CHTL JS是CHTL项目的扩展语法，并不是JS的超集，也不支持JS的语法
CHTL JS完全独立于JS，是一门独立的编程语言，与JS毫无关系，只是最终转变为JS代码

CHTL JS的函数皆为声明式语法
均支持无序键值对，可选键值对，无修饰字面量

本语法规范属于CHTL JS，由于CHTL与CHTL JS存在一些交互信息
为此语法规范会包含CHTL的一部分规范
请你忽略这部分规范

## 职权
CHTL不具有处理JS的能力，CHTL只服务于HTML + CSS，不服务于JS，不会处理那些具有动态特征的语法
这些动态特征皆通过盐桥，CHTL JS编译器等组件，让CHTL JS去处理
盐桥是CHTL与CHTL JS之间的通信桥梁

通常情况下，script块的内容将由CHTL JS编译器进行解析，必要时会通过盐桥请求CHTL编译器，反之CHTL编译器也会通过盐桥请求CHTL JS编译器

### 文件后缀
对于包含CHTL JS的JS文件，你可以命名为*.cjjs

### 预处理器
由于CHTL JS会与JS代码混合使用，这会大大增加解析的难度
为此CHTL JS应该在语法分析之前进行预处理
将CHTL JS语法放入[__CHTLJS__] ... [__CHTLJSEND__]之中
这样就能够大大减少可能的冲突

对于那些嵌套在CHTL JS深层的JS语法，也能够得到很好的解决
如果开发者能够显性使用[__CHTLJS__] ... [__CHTLJSEND__]进行包裹，这会大大提高解析效率

预处理器是一个很重要的组件，它将负责CHTL JS和CJMOD

```cjjs
let x = {{box}};
```

```cjjs
let x = [__CHTLJS__]{{box}}[__CHTLJSEND__];
```

### 脚本加载器
你可以使用ScriptLoader {}来导入文件
CHTL JS实现了AMD风格JavaScript文件加载器
目的是让开发者能够无序引入js文件，无需考虑加载顺序，文件依赖等问题
支持无序键值对，可选键值对，无修饰字面量

```chtl
你可以使用ScriptLoader {
    load: ./module.cjjs,
    load: ./module2.cjjs,
    load: ./module3.cjjs,
    load: ./module4.cjjs
    load: ./module5.js  // 普通js文件也可以

    load:
    ./module.cjjs
    ,./module2.cjjs
    ,./module3.cjjs
    ,./module4.cjjs
    ,./module5.js  // 也支持链式语法
}
```

### 局部script
CHTL允许在元素中使用script{}来编写JS代码
局部script会被添加到一个不会全局污染，具有高优先级的全局script块之中

```chtl
div
{
    style
    {
        .box
        {
            width: 100px;
            height: 100px;
            background-color: red;
        }
    }

    script
    {
        {{box}}.addEventListener('click', () => {
            console.log('Box clicked!');
        });

        // 引用功能
        {{&}}->addEventListener('click', () => {
            console.log('Box clicked!');
        });
    }
}
```

### 增强选择器
你可以使用{{CSS选择器}}来创建一个DOM对象
支持无修饰字面量

```chtl
button
{
    style
    {
        .box
        {

        }
    }
}

script
{
    {{box}}.textContent()  // 先判断是否为tag，然后查找类名 / id = box(id优先)的元素，并创建DOM对象
    {{.box}}  // 查找类名为box的元素，并创建DOM对象
    {{#box}}  // 查找id为box的元素，并创建DOM对象
    {{button}}  // 所有的button元素
    {{.box button}}  // 查找类名为box的元素的所有的button后代，并创建DOM对象

    // 精确访问
    {{button[0]}}  // 第一个button元素

    // 增强选择器仅支持上述的种类，这是出于性能与复杂性之间的考虑
    // .boxbutton这种交集选择器会消耗很多性能，因此这里不得不放弃支持
    // 增强选择器不同于CSS属性条件表达式的选择器(因为那个不需要解析)，无法做到多类型支持
}
```

#### 自动化规则
你可以在[Configuration]之中添加下述规则禁用自动化

```chtl
[Configuration]
{
    // 禁止局部脚本自动添加类选择器
    DISABLE_SCRIPT_AUTO_ADD_CLASS = true;
    // 禁止局部脚本自动添加id选择器
    DISABLE_SCRIPT_AUTO_ADD_ID = true;
}
```
如果DISABLE_SCRIPT_AUTO_ADD_CLASS 和 DISABLE_SCRIPT_AUTO_ADD_ID 为真
当局部script内部存在多组类选择器时，若class属性，局部style没有触发class自动化添加缺失，第一个{{.box}}会被自动添加
当局部script内部存在多组id选择器时，若id属性，局部style没有触发id自动化添加缺失，第一个{{#box}}会被自动添加
{{box}}不会自动添加，只有{{.box}}和{{#box}}能够触发自动化添加
对于局部script来说，& 引用选择器优先选择id

### 明确使用CHTL语法
使用到CHTL JS语法时，我们推荐使用->代替.
以便明确使用了CHTL JS语法
->与.是完全等价的，因此你可以直接使用->进行链式访问

```chtl
button
{
    style
    {
        .box
        {

        }
    }
}

script
{
    {{box}}->textContent();
}
```

### 增强监听器
你现在可以使用Listen来快捷绑定事件监听器
支持无序键值对，可选键值对，无修饰字面量

```chtl
button
{
    style
    {
        .box
        {

        }
    }
}

script
{
    // 声明式
    {{box}}->Listen {
        click: () => {

        },

        mouseenter: mouseEnterEvent,  // 已经存在的函数

        mousemove: function() {

        }
    };
}
```

### 事件绑定操作符
你现在可以使用 &-> 快捷为一个DOM对象绑定事件
支持无序键值对，可选键值对，无修饰字面量

```chtl
{{box}} &-> click: () => {};
```

#### 多事件绑定
你可以使用 , 分隔多个事件，同时为多个事件绑定同一个函数

```chtl
{{box}} &-> click, mouseenter, mouseleave: () => {};
```

#### 链式绑定
事件绑定操作符支持链式绑定，注意使用 , 分隔

```chtl
{{box}} &-> click: () => {},
        &-> mouseenter: () => {},
        &-> mouseleave: () => {};
```

#### 绑定块
如果上述用法不适合你，你可以使用绑定块

```chtl
{{box}} &-> {
    click: () => {},
    mouseenter: () => {},
    mouseleave: () => {}
}
```

### 事件委托
为了解决SPA页面中元素动态更新导致事件监听丢失的问题，提供了基于事件委托的增强语法
通过将事件绑定到不会销毁的父元素，监听冒泡阶段的事件，从而实现稳定的事件绑定
支持无序键值对，可选键值对，无修饰字面量

```chtl
script
{
    {{父元素选择器}}->Delegate {
        target: {{选择器}} | [{{选择器1}}, {{选择器2}},...], // 要代理的子元素对象 / 子元素对象数组
        click: 函数,  // 绑定的事件类型及对应回调函数
        mouseenter: 函数,
        mouseleave: 函数,
    };
}
```
需要创建一个全局注册表，管理所有事件委托的父元素，重复绑定父元素的子元素会作为分支合并在同一个事件委托之中
避免创建多个相同的事件委托

### 动画
CHTL JS简化了动画的使用，封装了requestAnimationFrame
支持无序键值对，可选键值对，无修饰字面量

```chtl
script
{
    const anim = Animate {
        target: {{选择器}} || [{{选择器1}}, {{选择器2}}] || DOM对象
        duration: 100,  // 动画持续时间，ms
        easing: ease-in-out,  // 缓慢函数

        begin: {  // 起始状态，写css代码

        }

        when: [
            {
                at: 0.4;  // 动画播放到什么时刻

                // css代码
                opacity: 0.5,
                transform: 'scale(1.1)'
            },
            {
                at: 0.8;  // 动画播放到什么时刻
                // css代码
            }
        ]

        end: {  // 终止状态，写css代码

        }

        loop: -1,  // 循环次数
        direction: ,  // 播放的方向
        delay:  ,  // 开始播放的延迟，ms
        callback: function,  // 播放完毕后做什么
    };
}
```

### 虚对象
虚对象是CHTL JS重要的特征之一，虚对象提供了访问CHTL JS函数的元信息能力
虚对象能够获取CHTL JS函数的任意键的键值
支持无序键值对，可选键值对，无修饰字面量

```
Vir test = Listen {
    click: () => {

    }

    other: {

    }
};

test->click();  // 解析click为函数引用
test->other;  // 解析other为对象
```

Vir是CHTL JS层面的语法糖，不涉及JS
Listen会按原样生成JS代码
Vir本身就不存在，是编译期间的语法糖

```
Vir Test = Listen {
    click: ()=>{

    }
};
```
编译器扫描到Vir时，会创建一个C++对象，这个C++对象负责Vir的解析
假设这个对象为View
View对象需要做两件事情，一件是记录Vir虚对象的名称，第二个是解析CHTL JS函数中的键，并创建对应表
后续在解析时，遇到Test->click;时，会根据键值的类型，转换成不同的内容，比如函数引用，对象，数组等，并且这些结果会缓存在View之中，以便后续的解析

### 路由
你可以使用路由快速创建SPA页面的基本架构
支持无序键值对，可选键值对，无修饰字面量

```chtl
Router {  // 1对1的关系
    url: "/home",
    page: {{选择器}},
}

Router {  // 1对1的关系
    url: "/home", "/about"
    page: {{选择器1}}, {{选择器2}}
}
```

```chtl
Router {  // 1对1的关系，对象形式
    page: {"/home", {{选择器}}}
        , {"/about",{{选择器2}}}
}

Router {  // 设置根路径 / 设置根容器
    root: "/" || {{选择器}},  // 默认根路径 / 变化元素的根容器，这是二选一形式
}

Router {  // 设置根路径的同时设置根容器
    root: {"/", {{选择器}}},  // 也可以是全选，对象的形式
}

Router {  // 模式
    mode: "history" || "hash",  // 路由的模式
}
```

### 动态属性条件表达式
无论是属性条件表达式，还是引用属性条件表达式，其本质都是静态的
而CHTL JS提供了动态的属性条件表达式
全局style不支持这一种用法

```chtl
div
{
    id: box;
    style
    {
        width: 100px;
    }
}

div
{
    style
    {
        width: {{box}}->width > 2 ? 100px : 50px;  // 动态变化
    }
}
```


### 动态条件渲染
静态条件渲染必然是存在限制的，为此，CHTL JS提供了动态条件渲染

```chtl
html
{
    head
    {

    }

    body
    {
        if
        {
            condition: {{html}}.width < 500px,  // 使用动态属性 / 响应式值时，使用动态条件渲染
            display: none,
        }
    }
}
```

### 响应式值
这是CHTL与CHTL JS的首次交互尝试
响应式值属于CHTL JS的范畴，但是与CHTL交互
语法：$JS变量名$

```chtl
div
{
    class = $boxClass$;  // 根据JS变量来设置值

    style
    {
        width: $boxWidth$ + 20px;  // 根据JS变量来设置值，算术运算自然是支持的
    }

    script
    {
        let boxClass = "box";  // 绑定的JS变量
        let boxWidth = 100;  // 绑定的JS变量

        {{boxClass}}.textContent = "Hello, CHTL!";
    }
}
```
同样，全局样式块因为缺少层级概念，为此响应式值不支持全局样式块

## 模块
### 模块路径
什么是模块路径？
模块路径就是CHTL编译器存放模块源码 / 模块搜索等功能的路径

主要有如下规则
源码文件夹 -> Module文件夹
官方模块目录 -> 编译器二进制文件所在目录的module文件夹
用户模块目录  ->  编译文件所在目录的module文件夹

上述的文件夹都支持两种结构，无序结构和有序结构
无序结构：cmod，chtl，cjmod文件混杂在一起，不分文件夹
有序结构：使用cmod / Cmod / CMOD + cjmod / CJmod / CJMOD两个文件夹进行分类

### CJMOD
CJMOD是CHTL JS提供一种模块化方式，开发者可以使用CHTL JS提供的CJMOD API来实现CHTL JS语法
CJMOD让扩展CHTL JS语法变得更简单，无需阅读源码即可实现CHTL JS语法的扩展，并通过CJMOD的方式发布
CJMOD通常会被CHTL JS的预处理器注册，然后使用[__CJMOD__] ... [__CJMODEND__]进行包裹
如果开发者能够显性使用[__CJMOD__] ... [__CJMODEND__]进行包裹，这会大大提高解析效率

#### 模块结构
CJmod具有严格的模块结构
对于没有子模块的CJMOD，通常结构如下，CJMOD无需像CJMOD一样，需要模块文件夹名称，主模块文件，信息文件三者同名
仅仅需要模块文件夹与信息文件两者同名即可

```chtl
CJmod文件夹
    src/xxx.cpp xxx.h
    info/CJmod文件夹名称.chtl
```

#### 模块信息
与CMOD不同，CJMOD的info文件无法使用[Export]，这源于CJMOD的特殊性
CJMOD只需要使用[Info]即可

```chtl
[Info]
{
    name = "chtholly";
    version = "1.0.0";
    description = "珂朵莉主题模块 - 世界上最幸福的女孩";
    author = "CHTL Team";
    license = "MIT";
    dependencies = "";
    category = "theme";
    minCHTLVersion = "1.0.0";
    maxCHTLVersion = "2.0.0";
}
```

#### 包含子模块的模块结构
```chtl
CJmod文件夹
    src/xxx.cpp xxx.h
        Box
            src/xxx.cpp xxx.h
            info/Box.chtl
        Box2
            src/xxx.cpp xxx.h
            info/Box2.chtl
    info/CJmod文件夹名称.chtl
```
对于包含子模块的模块结构，仅且只能像上述演示的结构一样创建
主文件直接位于src之下，其他内容则位于相关的模块文件夹内部
同样，CJMOD也严格使用src + info的结构

#### 模块的导入
导入CJmod文件
[Import] @CJmod from cjmod文件路径

对于@CJmod类型来说
名称（不带后缀）：优先搜索官方模块目录，其次搜索当前目录module文件夹，最后搜索当前目录，仅匹配cjmod文件
具体名称（带后缀）：按官方模块目录→当前目录module文件夹→当前目录顺序搜索指定文件
具体路径（含文件信息）：直接按路径查找，未找到则报错
具体路径（不含文件信息）：触发报错
对于使用官方模块前缀，直接在官方模块目录中搜索

### CMOD + CJMOD
如果你想要提供组件的同时提供扩展CHTL JS语法
你可以使用CMOD + CJMOD的混合模块结构
CMOD + CJMOD的混合模块始终使用src + info的结构

注：CMOD + CJMOD的混合模块结构最终得到的是CMOD模块文件

#### 模块结构
```chtl
模块名称
    CMOD / Cmod / cmod
        src/
            (模块名称.chtl)  // -> 主模块文件，子模块存在时可选
            (xxx.chtl)  // -> 其他模块文件
            Box  // ->子模块
                src/Box.chtl, Other.chtl
                info/Box.chtl
            Box
        info/模块名称.chtl
    CJMOD / CJmod / cjmod
        src/xxx.cpp xxx.h  // -> 主模块文件，必须
            Box  // ->子模块
                src/xxx.cpp xxx.h
                info/Box.chtl
        info/模块名称.chtl
```
注意，CMOD和CJMOD并不共用信息文件，你可以吧CMOD + CJMOD的混合模块结构，视为一个module文件夹，内部存放着标准的cmod和cjmod模块
CMOD和CJMOD文件夹内部实际使用的是标准的CMOD和CJMOD的组织方式
由于信息文件的不共用，你可以自由控制CMOD和CJMOD的版本，允许他们在不同的版本之中兼容

如何使用？例如这个模块叫Box，那么如果我想要调用Box的CMOD模块时，我们直接使用[Import] @Chtl即可
如果需要使用CJMOD，需要使用[Import] @CJmod，CHTL不会对此进行统一处理，我们不推荐使用@Chtl同时管理CMOD和CJMOD

### 通配符
[Import] @Chtl from 具体路径.*  // 导入具体路径下的所有.cmod和.chtl文件
[Import] @Chtl from 具体路径.*.cmod  // 导入具体路径下的所有.cmod文件
[Import] @Chtl from 具体路径.*.chtl  // 导入具体路径下的所有.chtl文件
等价于
[Import] @Chtl from 具体路径/*  // 导入具体路径下的所有.cmod和.chtl文件
[Import] @Chtl from 具体路径/*.cmod  // 导入具体路径下的所有.cmod文件
[Import] @Chtl from 具体路径/*.chtl  // 导入具体路径下的所有.chtl文件

// 导入子模块时，支持使用'/'替代'.'作为路径分隔符，因为在CHTL中，'.'和'/'等价
[Import] @Chtl from Chtholly.*  // 导入Chtholly模块的所有子模块
[Import] @Chtl from Chtholly.Space  // 导入Chtholly模块中指定的Space子模块

### 官方模块
#### 官方模块前缀
你可以使用"chtl::"明确表明使用官方模块
[Import] @Chtl from chtl::Chtholly  // 导入官方模块Chtholly

#### Chtholly 珂朵莉模块
珂朵莉对于我来说，是一个很特别的角色，是我一直喜欢着的人物，我希望我能让珂朵莉成为CHTL中重要的模块
珂朵莉模块采用CMOD + CJMOD的混合模块

##### CMOD
###### 手风琴
###### 四叶窗相册
###### 备忘录
###### 暖色笔记
###### 樱花雨
###### 鼠标特效
###### 鼠标拖尾
###### 视差滚动背景
###### 右键菜单栏
###### 进度条

##### CJMOD
###### PrintMylove
PrintMylove可以将一张图片变成字符像素块的形式，你可以使用PrintMylove来把图片转换成字符像素块或ASCII的形式
然后输出到控制台
支持无序键值对，可选键值对，无修饰字面量
```chtl
const str = PrintMylove {
    url: ,
    mode: ,  // 模式可以选择ASCII或Pixel
    width: ,  // 宽度，支持的单位有CSS单位以及百分比，小数，纯数字(像素)
    height: ,  // 高度
    scale:  ,  // 缩放倍数，限定为等比缩放策略
};
```

#### Yuigahama 由比滨结衣模块
由比滨结衣模块使用CMOD

##### 音乐播放器
##### 手风琴
##### 四叶窗相册
##### 备忘录
##### 暖色笔记
##### 樱花雨
##### 鼠标特效

编译生成的module文件夹(官方模块)有两种结构，一种就是常规的混杂，chtl文件，cmod，cjmod
一种是使用CMOD / cmod / Cmod(包括chtl文件) + CJMOD / cjmod / CJmod(不包括chtl文件)两个文件夹进行分类

对于用户来说，他们创建的module文件夹也能够使用分类结构
值得一提，源代码目录下的模块源码目录Module也可以使用分类结构

[Import] @Chtl from 具体路径.*  // 导入具体路径下的所有.cmod和.chtl文件
[Import] @Chtl from 具体路径.*.cmod  // 导入具体路径下的所有.cmod文件
[Import] @Chtl from 具体路径.*.chtl  // 导入具体路径下的所有.chtl文件
等价于
[Import] @Chtl from 具体路径/*  // 导入具体路径下的所有.cmod和.chtl文件
[Import] @Chtl from 具体路径/*.cmod  // 导入具体路径下的所有.cmod文件
[Import] @Chtl from 具体路径/*.chtl  // 导入具体路径下的所有.chtl文件

// 导入子模块时，支持使用'/'替代'.'作为路径分隔符
[Import] @Chtl from Chtholly.*  // 导入Chtholly模块的所有子模块
[Import] @Chtl from Chtholly.Space  // 导入Chtholly模块中指定的Space子模块

[Import] @CJmod from 模块名称  // 导入指定名称的CJmod模块

CJmod与Cmod采用相同的路径搜索策略。

## 项目结构建议
由于CHTL项目的复杂性，我们强烈建议使用状态机(state) + 策略模式模式(Strategy)来组织CHTL项目
递归下降也可以，只是会复杂很多

```chtl
CHTL(项目文件夹，可以换成src)
    -CHTL(CHTL编译器)
        -CHTLContext
        -CHTLGenerator
        -CHTLLexer(内含Lexer，GlobalMap，Token文件)
        -CHTLLoader
        -CHTLManage(如果需要管理器)
        -CHTLNode(内含BaseNode文件，ElementNode，TextNode，CommentNode，TemplateNode，CustomNode，StyleNode，ScriptNode，OriginNode，ImportNode，ConfigNode，NamespaceNode，OperatorNode(delete，insert，use...)等文件)
        -CHTLParser
        -CHTLState
        -CHTLStrategy
	    -CHTLIOStream(如果需要IO流)
        -CMODSystem
        ... (模块化开发)

    -CHTLJS
        -CHTLJSContext
        -CHTLJSGenerator
        -CHTLJSLexer(Lexer，GlobalMap，Token文件)
        -CHTLJSLoader
        -CHTLJSManage(如果需要管理器)
        -CHTLJSNode(CHTLJSBaseNode等CHTL JS节点)
        -CHTLJSParser
        -CHTLJSState
        -CHTLJSStrategy
	    -CHTLJSIOStream(如果需要IO流)
        -CJMODSystem
        ... (模块化开发)

    -CSS

    -JS

    -SharedCore(盐桥，共享CHTL和CHTL JS的数据)

    -ThirdParty

    -Util
        FileSystem(文件夹)
	    ZipUtil(文件夹)
	    StringUtil(文件夹)

    -Test
        -UtilTest(文件夹，提供UtilTest功能)
        -TokenTest(文件夹，提供TokenPrint和TokenTable功能)
        -AstTest(文件夹，提供ASTPrint，ASTGraph)

    -Module(模块源码文件夹)
```

## 项目流程
                    ┌─────────────┐                   ┌─────────────┐
                    │    CHTL     │     盐桥机制       │   CHTL JS   │
                    │  Compiler   │   <---------->    │  Compiler   │
                    │  (手写)     │                    │  (手写)      │
                    └─────────────┘                    └─────────────┘
                        │                                    │
                        └──────────────----------------------┴
                            ▼ CHTL和CHTL JS处理后的完整代码

                            ... 借助外部编译器进行语法验证
                    ┌─────────────┐ ┌─────────────┐
                    │     CSS     │ │JavaScript   │
                    │  Compiler   │ │  Compiler   │
                    │  (ANTLR)    │ │  (ANTLR)    │
                    └─────────────┘ └─────────────┘
                        │              │
                        └──────────────┴
                              ▼
                            ┌───────────
                            │编译结果合并│
                            │(HTML输出) │
                            └──────────

编译器对应关系说明如下：
- 局部样式（style）→ CHTL编译器
- 全局样式（style）→ CSS编译器
- 脚本（script）→ 由CHTL编译器、CHTL JS编译器及JS编译器共同管理

注意，CHTL项目依托两个外部引入的CSS / JS编译器，你可以使用ANTLR4 / 合适的库引入CSS / JS编译器，不强制使用ANTLR4
这里有一些可选推荐  ->  ANTLR4，libcss，sassc，V8，QuickJS

## 语法边界
全局样式块仅允许使用以下语法元素：属性运算，属性条件表达式，模板变量，自定义变量，自定义变量特例化，模板样式组，自定义样式组，无值样式组，自定义样式组特例化，delete属性，delete继承，样式组间继承，生成器注释，全缀名，任意类型原始嵌入（原始嵌入可在任意位置使用），以及通过命名空间引入模板变量，自定义变量，模板样式组，自定义样式组，无值样式组，即from语法

除局部script外，其他script禁止使用任何CHTL语法，允许例外的元素为：注释及任意类型原始嵌入(二者为特殊存在，可在任意位置使用)

局部样式块允许使用以下语法元素：模板变量，自定义变量，自定义变量特例化，模板样式组，自定义样式组，无值样式组，自定义样式组特例化，delete属性，delete继承，样式组间继承，生成器注释，全缀名，任意类型原始嵌入(原始嵌入可在任意位置使用)，以及通过命名空间引入模板变量，自定义变量，模板样式组，自定义样式组，无值样式组，即from语句

局部script允许使用以下CHTL语法元素：模板变量，自定义变量组，变量组特例化，命名空间from，注释及任意类型原始嵌入(注释和原始嵌入为特殊存在，可在任意位置使用)，{{&}}属于CHTL与CHTL JS的交互语法，上述的行为都需要盐桥的介入

## CJMOD API
### 简介
CJMOD是CHTL项目CHTL JS模块的扩展组件，用于提供模块化分发CHTL JS代码
CHTL提供了CJMOD API，CJMOD API极其强大，能够高效创建CHTL JS语法

### CJMOD API
#### 使用案例
```cpp
Arg args = Syntax::analyze("$ ** $");  // 语法分析
args.print();  // 输出-> ["$", "**", "$"]

args.bind("$", [](const std::string& value) {
    return value;
});

args.bind("**", [](const std::string& value) {
    return value;
});

args.bind("$", [](const std::string& value) {
    return value;
});

Arg result = CJMODScanner::scan(args, "**");
result.print();  // 输出-> ["3", "**", "4"]

args.fillValue(result);
std::cout << arg[0].value << std::endl;  // 输出-> 3
std::cout << arg[1].value << std::endl;  // 输出-> **
std::cout << arg[2].value << std::endl;  // 输出-> 4

// args.transform("pow(" + args[0].value + args[2].value + ")");

args.transform("pow(". +
args.match("$", [](const std::string& value) { return value; }) +
args.match("$", [](const std::string& value) { return value; }) +
")");

CJMODGenerator::exportResult(args);
```

```chtl
script
{
    console.log(3 ** 4);
}

->

<script>
    console.log(pow(3, 4));
</script>
```

##### Syntax
语法分析类，这个类负责对语法进行解析

###### analyze
分析语法，返回一个Arg对象，Arg对象包含了解析出的参数列表

```cpp
Arg args = Syntax::analyze("$ ** $");  // 语法分析
args.print();  // 输出-> ["$", "**", "$"]
```

###### isObject
判断是否是JS对象

```cpp
Syntax::isObject("{b: 1}");  // 输出-> true
```

###### isFunction
判断是否是JS函数

```cpp
Syntax::isFunction("function a(){}");  // 输出-> true
```

###### isArray
判断是否是JS数组

```cpp
Syntax::isArray("[1, 2, 3]");  // 输出-> true
```

###### isCHTLJSFunction
判断是否是CHTL JS函数

```cpp
Syntax::isCHTLJSFunction("test {test: 1, test2: 2};");  // 输出-> true
```

##### Arg
参数列表类，这个类包含了解析出的参数列表，并且提供了参数的绑定、填充、转换等操作

###### bind
让一个原子Arg绑定获取值的函数

```cpp
Arg args = Syntax::analyze("$ ** $");  // 语法分析
args.bind("$", [](const std::string& value) {
    return value;
});

args.bind("**", [](const std::string& value) {
    return Syntax::isCHTLJSFunction(value) ? "" : value;
});
```

###### match
匹配参数列表中的参数
会自动对匹配到的占位符进行计数 + 1

例如第一次匹配$时，参数列表中的参数$的计数为1，表示第一个$参数
第二次匹配$时，参数列表中的参数$的计数+1，表示第二个$参数

```cpp
args.match("$", [](const std::string& value) { return value; });
```

###### fillValue
填充参数列表的值

```cpp
Arg result = CJMODScanner::scan(args);
args.fillValue(result);
args.fillValue(Arg(["3", "**", "4"]));
```

###### transform
参数最终输出什么JS代码

```cpp
args.transform("pow(" + args[0].value + args[2].value + ")");
```

##### CJMODScanner
特殊的扫描器，用于扫描语法片段

###### scan
扫描语法片段，第二参数为扫描的关键字

```cpp
Arg result = CJMODScanner::scan(args, "**");
```
scan方法需要拿到真实的代码片段

#### CJMODGenerator
生成器用于CMJMOD API的接口

###### exportResult
导出最终的JS代码

```cpp
CJMODGenerator::exportResult(args);
```

#### AtomArg
原子参数，Arg封装此

##### $
占位符

##### $?
可选占位符

##### $!
必须占位符

##### $_
无序占位符

上述占位符可以组合
例如$!_

##### ...
不定参数占位符

##### bind
绑定获取值的函数

```cpp
args[0].bind([](const std::string& value) {return value;});
```

##### fillValue
填充参数值

```cpp
args[0].fillValue("3");
args[0].fillValue(3);
```

#### CHTLJSFunction
CHTL JS函数，用于CJMOD API的接口

##### CreateCHTLJSFunction
封装了原始API构建语法的流程，能够快速构建CHTL JS函数，这些CHTL JS函数天然支持虚对象Vir以及无修饰字符串
构建的函数天然支持无序键值对，可选键值对，无修饰字面量

```cpp
CHTLJSFunction func;
CHTLJSFunction::CreateCHTLJSFunction("PrintMylove {url: $!_, mode: $?_}");
```

```chtl
script
{
    PrintMylove({url: "https://www.baidu.com", mode: "auto"});
}

天然支持Vir
script
{
    Vir test = PrintMylove({url: "https://www.baidu.com", mode: "auto"});
}
```

##### bindVirtualObject
绑定虚对象Vir
对于不使用CreateCHTLJSFunction创建的，但是符合CHTL JS函数的语法
可以使用bindVirtualObject手动绑定虚对象Vir，获得虚对象的支持

```cpp
Syntax::isCHTLJSFunction("PrintMylove {url: $!_, mode: $?_}");  // 输出-> true
CHTLJSFunction::bindVirtualObject("PrintMylove");  // 写函数名称
```

### 扫描算法
这是我目前已知的两种可用的扫描算法，如果有更好的算法，建议更换更好的扫描算法

#### 双指针扫描
CJMOD的代码总是以片段出现，双指针扫描一开始两个指针同时位于0位置
然后预先扫描一个片段是否存在关键字，没有就移动前指针到合适的位置
然后类似滑动窗口算法同步向前，如果前指针遇到了关键字，就通知后指针准备收集
确保语法片段能够成功返回CJMOD，使其正确填充参数

#### 动态感知
例如arg ** arg，识别之后动态识别前后

## CLI
CHTL提供了两个CLI工具，分别为常规命令行与命令行程序
常规命令行与我们平时使用的cmd，powershell无异，不具备渲染等特征，主打一个简洁

命令行程序则支持常规命令行很多不支持的功能，例如画面渲染，RGB，背景图，半透明等

CLI支持事件循环

### 默认结构
注意，默认情况下，CHTL编译器不应该提供默认的结构，而是根据用户写了什么内容，生成什么内容
这是因为CHTL编译器必须高度支持SPA页面，SPA页面通常不具有结构，尽管生成的结构会被Web处理器忽略，但提供纯净的内容更适合
你可以使用编译器指令--default-struct -> 来输出带有默认结构的HTML代码

### 内联
默认情况下，CHTL编译后将得到独立的HTML，CSS，JS文件
你可以使用--inline指令指示编译器生成内联的HTML文件，而非独立的文件
--inline指令共有三条

--inline
--inline-css
--inline-js

## VSCode IDE
CHTL项目推荐使用VSCode IDE
VSCode IDE需要满足下述基本要求

1. 代码高亮
2. 代码格式化(JS和CHTL JS使用JS风格代码格式化，其他使用C++风格代码格式化)
3. 代码提示
4. 页面预览
5. 右键浏览器打开
6. 右键打开文档
7. 右键导出HTML
8. 右键导出CSS
9. 右键导出JS
10. 实时预览
11. 内置编译器和官方模块
12. 自动模块解包和JSON查询表
13. 右键编译
14. 解决[]自动补全冲突
15. 模块语法提示

### 自动模块解包和JSON查询表
如果导入的是CMOD模块，则导出[Export]块的内容，并根据此优化性能，提供语法提示，并创建json表
如果导入的是CJMOD模块，则根据scan，CHTLJSFunction，analyze这三个函数接收的代码片段，提供语法提示，并创建json表

## 编译监视器
编译计时器能够监视编译器的编译时间和使用内存，必要时杀死程序，防止对开发者的造成可能的危害