#!/bin/sh
TARGET='MARSHMALLOW_VERSION_MAJOR'

SCRIPTS_DIR=`dirname $0`
GLOBAL_H="${SCRIPTS_DIR}/../../include/core/global.h"
CURRENT_REVISION=`sed -n "s/\(#define ${TARGET}\)\( \+\)\(.*\)/\3/p" ${GLOBAL_H}`
NEXT_REVISION=$((${CURRENT_REVISION} + 1))
NEXT_REVISION_STR=`printf "0x%02x" ${NEXT_REVISION}`
exec sed -i -e "s/\(#define ${TARGET}\)\( \+\)\(.*\)/\1\2${NEXT_REVISION_STR}/" ${GLOBAL_H}
