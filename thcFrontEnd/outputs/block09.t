|index| opcode ||  op1 |  op2 | dest |
|     |        ||  sr  |  sr  |  sr  |  
|0    | loadI  || 0    | -    | r0   |
|1    | loadI  || 4    | -    | r1   |
|2    | loadI  || 0    | -    | r2   |
|3    | loadI  || 1    | -    | r3   |
|4    | loadI  || 2000 | -    | r20  |
|5    | add    || r2   | r3   | r4   |
|6    | add    || r4   | r2   | r5   |
|7    | add    || r5   | r4   | r6   |
|8    | add    || r6   | r5   | r7   |
|9    | add    || r7   | r6   | r8   |
|10   | add    || r8   | r7   | r9   |
|11   | add    || r9   | r8   | r10  |
|12   | add    || r10  | r9   | r11  |
|13   | add    || r11  | r10  | r12  |
|14   | add    || r12  | r11  | r13  |
|15   | store  || r2   | r20  | -    |
|16   | add    || r20  | r1   | r21  |
|17   | store  || r4   | r21  | -    |
|18   | add    || r21  | r1   | r22  |
|19   | store  || r5   | r22  | -    |
|20   | add    || r22  | r1   | r23  |
|21   | store  || r6   | r23  | -    |
|22   | add    || r23  | r1   | r24  |
|23   | store  || r7   | r24  | -    |
|24   | add    || r24  | r1   | r25  |
|25   | store  || r8   | r25  | -    |
|26   | add    || r25  | r1   | r26  |
|27   | store  || r9   | r26  | -    |
|28   | add    || r26  | r1   | r27  |
|29   | store  || r10  | r27  | -    |
|30   | add    || r27  | r1   | r28  |
|31   | store  || r11  | r28  | -    |
|32   | add    || r28  | r1   | r29  |
|33   | store  || r12  | r29  | -    |
|34   | add    || r29  | r1   | r30  |
|35   | store  || r13  | r30  | -    |
|36   | output || 2000 | -    | -    |
|37   | output || 2004 | -    | -    |
|38   | output || 2008 | -    | -    |
|39   | output || 2012 | -    | -    |
|40   | output || 2016 | -    | -    |
|41   | output || 2020 | -    | -    |
|42   | output || 2024 | -    | -    |
|43   | output || 2028 | -    | -    |
|44   | output || 2032 | -    | -    |
|45   | output || 2036 | -    | -    |
|46   | output || 2040 | -    | -    |