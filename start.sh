#!/bin/bash

cd ./bin

nohup ./gate_server >/dev/null 2>/dev/null &

nohup ./login_server >/dev/null 2>/dev/null &

nohup ./rank_server >/dev/null 2>/dev/null &
