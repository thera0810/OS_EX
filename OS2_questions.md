# 操作系统实验二问题记录

## dml-0405-说明

1. 原始的synch.cc和synch.h已经备份至**synch.cc.bck和synch.h.bck**，现在的synch.cc和synch.h可直接用于实验。

2. 2.1.2代码保存为**synch-sleep.cc和synch-sleep.h**，测试时可以直接拷贝内容到synch.cc和synch.h。

3. 2.1.3代码保存为**synch-sem.cc和synch-sem.h**，测试时可以直接拷贝内容到synch.cc和synch.h。

4. 目前只测试了case20 21 22

   |            | -q 20 | -q 21 | -q 22（同步）   |
   | ---------- | ----- | ----- | --------------- |
   | lock-sleep | ok    | ok    | —               |
   | cond-sleep | —     | —     | ok（broadcast） |
   | lock-sem   | ok    | ok    | —               |
   | cond-sem   | —     | —     | ok（broadcast） |

## dml-问题记录

1. 锁是保证互斥的，条件变量是保证同步的。

2. condition signal()不能保证阻塞队列里所有进程都会有人唤醒

3. 用semaphore实现condition，broadcast后，会唤醒所有阻塞队列，但是由于semaphore::P()的实现是while(...)，即被唤醒后会再次检查条件，所以broadcast后只有一个进程真的被唤醒，其他进程继续阻塞。并且没有进程再唤醒。

   **解决方案**：wakeup的进程，get后检查是否链表非空，如果非空的话，就broadcast以唤醒其他阻塞进程。

4. 用友元类（friend）来访问semaphore的私有成员







 