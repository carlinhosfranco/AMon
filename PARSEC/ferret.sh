#!/bin/bash
parsecmgmt -a run -p ferret -i native -n $1 >> ~/AMon/logs/ferret_app.log&
echo $!

