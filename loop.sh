if [ $# -eq 1 ] 
then mesh=$1 
else mesh=input/ico.off
fi

make && src/meshpro $mesh out.off -loop && src/meshview out.off
