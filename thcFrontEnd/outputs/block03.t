|index| opcode ||  op1 |  op2 | dest |
|     |        ||  sr  |  sr  |  sr  |  
|0    | loadI  || 1032 | -    | r1   |
|1    | loadI  || 1024 | -    | r10  |
|2    | load   || r10  | -    | r10  |
|3    | loadI  || 4    | -    | r11  |
|4    | loadI  || 1028 | -    | r12  |
|5    | load   || r12  | -    | r12  |
|6    | store  || r10  | r1   | -    |
|7    | add    || r1   | r11  | r1   |
|8    | add    || r10  | r12  | r10  |
|9    | store  || r10  | r1   | -    |
|10   | add    || r1   | r11  | r1   |
|11   | add    || r10  | r12  | r10  |
|12   | store  || r10  | r1   | -    |
|13   | add    || r1   | r11  | r1   |
|14   | add    || r10  | r12  | r10  |
|15   | store  || r10  | r1   | -    |
|16   | add    || r1   | r11  | r1   |
|17   | add    || r10  | r12  | r10  |
|18   | store  || r10  | r1   | -    |
|19   | add    || r1   | r11  | r1   |
|20   | add    || r10  | r12  | r10  |
|21   | store  || r10  | r1   | -    |
|22   | add    || r1   | r11  | r1   |
|23   | add    || r10  | r12  | r10  |
|24   | store  || r10  | r1   | -    |
|25   | add    || r1   | r11  | r1   |
|26   | add    || r10  | r12  | r10  |
|27   | store  || r10  | r1   | -    |
|28   | add    || r1   | r11  | r1   |
|29   | add    || r10  | r12  | r10  |
|30   | store  || r10  | r1   | -    |
|31   | add    || r1   | r11  | r1   |
|32   | add    || r10  | r12  | r10  |
|33   | store  || r10  | r1   | -    |
|34   | add    || r1   | r11  | r1   |
|35   | add    || r10  | r12  | r10  |
|36   | store  || r10  | r1   | -    |
|37   | load   || r1   | -    | r2   |
|38   | loadI  || 1036 | -    | r3   |
|39   | load   || r3   | -    | r4   |
|40   | mult   || r2   | r4   | r5   |
|41   | loadI  || 1040 | -    | r6   |
|42   | store  || r5   | r6   | -    |
|43   | loadI  || 1044 | -    | r1   |
|44   | load   || r1   | -    | r2   |
|45   | loadI  || 1048 | -    | r3   |
|46   | load   || r3   | -    | r4   |
|47   | mult   || r2   | r4   | r5   |
|48   | loadI  || 1052 | -    | r6   |
|49   | store  || r5   | r6   | -    |
|50   | loadI  || 1056 | -    | r1   |
|51   | load   || r1   | -    | r2   |
|52   | loadI  || 1060 | -    | r3   |
|53   | load   || r3   | -    | r4   |
|54   | mult   || r2   | r4   | r5   |
|55   | loadI  || 1064 | -    | r6   |
|56   | store  || r5   | r6   | -    |
|57   | loadI  || 1068 | -    | r1   |
|58   | load   || r1   | -    | r2   |
|59   | loadI  || 1072 | -    | r3   |
|60   | load   || r3   | -    | r4   |
|61   | mult   || r2   | r4   | r5   |
|62   | loadI  || 1076 | -    | r6   |
|63   | store  || r5   | r6   | -    |
|64   | output || 1040 | -    | -    |
|65   | output || 1052 | -    | -    |
|66   | output || 1064 | -    | -    |
|67   | output || 1076 | -    | -    |
