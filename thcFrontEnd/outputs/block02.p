loadI  1024         => r0
loadI  6            => r2
store  r2           => r0
loadI  10           => r2
loadI  4            => r1
add    r0   , r1    => r1
store  r2           => r1
load   r0           => r3
load   r1           => r4
add    r3   , r4    => r5
loadI  1020         => r6
store  r5           => r6
output 1020              