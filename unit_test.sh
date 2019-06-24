# 复制thirdparty相关文件到build文件夹
# cp thirdparty/1.dcm build/1.dcm
# cp thirdparty/2.dcm build/2.dcm
# cp thirdparty/3.dcm build/3.dcm
cp thirdparty/libcharls.so build/libcharls.so
cp thirdparty/libSvrCallImageEngine.so build/libSvrCallImageEngine.so

# 执行java的设置
#source /etc/profile
# 进入Java文件所在路径， 得到.class .h 
clear
cd SvrJava
#javac SvrCallImageEngine.java
#javah -jni SvrCallImageEngine
javac ./com/deepwise/cta/component/rebuild/SvrCallImageEngine.java
javah -jni com.deepwise.cta.component.rebuild.SvrCallImageEngine
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
# java -Djava.library.path=../build SvrCallImageEngine
java -Djava.library.path=../build com.deepwise.cta.component.rebuild.SvrCallImageEngine

# 返回到批处理文件所在的路径
cd ..

