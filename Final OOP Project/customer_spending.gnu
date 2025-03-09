set terminal png size 800,600
set output 'customer_spending.png'
set title 'Customer Spending'
set xlabel 'Customer'
set ylabel 'Total Spending ($)'
plot 'customer_spending.txt' using 2:xtic(1) with boxes title 'Spending'
