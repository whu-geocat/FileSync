# FileSync
C++ with TCP/IP for sychronization of files between 2 computers.
Project is built by Qt, which is used for building the GUI.

# FileSync
用C++写的基于 TCP/IP 的在两台电脑间进行文件同步的工具。
项目是qt的项目，基于qt写的用户界面。

## 当前不足
  1. 对于文档（.doc/.docx等），服务器在接收到数据之后写入到文件会失败。
  2. 客户端进度条的进度值设置有点问题，原因是无法在子线程中给进度条设置值。
  3. 没有使用心跳，连接断开时C/S都没有断开连接的提示
