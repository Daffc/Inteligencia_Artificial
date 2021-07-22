for i in [1..10]; do  
    row=100;  
    col=100; 
    color=20; 
    ./accessories/geramapa $row $col $color $seed > /tmp/flood_tmp;

    echo "v1";
    time ./v1/floodit < /tmp/flood_tmp;         
    echo "floodit_h1";
    time ./accessories/floodit_h1 < /tmp/flood_tmp;     
    echo "floodit_h2";
    time ./accessories/floodit_h2 < /tmp/flood_tmp;       
done