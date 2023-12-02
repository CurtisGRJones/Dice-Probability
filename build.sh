if [ ! -d "dest" ]
then
    mkdir dest;
fi


# TODO find better way to compile everything
g++ -o dest/dice-prob \
       main.cpp       