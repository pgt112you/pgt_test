# -x 是说指定以c++的语法来检查，不指定的话，会根据文件的后缀名，如果还是判断不出来的话，就默认使用c的语法
g++ -x c++ - -o /dev/null 2>/dev/null  <<EOF  
     #include <zlib.h>
     int main() {}
EOF
if [ "$?" = 0 ]; then    # 这里$?是0
    echo "hello"
fi

g++ -x c++ - -o /dev/null 2>/dev/null  <<EOF
     #include <zlib11.h>
     int main() {}
EOF
if [ "$?" = 0 ]; then     # 这里$?是1
    echo "world"
fi

