|index| opcode ||  op1 |  op2 | dest |
|     |        ||  sr  |  sr  |  sr  |  
|0    | loadI  || 1024 | -    | r0   |
|1    | loadI  || 6    | -    | r2   |
|2    | store  || r2   | r0   | -    |
|3    | loadI  || 10   | -    | r2   |
|4    | loadI  || 4    | -    | r1   |
|5    | add    || r0   | r1   | r1   |
|6    | store  || r2   | r1   | -    |
|7    | load   || r0   | -    | r3   |
|8    | load   || r1   | -    | r4   |
|9    | add    || r3   | r4   | r5   |
|10   | loadI  || 1020 | -    | r6   |
|11   | store  || r5   | r6   | -    |
|12   | output || 1020 | -    | -    |
