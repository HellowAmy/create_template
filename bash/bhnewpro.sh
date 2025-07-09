filepath=$(dirname "$(readlink -f "$0")")
python3 ${filepath}/newpro.py $1 $2 .
