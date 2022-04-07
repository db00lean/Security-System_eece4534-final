CC:=arm-esl-linux-gnueabi-g++ -mthumb -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a9 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=/home/bpalmer/eece4534sdk/sysroots/cortexa9t2hf-neon-esl-linux-gnueabi
libs:= -lpthread

test: system_management_client.cpp
	${CC} system_management_client.cpp -o main ${libs}

