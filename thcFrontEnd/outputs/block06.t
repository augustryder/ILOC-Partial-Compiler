|index| opcode ||  op1 |  op2 | dest |
|     |        ||  sr  |  sr  |  sr  |  
|0    | loadI  || 1024 | -    | r8   |
|1    | loadI  || 1028 | -    | r9   |
|2    | load   || r8   | -    | r8   |
|3    | load   || r9   | -    | r9   |
|4    | loadI  || 1032 | -    | r10  |
|5    | loadI  || 1036 | -    | r11  |
|6    | loadI  || 1040 | -    | r12  |
|7    | loadI  || 1044 | -    | r13  |
|8    | store  || r8   | r10  | -    |
|9    | add    || r8   | r9   | r8   |
|10   | store  || r8   | r11  | -    |
|11   | add    || r8   | r9   | r8   |
|12   | store  || r8   | r12  | -    |
|13   | store  || r9   | r13  | -    |
|14   | load   || r10  | -    | r1   |
|15   | load   || r11  | -    | r2   |
|16   | load   || r12  | -    | r3   |
|17   | lshift || r1   | r9   | r1   |
|18   | mult   || r1   | r2   | r1   |
|19   | load   || r13  | -    | r2   |
|20   | mult   || r1   | r3   | r1   |
|21   | mult   || r1   | r2   | r1   |
|22   | store  || r1   | r10  | -    |
|23   | output || 1032 | -    | -    |
