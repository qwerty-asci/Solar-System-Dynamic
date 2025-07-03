set term gif animate

set output 'animacion.gif'

set xra[-3:3]
set yra[-3:3]

do for[a=0:10000:9] {plot 'movimiento_cuerpos/0.dat' i 0:a u 2:3 t 'Sol' pt 7 lt rgb '#DEFF00','movimiento_cuerpos/1.dat' i 0:a u 2:3 t 'M' pt 7 lt rgb '#FFA200','movimiento_cuerpos/2.dat' i 0:a u 2:3 t 'V' pt 7 lt rgb '#ABFF00','movimiento_cuerpos/3.dat' i 0:a u 2:3 t 'T' pt 7 lt rgb '#0000FF','movimiento_cuerpos/4.dat' i 0:a u 2:3 t 'Mt' pt 7 lt rgb '#FF5500'}


