 # real_sum; expected_sum; consumers_count
 function validate {
    TEMPLATE="(real_sum = $1; expected_sum = $2; consumers = $3)"
    if [[ $1 != $2 ]]; then
        echo "error: $TEMPLATE"
    else
        echo "ok: $TEMPLATE"
    fi
 }
 
 SHORT_DATA='1'
 LONG_DATA=''
 for (( I=0; I < 4200; ++I)); do  
    LONG_DATA="$LONG_DATA 1"
 done
 for (( I=1; I < 128; ++I)); do  
    validate $(echo $SHORT_DATA | ../posix $I 0) 1 $I
    validate $(echo $LONG_DATA | ../posix $I 0) 4200 $I
done