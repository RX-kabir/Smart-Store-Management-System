set terminal png size 1500,600
set output 'inventory_levels.png'
set title 'Inventory Levels'
set xlabel 'Product'
set ylabel 'Quantity'
set xtics rotate by -45
set xtics font ",8"
plot 'inventory_data.txt' using 2:xtic(1) with boxes title 'Inventory'
