#!/bin/bash

cd ./bin

nohup ./gate_server &

nohup ./login_server &

nohup ./rank_server &