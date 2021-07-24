
cd v1 && make  && cd ..;
cd v2 && make  && cd ..;
 
row=100;  
col=100; 
color=20; 

./accessories/geramapa $row $col $color $seed > /tmp/flood_tmp;

printf '=%.0s' {1..100};
printf '\n';
echo "VERSÃO v1";
time ./v1/floodit < /tmp/flood_tmp > /tmp/flood_out;
cat /tmp/flood_out;

cat /tmp/flood_tmp /tmp/flood_out | ./accessories/verifica;
echo "VERIFICAÇÃO: $?";

printf '=%.0s' {1..100};
printf '\n';

echo "VERSÃO v2";
time ./v2/floodit < /tmp/flood_tmp > /tmp/flood_out;  
cat /tmp/flood_out;
cat /tmp/flood_tmp /tmp/flood_out | ./accessories/verifica;
echo "VERIFICAÇÃO: $?";  
    

printf '=%.0s' {1..100};
printf '\n';
echo "VERSÃO floodit_h1";
time ./accessories/floodit_h1 < /tmp/flood_tmp > /tmp/flood_out;  
cat /tmp/flood_out;  
cat /tmp/flood_tmp /tmp/flood_out | ./accessories/verifica;
echo "VERIFICAÇÃO: $?";

printf '=%.0s' {1..100};
printf '\n';
echo "VERSÃO floodit_h2";
time ./accessories/floodit_h2 < /tmp/flood_tmp > /tmp/flood_out; 
cat /tmp/flood_out; 
cat /tmp/flood_tmp /tmp/flood_out | ./accessories/verifica;
echo "VERIFICAÇÃO: $?";     

cd v1 && make purge  && cd ..;
cd v2 && make purge  && cd ..;