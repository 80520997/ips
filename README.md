# ips
IP地址转城市地址

*这个程序是基于IPIP.NET网站的提供的[datx文件解析程序](https://github.com/17mon/c)(然而这玩意儿好像不能用)和ASIO编写的。目的是为了解决大并发情况下解析IP地址低效的问题。这个程序没有经过严格测试，对效率也不打包票，但是到烂一点儿的机器上每秒几万的处理能力应该是没什么问题的。程序使用二进制简单协议。可以参考script目录中的脚本思路


在bin目录下已经编译好了基于64位linux的二进制文件。大多数情况下拷过就去能直接用

命令行下执行

*./ips xxx.datx [端口号] [绑定本地的IP地址]

前面的xxx.datx是IP库文件的地址。后面两个是可选的。默认绑定0.0.0.0:8888


