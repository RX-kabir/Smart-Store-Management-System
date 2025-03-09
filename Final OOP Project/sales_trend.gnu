set terminal png size 800,600
set output 'sales_trend.png'
set title 'Sales Trend'
set xlabel 'Product'
set ylabel 'Total Sales ($)'
plot 'sales_data.txt' using 2:xtic(1) with linespoints title 'Sales'
