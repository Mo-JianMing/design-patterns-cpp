### 优点
如果程序中有很多相似对象， 那么你将可以节省大量内存
### 缺点
1、你可能需要牺牲执行速度来换取内存， 因为他人每次调用享元方法时都需要重新计算部分情景数据 </br>
2、代码会变得更加复杂。 团队中的新成员总是会问： “为什么要像这样拆分一个实体的状态？”
