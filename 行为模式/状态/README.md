### 优点
1、单一职责原则。 将与特定状态相关的代码放在单独的类中 </br>
2、开闭原则。 无需修改已有状态类和上下文就能引入新状态 </br>
3、通过消除臃肿的状态机条件语句简化上下文代码
### 缺点
如果状态机只有很少的几个状态， 或者很少发生改变， 那么应用该模式可能会显得小题大作
