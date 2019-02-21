#!/bin/bash

TEST_FILE_PATH="testfile/"
ROOT_PATH="$(cd `dirname $0`; pwd)/"
INPUT_FILE_PATH="${ROOT_PATH}/input.txt"
OUTPUT_FILE_PATH="${ROOT_PATH}/output.txt"
RIGHT_FILE_PATH="${ROOT_PATH}/right.txt"

let success=0
let index=0
let diff_result=0
let PASS_NUM=0
let FAIL_NUM=0


for file in $(ls ${ROOT_PATH}/${TEST_FILE_PATH})
do
    echo "${file}"
    if [[ $file =~ \.txt$ ]]
    then
        echo "把${file}内容输出到input.txt和right.txt中..."
        sed -n '1,2p' ${ROOT_PATH}/${TEST_FILE_PATH}${file} > ${INPUT_FILE_PATH}
        sed -n '3,3p' ${ROOT_PATH}/${TEST_FILE_PATH}${file} > ${RIGHT_FILE_PATH}
        echo "计算结果并输出到output.txt中..."
        ./multisum
        echo "计算完成..."
        echo "开始比对output.txt和right.txt..."
        diff "${ROOT_PATH}output.txt" "${ROOT_PATH}right.txt"
        let diff_result=$?
        if [[ ${diff_result} -eq ${success} ]];then
            let PASS_NUM+=1
        else
            let FAIL_NUM+=1
            echo "${file}测试失败"
            FAIL[${index}]="${file}"
            let index+=1
        fi
    else
        continue
    fi
done

if [ ${FAIL_NUM} -eq ${success} ]
then
   echo "${PASS_NUM}个测试通过，通过率100%"
else
   echo "失败样例：${FAIL[@]}"
fi
