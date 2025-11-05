# 关于CHTL
CHTL是基于C++ / Rust语言实现的超文本语言，其本质是为了提供一种更符合开发者编写HTML代码的方式，使用MIT开源协议

本语法规范属于CHTL，由于CHTL与CHTL JS存在一些交互信息
为此语法规范会包含CHTL JS的一部分规范
请你忽略这部分规范

## 注释
在CHTL中，使用//表示注释
使用/**/代表多行注释
使用# 代表会被生成器识别的注释
//和/**/注释不会被生成器所识别，生成的HTML不会带有这些注释
而# 注释则会根据上下文生成不同编程语言类型的注释
生成器注释必须是# + 空格 + 注释内容

```chtl
# 这是一个注释
#这不是一个注释
```

## 文本节点
在CHTL中，使用text { }表示一段文本

```chtl
text
{
    "这是一段文本"
}
```

除此之外，你也可以使用text属性来表示一段文本
text属性只能在元素内部使用，而text { }可以在元素外使用

```chtl
div
{
    text: "这是一段文本";
}
```

## 字面量
CHTL支持text与属性值使用无修饰字面量(没有引号的字符串)，这允许你像CSS一样无需书写引号
除此之外，CHTL支持双引号字符串("")，单引号字符串('')

```chtl
text
{
    这是一段文本
}

style
{
    color: red;
}
```

## CE对等式
CE对等式即Colon = Equal
即在CHTL之中，':'与'='完全等价
你可以在一些推荐的情景下使用'='

## 元素节点
CHTL支持HTML所有的元素，无论是单标签还是双标签，还是块级，行内，行内块元素

```chtl
html
{
    head
    {

    }

    body
    {
        div
        {
            text
            {

            }
        }

        span
        {

        }
    }
}
```

## 属性
在CHTL中，使用`属性名 : "属性值";`表示属性

```chtl
div
{
    id: box;
    class: welcome;

    text
    {
        HelloWorld
    }
}
```

## 局部样式块
CHTL对<style></style>进行了改进，允许开发者在元素的内部嵌套style {}，以此进行一些css操作
包括但不限于内联样式，类选择器，id选择器，伪类选择器，伪元素选择器
类选择器，id选择器，伪类选择器，伪元素选择器相关的代码将会被自动添加至全局样式块之中
注意！CHTL并没有对全局样式块进行增强，请不要尝试在全局样式块使用局部样式块的功能

### 内联样式
你可以在style{}内部直接添加属性，这些属性会成为元素的内联样式

```chtl
body
{
    div
    {
        // 内联样式
        style
        {
            width: 100px;
            height: 200px;
        }
    }
}
```

## 局部脚本块
CHTL允许在元素内部使用script{}来编写JS代码
局部script会被添加到一个不会全局污染，具有高优先级的全局script块之中
注意，CHTL不会对JS进行负责，因此只会直接输出JS

### 自动化类名 / id
你可以直接在局部样式块中使用类 / id选择器
无需手动编写class / id
CHTL会自动为元素添加类名 / id

```chtl
div
{
    style
    {
        .box  // 自动添加类名box给元素，且整部分代码将自动添加至全局样式块
        {
            width: 300px;
        }
    }
}
```

### 上下文推导
你可以在局部样式块之中使用&表示类名 / id(优先类名)
&将会根据上下文来推导成类名 / id，例如检查元素的class / id

这里有两种使用方式，一种是上文使用class / id
下文使用&，&:hover，&::before代替类 / id选择器
CHTL并不建议使用这一种方式

而是建议下述这一种使用方式
不写元素的class / id，直接使用.box / #box，让CHTL自动添加类名 / id
将&用于简化伪类选择器(&hover)与伪元素选择器(&::before)的使用

```chtl
div
{
    style
    {
        .box  // 让CHTL自动添加类名
        {

        }

        &:hover  // 使用&简化伪类的使用，之后&会被解析成类名，然后整体被添加至全局样式块
        {

        }
    }
}
```

### 属性运算
在原生CSS之中，你需要使用calc()函数来进行属性运算
在CHTL中，属性值可以直接使用算术运算符(+ - * / % **)直接进行算术运算
其中%为取模，**为幂运算符
全局style支持这一种用法(这可能需要一个CSS预处理器，接管这些工作[__CSS__] ... [__CSSEND__])
注意，由于CSS带有单位，因此需要遵循同单位合并，严禁不同单位之间进行运算

#### 运算规则详解
##### 单位合并
同单位之间进行运算，CHTL将合并单位
不同单位之间进行运算时，CHTL将报错

##### 左结合
无单位属性进行运算时，会寻找左侧是否存在属性值，若存在，则自动添加单位
若左侧无属性值时，将右结合

##### 字符串解耦
对于属性值`linear 0.5s all`，CHTL将会对此进行解耦，拿到数字字面量
以此确保能够进行属性运算，值得一提，这种情况仅限于无修饰字面量

##### 百分号
`1px + 5%`这样的运算是否成立？
答案是成立，CHTL将自动将百分比转换为数字字面量，然后进行运算

##### 特殊处理需求
`z-index: 1px + 5px`这种情况是否应该删除单位，然后得到不带单位的数字6?
答案是不应该，这种属于开发者行为，不属于语法问题

### 引用属性
在原生CSS之中，并不存在相对某一个属性的概念
为此CHTL引入了指向属性
现在你可以引用任何一个元素的属性值
全局style不支持这一种用法
这个功能需要收集数据，可能交给预处理器好一些

语法为CSS选择器.属性
处于性能的考量，只支持下述选择器
box  自动推断，tag -> id -> class
.box 类选择器
#box id选择器
button tag选择器
.box button 后代选择器
button[0] 精确访问

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
        width: 100px + box.width;  // 引用box的width属性，并进行运算
    }
}
```

### 属性条件表达式
元素自身的属性的表达式 ? 选项 : 选项
全局style支持这一种用法

div
{
    style
    {
        width: 100px;
        height: 100px;
        background-color: width > 50px ? "red" : "blue";
    }
}

#### 算术运算符
算术运算符(+ - * / % **)
属性表达式是支持算术运算符的
其中%为取模，**为幂运算符

#### 逻辑运算符
逻辑与(&&)与逻辑或(||)
属性表达式允许使用逻辑与(&&)与逻辑或(||)

```chtl
div
{
    style
    {
        width: 100px;
        height: 100px;
        background-color: width > 50px && width < 100px ? "red" : "blue";
    }
}
```

#### 引用属性条件表达式
属性条件表达式是支持参考不同元素的属性来进行条件表达的
语法为CSS选择器.属性
处于性能考虑，属性条件表达式只支持下述选择器
box  自动推断，tag -> id -> class
.box 类选择器
#box id选择器
button tag选择器
.box button 后代选择器
button[0] 精确访问

全局style不支持这一种用法

```chtl
div
{
    class: box;
    style
    {
        width: 100px;
    }
}

div
{
    style
    {
        width: .box.width > 50px ? .box.width / 2 : .box.height / 2;
    }
}
```

### 行为
```chtl
div
{
    style
    {

    }

    script
    {

    }

    text
    {

    }
}
```
上述是一个常见的CHTL元素
它的内部具有style，script，text三个块
这些块被称为行为
决定了这个元素具有什么样的特征
例如style块决定了元素具有什么样式
script块决定了元素具有什么行为
text块决定了元素具有什么文本内容

行为是CHTL元素的核心，它控制着CHTL元素的特征，如何显示，如何响应，如何交互
在默认情况下，行为使用声明块表示，例如style{}，script{}，text{}

如果你想要引入更多的行为，请尽可能使用声明式语法取代程序式语法
例如if

```cpp
if(条件表达式)
{
    // 条件成立时执行的代码
} else if(条件表达式)
{
    // 条件成立时执行的代码
} else
{
    // 条件不成立时执行的代码
}
```

```chtl
if
{
    condition: 与属性相关的条件表达式,
    // 条件成立时执行的代码，写属性
    width: 100px,
    display: block,
}
else if
{
    condition: 与属性相关的条件表达式,
    // CSS代码
}
else
{
    // CSS代码
}
```

当然，这只是一种建议，你也完全可以使用原本的程序式语法
或者两者同时提供


### 条件渲染
现在，你可以在元素内部定义if块，用于条件渲染
条件渲染属于静态特征

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
            condition: html.width < 500px,
            display: none,
        }
    }
}
```

## 模板
你可以使用[Template]来创建模板，在CHTL之中，存在样式组模板，元素模板，变量组模板
支持使用无修饰字面量

### 样式组模板
使用`[Template] @Style 组名`来创建样式组模板

```chtl
[Template] @Style DefaultText
{
	color: "black";
	line-height: 1.6;
}

div
{
    style
    {
        @Style DefaultText;  // 使用样式组模板，模板之中所有的属性都会被添加到div身上
    }
}
```

### 元素模板
使用`[Template] @Element 元素名`来创建元素模板

```chtl
[Template] @Element Box
{
    span
    {
        text
        {
            这是一组div
        }
    }

    div
    {
        style
        {
            width: 200px;
            height: 200px;
            background-color: red;
        }
    }

    div
    {
        style
        {
            width: 200px;
            height: 200px;
            background-color: blue;
        }
    }
}

body
{
    @Element Box;  // 使用元素模板
}
```

### 变量组模板
使用`[Template] @Var 变量组名`来创建变量组名

```chtl
[Template] @Var ThemeColor
{
    // 无需像CSS一样以--为前缀，因为变量组本质上不是CSS变量，而是值的替换
    tableColor: "rgb(255, 192, 203)";
}

[Template] @Element Box
{
    div
    {
        style
        {
            color: ThemeColor(tableColor);  // 使用模板变量组，无需以@Var为前缀，可以写，但不必要
        }
    }
}

body
{
    div
    {
        style
        {
            background-color: ThemeColor(tableColor);
        }
    }
}
```

### 组合继承
模板可以继承，无论什么类型，都能够继承同种类型的模板

```chtl
[Template] @Style ThemeColor
{
    color: rgba(255, 192, 203, 1);
    background-color: rgba(253, 144, 162, 1);
}

[Template] @Style ThemeColor2
{
    background-color: yellow;  // 重复的元素，根据书写的顺序进行值的替换
    @Style ThemeColor;  // 样式组模板继承，获得ThemeColor所有的属性
}
```

#### 显性继承
除了组合式继承，你还可以使用inherit显式继承

```chtl
[Template] @Style ThemeColor
{
    color: rgba(255, 192, 203, 1);
    background-color: rgba(253, 144, 162, 1);
}

[Template] @Style ThemeColor2
{
    background-color: yellow;
    inherit @Style ThemeColor;
}
```

## 自定义
自定义是模板的的扩展，自定义具有极高的灵活性以及更多的扩展操作
你可以使用[Custom]创建自定义内容
自定义与模板之间最大的差别就是自定义允许特例化操作
模板与自定义之间允许相互继承，只要类型匹配
支持使用无修饰字面量

### 自定义样式组
#### 无值样式组
自定义样式组允许样式组属性不具有值，开发者需要在使用时自行填入值

```chtl
[Custom] @Style TextSet
{
    color,
    font-size;
}

[Custom] @Style WhiteText
{
    @Style TextSet
    {
        color: white;
        font-size: 16px;
    }
}

div
{
    style
    {
        @Style TextSet
        {
            color: red;
            font-size: 16px;
        }
    }
}
```

#### 样式组的特例化
##### 删除属性
```chtl
[Template] @Style WhiteText
{
    color: white;
    font-size: 16px;
    line-height: 1.6;
    border: 1px black soild;
}

[Custom] @Style YellowText
{
    @Style WhiteText
    {
        delete line-height, border;
    }
    color: yellow;
}

div
{
    style
    {
        @Style YellowText
        {
            delete color;
        }
    }
}
```

##### 删除样式组继承
```chtl
[Template] @Style WhiteText
{
    color: white;
    font-size: 16px;
    line-height: 1.6;
    border: 1px black soild;
}

[Custom] @Style YellowText
{
    @Style WhiteText
    color: yellow;
}

div
{
    style
    {
        @Style YellowText
        {
            delete @Style WhiteText;
        }
    }
}
```

### 自定义元素
#### 自定义元素的特例化
##### 增加样式
```chtl
[Custom] @Element Box
{
    div
    {

    }

    div
    {

    }

    span
    {

    }
}

body
{
    @Element Box
    {
        span
        {
            // 为span添加样式
            style
            {

            }
        }

        div
        {

        }

        div
        {
            // 为第二个div添加样式
            style
            {

            }
        }
    }
}
```

##### 索引访问
你可以使用[index]索引来访问自定义元素中的某一个元素

```chtl
[Custom] @Element Box
{
    div
    {

    }

    div
    {

    }

    span
    {

    }
}

body
{
    @Element Box
    {
        div[1]  // 索引访问
        {
            // 为第二个div添加样式
            style
            {

            }
        }
    }
}
```

##### 插入元素
使用`insert 位置 选择器`插入元素

```chtl
[Custom] @Element Box
{
    div
    {

    }

    div
    {

    }

    span
    {

    }
}

body
{
    @Element Box
    {
        // 在第一个div后面插入内容
        insert after div[0] {  // 可选关键字，after，before，replace，at top / at bottom(可以不使用div[0])
            div
            {
                style
                {

                }
            }

            @Element Line;
        }
    }
}
```

##### 删除元素
```chtl
[Custom] @Element Box
{
    div
    {

    }

    div
    {

    }

    span
    {

    }

    div
    {

    }
}

body
{
    @Element Box
    {
        delete span;
        delete div[1];
    }
}
```

##### 删除元素继承
```chtl
[Custom] @Element Box
{
    div
    {

    }

    div
    {

    }

    span
    {

    }

    @Element Line;
}

body
{
    @Element Box
    {
        delete @Element Line;
    }
}
```

### 变量组
#### 变量组特例化
```chtl
[Custom] @Var ThemeColor
{
    tableColor: "rgb(255, 192, 203)";
    TextColor: "black";
}

div
{
    style
    {
        color: ThemeColor(tableColor = rgb(145, 155, 200));
    }
}
```

### 全缀名
CHTL允许使用全缀名来访问模板元素，样式组，变量组，自定义元素，样式组，变量组
这在处理命名冲突时非常有用

```chtl
[Custom] @Element Box
{
    div
    {

    }

    div
    {

    }
}

body
{
    @Element Box;  // 使用全缀名访问自定义元素Box

    [Custom] @Element Box;  // 使用全缀名访问自定义元素Box
}
```

## 原始嵌入
在CHTL中，你可以使用[Origin]表示这是一段原始的代码，这部分代码不会被CHTL处理，而是让生成器直接生成
原始嵌入是CHTL的兼容处理机制，避免CHTL考虑不到的极端问题
原始嵌入允许在任意节点中被解析
原始嵌入是直接把内容进行输出，绝对不会进行处理
原始嵌入的类型无作用，仅提供标识，不要根据类型将其包装在不同的标签中

### 嵌入HTML代码
```chtl
[Origin] @Html
{

}

body
{
    [Origin] @Html
    {

    }
}
```

### 嵌入CSS代码
```chtl
[Origin] @Style
{

}
```

### 嵌入JS代码
```chtl
[Origin] @JavaScript
{

}
```

### 带名原始嵌入
你可以为原始嵌入块赋予一个名称
```chtl
[Origin] @Html box
{

}

body
{
    [Origin] @Html box;
}
```

## 导入
你可以使用[Import]导入CHTL，HTML，CSS，JS文件
路径支持无修饰字面量

### 导入HTML，CSS，JS文件
导入HTML文件
[Import] @Html from html文件路径 as(必须) 命名为

导入CSS文件
[Import] @Style from css文件路径 as(必须) 命名为

导入JS / CJJS文件
[Import] @JavaScript from js文件路径 as(必须) 命名为  // 如果不具有as，直接跳过，具有as，则是创建命名原始嵌入节点

对于上述三种类型
如果写的是文件名（不带后缀）：在编译文件所在目录（非递归）按类型搜索相关文件
具体文件名（带后缀）：在编译文件所在目录（非递归）直接搜索该文件
如果路径为文件夹或不包含具体文件信息时，触发报错

### 导入CHTL文件
#### 精确导入

导入另一个chtl文件之中的自定义元素
[Import] [Custom] @Element 需要导入的自定义元素名 from chtl文件路径 as(可选) 命名为

导入另一个chtl文件之中的自定义样式组
[Import] [Custom] @Style 需要导入的样式组名称 from chtl文件路径 as(可选) 命名为

导入另一个chtl文件之中的自定义变量组
[Import] [Custom] @Var 需要导入的变量组名称 from chtl文件路径 as(可选) 命名为

导入另一个chtl文件之中的元素模板
[Import] [Template] @Element 需要导入的自定义元素名 from chtl文件路径 as(可选) 命名为

导入另一个chtl文件之中的样式组模板
[Import] [Template] @Style 需要导入的样式组名称 from chtl文件路径 as(可选) 命名为

导入另一个chtl文件之中的变量组模板
[Import] [Template] @Var 需要导入的变量组名称 from chtl文件路径 as(可选) 命名为

导入另一个chtl文件之中的命名原始嵌入
[Import] [Origin] @Html 名称 from chtl文件路径 as(可选) 命名为
[Import] [Origin] @Style 名称 from chtl文件路径 as(可选) 命名为
[Import] [Origin] @JavaScript 名称 from chtl文件路径 as(可选) 命名为

#### 类型导入
导入另一个chtl文件之中所有的自定义元素
[Import] [Custom] @Element from chtl文件路径 as(无效) 命名为

导入另一个chtl文件之中所有的自定义样式组
[Import] [Custom] @Style from chtl文件路径 as(无效) 命名为

导入另一个chtl文件之中所有的自定义变量组
[Import] [Custom] @Var from chtl文件路径 as(无效) 命名为

导入另一个chtl文件之中所有的元素模板
[Import] [Template] @Element from chtl文件路径 as(无效) 命名为

导入另一个chtl文件之中所有的样式组模板
[Import] [Template] @Style from chtl文件路径 as(无效) 命名为

导入另一个chtl文件之中所有的变量组模板
[Import] [Template] @Var from chtl文件路径 as(无效) 命名为

#### 通配导入
导入所有模板
[Import] [Template] from chtl文件路径 as(无效) 命名为

导入所有自定义
[Import] [Custom] from chtl文件路径 as(无效) 命名为

导入所有命名原始嵌入(注意！命名)
[Import] [Origin] from chtl文件路径 as(无效) 命名为

导入一个chtl文件
[Import] @Chtl from chtl文件路径

#### . /对等式
在CHTL，路径具有两种表达方式，可以使用'.'来表示'/'

## 命名空间
你可以使用[Namespace]创建命名空间，命名空间能够有效防止模块污染
导入一整个文件，或导入了重名的任意单元时，命名空间起效
命名空间允许不使用{}

test.chtl
```chtl
[Namespace] space  // 创建space命名空间

[Custom] @Element Box
{
    div
    {
        style
        {

        }
    }

    div
    {
        style
        {

        }
    }
}
```

```chtl
[Import] @Chtl from test.chtl的路径  // 导入一个chtl文件时，如果对方没有定义命名空间，则默认使用文件名作为命名空间

body
{
    @Element Box from space;  // 使用space命名空间中的Box自定义元素
}
```

通常情况下，from是非必要使用的，这是因为CHTL使用的是一种伪合并的机制，让多个命名空间共享上下文
只有在必要的使用，才需要使用from

### 命名空间嵌套
```chtl
[Namespace] space
{
    [Namespace] room  // 嵌套命名空间

    [Custom] @Element Box
    {
        div
        {
            style
            {

            }
        }

        div
        {
            style
            {

            }
        }
    }
}
```

```chtl
[Namespace] space
{
    [Namespace] room


    [Namespace] room2

        [Custom] @Element Box
        {
            div
            {
                style
                {

                }
            }

            div
            {
                style
                {

                }
            }
        }
}
```

```chtl
[Import] @Chtl from test.chtl的路径

body
{
    @Element Box from space.room2;  // 嵌套命名空间的使用
}
```

同名的命名空间自动合并，并且具有冲突检测策略
对于没有使用命名空间的文件，在被导入时，会默认以文件名作为命名空间

## 约束
你可以使用except关键字来进行定义域约束

### 精确约束
精确约束能够作用的对象有HTML元素，自定义与模板对象

```chtl
div
{
    except span, [Custom] @Element Box;  // 禁止使用span与Box
}
```

### 类型约束
精确约束能够作用的对象有@Html，[Custom]，以及模板[Template]

```chtl
div
{
    except @Html;  // 禁止html元素
    except [Template] @Var; // 禁止在div内部使用变量组模板的对象
    except [Custom];  // 禁止出现[Custom]类型的对象
}
```

### 全局约束
在命名空间内使用全局约束，全局约束只支持前面列出来的类型

```chtl
[Namespace] space
{
    except [Template];

    [Template] // 错误，不允许定义模板
}
```

## 配置组
配置组允许开发者自定义很多行为
配置组支持使用无修饰字面量

```chtl
[Configuration]
{
    // 索引的起始计数
    INDEX_INITIAL_COUNT = 0;

    // DEBUG模式
    DEBUG_MODE = false;
}
```

### Name
你可以通过修改配置组中的[Name]块来修改关键字名称
支持ASCII码范围内除了空格，以及可能产生歧义的特殊字符，如'/'等外的所有字符
同样支持中文

注意，配置组修改关键字名称是逆向替换，将自定义的关键字替换回标准规范
[Configuration]不可定义，其他都允许被替换

```chtl
[Configuration]
{
    INDEX_INITIAL_COUNT = 0;

    // 是否禁用Name配置组(即是否允许自定义关键字名称)
    DISABLE_NAME_GROUP = true;

    DEBUG_MODE = false;

    // Name配置块，存放关键字名称用
    [Name]
    {
        // 这里可以定义关键字的名称，无需改动源码即可实现支持
        // 组选项，即CUSTOM_STYLE具有多个值
        CUSTOM_STYLE = [@Style, @style, @CSS, @Css, @css];
        CUSTOM_ELEMENT = @Element;
        CUSTOM_VAR = @Var;
        TEMPLATE_STYLE = @Style;
        TEMPLATE_ELEMENT = @Element;
        TEMPLATE_VAR = @Var;
        ORIGIN_HTML = @Html;
        ORIGIN_STYLE = @Style;
        ORIGIN_JAVASCRIPT = @JavaScript;
        IMPORT_HTML = @Html;
        IMPORT_STYLE = @Style;
        IMPORT_JAVASCRIPT = @JavaScript;
        IMPORT_CHTL = @Chtl;
        IMPORT_CRMOD = @CJmod;
        KEYWORD_INHERIT = inherit;
        KEYWORD_DELETE = delete;
        KEYWORD_INSERT = insert;
        KEYWORD_AFTER = after;
        KEYWORD_BEFORE = before;
        KEYWORD_REPLACE = replace;
        KEYWORD_ATTOP = at top;
        KEYWORD_ATBOTTOM = at bottom;
        KEYWORD_FROM = from;
        KEYWORD_AS = as;
        KEYWORD_EXCEPT = except;
        KEYWORD_USE = use;
        KEYWORD_HTML5 = html5;
        KEYWORD_TEXT = text;
        KEYWORD_STYLE = style;  // 局部样式块
        KEYWORD_SCRIPT = script;  // 局部脚本块
        KEYWORD_CUSTOM = [Custom];
        KEYWORD_TEMPLATE = [Template];
        KEYWORD_ORIGIN = [Origin];
        KEYWORD_IMPORT = [Import]
        KEYWORD_NAMESPACE = [Namespace]

        // 组选项的数量限制，避免在大型项目中对性能的过高消耗
        OPTION_COUNT = 3;
    }
}
```

### 命名配置组
配置组可以命名，命名配置组不会被使用，不被命名的配置组才会启用，如果存在多个无名的配置组则冲突，命名配置组可以创建多个，命名配置组通常服务于导入[Import]

```chtl
[Configuration] @Config Basic
{
    [Name]
    {
        CONFIGURATION_CONFIG = @Config;  // 新增
        IMPORT_CONFIG = @Config;  // 新增
    }
}

[Configuration] @Config Std
{

}
```

### 导入配置组
[Import] @Config 导入的配置组名称(可选，不写默认导入无名配置组，如果没有则是第一个有名) from chtl文件路径 as(如果使用as，需要与use搭配使用) 命名为

全缀名写法
[Import] [Configuration] @Config 导入的配置组名称(可选，不写默认导入无名配置组，如果没有则是第一个有名) from chtl文件路径 as(如果使用as，需要与use搭配使用) 命名为

导入另一个Chtl文件之中所有的命名配置组
[Import] [Configuration] from chtl文件路径 as(无效) 命名为

### 局部样式块自动化规则
你可以在[Configuration]之中添加下述规则禁用局部样式块class / id的自动化添加

```chtl
[Configuration]
{
    // 禁止局部样式块自动添加类选择器
    DISABLE_STYLE_AUTO_ADD_CLASS = false;
    // 禁止局部样式块自动添加id选择器
    DISABLE_STYLE_AUTO_ADD_ID = false;
}
```

当局部style内部存在多组类选择器时，若class属性缺失，则自动添加第一个类选择器
当局部style内部存在多组id选择器时，若id属性缺失，则自动添加第一个id选择器
对于局部style来说，& 引用选择器优先选择class

### 默认命名空间
你可以在[Configuration]之中添加下述规则禁用默认命名空间功能，这意味着导入的文件与当前文件所使用的命名空间一致，这可能会造成污染

```chtl
[Configuration]
{
    DISABLE_DEFAULT_NAMESPACE = false;
}
```

### 自定义原始嵌入类型
@Html，@Style，@JavaScript是CHTL中基本的类型，CHTL只为原始嵌入提供了这三种类型
如果你需要更多的类型，你可以直接创建你想要的类型的原始嵌入，注意！必须以@为前缀

```chtl
[Origin] @Vue box
{

}

body
{
    [Origin] @Vue box;
}
```

自定义类型系统会隐式创建一个配置块

```chtl
[Configuration]
{
    DISABLE_CUSTOM_ORIGIN_TYPE = false;

    [OriginType]  // 定义Origin具有什么自定义类型
    {
        ORIGINTYPE_VUE = @Vue;  // 如果我创建了一个@Vue类型的原始嵌入，那么CHTL会隐式创建

        // 你也可以显式创建，更明确说明，但是要注意
        // ORIGINTYPE_VUE = @Vue;
        // ORIGINTYPE_MARKDOWN = @Markdown;
        // CHTL强制要求使用ORIGINTYPE_全写的类型名称 = @全大写后 = 全写的类型名称的内容
    }
}
```

#### 导入自定义原始嵌入类型
// 导入另一个chtl文件之中某一个命名自定义原始嵌入
[Import] [Origin] @Vue 原始嵌入名称 from xx.chtl as(可选) 命名

// 导入另一个chtl文件之中所有的命名自定义原始嵌入
[Import] [Origin] @Vue from xx.chtl as(无效) 命名

## use
use语法能够明确当前文件使用什么配置组
use语法必须在文件开头，且只能有一个用于配置组

### HTML5类型
```chtl
use html5;
```

生成HTML5声明

### 使用命名配置组
```chtl
use @Config Basic;  // 此文件使用Basic配置组

use [Configuration] @Config Basic;  // 也可以使用全缀名
```

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

### CMOD
CMOD是CHTL提供的一种模块化方式
CHTL编译器 / 打包脚本能够将符合CMOD格式的文件夹打包成.cmod文件
你需要获取CHTL编译器的源码，CHTL源码中包含了打包脚本，如果你会编译CHTL编译器，那么更推荐你使用CHTL编译器来打包CMOD

#### 模块结构
CMOD具有严格的模块结构

```chtl
Chtholly
    src
        Chtholly.chtl
        Other.chtl
    info
        Chtholly.chtl
```

src是模块的源码，而info则是存放模块信息的文件夹
CHTL要求，模块文件夹，主模块chtl文件，模块信息chtl文件必须同名

在没有子模块的情况下，主模块chtl文件必须存在

如果src中存在子模块，则src文件夹内部的主模块chtl文件可以省略(推荐)，也可以保留，保留的意义并不大
最好是让主模块作为一个框架

#### 模块信息
info文件夹中的chtl文件主要提供两个功能
提供模块的信息与提供外部查询表

如下所示
```chtl
// 只需要写这个即可
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

// 外部查询表，优化性能用，同时决定向外暴露什么内容
// 不需要手动写这个，系统自动生成，也可以手动书写，决定向外暴露什么内容
[Export]
{
    [Custom] @Style ChthollyStyle, ChthollyCard, ChthollyButton,
           ChthollyHeader, ChthollyFooter;

    [Custom] @Element ChthollyPage, ChthollySection, ChthollyNav,
             ChthollyModal, ChthollyGallery;

    [Custom] @Var ChthollyColors, ChthollyFonts, ChthollySpacing,
         ChthollyBreakpoints, ChthollyAnimations;

    [Template] @Style ...;
    [Template] @Element ...;
    [Template] @Var ...;
    [Origin] @Html ...;
    [Origin] @Style ...;
    [Origin] @Javascript ...;
    [Origin] @Vue ...;  // 一样支持导出自定义类型的原始嵌入
    [Configuration] @Config ...;
}
```

#### 包含子模块的模块结构
```chtl
Chtholly
    src
        (Chtholly.chtl)  // 存在子模块，可选
        Other
            src
                Other.chtl
            info
                Other.chtl
        Space
            src
                Space.chtl
            info
                Space.chtl
    info
        Chtholly.chtl
```
拥有子模块的模块文件夹内部主模块chtl文件可以省略，也可以保留，保留的意义不大
但是必须注意的是，结构始终是src + info，这是不可忽略的
主模块chtl文件即使省略了也必须拥有info
相应的子模块均使用src + info的结构
这是强制的要求

#### 模块的导入
导入一个chtl文件  /  cmod模块
[Import] @Chtl from chtl / cmod文件路径

##### 路径搜索
对于@Chtl类型来说
名称（不带后缀）：优先搜索官方模块目录(源码编译后生成的module文件夹，通常和编译器同一个文件夹，含cmod，chtl和cjmod文件，module文件夹可能分为两种情况，一种是乱序结构，cmod，chtl，cjmod的文件混杂在一起，一种是有序结构，使用cmod / Cmod / CMOD + cjmod / CJmod / CJMOD两个文件夹进行分类），其次搜索编译文件所在的目录module(module文件夹可能分为两种情况，一种是乱序结构，cmod，chtl，cjmod的文件混杂在一起，一种是有序结构，使用cmod / Cmod / CMOD + cjmod / CJmod / CJMOD两个文件夹进行分类)文件夹，最后搜索编译文件所在目录，优先匹配cmod文件，其次chtl，不匹配cjmod文件)
具体名称（带后缀）：按官方模块目录→当前目录module文件夹→当前目录顺序搜索指定文件
具体路径（含文件信息）：直接按路径查找，未找到则报错
具体路径（不含文件信息）：触发报错
对于使用官方模块前缀，直接在官方模块目录中搜索

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