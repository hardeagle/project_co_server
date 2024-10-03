#!/bin/bash

# 后续捕获signal关闭
ps -ef | grep gate_server | grep -v grep | awk {'print $2'} | xargs kill

ps -ef | grep login_server | grep -v grep | awk {'print $2'} | xargs kill

ps -ef | grep rank_server | grep -v grep | awk {'print $2'} | xargs kill
