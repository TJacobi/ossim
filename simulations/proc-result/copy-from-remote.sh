#!/bin/bash

#
# copy all the configuration files and result files to a separate folder 
# -- for backup and post-processing
#

#
# Syntax:
#
# copy-from-remote.sh <remote_simulations_folder> <local_storage_folder> <configuration>
#

#
# Example:
#
#$1=118:/home/giang/sim1/omnetpp/so/simulations
#$2=/nix/workspace/omnet422-inet118/store/sim-results/wMathias/delays
#$3=delays-500n-srate500kbps-upbw2500kbps-5partners
#

# ------------------------------------------------------------------------------

REMOTE=$1
LOCAL=$2

STORE_DIR=$LOCAL/$3

mkdir $STORE_DIR

scp $REMOTE/results/*.sca $STORE_DIR
scp $REMOTE/Donet.ini $STORE_DIR
scp $REMOTE/Donet.ned $STORE_DIR
scp $REMOTE/include/churn.ini $STORE_DIR
scp $REMOTE/include/seed.ini $STORE_DIR


