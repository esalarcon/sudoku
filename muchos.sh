#!/bin/bash
echo "#!/bin/bash" > $2
while IFS='' read -r line || [[ -n "$line" ]]; do
    echo "./sudoku $line | tail -n 1" >> $2
done < "$1"
chmod +x $2


