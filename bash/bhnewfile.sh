filepath=$(dirname "$(readlink -f "$0")")
python3 ${filepath}/newfile.py $1 $2 .