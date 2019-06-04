# 执行java的设置
#source /etc/profile
# 进入Java文件所在路径， 得到.class .h 
clear
cd SvrJava
javac SvrCallImageEngine.java
javah -jni SvrCallImageEngine

# 返回到批处理文件所在的路径
cd ..

# 进入build文件夹，开始生成.so
cd build
cmake ..
make

# 返回到批处理文件所在的路径
cd ..

# 再次进入到Java文件所在的路径，执行java的调用
cd SvrJava
java -Djava.library.path=../build SvrCallImageEngine

# 返回到批处理文件所在的路径
cd ..

