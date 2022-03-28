# project_asio_server

基于boost asio的服务端框架

1、同时支持socket，websocket（beast）连接，即h5与app数据互通，支持ssl
2、网络处理与业务逻辑分离，io线程与工作线程分离
3、redis基于hiredis的线程池非异步操作
4、协议采用protocol格式
5、日志采用glog


待完善
1、支持rpc


待评估
    曾使用libgo以及boost co_spawn协程实现网络及redis模块
    libgo非常消耗内存
    co_spawn相关配套设施不完善，如协程间同步
    接入第三方库时，支持协程操作比较麻烦