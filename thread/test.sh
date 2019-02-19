#!/bin/bash
./multisum
INOUT_FILE_PATH="testfile/"
OUTPUT_FILE_PATH="outfile/"
RIGHT_FILE_PATH="rightfile/"
ROOT_PATH="$(cd `dirname $0`; pwd)/"


let success=0
let index=0
let diff_result=0
let PASS_NUM=0
let FAIL_NUM=0


for file in $(ls ${ROOT_PATH}/${OUTPUT_FILE_PATH})
do
    echo "${file}"
    if [[ $file =~ \.txt$ ]]
    then
        diff ${ROOT_PATH}${OUTPUT_FILE_PATH}${file} ${ROOT_PATH}${RIGHT_FILE_PATH}${file}
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