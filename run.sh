if [ $# -eq 2 ]
then args="$1 $2"
else args=$1
fi
echo $args

make && time src/meshpro $args out.off && src/meshview out.off
	
