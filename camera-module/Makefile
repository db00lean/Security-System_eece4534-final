CC:=arm-esl-linux-gnueabi-g++ -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=/home/bpalmer/eece4534sdk/sysroots/cortexa9t2hf-neon-esl-linux-gnueabi

test: test.cpp
	${CC} test.cpp -o test

