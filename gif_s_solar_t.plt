set term gif animate

set output 'animacion_t.gif'

set xra[-50 to 50]
set yra[-50 to 50]

do for[a=0:100000:200] {plot 'movimiento_cuerpos/0.dat' i a u 2:3 t 'Sol' pt 7 lt rgb '#DEFF00','movimiento_cuerpos/5.dat' i a u 2:3 t 'J' pt 7,'movimiento_cuerpos/6.dat' i a u 2:3 t 'S' pt 7,'movimiento_cuerpos/7.dat' i a u 2:3 t 'U' pt 7 ,'movimiento_cuerpos/8.dat' i a u 2:3 t 'N' pt 7,'movimiento_cuerpos/9.dat' i a u 2:3 t 'P' pt 7}


