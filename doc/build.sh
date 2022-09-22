#!/usr/bin/bash

# 使用方法を表示する
function show_usage(){
    cat << EOS 
usage:
    ${0} 

dependency apps:
    graphviz: https://www.graphviz.org/download/
    asciidoctor: https://asciidoctor.org/
EOS
}

# graphvizがインストールされているか
function hasGraphviz() {
    path=`which dot 2>/dev/null | cut -c1`
    # echo "path=${path}"
    if [[ ${path} == "/" ]] ; then
        echo true
    else
        echo false
    fi
}

# 画像出力先
function makeImageDir() {
    CURRENT=$(cd $(dirname $0);pwd)
    image_dir="${CURRENT}/images"

    if [ -d ${image_dir} ] ; then
        echo "${image_dir}"
    else
        mkdir -p ${image_dir}
        echo "${image_dir}"
    fi
}

# graphviz用ファイル格納場所
function dotDir() {
    CURRENT=$(cd $(dirname $0);pwd)
    echo "${CURRENT}/dots"
}

# graphvizの画像出力
function dot_output() {
    if [ $# != 2 ] ; then
        echo "target directory is null.";
        return
    fi

    while read -r f; do
        svg=`echo $f | sed -e "s/\.dot$/.svg/g" | sed -e "s/^.*\///g"`
        command="dot -Kdot -Tsvg ${f} -o${2}/${svg}"
        echo ${command}
        dot -Kdot -Tsvg ${f} -o"${2}/${svg}"
    done < <(find ${1}/ -mindepth 1 -maxdepth 1 -name "*.dot")
}

# asciidoctorがインストールされているか
function hasAsciidoctor() {
    path=`which asciidoctor 2>/dev/null | cut -c1`
    # echo "path=${path}"

    if [[ ${path} == "/" ]] ; then
        echo true
    else
        echo false
    fi
}

# asciidocの出力
function adoc_output() {
    asciidoctor design.adoc
}

graphviz=`hasGraphviz`
# echo "return:${graphviz}"
if [ "${graphviz}" == false ]  ; then
    echo "no graphviz"
    show_usage
    exit 0
fi

asciidoctor=`hasAsciidoctor`
if [ "${asciidoctor}" == false ] ; then
    echo "no asciidoctor"
    show_usage
    exit 0
fi
out_dir=`makeImageDir`

dot_output `dotDir` ${out_dir}

adoc_output 