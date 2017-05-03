#!/bin/bash
export OMP_NUM_THREAD=$1
~/NPB3.3.1/NPB3.3-OMP/bin/ft.B.x >> ~/AMon/logs/ft_app.log&
echo $!

