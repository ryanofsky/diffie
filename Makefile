INCLUDES = /cygdrive/c/Download/ocean/ocean_shitty/boost_1_28_0_mpl
main.o : main.cpp append.hpp dhaes.hpp ownership.hpp Pocket.hpp policy.hpp SecBufferManager.hpp SimpleString.hpp SmartResource.hpp typelist.hpp workaround.hpp
	g++ -c -o main.o main.cpp -I$(INCLUDES)
main.exe : main.o
	g++ -o main.exe main.o
